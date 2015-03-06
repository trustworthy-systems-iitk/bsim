#include <string>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <limits.h>
#include "node.h"
#include "word.h"
#include "aggr.h"
#include "flat_module.h"
#include "main.h"

std::ostream& operator<<(std::ostream& out, bitslice_match_t& m)
{
    out << " module : " << m.module->get_module_name();
    out << " output : " << m.output;
    out << " polarity : " << (m.polarity ? "-" : "+");
    out << " inputs : " << *(m.cover) ;
    return out;
}

node_t::type_t node_t::get_type(lib_elem_t::type_t t)
{
    switch(t) {
        case lib_elem_t::GATE: return node_t::GATE;
        case lib_elem_t::MACRO: return node_t::MACRO;
        case lib_elem_t::LATCH: return node_t::LATCH;
        default: assert(false);
    }
}

node_t::node_t(type_t t, const std::string& n, flat_module_t* module, lib_elem_t* libelem)
  : type(t),
    name(n),
    output(false),
    lib_elem(libelem),
    level(-1),
    module(module),
    cnf_valid(false),
    latch_gate(false),
    logic_loop(false)
    
{
    aux = NULL;
    orig = NULL;
    color = 0;
    clk_trees = 0;
    rst_trees = 0;
    rst2 = 0;
    dead = false;
    support_rep = NULL;

    input_distance = SHRT_MAX;
    output_distance = SHRT_MAX;
    nearest_input = NULL;
    nearest_output = NULL;

    in_rsttree = false;
    in_clktree = false;
    is_scan_enable = false;
    sibling = NULL;
    sibling_back = NULL;
    macro = NULL;
    suppress_gate = false;
    suppress_output = false;
    suppress_input_map.resize(lib_elem != NULL ? (lib_elem->num_inputs()+1/*for macro*/) : 0, false);

    dist_to_source = std::numeric_limits<float>::infinity(); //dist to source node for shortest path algorithm
    previous = NULL;
    VISITED = false;

    assert(!(type == GATE || type == LATCH) || lib_elem != NULL);
}

void node_t::morph_latch(node_t* d, lib_elem_t* le)
{
    assert(is_latch());
    assert(inputs.size() >= 2);

    node_t* last_inp = inputs[inputs.size()-1];
    inputs.resize(2);
    inputs[0] = d;
    inputs[1] = last_inp;
    lib_elem = le;

    cnf_valid = false;
    cnf.formula.clear();
    suppress_gate = false;
    suppress_output = false;
    suppress_input_map.resize(lib_elem != NULL ? lib_elem->num_inputs() : 0, false);
    sibling = NULL;
    sibling_back = NULL;
}

node_t::~node_t()
{
    for(kcoverlist_t::iterator it = kcovers.begin(); it != kcovers.end(); it++) {
        kcover_t* kc = *it;
        delete kc;
    }
}

aggr::id_module_t* node_t::get_covering_module()
{
    assert(modules.size() <= 1);
    if(modules.size() == 0) return NULL;
    else return *modules.begin();
}

void node_t::get_fanout_types(std::vector<lib_elem_t*>& fanoutTypes)
{
    for(fanout_iterator it = fanouts_begin(); it != fanouts_end(); it++) {
        node_t* n = *it;
        lib_elem_t* l = n->get_lib_elem();
        fanoutTypes.push_back(l);
    }
}

node_t* node_t::get_input_by_name(const char* name)
{
    int i = lib_elem->get_input_index(name);
    if(i == -1) return NULL;
    else return inputs[i];
}

node_t* node_t::get_output_by_name(const char* name)
{
    int i = lib_elem->get_output_index(name);
    if(i == -1) return NULL;
    else return macro_outputs[i];
}

void node_t::dump_verilog_decl(std::ostream& out)
{
    if(type == INPUT) {
        out << "  input " << name << ";" << std::endl;
    } else {
        if(output) {
            out << "  output " << name << ";" << std::endl;
        } else {
            out << "  wire " << name << ";" << std::endl;
        }
    }
}

void node_t::dump_modules(std::ostream& out)
{
    for(moduleset_t::iterator it = modules.begin(); it != modules.end(); it++) {
        if(!(*it)->is_dominated()) {
            out << (*it)->get_name() << " ";
        }
    }
    out << std::endl;
}

