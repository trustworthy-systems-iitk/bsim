#include "library.h"
#include "common.h"
#include "sat.h"
#include "node.h"
#include "flat_module.h"
#include "main.h"

#include <stdarg.h>
#include <string>

lib_elem_t::cover_memo_t lib_elem_t::cover_memo;
lib_elem_t::symm_inputs_memo_t lib_elem_t::symm_inputs_memo;
lib_elem_t::is_buffer_flag_t lib_elem_t::is_buffer_flag;
lib_elem_t::is_inverter_flag_t lib_elem_t::is_inverter_flag;
lib_elem_t::is_nor2xb_flag_t lib_elem_t::is_nor2xb_flag;

lib_elem_t::lib_elem_t(const char* n, eval_fun_t e, type_t type)
    : name(n),
      eval_fn(e),
      type(type)
{
    d_index = -1;
    si_index = -1;
    se_index = -1;
}

lib_elem_t::~lib_elem_t()
{ 
    free((void*)name); 
}

void lib_elem_t::register_input(const char* i, int index)
{
    assert(inputIdx.find(index) == inputIdx.end());
    inputIdx.insert(index);
    inputs[(i)] = index;
}

void lib_elem_t::register_output(const char* o, int index)
{
    assert(outputIdx.find(index) == outputIdx.end());
    outputIdx.insert(index);
    outputs[(o)] = index;
}

void lib_elem_t::register_port(const char* p, int index)
{
    assert(portIdx.find(index) == portIdx.end());
    portIdx.insert(index);
    ports[p] = index;
}

bool lib_elem_t::is_input(const char* s)
{
    return(inputs.find(s) != inputs.end());
}

int lib_elem_t::get_input_index(const char* s)
{
    if(inputs.find(s) != inputs.end()) return inputs[s];
    else return -1;
}

int lib_elem_t::get_output_index(const char* s)
{
    if(outputs.find(s) != outputs.end()) return outputs[s];
    else return -1;
}

int lib_elem_t::get_port_index(const char* s)
{
    if(ports.find(s) != ports.end()) return ports[s];
    else return -1;
}

const char* lib_elem_t::get_input_name(int i)
{
    for(string_map_t::iterator it  = inputs.begin();
                               it != inputs.end();
                               it++)
    {
        if(it->second == i) return it->first;
    }
    return NULL;
}

const char* lib_elem_t::get_output_name(int i)
{
    for(string_map_t::iterator it  = outputs.begin();
                               it != outputs.end();
                               it++)
    {
        if(it->second == i) return it->first;
    }
    return NULL;
}

const char* lib_elem_t::get_port_name(int i)
{
    for(string_map_t::iterator it  = ports.begin();
                               it != ports.end();
                               it++)
    {
        if(it->second == i) return it->first;
    }
    return NULL;
}

bool lib_elem_t::has_input(const char* n) const
{
    return (inputs.find(n) != inputs.end());
}

bool lib_elem_t::has_output(const char* n) const
{
    return (outputs.find(n) != outputs.end());
}

bool lib_elem_t::is_output(const char* s)
{
    return(outputs.find(s) != outputs.end());
}

bool lib_elem_t::is_port(const char* p)
{
    return (ports.find(p) != ports.end());
}

lib_elem_t* lib_elem_t::create_block_ex(const char* name, int n_inputs, int n_outputs, int n_ports, ...)
{
    lib_elem_t* elem = new lib_elem_t(name, NULL, MACRO);

    va_list ap;
    va_start(ap, n_ports);
    const char* p;
    for(int i = 0; i < n_inputs; i++) {
        p = va_arg(ap, const char*);
        elem->register_input(p, i);
    }
    for(int i = 0; i < n_outputs; i++) {
        p = va_arg(ap, const char*);
        elem->register_output(p, i);
    }
    for(int i = 0; i < n_ports; i++) {
        p = va_arg(ap, const char*);
        elem->register_port(p, i);
    }
    va_end(ap);
    return elem;
}

lib_elem_t* lib_elem_t::create_lib_elem(const char* name, bool seq, int n_inputs, int n_outputs, eval_fun_t e, ...)
{
    lib_elem_t* elem = new lib_elem_t(name, e, seq ? LATCH : GATE);

    va_list ap;
    va_start(ap, e);
    const char* p;
    for(int i = 0; i < n_inputs; i++) {
        p = va_arg(ap, const char*);
        elem->register_input(p, i);
    }
    for(int i = 0; i < n_outputs; i++) {
        p = va_arg(ap, const char*);
        elem->register_output(p, i);
    }
    va_end(ap);
    if(n_outputs) {
        elem->compute_input_symmetry();
        elem->compute_covers();
        elem->is_buffer_compute();
        elem->is_inverter_compute();
        elem->is_nor2xb_compute();
        // elem->verify_satisfy_count();
    }
    return elem;
}

void lib_elem_t::create_seq_elem(
    ILibrary::map_t& blocks, 
    const char* name, 
    bool seq, 
    int n_inputs, 
    int n_outputs, 
    eval_fun_t e, 
    ...
)
{
    assert(seq);

    lib_elem_t* elem = new lib_elem_t(strdup(name), e, LATCH);

    va_list ap;
    va_start(ap, e);
    const char* p;
    for(int i = 0; i < n_inputs; i++) {
        p = va_arg(ap, const char*);
        elem->register_input(p, i);
    }
    for(int i = 0; i < n_outputs; i++) {
        p = va_arg(ap, const char*);
        elem->register_output(p, i);
    }
    va_end(ap);
    elem->compute_input_symmetry();
    elem->compute_covers();
    elem->is_buffer_compute();


    std::string gate_name = std::string(name) + ".GATE";
    lib_elem_t* gate = new lib_elem_t(strdup(gate_name.c_str()), e, GATE);

    va_list ap2;
    va_start(ap2, e);

    int d_index = -1;
    int si_index = -1;
    int se_index = -1;
    assert(n_inputs > 1);
    for(int i = 0; i < n_inputs-1; i++) {
        p = va_arg(ap2, const char*);
        gate->register_input(p, i);
        if(strcmp(p, "D") == 0) {
            d_index = i;
        }
        if(strcmp(p, "SI") == 0) {
            si_index = i;
        }
        if(strcmp(p, "SE") == 0) {
            se_index = i;
        }
    }
    gate->set_dindex(d_index);
    gate->set_si_index(si_index);
    gate->set_se_index(se_index);
    gate->register_output("Y", 0);
    va_end(ap2);

    gate->compute_input_symmetry();
    gate->compute_covers();
    gate->is_buffer_compute();
    gate->is_inverter_compute();
    gate->is_nor2xb_compute();
    // gate->verify_satisfy_count();

    blocks[strdup(name)] = elem;
    blocks[strdup(gate_name.c_str())] = gate;
}

void lib_elem_t::compute_covers()
{
    if(!is_gate()) return;

    if(cover_memo.find(eval_fn) != cover_memo.end()) {
        return;
    }

    dummy_input_provider_t dip;
    BDD fn = eval_fn(&dip);
    cover_memo[eval_fn] = cover_t();
    if(fn == dip.one()) {
        cover_memo[eval_fn].constant = true;
        cover_memo[eval_fn].const_value = 1;
    } else if(fn == dip.zero()) {
        cover_memo[eval_fn].constant = true;
        cover_memo[eval_fn].const_value = 0;
    } else {
        cover_memo[eval_fn].constant = false;
        cover_memo[eval_fn].const_value = -1;
        sat_n::computeCovers(dip.cudd, fn, cover_memo[eval_fn].onset, cover_memo[eval_fn].offset);
    }
}

void lib_elem_t::verify_satisfy_count()
{
    using namespace sat_n; // for incrementArray.

    if(!is_gate()) return;

    std::cout << "---------------------------------" << std::endl;
    std::cout << "VERIFYING: " << name << std::endl;
    std::cout << "---------------------------------" << std::endl;

    dummy_input_provider_t dip;

    Cudd& mgr = dip.cudd;
    BDD fn = eval_fn(&dip);
    int numVars = fn.SupportSize();
    int *support = new int[numVars];
    BDD b1 = mgr.bddOne();
    BDD b0 = mgr.bddZero();

    std::fill(support, support + numVars, 0);
    bool done = false;
    double satCnt = 0;
    do {
        BDD r = fn.Eval(support);
        if(r == b1) {
            satCnt += 1;
        }
        done = !incrementArray(support, numVars);
    } while(!done);
    double fnSatCnt = fn.SatisfyCount(fn.SupportSize());
    if(satCnt != fnSatCnt) {
        std::cout << std::endl << "ERROR: satisfy count failed. simple: " << satCnt << "; bdd: " << fnSatCnt << std::endl;
        printBDD(stdout, fn);
    }
    assert(satCnt == fnSatCnt);
}

const lib_elem_t::cover_t& lib_elem_t::get_cover() const
{
    assert(is_gate());

    cover_memo_t::iterator pos = cover_memo.find(eval_fn);
    assert(pos != cover_memo.end());

    return pos->second;
}

void lib_elem_t::compute_input_symmetry()
{
    if(!is_gate()) return;

    if(symm_inputs_memo.find(eval_fn) != symm_inputs_memo.end()) {
        intset_t* s = symm_inputs_memo[eval_fn];
        for(intset_t::iterator it = s->begin(); it != s->end(); it++) {
            int i = *it;
            symm_inputs.insert(i);
        }
        return;
    }

    int n = inputs.size();
    dummy_input_provider_t dip;
    BDD fn = eval_fn(&dip);

    for(int i = 0; i != n; i++) {
        if(symm_inputs.find(i) != symm_inputs.end()) continue;
        for(int j = 0; j < i; j++) {
            if(symm_inputs.find(j) != symm_inputs.end()) continue;
            BDD f1 = fn.Cofactor(!dip.inp(i) & dip.inp(j));
            BDD f2 = fn.Cofactor(dip.inp(i) & !dip.inp(j));
            if(f1 == f2) {
                symm_inputs.insert(i);
                symm_inputs.insert(j);
            }
        }
    }
    // memoize
    symm_inputs_memo[eval_fn] = &symm_inputs;

    const bool debug = false;
    if(debug) {
        if(symm_inputs.size() > 0) {
            std::cout << name << " has symmetric inputs : ";
            for(intset_t::iterator it = symm_inputs.begin(); it != symm_inputs.end(); it++) {
                std::cout << *it << " ";
            }
            std::cout << std::endl;
        }
    }
}

void lib_elem_t::is_buffer_compute()
{
    if(!is_gate()) return;

    is_buffer_flag_t::iterator pos = is_buffer_flag.find(eval_fn);
    if(pos != is_buffer_flag.end()) return;

    dummy_input_provider_t dip;
    BDD y = eval_fn(&dip);
    if(y == dip.inp(0)) {
        is_buffer_flag[eval_fn] = true;
    } else {
        is_buffer_flag[eval_fn] = false;
    }
}

void lib_elem_t::is_inverter_compute()
{
    if(!is_gate()) return;

    is_inverter_flag_t::iterator pos = is_inverter_flag.find(eval_fn);
    if(pos != is_inverter_flag.end()) return;

    dummy_input_provider_t dip;
    BDD y = eval_fn(&dip);
    if(y == !dip.inp(0)) {
        is_inverter_flag[eval_fn] = true;
    } else {
        is_inverter_flag[eval_fn] = false;
    }
}

void lib_elem_t::is_nor2xb_compute()
{
    if(options.removeNOR2XB.size() == 0) return;
    if(!is_gate()) return;

    is_nor2xb_flag_t::iterator pos = is_nor2xb_flag.find(eval_fn);
    if(pos != is_nor2xb_flag.end()) return;

    dummy_input_provider_t dip;
    BDD y = eval_fn(&dip);
    if(y == !(dip.inp(0) + !dip.inp(1))) {
        is_nor2xb_flag[eval_fn] = true;
    } else {
        is_nor2xb_flag[eval_fn] = false;
    }
}

lib_elem_t* ILibrary::get_elem(const char* str)
{
    map_t::iterator it = blocks.find(str);
    if(it == blocks.end()) return NULL;
    else return it->second;
}

lib_elem_t* ILibrary:: get_replacement(lib_elem_t* ff)
{
    assert(ff->is_seq());
    return get_elem("BSIM_DFF");
}

void lib_elem_t::getMissingInputs(
    module_inst_t* mi, 
    std::vector<std::string>& missing_inputs
)
{
#if 0
    bindinglist_t& bindings = *(mi->bindings);
    std::set< std::string > bindings;
    for(unsigned i=0; i != bindings.size(); i++) {
    }
#endif
}


namespace lib_12soi_n {

    struct lib_12soi_t : public ILibrary {
        virtual void init();
        virtual void preprocess(module_list_t& modules);
        virtual void rewrite_node(node_t* n, flat_module_t* mod);
        virtual ~lib_12soi_t();
        void genGateTypes(std::vector<std::string>& gateTypes);

        virtual lib_elem_t* get_replacement(lib_elem_t* ff);
        void process_module_inst(module_t* mod, module_inst_t* mi);
        void preprocess_module(module_t* mod);
        void process_adder(std::string& type, module_t* mod, module_inst_t* mi);
    };
    typedef input_provider_t* ipp;