void node_t::dump_masked_strings(std::ostream& out, int mask, stringlist_t& strings)
{
    bool first = true;
    for(unsigned i = 0; i != strings.size(); i++) {
        unsigned m = 1 << i;
        if((m & mask)) {
            if(first) { first = false; }
            else { out << " "; }
            out << strings[i];
        }
    }
    if(first) { out << "N/A"; }
    out << "; ";
}

void node_t::dump_verilog_comment(std::ostream& out)
{
    out << "  // ";
    out << "level: " << get_level () << "; ";
    out << "fanouts: " << num_fanouts () << "; ";
    out << "inp_dist: " << get_input_distance() << "; ";
    if(nearest_input) {
        out << "nearest_inp: " << nearest_input->get_name() << "; ";
    }
    out << "out_dist: " << get_output_distance() << "; ";
    if(nearest_output) {
        out << "nearest_out: " << nearest_output->get_name() << "; ";
    }
    if(dead) {
        out << "dead; ";
    }

    out << "clks: "; dump_masked_strings(out, get_clktrees(), options.clockTreeRoots);
    out << "resets:"; dump_masked_strings(out, get_rsttrees(), options.resetTreeRoots);
    out << std::endl;
}

void node_t::dump_verilog_defn(std::ostream& out, bool comments, bool dump_annot, const char* annotation_mod_name, const aggr::id_module_t* mod)
{
    renamer_t r(mod);

    if(type == MACRO) {
        lib_elem_t* libelem = get_lib_elem();

        bool has_input_port = false;
        bool has_output_port = false;
        if(num_inputs() == libelem->num_inputs()+1) { has_input_port = true; } 
        else if(num_macro_outputs() == libelem->num_outputs()+1) { has_output_port = true; }

        assert(!(has_input_port && has_output_port));
        assert(!suppress_gate);

        dump_annotation(out, dump_annot, annotation_mod_name);
        out << "  " << libelem->get_name() << " " << get_instance_name() << " (";
        bool first = true;
        unsigned i=0;
        for(inputlist_t::iterator it=inputs.begin(); it != inputs.end(); it++, i++) {
            assert(!is_suppress_input_map(i));
            if(first) { first = false; } else { out << ", "; }
            if(i == libelem->num_inputs()) {
                assert(has_input_port);
                out << "." << lib_elem->get_port_name(0);
            } else {
                out << "." << lib_elem->get_input_name(i);
            }
            out << "(" << r.get_renaming((*it)->name) << ")";
        }
        i=0;
        for(nodelist_t::iterator it = macro_outputs.begin(); it != macro_outputs.end(); it++, i++) {
            if(first) { first = false; } else { out << ", "; }
            if(i == libelem->num_outputs()) {
                assert(has_output_port);
                out << "." << lib_elem->get_port_name(0);
            } else {
                out << "." << lib_elem->get_output_name(i);
            }
            out << "(" << r.get_renaming((*it)->name) << ")";
        }
        out << ");" << std::endl;
    } else if(type == MACRO_OUT) {
        // ignore MACRO_OUT nodes.
    } else if(type != INPUT) {
        int numModules = 0;
        if(comments) {
            if(is_covered()) {
                out << "  // BSIM@ modules: ";
                for(moduleset_t::iterator it = modules.begin(); it != modules.end(); it++) {
                    if(!(*it)->is_dominated() && !(*it)->is_marked_bad()) {
                        out << (*it)->get_name() << " ";
                        numModules += 1;
                    } else {
                        const bool debug = false;
                        if(debug) {
                            if((*it)->is_dominated()) {
                                out << "dom:" << (*it)->get_name() << " ";
                            }
                            if((*it)->is_marked_bad()) {
                                out << "bad: " << (*it)->get_name() << " ";
                            }
                        }
                    }
                }
                out << std::endl;
            }
            for(matchlist_t::iterator it =  matches_begin(); it != matches_end(); it++) {
                out << "  // BSIM@bitslice: " << *it << std::endl;
            }

            if(numModules > 0) {
                out << "  // BSIM@ covered_modules=" << numModules << " : ";
            }
        }

        const bool verbose = false;

        if(verbose) {
            if(sibling) {
                out << "  // BSIM@sibling: " << sibling->get_name() << std::endl;
            }
            if(suppress_gate) {
                out << "  // BSIM@suppress_gate: ";
            }
        } else {
            if(suppress_gate) {
                return;
            }
        }

        dump_annotation(out, dump_annot, annotation_mod_name);
        if(strcmp(lib_elem->get_name(), "BUF") != 0) {
            out << "  ";

            // output the name of the module being instantiated.
            if(joint_module.size()) { out << joint_module; } 
            else { out << lib_elem->get_name(); }

            // the instance name.
            out << " " << instance << " (";

            unsigned i=0;
            for(inputlist_t::iterator it=inputs.begin(); it != inputs.end(); it++, i++) {
                if(!is_suppress_input_map(i)) {
                    out << "." << lib_elem->get_input_name(i);
                    out << "(" << r.get_renaming((*it)->name) << "), ";
                }
            }
            if(!is_suppress_output()) {
                out << "." << lib_elem->get_output_name(0) 
                    << "(" << r.get_renaming(name) << ")";
            }
            if(sibling) {
                sibling->dump_sibling_bindings(out, r);
            }

            out << ");" << std::endl;
        } else {
            out << "  assign " << r.get_renaming(name) << " = " 
                << r.get_renaming(inputs[0]->name) << ";" << std::endl;
        }
    }
}

unsigned node_t::num_real_fanouts() const
{
    unsigned cnt = 0;
    for(node_t::const_fanout_iterator it = fanouts_begin(); it != fanouts_end(); it++) {
        node_t* f = *it;
        if(!(f->is_gate() && !f->is_latch_gate() && (f->is_inverter() || f->is_buffer()))) {
            cnt += 1;
        }
    }
    return cnt;
}

void node_t::dump_annotation(std::ostream& out, bool dump_annotation, const char* annotation_mod_name)
{
    if(dump_annotation) {
        static const char* annotation_fmt =
            "  /*CASIO@<instance>\n"
            "     <id>%s</id>\n"
            "     <module>%s</module>\n"
            "     <creationtool>bsim</creationtool>\n"
            "     <highlevelcorrespond>%s</highlevelcorrespond>\n"
            "     <confidence>0.9</confidence>\n"
            "     </instance>\n"
            "  */";
        char buffer[1024];
        sprintf(buffer, 
            annotation_fmt, 
            get_instance_name().c_str(), 
            (joint_module.size() ? joint_module.c_str() : get_lib_elem()->get_name()), 
            annotation_mod_name);
        out << buffer << std::endl;
    }
}

void node_t::dump_sibling_bindings(std::ostream& out, renamer_t& r)
{
    unsigned idx=0;
    for(inputlist_t::iterator it = inputs.begin(); it != inputs.end(); it++, idx++) {
        if(!is_suppress_input_map(idx)) {
            out << ", ";
            out << "." << lib_elem->get_input_name(idx);
            out << "(" << r.get_renaming((*it)->name) << ")";
        }
    }
    if(!is_suppress_output()) {
        out << ", " << "." << lib_elem->get_output_name(0)
            << "(" << r.get_renaming(name) << ")";
    }

    if(sibling) {
        dump_sibling_bindings(out, r);
    }
}

void node_t::dump(std::ostream& out)
{
    if(type == INPUT) { out << ".input "; }
    else if(type == GATE) { out << ".gate "; }
    else if(type == LATCH) { out << ".latch "; }

    if(type == GATE || type == LATCH) {
        out << (output ? "primaryoutput" : "notprimaryoutput") << " ";
        out << "type:" << lib_elem->get_name() << " " << "out:";
    }

    out << name << " ";

    unsigned i=0;
    for(inputlist_t::iterator it=inputs.begin(); it != inputs.end(); it++, i++) {
        out << lib_elem->get_input_name(i) << ":" << (*it)->name << " ";
    }
    out << " fanouts: " << num_fanouts();
    out << " level: " << get_level();
    out << std::endl;
}