    lib_elem_t* lib_12soi_t:: get_replacement(lib_elem_t* ff)
    {
        assert(ff->is_seq());
        if(ff->has_input("CK")) {
            if(ff->has_output("QN")) {
                return get_elem("BSIM_DFFQN");
            } else {
                return get_elem("BSIM_DFF");
            }
        } else if(ff->has_input("CKN")) {
            if(ff->has_output("QN")) {
                return get_elem("BSIM_DFFNQN");
            } else {
                return get_elem("BSIM_DFFN");
            }
        } else if(ff->has_input("G")) {
            if(ff->has_output("QN")) {
                return get_elem("BSIM_LATQN");
            } else {
                return get_elem("BSIM_LAT");
            }
        } else if(ff->has_input("GN")) {
            if(ff->has_output("QN")) {
                return get_elem("BSIM_LATNQN");
            } else {
                return get_elem("BSIM_LATN");
            }
        } else {
            assert(false);
            return NULL;
        }
    }

    // and
    BDD and2(ipp e) { return e->inp(0) & e->inp(1); }
    BDD and3(ipp e) { return e->inp(0) & e->inp(1) & e->inp(2); }
    BDD and4(ipp e) { return e->inp(0) & e->inp(1) & e->inp(2) & e->inp(3); }
    BDD invalid(ipp e) { assert(false); return e->inp(0); }

    // nand
    BDD nand2b(ipp e) { return !((!e->inp(0)) & e->inp(1)); }
    BDD nand2xb(ipp e) { return !((e->inp(0)) & (!e->inp(1))); }
    BDD nand2(ipp e) { return !(e->inp(0) & e->inp(1)); }
    BDD nand3b(ipp e) { return !((!e->inp(0)) & e->inp(1) & e->inp(2)); }
    BDD nand3xxb(ipp e) { return !(e->inp(0) & e->inp(1) & (!e->inp(2))); }
    BDD nand3(ipp e) { return !(e->inp(0) & e->inp(1) & e->inp(2)); }
    BDD nand4b(ipp e) { return !((!e->inp(0)) & e->inp(1) & e->inp(2) & e->inp(3)); }
    BDD nand4xxxb(ipp e) { return !(e->inp(0) & e->inp(1) & e->inp(2) & (!e->inp(3))); }
    BDD nand4(ipp e) { return !(e->inp(0) & e->inp(1) & e->inp(2) & e->inp(3)); }

    // nor
    BDD nor2b(ipp e) { return !((!e->inp(0)) | e->inp(1)); }
    BDD nor2xb(ipp e) { return !((e->inp(0)) | (!e->inp(1))); }
    BDD nor2(ipp e) { return !(e->inp(0) | e->inp(1)); }
    BDD nor3(ipp e) { return !(e->inp(0) | e->inp(1) | e->inp(2)); }

    // xor
    BDD xor2(ipp e) { return e->inp(0) ^ e->inp(1); }
    BDD xor3(ipp e) { return e->inp(0) ^ e->inp(1) ^ e->inp(2); }

    // xnor
    BDD xnor2(ipp e) { return !(e->inp(0) ^ e->inp(1)); }
    BDD xnor3(ipp e) { return !(e->inp(0) ^ e->inp(1) ^ e->inp(2)); }

    // or
    BDD or2(ipp e) { return e->inp(0) | e->inp(1); }
    BDD or3(ipp e) { return e->inp(0) | e->inp(1) | e->inp(2); }
    BDD or4(ipp e) { return e->inp(0) | e->inp(1) | e->inp(2) | e->inp(3); }
    BDD or6(ipp e) { return e->inp(0) | e->inp(1) | e->inp(2) | e->inp(3) | e->inp(4) | e->inp(5); }

    // not
    BDD inv(ipp e) { return !e->inp(0); }

    // constants
    BDD one(ipp e) { return e->one(); }
    BDD zero(ipp e) { return e->zero(); }

    // buf
    BDD buf(ipp e) { return e->inp(0); }

    // and-or
    BDD ao1b2(ipp e) { return !e->inp(0) | (e->inp(1) & e->inp(2)); }
    BDD ao21(ipp e) { return (e->inp(0) & e->inp(1)) | e->inp(2); }
    BDD ao21b(ipp e) { return (e->inp(0) & e->inp(1)) | !e->inp(2); }
    BDD ao21a1ai2(ipp e) { return !(((e->inp(0) & e->inp(1)) | e->inp(2)) & e->inp(3)); }
    BDD aoi21(ipp e) { return !((e->inp(0) & e->inp(1)) | e->inp(2)); }
    BDD aoi21b(ipp e) { return !((e->inp(0) & e->inp(1)) | !e->inp(2));   }
    BDD aoi31(ipp e) { return !((e->inp(0) & e->inp(1) & e->inp(2)) | e->inp(3)); }
    BDD aoi32(ipp e) { return !((e->inp(0) & e->inp(1) & e->inp(2)) | (e->inp(3) & e->inp(4))); }
    BDD aoi211(ipp e) { return !((e->inp(0) & e->inp(1)) | e->inp(2) | e->inp(3)); }
    BDD ao22(ipp e) { return (e->inp(0) & e->inp(1)) | (e->inp(2) & e->inp(3)); }
    BDD aoi22(ipp e) { return !((e->inp(0) & e->inp(1)) | (e->inp(2) & e->inp(3))); }
    BDD aoi221(ipp e) { return !((e->inp(0) & e->inp(1)) | (e->inp(2) & e->inp(3)) | e->inp(4)); }
    BDD aoi222(ipp e) { return !((e->inp(0) & e->inp(1)) | (e->inp(2) & e->inp(3)) | (e->inp(4) & e->inp(5))); }
    BDD aoi2xb1(ipp e) { return !((e->inp(0) & (!e->inp(1))) | e->inp(2)); }

    // carry gen
    BDD cgen(ipp e) { 
        BDD a = e->inp(0);
        BDD b = e->inp(1);
        BDD c = e->inp(2);
        return (a&b) | (b&c) | (c&a); 
    }
    // carry gen inverted
    BDD cgeni(ipp e) { 
        BDD a = e->inp(0);
        BDD b = e->inp(1);
        BDD c = e->inp(2);
        return !((a&b) | (b&c) | (c&a)); 
    }
    // carry gen (for full adder, low carry-in)
    BDD cgencin(ipp e) {
        BDD a = e->inp(0);
        BDD b = e->inp(1);
        BDD c = e->inp(2);
        return (a&b) | (a&!c) | (b&!c);
    }


    // muxes.
    BDD mux2(ipp e) { 
        BDD s = e->inp(2);
        BDD a = e->inp(0);
        BDD b = e->inp(1);
        return ((!s & a) | (s & b));
    }
    BDD mux2i(ipp e) { return !mux2(e); }
    BDD mux4(ipp e) {
        BDD a = e->inp(0);
        BDD b = e->inp(1);
        BDD c = e->inp(2);
        BDD d = e->inp(3);

        BDD s0 = e->inp(4);
        BDD s1 = e->inp(5);

        return (!s1 & !s0 & a) |
               (!s1 &  s0 & b) |
               (s1  & !s0 & c) |
               (s1  &  s0 & d);
    }
    BDD mux4i(ipp e) {
        return !mux4(e);
    }

    // or-and
    BDD oa211(ipp e) { return (e->inp(0) | e->inp(1)) & e->inp(2) & e->inp(3); }
    BDD oa21a1oi2(ipp e) { return !(((e->inp(0) | e->inp(1)) & e->inp(2)) | e->inp(3)); }
    BDD oa21(ipp e) { return (e->inp(0) | e->inp(1)) & e->inp(2); }
    BDD oa22(ipp e) { return (e->inp(0) | e->inp(1)) & (e->inp(2) | e->inp(3)); }
    BDD oai21(ipp e) { return !((e->inp(0) | e->inp(1)) & e->inp(2)); }
    BDD oai21b(ipp e) { return !((e->inp(0) | e->inp(1)) & !e->inp(2)); }
    BDD oai2xb1(ipp e) { return !((e->inp(0) | (!e->inp(1))) & e->inp(2)); }
    BDD oai31(ipp e) { return !((e->inp(0) | e->inp(1) | e->inp(2)) & e->inp(3)); }
    BDD oai211(ipp e) { return !((e->inp(0) | e->inp(1)) & e->inp(2) & e->inp(3)); }
    BDD oai221(ipp e) { return !((e->inp(0) | e->inp(1)) & (e->inp(2) | e->inp(3)) & (e->inp(4))); }
    BDD oai222(ipp e) { return !((e->inp(0) | e->inp(1)) & (e->inp(2) | e->inp(3)) & (e->inp(4) | e->inp(5))); }
    BDD oai22(ipp e) { return !((e->inp(0) | e->inp(1)) & (e->inp(2) | e->inp(3))); }

    // sequential elements.
    BDD a2sdffq(ipp e) {
        BDD a = e->inp(0);
        BDD b = e->inp(1);
        BDD si = e->inp(2);
        BDD se = e->inp(3);
        return (se & si) + (!se & (a&b));
    }

    BDD a2sdffqn(ipp e) {
        BDD a = e->inp(0);
        BDD b = e->inp(1);
        BDD si = e->inp(2);
        BDD se = e->inp(3);
        return !((se & si) + (!se & (a&b)));
    }

    BDD m2sdffq(ipp e) {
        BDD se = e->inp(0);
        BDD si = e->inp(1);

        BDD s0 = e->inp(2);
        BDD d0 = e->inp(3);
        BDD d1 = e->inp(4);

        return ((se&si) + (!se & ((s0&d1) + (!s0&d0))));
    }

    BDD m2sdffqn(ipp e) {
        BDD se = e->inp(0);
        BDD si = e->inp(1);

        BDD s0 = e->inp(2);
        BDD d0 = e->inp(3);
        BDD d1 = e->inp(4);

        return !((se & si) + (!se & ( (s0&d1) + (!s0&d0) )));
    }

    BDD dffrpq(ipp e) {
        BDD r = e->inp(0);
        BDD d = e->inp(1);
        return !r&d;
    }

    BDD dffrpqn(ipp e) {
        BDD r = e->inp(0);
        BDD d = e->inp(1);
        return !(!r&d);
    }

    BDD dffsq(ipp e) {
        BDD sn = e->inp(0);
        BDD d = e->inp(1);
        return !sn + d;
    }

    BDD dffsqn(ipp e) {
        BDD sn = e->inp(0);
        BDD d = e->inp(1);
        return !(!sn + d);
    }

    BDD dffsrpq(ipp e) {
        BDD r = e->inp(0);
        BDD sn = e->inp(1);
        BDD d = e->inp(2);
        return (!sn + (!r&d));
    }

    BDD sdffq(ipp e) {
        BDD d = e->inp(0);
        BDD si = e->inp(1);
        BDD se = e->inp(2);

        return (se&si) + (!se&d);
    }

    BDD sdffqn(ipp e) {
        BDD d = e->inp(0);
        BDD si = e->inp(1);
        BDD se = e->inp(2);

        return !((se&si) + (!se&d));
    }

    BDD sdffrpq(ipp e) {
        BDD r = e->inp(0);
        BDD d = e->inp(1);
        BDD si = e->inp(2);
        BDD se = e->inp(3); 

        // r = 0 kills D.
        return (!r & ((se&si) + (!se&d)));
    }

    BDD sdffrpqn(ipp e) {
        BDD r = e->inp(0);
        BDD d = e->inp(1);
        BDD si = e->inp(2);
        BDD se = e->inp(3); 

        // r = 0 kills D.
        return !(!r & ((se&si) + (!se&d)));
    }

    BDD sdffsq(ipp e) {
        BDD sn = e->inp(0);
        BDD d = e->inp(1);
        BDD si = e->inp(2);
        BDD se = e->inp(3);

        return !sn + (se&si) + (!se&d);
    }

    BDD sdffsqn(ipp e) {
        BDD sn = e->inp(0);
        BDD d = e->inp(1);
        BDD si = e->inp(2);
        BDD se = e->inp(3);

        return !(!sn + (se&si) + (!se&d));
    }

    // "SN", "R", "D", "SI", "SE", "CK", "Q"
    BDD sdffsrpq(ipp e) {
        BDD sn = e->inp(0);
        BDD r = e->inp(1);
        BDD d = e->inp(2);
        BDD si = e->inp(3);
        BDD se = e->inp(4);

        return !sn + (!r & ((se&si) + (!se&d)) );
    }

    BDD latrq(ipp e) {
        BDD rn = e->inp(0);
        BDD d = e->inp(1);
        return rn & d;
    }

    BDD latspq(ipp e) {
        BDD s = e->inp(0);
        BDD d = e->inp(1);
        return s + d;
    }

    void lib_12soi_t::genGateTypes(std::vector<std::string>& gateTypes)
    {
        std::string prefixes[] = {
            "X0P5M", "X0P6M", "X0P5A", "X0P5B", "X0P7M", "X0P7A", "X0P6B",
            "X0P7B", "X0P8M", "X0P8B", "X1P2M", "X1P2B", "X1P4M", "X1P4B",
            "X1P4A", "X1P7M", "X1P7B", "X2P5M", "X2P5B", "X3P5M", "X3P5B",
            "X7P5M", "X7P5B", "X1M", "X1A", "X1B", "X2M", "X2A", "X2B", "X2M",
            "X3A", "X3M", "X3B", "X4M", "X4A", "X4B", "X5B", "X5M", "X6M",
            "X6A", "X6B", "X6M", "X8M", "X9M", "X9B", "X11M", "X11B", "X13M",
            "X13B", "X16M", "X16B"
        };
        unsigned numPrefixes = sizeof(prefixes) / sizeof(prefixes[0]);

        std::string suffixes[] = {
            "A12TR",
            "A12TS",
            "A9TS"
        };
        unsigned numSuffixes = sizeof(suffixes) / sizeof(suffixes[0]);

        for(unsigned i=0; i != numPrefixes; i++) {
            for(unsigned j=0; j != numSuffixes; j++) {
                std::string s = prefixes[i] + "_" + suffixes[j];
                gateTypes.push_back(s);
            }
            std::string s = prefixes[i];
            gateTypes.push_back(s);
        }
    }