kcoverlist_t& node_t::get_kcovers()
{
    if(kcovers.size() > 0) return kcovers;

    kcover_t* cov = new kcover_t(this);
    if(is_input() || is_latch() || is_macro() || is_macro_out()) {
        cov->add_leaf(this);
        kcovers.push_back(cov);
    } else {
        compute_covers(cov, 0);
        assert(cov->size() == 0);
        cov->add_leaf(this);
        kcovers.push_back(cov);
    }
    return kcovers;
}

kcover_t* node_t::get_deepest()
{
    if(kcovers.size() <= 0) return NULL;

    kcover_t* deepest = NULL;
    int max_depth = 0;
    for(kcoverlist_t::iterator it = kcovers.begin(); it != kcovers.end(); it++) {
	int current_depth = (*it)->get_max_depth();
	if (current_depth > max_depth) {
		max_depth = current_depth;
		deepest = (*it);
	}
    }

    return deepest;
}

bool sort_by_depth(kcover_t* a, kcover_t* b) {
	return a->get_max_depth() < b->get_max_depth();
}

kcoverlist_t* node_t::get_n_deepest(unsigned n)
{
    assert(n > 0);

    kcoverlist_t* deepestCovers = new kcoverlist_t();
    *deepestCovers = kcovers;

    std::sort(deepestCovers->begin(), deepestCovers->end(), sort_by_depth);
	if(n > kcovers.size()) n =  kcovers.size();	
	deepestCovers->resize(n);

	return deepestCovers;
 
}
bool node_t::is_duplicate(kcover_t* kc) const
{
    kcover_t& new_cov = *kc;
    for(kcoverlist_t::const_iterator it  = kcovers.begin(); it != kcovers.end(); it++)
    {
        kcover_t* kcptr = *it;
        kcover_t& old_cov = *kcptr;
        if(old_cov == new_cov) return true;
    }
    return false;
}

bool node_t::is_covered() const
{
    for(moduleset_t::iterator it = modules.begin(); it != modules.end(); it++)
    {
        aggr::id_module_t* mod = *it;
        if(!mod->is_marked_bad() && !mod->candidate()) {
            return true;
        }
    }
    return false;
}

bool node_t::is_covered_by_seq() const
{
    for(moduleset_t::iterator it = modules.begin(); it != modules.end(); it++)
    {
        aggr::id_module_t* mod = *it;
        if(strcmp(mod->get_type(), "ram") == 0 || 
           strcmp(mod->get_type(), "shiftreg") == 0 ||
           strcmp(mod->get_type(), "counter") == 0) 
        {
            return true;
        }
    }
    return false;
}

bool node_t::is_covered_candidate() const
{
    for(moduleset_t::iterator it = modules.begin(); it != modules.end(); it++)
    {
        aggr::id_module_t* mod = *it;
        if(!mod->is_marked_bad()) {
            return true;
        }
    }
    return false;
}

void node_t::compute_covers(kcover_t* cov, unsigned pos)
{
    assert(pos <= inputs.size());

    if(pos == inputs.size()) {
        const flat_module_t* module = this->get_module();
        bool needs_prune = cov->prune(module->get_ipp(cov->size()));

        // pruning related book-keeping.
        if(needs_prune) const_cast<flat_module_t*>(module)->incr_prune_count();
        const_cast<flat_module_t*>(module)->incr_cover_count();

        if(cov->size() <= options.kcoverSize) {
            kcover_t* new_cover = new kcover_t(*cov);
            if(!is_duplicate(new_cover)) {
                kcovers.push_back(new_cover);
            } else {
                delete new_cover;
            }
        }
    } else {
        node_t* inp = k_inputs[pos];
        kcoverlist_t& inp_covers = inp->get_kcovers();
        for(kcoverlist_t::iterator it  = inp_covers.begin();
                                   it != inp_covers.end();
                                   it++)
        {
            kcover_t* new_cov = *it;
            kcover_t* merged_cov = cov->simple_merge(new_cov, this);
            if(merged_cov != NULL) {
                compute_covers(merged_cov, pos+1);
                delete merged_cov;
            }
        }
    }
}

bool node_t::update_level()
{
    int old_level = level;
    if(get_type() == LATCH || get_type() == INPUT || get_type() == MACRO) {
        level = 0;
    } else {
        int max_input_level = 0;
        for(input_iterator it =  inputs_begin();
                           it != inputs_end();
                           it++)
        {
            node_t* i = *it;
            if(i->level > max_input_level) max_input_level = i->level;
        }
        level = max_input_level + 1;
    }
    if(old_level != level) return true;
    else return false;
}

int node_t::count_modules() const
{
    return modules.size();
}


int node_t::count_undominated_modules() const
{
    using namespace aggr;
    int cnt=0;
    for(moduleset_t::iterator it = modules.begin(); it != modules.end(); it++) {
        aggr::id_module_t *m = *it;
        if(!m->is_dominated()) cnt += 1;
    }
    return cnt;
}

bool node_t::is_noninferred() const
{
    using namespace aggr;
    for(moduleset_t::iterator it = modules.begin(); it != modules.end(); it++) {
        id_module_t* m = *it;
        if(m->inferred() || m->userDefined()) return false;
    }
    return true;
}

void node_t::mark_overlapping_modules()
{
    using namespace aggr;
    int cnt = 0;
    if(modules.size() > 1) {
        for(moduleset_t::iterator it = modules.begin(); it != modules.end(); it++) {
            id_module_t* m = *it;
            if(!m->is_dominated()) cnt += 1;
            m->mark_overlapping2();
        }
    }
    if(cnt > 1) {
        for(moduleset_t::iterator it = modules.begin(); it != modules.end(); it++) {
            id_module_t* m = *it;
            m->mark_overlapping();
        }
    }
}

void node_t::merge_modules(void)
{
    for(moduleset_t::iterator it = modules.begin(); it != modules.end(); it++) {
        aggr::id_module_t* mi = *it;
        if(mi->is_dominated()) continue;
        for(moduleset_t::iterator jt = it; jt != modules.end(); jt++) {
            if(it == jt) continue;
            aggr::id_module_t* mj = *jt;
            if(mj->is_dominated()) continue;
            if(mi->contains(mj) && mi->isDominateable() && mj->isDominateable()) {
                mj->set_dominator(mi);
            } else if(mj->contains(mi) && mj->isDominateable() && mi->isDominateable()) {
                mi->set_dominator(mj);
            }
        }
    }
}

bool node_t::update_driven_latches(bool update_macros)
{
    unsigned initial_size = driven_latches.size();
    if((!is_macro() && !is_macro_out()) || update_macros) {
        for(fanout_iterator it = fanouts_begin(); it != fanouts_end(); it++) {
            node_t* i = *it;
            if(i->is_latch()) {
                driven_latches.insert(i);
            } else if(i->is_latch_gate() && i->get_si_input() == this) {
                continue;
            } else {
                const nodeset_t* s = i->get_driven_latches();
                for(nodeset_t::const_iterator jt = s->begin(); jt != s->end(); jt++) {
                    node_t* n = *jt;
                    driven_latches.insert(n);
                }
            }
        }
    }
    unsigned final_size = driven_latches.size();
    assert(final_size == initial_size || final_size > initial_size);
    return final_size != initial_size;
}

void node_t::compute_driving_latches_mp()
{
    for(nodeset_t::iterator it = driving_latches.begin(); it != driving_latches.end(); it++)
    {
        node_t* dl = *it;
        int cnt = 0;
        for(input_iterator inpit = inputs_begin(); inpit != inputs_end(); inpit++) {
            node_t* inp = *inpit;
            if(inp->is_latch()) {
                if(dl == inp) {
                    cnt += 1;
                }
            } else {
                if(inp->is_driving_latch_sp(dl)) {
                    cnt += 1;
                } else if(inp->is_driving_latch(dl)) {
                    // above means it's a driving latch but not an SPDL.
                    cnt += 2;
                    break;
                }
            }
        }
        assert(cnt > 0);
        if(cnt == 1) {
            driving_latches_sp.insert(dl);
        }
    }

#ifdef DEBUG_DRIVING_LATCH_SP
    const nodeset_t& cone = is_latch() ? get_input(0)->get_fanin_cone() : get_fanin_cone();
    nodeset_t driving_latches_sp2;
    for(nodeset_t::iterator it = driving_latches.begin(); it != driving_latches.end(); it++)
    {
        bool mps = false;
        node_t* lat = *it;
       
        for(nodeset_t::const_iterator jt = cone.begin(); jt != cone.end(); jt++) {
            node_t* n = *jt;
            int cnt = 0;
            for(input_iterator inpit = n->inputs_begin(); inpit != n->inputs_end(); inpit++) {
                node_t* inp = *inpit;
                if(inp->is_latch()) {
                    if(inp == lat) cnt += 1;
                } else {
                    if(inp->is_driving_latch(lat)) cnt += 1;
                }
            }

            if(cnt > 1) {
                mps = true;
                break;
            }
        }
        if(!mps) {
            driving_latches_sp2.insert(lat);
        }
    }
    if(driving_latches_sp2.size() != driving_latches_sp.size()) {
        std::cout << "drv: " << driving_latches << std::endl;
        dump(std::cout);

        std::cout << "org: " << driving_latches_sp2 << std::endl;
        std::cout << "new: " << driving_latches_sp << std::endl;
    }
    assert(driving_latches_sp2.size() == driving_latches_sp.size());
    nodeset_t::iterator it = driving_latches_sp.begin();
    nodeset_t::iterator jt = driving_latches_sp2.begin();
    for(; it != driving_latches_sp.end() && jt != driving_latches_sp2.end(); it++, jt++) {
        node_t* a = *it;
        node_t* b = *jt;
        assert(a == b);
    }
#endif
}