    void lib_12soi_t::init ()
    {
        using namespace std;

        std::cout << "Initializing IBM 12SOI library ... "; cout.flush();

        int i;
        vector<string> gateType;
        genGateTypes(gateType);
        int numGateTypes = gateType.size();

        for (i = 0; i < numGateTypes; i++)
        {
            /*AND Gates*/
            blocks[strdup(("AND2_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AND2_" + gateType[i]).c_str()), false, 2, 1, and2, "A", "B", "Y");
            blocks[strdup(("AND3_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AND3_" + gateType[i]).c_str()), false, 3, 1, and3, "A", "B", "C", "Y");
            blocks[strdup(("AND4_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AND4_" + gateType[i]).c_str()), false, 4, 1, and4, "A", "B", "C", "D", "Y");

            //AO Gates
            blocks[strdup(("AO1B2_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AO1B2_" + gateType[i]).c_str()), false, 3, 1, ao1b2, "A0N", "B0", "B1", "Y");
            blocks[strdup(("AO21A1AI2_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AO21A1AI2_" + gateType[i]).c_str()), false, 4, 1, ao21a1ai2, "A0", "A1", "B0", "C0", "Y");
            blocks[strdup(("AO21B_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AO21B_" + gateType[i]).c_str()), false, 3, 1, ao21b, "A0", "A1", "B0N", "Y");
            blocks[strdup(("AO21_" + gateType[i]).c_str())] =  lib_elem_t::create_lib_elem(strdup(("AO21_" + gateType[i]).c_str()), false, 3, 1, ao21, "A0", "A1", "B0", "Y");
            blocks[strdup(("AO22_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AO22_" + gateType[i]).c_str()), false, 4, 1, ao22, "A0", "A1", "B0", "B1", "Y");
            blocks[strdup(("AOI211_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AOI211_" + gateType[i]).c_str()), false, 4, 1, aoi211, "A0", "A1", "B0", "C0", "Y");
            blocks[strdup(("AOI21_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AOI21_" + gateType[i]).c_str()), false, 3, 1, aoi21, "A0", "A1", "B0", "Y");
            blocks[strdup(("AOI21B_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AOI21B_" + gateType[i]).c_str()), false, 3, 1, aoi21b, "A0", "A1", "B0N", "Y");
            blocks[strdup(("AOI2XB1_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AOI2XB1_" + gateType[i]).c_str()), false, 3, 1, aoi2xb1, "A0", "A1N", "B0", "Y");
            blocks[strdup(("AOI221_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AOI221_" + gateType[i]).c_str()), false, 5, 1, aoi221, "A0", "A1", "B0", "B1", "C0", "Y");
            blocks[strdup(("AOI222_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AOI222_" + gateType[i]).c_str()), false, 6, 1, aoi222, "A0", "A1", "B0", "B1", "C0", "C1", "Y");
            blocks[strdup(("AOI22_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AOI22_" + gateType[i]).c_str()), false, 4, 1, aoi22, "A0", "A1", "B0", "B1", "Y");
            blocks[strdup(("AOI31_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AOI31_" + gateType[i]).c_str()), false, 4, 1, aoi31, "A0", "A1", "A2", "B0", "Y");
            blocks[strdup(("AOI32_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("AOI32_" + gateType[i]).c_str()), false, 5, 1, aoi32, "A0", "A1", "A2", "B0", "B1", "Y");

            //Carry Generator Gates
            blocks[strdup(("CGEN_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("CGEN_" + gateType[i]).c_str()), false, 3, 1, cgen, "A", "B", "CI", "CO");
            blocks[strdup(("CGENI_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("CGENI_" + gateType[i]).c_str()), false, 3, 1, cgeni, "A", "B", "CI", "CON");
            blocks[strdup(("CGENCIN_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("CGENCIN_" + gateType[i]).c_str()), false, 3, 1, cgencin, "A", "B", "CI", "CO");

            //Inverters
            blocks[strdup(("INV_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("INV_" + gateType[i]).c_str()), false, 1, 1, inv, "A", "Y");

            //Multiplexers
            blocks[strdup(("MX2_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("MX2_" + gateType[i]).c_str()), false, 3, 1, mux2, "A", "B", "S0", "Y");
            blocks[strdup(("MXIT2_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("MXIT2_" + gateType[i]).c_str()), false, 3, 1, mux2i, "A", "B", "S0", "Y");
            blocks[strdup(("MXT2_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("MXT2_" + gateType[i]).c_str()), false, 3, 1, mux2, "A", "B", "S0", "Y");
            blocks[strdup(("MXIT4_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("MXIT4_" + gateType[i]).c_str()), false, 6, 1, mux4i, "A", "B", "C", "D", "S0", "S1", "Y");
            blocks[strdup(("MXT4_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("MXT4_" + gateType[i]).c_str()), false, 6, 1, mux4, "A", "B", "C", "D", "S0", "S1", "Y");

            //NAND gates
            blocks[strdup(("NAND2_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("NAND2_" + gateType[i]).c_str()), false, 2, 1, nand2, "A", "B", "Y");
            blocks[strdup(("NAND2B_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("NAND2B_" + gateType[i]).c_str()), false, 2, 1, nand2b, "AN", "B", "Y");
            blocks[strdup(("NAND2XB_" + gateType[i]).c_str())] =  lib_elem_t::create_lib_elem(strdup(("NAND2XB_" + gateType[i]).c_str()), false, 2, 1, nand2xb, "A", "BN", "Y");
            blocks[strdup(("NAND3_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("NAND3_" + gateType[i]).c_str()), false, 3, 1, nand3, "A", "B", "C", "Y");
            blocks[strdup(("NAND3B_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("NAND3B_" + gateType[i]).c_str()), false, 3, 1, nand3b, "AN", "B", "C", "Y");
            blocks[strdup(("NAND3XXB_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("NAND3XXB_" + gateType[i]).c_str()), false, 3, 1, nand3xxb, "A", "B", "CN", "Y");
            blocks[strdup(("NAND4_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("NAND4_" + gateType[i]).c_str()), false, 4, 1, nand4, "A", "B", "C", "D", "Y");
            blocks[strdup(("NAND4B_" + gateType[i]).c_str())] =  lib_elem_t::create_lib_elem(strdup(("NAND4B_" + gateType[i]).c_str()), false, 4, 1, nand4b, "AN", "B", "C", "D", "Y");
            blocks[strdup(("NAND4XXXB_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("NAND4XXXB_" + gateType[i]).c_str()), false, 4, 1, nand4xxxb, "A", "B", "C", "DN", "Y");

            //NOR gates
            blocks[strdup(("NOR2_" + gateType[i]).c_str())] =  lib_elem_t::create_lib_elem(strdup(("NOR2_" + gateType[i]).c_str()), false, 2, 1, nor2, "A", "B", "Y");
            blocks[strdup(("NOR2XB_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("NOR2XB_" + gateType[i]).c_str()), false, 2, 1, nor2xb, "A", "BN", "Y");
            blocks[strdup(("NOR2B_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("NOR2B_" + gateType[i]).c_str()), false, 2, 1, nor2b, "AN", "B", "Y");
            blocks[strdup(("NOR3_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("NOR3_" + gateType[i]).c_str()), false, 3, 1, nor3, "A", "B", "C", "Y");

            //OR-AND gates
            blocks[strdup(("OA211_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("OA211_" + gateType[i]).c_str()), false, 4, 1, oa211, "A0", "A1", "B0", "C0", "Y");
            blocks[strdup(("OA21A1OI2_" + gateType[i]).c_str())] =  lib_elem_t::create_lib_elem(strdup(("OA21A1OI2_" + gateType[i]).c_str()), false, 4, 1, oa21a1oi2, "A0", "A1", "B0", "C0", "Y");
            blocks[strdup(("OA21_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("OA21_" + gateType[i]).c_str()), false, 3, 1, oa21, "A0", "A1", "B0", "Y");
            blocks[strdup(("OA22_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("OA22_" + gateType[i]).c_str()), false, 4, 1, oa22, "A0", "A1", "B0", "B1", "Y");
            blocks[strdup(("OAI21_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("OAI21_" + gateType[i]).c_str()), false, 3, 1, oai21, "A0", "A1", "B0", "Y");
            blocks[strdup(("OAI21B_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("OAI21B_" + gateType[i]).c_str()), false, 3, 1, oai21b, "A0", "A1", "B0N", "Y");
            blocks[strdup(("OAI211_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("OAI211_" + gateType[i]).c_str()), false, 4, 1, oai211, "A0", "A1", "B0", "C0", "Y");
            blocks[strdup(("OAI221_" + gateType[i]).c_str())] =  lib_elem_t::create_lib_elem(strdup(("OAI221_" + gateType[i]).c_str()), false, 5, 1, oai221, "A0", "A1", "B0", "B1", "C0", "Y");
            blocks[strdup(("OAI222_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("OAI222_" + gateType[i]).c_str()), false, 6, 1, oai222, "A0", "A1", "B0", "B1", "C0", "C1", "Y");
            blocks[strdup(("OAI22_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("OAI22_" + gateType[i]).c_str()), false, 4, 1, oai22, "A0", "A1", "B0", "B1", "Y");
            blocks[strdup(("OAI2XB1_" + gateType[i]).c_str())] =  lib_elem_t::create_lib_elem(strdup(("OAI2XB1_" + gateType[i]).c_str()), false, 3, 1, oai2xb1, "A0", "A1N", "B0", "Y");
            blocks[strdup(("OAI31_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("OAI31_" + gateType[i]).c_str()), false, 4, 1, oai31, "A0", "A1", "A2", "B0", "Y");

            //OR Gates
            blocks[strdup(("OR2_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("OR2_" + gateType[i]).c_str()), false, 2, 1, or2, "A", "B", "Y");
            blocks[strdup(("OR3_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("OR3_" + gateType[i]).c_str()), false, 3, 1, or3, "A", "B", "C", "Y");
            blocks[strdup(("OR4_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("OR4_" + gateType[i]).c_str()), false, 4, 1, or4, "A", "B", "C", "D", "Y");
            blocks[strdup(("OR6_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("OR6_" + gateType[i]).c_str()), false, 6, 1, or6, "A", "B", "C", "D", "E", "F", "Y");

            //Flip-flops and latches

            lib_elem_t::create_seq_elem(blocks, ("A2SDFFQ_" + gateType[i]).c_str(), true, 5, 1, a2sdffq, "A", "B", "SI", "SE", "CK", "Q");
            lib_elem_t::create_seq_elem(blocks, ("A2SDFFQN_" + gateType[i]).c_str(), true, 5, 1, a2sdffqn, "A", "B", "SI", "SE", "CK", "QN");

            lib_elem_t::create_seq_elem(blocks, ("M2SDFFQ_" + gateType[i]).c_str(), true, 6, 1, m2sdffq, "SE", "SI", "S0", "D0", "D1", "CK", "Q");
            lib_elem_t::create_seq_elem(blocks, ("M2SDFFQN_" + gateType[i]).c_str(), true, 6, 1, m2sdffqn, "SE", "SI", "S0", "D0", "D1", "CK", "QN");

            lib_elem_t::create_seq_elem(blocks, ("DFFRPQ_" + gateType[i]).c_str(), true, 3, 1, dffrpq, "R", "D", "CK", "Q");
            lib_elem_t::create_seq_elem(blocks, ("DFFRPQN_" + gateType[i]).c_str(), true, 3, 1, dffrpqn, "R", "D", "CK", "QN");

            lib_elem_t::create_seq_elem(blocks, ("DFFSQ_" + gateType[i]).c_str(), true, 3, 1, dffsq, "SN", "D", "CK", "Q");
            lib_elem_t::create_seq_elem(blocks, ("DFFSQN_" + gateType[i]).c_str(), true, 3, 1, dffsqn, "SN", "D", "CK", "QN");

            lib_elem_t::create_seq_elem(blocks, ("DFFSRPQ_" + gateType[i]).c_str(), true, 4, 1, dffsrpq, "R", "SN", "D", "CK", "Q");
            lib_elem_t::create_seq_elem(blocks, ("DFFNSRPQ_" + gateType[i]).c_str(), true, 4, 1, dffsrpq, "R", "SN", "D", "CKN", "Q"); // just negative edge triggered version of DFFSRPQ.

            lib_elem_t::create_seq_elem(blocks, ("SDFFQ_" + gateType[i]).c_str(), true, 4, 1, sdffq, "D", "SI", "SE", "CK", "Q");
            lib_elem_t::create_seq_elem(blocks, ("SDFFNQ_" + gateType[i]).c_str(), true, 4, 1, sdffq, "D", "SI", "SE", "CK", "Q"); //negedge
            lib_elem_t::create_seq_elem(blocks, ("SDFFYQ_" + gateType[i]).c_str(), true, 4, 1, sdffq, "D", "SI", "SE", "CK", "Q");
            lib_elem_t::create_seq_elem(blocks, ("SDFFQN_" + gateType[i]).c_str(), true, 4, 1, sdffqn, "D", "SI", "SE", "CK", "QN");

            lib_elem_t::create_seq_elem(blocks, ("SDFFRPQ_" + gateType[i]).c_str(), true, 5, 1, sdffrpq, "R", "D", "SI", "SE", "CK", "Q");
            lib_elem_t::create_seq_elem(blocks, ("SDFFNRPQ_" + gateType[i]).c_str(), true, 5, 1, sdffrpq, "R", "D", "SI", "SE", "CKN", "Q"); // neg edge. trigger
            lib_elem_t::create_seq_elem(blocks, ("SDFFRPQN_" + gateType[i]).c_str(), true, 5, 1, sdffrpqn, "R", "D", "SI", "SE", "CK", "QN");

            lib_elem_t::create_seq_elem(blocks, ("SDFFSQ_" + gateType[i]).c_str(), true, 5, 1, sdffsq, "SN", "D", "SI", "SE", "CK", "Q");
            lib_elem_t::create_seq_elem(blocks, ("SDFFNSQ_" + gateType[i]).c_str(), true, 5, 1, sdffsq, "SN", "D", "SI", "SE", "CKN", "Q"); // negedge
            lib_elem_t::create_seq_elem(blocks, ("SDFFSQN_" + gateType[i]).c_str(), true, 5, 1, sdffsqn, "SN", "D", "SI", "SE", "CK", "QN");

            lib_elem_t::create_seq_elem(blocks, ("SDFFSRPQ_" + gateType[i]).c_str(), true, 6, 1, sdffsrpq, "SN", "R", "D", "SI", "SE", "CK", "Q");
            lib_elem_t::create_seq_elem(blocks, ("SDFFNSRPQ_" + gateType[i]).c_str(), true, 6, 1, sdffsrpq, "SN", "R", "D", "SI", "SE", "CKN", "Q"); //negedge

            lib_elem_t::create_seq_elem(blocks, ("DFFQ_" + gateType[i]).c_str(), true, 2, 1, buf, "D", "CK", "Q");
            lib_elem_t::create_seq_elem(blocks, ("DFFNQ_" + gateType[i]).c_str(), true, 2, 1, buf, "D", "CKN", "Q"); //neg edge
            lib_elem_t::create_seq_elem(blocks, ("DFFQN_" + gateType[i]).c_str(), true, 2, 1, inv, "D", "CK", "QN");

            lib_elem_t::create_seq_elem(blocks, ("ESDFFQ_" + gateType[i]).c_str(), true, 5, 1, sdffq, "D", "SI", "SE", "E", "CK", "Q");
            lib_elem_t::create_seq_elem(blocks, ("ESDFFQN_" + gateType[i]).c_str(), true, 5, 1, sdffqn, "D", "SI", "SE", "E", "CK", "QN");

            lib_elem_t::create_seq_elem(blocks, ("LATQ_" + gateType[i]).c_str(), true, 2, 1, buf, "D", "G", "Q");
            lib_elem_t::create_seq_elem(blocks, ("LATQN_" + gateType[i]).c_str(), true, 2, 1, inv, "D", "G", "QN");
            lib_elem_t::create_seq_elem(blocks, ("LATNQ_" + gateType[i]).c_str(), true, 2, 1, buf, "D", "GN", "Q");
            lib_elem_t::create_seq_elem(blocks, ("LATNQN_" + gateType[i]).c_str(), true, 2, 1, buf, "D", "GN", "QN");

            lib_elem_t::create_seq_elem(blocks, ("LATRQ_" + gateType[i]).c_str(), true, 3, 1, latrq, "RN", "D", "G", "Q");
            lib_elem_t::create_seq_elem(blocks, ("LATNRQ_" + gateType[i]).c_str(), true, 3, 1, latrq, "RN", "D", "GN", "Q");

            lib_elem_t::create_seq_elem(blocks, ("LATSPQ_" + gateType[i]).c_str(), true, 3, 1, latspq, "S", "D", "G", "Q");
            lib_elem_t::create_seq_elem(blocks, ("LATNSPQ_" + gateType[i]).c_str(), true, 3, 1, latspq, "S", "D", "GN", "Q");

            lib_elem_t::create_seq_elem(blocks, ("LATSQN_" + gateType[i]).c_str(), true, 3, 1, dffsqn, "SN", "D", "G", "QN");
            lib_elem_t::create_seq_elem(blocks, ("LATNSQN_" + gateType[i]).c_str(), true, 3, 1, dffsqn, "SN", "D", "GN", "QN");

            lib_elem_t::create_seq_elem(blocks, ("LATRPQN_" + gateType[i]).c_str(), true, 3, 1, dffrpqn, "R", "D", "G", "QN");
            lib_elem_t::create_seq_elem(blocks, ("LATNRPQN_" + gateType[i]).c_str(), true, 3, 1, dffrpqn, "R", "D", "GN", "QN");

            // TODO: figure out how to handle these weird things!
            //lib_elem_t::create_seq_elem(blocks, ("POSTICG_" + gateType[i]).c_str(), true, 3, 1, invalid, "E", "SEN", "CK", "Q");
            //lib_elem_t::create_seq_elem(blocks, ("PREICG_" + gateType[i]).c_str(), true, 3, 1, invalid, "E", "SE", "CK", "Q");

            //XOR/XNOR gates
            blocks[strdup(("XOR2_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("XOR2_" + gateType[i]).c_str()), false, 2, 1, xor2, "A", "B", "Y");
            blocks[strdup(("XOR3_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("XOR3_" + gateType[i]).c_str()), false, 3, 1, xor3, "A", "B", "C", "Y");
            blocks[strdup(("XNOR2_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("XNOR2_" + gateType[i]).c_str()), false, 2, 1, xnor2, "A", "B", "Y");
            blocks[strdup(("XNOR3_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("XNOR3_" + gateType[i]).c_str()), false, 3, 1, xnor3, "A", "B", "C", "Y");

            //Buffers
            blocks[strdup(("BUF_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("BUF_" + gateType[i]).c_str()), false, 1, 1, buf, "A", "Y");
            blocks[strdup(("DLY2_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("DLY2_" + gateType[i]).c_str()), false, 1, 1, buf, "A", "Y");
            blocks[strdup(("DLY2S1_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("DLY2S1_" + gateType[i]).c_str()), false, 1, 1, buf, "A", "Y");
            blocks[strdup(("DLY2S3_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("DLY2S3_" + gateType[i]).c_str()), false, 1, 1, buf, "A", "Y");
            blocks[strdup(("DLY2S4_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("DLY2S4_" + gateType[i]).c_str()), false, 1, 1, buf, "A", "Y");
            blocks[strdup(("DLY4_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("DLY4_" + gateType[i]).c_str()), false, 1, 1, buf, "A", "Y");
            blocks[strdup(("BUFH_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("BUFH_" + gateType[i]).c_str()), false, 1, 1, buf, "A", "Y");
            blocks[strdup(("BUFZ_" + gateType[i]).c_str())] = lib_elem_t::create_lib_elem(strdup(("BUFZ_" + gateType[i]).c_str()), false, 2, 1, buf, "A", "OE", "Y");

        }

        blocks[strdup("BUF")] = lib_elem_t::create_lib_elem(strdup("BUF"), false, 1, 1, buf, "A", "Y");

        // fake nodes
        blocks[strdup("BSIM_ADDF_SUM")] = lib_elem_t::create_lib_elem(strdup("BSIM_ADDF_SUM"), false, 3, 1, xor3, "A", "B", "CI", "S");
        blocks[strdup("BSIM_ADDF_CARRY")] = lib_elem_t::create_lib_elem(strdup("BSIM_ADDF_CARRY"), false, 3, 1, cgen, "A", "B", "CI", "CO");

        blocks[strdup("BSIM_ADDFH_SUM")] = lib_elem_t::create_lib_elem(strdup("BSIM_ADDF_SUM"), false, 3, 1, xor3, "A", "B", "CI", "SUM");
        blocks[strdup("BSIM_ADDFH_CARRY")] = lib_elem_t::create_lib_elem(strdup("BSIM_ADDF_CARRY"), false, 3, 1, cgen, "A", "B", "CI", "CO");

        blocks[strdup("BSIM_ADDH_SUM")] = lib_elem_t::create_lib_elem(strdup("BSIM_ADDH_SUM"), false, 2, 1, xor2, "A", "B", "S");
        blocks[strdup("BSIM_ADDH_CARRY")] = lib_elem_t::create_lib_elem(strdup("BSIM_ADDH_CARRY"), false, 2, 1, and2, "A", "B", "CO");

        blocks[strdup("BSIM_MUX21")] = lib_elem_t::create_lib_elem(strdup("BSIM_MUX21"), false, 3, 1, mux2, "A", "B", "S", "Y");
        blocks[strdup("BSIM_AND2")] = lib_elem_t::create_lib_elem(strdup("BSIM_AND2"), false, 2, 1, and2, "A", "B", "Y");
        blocks[strdup("BSIM_INV")] = lib_elem_t::create_lib_elem(strdup("BSIM_INV"), false, 2, 1, inv, "A", "B", "Y");

        // fake flip-flop (right now we don't care about the distinction between latches and flip-flops.)
        blocks[strdup("BSIM_DFF")] = lib_elem_t::create_lib_elem(strdup("BSIM_DFF"), true, 2, 1, invalid, "D", "CK", "Q");
        blocks[strdup("BSIM_DFFQN")] = lib_elem_t::create_lib_elem(strdup("BSIM_DFF"), true, 2, 1, invalid, "D", "CK", "QN");

        blocks[strdup("BSIM_DFFN")] = lib_elem_t::create_lib_elem(strdup("BSIM_DFF"), true, 2, 1, invalid, "D", "CKN", "Q");
        blocks[strdup("BSIM_DFFNQN")] = lib_elem_t::create_lib_elem(strdup("BSIM_DFF"), true, 2, 1, invalid, "D", "CKN", "QN");

        blocks[strdup("BSIM_LAT")] = lib_elem_t::create_lib_elem(strdup("BSIM_DFF"), true, 2, 1, invalid, "D", "G", "Q");
        blocks[strdup("BSIM_LATQN")] = lib_elem_t::create_lib_elem(strdup("BSIM_DFF"), true, 2, 1, invalid, "D", "G", "QN");

        blocks[strdup("BSIM_LATN")] = lib_elem_t::create_lib_elem(strdup("BSIM_DFF"), true, 2, 1, invalid, "D", "GN", "Q");
        blocks[strdup("BSIM_LATNQN")] = lib_elem_t::create_lib_elem(strdup("BSIM_DFF"), true, 2, 1, invalid, "D", "GN", "QN");

        /*Wired high/lo*/
        blocks[strdup("TIEHI_X1M_A12TR")] = lib_elem_t::create_lib_elem(strdup("TIEHI_X1M_A12TR"), false, 0, 1, one, "Y");
        blocks[strdup("TIEHI_X1M_A12TS")] = lib_elem_t::create_lib_elem(strdup("TIEHI_X1M_A12TS"), false, 0, 1, one, "Y");
        blocks[strdup("TIEHI_X1M_A9TS")] = lib_elem_t::create_lib_elem(strdup("TIEHI_X1M_A9TS"), false, 0, 1, one, "Y");
        blocks[strdup("TIEHI_X1M")] = lib_elem_t::create_lib_elem(strdup("TIEHI_X1M"), false, 0, 1, one, "Y");
        
        blocks[strdup("TIELO_X1M_A12TR")] = lib_elem_t::create_lib_elem(strdup("TIELO_X1M_A12TR"), false, 0, 1, zero, "Y");
        blocks[strdup("TIELO_X1M_A12TS")] = lib_elem_t::create_lib_elem(strdup("TIELO_X1M_A12TS"), false, 0, 1, zero, "Y");
        blocks[strdup("TIELO_X1M_A9TS")] = lib_elem_t::create_lib_elem(strdup("TIELO_X1M_A9TS"), false, 0, 1, zero, "Y");
        blocks[strdup("TIELO_X1M")] = lib_elem_t::create_lib_elem(strdup("TIELO_X1M"), false, 0, 1, zero, "Y");

        /* power pads. */
        blocks[strdup("PBIDIR_18_PL_V")] = lib_elem_t::create_block_ex(
            strdup("PBIDIR_18_PL_V"), 
            10, 5, 1,
            "A",  "DS0", "DS1", "IE",  "IS",  "OE",
            "PE", "POE", "PS",  "SR", 
            "RTO", "SNS", "TRIGGER",    /* outputs */
            "PO", "Y",                  /* outputs */
            "PAD"                       /* ports */
        );

        blocks[strdup("PBIDIR_18_PL_H")] = lib_elem_t::create_block_ex(
            strdup("PBIDIR_18_PL_H"), 
            10, 5, 1,
            "A",  "DS0", "DS1", "IE",  "IS",  "OE",
            "PE", "POE", "PS",  "SR", 
            "RTO", "SNS", "TRIGGER",    /* outputs */
            "PO", "Y",                  /* outputs */
            "PAD"                       /* ports */
        );

        blocks[strdup("PDVSS_18_PL_V")] = lib_elem_t::create_block_ex(strdup("PDVSS_18_PL_V"), 0, 3, 0, "RTO", "SNS", "TRIGGER");
        blocks[strdup("PDVSS_18_PL_H")] = lib_elem_t::create_block_ex(strdup("PDVSS_18_PL_H"), 0, 3, 0, "RTO", "SNS", "TRIGGER");
        blocks[strdup("PVSS_18_PL_V")] = lib_elem_t::create_block_ex(strdup("PVSS_18_PL_V"), 0, 3, 0, "RTO", "SNS", "TRIGGER");
        blocks[strdup("PVSS_18_PL_H")] = lib_elem_t::create_block_ex(strdup("PVSS_18_PL_H"), 0, 3, 0, "RTO", "SNS", "TRIGGER");
        blocks[strdup("PDVDD_18_PL_V")] = lib_elem_t::create_block_ex(strdup("PDVDD_18_PL_V"), 0, 3, 0, "RTO", "SNS", "TRIGGER");
        blocks[strdup("PDVDD_18_PL_H")] = lib_elem_t::create_block_ex(strdup("PDVDD_18_PL_H"), 0, 3, 0, "RTO", "SNS", "TRIGGER");
        blocks[strdup("PVDD_18_PL_V")] = lib_elem_t::create_block_ex(strdup("PVDD_18_PL_V"), 0, 3, 0, "RTO", "SNS", "TRIGGER");
        blocks[strdup("PVDD_18_PL_H")] = lib_elem_t::create_block_ex(strdup("PVDD_18_PL_H"), 0, 3, 0, "RTO", "SNS", "TRIGGER");
        blocks[strdup("PVSENSETIE_18_PL_V")] = lib_elem_t::create_block_ex(strdup("PVSENSETIE_18_PL_V"), 0, 3, 0, "RTO", "SNS", "TRIGGER");

        /* SRAMs. */
        blocks[strdup("SRAM2SBSN01024X008D08")] = lib_elem_t::create_block_ex(
            strdup("SRAM2SBSN01024X008D08"), 
            197,
            16,
            0,
            "AA0", "AA1", "AA2", "AA3", "AA4", "AA5", "AA6", "AA7", "AA8",
            "AA9", "AB0", "AB1", "AB2", "AB3", "AB4", "AB5", "AB6", "AB7",
            "AB8", "AB9", "BENA", "BENB", "CENA", "CENB", "CLKA", "CLKB",
            "CLKMUX", "CLKSEL", "COLLDISN", "CRE1", "CRE2", "DA0", "DA1",
            "DA2", "DA3", "DA4", "DA5", "DA6", "DA7", "DB0", "DB1", "DB2",
            "DB3", "DB4", "DB5", "DB6", "DB7", "EMAA0", "EMAA1", "EMAA2",
            "EMAB0", "EMAB1", "EMAB2", "EMASA", "EMASB", "EMAWA0", "EMAWA1",
            "EMAWB0", "EMAWB1", "FCA10", "FCA11", "FCA12", "FCA20", "FCA21",
            "FCA22", "FRA10", "FRA11", "FRA12", "FRA13", "FRA14", "FRA15",
            "FRA16", "FRA20", "FRA21", "FRA22", "FRA23", "FRA24", "FRA25",
            "FRA26", "FRA30", "FRA31", "FRA32", "FRA33", "FRA34", "FRA35",
            "FRA36", "FRA40", "FRA41", "FRA42", "FRA43", "FRA44", "FRA45",
            "FRA46", "GWENA", "GWENB", "NWAA", "NWAB", "RDTA", "RDTB", "RRE1",
            "RRE2", "RRE3", "RRE4", "STOVA", "STOVB", "TAA0", "TAA1", "TAA2",
            "TAA3", "TAA4", "TAA5", "TAA6", "TAA7", "TAA8", "TAA9", "TAB0",
            "TAB1", "TAB2", "TAB3", "TAB4", "TAB5", "TAB6", "TAB7", "TAB8",
            "TAB9", "TCENA", "TCENB", "TDA0", "TDA1", "TDA2", "TDA3", "TDA4",
            "TDA5", "TDA6", "TDA7", "TDB0", "TDB1", "TDB2", "TDB3", "TDB4",
            "TDB5", "TDB6", "TDB7", "TENA", "TENB", "TGWENA", "TGWENB", "TQA0",
            "TQA1", "TQA2", "TQA3", "TQA4", "TQA5", "TQA6", "TQA7", "TQB0",
            "TQB1", "TQB2", "TQB3", "TQB4", "TQB5", "TQB6", "TQB7", "TWENA0",
            "TWENA1", "TWENA2", "TWENA3", "TWENA4", "TWENA5", "TWENA6",
            "TWENA7", "TWENB0", "TWENB1", "TWENB2", "TWENB3", "TWENB4",
            "TWENB5", "TWENB6", "TWENB7", "WBTA", "WBTB", "WENA0", "WENA1",
            "WENA2", "WENA3", "WENA4", "WENA5", "WENA6", "WENA7", "WENB0",
            "WENB1", "WENB2", "WENB3", "WENB4", "WENB5", "WENB6", "WENB7",
            "QA0", "QA1", "QA2", "QA3", "QA4", "QA5", "QA6", "QA7", "QB0",
            "QB1", "QB2", "QB3", "QB4", "QB5", "QB6", "QB7"
        );
        blocks[strdup("SRAM2SBSN00512X016D04")] = lib_elem_t::create_block_ex(
            strdup("SRAM2SBSN00512X016D04"),
            273,
            32,
            0,
            "AA0", "AA1", "AA2", "AA3", "AA4", "AA5", "AA6", "AA7", "AA8", "AB0", "AB1",
            "AB2", "AB3", "AB4", "AB5", "AB6", "AB7", "AB8", "BENA", "BENB", "CENA",
            "CENB", "CLKA", "CLKB", "CLKMUX", "CLKSEL", "COLLDISN", "CRE1", "CRE2", "DA0",
            "DA1", "DA10", "DA11", "DA12", "DA13", "DA14", "DA15", "DA2", "DA3", "DA4",
            "DA5", "DA6", "DA7", "DA8", "DA9", "DB0", "DB1", "DB10", "DB11", "DB12",
            "DB13", "DB14", "DB15", "DB2", "DB3", "DB4", "DB5", "DB6", "DB7", "DB8", "DB9",
            "EMAA0", "EMAA1", "EMAA2", "EMAB0", "EMAB1", "EMAB2", "EMASA", "EMASB",
            "EMAWA0", "EMAWA1", "EMAWB0", "EMAWB1", "FCA10", "FCA11", "FCA12", "FCA20",
            "FCA21", "FCA22", "FRA10", "FRA11", "FRA12", "FRA13", "FRA14", "FRA15",
            "FRA16", "FRA20", "FRA21", "FRA22", "FRA23", "FRA24", "FRA25", "FRA26",
            "FRA30", "FRA31", "FRA32", "FRA33", "FRA34", "FRA35", "FRA36", "FRA40",
            "FRA41", "FRA42", "FRA43", "FRA44", "FRA45", "FRA46", "GWENA", "GWENB", "NWAA",
            "NWAB", "RDTA", "RDTB", "RRE1", "RRE2", "RRE3", "RRE4", "STOVA", "STOVB",
            "TAA0", "TAA1", "TAA2", "TAA3", "TAA4", "TAA5", "TAA6", "TAA7", "TAA8", "TAB0",
            "TAB1", "TAB2", "TAB3", "TAB4", "TAB5", "TAB6", "TAB7", "TAB8", "TCENA",
            "TCENB", "TDA0", "TDA1", "TDA10", "TDA11", "TDA12", "TDA13", "TDA14", "TDA15",
            "TDA2", "TDA3", "TDA4", "TDA5", "TDA6", "TDA7", "TDA8", "TDA9", "TDB0", "TDB1",
            "TDB10", "TDB11", "TDB12", "TDB13", "TDB14", "TDB15", "TDB2", "TDB3", "TDB4",
            "TDB5", "TDB6", "TDB7", "TDB8", "TDB9", "TENA", "TENB", "TGWENA", "TGWENB",
            "TQA0", "TQA1", "TQA10", "TQA11", "TQA12", "TQA13", "TQA14", "TQA15", "TQA2",
            "TQA3", "TQA4", "TQA5", "TQA6", "TQA7", "TQA8", "TQA9", "TQB0", "TQB1",
            "TQB10", "TQB11", "TQB12", "TQB13", "TQB14", "TQB15", "TQB2", "TQB3", "TQB4",
            "TQB5", "TQB6", "TQB7", "TQB8", "TQB9", "TWENA0", "TWENA1", "TWENA10",
            "TWENA11", "TWENA12", "TWENA13", "TWENA14", "TWENA15", "TWENA2", "TWENA3",
            "TWENA4", "TWENA5", "TWENA6", "TWENA7", "TWENA8", "TWENA9", "TWENB0", "TWENB1",
            "TWENB10", "TWENB11", "TWENB12", "TWENB13", "TWENB14", "TWENB15", "TWENB2",
            "TWENB3", "TWENB4", "TWENB5", "TWENB6", "TWENB7", "TWENB8", "TWENB9", "WBTA",
            "WBTB", "WENA0", "WENA1", "WENA10", "WENA11", "WENA12", "WENA13", "WENA14",
            "WENA15", "WENA2", "WENA3", "WENA4", "WENA5", "WENA6", "WENA7", "WENA8",
            "WENA9", "WENB0", "WENB1", "WENB10", "WENB11", "WENB12", "WENB13", "WENB14",
            "WENB15", "WENB2", "WENB3", "WENB4", "WENB5", "WENB6", "WENB7", "WENB8",
            "WENB9", "QA0", "QA1", "QA10", "QA11", "QA12", "QA13", "QA14", "QA15", "QA2",
            "QA3", "QA4", "QA5", "QA6", "QA7", "QA8", "QA9", "QB0", "QB1", "QB10", "QB11",
            "QB12", "QB13", "QB14", "QB15", "QB2", "QB3", "QB4", "QB5", "QB6", "QB7",
            "QB8", "QB9"
        );
        blocks[strdup("RF1CSN0256X128D2")] = lib_elem_t::create_block_ex(
            strdup("RF1CSN0256X128D2"),
            684,
            128,
            0,
            "AC0", "AW0", "AW1", "AW2", "AW3", "AW4", "AW5", "AW6", "BW0", "BW1", "BW10",
            "BW100", "BW101", "BW102", "BW103", "BW104", "BW105", "BW106", "BW107",
            "BW108", "BW109", "BW11", "BW110", "BW111", "BW112", "BW113", "BW114", "BW115",
            "BW116", "BW117", "BW118", "BW119", "BW12", "BW120", "BW121", "BW122", "BW123",
            "BW124", "BW125", "BW126", "BW127", "BW13", "BW14", "BW15", "BW16", "BW17",
            "BW18", "BW19", "BW2", "BW20", "BW21", "BW22", "BW23", "BW24", "BW25", "BW26",
            "BW27", "BW28", "BW29", "BW3", "BW30", "BW31", "BW32", "BW33", "BW34", "BW35",
            "BW36", "BW37", "BW38", "BW39", "BW4", "BW40", "BW41", "BW42", "BW43", "BW44",
            "BW45", "BW46", "BW47", "BW48", "BW49", "BW5", "BW50", "BW51", "BW52", "BW53",
            "BW54", "BW55", "BW56", "BW57", "BW58", "BW59", "BW6", "BW60", "BW61", "BW62",
            "BW63", "BW64", "BW65", "BW66", "BW67", "BW68", "BW69", "BW7", "BW70", "BW71",
            "BW72", "BW73", "BW74", "BW75", "BW76", "BW77", "BW78", "BW79", "BW8", "BW80",
            "BW81", "BW82", "BW83", "BW84", "BW85", "BW86", "BW87", "BW88", "BW89", "BW9",
            "BW90", "BW91", "BW92", "BW93", "BW94", "BW95", "BW96", "BW97", "BW98", "BW99",
            "CLK", "CR0", "CR1", "CR2", "CR3", "CR4", "CR5", "CRE", "D0", "D1", "D10",
            "D100", "D101", "D102", "D103", "D104", "D105", "D106", "D107", "D108", "D109",
            "D11", "D110", "D111", "D112", "D113", "D114", "D115", "D116", "D117", "D118",
            "D119", "D12", "D120", "D121", "D122", "D123", "D124", "D125", "D126", "D127",
            "D13", "D14", "D15", "D16", "D17", "D18", "D19", "D2", "D20", "D21", "D22",
            "D23", "D24", "D25", "D26", "D27", "D28", "D29", "D3", "D30", "D31", "D32",
            "D33", "D34", "D35", "D36", "D37", "D38", "D39", "D4", "D40", "D41", "D42",
            "D43", "D44", "D45", "D46", "D47", "D48", "D49", "D5", "D50", "D51", "D52",
            "D53", "D54", "D55", "D56", "D57", "D58", "D59", "D6", "D60", "D61", "D62",
            "D63", "D64", "D65", "D66", "D67", "D68", "D69", "D7", "D70", "D71", "D72",
            "D73", "D74", "D75", "D76", "D77", "D78", "D79", "D8", "D80", "D81", "D82",
            "D83", "D84", "D85", "D86", "D87", "D88", "D89", "D9", "D90", "D91", "D92",
            "D93", "D94", "D95", "D96", "D97", "D98", "D99", "DEEPSLEEP", "MIEMAM",
            "MIEMAS0", "MIEMAS1", "MIEMAS2", "MIEMAW0", "MIEMAW1", "MIEMAWASS0",
            "MIEMAWASS1", "MIFLOOD", "MIPGDISABLE", "MITESTM1", "MITESTM3", "MIWASSD",
            "MIWRTM", "READ", "TAC0", "TAW0", "TAW1", "TAW2", "TAW3", "TAW4", "TAW5",
            "TAW6", "TBW0", "TBW1", "TBW10", "TBW100", "TBW101", "TBW102", "TBW103",
            "TBW104", "TBW105", "TBW106", "TBW107", "TBW108", "TBW109", "TBW11", "TBW110",
            "TBW111", "TBW112", "TBW113", "TBW114", "TBW115", "TBW116", "TBW117", "TBW118",
            "TBW119", "TBW12", "TBW120", "TBW121", "TBW122", "TBW123", "TBW124", "TBW125",
            "TBW126", "TBW127", "TBW13", "TBW14", "TBW15", "TBW16", "TBW17", "TBW18",
            "TBW19", "TBW2", "TBW20", "TBW21", "TBW22", "TBW23", "TBW24", "TBW25", "TBW26",
            "TBW27", "TBW28", "TBW29", "TBW3", "TBW30", "TBW31", "TBW32", "TBW33", "TBW34",
            "TBW35", "TBW36", "TBW37", "TBW38", "TBW39", "TBW4", "TBW40", "TBW41", "TBW42",
            "TBW43", "TBW44", "TBW45", "TBW46", "TBW47", "TBW48", "TBW49", "TBW5", "TBW50",
            "TBW51", "TBW52", "TBW53", "TBW54", "TBW55", "TBW56", "TBW57", "TBW58",
            "TBW59", "TBW6", "TBW60", "TBW61", "TBW62", "TBW63", "TBW64", "TBW65", "TBW66",
            "TBW67", "TBW68", "TBW69", "TBW7", "TBW70", "TBW71", "TBW72", "TBW73", "TBW74",
            "TBW75", "TBW76", "TBW77", "TBW78", "TBW79", "TBW8", "TBW80", "TBW81", "TBW82",
            "TBW83", "TBW84", "TBW85", "TBW86", "TBW87", "TBW88", "TBW89", "TBW9", "TBW90",
            "TBW91", "TBW92", "TBW93", "TBW94", "TBW95", "TBW96", "TBW97", "TBW98",
            "TBW99", "TD0", "TD1", "TD10", "TD100", "TD101", "TD102", "TD103", "TD104",
            "TD105", "TD106", "TD107", "TD108", "TD109", "TD11", "TD110", "TD111", "TD112",
            "TD113", "TD114", "TD115", "TD116", "TD117", "TD118", "TD119", "TD12", "TD120",
            "TD121", "TD122", "TD123", "TD124", "TD125", "TD126", "TD127", "TD13", "TD14",
            "TD15", "TD16", "TD17", "TD18", "TD19", "TD2", "TD20", "TD21", "TD22", "TD23",
            "TD24", "TD25", "TD26", "TD27", "TD28", "TD29", "TD3", "TD30", "TD31", "TD32",
            "TD33", "TD34", "TD35", "TD36", "TD37", "TD38", "TD39", "TD4", "TD40", "TD41",
            "TD42", "TD43", "TD44", "TD45", "TD46", "TD47", "TD48", "TD49", "TD5", "TD50",
            "TD51", "TD52", "TD53", "TD54", "TD55", "TD56", "TD57", "TD58", "TD59", "TD6",
            "TD60", "TD61", "TD62", "TD63", "TD64", "TD65", "TD66", "TD67", "TD68", "TD69",
            "TD7", "TD70", "TD71", "TD72", "TD73", "TD74", "TD75", "TD76", "TD77", "TD78",
            "TD79", "TD8", "TD80", "TD81", "TD82", "TD83", "TD84", "TD85", "TD86", "TD87",
            "TD88", "TD89", "TD9", "TD90", "TD91", "TD92", "TD93", "TD94", "TD95", "TD96",
            "TD97", "TD98", "TD99", "TDEEPSLEEP", "TQ0", "TQ1", "TQ10", "TQ100", "TQ101",
            "TQ102", "TQ103", "TQ104", "TQ105", "TQ106", "TQ107", "TQ108", "TQ109", "TQ11",
            "TQ110", "TQ111", "TQ112", "TQ113", "TQ114", "TQ115", "TQ116", "TQ117",
            "TQ118", "TQ119", "TQ12", "TQ120", "TQ121", "TQ122", "TQ123", "TQ124", "TQ125",
            "TQ126", "TQ127", "TQ13", "TQ14", "TQ15", "TQ16", "TQ17", "TQ18", "TQ19",
            "TQ2", "TQ20", "TQ21", "TQ22", "TQ23", "TQ24", "TQ25", "TQ26", "TQ27", "TQ28",
            "TQ29", "TQ3", "TQ30", "TQ31", "TQ32", "TQ33", "TQ34", "TQ35", "TQ36", "TQ37",
            "TQ38", "TQ39", "TQ4", "TQ40", "TQ41", "TQ42", "TQ43", "TQ44", "TQ45", "TQ46",
            "TQ47", "TQ48", "TQ49", "TQ5", "TQ50", "TQ51", "TQ52", "TQ53", "TQ54", "TQ55",
            "TQ56", "TQ57", "TQ58", "TQ59", "TQ6", "TQ60", "TQ61", "TQ62", "TQ63", "TQ64",
            "TQ65", "TQ66", "TQ67", "TQ68", "TQ69", "TQ7", "TQ70", "TQ71", "TQ72", "TQ73",
            "TQ74", "TQ75", "TQ76", "TQ77", "TQ78", "TQ79", "TQ8", "TQ80", "TQ81", "TQ82",
            "TQ83", "TQ84", "TQ85", "TQ86", "TQ87", "TQ88", "TQ89", "TQ9", "TQ90", "TQ91",
            "TQ92", "TQ93", "TQ94", "TQ95", "TQ96", "TQ97", "TQ98", "TQ99", "TREAD",
            "TWRITE", "WRITE", "Q0", "Q1", "Q10", "Q100", "Q101", "Q102", "Q103", "Q104",
            "Q105", "Q106", "Q107", "Q108", "Q109", "Q11", "Q110", "Q111", "Q112", "Q113",
            "Q114", "Q115", "Q116", "Q117", "Q118", "Q119", "Q12", "Q120", "Q121", "Q122",
            "Q123", "Q124", "Q125", "Q126", "Q127", "Q13", "Q14", "Q15", "Q16", "Q17",
            "Q18", "Q19", "Q2", "Q20", "Q21", "Q22", "Q23", "Q24", "Q25", "Q26", "Q27",
            "Q28", "Q29", "Q3", "Q30", "Q31", "Q32", "Q33", "Q34", "Q35", "Q36", "Q37",
            "Q38", "Q39", "Q4", "Q40", "Q41", "Q42", "Q43", "Q44", "Q45", "Q46", "Q47",
            "Q48", "Q49", "Q5", "Q50", "Q51", "Q52", "Q53", "Q54", "Q55", "Q56", "Q57",
            "Q58", "Q59", "Q6", "Q60", "Q61", "Q62", "Q63", "Q64", "Q65", "Q66", "Q67",
            "Q68", "Q69", "Q7", "Q70", "Q71", "Q72", "Q73", "Q74", "Q75", "Q76", "Q77",
            "Q78", "Q79", "Q8", "Q80", "Q81", "Q82", "Q83", "Q84", "Q85", "Q86", "Q87",
            "Q88", "Q89", "Q9", "Q90", "Q91", "Q92", "Q93", "Q94", "Q95", "Q96", "Q97",
            "Q98", "Q99"
        );
        blocks[strdup("RF1CSN0256X021D2")] = lib_elem_t::create_block_ex(
            strdup("RF1CSN0256X021D2"),
            147,
            20,
            0,
            "AC0", "AW0", "AW1", "AW2", "AW3", "AW4", "AW5", "AW6", "BW0", "BW1", "BW10",
            "BW11", "BW12", "BW13", "BW14", "BW15", "BW16", "BW17", "BW18", "BW19", "BW2",
            "BW20", "BW3", "BW4", "BW5", "BW6", "BW7", "BW8", "BW9", "CLK", "CR0", "CR1",
            "CR2", "CR3", "CRE", "D0", "D1", "D10", "D11", "D12", "D13", "D14", "D15",
            "D16", "D17", "D18", "D19", "D2", "D20", "D3", "D4", "D5", "D6", "D7", "D8",
            "D9", "DEEPSLEEP", "MIEMAM", "MIEMAS0", "MIEMAS1", "MIEMAS2", "MIEMAW0",
            "MIEMAW1", "MIEMAWASS0", "MIEMAWASS1", "MIFLOOD", "MIPGDISABLE", "MITESTM1",
            "MITESTM3", "MIWASSD", "MIWRTM", "READ", "TAC0", "TAW0", "TAW1", "TAW2",
            "TAW3", "TAW4", "TAW5", "TAW6", "TBW0", "TBW1", "TBW10", "TBW11", "TBW12",
            "TBW13", "TBW14", "TBW15", "TBW16", "TBW17", "TBW18", "TBW19", "TBW2", "TBW20",
            "TBW3", "TBW4", "TBW5", "TBW6", "TBW7", "TBW8", "TBW9", "TD0", "TD1", "TD10",
            "TD11", "TD12", "TD13", "TD14", "TD15", "TD16", "TD17", "TD18", "TD19", "TD2",
            "TD20", "TD3", "TD4", "TD5", "TD6", "TD7", "TD8", "TD9", "TDEEPSLEEP", "TQ0",
            "TQ1", "TQ10", "TQ11", "TQ12", "TQ13", "TQ14", "TQ15", "TQ16", "TQ17", "TQ18",
            "TQ19", "TQ2", "TQ20", "TQ3", "TQ4", "TQ5", "TQ6", "TQ7", "TQ8", "TQ9",
            "TREAD", "TWRITE", "WRITE", "Q0", "Q1", "Q10", "Q11", "Q12", "Q13", "Q14",
            "Q15", "Q16", "Q17", "Q18", "Q19", "Q2", "Q3", "Q4", "Q5", "Q6", "Q7", "Q8",
            "Q9"
        );
        blocks[strdup("RF2TCSG0064X032D1")] = lib_elem_t::create_block_ex(
            strdup("RF2TCSG0064X032D1"),
            217,
            32,
            0,
            "ARW0", "ARW1", "ARW2", "ARW3", "ARW4", "ARW5", "AWW0", "AWW1", "AWW2", "AWW3",
            "AWW4", "AWW5", "BW0", "BW1", "BW10", "BW11", "BW12", "BW13", "BW14", "BW15",
            "BW16", "BW17", "BW18", "BW19", "BW2", "BW20", "BW21", "BW22", "BW23", "BW24",
            "BW25", "BW26", "BW27", "BW28", "BW29", "BW3", "BW30", "BW31", "BW4", "BW5",
            "BW6", "BW7", "BW8", "BW9", "CLKR", "CLKW", "CR00", "CR01", "CR02", "CR03",
            "CR10", "CR11", "CR12", "CR13", "CRE0", "CRE1", "D0", "D1", "D10", "D11",
            "D12", "D13", "D14", "D15", "D16", "D17", "D18", "D19", "D2", "D20", "D21",
            "D22", "D23", "D24", "D25", "D26", "D27", "D28", "D29", "D3", "D30", "D31",
            "D4", "D5", "D6", "D7", "D8", "D9", "DEEPSLEEP", "MIEMAS0", "MIEMAS1",
            "MIEMAS2", "MIEMAW0", "MIEMAW1", "MIEMAWASS0", "MIEMAWASS1", "MIFLOOD",
            "MIRPRESEL", "MITESTM1", "MITESTM3", "MITPWLC", "MITPWLCE", "MIWASSD",
            "MIWRTM", "READ", "TARW0", "TARW1", "TARW2", "TARW3", "TARW4", "TARW5",
            "TAWW0", "TAWW1", "TAWW2", "TAWW3", "TAWW4", "TAWW5", "TBW0", "TBW1", "TBW10",
            "TBW11", "TBW12", "TBW13", "TBW14", "TBW15", "TBW16", "TBW17", "TBW18",
            "TBW19", "TBW2", "TBW20", "TBW21", "TBW22", "TBW23", "TBW24", "TBW25", "TBW26",
            "TBW27", "TBW28", "TBW29", "TBW3", "TBW30", "TBW31", "TBW4", "TBW5", "TBW6",
            "TBW7", "TBW8", "TBW9", "TD0", "TD1", "TD10", "TD11", "TD12", "TD13", "TD14",
            "TD15", "TD16", "TD17", "TD18", "TD19", "TD2", "TD20", "TD21", "TD22", "TD23",
            "TD24", "TD25", "TD26", "TD27", "TD28", "TD29", "TD3", "TD30", "TD31", "TD4",
            "TD5", "TD6", "TD7", "TD8", "TD9", "TDEEPSLEEP", "TQ0", "TQ1", "TQ10", "TQ11",
            "TQ12", "TQ13", "TQ14", "TQ15", "TQ16", "TQ17", "TQ18", "TQ19", "TQ2", "TQ20",
            "TQ21", "TQ22", "TQ23", "TQ24", "TQ25", "TQ26", "TQ27", "TQ28", "TQ29", "TQ3",
            "TQ30", "TQ31", "TQ4", "TQ5", "TQ6", "TQ7", "TQ8", "TQ9", "TREAD", "TWRITE",
            "WRITE", "Q0", "Q1", "Q10", "Q11", "Q12", "Q13", "Q14", "Q15", "Q16", "Q17",
            "Q18", "Q19", "Q2", "Q20", "Q21", "Q22", "Q23", "Q24", "Q25", "Q26", "Q27",
            "Q28", "Q29", "Q3", "Q30", "Q31", "Q4", "Q5", "Q6", "Q7", "Q8", "Q9"
        );

        std::cout << "[DONE]" << std::endl;
    }

    void lib_12soi_t::rewrite_node(node_t* n, flat_module_t* mod)
    {
        lib_elem_t* le = n->get_lib_elem();
        if(le == NULL) return;
    }


    void lib_12soi_t::preprocess(module_list_t& modules)
    {
        for(unsigned i=0; i != modules.size(); i++) {
            preprocess_module(modules[i]);
        }
    }

    void lib_12soi_t::preprocess_module(module_t* mod)
    {
        for(unsigned i=0; i != mod->stms->size(); i++) {
            stm_t* s = (*mod->stms)[i];
            if(s->type == stm_t::MODULE_INST) {
                module_inst_t* mi = s->module_inst;
                process_module_inst(mod, mi);
            }
        }
    }

    void lib_12soi_t::process_module_inst(module_t* mod, module_inst_t* mi)
    {
        using namespace std;

        if(strstr(mi->module->full_name, "ADDF_") == mi->module->full_name) {
            string name("ADDF");
            process_adder(name, mod, mi);
        } else if(strstr(mi->module->full_name, "ADDFH_") == mi->module->full_name) {
            string name("ADDFH");
            process_adder(name, mod, mi);
        } else if(strstr(mi->module->full_name, "ADDH_") == mi->module->full_name) {
            string name("ADDH");
            process_adder(name, mod, mi);
        }
    }

    void lib_12soi_t::process_adder(std::string& type, module_t* mod, module_inst_t* mi)
    {
        wirename_t *module_name_sum = new wirename_t(("BSIM_" + type + "_SUM").c_str());
        wirename_t *module_name_carry = new wirename_t(("BSIM_" + type + "_CARRY").c_str());

        string instance_name_sum_str = string(mi->instance->full_name);
        string instance_name_carry_str = string(mi->instance->full_name) + "_CARRY";

        wirename_t *instance_name_sum = new wirename_t(instance_name_sum_str.c_str());
        wirename_t *instance_name_carry = new wirename_t(instance_name_carry_str.c_str());

        bindinglist_t* binding_list_carry = new bindinglist_t();
        bindinglist_t* binding_list_sum = new bindinglist_t();
        for(unsigned i=0; i != mi->bindings->size(); i++) {
            wirebinding_t& wb = (*mi->bindings)[i];
            if(strcmp(wb.formal.full_name, "S") != 0 &&
               strcmp(wb.formal.full_name, "SUM") != 0) 
            {
                binding_list_carry->push_back(wb);
            }
            if(strcmp(wb.formal.full_name, "CO") != 0) {
                binding_list_sum->push_back(wb);
            }
        }
        for(unsigned i=0; i != binding_list_carry->size(); i++) {
            wirebinding_t& wb = (*binding_list_carry)[i];
            if(strcmp(wb.formal.full_name, "CO") != 0) {
                wb.enable_output = false;
            }
        }

        // create a new carry instances.
        module_inst_t* carry = new module_inst_t(
                module_name_carry, 
                instance_name_carry,
                binding_list_carry);
        carry->suppress = true;
        if(carry->has_port("CO")) {
            mi->sibling = carry;
            mod->stms->push_back(new stm_t(carry));
        } else {
            delete carry;
        }

        // modify the sum instance.
        mi->joint_module = new wirename_t(mi->module->full_name);
        delete mi->module;
        delete mi->instance;
        delete mi->bindings;
        mi->module = module_name_sum;
        mi->instance = instance_name_sum;
        mi->bindings = binding_list_sum;
    }


    lib_12soi_t::~lib_12soi_t()
    {
        for(map_t::iterator it = blocks.begin(); it != blocks.end(); it++) {
            free( (void*) it->first );
            delete it->second;
        }
    }
}

namespace lib_gtech_n {
    struct lib_gtech_t : public ILibrary {
        virtual void init();
        virtual void preprocess(module_list_t& modules);
        void preprocess_module(module_t* mod);
        void bind_onezero(module_t* mod);
        void process_module_inst(module_t* mod, module_inst_t* mi);
        void process_dff_insts(module_t* mod, module_inst_t* mi);
        void process_dff_q_qn(module_t* mod, module_inst_t* mi, const char* qwire, const char* qnwire);
        void process_assign(assgn_t* assgn);
    };
    typedef input_provider_t* ipp;

    // and
    BDD and2(ipp e) { return e->inp(0) & e->inp(1); }
    BDD and3(ipp e) { return e->inp(0) & e->inp(1) & e->inp(2); }
    BDD and4(ipp e) { return e->inp(0) & e->inp(1) & e->inp(2) & e->inp(3); }
    BDD and5(ipp e) { return e->inp(0) & e->inp(1) & e->inp(2) & e->inp(3) & e->inp(4); }

    // nand
    BDD nand2(ipp e) { return !(e->inp(0) & e->inp(1)); }
    BDD nand3(ipp e) { return !(e->inp(0) & e->inp(1) & e->inp(2)); }
    BDD nand4(ipp e) { return !(e->inp(0) & e->inp(1) & e->inp(2) & e->inp(3)); }
    BDD nand5(ipp e) { return !(e->inp(0) & e->inp(1) & e->inp(2) & e->inp(3) & e->inp(4)); }

    // or
    BDD or2(ipp e) { return e->inp(0) | e->inp(1); }
    BDD or3(ipp e) { return e->inp(0) | e->inp(1) | e->inp(2); }
    BDD or4(ipp e) { return e->inp(0) | e->inp(1) | e->inp(2) | e->inp(3); }
    BDD or5(ipp e) { return e->inp(0) | e->inp(1) | e->inp(2) | e->inp(3) | e->inp(4); }

    // nor
    BDD nor2(ipp e) { return !(e->inp(0) | e->inp(1)); }
    BDD nor3(ipp e) { return !(e->inp(0) | e->inp(1) | e->inp(2)); }
    BDD nor4(ipp e) { return !(e->inp(0) | e->inp(1) | e->inp(2) | e->inp(3)); }
    BDD nor5(ipp e) { return !(e->inp(0) | e->inp(1) | e->inp(2) | e->inp(3) | e->inp(4)); }

    // xor
    BDD xor2(ipp e) { return e->inp(0) ^ e->inp(1); }
    BDD xor3(ipp e) { return e->inp(0) ^ e->inp(1) ^ e->inp(2); }
    BDD xor4(ipp e) { return e->inp(0) ^ e->inp(1) ^ e->inp(2) ^ e->inp(3); }
    BDD xor5(ipp e) { return e->inp(0) ^ e->inp(1) ^ e->inp(2) ^ e->inp(3) ^ e->inp(4); }

    // xnor
    BDD xnor2(ipp e) { return !(e->inp(0) ^ e->inp(1)); }
    BDD xnor3(ipp e) { return !(e->inp(0) ^ e->inp(1) ^ e->inp(2)); }
    BDD xnor4(ipp e) { return !(e->inp(0) ^ e->inp(1) ^ e->inp(2) ^ e->inp(3)); }
    BDD xnor5(ipp e) { return !(e->inp(0) ^ e->inp(1) ^ e->inp(2) ^ e->inp(3) ^ e->inp(4)); }

    // and-not and or-not
    BDD and_not(ipp e) { return e->inp(0) & (!e->inp(1)); }
    BDD or_not(ipp e) { return e->inp(0) | (!e->inp(1)); }

    // and-or
    BDD ao21(ipp e) { return (e->inp(0) & e->inp(1)) | e->inp(2); }
    BDD aoi21(ipp e) { return !((e->inp(0) & e->inp(1)) | e->inp(2)); }
    BDD ao22(ipp e) { return (e->inp(0) & e->inp(1)) | (e->inp(2) & e->inp(3)); }
    BDD aoi22(ipp e) { return !((e->inp(0) & e->inp(1)) | (e->inp(2) & e->inp(3))); }
    BDD aoi2n2(ipp e) { return !((e->inp(0) & e->inp(1)) | (!(e->inp(2) & e->inp(3)))); }
    BDD aoi222(ipp e) { return !((e->inp(0) & e->inp(1)) | (e->inp(2) & e->inp(3)) | (e->inp(4) & e->inp(5))); }

    // or-and
    BDD oa21(ipp e) { return (e->inp(0) | e->inp(1)) & e->inp(2); }
    BDD oai21(ipp e) { return !((e->inp(0) | e->inp(1)) & e->inp(2)); }
    BDD oa22(ipp e) { return (e->inp(0) | e->inp(1)) & (e->inp(2) | e->inp(3)); }
    BDD oai22(ipp e) { return !((e->inp(0) | e->inp(1)) & (e->inp(2) | e->inp(3))); }
    BDD oai2n2(ipp e) { return !((e->inp(0) | e->inp(1)) & (!(e->inp(2) | e->inp(3)))); }

    // misc.
    BDD maj23(ipp e) { return (e->inp(0) & e->inp(1)) | (e->inp(1) & e->inp(2)) | (e->inp(2) & e->inp(0)); }
    BDD mux2(ipp e) { 
        BDD s = e->inp(2);
        BDD a = e->inp(0);
        BDD b = e->inp(1);
        return ((!s & a) | (s & b));
    }
    BDD mux2i(ipp e) { return !mux2(e); }
    BDD mux3(ipp e) {
        BDD a = e->inp(0);
        BDD b = e->inp(1);
        BDD c = e->inp(2);
        BDD d = e->inp(3);

        BDD s0 = e->inp(4);
        BDD s1 = e->inp(5);

        return (!s1 & !s0 & a) |
               (!s1 &  s0 & b) |
               (s1  & !s0 & c) |
               (s1  &  s0 & d);
    }
    BDD inv(ipp e) { return !e->inp(0); }
    BDD one(ipp e) { return e->one(); }
    BDD zero(ipp e) { return e->zero(); }
    BDD buf(ipp e) { return e->inp(0); }
    BDD invalid(ipp e) { assert(false); return e->inp(0); }

    void lib_gtech_t::init()
    {
        // and
        blocks["GTECH_AND2"] = lib_elem_t::create_lib_elem("GTECH_AND2", false, 2, 1, and2, "A", "B", "Z"); 
        // fake and2
        blocks["AN2"] = lib_elem_t::create_lib_elem("AN2", false, 2, 1, and2, "A", "B", "Z"); 
        blocks["GTECH_AND3"] = lib_elem_t::create_lib_elem("GTECH_AND3", false, 3, 1, and3, "A", "B", "C", "Z"); 
        blocks["GTECH_AND4"] = lib_elem_t::create_lib_elem("GTECH_AND4", false, 4, 1, and4, "A", "B", "C", "D", "Z"); 
        blocks["GTECH_AND5"] = lib_elem_t::create_lib_elem("GTECH_AND5", false, 5, 1, and5, "A", "B", "C", "D", "E", "Z"); 

        // nand
        blocks["GTECH_NAND2"] = lib_elem_t::create_lib_elem("GTECH_NAND2", false, 2, 1, nand2, "A", "B", "Z"); 
        blocks["GTECH_NAND3"] = lib_elem_t::create_lib_elem("GTECH_NAND3", false, 3, 1, nand3, "A", "B", "C", "Z"); 
        blocks["GTECH_NAND4"] = lib_elem_t::create_lib_elem("GTECH_NAND4", false, 4, 1, nand4, "A", "B", "C", "D", "Z"); 
        blocks["GTECH_NAND5"] = lib_elem_t::create_lib_elem("GTECH_NAND5", false, 5, 1, nand5, "A", "B", "C", "D", "E", "Z"); 

        // or
        blocks["GTECH_OR2"] = lib_elem_t::create_lib_elem("GTECH_OR2", false, 2, 1, or2, "A", "B", "Z"); 
        // fake or2
        blocks["OR2"] = lib_elem_t::create_lib_elem("OR2", false, 2, 1, or2, "A", "B", "Z"); 
        blocks["GTECH_OR3"] = lib_elem_t::create_lib_elem("GTECH_OR3", false, 3, 1, or3, "A", "B", "C", "Z"); 
        blocks["GTECH_OR4"] = lib_elem_t::create_lib_elem("GTECH_OR4", false, 4, 1, or4, "A", "B", "C", "D", "Z"); 
        blocks["GTECH_OR5"] = lib_elem_t::create_lib_elem("GTECH_OR5", false, 5, 1, or5, "A", "B", "C", "D", "E", "Z"); 

        // nor
        blocks["GTECH_NOR2"] = lib_elem_t::create_lib_elem("GTECH_NOR2", false, 2, 1, nor2, "A", "B", "Z"); 
        blocks["GTECH_NOR3"] = lib_elem_t::create_lib_elem("GTECH_NOR3", false, 3, 1, nor3, "A", "B", "C", "Z"); 
        blocks["GTECH_NOR4"] = lib_elem_t::create_lib_elem("GTECH_NOR4", false, 4, 1, nor4, "A", "B", "C", "D", "Z"); 
        blocks["GTECH_NOR5"] = lib_elem_t::create_lib_elem("GTECH_NOR5", false, 5, 1, nor5, "A", "B", "C", "D", "E", "Z"); 

        // xor
        blocks["GTECH_XOR2"] = lib_elem_t::create_lib_elem("GTECH_XOR2", false, 2, 1, xor2, "A", "B", "Z"); 
        blocks["GTECH_XOR3"] = lib_elem_t::create_lib_elem("GTECH_XOR3", false, 3, 1, xor3, "A", "B", "C", "Z"); 
        blocks["GTECH_XOR4"] = lib_elem_t::create_lib_elem("GTECH_XOR4", false, 4, 1, xor4, "A", "B", "C", "D", "Z"); 
        blocks["GTECH_XOR5"] = lib_elem_t::create_lib_elem("GTECH_XOR5", false, 5, 1, xor5, "A", "B", "C", "D", "E", "Z"); 

        // xnor
        blocks["GTECH_XNOR2"] = lib_elem_t::create_lib_elem("GTECH_XNOR2", false, 2, 1, xnor2, "A", "B", "Z"); 
        blocks["GTECH_XNOR3"] = lib_elem_t::create_lib_elem("GTECH_XNOR3", false, 3, 1, xnor3, "A", "B", "C", "Z"); 
        blocks["GTECH_XNOR4"] = lib_elem_t::create_lib_elem("GTECH_XNOR4", false, 4, 1, xnor4, "A", "B", "C", "D", "Z"); 
        blocks["GTECH_XNOR5"] = lib_elem_t::create_lib_elem("GTECH_XNOR5", false, 5, 1, xnor5, "A", "B", "C", "D", "E", "Z"); 

        // and-not and or-not
        blocks["GTECH_AND_NOT"] = lib_elem_t::create_lib_elem("GTECH_AND_NOT", false, 2, 1, and_not, "A", "B", "Z");
        blocks["GTECH_OR_NOT"] = lib_elem_t::create_lib_elem("GTECH_OR_NOT", false, 2, 1, or_not, "A", "B", "Z");

        // and-or
        blocks["GTECH_AO21"] = lib_elem_t::create_lib_elem("GTECH_AO21", false, 3, 1, ao21, "A", "B", "C", "Z");
        blocks["GTECH_AO22"] = lib_elem_t::create_lib_elem("GTECH_AO22", false, 4, 1, ao22, "A", "B", "C", "D", "Z");
        blocks["GTECH_AOI21"] = lib_elem_t::create_lib_elem("GTECH_AOI21", false, 3, 1, aoi21, "A", "B", "C", "Z");
        blocks["GTECH_AOI22"] = lib_elem_t::create_lib_elem("GTECH_AOI22", false, 4, 1, aoi22, "A", "B", "C", "D", "Z");
        blocks["GTECH_AOI2N2"] = lib_elem_t::create_lib_elem("GTECH_AOI2N2", false, 4, 1, aoi2n2, "A", "B", "C", "D", "Z");
        blocks["GTECH_AOI222"] = lib_elem_t::create_lib_elem("GTECH_AOI222", false, 6, 1, aoi222, "A", "B", "C", "D", "E", "F", "Z");

        // or-and
        blocks["GTECH_OA21"] = lib_elem_t::create_lib_elem("GTECH_OA21", false, 3, 1, oa21, "A", "B", "C", "Z");
        blocks["GTECH_OA22"] = lib_elem_t::create_lib_elem("GTECH_OA22", false, 4, 1, oa22, "A", "B", "C", "D", "Z");
        blocks["GTECH_OAI21"] = lib_elem_t::create_lib_elem("GTECH_OAI21", false, 3, 1, oai21, "A", "B", "C", "Z");
        blocks["GTECH_OAI22"] = lib_elem_t::create_lib_elem("GTECH_OAI22", false, 4, 1, oai22, "A", "B", "C", "D", "Z");
        blocks["GTECH_OAI2N2"] = lib_elem_t::create_lib_elem("GTECH_OAI2N2", false, 4, 1, oai2n2, "A", "B", "C", "D", "Z");

        // majority 2 of 3
        blocks["GTECH_MAJ23"] = lib_elem_t::create_lib_elem("GTECH_MAJ23", false, 3, 1, maj23, "A", "B", "C", "Z"); 
        // mux21
        blocks["GTECH_MUX2"] = lib_elem_t::create_lib_elem("GTECH_MUX2", false, 3, 1, mux2, "A", "B", "S", "Z"); 
        blocks["GTECH_MUX2I"] = lib_elem_t::create_lib_elem("GTECH_MUX2I", false, 3, 1, mux2i, "A", "B", "S", "Z"); 
        // mux42
        blocks["GTECH_MUX4"] = lib_elem_t::create_lib_elem("GTECH_MUX4", false, 6, 1, mux2i, "D0", "D1", "D2", "D3", "A", "B", "Z"); 
        // inv
        blocks["GTECH_NOT"] = lib_elem_t::create_lib_elem("GTECH_NOT", false, 1, 1, inv, "A", "Z"); 
        blocks["IV"] = lib_elem_t::create_lib_elem("IV", false, 1, 1, inv, "A", "Z"); 
        // one
        blocks["GTECH_ONE"] = lib_elem_t::create_lib_elem("GTECH_ONE", false, 0, 1, one, "Z"); 
        // fake one.
        blocks["ONE"] = lib_elem_t::create_lib_elem("ONE", false, 0, 1, one, "Z"); 
        // zero
        blocks["GTECH_ZERO"] = lib_elem_t::create_lib_elem("GTECH_ZERO", false, 0, 1, zero, "Z"); 
        //fake zero
        blocks["ZERO"] = lib_elem_t::create_lib_elem("ZERO", false, 0, 1, zero, "Z"); 
        // buffer.
        blocks["BUF"] = lib_elem_t::create_lib_elem("BUF", false, 1, 1, buf, "A", "Z");
        // DFFQ
        blocks["DFFQ"] = lib_elem_t::create_lib_elem("DFFQ", true, 6, 1, invalid, "next_state", "clocked_on", "force_00", "force_01", "force_10", "force_11", "Q");
        // DFFQN
        blocks["DFFQN"] = lib_elem_t::create_lib_elem("DFFQN", true, 6, 1, invalid, "next_state", "clocked_on", "force_00", "force_01", "force_10", "force_11", "QN");
    }

    void lib_gtech_t::preprocess(module_list_t& modules)
    {
        for(unsigned i=0; i != modules.size(); i++) {
            preprocess_module(modules[i]);
        }
    }

    void lib_gtech_t::preprocess_module(module_t* mod)
    {
        // first we want to bind 1 and zero to some wires.
        bind_onezero(mod);

        for(unsigned i=0; i != mod->stms->size(); i++) {
            stm_t* s = (*mod->stms)[i];
            if(s->type == stm_t::MODULE_INST) {
                module_inst_t* mi = s->module_inst;
                process_module_inst(mod, mi);
            } else if(s->type == stm_t::ASSGN) {
                assgn_t* assgn = s->assgn;
                process_assign(assgn);
            }
        }
    }

    void lib_gtech_t::bind_onezero(module_t* mod)
    {
        // declare the wire '#one'.
        wirename_t w1decl("#one");
        wire_list_t *w1_decl_list = new wire_list_t(1, w1decl);
        stm_t* s1decl = new stm_t(stm_t::WIRE, w1_decl_list);
        mod->stms->push_back(s1decl);

        // assign 1 to it.
        wirename_t* w_tiehi = new wirename_t("ONE");
        wirename_t* w_tiehi_inst = new wirename_t("#mod_inst_ONE");
        wirename_t w_tiehi_z("Z");
        wirename_t *w1 = new wirename_t("#one");
        exp_t* e1 = new exp_t(w1);
        wirebinding_t wb1(w_tiehi_z, e1);
        bindinglist_t *b1 = new bindinglist_t(1, wb1);
        module_inst_t* m1 = new module_inst_t(w_tiehi, w_tiehi_inst, b1);
        stm_t* s1 = new stm_t(m1);
        mod->stms->push_back(s1);

        // declare the wire '#zero'.
        wirename_t w0decl("#zero");
        wire_list_t *w0_decl_list = new wire_list_t(1, w0decl);
        stm_t* s0decl = new stm_t(stm_t::WIRE, w0_decl_list);
        mod->stms->push_back(s0decl);

        // assign 1 to it.
        wirename_t* w_tielo = new wirename_t("ZERO");
        wirename_t* w_tielo_inst = new wirename_t("#mod_inst_ZERO");
        wirename_t w_tielo_z("Z");
        wirename_t *w0 = new wirename_t("#zero");
        exp_t* e0 = new exp_t(w0);
        wirebinding_t wb0(w_tielo_z, e0);
        bindinglist_t *b0 = new bindinglist_t(1, wb0);
        module_inst_t* m0 = new module_inst_t(w_tielo, w_tielo_inst, b0);
        stm_t* s0 = new stm_t(m0);
        mod->stms->push_back(s0);
    }

    void lib_gtech_t::process_assign(assgn_t* a)
    {
        if(a->exp->type == exp_t::CNST) {
            if(a->exp->cnst->bit_width > 1) {
                printf("ERROR: Can't handle multilbit constants yet.\n");
                exit(0);
            } else {
                int v = a->exp->cnst->value;
                delete a->exp;
                if(v == 1) {
                    a->exp = new exp_t(new wirename_t("#one"));
                } else if(v == 0) {
                    a->exp = new exp_t(new wirename_t("#zero"));
                } else {
                    assert(false);
                    printf("ERROR: Expecting 0 or 1 as constant.\n");
                    exit(0);
                }
            }
        } else if(a->exp->type == exp_t::AGGR) {
            printf("ERROR: Can't handle binding of aggregates yet.\n");
            exit(0);
        }
    }

    void lib_gtech_t::process_module_inst(module_t* mod, module_inst_t* mi)
    {
        for(unsigned i=0; i != mi->bindings->size(); i++) {
            wirebinding_t& wb = (*mi->bindings)[i];
            if(wb.actual->type == exp_t::CNST) {
                if(wb.actual->cnst->bit_width > 1) {
                    printf("ERROR: Can't handle multilbit constants yet.\n");
                    exit(0);
                } else {
                    assert(wb.actual->cnst->value == 0 ||
                           wb.actual->cnst->value == 1);
                    // delet the old binding.
                    delete wb.actual;
                    // now bind to a wire instead of a const.
                    if(wb.actual->cnst->value == 0) {
                        wb.actual = new exp_t(new wirename_t("#zero"));
                    } else {
                        wb.actual = new exp_t(new wirename_t("#one"));
                    }
                }
            } else if(wb.actual->type == exp_t::AGGR) {
                printf("ERROR: Can't handle binding of aggregates yet.\n");
                exit(0);
            }
        }
        if(strcmp(mi->module->full_name, "DFF") == 0) {
            process_dff_insts(mod, mi);
        }
    }

    void lib_gtech_t::process_dff_insts(module_t* mod, module_inst_t* mi)
    {
        assert(strcmp(mi->module->full_name, "DFF") == 0);
        bool q_fnd = false, qn_fnd = false;
        const char* q_name = NULL;
        const char* qn_name =NULL;
        for(unsigned i=0; i != mi->bindings->size(); i++) {
            wirebinding_t& wb = (*mi->bindings)[i];
            if(strcmp(wb.formal.full_name, "Q") == 0) {
                q_fnd = true;
                assert(wb.actual->type == exp_t::WIRE);
                q_name = wb.actual->wire->full_name;
            } else if(strcmp(wb.formal.full_name, "QN") == 0) {
                qn_fnd = true;
                assert(wb.actual->type == exp_t::WIRE);
                qn_name = wb.actual->wire->full_name;
            }
        }
        if(q_fnd && qn_fnd) {
            process_dff_q_qn(mod, mi, q_name, qn_name);
        } else if(q_fnd) {
            delete mi->module;
            mi->module = new wirename_t("DFFQ");
        } else if(qn_fnd) {
            delete mi->module;
            mi->module = new wirename_t("DFFQN");
        } else {
            printf("Error: DFF found with neither Q or QN.\n");
            exit(1);
            std::cout << "neither   : " << *mi << std::endl;
        }
    }

    void lib_gtech_t::process_dff_q_qn(module_t* mod, module_inst_t* mi, const char* qwire, const char* qnwire)
    {
        static int instCtr = 0;

        // first create the inverter between Q and QN.
        char instName[32];
        sprintf(instName, "#mod_inst_INV%d", instCtr++);
        wirename_t* module = new wirename_t("IV");
        wirename_t* instance = new wirename_t(instName);

        wirename_t wzf("Z");
        wirename_t *wz = new wirename_t(qnwire);
        exp_t* ez = new exp_t(wz);
        wirebinding_t wbz(wzf, ez);

        wirename_t waf("A");
        wirename_t *wa = new wirename_t(qwire);
        exp_t* ea = new exp_t(wa);
        wirebinding_t wba(waf, ea);

        bindinglist_t *ibl = new bindinglist_t();
        ibl->push_back(wba);
        ibl->push_back(wbz);

        module_inst_t* modinst = new module_inst_t(module, instance, ibl);
        stm_t* stm = new stm_t(modinst);
        mod->stms->push_back(stm);

        // now delete QN from the binding list.
        bindinglist_t* bl = new bindinglist_t;
        for(unsigned i=0; i != mi->bindings->size(); i++) {
            wirebinding_t& wb = (*mi->bindings)[i];
            if(strcmp(wb.formal.full_name, "QN") != 0) {
                bl->push_back(wirebinding_t(wb));
            }
        }
        delete mi->bindings;
        mi->bindings = bl;

        delete mi->module;
        mi->module = new wirename_t("DFFQ");
    }
}

ILibrary* createLib(std::string& name)
{
    static ILibrary* Lib12soi = NULL;
    static ILibrary* LibGtech = NULL;
    if(name == "12soi") {
	if(Lib12soi == NULL) {
		Lib12soi = new lib_12soi_n::lib_12soi_t();

		Lib12soi->init();
	}
        return Lib12soi;
    } else if(name == "gtech") {
	if(LibGtech == NULL) {
		LibGtech = new lib_gtech_n::lib_gtech_t();
		LibGtech->init();
	}
        return LibGtech;
    } else {
        printf("Error: Unknown technology library: '%s'.\n", name.c_str());
        exit(1);
    }
}