bool node_t::update_driving_latches(bool update_macros)
{
    unsigned init_size = driving_latches.size();
    if((!is_macro() && !is_macro_out()) || update_macros) {
        const bool latch_gate = is_latch_gate();
        node_t* si = latch_gate ? get_si_input() : NULL;
        for(input_iterator it = inputs_begin(); it != inputs_end(); it++) {
            node_t *i = *it;
            if(i == si) { continue; }
            if(i->is_latch()) {
                driving_latches.insert(i);
            } else {
                const nodeset_t* s = i->get_driving_latches();
                for(nodeset_t::const_iterator jt = s->begin(); jt != s->end(); jt++) {
                    node_t* n = *jt;
                    driving_latches.insert(n);
                }
            }
        }
    }
    unsigned final_size = driving_latches.size();
    assert(final_size >= init_size);
    return final_size != init_size;
}


bool node_t::rewrite_inputs(node_t* old, node_t* newt)
{
    bool changed = false;
    for(unsigned i=0; i != inputs.size(); i++) {
        if(inputs[i] == old) {
            inputs[i] = newt;
            changed = true;
        }
    }
    return changed;
}

const nodeset_t& node_t::get_fanin_cone()
{
    if(fanin_cone.size() == 0)  {
        if( !(is_latch() || is_input() || is_macro() || is_macro_out()) ) {
            compute_fanin_cone(this);
        }
    }
    return fanin_cone;
}

const nodeset_t& node_t::get_fanin_cone_inputs()
{
    if(fanin_cone.size() == 0) {
        if( !(is_latch() || is_input() || is_macro() || is_macro_out()) ) {
            compute_fanin_cone(this);
        }
    }
    return fanin_cone_inputs;
}

// FIXME: don't revist nodes.
void node_t::compute_fanin_cone(node_t* n)
{
    // already visited?
    if(fanin_cone.find(n) != fanin_cone.end()) return;
    // latch/input?
    if(n->is_latch() || n->is_input() || n->is_macro() || n->is_macro_out()) {
        fanin_cone_inputs.insert(n);
        return;
    }
    // add this node.
    fanin_cone.insert(n);
    // go to its inputs.
    for(input_iterator it = n->inputs_begin(); it != n->inputs_end(); it++) {
        node_t *i = *it;
        compute_fanin_cone(i);
    }
}

sat_n::cnf_t& node_t::get_cnf()
{
    if(!cnf_valid) {
        compute_cnf();
    }
    return cnf;
}

void node_t::dump_gate_cnf(std::ostream& out, std::map<node_t*, int>& mapping)
{
    using namespace sat_n;

    cnf_t& cnf = get_cnf();
    if(cnf.constant) {
        out << "const " << cnf.const_value << std::endl;
        return;
    }
    for(formula_t::iterator it = cnf.formula.begin(); it != cnf.formula.end(); it++) {
        clause_t& c = *it;
        out << "clause " << c.size() << " ";
        for(clause_t::iterator it = c.begin(); it != c.end(); it++) {
            node_t* n = it->node;
            assert(mapping.find(n) != mapping.end());
            if(it->polarity == ZERO) {
                out << "-";
            }
            out << mapping[n] << " ";
        }
        out << std::endl;
    }
}

int node_t::count_clauses(const nodeset_t& cone)
{
    int cnt = 0;
    for(nodeset_t::const_iterator it = cone.begin(); it != cone.end(); it++) {
        node_t* n = *it;
        sat_n::cnf_t& cnf = n->get_cnf();
        cnt += (cnf.constant ? 1 : cnf.formula.size());
    }
    return cnt;
}

void node_t::dump_cnf(std::ostream& out)
{
    using namespace sat_n;

    const nodeset_t& inputs = get_fanin_cone_inputs();
    const nodeset_t& cone = get_fanin_cone();

    out << inputs.size() << " " << count_clauses(cone) << std::endl;

    std::map<node_t*, int> mapping;

    int next=1;
    out << "inputs ";
    for(nodeset_t::const_iterator it = inputs.begin(); it != inputs.end(); it++) {
        node_t* n = *it;
        if(mapping.find(n) == mapping.end()) {
            mapping[n] = next++;
        }
        out << mapping[n] << " ";
    }
    out << std::endl;

    for(nodeset_t::const_iterator it = cone.begin(); it != cone.end(); it++) {
        node_t* n = *it;
        if(mapping.find(n) == mapping.end()) {
            mapping[n] = next++;
        }
    }

    assert(mapping.find(this) != mapping.end());
    out << "output " << mapping[this] << std::endl;

    for(nodeset_t::const_iterator it = cone.begin(); it != cone.end(); it++) {
        node_t* n = *it;
        n->dump_gate_cnf(out, mapping);
    }
}

void node_t::compute_cnf()
{
    assert(!cnf_valid);
    lib_elem_t* typ = get_lib_elem();

    cnf_valid = true;
    if(typ->is_constant()) {
        cnf.constant = true;
        cnf.const_value = typ->get_const_value();
        cnf.node = this;
    } else if(options.ignoreScanInputs && get_is_scan_enable()) {
        std::cout << "scan enable: " << get_name() << std::endl;
        cnf.constant = true;
        cnf.const_value = 0;
        cnf.node = this;
    } else {
        using namespace sat_n;
        using namespace simplify_n;

        const cube_list_t& onset = typ->get_onset();
        const cube_list_t& offset = typ->get_offset();

        lit_t out_lit(this, sat_n::ONE);
        lit_t out_n_lit(this, sat_n::ZERO);

        for(cube_list_t::const_iterator it = onset.begin(); it != onset.end(); it++) {
            clause_t clause;
            createClause(*it, this, out_lit, clause);
            cnf.formula.push_back(clause);
        }
        for(cube_list_t::const_iterator it = offset.begin(); it != offset.end(); it++) {
            clause_t clause;
            createClause(*it, this, out_n_lit, clause);
            cnf.formula.push_back(clause);
        }
    }
}

decl_list_t::~decl_list_t()
{
    for(map_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
        free( (void*) it->first );
    }
    for(map_t::iterator it = outputs.begin(); it != outputs.end(); it++) {
        free( (void*) it->first );
    }
    for(map_t::iterator it = wires.begin(); it != wires.end(); it++) {
        free( (void*) it->first );
    }
}

void decl_list_t::dump_verilog_wire(
    std::ostream& out, 
    const char* type, 
    const char* wire, 
    int min0, 
    int max0, 
    int word_index
)
{
    if(min0 == 0 && max0 == 0) {
        out << "  " << type << " " << wire << ";";
    } else {
        out << "  " << type << " [" << max0 << ":" << min0 << "] " << wire << ";";
    }

    if(word_index != -1) {
        out << " // word: " << word_index;
    }
    out << std::endl;
}

void decl_list_t::dump_verilog(std::ostream& out)
{
    // dump input and output names.
    out << std::endl << "(" << std::endl;
    int last = inputs.size() + outputs.size();
    int pos = 0;
    for(map_t::iterator it = inputs.begin(); it != inputs.end(); it++, pos++) {
        out << "  " << it->first;
        if(pos+1 != last) out << ", ";
        out << std::endl;
    }
    for(map_t::iterator it = outputs.begin(); it != outputs.end(); it++, pos++) {
        out << "  " << it->first;
        if(pos+1 != last) out << ",";
        out << std::endl;
    }
    out << ");" << std::endl;

    for(map_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
        dump_verilog_wire(out, "input", it->first, it->second.min0, it->second.max0, it->second.word_index);
    }
    for(map_t::iterator it = outputs.begin(); it != outputs.end(); it++) {
        dump_verilog_wire(out, "output", it->first, it->second.min0, it->second.max0, it->second.word_index);
    }
    for(map_t::iterator it = wires.begin(); it != wires.end(); it++) {
        dump_verilog_wire(out, "wire", it->first, it->second.min0, it->second.max0, it->second.word_index);
    }
}

void decl_list_t::dump_instantiation(std::ostream& out, const char* moduleName, const char* instanceName, const aggr::id_module_t* mod)
{
    assert(mod);

    out << "  " << moduleName << " " << instanceName << "(";

    int last = inputs.size() + outputs.size();
    int pos = 0;
    for(map_t::iterator it = inputs.begin(); it != inputs.end(); it++, pos++) {
        const char* name = it->first;
        out << "." << name << "(" << mod->get_rev_renaming(name) << ")";
        if(pos+1 != last) out << ", ";
    }
    for(map_t::iterator it = outputs.begin(); it != outputs.end(); it++, pos++) {
        const char* name = it->first;
        out << "." << name << "(" << mod->get_rev_renaming(name) << ")";
        if(pos+1 != last) out << ", ";
    }
    out << ");" << std::endl;
}


void decl_list_t::add_item(decl_list_t::map_t& map, const char* name_in, int word_index)
{
    char* name = strdup(name_in);
    char* p = strchr(name, '[');
    if(p == NULL) {
        if(map.find(name) == map.end()) {
            index_t i = { 0, 0, word_index };
            map[name] = i;
        } else {
            free(name);
        }
    } else {
        *p = '\0';
        p = p + 1;

        index_t i = { INT_MAX, INT_MIN, word_index };
        map_t::iterator it = map.find(name);
        if(it != map.end()) {
            i = it->second;
        }

        char* q = strchr(p, ']');
        assert(q != NULL);
        *q = '\0';
        q = q + 1;
        int index = atoi(p);
        if(index < i.min0) i.min0 = index;
        if(index > i.max0) i.max0 = index;

        if(strchr(q, '[') == NULL) {
            map[name] = i;
        } else {
            fprintf(stderr, "Error: multi-dimensional arrays aren't handled yet.\n");
            exit(1);
        }
        if(it != map.end()) {
            free(name);
        }
    }
}

bool decl_list_t::is_item(const decl_list_t::map_t& map, const char* name_in) const
{
    char* name = strdup(name_in);
    char* p = strchr(name, '[');
    bool result = false;
    if(p == NULL) {
        result = map.find(name) != map.end();
    } else {
        *p = '\0';
        result = map.find(name) != map.end();
    }
    free(name);
    return result;
}

bool decl_list_t::is_input(const char* name) const
{
    return is_item(inputs, name);
}

void decl_list_t::add_input(const char* name, int word_index)
{
    add_item(inputs, name, word_index);
}

bool decl_list_t::is_output(const char* name) const
{
    return is_item(outputs, name);
}

void decl_list_t::add_output(const char* name, int word_index)
{
    add_item(outputs, name, word_index);
}

bool decl_list_t::is_wire(const char* name) const
{
    return is_item(wires, name);
}

void decl_list_t::add_wire(const char* name)
{
    add_item(wires, name, -1);
}

std::ostream& operator<<(std::ostream& out, const nodeset_t& s)
{
    out << "{ ";
    for(nodeset_t::const_iterator it = s.begin(); it != s.end(); it++) {
        node_t* n = *it;
        out << n->get_name() << " ";
    }
    out << "}";
    return out;
}

std::ostream& operator<<(std::ostream& out, const nodelist_t& b)
{
    out << "[ ";
    for(unsigned i=0; i != b.size(); i++) {
        out << b[i]->get_name() << " ";
    }
    out << "]";
    return out;
}

