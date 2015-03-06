#include <string>
#include <iomanip>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <queue>

#include <limits.h>
#include <boost/lexical_cast.hpp>

#include <glpk.h>

#ifdef USE_CPLEX
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN
#endif

#include "flat_module.h"
#include "node.h"
#include "aggr.h"
#include "counter.h"
#include "main.h"
#include "gae.h"
#include "shiftreg.h"
#include "vcd.h"
#include "timer.h"
#include <sys/stat.h>
#include <sys/types.h>

#include <boost/property_tree/json_parser.hpp>

int enable_propagation = 0;
Cudd flat_module_t::fullMgr; // manager for the full functions.
std::vector<Cudd*> flat_module_t::mgrs;
flat_module_t::ipp_t* flat_module_t::full_ipp;
std::vector<flat_module_t::ipp_t*> flat_module_t::ipps;

flat_module_t::flat_module_t(ILibrary* library, module_list_t* modules, int index)
  : debug(0),
    summarize_modules(0),
    lib(library),
    error(false)

{
    prune_count = 0;
    cover_count = 0;
    lcg_computed = false;

    wire_ctr = 0;
    instance_ctr = 0;

    if(index == -1) {
        assert(modules->size() == 1);
        index = 0;
    }

    // now get the main module.
    module_t* module = (*modules)[index];
    module_name = strdup(module->name);

    // make a pass through the module looking for instances.
    if(summarize_modules) {
        summarize_instances(module);
    }

    // now make one pass looking for the input, output and wire declarations.
    find_declarations(module);
    // run through the module instantiations
    if(!verify_module_inst(module)) {
        error = true;
        return;
    }
    // create the circuit graph now.
    map_t port_map;
    if(!create_wirenames(module, port_map)) {
        error = true;
        return;
    }
    if(!handle_ports(module, port_map)) {
        error = true;
        return;
    }

    if(!mark_outputs(module)) {
        error = true;
        return;
    }

    if(!create_nodeinputs(module)) {
        error = true;
        return;
    }

    create_latch_gates();

    rewrite_nodes();
    create_nodelists();
    create_fanouts();
    if(options.rewriteBuffers) {
        FILE* fp = NULL;
        if(options.rewriteLog.size() > 0) {
            fp = fopen(options.rewriteLog.c_str(), "wt");
        }
        rewriteInverters(fp);
        rewriteBuffers(fp);

        if(options.removeDeadNodes) {
            removeDeadNodes(fp);
        }

        if(fp) {
            fclose(fp);
        }
    }
    assign_indices();
    topo_sort();
    mark_scan_enables();
    create_words();
    compute_distances();
}

void flat_module_t::dumpInstanceNames(std::ostream& input_file, std::ostream& output_file, std::ostream& instance_file)
{
    std::set<std::string> inputs;
    std::set<std::string> outputs;
    std::set<std::string> instances;

    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t* n = it->second;
        if(!n->is_suppress_gate()) {
            if(!n->is_input()) {
                instances.insert(n->get_instance_name());
            } else {
                inputs.insert(n->get_name());
            }
            if(n->is_output()) {
                outputs.insert(n->get_name());
            }
        }
    }

    dump_to_file(input_file, inputs);
    dump_to_file(output_file, outputs);
    dump_to_file(instance_file, instances);
}

void flat_module_t::dump_to_file(std::ostream& out, const std::set<std::string>& set)
{
    for(std::set<std::string>::const_iterator it = set.begin(); it != set.end(); it++) {
        out << *it << std::endl;
    }
}

void flat_module_t::mark_scan_enables()
{
    std::cout << "starting mark_scan_enables ... "; std::cout.flush();
    for(unsigned i=0; i != latches.size(); i++) {
        node_t* lg = latches[i]->get_input(0);
        node_t* se = lg->get_se_input();
        if(se) {
            se->set_is_scan_enable();
        }
    }
    std::cout << "done!" << std::endl;
}

std::string flat_module_t::gen_wirename()
{
    return "$wire_" + boost::lexical_cast<std::string>(wire_ctr++);
}

std::string flat_module_t::gen_instancename()
{
    return "$instance_" + boost::lexical_cast<std::string>(instance_ctr++);
}

flat_module_t::~flat_module_t()
{
    inputBDDs.clear();
    outputBDDs.clear();
    partialFuncBDDs.clear();
    bdds.clear();

    for(unsigned i=0; i != modules.size(); i++) {
        delete modules[i];
    }
    for(unsigned i=0; i != words.size(); i++) {
        delete words[i];
    }

    for(unsigned i=0; i != libraryElements.size(); i++) {
        delete libraryElements[i];
    }

    for(unsigned i=0; i != partialFuncModules.size(); i++) {
        delete partialFuncModules[i];
    }
    for(ram_map_t::iterator it = rams.begin(); it != rams.end(); it++) {
        ramlist_t& vec = it->second;
        for(unsigned i=0; i != vec.size(); i++) {
            delete vec[i];
        }
        vec.clear();
    }
    rams.clear();

    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t *n = it->second;
        delete n;
    }
    map.clear();


    free((void*) module_name);
}

void flat_module_t::summarize_instances(module_t* module)
{
    int assign_ctr = 0;
    int counter = 0;

    for(stm_list_t::iterator i  = module->stms->begin();
            i != module->stms->end();
            i++)
    {
        stm_t* stm = *i;
        if(stm->type == stm_t::ASSGN) { assign_ctr += 1; }
        else if(stm->type == stm_t::MODULE_INST) {
            counter++;
        }
    }
    std::cout << "# of instances: " << counter << std::endl;
    std::cout << "# of assigns  : " << assign_ctr << std::endl;
}

void flat_module_t::find_declarations(module_t* module)
{
    for(stm_list_t::iterator i  = module->stms->begin();
            i != module->stms->end();
            i++)
    {
        stm_t* stm = *i;
        switch(stm->type) {
            case stm_t::INPUT:
            case stm_t::OUTPUT:
            case stm_t::WIRE:
                for(wire_list_t::iterator i  = stm->ids->begin();
                        i != stm->ids->end();
                        i++)
                {
                    wirename_t& w = *i;
                    parse_wire(stm, w);
                }
                break;
            default:
                break;
        }
    }
}

void flat_module_t::parse_wire(stm_t* stm, wirename_t& w)
{
    stbl_t::iterator pos;
    if((pos = symbols.get_symbol(w.full_name)) != symbols.end()) {
        if(pos->second.type == stm_t::WIRE &&
                (stm->type == stm_t::INPUT || stm->type == stm_t::OUTPUT))
        {
            pos->second.type = stm->type;
            if(debug) std::cout << "Reassigned: " << w.full_name << std::endl;
        } else if(pos->second.type == stm_t::INPUT &&
                stm->type == stm_t::WIRE) {
            if(debug) std::cout << "Input redeclared as wire: " << w.full_name << std::endl;
        } else if(pos->second.type == stm_t::OUTPUT &&
                stm->type == stm_t::WIRE) {
            if(debug) std::cout << "Output redeclared as wire: " << w.full_name << std::endl;
        } else {
            std::cout << "ERROR: Multiple declarations of wire? " << w.full_name << std::endl;
            assert(false);
        }
    } else {
        entry_t t;

        /* figure out the type again. :-( */
        if(stm->type == stm_t::INPUT) t.type = stm_t::INPUT;
        else if(stm->type == stm_t::OUTPUT) t.type = stm_t::OUTPUT;
        else { assert(stm->type == stm_t::WIRE); t.type = stm_t::WIRE; }

        symbols.add_symbol(w.full_name, t);
    }
}

bool flat_module_t::verify_module_inst(module_t* module)
{
    for(stm_list_t::iterator i  = module->stms->begin();
            i != module->stms->end();
            i++)
    {
        stm_t* stm = *i;
        if(stm->type == stm_t::MODULE_INST) {
            if(!verify_module_inst(stm->module_inst)) {
                error = true;
            }
        }
    }
    return !error;
}

bool flat_module_t::verify_module_inst(module_inst_t* mi)
{
    char* module = mi->module->full_name;
    lib_elem_t* libelem = lib->get_elem(module);

    if(libelem == NULL) {
        std::cerr << "ERROR: Unable to find module in library: " << module << std::endl;
        error = true;
        return false;
    }

    if(libelem->num_connections() != mi->bindings->size()) {
        std::cerr << "ERROR: Not all wires connected for: " << mi->instance->full_name << std::endl;
        error = true;
        return false;
    }

    for(bindinglist_t::iterator it = mi->bindings->begin();
            it != mi->bindings->end();
            it++)
    {

        wirebinding_t& wb = *it;
        char* formal = wb.formal.full_name;
        if(!(libelem->is_input(formal) || libelem->is_output(formal) || libelem->is_port(formal))) {
            std::cerr << "ERROR: Unknown formal parameter: " << formal << " for instance: ";
            std::cerr << mi->instance->full_name << std::endl;
            error = true;
            return false;
        }
    }
    return true;
}

bool flat_module_t::handle_ports(module_t* module, map_t& port_map)
{
    for(map_t::iterator it = port_map.begin(); it != port_map.end(); it++) {
        const std::string& name = it->first;
        node_t* node = it->second;
        (void) node;

        map_t::iterator pos = map.find(name);
        if(pos == map.end()) {
            // std::cout << "Must be a PO: " << name << std::endl;
            char buf[256]; sprintf(buf, "%s__port", node->get_instance_name().c_str());
            std::string instname(buf);

            node_t* n2 = new node_t(node_t::MACRO_OUT, name, this, NULL);
            n2->set_instance_name(instname);
            n2->add_input(node, 0);

            map[n2->get_name()] = n2;
            node->add_macro_output(n2);
        } else {
            // NOTHING TODO HERE.
        }
    }
    return true;
}

bool flat_module_t::create_wirenames(module_t* module, map_t& port_map)
{
    // the sibling map is a map between module_inst_t pointers (from the AST)
    // and node_t pointers (in the  circuit graph). we will use this to fill
    // up the sibling field of the node_t structure.
    sibling_map_t sibling_map;

    for(stm_list_t::iterator i  = module->stms->begin();
            i != module->stms->end();
            i++)
    {
        stm_t* stm = *i;
        if(stm->type == stm_t::INPUT) {
            for(wire_list_t::iterator i  = stm->ids->begin();
                    i != stm->ids->end();
                    i++)
            {
                wirename_t& w = *i;
                node_t* n = NULL;
                std::string name(w.full_name);
                n = new node_t(node_t::INPUT, name, this);
                if(map.find(n->get_name()) != map.end()) {
                    std::cerr << "ERROR: Multiple nodes named: " << n->get_name() << std::endl;
                    error = 1;
                    return false;
                }
                map[n->get_name()] = n;
            }
        } else if(stm->type == stm_t::MODULE_INST) {
            module_inst_t* mi = stm->module_inst;
            lib_elem_t *libelem = lib->get_elem(mi->module->full_name);

            // ignore elements without outputs.
            if(libelem->is_gate() || libelem->is_seq()) {
                if(libelem->num_outputs() == 0) {
                    std::cerr << "Warning: Ignoring instance " << mi->instance->full_name << std::endl;
                    continue;
                }
                bool f = create_simple_node(mi, libelem, sibling_map);
                if(!f) return false;
            } else {
                bool f = create_macro_node(mi, libelem, sibling_map, port_map);
                if(!f) return false;
            }
        } else if(stm->type == stm_t::ASSGN) {
            assgn_t* a = stm->assgn;
            std::string objname(a->obj->full_name);
            node_t *n = new node_t(node_t::GATE, objname, this, lib->get_elem("BUF"));
            if(map.find(n->get_name()) != map.end()) {
                std::cerr << "ERROR: Multiple nodes named: " << n->get_name() << std::endl;
                error = 1;
                return false;
            }
            map[n->get_name()] = n;
        }
    }
    process_siblings(sibling_map, module);
    return true;
}

bool flat_module_t::create_macro_node(module_inst_t* mi, lib_elem_t* libelem, sibling_map_t& sibling_map, map_t& port_map)
{
    assert(libelem->get_type() == lib_elem_t::MACRO);

    stringlist_t outputs;
    if(!get_outputs(mi, libelem, outputs)) return false;

    // assert the following properties.
    assert(mi->joint_module == NULL);
    assert(!mi->suppress);

    std::string instance(mi->instance->full_name);
    instance += "_macro";
    while(map.find(instance) != map.end()) { instance += "_"; }

    // create the macro node.
    node_t* n = new node_t(node_t::MACRO, instance, this, libelem);
    mi->node = n;
    map[n->get_name()] = n;

    std::string instname(mi->instance->full_name);
    n->set_instance_name(instname);

    for(unsigned i=0; i < outputs.size(); i++) {
        char buf[256]; sprintf(buf, "%s__out_%d", mi->instance->full_name, i);
        std::string instname(buf);

        node_t* n2 = new node_t(node_t::MACRO_OUT, outputs[i], this, NULL);
        n2->set_instance_name(instname);
        n2->add_input(n, 0);

        if(map.find(outputs[i]) != map.end()) {
            if(std::find(options.undrivenSignals.begin(), options.undrivenSignals.end(), outputs[i]) == options.undrivenSignals.end()) {
                std::cerr << "ERROR: Multiple drivers for: " << outputs[i] << std::endl;
                error = true;
                return false;
            } else {
                static const bool debug = false;
                if(debug) {
                    std::cerr << "Warning! Ignoring multiple drivers for: " << outputs[i] << std::endl;
                }
            }
        }
        map[n2->get_name()] = n2;
        n->add_macro_output(n2);
    }

    stringlist_t ports;
    if(!get_ports(mi, libelem, ports)) return false;
    if(ports.size() > 1) {
        std::cerr << "ERROR: Can't handle more than one port yet." << std::endl;
        error = true;
        return false;
    }
    for(unsigned i=0; i != ports.size(); i++) {
        port_map[ports[i]] = n;
    }

    return true;
}

bool flat_module_t::get_outputs(module_inst_t* mi, lib_elem_t* libelem, stringlist_t& outputs)
{
    std::map<int, std::string> index_map;

    for(bindinglist_t::iterator it =  mi->bindings->begin();
            it != mi->bindings->end();
            it++)
    {
        wirebinding_t& wb = *it;
        if(wb.actual->type != exp_t::WIRE) {
            std::cerr << "ERROR: Module port is not a simple wire." << std::endl;
            error = true;
            return false;
        }
        if(libelem->is_output(wb.formal.full_name)) {
            std::string wirename (wb.actual->wire->full_name);
            int index = libelem->get_output_index(wb.formal.full_name);
            assert(index_map.find(index) == index_map.end());
            index_map[index] = wirename;
        }
    }
    int last = 0;
    for(std::map<int, std::string>::iterator it = index_map.begin(); it != index_map.end(); it++) {
        assert(last == it->first);
        last++;

        outputs.push_back(it->second);
    }
    return true;
}

bool flat_module_t::get_ports(module_inst_t* mi, lib_elem_t* libelem, stringlist_t& ports)
{
    std::map<int, std::string> index_map;

    for(bindinglist_t::iterator it =  mi->bindings->begin();
            it != mi->bindings->end();
            it++)
    {
        wirebinding_t& wb = *it;
        if(wb.actual->type != exp_t::WIRE) {
            std::cerr << "ERROR: Module port is not a simple wire." << std::endl;
            error = true;
            return false;
        }
        if(libelem->is_port(wb.formal.full_name)) {
            std::string wirename (wb.actual->wire->full_name);
            int index = libelem->get_port_index(wb.formal.full_name);
            assert(index_map.find(index) == index_map.end());
            index_map[index] = wirename;
        }
    }
    int last = 0;
    for(std::map<int, std::string>::iterator it = index_map.begin(); it != index_map.end(); it++) {
        assert(last == it->first);
        last++;

        ports.push_back(it->second);
    }
    return true;
}

bool flat_module_t::create_simple_node(module_inst_t* mi, lib_elem_t* libelem, sibling_map_t& sibling_map)
{
    int outputs = 0;
    for(bindinglist_t::iterator it =  mi->bindings->begin();
            it != mi->bindings->end();
            it++)
    {
        wirebinding_t& wb = *it;
        if(wb.actual->type != exp_t::WIRE) {
            std::cerr << "ERROR: Module input is not a simple wire." << std::endl;
            error = true;
            return false;
        }
        if(libelem->is_output(wb.formal.full_name)) {
            std::string wirename (wb.actual->wire->full_name);
            outputs+=1;
            if(outputs == 1) {
                node_t::type_t t = node_t::get_type(libelem->get_type());
                //libelem->is_seq() ? node_t::LATCH : node_t::GATE;
                node_t *n = new node_t(t, (wirename), this, libelem);
                if(mi->joint_module != NULL) {
                    n->set_joint_module(mi->joint_module->full_name);
                }

                // add to the sibling map.
                assert(sibling_map.find(mi) == sibling_map.end());
                sibling_map[mi] = n;
                // set the suppress_gate flag.
                if(mi->suppress) {
                    n->set_suppress_gate();
                }

                if(map.find(n->get_name()) != map.end()) {
                    std::cerr << "ERROR: Multiple nodes named: " << n->get_name() << std::endl;
                    error = 1;
                    return false;
                }
                std::string instname(mi->instance->full_name);
                n->set_instance_name(instname);
                map[n->get_name()] = n;
                if(!wb.enable_output) {
                    n->set_suppress_output();
                }
            } else if(outputs > 1) {
                std::cerr << "ERROR: Can't handle gates or latches more with more than one output." << std::endl;
                error = true;
                return false;
            } else {
                // Ignore items with zero outputs.
                assert(outputs == 0);
                std::cerr << "Warning: Ignoring element with no outputs. "
                    << mi->module->full_name << std::endl;
            }
        }
    }
    return true;
}
void flat_module_t::process_siblings(flat_module_t::sibling_map_t& sibling_map, module_t* module)
{
    for(stm_list_t::iterator i  = module->stms->begin();
            i != module->stms->end();
            i++)
    {
        stm_t* stm = *i;
        if(stm->type == stm_t::MODULE_INST) {
            module_inst_t* this_mi = stm->module_inst;
            module_inst_t* sib_mi = this_mi->sibling;

            node_t* this_node = NULL;
            node_t* sib_node = NULL;
            if(this_mi->sibling != NULL) {

                assert(sibling_map.find(this_mi) != sibling_map.end());
                assert(sibling_map.find(sib_mi) != sibling_map.end());

                this_node = sibling_map[this_mi];
                sib_node = sibling_map[sib_mi];

                assert(this_node);
                assert(sib_node);

                this_node->set_sibling(sib_node);
            }
        }
    }
}

bool flat_module_t::mark_outputs(module_t* module)
{
    for(stm_list_t::iterator i  = module->stms->begin();
            i != module->stms->end();
            i++)
    {
        stm_t* stm = *i;
        if(stm->type == stm_t::OUTPUT) {
            for(wire_list_t::iterator i  = stm->ids->begin();
                    i != stm->ids->end();
                    i++)
            {
                wirename_t& w = *i;
                if(map.find(w.full_name) == map.end()) {
                    std::cerr << "WARNING: Unable to find output named: " << w.full_name << std::endl;
                } else {
                    node_t* n = map[w.full_name];
                    n->mark_output();
                }
            }
        }
    }
    return true;
}

bool flat_module_t::create_inputs(module_inst_t* mi, lib_elem_t* libelem)
{
    std::map<int,node_t*> idxmap;
    std::map<int,bool> suppress_input_map;
    node_t* gate = NULL;

    /* walk through the module inputs. */
    for(bindinglist_t::iterator it =  mi->bindings->begin(); it != mi->bindings->end(); it++)
    {
        wirebinding_t& wb = *it;
        if(wb.actual->type != exp_t::WIRE) {
            std::cerr << "ERROR: Module input is not a simple wire." << std::endl;
            error = true;
            return false;
        }
        if(libelem->is_input(wb.formal.full_name)) {
            int idx = libelem->get_input_index(wb.formal.full_name);
            char* input_name = wb.actual->wire->full_name;

            if(options.createUnknownInputs) {
                if(map.find(input_name) == map.end()) {
                    std::cerr << "WARNING: Unable to find module input: " << input_name << std::endl;
                    node_t* n = new node_t(node_t::INPUT, input_name, this);
                    map[n->get_name()] = n;
                }
            } else {
                if(map.find(input_name) == map.end()) {
                    std::cerr << "ERROR: Unable to find module input: " << input_name << std::endl;
                    error = true;
                    return false;
                }
            }
            idxmap[idx] = map[input_name];
            suppress_input_map[idx] = !wb.enable_output;
        } else {
            assert(libelem->is_output(wb.formal.full_name));
            char* output_name = wb.actual->wire->full_name;
            assert(map.find(output_name) != map.end());
            assert(gate == NULL);
            gate = map[output_name];
        }

    }

    assert(gate);
    for(std::map<int,node_t*>::iterator it  = idxmap.begin();
            it != idxmap.end();
            it++)
    {
        int idx = it->first;
        node_t* n = it->second;
        gate->add_input(n, idx);

        if(suppress_input_map[idx]) {
            gate->set_suppress_input_map(idx);
        }
    }
    return true;
}

bool flat_module_t::create_macro_inputs(module_inst_t* mi, lib_elem_t* libelem)
{
    std::map<int,node_t*> idxmap;

    node_t* node = mi->node;
    node_t* input_port = NULL;
    assert(node);
    for(bindinglist_t::iterator it =  mi->bindings->begin(); it != mi->bindings->end(); it++)
    {
        wirebinding_t& wb = *it;
        if(libelem->is_input(wb.formal.full_name)) {
            int idx = libelem->get_input_index(wb.formal.full_name);
            char* input_name = wb.actual->wire->full_name;

            if(options.createUnknownInputs) {
                if(map.find(input_name) == map.end()) {
                    std::cerr << "WARNING: Unable to find module input: " << input_name << std::endl;
                    node_t* n = new node_t(node_t::INPUT, input_name, this);
                    map[n->get_name()] = n;
                }
            } else {
                if(map.find(input_name) == map.end()) {
                    std::cerr << "ERROR: Unable to find module input: " << input_name << std::endl;
                    error = true;
                    return false;
                }
            }
            idxmap[idx] = map[input_name];
            assert(wb.enable_output); // can't disable outputs for macros!
        } else if(libelem->is_port(wb.formal.full_name)) {
            char* port_name = wb.actual->wire->full_name;
            map_t::iterator it = map.find(port_name);
            assert(it != map.end());

            if(!node->is_macro_output(it->second)) {
                input_port = it->second;
            }
        }
    }
    for(std::map<int,node_t*>::iterator it  = idxmap.begin(); it != idxmap.end(); it++)
    {
        int idx = it->first;
        node_t* n = it->second;
        node->add_input(n, idx);
    }

    if(input_port != NULL) {
        node->add_input(input_port, node->num_inputs());
    }

    return true;
}

bool flat_module_t::create_nodeinputs(module_t* module)
{
    std::cout << "inside create_nodeinputs." << std::endl;

    for(stm_list_t::iterator i  = module->stms->begin();
            i != module->stms->end();
            i++)
    {
        stm_t* stm = *i;
        if(stm->type == stm_t::MODULE_INST) {
            module_inst_t* mi = stm->module_inst;
            lib_elem_t *libelem = lib->get_elem(mi->module->full_name);
            if(libelem->num_outputs() == 0) continue;

            if(libelem->is_gate() || libelem->is_seq()) {
                if(!create_inputs(mi, libelem)) {
                    return false;
                }
            } else {
                if(!create_macro_inputs(mi, libelem)) {
                    return false;
                }
            }
        } else if(stm->type == stm_t::ASSGN) {
            assgn_t* a = stm->assgn;
            if(a->exp->type != exp_t::WIRE) {
                std::cerr << "ERROR: assignment to something more than just a wire." << std::endl;
                std::cerr << "statement: " << *stm << std::endl;
                error = true;
                return false;
            }
            char* lhsname = a->obj->full_name;
            char* rhsname = a->exp->wire->full_name;
            if(map.find(rhsname) == map.end()) {
                std::cerr << "ERROR: Unable to find assignment expression: " << rhsname << std::endl;
            }
            assert(map.find(lhsname) != map.end());
            node_t* n = map[lhsname];
            node_t* i = map[rhsname];
            n->add_input(i, 0);
        }
    }
    return true;
}

void flat_module_t::dump_verilog(std::ostream& out, std::ostream& libout)
{
    decl_list_t decls;
    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t* n = it->second;
        if(n->get_type() == node_t::INPUT) {
            decls.add_input(n->get_name().c_str());
        } else if(n->is_output()) {
            decls.add_output(n->get_name().c_str());
        } else {
            if(!n->is_latch_gate() && !n->is_macro() && !n->is_dead()) {
                decls.add_wire(n->get_name().c_str());
            }
        }
    }
    out << "module " << module_name << " ";
    decls.dump_verilog(out);

    for(wordlist_t::iterator it = words.begin(); it != words.end(); it++) {
        word_t* w = *it;
        out << "  // sz:" << w->size() << ": " << *w << std::endl;
    }

    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t* n = it->second;
        if(n->get_type() != node_t::INPUT) {
            if(options.dumpGateComments &&
                    !n->is_suppress_gate() &&
                    !n->is_dead() &&
                    (n->is_macro() || (n->is_gate() && !n->is_latch_gate()) || n->is_latch()) )
            {
                n->dump_verilog_comment(out);
            }
            if(!n->is_dead()) {
                n->dump_verilog_defn(out, true, false, NULL, NULL);
            }
        }
    }
    verilog_lib_t vlib;
    // dump the modules.
    for(unsigned i=0; i != modules.size(); i++) {
        aggr::id_module_t* m = modules[i];
        if(!m->is_dominated() && !m->is_marked_bad()) {
            m->dump_verilog(out, libout, &vlib);
        } else {
            out << "  // skipping: " << m->get_name() << "; type: " << m->get_type();
            if(m->is_dominated() && !m->is_marked_bad()) {
                out << "; dominated by: " << m->get_dominator()->get_name();
            }
            if(m->is_marked_bad()) {
                out << "; marked bad";
            }
            out << std::endl;
        }
    }
    vlib.dump(libout);
    out << "endmodule" << std::endl;
}

namespace {
    bool compare_modules_by_input(const aggr::id_module_t* const i, const aggr::id_module_t* const j)
    {
        int m1 = i->count_noninferred_gates(); // -((int)i->num_internals());
        int m2 = j->count_noninferred_gates(); // -((int)j->num_internals());
        int i1 = (i->num_word_inputs());
        int i2 = (j->num_word_inputs());
        int n1 = (i->num_inputs() + i->num_outputs()) / 4;
        int n2 = (j->num_inputs() + j->num_outputs()) / 4;

        // compare by number of word inputs.
        if(i1 < i2) return true;
        else if(i1 > i2) return false;

        // compare by number of unknown gates.
        if(m1 > m2) return true;
        else if(m1 < m2) return false;

        // compare by number of bit inputs.
        if (n1 < n2) return true;
        else if(n1 > n2) return false;

        return false;
    }
}

void flat_module_t::dump_unknown_modules(std::ostream& out)
{
    std::sort(modules.begin(), modules.end(), compare_modules_by_input);
    std::cout << "Completed sorting unknown modules." << std::endl;
    for(unsigned i=0; i != modules.size(); i++) {
        verilog_lib_t vlib;
        aggr::id_module_t* m = modules[i];
        if(!m->is_marked_bad() && (m->candidate() || m->userDefined()))
        {
            std::ofstream fout("bsim.tmp");
            m->dump_verilog(fout, out, &vlib);
        }
        fprintf(stderr, "PROCESSING module %7d/%7d\r", i+1, (int)modules.size());
        vlib.dump(out);
    }
    printf("\n");
}

void verilog_lib_t::dump(std::ostream& out)
{
    char* buf = (char*) malloc(16384);
    for(std::set<int>::iterator it = mux21.begin(); it != mux21.end(); it++) {
        int n = *it;
        sprintf(buf,
                "module mux21_%d_casio1b (s, a, b, y);\n"
                "  input [%d:0] a;\n"
                "  input [%d:0] b;\n"
                "  input s;\n"
                "  output [%d:0] y;\n"
                "  assign y = s ? b : a;\n"
                "endmodule\n", n, n-1, n-1, n-1);
        out << buf << std::endl;
    }
    for(std::set<int>::iterator it = mux21i.begin(); it != mux21i.end(); it++) {
        int n = *it;
        sprintf(buf,
                "module mux21i_%d_casio1b (s, a, b, y);\n"
                "  input [%d:0] a;\n"
                "  input [%d:0] b;\n"
                "  input s;\n"
                "  output [%d:0] y;\n"
                "  assign y = s ? ~b : ~a;\n"
                "endmodule\n", n, n-1, n-1, n-1);
        out << buf << std::endl;
    }
    for(std::set<int>::iterator it = mux41.begin(); it != mux41.end(); it++) {
        int n = *it;
        sprintf(buf,
                "module mux41_%d_casio1b (s0, s1, a, b, c, d, y);\n"
                "  input [%d:0] a;\n"
                "  input [%d:0] b;\n"
                "  input [%d:0] c;\n"
                "  input [%d:0] d;\n"
                "  input s0;\n"
                "  input s1;\n"
                "  wire [1:0] sel = { s1, s0 };\n"
                "  output [%d:0] y;\n"
                "  assign y = sel == 2'b00 ? a\n"
                "           : sel == 2'b01 ? b\n"
                "           : sel == 2'b10 ? c\n"
                "           : d;\n"
                "endmodule\n", n, n-1, n-1, n-1, n-1, n-1);
        out << buf << std::endl;
    }
    for(std::set<int>::iterator it = mux41i.begin(); it != mux41i.end(); it++) {
        int n = *it;
        sprintf(buf,
                "module mux41i_%d_casio1b (s0, s1, a, b, c, d, y);\n"
                "  input [%d:0] a;\n"
                "  input [%d:0] b;\n"
                "  input [%d:0] c;\n"
                "  input [%d:0] d;\n"
                "  input s0;\n"
                "  input s1;\n"
                "  wire [1:0] sel = { s1, s0 };\n"
                "  output [%d:0] y;\n"
                "  assign y = sel == 2'b00 ? ~a\n"
                "           : sel == 2'b01 ? ~b\n"
                "           : sel == 2'b10 ? ~c\n"
                "           : ~d;\n"
                "endmodule\n", n, n-1, n-1, n-1, n-1, n-1);
        out << buf << std::endl;
    }
    free(buf);
}

void flat_module_t::dump_nodes(std::ostream& out)
{
    for(map_t::iterator it =  map.begin();
            it != map.end();
            it++)
    {
        node_t* n = it->second;
        if(n->get_type() == node_t::INPUT)
            n->dump(out);
    }
    for(map_t::iterator it =  map.begin();
            it != map.end();
            it++)
    {
        node_t* n = it->second;
        if(n->get_type() == node_t::LATCH)
            n->dump(out);
    }
    for(map_t::iterator it =  map.begin();
            it != map.end();
            it++)
    {
        node_t* n = it->second;
        if(n->get_type() == node_t::GATE)
            n->dump(out);
    }
    out << std::endl;
    for(unsigned i=0; i != modules.size(); i++) {
        aggr::id_module_t* m = modules[i];
        if(!m->is_dominated() && !m->is_marked_bad()) {
            out << *m << std::endl;
        }
    }
    out << std::endl;
    for(unsigned i=0; i != words.size(); i++) {
        word_t* w = words[i];
        out << ".word " << *w << std::endl;
    }
}

void flat_module_t::create_latch_gates()
{
    std::cout << "inside create_latch_gates" << std::endl;
    std::list<node_t*> new_nodes;
    for(map_t::iterator it =  map.begin();
            it != map.end();
            it++)
    {
        node_t* n = it->second;
        if(n->is_latch()) {
            create_latch_gate(n, new_nodes);
        }
    }

    for(std::list<node_t*>::iterator it = new_nodes.begin();
            it != new_nodes.end();
            it++)
    {
        node_t* n = *it;
        assert(map.find(n->get_name()) == map.end());
        map[n->get_name()] = n;
    }
}

void flat_module_t::create_latch_gate(node_t* n, std::list<node_t*>& new_nodes)
{
    std::string wirename = gen_wirename();
    lib_elem_t* le = n->get_lib_elem();

    std::string gate_name = le->get_name() + std::string(".GATE");
    lib_elem_t* gate_elem = lib->get_elem(gate_name.c_str());
    lib_elem_t* ff_elem = lib->get_replacement(le);
    assert(ff_elem);

    assert(gate_elem);
    assert(ff_elem);

    // first create the gate.
    node_t* gg = new node_t(node_t::GATE, wirename, this, gate_elem);

    assert(n->num_inputs() >= 2);
    for(unsigned i=0; i != n->num_inputs()-1; i++) {
        node_t* inp = n->get_input(i);
        gg->add_input(inp, i);
    }
    assert(gg->num_inputs() == gate_elem->num_inputs());
    gg->set_latch_gate();
    gg->set_suppress_gate();
    gg->set_suppress_output();

    // now create the flip flop.
    n->morph_latch(gg, ff_elem);
    n->set_sibling(gg);
    n->set_suppress_input_map(0); // i.e., the D-input which comes from the sibling.
    n->set_joint_module(le->get_name());

    // make sure the names match and the temp name isn't already in the map.
    assert(map.find(gg->get_name()) == map.end());

    // replace the node.
    new_nodes.push_back(gg);
}

void flat_module_t::replace_inputs(node_t* this_node, node_t* old_input, node_t* new_input)
{
    for(unsigned i=0; i != this_node->num_inputs(); i++) {
        node_t* inp = this_node->get_input(i);
        if(inp == old_input) {
            this_node->set_input(i, new_input);
        }
    }
}

void flat_module_t::create_nodelists()
{
    std::cout << "inside create_nodelists." << std::endl;

    real_gates = 0;
    int total = map.size();
    int pos = 1;
    for(map_t::iterator it  = map.begin();
            it != map.end();
            it++)
    {
        node_t* n = it->second;
        if(n->get_type() == node_t::INPUT) {
            inputs.push_back(n);
        } else if(n->get_type() == node_t::GATE) {
            if(!n->is_latch_gate()) real_gates += 1;
            gates.push_back(n);
        } else if(n->get_type() == node_t::LATCH) {
            latches.push_back(n);
        } else if(n->get_type() == node_t::MACRO) {
            macros.push_back(n);
        } else if(n->get_type() == node_t::MACRO_OUT) {
            macro_outs.push_back(n);
        }

        if(n->is_output()) {
            outputs.push_back(n);
        }
        (void)pos;
        (void)total;
        //printf("PROGRESS: %7d/%7d\r", pos++, total);
        //fflush(stdout);
    }
    //printf("\n");
}

void flat_module_t::color_analysis()
{
    static const int IN = 0, OUT = 1;

    //std::cout << "starting color analysis." << std::endl;
    std::queue<col_t> bfsq;
    if(options.pinGroups.size() >= 31) {
        std::cerr << "ERROR: Too many pin groups!" << std::endl;
        return;
    }

    // convert the list of list of strings to queue entries.
    for(unsigned i=0; i != options.pinGroups.size(); i++) {
        stringlist_t& strList = options.pinGroups[i];
        int color = (1 << i);
        for(unsigned j=0; j != strList.size(); j++) {
            std::string& s = strList[j];
            node_t* n_pad = get_node_by_name(s.c_str());
            if(n_pad == NULL) {
                std::cerr << "ERROR: Unable to find node: " << s << std::endl;
                return;
            }
            if(n_pad->is_output() || n_pad->is_input()) {
                int dir = -1;
                if(n_pad->is_input()) {
                    dir = IN;
                } else {
                    assert(n_pad->is_output());
                    dir = OUT;
                }

                node_t* n = get_pad_port(n_pad);
                assert(n != NULL);
                col_t np(n, color,  dir);
                bfsq.push(np);
            } else {
                std::cerr << "ERROR: Not an input/output: " << s << std::endl;
                return;
            }
        }
    }

    typedef std::map<int, int> cntmap_t;
    cntmap_t counts;

    while(!bfsq.empty()) {
        col_t en = bfsq.front();
        bfsq.pop();

        if(en.n->in_logic_loop()) continue;

        en.n->add_color(en.col);
        counts[en.col] += 1;
        if(en.dir == IN) {
            for(node_t::fanout_iterator it = en.n->fanouts_begin(); it != en.n->fanouts_end(); it++) {
                node_t* fnout = *it;
                if(!fnout->is_colored()) {
                    col_t new_en(fnout, en.col, IN);
                    bfsq.push(new_en);
                }
            }
        } else {
            assert(en.dir == OUT);
            for(node_t::input_iterator it = en.n->inputs_begin(); it != en.n->inputs_end(); it++) {
                node_t* inp = *it;
                if(!inp->is_colored()) {
                    col_t new_en(inp, en.col, IN);
                    bfsq.push(new_en);
                }
            }
        }
    }
    create_color_modules();
}

void flat_module_t::pinBackProp()
{
    typedef std::pair<node_t*, int> qen_t;

    for(unsigned i=0; i != options.backProps.size(); i++) {
        std::queue<qen_t> bfsq;
        stringlist_t& strList = options.backProps[i];
        for(unsigned j=0; j != strList.size(); j++) {
            std::string& s = strList[j];
            node_t* n_pad = get_node_by_name(s.c_str());
            if(!n_pad->is_input()) {
                std::cout << s << ": is not an input." << std::endl;
                return;
            }
            node_t* n = get_pad_port(n_pad);
            assert(n != NULL);
            qen_t en(n, 0);
            bfsq.push(en);
        }
        backProp(bfsq);
    }
}

void flat_module_t::backProp(std::queue< std::pair<node_t*, int> >& bfsq)
{
    typedef std::pair<node_t*, int> qen_t;
    int prev_level = -1;
    while(!bfsq.empty()) {
        qen_t en = bfsq.front();
        bfsq.pop();

        node_t* node = en.first;
        int level = en.second;
        if(level < options.backPropLevels) {
            if(level != prev_level) {
                std::cout << "Level " << level << std::endl << std::endl;
                prev_level = level;
            }
            if(node->get_lib_elem()) {
                node->dump(std::cout);
            }

            for(node_t::fanout_iterator fit = node->fanouts_begin(); fit != node->fanouts_end(); fit++) {
                node_t* n = *fit;
                qen_t new_en(n, level+1);
                bfsq.push(new_en);
            }
        }
    }
}

int flat_module_t::get_color(int c)
{
    assert((c&(c-1)) == 0);
    for(int i = 0; i <= 32; i++) {
        if((1<<i) == c) return i;
    }
    assert(false);
    return -1;
}

void flat_module_t::create_color_modules()
{
    int numColors = options.pinGroups.size();
    std::vector<nodeset_t> nodes(numColors);
    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        int col = (it->second)->is_colored();
        if(col != 0 && ((col&(col-1)) == 0)) {
            int cidx = get_color(col);
            assert(cidx < (int)nodes.size());
            nodes[cidx].insert(it->second);
        }
    }
    for(int i=0; i != numColors; i++) {
        // char buf[256];
        // sprintf(buf, "color%d", i);
        // create_module(buf, nodes[i]);
    }
}

void flat_module_t::count_unate_vars()
{
    using namespace sat_n;
    using namespace Minisat;
    for(unsigned i=0; i != outputs.size(); i++) {
        node_t* n = outputs[i];
        if(n->is_gate()) {
            const nodeset_t& cone = n->get_fanin_cone();
            const nodeset_t& cone_inputs = n->get_fanin_cone_inputs();

            for(nodeset_t::const_iterator it = cone_inputs.begin(); it != cone_inputs.end(); it++) {
                node_t* inp = *it;

                cofactor_map_t cf;
                satchecker_t sat;

                cf[inp] = 0;
                Lit a = sat.addFunc(n, cone, cf, NULL, NULL);

                cf[inp] = 1;
                Lit b = sat.addFunc(n, cone, cf, NULL, NULL);

                Lit y = sat.getNewLit();
                sat.addClause(a, y);
                sat.addClause(~b, y);
                sat.addClause(~a, b, ~y);

                Lit z = sat.getNewLit();
                sat.addClause(b, z);
                sat.addClause(~a, z);
                sat.addClause(~b, a, ~z);

                if(!sat.isSAT(~y) || !sat.isSAT(~z)) {
                    std::cout << "unate: " << inp->get_name() << std::endl;
                }
            }
        }
    }
}

aggr::id_module_t* flat_module_t::create_module(const char* name, const nodeset_t& gates)
{
    nodeset_t inputs;
    nodeset_t outputs;
    for(nodeset_t::iterator it = gates.begin(); it != gates.end(); it++) {
        node_t* n = *it;
        for(node_t::input_iterator jt = n->inputs_begin(); jt != n->inputs_end(); jt++) {
            node_t* inp = *jt;
            if(gates.find(inp) == gates.end()) {
                inputs.insert(inp);
            }
        }
        bool all_fanouts_used = true;
        for(node_t::fanout_iterator jt = n->fanouts_begin(); jt != n->fanouts_end(); jt++) {
            node_t* fout = *jt;
            if(gates.find(fout) == gates.end()) {
                all_fanouts_used = false;
                break;
            }
        }
        if(!all_fanouts_used) {
            // assert(!n->is_latch_gate());
            if(!n->is_latch_gate()) {
                outputs.insert(n);
            }
        }
    }

    using namespace aggr;
    id_module_t* mod = new id_module_t(name, id_module_t::UNSLICEABLE, id_module_t::INFERRED);
    for(nodeset_t::iterator it = outputs.begin(); it != outputs.end(); it++) {
        mod->add_output(*it);
    }
    for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
        mod->add_input(*it);
    }
    mod->compute_internals();
    add_module(mod);
    return mod;
}

void flat_module_t::mark_tree(node_t* clk, int clk_index, marking_t type)
{
    // make sure the index is a power of two.
    assert((clk_index & (clk_index-1)) == 0);
    using namespace std;
    queue<node_t*> q;
    q.push(clk);
    nodeset_t visited;
    while(!q.empty()) {
        node_t* n = q.front();
        if(type == MARK_CLKTREE) {
            n->add_clktree(clk_index);
            n->set_inclktree();
        } else if(type == MARK_RSTTREE) {
            n->add_rsttree(clk_index);
            n->set_inrsttree();
        } else assert(false);
        q.pop();

        for(node_t::fanout_iterator it = n->fanouts_begin(); it != n->fanouts_end(); it++) {
            node_t* fout = *it;

            if(fout->is_gate() && !fout->is_latch_gate()) {
                if(visited.find(fout) == visited.end()) {
                    visited.insert(fout);
                    q.push(fout);
                }
            }
            if(fout->is_latch() || fout->is_macro()) {
                if(type == MARK_CLKTREE) {
                    fout->add_clktree(clk_index);
                } else if(type == MARK_RSTTREE) {
                    fout->add_rsttree(clk_index);
                } else assert(false);
            }
            if(fout->is_latch_gate()) {
                if(type == MARK_RSTTREE) {
                    fout->add_rsttree(clk_index);
                    assert(fout->num_fanouts() == 1);
                    node_t* latch = *fout->fanouts_begin();
                    latch->add_rsttree(clk_index);
                }
            }
        }
    }

}

void flat_module_t::summarize_tree_info(std::ostream& out)
{
    std::map<int, int> gate_clk_cnt, gate_rst_cnt;
    std::map<int, int> latch_clk_cnt, latch_rst_cnt;
    std::map<int, int> macro_clk_cnt, macro_rst_cnt;

    count_tree_info(gates, gate_clk_cnt, gate_rst_cnt);
    dump_tree_info(gate_clk_cnt, gate_rst_cnt, "gates", out);

    count_tree_info(latches, latch_clk_cnt, latch_rst_cnt);
    dump_tree_info(latch_clk_cnt, latch_rst_cnt, "latches", out);

    count_tree_info(macros, macro_clk_cnt, macro_rst_cnt);
    dump_tree_info(macro_clk_cnt, macro_rst_cnt, "macros", out);
}

void flat_module_t::propagateTreeMarks()
{
    for(unsigned i=0; i != gates.size(); i++) {
        node_t* g = gates[i];
        int clk = 0, rst = 0;
        for(node_t::input_iterator it = g->inputs_begin(); it != g->inputs_end(); it++) {
            node_t* inp = *it;
            clk = clk | inp->get_clktrees();
            rst = rst | inp->get_rsttrees();
        }
        if(g->get_clktrees() == 0) {
            g->set_clktrees(clk);
        }
        if(g->get_rsttrees() == 0) {
            g->set_rsttrees(rst);
        }
    }

    // propagate backwards.
    for(int i = gates.size()-1; i >= 0; i--) {
        node_t* g = gates[i];
        int c = 0;
        for(node_t::fanout_iterator it = g->fanouts_begin(); it != g->fanouts_end(); it++) {
            node_t* f = *it;
            c = c | f->get_rsttrees() | f->get_rst2();
        }
        g->set_rst2(c);
    }
    // now to the latches!
    for(unsigned i = 0; i != latches.size(); i++) {
        node_t* l = latches[i];
        int c = 0;
        for(node_t::fanout_iterator it = l->fanouts_begin(); it != l->fanouts_end(); it++) {
            node_t* f = *it;
            c = c | f->get_rsttrees() | f->get_rst2();
        }
        l->set_rst2(c);
    }

    for(unsigned i=0; i != macros.size(); i++) {
        node_t* n = macros[i];
        if(strcmp("RF2TCSG0064X032D1", n->get_lib_elem()->get_name()) == 0) {
            int rst = 0;
            for(node_t::input_iterator it = n->inputs_begin(); it != n->inputs_end(); it++) {
                node_t* inp = *it;
                rst = rst | (inp->get_rsttrees() | inp->get_rst2());
            }
            n->set_rsttrees(rst);
        }
    }

    /*
       std::map<node_t*, int> nearest_inp;
       std::map<node_t*, int> nearest_out;
       for(unsigned i=0; i != gates.size(); i++) {
       node_t* n = gates[i];
       if(n->get_rsttrees() == 64 || n->get_rsttrees() == 65) {
       node_t* ni = n->get_nearest_input();
       node_t* no = n->get_nearest_output();
       nearest_inp[ni] += 1;
       nearest_out[no] += 1;
       }
       }

       std::cout << "nearest inputs to UART: " << std::endl;
       dump_node_int_map(std::cout, nearest_inp);
       std::cout << "nearest outputs to UART: " << std::endl;
       dump_node_int_map(std::cout, nearest_out);
       */
}

void flat_module_t::dump_node_int_map(std::ostream& out, const std::map<node_t*, int>& m)
{
    for(std::map<node_t*, int>::const_iterator it = m.begin(); it != m.end(); it++) {
        if(it->first) {
            std::cout << std::setw(20) << it->first->get_name() << " : " << std::setw(6) << it->second << std::endl;
        }
    }
}

void flat_module_t::dump_node_int_map2(std::ostream& out, const std::map<node_t*, int>& m)
{
    for(std::map<node_t*, int>::const_iterator it = m.begin(); it != m.end(); it++) {
        if(it->first) {
            std::cout << std::setw(20) << it->first->get_name() << " : " << std::setw(6) << it->second << "; ";
            node_t::dump_masked_strings(std::cout, it->first->get_rst2(), options.resetTreeRoots);
            std::cout << std::endl;
        }
    }
}

void flat_module_t::createModuleFromResetTree(int color, std::string& name)
{
    color = 1 << color;
    int base_color = 1;
    int offset = 1;

    nodeset_t internals;

#if 1
    for(unsigned i=0; i != latches.size(); i++) {
        node_t* l = latches[i];
        int rst = l->get_rsttrees();
        if(rst == color || rst == (color+offset) || (options.bigModules && rst == base_color)) {
            internals.insert(l);
        }
    }
    for(unsigned i=0; i != gates.size(); i++) {
        node_t* g = gates[i];
        int rst = g->get_rsttrees();
        if(rst == color || rst == (color+offset) || (options.bigModules && rst == base_color)) {
            internals.insert(g);
        }
    }
    for(unsigned i=0; i != macros.size(); i++) {
        node_t* n = macros[i];
        if(strcmp("RF2TCSG0064X032D1", n->get_lib_elem()->get_name()) == 0) {
            int rst = n->get_rsttrees();
            if(rst == color || rst == (color+offset) || (options.bigModules && rst == base_color)) {
                internals.insert(n);
                for(unsigned j=0; j != n->num_macro_outputs(); j++) {
                    internals.insert(n->get_macro_output(j));
                }
            }
        }
    }
#endif

    std::cout << "creating module: " << name << std::endl;
    std::cout << "# of internals: " << internals.size() << std::endl;
    nodeset_t new_latches;
    nodeset_t del_latches;
    for(nodeset_t::iterator it = internals.begin(); it != internals.end(); it++) {
        node_t* n = *it;
        if(n->is_latch_gate()) {
            assert(n->num_fanouts() == 1);
            node_t* l = *n->fanouts_begin();
            if(internals.find(l) == internals.end()) {
                bool del = true;
                for(node_t::fanout_iterator fit = l->fanouts_begin(); fit != l->fanouts_end(); fit++) {
                    node_t* fout = *fit;
                    int rst2 = fout->get_rsttrees();
                    if(rst2 == color || (color+offset) == rst2) {
                        del = false;
                        new_latches.insert(l);
                        break;
                    }
                }
                if(del) {
                    del_latches.insert(n);
                }
            }
        }
    }
    std::copy(new_latches.begin(), new_latches.end(), std::inserter(internals, internals.end()));
    std::cout << "# of internals after augmentation: " << internals.size() << std::endl;
    std::cout << "# of latch gates to remove: " << del_latches.size() << std::endl;
    for(nodeset_t::iterator it = del_latches.begin(); it != del_latches.end(); it++) {
        node_t* n = *it;
        internals.erase(n);
    }
    std::cout << "# of internals after deletion: " << internals.size() << std::endl;

    // expand downwards towards the latch inputs.
    nodeset_t new_nodes;
    for(nodeset_t::iterator it = internals.begin(); it != internals.end(); it++) {
        node_t* n = *it;
        if(!n->is_latch()) continue;
        const nodeset_t& fanins = n->get_input(0)->get_fanin_cone();
        std::copy(fanins.begin(), fanins.end(), std::inserter(new_nodes, new_nodes.end()));
    }
    std::cout << "# of new nodes: " << new_nodes.size() << std::endl;
    std::copy(new_nodes.begin(), new_nodes.end(), std::inserter(internals, internals.end()));
    std::cout << "# of internals after augmentation: " << internals.size() << std::endl;

    aggr::id_module_t* mod = create_id_module(name.c_str(), aggr::id_module_t::INFERRED, internals);
    mod->enable_annotation(name);
    add_module(mod);
}

aggr::id_module_t* create_id_module(const char* name, aggr::id_module_t::type_t typ, nodeset_t& nodes)
{
    using namespace aggr;

    nodeset_t outputs, inputs;
    for(nodeset_t::iterator it = nodes.begin(); it != nodes.end(); it++) {
        node_t* n = *it;
        for(node_t::input_iterator it = n->inputs_begin(); it != n->inputs_end(); it++) {
            node_t* inp = *it;
            if(nodes.find(inp) == nodes.end()) {
                if(inp->is_latch_gate()) {
                    std::cout << "node: " << n->get_name() << std::endl;
                }
                inputs.insert(inp);
            }
        }
        bool going_out = false;
        for(node_t::fanout_iterator it = n->fanouts_begin(); it != n->fanouts_end(); it++) {
            node_t* fout = *it;
            if(nodes.find(fout) == nodes.end()) {
                going_out = true;
                break;
            }
        }
        if(going_out) {
            outputs.insert(n);
        }
    }
    id_module_t* mod = new id_module_t(name, id_module_t::UNSLICEABLE, typ);
    for(nodeset_t::iterator it = outputs.begin(); it != outputs.end(); it++) {
        mod->add_output(*it);
    }
    for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
        mod->add_input(*it);
    }
    mod->compute_internals2();
    return mod;
}

void flat_module_t::count_tree_info(nodelist_t& nodes, std::map<int, int>& clk_cnt, std::map<int, int>& rst_cnt)
{
    for(unsigned i=0; i != nodes.size(); i++) {
        node_t* g = nodes[i];
        if(g->get_clktrees()) {
            clk_cnt[g->get_clktrees()] += 1;
        } else {
            clk_cnt[0] += 1;
        }
        if(g->get_rsttrees()) {
            rst_cnt[g->get_rsttrees()] += 1;
        } else {
            rst_cnt[0] += 1;
        }
    }
}

void flat_module_t::dump_tree_info(std::map<int, int>& clk_cnt, std::map<int, int>& rst_cnt, const char* label, std::ostream& out)
{
    out << label << ": clock tree" << std::endl;
    for(std::map<int, int>::iterator it = clk_cnt.begin(); it != clk_cnt.end(); it++) {
        out << "clk: " << std::setw(8) << it->first << "; cnt: " << std::setw(9) << it->second << std::endl;
    }
    out << label << ": reset tree" << std::endl;
    for(std::map<int, int>::iterator it = rst_cnt.begin(); it != rst_cnt.end(); it++) {
        out << "rst: " << std::setw(8) << it->first << "; cnt: " << std::setw(9) << it->second << std::endl;
    }
}

void flat_module_t::dist_bfs_input(node_t* inp)
{
    typedef std::pair<node_t*, int> qent_t;
    typedef std::queue<qent_t> bfsq_t;

    bfsq_t bfsq;
    assert(inp->is_input());

    node_t* pad = get_pad_port(inp);
    qent_t e(pad, 1);
    bfsq.push(e);

    while(!bfsq.empty()) {
        qent_t e = bfsq.front();
        bfsq.pop();

        node_t* n = e.first;
        int d = e.second;

        if(d < n->get_input_distance()) {
            n->set_input_distance(d, inp);
            for(node_t::fanout_iterator it = n->fanouts_begin(); it != n->fanouts_end(); it++) {
                node_t* f = *it;
                qent_t new_ent(f, d+1);
                bfsq.push(new_ent);
            }
        }
    }
}

void flat_module_t::dist_bfs_output(node_t* out)
{
    typedef std::pair<node_t*, int> qent_t;
    typedef std::queue<qent_t> bfsq_t;

    bfsq_t bfsq;
    assert(out->is_output());

    node_t* pad = get_pad_port(out);
    qent_t e(pad, 1);
    bfsq.push(e);

    while(!bfsq.empty()) {
        qent_t e = bfsq.front();
        bfsq.pop();

        node_t* n = e.first;
        int d = e.second;

        if(d < n->get_output_distance()) {
            n->set_output_distance(d, out);
            for(node_t::input_iterator it = n->inputs_begin(); it != n->inputs_end(); it++) {
                node_t* in = *it;
                qent_t new_ent(in, d+1);
                bfsq.push(new_ent);
            }
        }
    }
}

void flat_module_t::tree_bfs(const char* modulename, const char* signalname, node_t* clk)
{
    using namespace std;
    queue<node_t*> q;
    q.push(clk);
    nodeset_t visited;
    while(!q.empty()) {
        node_t* n = q.front();
        q.pop();
        for(node_t::fanout_iterator it = n->fanouts_begin(); it != n->fanouts_end(); it++) {
            node_t* fout = *it;
            if(fout->is_gate() && !fout->is_latch_gate()) {
                if(visited.find(fout) == visited.end()) {
                    visited.insert(fout);
                    q.push(fout);
                }
            }
        }
    }
    nodeset_t inputs;
    for(nodeset_t::iterator it = visited.begin(); it != visited.end(); it++) {
        node_t* n = *it;
        for(node_t::input_iterator jt = n->inputs_begin(); jt != n->inputs_end(); jt++) {
            node_t* inp = *jt;
            if(visited.find(inp) == visited.end()) {
                inputs.insert(inp);
            }
        }
    }
    using namespace aggr;
    char buf[256];
    sprintf(buf, "%s_%s", modulename, signalname);
    id_module_t* mod = new id_module_t(buf, id_module_t::UNSLICEABLE, id_module_t::INFERRED);
    for(nodeset_t::iterator it = visited.begin(); it != visited.end(); it++) {
        mod->add_output(*it);
    }
    for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
        mod->add_input(*it);
    }
    mod->compute_internals();
    add_module(mod);

}

bool flat_module_t::check_connectivity(node_t* nf, node_t* ni, nodeset_t& visited){
    //(node_t* n, std::map<node_t*, int>& chklist,nodeset_t& visited,nodeset_t& inps, nodeset_t& outs, bool stop_latch){
    visited.insert(nf);
    //std::cout << (n)->get_name() <<std::endl;
    if(nf == ni){
        return true;
    }
    else{
        if(nf->is_latch()){
            return false;
        }
        else{
            for(node_t::input_iterator it = nf->inputs_begin(); it != nf->inputs_end(); it++){
                node_t* inp = (*it);
                if(visited.find(inp) == visited.end()) {
                    //std::cout << "input of "<< n->get_name()<< "(not visited): "<< inp->get_name() <<std::endl;
                    if( check_connectivity(inp,ni,visited)){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool flat_module_t::check_latch_connectivity(node_t* nf, node_t* ni, nodeset_t& visited){
    //(node_t* n, std::map<node_t*, int>& chklist,nodeset_t& visited,nodeset_t& inps, nodeset_t& outs, bool stop_latch){
    visited.insert(nf);
    //std::cout << (n)->get_name() <<std::endl;
    if(nf == ni){
        return true;
    }
    else{
            nodeset_t::iterator it_b;
            nodeset_t::iterator it_e;
            it_b = (*(nf->get_driving_latches())).begin();
            it_e = (*(nf->get_driving_latches())).end();
            
            for(nodeset_t::iterator it = it_b; it != it_e; it++){
                node_t* inp = (*it);
                if(visited.find(inp) == visited.end()) {
                    //std::cout << "input of "<< n->get_name()<< "(not visited): "<< inp->get_name() <<std::endl;
                    if( check_latch_connectivity(inp,ni,visited)){
                        return true;
                    }
                }
            }
    }
    return false;
}

bool flat_module_t::check_inp_connectivity_with_set(node_t* nf, nodelist_t & nset, nodeset_t& visited, nodeset_t & ndpth){
    //(node_t* n, std::map<node_t*, int>& chklist,nodeset_t& visited,nodeset_t& inps, nodeset_t& outs, bool stop_latch){
    //std::cout << (n)->get_name() <<std::endl;
    visited.insert(nf);
    if(std::find((nset).begin(),(nset).end(),nf)!=(nset).end() ){
        return true;
    }
    else{
        if(nf->is_latch()){
            return false;
        }
        else{
            for(node_t::input_iterator it = nf->inputs_begin(); it != nf->inputs_end(); it++){
                node_t* inp = (*it);
                if(visited.find(inp) == visited.end()) {
                    //std::cout << "input of "<< n->get_name()<< "(not visited): "<< inp->get_name() <<std::endl;
                    if( check_inp_connectivity_with_set(inp,nset,visited,ndpth)){
                        ndpth.insert(nf);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool flat_module_t::check_inp_1st_connectivity_with_set(node_t* nf, nodelist_t & nset){
    //(node_t* n, std::map<node_t*, int>& chklist,nodeset_t& visited,nodeset_t& inps, nodeset_t& outs, bool stop_latch){
    //std::cout << (n)->get_name() <<std::endl;
    for(node_t::input_iterator it = nf->inputs_begin(); it != nf->inputs_end(); it++){
        node_t* inp = (*it);
        //std::cout << "input of "<< n->get_name()<< "(not visited): "<< fout->get_name() <<std::endl;
        if(std::find((nset).begin(),(nset).end(),inp)!=(nset).end() ){
            return true;
        }
    }
    return false;
}

bool flat_module_t::check_fout_connectivity_with_set(node_t* ni, nodelist_t & nset, nodeset_t& visited, nodeset_t & ndpth){
    //(node_t* n, std::map<node_t*, int>& chklist,nodeset_t& visited,nodeset_t& inps, nodeset_t& outs, bool stop_latch){
    //std::cout << (n)->get_name() <<std::endl;
    visited.insert(ni);
    if(std::find((nset).begin(),(nset).end(),ni)!=(nset).end() ){
        return true;
    }
    else{
        if(ni->is_latch()){
            return false;
        }
        else{
            for(node_t::fanout_iterator it = ni->fanouts_begin(); it != ni->fanouts_end(); it++){
                node_t* fout = (*it);
                if(visited.find(fout) == visited.end()) {
                    //std::cout << "input of "<< n->get_name()<< "(not visited): "<< fout->get_name() <<std::endl;
                    if( check_fout_connectivity_with_set(fout,nset,visited,ndpth)){
                        ndpth.insert(ni);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool flat_module_t::check_fout_1st_connectivity_with_set(node_t* ni, nodelist_t & nset){
    //(node_t* n, std::map<node_t*, int>& chklist,nodeset_t& visited,nodeset_t& inps, nodeset_t& outs, bool stop_latch){
    //std::cout << (n)->get_name() <<std::endl;
    for(node_t::fanout_iterator it = ni->fanouts_begin(); it != ni->fanouts_end(); it++){
        node_t* fout = (*it);
        //std::cout << "input of "<< n->get_name()<< "(not visited): "<< fout->get_name() <<std::endl;
        if(std::find((nset).begin(),(nset).end(),fout)!=(nset).end() ){
            return true;
        }
    }
    return false;
}

void flat_module_t::aggregate_nodes(node_t * n, nodelist_t& nset, nodeset_t& checked, nodelist_t * inputs, nodelist_t * outputs, nodeset_t & drivennodes, nodeset_t & drivingnodes){
    //std::cout << (n)->get_name() <<std::endl;

    if(checked.find(n) == checked.end()){
        checked.insert(n);
        if(/*(!n->is_latch()) &&*/ (std::find((*inputs).begin(),(*inputs).end(),n)==(*inputs).end()) ){
            //inputs..
            for(node_t::input_iterator it = n->inputs_begin(); it != n->inputs_end(); it++){
                node_t* inp = (*it);
                //if(std::find((nset).begin(),(nset).end(),inp)==(nset).end() ){
                if(drivennodes.find(inp) != drivennodes.end()){
                    if(std::find((nset).begin(),(nset).end(),inp)==(nset).end() ){
                        nset.push_back(inp);
                        //std::cout << "input of "<< n->get_name()<< ": "<< inp->get_name() <<std::endl;
                    }
                    //aggrnodes.push_back(inp);
                    aggregate_nodes(inp,nset,checked,inputs,outputs,drivennodes,drivingnodes);                
                }
                else{
                    nodeset_t visiteds;
                    nodeset_t& visited = visiteds;
                    if(check_inp_connectivity_with_set(inp,nset,visited,drivennodes)){
                        //if(check_inp_1st_connectivity_with_set(inp,nset)){
                        if(std::find((nset).begin(),(nset).end(),inp)==(nset).end() ){
                            nset.push_back(inp);
                            //std::cout << "input of "<< n->get_name()<< ": "<< inp->get_name() <<std::endl;
                        }
                        //aggrnodes.push_back(inp);
                        aggregate_nodes(inp,nset,checked,inputs,outputs,drivennodes,drivingnodes);
                    }                
                }
                //}
            }
                //outputs..
            /*for(node_t::fanout_iterator it = n->fanouts_begin(); it != n->fanouts_end(); it++){
                node_t* fout = (*it);
                //if(std::find((nset).begin(),(nset).end(),fout)==(nset).end() ){
                if(drivingnodes.find(fout) != drivingnodes.end()){
                    //aggrnodes.push_back(fout);
                    if(std::find((nset).begin(),(nset).end(),fout)==(nset).end() ){
                        nset.push_back(fout);
                        std::cout << "output of "<< n->get_name()<< ": "<< fout->get_name() <<std::endl;
                    }
                    aggregate_nodes(fout,nset,checked,inputs,outputs,drivennodes,drivingnodes,aggrnodes);                
                }
                else{
                    nodeset_t visiteds;
                    nodeset_t& visited = visiteds;
                    if(check_fout_connectivity_with_set(fout,nset,visited,drivingnodes)){
                        //if(check_fout_1st_connectivity_with_set(fout,nset)){
                        //aggrnodes.push_back(fout);
                        if(std::find((nset).begin(),(nset).end(),fout)==(nset).end() ){
                            nset.push_back(fout);
                            std::cout << "output of "<< n->get_name()<< ": "<< fout->get_name() <<std::endl;
                        }
                        aggregate_nodes(fout,nset,checked,inputs,outputs,drivennodes,drivingnodes,aggrnodes);
                    }                
                }
                //}
            }*/
        }
    }
}

void flat_module_t::get_connected_nodes(nodeset_t & nset, nodelist_t * inputs, std::vector<nodeset_t> & v_sets, bool include_latch){
    nodelist_t stack;
    nodeset_t chkd;
    nodeset_t & checked = chkd;
    std::vector<nodeset_t> v_nset1;
    std::vector<nodeset_t> & v_nset = v_nset1;
    for(nodeset_t::iterator iter =nset.begin(); iter != nset.end(); iter++){
        node_t* n = (*iter);
        //v_nset.push_back(tmp);
        //if(!n->is_latch()){
        bool ltchflg;

        if(checked.find(n)!=checked.end()){
            if(include_latch){
                ltchflg = true;
            }
            else{
                ltchflg = !n->is_latch();
            }
            if(ltchflg){//to prevent unnecesary computation, since latches are not inserted in the main algorithm
                int flg = 0;
                for(unsigned i = 0; ((i < v_nset.size()) && (flg == 0)); i++){
                    if(v_nset[i].find(n) != v_nset[i].end()){
                        v_sets[i].insert(n);
                        //std::cout << "inserted "<< n->get_name() <<std::endl;
                        flg = 1;
                    }
                }
            }
        }
        else{
            nodeset_t tmp;
            v_nset.push_back(tmp);
            tmp.insert(n);
            v_sets.push_back(tmp);
            stack.push_back(n);
            while (stack.size()>0){
                node_t * nn = stack[stack.size()-1];
                stack.pop_back();

                if(checked.find(nn)!=checked.end()){ 
                    continue; 
                } 
                else{ 
                    checked.insert(nn); 
                }
                bool lcnd;
                if(include_latch){
                    lcnd = true;
                }
                else{
                    lcnd = !nn->is_latch();
                }
                if(lcnd){
                    v_nset.back().insert(nn);
                    if(std::find((*inputs).begin(),(*inputs).end(),nn) == (*inputs).end() ) {
                        for(node_t::input_iterator it = nn->inputs_begin(); it != nn->inputs_end(); it++){
                            node_t* inp = (*it);
                            //std::cout << "input of "<< nn->get_name()<< ": "<< inp->get_name() <<std::endl;
                            stack.push_back(inp);
                        }
                        for(node_t::fanout_iterator it = nn->fanouts_begin(); it != nn->fanouts_end(); it++){
                            node_t* fout = (*it);
                            //std::cout << "output of "<< nn->get_name()<< ": "<< fout->get_name() <<std::endl;
                            stack.push_back(fout);
                        }
                    }
                }
            }
        }
        //}
    }
}

void flat_module_t::inp_narrow_connected_set(std::vector<nodeset_t> & v_sets, std::vector<nodelist_t> & v_lists){
    for(unsigned i =0; i<v_sets.size();i++){
        if(v_sets[i].size()>1){
            nodelist_t tmp;
            v_lists.push_back(tmp);
            for(nodeset_t::iterator iter =(v_sets[i]).begin(); iter != (v_sets[i]).end(); iter++){
                node_t* n = (*iter);
                //std::cout << n->get_name() << std::endl;
                int flg1 = 0;
                int flg2 = 0;
                for(node_t::input_iterator it = n->inputs_begin(); ((it != n->inputs_end()) && (flg1 == 0)); it++){
                    node_t* inp = (*it);
                    if(v_sets[i].find(inp) != v_sets[i].end()){
                        flg1 ++;
                        //std::cout << inp->get_name() << std::endl;
                    }
                }
                for(node_t::fanout_iterator it = n->fanouts_begin(); ((it != n->fanouts_end()) && (flg2 == 0)); it++){
                    node_t* fout = (*it);
                    if(v_sets[i].find(fout) != v_sets[i].end()){
                        flg2 ++;
                        //std::cout << fout->get_name() << std::endl;
                    }
                }
                if (flg1 != 0){
                    v_lists.back().push_back(n);
                }
            }

        }
    }
}

void flat_module_t::narrow_connected_set(std::vector<nodeset_t> & v_sets, std::vector<nodelist_t> & v_lists){
    for(unsigned i =0; i<v_sets.size();i++){
        if(v_sets[i].size()>1){
            nodelist_t tmp;
            v_lists.push_back(tmp);
            for(nodeset_t::iterator iter =(v_sets[i]).begin(); iter != (v_sets[i]).end(); iter++){
                node_t* n = (*iter);
                //std::cout << n->get_name() << std::endl;
                int flg1 = 0;
                int flg2 = 0;
                for(node_t::input_iterator it = n->inputs_begin(); ((it != n->inputs_end()) && (flg1 == 0)); it++){
                    node_t* inp = (*it);
                    if(v_sets[i].find(inp) != v_sets[i].end()){
                        flg1 ++;
                        //std::cout << inp->get_name() << std::endl;
                    }
                }
                for(node_t::fanout_iterator it = n->fanouts_begin(); ((it != n->fanouts_end()) && (flg2 == 0)); it++){
                    node_t* fout = (*it);
                    if(v_sets[i].find(fout) != v_sets[i].end()){
                        flg2 ++;
                        //std::cout << fout->get_name() << std::endl;
                    }
                }
                if ((flg1+flg2) > 1){
                    v_lists.back().push_back(n);
                }
            }

        }
    }
}

void flat_module_t::full_narrow_connected_set(std::vector<nodeset_t> & v_sets, std::vector<nodelist_t> & v_lists){
    for(unsigned i =0; i<v_sets.size();i++){
        if(v_sets[i].size()>1){
            nodelist_t tmp;
            v_lists.push_back(tmp);
            for(nodeset_t::iterator iter =(v_sets[i]).begin(); iter != (v_sets[i]).end(); iter++){
                node_t* n = (*iter);
                //std::cout << n->get_name() << std::endl;
                int flg1 = 0;
                int flg2 = 0;
                for(node_t::input_iterator it = n->inputs_begin(); ((it != n->inputs_end()) && (flg1 == 0)); it++){
                    node_t* inp = (*it);
                    if(v_sets[i].find(inp) == v_sets[i].end()){
                        flg1 ++;
                        //std::cout << inp->get_name() << std::endl;
                    }
                }
                for(node_t::fanout_iterator it = n->fanouts_begin(); ((it != n->fanouts_end()) && (flg2 == 0)); it++){
                    node_t* fout = (*it);
                    if(v_sets[i].find(fout) == v_sets[i].end()){
                        flg2 ++;
                        //std::cout << fout->get_name() << std::endl;
                    }
                }
                if ((flg1+flg2) == 0){
                    v_lists.back().push_back(n);
                }
            }

        }
    }
}

void flat_module_t::reduce_extra_inputs(node_t* n, nodeset_t& inps1, nodeset_t& inps_visited)
{
    //if(inps_visited.find(n) != inps_visited.end()){
    inps_visited.insert(n);
    if(n->is_latch()){
        inps1.insert(n);
    }
    else{
        for(node_t::input_iterator it = n->inputs_begin(); it != n->inputs_end(); it++){
            node_t* inp = (*it);
            if(inps_visited.find(inp) == inps_visited.end()) {
                //std::cout << "input of "<< n->get_name()<< "(not visited): "<< inp->get_name() <<std::endl;
                reduce_extra_inputs(inp,inps1,inps_visited);
            }
        }
    }
}

bool flat_module_t::check_node(node_t* n, std::map<node_t*, int>& chklist,nodeset_t& visited,nodeset_t& inps, nodeset_t& outs, nodeset_t& ilatches, bool stop_latch){
    bool result =false;
    visited.insert(n);
    //std::cout << (n)->get_name() <<std::endl;

    if(chklist.find(n) != chklist.end()){
        //std::cout << "Already checked in previous node traces: " << (n)->get_name() <<" with " << chklist[n] <<std::endl;
        if (chklist[n] == 1){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        if(inps.find(n) != inps.end()){
            chklist[n] = 1;
            //std::cout << "Checked as an input: " << (n)->get_name() <<std::endl;
            return true;
        }
        else{
            if(stop_latch && n->is_latch() && (outs.find(n) == outs.end()) && (ilatches.find(n) == ilatches.end()) ){
                chklist[n] = 0;
                return false;
                //it_b = n->get_input(0)->inputs_begin();
                //it_e = n->get_input(0)->inputs_end();
            }
            else{
                node_t::input_iterator it_b;
                node_t::input_iterator it_e;
                it_b = n->inputs_begin();
                it_e = n->inputs_end();
                for(node_t::input_iterator it = it_b; it != it_e; it++){
                    node_t* inp;
                    inp = (*it);
                    if(std::find((n->fanouts).begin(),(n->fanouts).end(),inp)==(n->fanouts).end() ) {
                        //if( (n->fanouts).find(inp) == (n->fanouts()).end() )
                        if(visited.find(inp) == visited.end()) {
                            //std::cout << "input of "<< n->get_name()<< "(not visited): "<< inp->get_name() <<std::endl;
                            if( check_node(inp,chklist,visited,inps,outs,ilatches,stop_latch)){
                                result = true;
                            }
                        }
                        else{
                            if(chklist.find(inp) != chklist.end()) {
                                if(chklist[inp]==1){
                                    //std::cout<< "node "<< n->get_name()<< " is inserted due to checked input: " << inp->get_name() <<std::endl;
                                    result = true;
                                }
                            }
                        }
                    }
                    else{
                        visited.insert(inp);
                        chklist[inp] = 0;
                    }                    
                }
            }
        }

        if(result){
            chklist[n] = 1;
            //std::cout << "Checked node: " << (n)->get_name() << " with " << chklist[n] <<std::endl;
        }
        else {
            chklist[n] = 0;
        }
        ////std::cout << "Checked node: " << (n)->get_name() << " with " << chklist[n] <<std::endl;
    }
    return result;
}

bool flat_module_t::get_intermediate_latches(node_t* n, std::map<node_t*, int>& chklist,nodeset_t& visited,nodeset_t& inps, nodeset_t& outs){
    bool result =false;
    visited.insert(n);
    //std::cout << (n)->get_name() <<std::endl;

    if(chklist.find(n) != chklist.end()){
        //std::cout << "Already checked in previous node traces: " << (n)->get_name() <<" with " << chklist[n] <<std::endl;
        if (chklist[n] == 1){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        if(inps.find(n) != inps.end()){
            chklist[n] = 1;
            //std::cout << "Checked as an input: " << (n)->get_name() <<std::endl;
            return true;
        }
        else{
            nodeset_t::iterator it_b;
            nodeset_t::iterator it_e;
            it_b = (*(n->get_driving_latches())).begin();
            it_e = (*(n->get_driving_latches())).end();
            
            for(nodeset_t::iterator it = it_b; it != it_e; it++){
                node_t* inp;
                inp = (*it);
                if((*(n->get_driven_latches())).find(inp) == (*(n->get_driven_latches())).end() ){
                    if(visited.find(inp) == visited.end()) {
                        //std::cout << "input of "<< n->get_name()<< "(not visited): "<< inp->get_name() <<std::endl;
                        if( get_intermediate_latches(inp,chklist,visited,inps,outs)){
                            result = true;
                        }
                    }
                    else{
                        if(chklist.find(inp) != chklist.end()) {
                            if(chklist[inp]==1){
                                //std::cout<< "node "<< n->get_name()<< " is inserted due to checked input: " << inp->get_name() <<std::endl;
                                result = true;
                            }
                        }
                    }
                }
                else{
                    visited.insert(inp);
                    chklist[inp] = 0;
                }
            }
        }

        if(result){
            chklist[n] = 1;
            //std::cout << "Checked node: " << (n)->get_name() << " with " << chklist[n] <<std::endl;
        }
        else {
            chklist[n] = 0;
        }
        ////std::cout << "Checked node: " << (n)->get_name() << " with " << chklist[n] <<std::endl;
    }
    return result;
}


void flat_module_t::get_module_givenIO(nodeset_t& outputs, nodeset_t& inputs,std::map<node_t*, int>& chklist, char * chr, nodeset_t& ilatches, bool stop_latch){

    for(nodeset_t::iterator it = outputs.begin(); it != outputs.end(); it++) {
        //nodeset_t::iterator it = outputs.begin();
        nodeset_t visited_nodes;
        nodeset_t& visited = visited_nodes;
        //std::cout << "Starting to trace node for "<< chr << " :" << (*it)->get_name() <<std::endl;
        check_node((*it),chklist,visited,inputs,outputs,ilatches,stop_latch);
    }

    nodeset_t inps0;
    nodeset_t inps1;
    nodeset_t& ref_inps1 = inps1;

    for( std::map<node_t*, int>::iterator it = chklist.begin(); it != chklist.end(); it++) {
        if(chklist[it->first] == 1){
            if((inputs.find(it->first) == inputs.end()) ) {
                node_t* n = it->first;
                for(node_t::input_iterator jt = n->inputs_begin(); jt != n->inputs_end(); jt++) { //check the inputs
                    node_t* inp = *jt;
                    if(chklist.find(inp) == chklist.end()) {
                        inps0.insert(inp);
                        //std::cout << "Not in chklist "<< inp->get_name() << " : from " << (n)->get_name() <<std::endl;
                    }
                    else{
                        if( (chklist[inp] == 0) && (outputs.find(inp) == outputs.end()) ){
                            //std::cout << "In chklist "<< inp->get_name() << " : from " << (n)->get_name() <<std::endl;
                            inps0.insert(inp);
                        }
                    }
                }
            }
        }
    }

    if(ilatches.size()>0){
        for(nodeset_t::iterator it = inps0.begin(); it != inps0.end(); it++) {
            if(outputs.find((*it)) == outputs.end()){
                inputs.insert(*it);
            }
        }
    }
    else{
        for(nodeset_t::iterator it =inps0.begin(); it != inps0.end(); it++){
            //call reduce method
            nodeset_t inps_visited;
            nodeset_t& ref_inps_visited = inps_visited;
            reduce_extra_inputs((*it), ref_inps1, ref_inps_visited);
        }

        for(nodeset_t::iterator it = ref_inps1.begin(); it != ref_inps1.end(); it++) {
            if(outputs.find((*it)) == outputs.end()){
                inputs.insert(*it);
            }
        }
    }

    //for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
        //nodeset_t::iterator it = outputs.begin();
        //std::cout << "Input "<< (*it)->get_name() <<std::endl;
    //}

    using namespace aggr;
    char buf[256];
    sprintf(buf, "NC_%s", chr);
    id_module_t* mod = new id_module_t(buf, id_module_t::UNSLICEABLE, id_module_t::INFERRED);
    for(nodeset_t::iterator it = outputs.begin(); it != outputs.end(); it++) {
        mod->add_output(*it);
    }
    for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
        mod->add_input(*it);
    }
    mod->compute_internals2();
    add_module(mod);

    assert(!mod->is_marked_bad());	
    std::cout << "Ending "<< chr << " :" <<std::endl;
}

//will be fixed later for itag1b
void flat_module_t::get_NC_circuit(std::string& vfn)
{
    //using namespace vcd_n;
    //if(vfn.size()>0) {
    //vcd_n::vcd_file_t vf(vfn);
    //intvec_t v(vf.bits.size(), -1);
    //intvec_t& v1 = v; //intvec_t *vlast = &v1
    //std::map<node_t*, int> nodebitnomap;
    //std::map<node_t*, int> & ref_nodebitnomap = nodebitnomap;
    //vf.prepare_NCinfo(ref_nodebitnomap,v1,this);

    //std::ofstream f("v1.out");
    //for(unsigned i = 0 ; i<v.size();i++){
    //f<< v1[i] <<std::endl;
    //}
    //f.close();

    //queue<node_t*> q;
    //node_t* NC = get_node_by_name("NC_1p");
    //node_t* nNC = get_pad_port(NC);
    //q.push(nNC);

    //nodeset_t visited;
    //while(!q.empty()) {
    //node_t* n = q.front();
    //q.pop();

    //std::cout << "fanouts of " << n->get_name() <<":"<<std::endl;
    //for(node_t::fanout_iterator it = n->fanouts_begin(); it != n->fanouts_end(); it++){
    //node_t* fout;
    //if((*it)->is_latch_gate()){
    //fout = (*it)->get_first_fanout();
    //std::cout << fout->get_name()<< " is latch gate" <<std::endl;
    //}
    //else{
    //fout = (*it);
    //}
    //fordebug(fout);
    //if((v1[ref_nodebitnomap[fout]]==-1))  {
    //if(visited.find(fout) == visited.end()) {
    //visited.insert(fout);
    //std::cout << "inserted: " << fout->get_name()<<" with bitno: "<< ref_nodebitnomap[fout] <<std::endl;
    //q.push(fout);
    //}
    //}
    //}
    //}
    //nodeset_t inputs;
    //for(nodeset_t::iterator it = visited.begin(); it != visited.end(); it++) {
    //node_t* n = *it;
    //for(node_t::input_iterator jt = n->inputs_begin(); jt != n->inputs_end(); jt++) {
    //node_t* inp = *jt;
    //if(visited.find(inp) == visited.end()) {
    //inputs.insert(inp);
    //}
    //}
    //}
    //using namespace aggr;
    //id_module_t* mod = new id_module_t("NC2", id_module_t::UNSLICEABLE, id_module_t::INFERRED);
    //for(nodeset_t::iterator it = visited.begin(); it != visited.end(); it++) {
    //mod->add_output(*it);
    //}
    //for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
    //mod->add_input(*it);
    //}
    //mod->compute_internals();
    //add_module(mod);
    //}
    using namespace vcd_n;
    if(vfn.size()>0) {
        vcd_n::vcd_file_t vf(vfn);
        intvec_t v(vf.bits.size(), -1);
        intvec_t& v1 = v; //intvec_t *vlast = &v1
        std::map<node_t*, int> nodebitnomap;
        std::map<node_t*, int> & ref_nodebitnomap = nodebitnomap;
        vf.prepare_NCinfo(ref_nodebitnomap,v1,this);

        std::ofstream f("v1.out");
        for(unsigned i = 0 ; i<v.size();i++){
            f<< v1[i] <<std::endl;
        }
        f.close();

        using namespace std;
        queue<node_t*> q;
        node_t* NC = get_node_by_name("MEM_RD_ENp");
        node_t* nNC = get_pad_port(NC);
        q.push(nNC);
        nodeset_t visited;
        while(!q.empty()) {
            node_t* n = q.front();
            q.pop();

            std::cout << "Is " << n->get_name() << " latch? "<< n->is_latch() <<std::endl;
            std::cout << "Is " << n->get_name() << " latch gate? "<< n->is_latch_gate() <<std::endl;
            std::cout << "Is " << n->get_name() << " input? "<< n->is_input() <<std::endl;
            std::cout << "Is " << n->get_name() << " output? "<< n->is_output() <<std::endl;
            std::cout << "Is " << n->get_name() << " gate? "<< n->is_gate() <<std::endl;
            std::cout << "Is " << n->get_name() << " macro? "<< n->is_macro() <<std::endl;
            std::cout << "Is " << n->get_name() << " macro out? "<< n->is_macro_out() <<std::endl;
            std::cout << "inputs of: " << n->get_name() <<std::endl;
            if(n->is_latch()){
                for(node_t::input_iterator it = n->get_input(0)->inputs_begin(); it != n->get_input(0)->inputs_end(); it++){
                    //std::cout << inp->get_name() <<std::endl;
                    if((v1[ref_nodebitnomap[(*it)]]==-1) )  {
                        node_t* inp;
                        if(visited.find((*it)) == visited.end()) {
                            if((*it)->is_latch_gate()){
                                inp = (*it)->get_input(0);
                            }
                            else{
                                inp = (*it);
                            }
                            std::cout << "inserted: " << inp->get_name() <<std::endl;
                            visited.insert(inp);
                            q.push(inp);
                        }
                    }
                }
            }
            else{
                for(node_t::input_iterator it = n->inputs_begin(); it != n->inputs_end(); it++) {
                    //std::cout << inp->get_name() <<std::endl;
                    if((v1[ref_nodebitnomap[(*it)]]==-1))  {
                        node_t* inp;
                        if(visited.find((*it)) == visited.end()) {
                            if((*it)->is_latch_gate()){
                                inp = (*it)->get_input(0);
                            }
                            else{
                                inp = (*it);
                            }
                            std::cout << "inserted: " << inp->get_name()<<" with bitno: "<< ref_nodebitnomap[inp] <<std::endl;
                            visited.insert(inp);
                            q.push(inp);
                        }
                    }
                }
            }
        }
        nodeset_t inputs;
        for(nodeset_t::iterator it = visited.begin(); it != visited.end(); it++) {
            node_t* n = *it;
            if(n->is_latch()){
                for(node_t::input_iterator jt = n->get_input(0)->inputs_begin(); jt != n->get_input(0)->inputs_end(); jt++) {
                    node_t* inp = *jt;
                    if(visited.find(inp) == visited.end()) {
                        std::cout << "in inputs " << inp->get_name()<<std::endl;
                        inputs.insert(inp);
                    }
                }
            }
            else{
                for(node_t::input_iterator jt = n->inputs_begin(); jt != n->inputs_end(); jt++) {
                    node_t* inp = *jt;
                    if(visited.find(inp) == visited.end()) {
                        std::cout << "in inputs " << inp->get_name()<<std::endl;
                        inputs.insert(inp);
                    }
                }
            }
        }
        using namespace aggr;
        id_module_t* mod = new id_module_t("NCcircuit1", id_module_t::UNSLICEABLE, id_module_t::INFERRED);
        mod->add_output(nNC);
        for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
            mod->add_input(*it);
        }
        mod->compute_internals();
        add_module(mod);
    }
}

bool flat_module_t::is_pad_input(node_t *n) const
{
    if(!n->is_input()) return false;
    if(n->num_fanouts() != 1) return false;

    node_t* pad = *(n->fanouts_begin());
    lib_elem_t* pad_type = pad->get_lib_elem();
    if(pad_type == NULL) return false;
    if(strcmp(pad_type->get_name(), "PBIDIR_18_PL_V") != 0 &&
       strcmp(pad_type->get_name(), "PBIDIR_18_PL_H") != 0) {

        return false;
    }

    return true;
}

node_t* flat_module_t::get_pad_port(node_t* n) const
{
    assert(n->is_input() || n->is_output());

    node_t* pad = NULL;

    if(n->is_input()) {
        if(n->num_fanouts() != 1) {
            std::cout << "trouble brewing with #fanouts for: " << n->get_name()
                << "; num_fanouts: " << n->num_fanouts() << std::endl;
        }
        assert(n->num_fanouts() == 1);
        pad = *(n->fanouts_begin());
    } else {
        assert(n->is_output());
        assert(n->get_type() == node_t::MACRO_OUT);
        pad = n->get_input(0);
    }
    assert(pad != NULL);
    lib_elem_t* pad_type = pad->get_lib_elem();

    assert(strcmp(pad_type->get_name(), "PBIDIR_18_PL_V") == 0 ||
           strcmp(pad_type->get_name(), "PBIDIR_18_PL_H") == 0);

    if(n->is_input()) {
        int idx = pad_type->get_output_index("Y");
        assert(idx != -1);
        return pad->get_macro_output(idx);
    } else {
        assert(n->is_output());
        int idx = pad_type->get_input_index("A");
        assert(idx != -1);
        return pad->get_input(idx);
    }

    return NULL;
}

void flat_module_t::rewrite_nodes()
{
    std::cout << "inside rewrite_nodes." << std::endl;
    for(map_t::iterator it =  map.begin();
            it != map.end();
            it++)
    {
        lib->rewrite_node(it->second, this);
    }
}

void flat_module_t::rewriteInverters(FILE* fp)
{
    std::cout << "inside rewriteInverters." << std::endl;
    typedef std::map<node_t*, node_t*> node2node_map_t;
    node2node_map_t not_map;

    bool changed = false;
    int iter=0;
    do {
        changed = false;
        for(unsigned i=0; i != gates.size(); i++) {
            node_t* out = gates[i];
            if(out->is_latch_gate()) continue;

            if(out->is_inverter()) {
                node_t* in = out->get_input(0);
                node2node_map_t::iterator pos;
                if((pos =  not_map.find(in)) == not_map.end()) {
                    not_map[in] = out;
                } else {
                    if(pos->second != out) {
                        changed = rewriteNodes(fp, out, pos->second) || changed;
                    }
                }
            }
        }

        iter++;
    } while(changed);
    //std::cout << "rewriteInverters finished after " << iter << " iterations." << std::endl;
}

void flat_module_t::rewriteBuffers(FILE* fp)
{
    std::cout << "inside rewriteBuffers." << std::endl;

    bool changed = false;
    int iter=0;
    do {
        changed = false;
        for(unsigned i=0; i != gates.size(); i++) {
            node_t* out = gates[i];
            if(out->is_latch_gate()) continue;

            if(gates[i]->is_buffer()) {
                node_t* in = out->get_input(0);
                changed = rewriteNodes(fp, out, in) || changed;
            } else if(out->is_inverter()) {
                node_t* inv_inp = out->get_input(0);
                if(inv_inp->is_gate() && inv_inp->is_inverter()) {
                    node_t* in = inv_inp->get_input(0);
                    changed = rewriteNodes(fp, out, in) || changed;
                }
            }
        }
        iter++;
    } while(changed);
    std::cout << "rewriteBuffers finished after " << iter << " iterations." << std::endl;
}

void flat_module_t::removeNOR2XB(FILE* fp)
{
    if(options.removeNOR2XB.size() == 0) return;
    node_t* n = get_node_by_name(options.removeNOR2XB.c_str());
    if(n == NULL) {
        std::cout << "ERROR: Unable to find node: " << options.removeNOR2XB << std::endl;
        return;
    }
    int cnt = 0;
    for(unsigned i=0; i != gates.size(); i++) {
        if(!gates[i]->is_latch_gate() && gates[i]->get_lib_elem()->is_nor2xb()) {
            node_t* bn = gates[i]->get_input_by_name("BN");
            if(bn != NULL && gates[i]->get_input_by_name("A")->get_name() == options.removeNOR2XB) {
                cnt += 1;
                rewriteNodes(fp, gates[i], bn);
            }
        }
    }
    std::cout << "# of nor2xb nodes removed: " << cnt << std::endl;
}

bool flat_module_t::rewriteNodes(FILE* fp, node_t* oldn, node_t* newn)
{
    if(oldn == newn) return false;

    if(fp) {
        fprintf(fp, "%s -> %s\n", oldn->get_name().c_str(), newn->get_name().c_str());
    }

    bool changed = false;
    for(node_t::fanout_iterator it = oldn->fanouts_begin(); it != oldn->fanouts_end(); it++) {
        node_t* n = *it;
        assert(n->rewrite_inputs(oldn, newn));
        changed = true;
        newn->add_fanout(n);
    }
    if(changed) {
        oldn->fanouts_clear();
        oldn->set_orig(newn);
    }
    return changed;
}

void flat_module_t::removeDeadNodes(FILE* fp)
{
    int cnt = 0;
    for(int i = gates.size()-1; i >= 0; i--) {
        node_t* g = gates[i];
        if(g->num_fanouts() == 0 && !g->is_output()) {
            assert(!g->is_latch_gate());
            if(g->num_inputs() == 1 || (options.removeNOR2XB.size() > 0 && g->get_lib_elem()->is_nor2xb())) {
                node_t* inp = g->get_input(0);
                inp->remove_fanout(g);
                g->mark_dead();
                cnt += 1;
            }
        }
    }
    std::cout << "marked " << cnt << " nodes dead!" << std::endl;
}


unsigned flat_module_t::max_index() const
{
    return map.size();
}

void flat_module_t::assign_indices()
{
    std::cout << "inside assign_indices." << std::endl;
    int idx=0;
    for(map_t::iterator it  = map.begin();
            it != map.end();
            it++, idx++)
    {
        node_t* n = it->second;
        n->set_index(idx);
    }
}

void flat_module_t::create_fanouts()
{
    std::cout << "inside create_fanouts" << std::endl;
    for(map_t::iterator it  = map.begin();
            it != map.end();
            it++)
    {
        node_t* n = it->second;
        for(node_t::input_iterator jt = n->inputs_begin(); jt != n->inputs_end(); jt++) {
            node_t *i = *jt;
            i->add_fanout(n);
        }
    }
}

void flat_module_t::compute_distances()
{
    if(!options.computeDistances) {
        std::cout << "COMPUTE DISTANCES DISABLED!" << std::endl;
        return;
    }

    std::cout << "computing distances ... "; std::cout.flush();
    for(unsigned i=0; i != inputs.size(); i++) {
        node_t* inp = inputs[i];
        const std::string& name = inp->get_name();
        if(std::find(options.resetTreeRoots.begin(), options.resetTreeRoots.end(), name) != options.resetTreeRoots.end()) {
            continue;
        }
        if(std::find(options.clockTreeRoots.begin(), options.clockTreeRoots.end(), name) != options.clockTreeRoots.end()) {
            continue;
        }
        if(std::find(options.ignorePins.begin(), options.ignorePins.end(), name) != options.ignorePins.end()) {
            continue;
        }
        dist_bfs_input(inp);
    }
    for(unsigned i=0; i != outputs.size(); i++) {
        if(std::find(options.ignorePins.begin(), options.ignorePins.end(), outputs[i]->get_name()) != options.ignorePins.end()) {
            continue;
        }
        dist_bfs_output(outputs[i]);
    }

    std::map<node_t*, int> inp_counts;
    std::map<node_t*, int> out_counts;
    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t* n = it->second;
        node_t* ni = n->get_nearest_input();
        node_t* no = n->get_nearest_output();
        inp_counts[ni] += 1;
        out_counts[no] += 1;
    }

    std::cout << "done!" << std::endl;

    /*
       for(std::map<node_t*, int>::iterator it = inp_counts.begin(); it != inp_counts.end(); it++) {
       node_t* n = it->first;
       int c = it->second;
       if(n) {
       std::cout << "input: " << std::left << std::setw(25) << n->get_name() << ": " << c << std::endl;
       }
       }
       for(std::map<node_t*, int>::iterator it = out_counts.begin(); it != out_counts.end(); it++) {
       node_t* n = it->first;
       int c = it->second;
       if(n) {
       std::cout << "input: " << std::left << std::setw(25) << n->get_name() << ": " << c << std::endl;
       }
       }
       */
}

void flat_module_t::create_words()
{
    if(!options.simpleWordAnalysis) return;

    for(ast_wordlist_t::iterator it = ast_words.begin(); it != ast_words.end(); it++) {
        word_t* w = new word_t(true, word_t::NETLIST);
        stringlist_t& sl = *it;
        unsigned pos=0;
        for(stringlist_t::iterator jt = sl.begin(); jt != sl.end(); jt++) {
            std::string& s = *jt;
            node_t* n = get_node_by_name(s.c_str());
            if(n == NULL) {
                delete w;
                w = NULL;
                break;
            } else {
                w->add_bit(n, pos++);
            }
        }
        if(w) {
            add_word(w);
        }
    }
}

void flat_module_t::create_a2dff_module(markings_t& markings)
{
    unsigned cnt = 0;
    for(unsigned i=0; i != gates.size(); i++) {
        node_t* g = gates[i];
        for(node_t::input_iterator it = g->inputs_begin(); it != g->inputs_end(); it++) {
            node_t* gi = *it;
            int gi_index = gi->get_index();
            if(markings[gi_index]) {
                markings[g->get_index()] = true;
                cnt += 1;
                break;
            }
        }
    }
    std::cout << "marked " << cnt << " gates. " << std::endl;

    nodeset_t internals;
    cnt=0;
    for(unsigned i=0; i != latches.size(); i++) {
        node_t* l = latches[i];
        node_t* lg = l->get_input(0);
        if(markings[l->get_index()] && markings[lg->get_index()]) {
            cnt += 1;
            add_marked_fanins(l, internals, markings);
        }
    }
    std::cout << "latches with marked inputs: " << cnt << std::endl;
    std::cout << "# of internals added: " << internals.size() << std::endl;

    aggr::id_module_t* mod = create_id_module("xbar", aggr::id_module_t::INFERRED, internals);

    const nodelist_t& inputs = *mod->get_inputs();
    nodeset_t inputset;
    std::copy(inputs.begin(), inputs.end(), std::inserter(inputset, inputset.end()));
    for(nodeset_t::iterator it = internals.begin(); it != internals.end(); it++) {
        node_t* n = *it;
        if(n->is_latch_gate()) {
            for(node_t::input_iterator jt=n->inputs_begin(); jt != n->inputs_end(); jt++) {
                node_t* inp = *jt;
                if(inp == n->get_si_input()) {
                    inputset.erase(inp);
                }
            }
        }
    }

    std::ostringstream ostr;
    ostr << "real inputs [" << inputset.size() << "]: " << inputset;
    std::string str(ostr.str());
    mod->add_comment(str);
    std::string axi("axi4s");
    mod->enable_annotation(axi);

    add_module(mod);
}

void flat_module_t::add_marked_fanins(node_t* n, nodeset_t& internals, markings_t& markings)
{
    if(!markings[n->get_index()]) return;
    if(internals.find(n) != internals.end()) return;
    if(n->is_latch() && !markings[n->get_input(0)->get_index()]) return;

    internals.insert(n);
    if(n->is_gate()) {
        if(n->get_sibling()) internals.insert(n->get_sibling());
        if(n->get_sibling_back()) internals.insert(n->get_sibling_back());
    }

    for(node_t::input_iterator it = n->inputs_begin(); it != n->inputs_end(); it++) {
        if(n->is_latch_gate() && (n->get_si_input() == *it || n->get_se_input() == *it)) continue;
        add_marked_fanins(*it, internals, markings);
    }
}

void flat_module_t::analyze_axi_nand()
{
    std::ofstream out("nand4.v");
    nodelist_t interesting_nands;

    for(unsigned i=0; i != gates.size(); i++) {
        if(gates[i]->is_latch_gate()) continue;
        if(gates[i]->get_rsttrees() == 1) {
            const char* name = gates[i]->get_lib_elem()->get_name();
            if(strstr(name, "NAND4_") == name) {
                gates[i]->dump_verilog_comment(out);
                const nodeset_t& driving = *(gates[i]->get_driving_latches());
                const nodeset_t& driven = *(gates[i]->get_driven_latches());

                if(driving.size() == 32 && driven.size() == 5) {
                    interesting_nands.push_back(gates[i]);
                }

                int driving_t = get_rsttrees(driving);
                int driven_t = get_rsttrees(driven);
                out << "  // driving: "; node_t::dump_masked_strings(out, driving_t, options.resetTreeRoots);
                out << ";  driven: "; node_t::dump_masked_strings(out, driven_t, options.resetTreeRoots);
                out << std::endl;

                out << "  // driving : " << driving.size() << " : " << driving << std::endl;
                out << "  // driven  : " << driven.size() << " : " << driven << std::endl;

                gates[i]->dump_verilog_defn(out, true, false, NULL, NULL);
            }
        }
    }

    std::cout << "the number of \"interesting\" nand4 gates is: " << interesting_nands.size() << std::endl;
    process_axi_nand4s(interesting_nands);

    out.close();
}

void flat_module_t::process_axi_nand4s(nodelist_t& nands)
{
    std::map<node_t*, int> input_count, output_count;
    for(unsigned i=0; i != nands.size(); i++) {
        const nodeset_t& driving = *(nands[i]->get_driving_latches());
        const nodeset_t& driven = *(nands[i]->get_driven_latches());

        for(nodeset_t::const_iterator it = driving.begin(); it != driving.end(); it++) {
            input_count[*it] += 1;
        }
        for(nodeset_t::const_iterator it = driven.begin(); it != driven.end(); it++) {
            output_count[*it] += 1;
        }
    }
    std::cout << "total # of driving latches : " << input_count.size() << std::endl;
    std::cout << "total # of driven latches  : " << output_count.size() << std::endl;
    create_xbar_module(nands, input_count, output_count);
}

void flat_module_t::create_xbar_module(const nodelist_t& nands, std::map<node_t*, int>& ins, std::map<node_t*, int>& outs)
{
    using namespace aggr;

    removeNOR2XB(NULL);

    nodeset_t module_outputs;
    std::vector< std::pair< node_t*, node_t* > > outputs;

    markings_t marks(max_index(), false);
    for(std::map<node_t*, int>::iterator it = ins.begin(); it != ins.end(); it++) {
        node_t* n = it->first;
        for(node_t::fanout_iterator jt = n->fanouts_begin(); jt != n->fanouts_end(); jt++) {
            if((*jt)->is_gate() && !((*jt)->is_latch_gate())) {
                mark_fanouts(*jt, marks);
            }
        }
    }

    nodeset_t visited, inputs;

    for(std::map<node_t*, int>::iterator it = outs.begin(); it != outs.end(); it++) {
        node_t* inp = get_latch_input2(it->first);
        module_outputs.insert(inp);

        assert(inp != NULL);
        if(!marks[inp->get_index()]) {
            std::cout << "latch: " << it->first->get_name() << std::endl;
            std::cout << "input: " << inp->get_name() << std::endl;
        }
        assert(marks[inp->get_index()]);

        outputs.push_back( std::pair<node_t*,node_t*>(it->first, inp) );
        xbar_dfs(inp, marks, visited, inputs);
        // std::cout << "DFS ENDED." << std::endl;
    }
    std::cout << "# of module outputs   : " << module_outputs.size() << std::endl;
    std::cout << "# of module inputs    : " << inputs.size() << std::endl;


    using namespace aggr;
    id_module_t* mod = new id_module_t("xbar", id_module_t::UNSLICEABLE);
    for(nodeset_t::iterator it = module_outputs.begin(); it != module_outputs.end(); it++) {
        node_t* n = *it;
        mod->add_output(n);
    }
    for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
        node_t* n = *it;
        mod->add_input(n);
    }

    std::map<int, nodeset_t> groups;
    for(unsigned i=0; i != outputs.size(); i++) {
        node_t* inp = outputs[i].second;
        node_t* lat = outputs[i].first;
        groups[lat->get_rst2()].insert(inp);
    }
    for(std::map<int, nodeset_t>::iterator it = groups.begin(); it != groups.end(); it++) {
        const nodeset_t& s = it->second;
        std::ostringstream stream;
        stream << "output group: ";
        node_t::dump_masked_strings(stream, it->first, options.resetTreeRoots);
        stream << "size: " << s.size() << "; ";
        for(nodeset_t::const_iterator jt=s.begin(); jt != s.end(); jt++) {
            stream << (*jt)->get_name() << " ";
        }
        std::string str = stream.str();
        mod->add_comment(str);
        process_xbar_output_group(mod, s);
    }

    // DUMP counts of usages.
    typedef std::map<node_t*, int> usage_count_map_t;
    usage_count_map_t uses;
    for(nodeset_t::iterator it = module_outputs.begin(); it != module_outputs.end(); it++) {
        node_t* n = *it;
        const nodeset_t& n_inputs = mod->get_inputmap(n);
        for(nodeset_t::const_iterator jt = n_inputs.begin(); jt != n_inputs.end(); jt++) {
            node_t* n = *jt;
            uses[n] += 1;
        }
    }
    // clear all the stupid maps.
    mod->reset_inputmaps();

    for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
        node_t* n = *it;
        assert(uses.find(n) != uses.end());
        std::ostringstream stream;
        stream << "node: " << n->get_name() << "; uses: " << uses[n];
        std::string str = stream.str();
        mod->add_comment(str);
    }

    mod->compute_internals();

    add_module(mod);

    verify_xbar(inputs, module_outputs, uses);
}

void flat_module_t::process_xbar_output_group(aggr::id_module_t* mod, const nodeset_t& bits)
{
    using namespace xbar_n;

    std::map<std::string, int> type_count;
    std::vector<group_t*> groups;

    for(nodeset_t::iterator it = bits.begin(); it != bits.end(); it++) {
        node_t* n = *it;
        node_t* rep = n;
        lib_elem_t* le = n->get_lib_elem();
        assert(le);
        std::string lib_name(le->get_name());
        if(lib_name.substr(0, 6) == "NOR2XB") {
            node_t* inp = n->get_input_by_name("BN");
            if(inp == NULL) {
                std::cout << n->get_name() << std::endl;
            }
            assert(inp);
            le = inp->get_lib_elem();
            assert(le);
            lib_name = le->get_name();
            rep = inp;
        }
        type_count[lib_name] += 1;

        group_t* grp = new_group(n, rep);
        groups.push_back(grp);
    }

    std::ostringstream stream;
    stream << "type counts: ";
    for(std::map<std::string, int>::iterator it = type_count.begin(); it != type_count.end(); it++) {
        stream << it->first << ":" << it->second << "; ";
    }
    std::string str(stream.str());

    for(unsigned i=0; i != groups.size(); i++) {
        group_t* g1 = groups[i];
        for(unsigned j=0; j < i; j++) {
            group_t* g2 = groups[j];
            if(same_group(g1, g2)) {
                // std::cout << "same group: " << g1->gate->get_name() << ", " << g2->gate->get_name() << std::endl;
                xbar_n::link(g1, g2);
            }
        }
    }

    std::map<group_t*, nodeset_t> word_groups;
    for(unsigned i=0; i != groups.size(); i++) {
        group_t* g = groups[i];
        group_t* r = get_root(g);
        word_groups[r].insert(g->gate);
    }

    for(std::map<group_t*, nodeset_t>::iterator it = word_groups.begin(); it != word_groups.end(); it++) {
        nodeset_t& s = it->second;
        std::ostringstream stream2;
        stream2 << "word size: " << s.size() << "-bits; ";

        /*
           nodeset_t inputs;
           bool first = true;
           */

        word_t* w = new word_t(false, word_t::SIMPLE_WORD_ANALYSIS);
        for(nodeset_t::iterator it = s.begin(); it != s.end(); it++) {
            node_t* n = *it;
            stream2 << n->get_name() << " ";
            w->add_bit(n);

        }
        w = get_canonical_word(w);
        add_word(w);

        std::string str2(stream2.str());
        mod->add_comment(str2);

        /*
           std::ostringstream stream3;
           stream3 << "common inputs: " << inputs;
           std::string str3 = stream3.str();
           mod->add_comment(str3);
           */
    }

    for(unsigned i=0; i != groups.size(); i++) {
        delete groups[i];
    }

    mod->add_comment(str);
}

int flat_module_t::count_included_fanouts(node_t* n, aggr::id_module_t* mod)
{
    int cnt = 0;
    for(node_t::fanout_iterator it = n->fanouts_begin(); it != n->fanouts_end(); it++) {
        node_t* f = *it;
        if(mod->is_internal(f)) {
            cnt += 1;
        }
    }
    return cnt;
}

bool flat_module_t::verify_xbar(const nodeset_t& inputs, const nodeset_t& outputs, std::map<node_t*, int>& uses)
{
    Cudd& mgr = getFullFnMgr();
    bdd_map_t vars;
    int idx = 0;

    int num_selects = 0;
    nodeset_t selects;
    // first add the selects.
    for(nodeset_t::const_iterator it = inputs.begin(); it != inputs.end(); it++) {
        node_t* n = *it;
        assert(uses.find(n) != uses.end());
        int cnt = uses[n];
        if(n->get_name() == "xfcq425948") {
            vars[n] = mgr.bddZero();
            selects.insert(n);
        } else if(cnt == 16 || cnt == 18 || cnt >= 80) {
            vars[n] = mgr.bddVar(idx++);
            selects.insert(n);
        }
    }
    num_selects = idx;
    std::cout << "# of select signals: " << num_selects << std::endl;

    // now add the datapath;
    for(nodeset_t::const_iterator it = inputs.begin(); it != inputs.end(); it++) {
        node_t* n = *it;
        if(selects.find(n) == selects.end()) {
            vars[n] = mgr.bddVar(idx++);
        }
    }

    int passes = 0;
    int fails = 0;
    bool failed = false;
    for(nodeset_t::const_iterator it = outputs.begin(); it != outputs.end(); it++) {
        bool result = verify_xbar_node(mgr, vars, *it, num_selects);
        if(!result) {
            fails += 1;
            failed = true;
        } else {
            passes += 1;
        }
        break;
    }
    if(!failed) {
        std::cout << "verification succeeded!" << std::endl;
    } else {
        std::cout << "passes: " << passes << "; fails: " << fails << std::endl;
    }
    return !failed;
}

bool flat_module_t::verify_xbar_node(Cudd& mgr, bdd_map_t& vars, node_t* out, int num_selects)
{
    BDD bdd = createFullFn(out, vars, false, -1);
    BDD bdd_supp = bdd.Support();
    std::vector<BDD> selects;
    std::vector<BDD> inputs;
    for(int i = 0; i != num_selects; i++) {
        BDD s = mgr.bddVar(i);
        if((bdd_supp & s) == bdd_supp) {
            selects.push_back(s);
        } else {
            inputs.push_back(s);
        }
    }

    std::cout << "# of vars: " << bdd_supp.SupportSize() << std::endl;
    std::cout << "# of sels: " << selects.size() << std::endl;

    for(unsigned i=0; i != inputs.size(); i++) {
        BDD xi = inputs[i];
        BDD res = bdd.Xnor(xi);
        std::cout << "reqd: "; printBDD(stdout, res);
    }

    return true;
}


void flat_module_t::xbar_dfs(node_t* n, markings_t& marks, nodeset_t& visited, nodeset_t& inputs)
{
    // std::cout << "dfs: " << n->get_name() << std::endl;

    if(visited.find(n) != visited.end()) { return; }
    visited.insert(n);

    if(marks[n->get_index()] == false) {
        inputs.insert(n);
        return;
    } else {
        for(node_t::input_iterator it = n->inputs_begin(); it != n->inputs_end(); it++) {
            node_t* inp = *it;
            xbar_dfs(inp, marks, visited, inputs);
        }
    }
}

node_t* flat_module_t::get_latch_input2(node_t* latch_out)
{
    node_t* lg = latch_out->get_input(0);
    node_t* n = lg->get_input_by_name("A");
    if(n != NULL) return n;
    else {
        n = lg->get_input_by_name("D");
        return n;
    }
}

void flat_module_t::mark_fanouts(node_t* n, markings_t& marks)
{
    if(marks[n->get_index()]) return;
    else {
        marks[n->get_index()] = true;
        for(node_t::fanout_iterator it = n->fanouts_begin(); it != n->fanouts_end(); it++) {
            node_t* f = *it;
            if(f->is_gate() && !f->is_latch_gate()) {
                mark_fanouts(f, marks);
            }
        }
    }
}

int flat_module_t::get_rsttrees(const nodeset_t& set)
{
    int rst = 0;
    for(nodeset_t::const_iterator it = set.begin(); it != set.end(); it++) {
        node_t* n = *it;
        rst = rst | (n->get_rsttrees());
    }
    return rst;
}

void flat_module_t::analyze_a2dff(std::ostream& out)
{
    compute_latch_connections();

    analyze_axi_nand();

    const char* name = "A2SDFFQ";

    typedef std::map<node_t*, nodelist_t> ffmap_t;
    typedef std::map<node_t*, node_t*> node_map_t;
    ffmap_t ffmap;
    node_map_t node_map;

    markings_t markings(max_index(), false);

    unsigned initcnt = 0;
    for(unsigned i=0; i != latches.size(); i++) {
        node_t* n = latches[i];
        const char* this_name = n->get_joint_module();
        if(strstr(this_name, name) == this_name) {
            node_t* lg = n->get_input(0);
            assert(lg->is_latch_gate());

            node_t* b = lg->get_input_by_name("B");
            assert(b);

            int index = n->get_index();
            markings[index] = true;
            initcnt += 1;

            ffmap[b].push_back(n);
            node_map[n] = b;
        }
    }

    // std::cout << "initially marked " << initcnt << " gates. " << std::endl;
    // create_a2dff_module(markings);

    typedef std::map<node_t*, nodeset_t> edges_t;
    edges_t edges_in;

    // std::ofstream flops("a2sdffq.txt");

    edges_t edges_in_dbg;

    for(ffmap_t::iterator it = ffmap.begin(); it != ffmap.end(); it++) {
        node_t* b_node = it->first;
        nodelist_t& list = it->second;

        /*
           bool dbg = false;
           if(b_node->get_name() == "affs21780") {
           dbg = true;
           }
           */

        for(unsigned i=0; i != list.size(); i++) {
            node_t* l = list[i];

            /*
               flops << "Q:" << l->get_name() << " A:" << l->get_input(0)->get_input_by_name("A")->get_name()
               << " B:" << l->get_input(0)->get_input_by_name("B")->get_name()
               << " rst:";
               node_t::dump_masked_strings(flops, l->get_rsttrees(), options.resetTreeRoots);
               flops << std::endl;
               */

            const nodeset_t* dl = l->get_driving_latches();
            for(nodeset_t::const_iterator it = dl->begin(); it != dl->end(); it++) {
                node_t* latch = *it;
                node_map_t::iterator pos = node_map.find(latch);
                if(pos != node_map.end()) {
                    node_t* other_b = pos->second;
                    edges_in[b_node].insert(other_b);
                    /*
                       if(dbg) {
                       if(other_b->get_name() == "affs21780") {
                       edges_in_dbg[l].insert(latch);
                       }
                       }
                       */
                }
            }
        }
    }

    // flops.close();
    if(edges_in_dbg.size()) {
        std::ofstream fout("dbg.dot");
        fout << "digraph G { " << std::endl;
        for(edges_t::iterator it = edges_in_dbg.begin(); it != edges_in_dbg.end(); it++) {
            node_t* in = it->first;
            const nodeset_t& s = it->second;
            for(nodeset_t::iterator it = s.begin(); it != s.end(); it++) {
                fout << "  ";
                fout << "\"" << in->get_name() << "\"";
                fout << " -> ";
                fout << "\"" << (*it)->get_name() << "\"";
                fout << std::endl;
            }
        }
        fout << "}" << std::endl;
        fout.close();
    }

    out << "digraph G { " << std::endl;

    /*
       node_t* se = get_node_by_name("xfen425997");
       assert(se != NULL);
       */

    for(edges_t::iterator it = edges_in.begin(); it != edges_in.end(); it++) {
        node_t* in = it->first;
        dump_dot_node(out, in, ffmap);

        // BDD CREATION.
        /*
           bdd_map_t vars;
           Cudd& mgr = getFullFnMgr();
           vars[se] = mgr.bddZero();
           for(unsigned i=0; i != options.resetTreeRoots.size(); i++) {
           node_t* n = get_pad_port(get_node_by_name(options.resetTreeRoots[i].c_str()));
           vars[n] = mgr.bddOne();
           }
           unsigned sz=vars.size();
           BDD fn = createFullFn(in, vars, false, -1);
           const nodeset_t& n = in->get_fanin_cone_inputs();
           std::cout << in->get_name() << " : " << n << std::endl;
           if(vars.size() == sz) {
           std::cout << in->get_name() << ": "; printBDD(stdout, fn);
           } else {
           std::cout << in->get_name() << ": FAILED: " << vars.size() << std::endl;
           }
           */
    }

    for(edges_t::iterator it = edges_in.begin(); it != edges_in.end(); it++) {
        node_t* in = it->first;
        const nodeset_t& s = it->second;
        for(nodeset_t::iterator it = s.begin(); it != s.end(); it++) {
            out << "  ";
            dump_dot_name(out, in, ffmap);
            out << " -> ";
            dump_dot_name(out, *it, ffmap);
            out << std::endl;
        }
    }
    out << "}" << std::endl;
}

void flat_module_t::dump_dot_name(std::ostream& out, node_t* n, std::map<node_t*, nodelist_t>& ffmap)
{
    assert(ffmap.find(n) != ffmap.end());

    int rst = 0;
    nodelist_t& l = ffmap[n];
    for(unsigned i=0; i != l.size(); i++) {
        rst |= l[i]->get_rsttrees();
    }

    out << "\"";
    out << n->get_name() << ":";
    node_t::dump_masked_strings(out, rst, options.resetTreeRoots);
    out << l.size();
    out << "\"";
}


void flat_module_t::dump_dot_node(std::ostream& out, node_t* n, std::map<node_t*, nodelist_t>& ffmap)
{
    assert(ffmap.find(n) != ffmap.end());

    int rst = 0;
    nodelist_t& l = ffmap[n];
    for(unsigned i=0; i != l.size(); i++) {
        rst |= l[i]->get_rsttrees();
    }

    out << "  node [shape=box] \"";
    out << n->get_name() << ":";
    node_t::dump_masked_strings(out, rst, options.resetTreeRoots);
    out << l.size();
    out << "\";" << std::endl;
}


void flat_module_t::dump_nodecounts(std::ostream& out)
{
    if(!error) {
        out << "module name       : " << std::setw(16) << module_name << std::endl;
        out << "number of inputs  : " << std::setw(16) << inputs.size() << std::endl;
        out << "number of outputs : " << std::setw(16) << outputs.size() << std::endl;
        out << "number of gates   : " << std::setw(16) << num_real_gates() << std::endl;
        out << "number of latches : " << std::setw(16) << latches.size() << std::endl;
    } else {
        out << "module is invalid." << std::endl;
    }
}

void flat_module_t::topo_sort()
{
    std::cout << "inside toposort." << std::endl;
    bool changed = false;
    int iterations = 0;

    nodelist_t loop_nodes;

    bool first = true;
    do {
        changed = false;
        if(first) {
            first = false;
            for(nodelist_t::iterator it = latches.begin(); it != latches.end(); it++) {
                node_t* n = *it;
                n->update_level();
            }
            for(nodelist_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
                node_t* n = *it;
                n->update_level();
            }
            for(nodelist_t::iterator it = macros.begin(); it != macros.end(); it++) {
                node_t* n = *it;
                n->update_level();
            }
            for(nodelist_t::iterator it = macro_outs.begin(); it != macro_outs.end(); it++) {
                node_t* n = *it;
                n->update_level();
            }
        }

        for(nodelist_t::iterator it = gates.begin(); it != gates.end(); it++) {
            node_t* n = *it;
            if(n->update_level()) {
                if(iterations == options.maxTopoSortIterations) {
                    loop_nodes.push_back(n);
                }
                changed = true;
            }
        }
        iterations += 1;
    } while(changed && (iterations <= options.maxTopoSortIterations));
    if(loop_nodes.size() > 0) {
        std::cout << "gave up topological sort as " << loop_nodes.size() << " gates appear to be part of a loop." << std::endl;
        mark_loop(loop_nodes);
    }
    std::cout << "toposort finished after " << iterations << " iterations." << std::endl;

    std::sort(gates.begin(), gates.end(), toposort_cmp());
}

void flat_module_t::mark_loop(nodelist_t& visited)
{
    for(nodelist_t::iterator it = visited.begin(); it != visited.end(); it++) {
        node_t* n = *it;
        n->set_logic_loop();
    }
}

void flat_module_t::test_jan23()
{
    compute_latch_connections();
    node_t* n = get_node_by_name("cok2402");
    assert(n);
    const nodeset_t* s = n->get_driven_latches();
    std::cout << s->size() << " elements." << std::endl;

    n = get_node_by_name("cpq2434");
    assert(n);
    s = n->get_driving_latches();
    std::cout << s->size() << " elements." << std::endl;
}

void flat_module_t::test_mar8()
{
    using namespace aggr;

    int n = 3;

    std::vector<BDD> bdds;
    computePopCountBDDs(getFullFnMgr(), n, bdds);

    for(unsigned i=0; i != bdds.size(); i++) {
        std::cout << "bit " << i << ":"; printBDD(stdout, bdds[i]);
    }
    bdds.clear();
}

void flat_module_t::compute_latch_connections()
{
    if(lcg_computed) return;

    bool changed = false;
    // compute driven latches first.
    int iterations = 0;
    do {
        changed = false;
        for(nodelist_t::iterator it = latches.begin(); it != latches.end(); it++) {
            node_t* n = *it;
            if(n->update_driven_latches(false)) changed = true;
        }
        for(nodelist_t::reverse_iterator it = gates.rbegin(); it != gates.rend(); it++) {
            node_t* n = *it;
            if(n->update_driven_latches(false)) changed = true;
        }
        iterations++;
    } while(changed);

    // now update the macros.
    for(nodelist_t::iterator it = macros.begin(); it != macros.end(); it++) {
        node_t* n = *it;
        if(n->update_driven_latches(true)) changed = true;
    }
    for(nodelist_t::iterator it = macro_outs.begin(); it != macro_outs.end() ; it++) {
        node_t* n = *it;
        if(n->update_driven_latches(true)) changed = true;
    }

    // print out some stats.
    std::cout << "driven latches computed after " << iterations << " iterations." << std::endl;
    unsigned max_dls = 0, min_dls = INT_MAX;
    double sum_dls = 0;
    for(nodelist_t::iterator it = latches.begin(); it != latches.end(); it++) {
        node_t* n = *it;
        unsigned dl = n->get_driven_latches()->size();
        max_dls = dl > max_dls ? dl : max_dls;
        min_dls = dl < min_dls ? dl : min_dls;
        sum_dls += dl;
    }
    //std::cout << "max number of driven latches: " << max_dls << std::endl;
    //std::cout << "min number of driven latches: " << min_dls << std::endl;
    //std::cout << "avg number of driven latches: " << sum_dls / latches.size() << std::endl;

    // compute driving latches next.
    iterations = 0;
    do {
        changed = false;
        for(nodelist_t::iterator it = latches.begin(); it != latches.end(); it++) {
            node_t* n = *it;
            if(n->update_driving_latches(false)) changed = true;
        }
        for(nodelist_t::iterator it = gates.begin(); it != gates.end(); it++) {
            node_t* n = *it;
            if(n->update_driving_latches(false)) changed = true;
        }
        iterations++;
    } while(changed);

    // again update the macros.
    for(nodelist_t::iterator it = macros.begin(); it != macros.end(); it++) {
        node_t* n = *it;
        if(n->update_driving_latches(true)) changed = true;
    }
    for(nodelist_t::iterator it = macro_outs.begin(); it != macro_outs.end() ; it++) {
        node_t* n = *it;
        if(n->update_driving_latches(true)) changed = true;
    }

    // print out some stats.
    //std::cout << "driving latches computed after " << iterations << " iterations." << std::endl;
    max_dls = 0; min_dls = INT_MAX;
    sum_dls = 0;

    unsigned max_dlsp = 0, min_dlsp= INT_MAX;
    double sum_dlsps = 0;

    for(nodelist_t::iterator it = latches.begin(); it != latches.end(); it++) {
        node_t* n = *it;
        unsigned dl = n->get_driving_latches()->size();
        max_dls = dl > max_dls ? dl : max_dls;
        min_dls = dl < min_dls ? dl : min_dls;
        sum_dls += dl;

        unsigned dlsp = n->get_driving_latches_sp()->size();
        max_dlsp = dlsp > max_dlsp ? dlsp : max_dlsp;
        min_dlsp = dlsp < min_dlsp ? dlsp : min_dlsp;
        sum_dlsps += dlsp;
    }
    //std::cout << "max number of driving latches: " << max_dls << std::endl;
    //std::cout << "min number of driving latches: " << min_dls << std::endl;
    //std::cout << "avg number of driving latches: " << sum_dls / latches.size() << std::endl;

    for(nodelist_t::iterator it = gates.begin(); it != gates.end(); it++) {
        node_t* n = *it;
        n->compute_driving_latches_mp();
    }
    for(nodelist_t::iterator it = latches.begin(); it != latches.end(); it++) {
        node_t* n = *it;
        n->compute_driving_latches_mp();
    }

    for(nodelist_t::iterator it = latches.begin(); it != latches.end(); it++) {
        node_t* n = *it;
        unsigned dlsp = n->get_driving_latches_sp()->size();
        max_dlsp = dlsp > max_dlsp ? dlsp : max_dlsp;
        min_dlsp = dlsp < min_dlsp ? dlsp : min_dlsp;
        sum_dlsps += dlsp;
    }
    //std::cout << "max number of driving latches [sp]: " << max_dlsp << std::endl;
    //std::cout << "min number of driving latches [sp]: " << min_dlsp << std::endl;
    //std::cout << "avg number of driving latches [sp]: " << sum_dlsps / latches.size() << std::endl;
    lcg_computed = true;
}

void flat_module_t::simple_word_analysis()
{
    // hardcoding the multiplexer names. sorry.
    const char* muxi = "MXIT2_X0P5M_A12TS";
    const char* mux  = "MXT2_X0P5M_A12TS";
    // map between select signals and muxes that use them.
    std::map<node_t*, nodelist_t> mxsel_map;

    for(nodelist_t::iterator it  = gates.begin();
            it != gates.end();
            it++)
    {
        node_t* n = *it;
        if(strcmp(n->get_lib_elem_name(), muxi) == 0 ||
                strcmp(n->get_lib_elem_name(), mux) == 0)
        {
            // more hardcoding :-(
            node_t* sel = n->get_input_by_name("s0");
            mxsel_map[sel].push_back(n);
        }
    }
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "          SIMPLE WORD ANALYSIS                             " << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "number of multiplexer groups: " << mxsel_map.size() << std::endl;
    std::cout << "words being created ... " << std::endl;
    for(std::map<node_t*, nodelist_t>::iterator it  = mxsel_map.begin();
            it != mxsel_map.end();
            it++)
    {
        node_t* sel = it->first;
        nodelist_t& nlst = it->second;
        if(nlst.size() % 8 == 0) {
            std::cout << "muxsel: " << std::left << std::setw(30) << sel->get_name() << "size: " << std::setw(5) << nlst.size() << std::endl;
            word_t* w = new word_t(false /* not ordered. */, word_t::SIMPLE_WORD_ANALYSIS);
            for(nodelist_t::iterator it =  nlst.begin(); it != nlst.end(); it++) {
                node_t* bit = *it;
                w->add_bit(bit);
            }
            w = get_canonical_word(w);
            if(w->size() > 4) {
                add_word(w);
            } else {
                delete w;
            }
        }
    }
    std::cout << "-----------------------------------------------------------" << std::endl;
}

void flat_module_t::add_word(word_t* w)
{
    if(w) {
        if(word_exists(w)) {
            return;
        }
        for(word_t::iterator it = w->begin(); it != w->end(); it++) {
            node_t* bit = *it;
            bit->add_word(w);
        }

        w->set_index(words.size());
        w->setLocked();
        word_htbl[w->get_sign()].push_back(w);
        words.push_back(w);
    }
}

word_t* flat_module_t::get_canonical_word(word_t* w)
{
    word_t::sign_t s = w->get_sign();
    if(word_htbl.find(s) == word_htbl.end()) return w;
    else {
        for(wordptr_list_t::iterator it = word_htbl[s].begin(); it != word_htbl[s].end(); it++) {
            word_t* v = *it;
            if(v->same_word(*w)) {
                // kill this word and use the other one.
                if(w->rf_word()) {
                    v->setSrc(w->getSrc());
                    v->set_module(w->get_module());
                    if(w->is_input_word()) v->set_input_word();
                    if(w->is_output_word()) v->set_output_word();
                }
                delete w;
                return v;
            }
        }
        return w;
    }
}

bool flat_module_t::similar_word_exists(word_hashtable_t& htbl, word_t* w) const
{
    word_t::sign_t s = w->get_sign();
    word_hashtable_t::const_iterator pos = htbl.find(s);
    if(pos == htbl.end()) return false;
    else {
        const wordptr_list_t& wl = pos->second;
        for(wordptr_list_t::const_iterator it = wl.begin(); it != wl.end(); it++) {
            const word_t* v = *it;
            if(v->same_bits(*w)) return true;
        }
        return false;
    }
}

bool flat_module_t::word_exists(word_t* w) const
{
    word_t::sign_t s = w->get_sign();
    word_hashtable_t::const_iterator pos = word_htbl.find(s);
    if(pos == word_htbl.end()) return false;
    else {
        const wordptr_list_t& wl = pos->second;
        for(wordptr_list_t::const_iterator it = wl.begin(); it != wl.end(); it++) {
            const word_t* v = *it;
            if(v->same_word(*w)) return true;
        }
        return false;
    }
}

void flat_module_t::find_loops()
{
    nodelist_t best_path;
    for(unsigned i=0; i != gates.size(); i++) {
        if(gates[i]->is_latch_gate()) continue;

        for(node_t::input_iterator it = gates[i]->inputs_begin(); it != gates[i]->inputs_end(); it++) {
            node_t* inp = *it;
            if(inp->is_gate()) {
                nodeset_t visited;
                nodelist_t path;
                do_loop_dfs(inp, gates[i], visited, path, best_path);
            }
        }
        printf("current node: %6d/%6d\r", (int)i,(int)gates.size());
        fflush(stdout);
    }
    printf("\n");
    std::cout << "best path: " << best_path << std::endl;
}

void flat_module_t::do_loop_dfs(node_t* n, node_t* root, nodeset_t& visited, nodelist_t& path, nodelist_t& best_path)
{
    if(n == root) {
        path.push_back(n);
        if((path.size() < best_path.size() || best_path.size() == 0)) {
            best_path.resize(path.size());
            std::copy(path.begin(), path.end(), best_path.begin());
            std::cout << "\nnew best path: " << best_path << std::endl;
        }
        path.pop_back();
        return;
    }

    if(visited.find(n) != visited.end()) return;

    visited.insert(n);
    path.push_back(n);

    for(node_t::input_iterator it = n->inputs_begin(); it != n->inputs_end(); it++) {
        node_t* inp = *it;
        if(inp->is_gate()) {
            do_loop_dfs(inp, root, visited, path, best_path);
        }
    }

    path.pop_back();
}

void flat_module_t::kcover_analysis()
{
    int n = 1;
    int total = options.ramAnalysis ? gates.size() : num_real_gates();

    unsigned long max_covers = 0;
    double sum_covers = 0;
    numPhonyCovers = 0;

    int cnt =  0;
    for(nodelist_t::iterator it=gates.begin(); it != gates.end(); it++, cnt++) {
        node_t* gate = *it;
        if(gate->is_latch_gate() && !options.ramAnalysis) continue;
        if(gate->in_logic_loop()) continue;
        if(gate->is_dead()) continue;

        gate->k_inputs.resize(gate->num_inputs());
        std::copy(gate->inputs_begin(), gate->inputs_end(), gate->k_inputs_begin());
        std::sort(gate->k_inputs_begin(), gate->k_inputs_end(), kcover_cnt_cmp());

        kcoverlist_t& kcovs = gate->get_kcovers();
        if(kcovs.size() > max_covers) { max_covers = kcovs.size(); }
        sum_covers += kcovs.size();

        int sz = kcovs.size();
        kcover_size_cnt[sz] += 1;

        for(kcoverlist_t::iterator kt = kcovs.begin(); kt != kcovs.end(); kt++) {
            kcover_t* kc = *kt;
            int min_dep = kc->get_min_depth();
            int max_dep = kc->get_max_depth();
            kcover_min_depth[min_dep] += 1;
            kcover_max_depth[max_dep] += 1;
            createFunction(kc);
        }
        fprintf(stderr, "PROGRESS: %5d/%5d\r", n++, total); fflush(stdout);
    }
    fprintf(stderr, "\n");
    double avg_covers = sum_covers / (num_real_gates());
    std::cout << "MAX k-covers: " << max_covers << std::endl;
    std::cout << "SUM k-covers: " << sum_covers << std::endl;
    std::cout << "AVG k-covers: " << avg_covers << std::endl;
    std::cout << "Prune count : " << prune_count << std::endl;
    std::cout << "Cover count : " << cover_count << std::endl;
    std::cout << "Phony covers: " << numPhonyCovers << std::endl;
    std::cout << "Excl11 size : " << exclusive11Relation.size() << std::endl;
    dump_kcover_info_file();
    dump_fn_stats();

    create_not_relation();
    match_lib_elems();
    // bdd_sweep();
}

void flat_module_t::match_lib_elems()
{
    FILE* fp = NULL;
    if(options.bitsliceMatchFile.size() > 0) {
        fp = fopen(options.bitsliceMatchFile.c_str(), "wt");
        if(fp == NULL) {
            fprintf(stderr,
                    "Unable to create file: %s.\n",
                    options.bitsliceMatchFile.c_str());
            exit(1);
        }
    }

    for(flat_module_list_t::iterator it =  libraryElements.begin();
            it != libraryElements.end();
            it++)
    {
        flat_module_t* libelem = *it;
        for(unsigned i=0; i != libelem->outputBDDs.size(); i++) {
            BDD bi = libelem->outputBDDs[i];
            fnInfo_t* fi = getFunction(bi);
            if(fi != NULL) {
                processLibElemMatch(fp, libelem, i, fi->canonicalPtr, 0);
            }
            BDD bin = !bi;
            fi = getFunction(bin);
            if(fi != NULL) {
                processLibElemMatch(fp, libelem, i, fi->canonicalPtr, 1);
            }
        }
    }
    if(fp) {
        fclose(fp);
        fp = NULL;
    }
}

void flat_module_t::processLibElemMatch(
        FILE* fp,
        flat_module_t* module,
        int i,
        fnInfo_t* info,
        int polarity
        )
{
    assert(info);

    if(fp) {
        fprintf(fp,
                "module/output/polarity: %-24s/%3d/%s\t matches:%10lu.\n",
                module->get_module_name(),
                i,
                (polarity ? "-" : "+"),
                info->canonicalPtr->covers.size());
        for(kcoverset_t::iterator it =  info->canonicalPtr->covers.begin();
                it != info->canonicalPtr->covers.end();
                it++)
        {
            kcover_t* kc = *it;
            kc->get_root()->add_match(module, i, polarity, kc);
        }
    }
}

void flat_module_t::create_not_relation()
{
    input_provider_t* ipp = get_ipp(1);
    BDD notBDD = !ipp->inp(0);
    fnInfo_t* notInfo = getFunction(notBDD);
    for(std::set<kcover_t*>::iterator i = notInfo->covers.begin();
            i != notInfo->covers.end();
            i++) {
        kcover_t* cov = *i;
        node_t* y = cov->get_root();
        node_t* x = cov->at(0);

        notRelation[x].insert(y);
        notRelation[y].insert(x);
    }

    ipp = get_ipp(2);

    BDD orBDD = ipp->inp(0) + ipp->inp(1);
    BDD norBDD = !orBDD;
    markNotRelation(orBDD, norBDD);

    BDD andBDD = ipp->inp(0) & ipp->inp(1);
    BDD nandBDD = !andBDD;
    markNotRelation(andBDD, nandBDD);

}

void flat_module_t::markNotRelation(BDD& x, BDD& notx)
{
    fnInfo_t* fn = getFunction(x);
    fnInfo_t* notfn = getFunction(notx);

    if(fn == NULL || notfn == NULL) return;
    fn = fn->canonicalPtr;
    notfn = notfn->canonicalPtr;

    typedef std::map<node_pair_t, nodeset_t> fn_map_t;
    fn_map_t map;
    for(kcoverset_t::iterator it = fn->covers.begin(); it != fn->covers.end(); it++) {
        kcover_t* kc = *it;
        assert(kc->size() == 2);
        node_pair_t p = createSymmPair(kc->at(0), kc->at(1));
        map[p].insert(kc->get_root());
    }

    for(kcoverset_t::iterator it = notfn->covers.begin(); it != notfn->covers.end(); it++) {
        kcover_t* kc = *it;
        assert(kc->size() == 2);
        node_pair_t p = createSymmPair(kc->at(0), kc->at(1));
        fn_map_t::iterator pos = map.find(p);
        if(pos == map.end()) continue;
        nodeset_t& nodes = pos->second;
        node_t* rt = kc->get_root();
        for(nodeset_t::iterator it = nodes.begin(); it != nodes.end(); it++) {
            node_t* y = *it;
            notRelation[rt].insert(y);
            notRelation[y].insert(rt);
        }
    }
}

bool flat_module_t::not_related(node_t* x, node_t* y) const
{
    not_relation_t::const_iterator pos = notRelation.find(x);
    if(pos == notRelation.end()) return false;
    else {
        return (pos->second.find(y) != pos->second.end());
    }
}

const nodeset_t* flat_module_t::get_not_related(node_t* x) const
{
    not_relation_t::const_iterator pos = notRelation.find(x);
    if(pos == notRelation.end()) return NULL;
    else {
        const nodeset_t& s = pos->second;
        return &s;
    }
}

bool flat_module_t::exclusive11_related(node_t* a, node_t* b) const
{
    node_pair_t p(a, b);
    if(exclusive11Relation.find(p) == exclusive11Relation.end()) return false;
    else return true;
}


void flat_module_t::dump_kcover_info_file()
{
    const bool dump_kcover_info = false;
    if(dump_kcover_info) {
        std::ofstream fout("kcover-info.txt");
        fout << "KCOVER SIZES" << std::endl;
        for(std::map<int, int>::iterator it  = kcover_size_cnt.begin();
                it != kcover_size_cnt.end();
                it++)
        {
            fout << std::setw(10) << it->first << " : " << std::setw(7) << it->second << std::endl;
        }
        fout << "KCOVER MIN DEPTH" << std::endl;
        for(std::map<int, int>::iterator it  = kcover_min_depth.begin();
                it != kcover_min_depth.end();
                it++)
        {
            fout << std::setw(10) << it->first << " : " << std::setw(7) << it->second << std::endl;
        }
        fout << "KCOVER MAX DEPTH" << std::endl;
        for(std::map<int, int>::iterator it  = kcover_max_depth.begin();
                it != kcover_max_depth.end();
                it++)
        {
            fout << std::setw(10) << it->first << " : " << std::setw(7) << it->second << std::endl;
        }
        fout.close();
    }
}

int flat_module_t::get_count(BDD bdd)
{
    if(bdds.find(bdd) == bdds.end()) return 0;
    else {
        fnInfo_t& fninfo = bdds[bdd];
        return fninfo.nodes.size();
    }
}

struct bdd_count_compare_t
{
    flat_module_t* module;
    bdd_count_compare_t(flat_module_t* mod) : module(mod) {}

    bool operator() (BDD l, BDD r) {
        return module->get_count(l) > module->get_count(r);
    }
};

void flat_module_t::dump_fn_stats()
{
}

BDD flat_module_t::ipp_t::inp(int i)
{
    return mgr->bddVar(i);
}

BDD flat_module_t::ipp_t::one()
{
    return mgr->bddOne();
}

BDD flat_module_t::ipp_t::zero()
{
    return mgr->bddZero();
}

void flat_module_t::setup_cudd()
{
    mgrs.resize(options.kcoverTh+1);
    ipps.resize(mgrs.size());
    for(unsigned i=0; i != mgrs.size(); i++) {
        mgrs[i] = new Cudd();
        ipps[i] = new ipp_t(mgrs[i]);
    }
    full_ipp = new ipp_t(&getFullFnMgr());
}

void flat_module_t::destroy_cudd()
{
    for(unsigned i=0; i != mgrs.size(); i++) {
        delete mgrs[i];
    }
    for(unsigned i=0; i != ipps.size(); i++) {
        delete ipps[i];
    }
    delete full_ipp;
}

void flat_module_t::markExclusive11s(kcover_t* cover, BDD& fn, input_provider_t* e)
{
    for(unsigned i=0; i != cover->numInputs(); i++) {
        BDD xi = e->inp(i);
        if(fn.Cofactor(xi) == e->zero()) {
            exclusive11Relation.insert( node_pair_t(cover->at(i), cover->get_root()) );
            exclusive11Relation.insert( node_pair_t(cover->get_root(), cover->at(i)) );
        }
    }
}

void flat_module_t::bdd_sweep()
{
    std::map<BDD, node_t*, bdd_compare_t> fns;
    for(unsigned i=0; i != gates.size(); i++)
    {
        node_t* gate = gates[i];
        kcoverlist_t& kcovs = gate->get_kcovers();
        for(kcoverlist_t::iterator it =  kcovs.begin();
                it != kcovs.end();
                it++)
        {
            kcover_t* kc = *it;
            BDD b = kc->getCktFn(get_ipp(-1));
            std::cout << *kc << std::endl;
            printBDD(stdout, b);
            if(fns.find(b) == fns.end()) {
                fns[b] = gate;
            } else {
                node_t* n = fns[b];
                std::cout << n->get_name() << " == " << gate->get_name() << std::endl;
                printBDD(stdout, b);
            }
        }
    }
}

void flat_module_t::createFunction(kcover_t* cover)
{
    int numInputs = cover->size();
    assert(numInputs < (int)ipps.size());
    BDD fn = cover->getFn(ipps[numInputs]);

    // markExclusive11s(cover, fn, ipps[numInputs]);

    int numFnInputs = fn.SupportSize();
    if(numFnInputs != numInputs) {
        numPhonyCovers += 1;
    }

    node_t* root = cover->get_root();
    (void)root;

    if(bdds.find(fn) == bdds.end()) {
        // permutation vector.
        std::vector<uint8_t> perm(numInputs);
        for(int i=0;i != numInputs; i++) perm[i] = i;

        bool first=true;
        BDD canon;
        fnInfo_t* canonical = NULL;

        do {
            kcover_t cov(root);

            for(int i=0; i != numInputs; i++) cov.add_leaf(cover->at(perm[i]));
            BDD b = cov.getFn(ipps[numInputs]);

            if(first) {
                assert(bdds.find(b) == bdds.end());
                canon = b;
                first = false;

                bdds[canon] = fnInfo_t(numInputs, perm, root, cover, NULL);
                canonical = bdds[canon].canonicalPtr = &(bdds[canon]);
                bdds[canon].isCanonical = true;
                cover->setCanonical(canonical);
                cover->setPerm(bdds[canon].permutation);
            } else {
                if(bdds.find(b) == bdds.end()) {
                    bdds[b] = fnInfo_t(numInputs, perm, NULL, NULL, canonical);
                    bdds[b].isCanonical = false;
                    cover->setPerm(bdds[b].permutation);
                }
            }
        } while(next_permutation(perm.begin(), perm.end()));
    } else {
        BDD other = (bdds.find(fn)->first);
        bdds[fn].canonicalPtr->nodes.insert(root);
        bdds[fn].canonicalPtr->covers.insert(cover);
        cover->setCanonical(bdds[fn].canonicalPtr);
        cover->setPerm(bdds[other].getPerm());
    }
}

fnInfo_t* flat_module_t::getFunction(const BDD& fn)
{
    bddset_t::const_iterator it = bdds.find(fn);
    if(it == bdds.end()) {
        return NULL;
    } else {
        fnInfo_t* fninfo = (fnInfo_t*) (&(it->second));
        return fninfo;
    }
}

void flat_module_t::findCounters(std::string& vfn)
{
    compute_latch_connections();

    //std::string filename("/u/bcakir/bsim/verilog/8051/8051/trunk/sim/rtl_sim/run/vcd/record.vcd");
    //std::string filename("/u/bcakir/bsim/test_articles/ta1risc_fpu/sim/ta1risc.vcd");
    if(vfn.size()>0) {
        vcd_n::vcd_file_t vf(vfn);
        vf.counter_analysis2(this);
    }

    find_counters(latches);
}


void flat_module_t::traceWords(std::string& vfn)
{
    compute_latch_connections();

    if(vfn.size()>0) {
        vcd_n::vcd_file_t vf(vfn);
        vf.traceWordsinVCD(this, inputs);
    }
}


void flat_module_t::findShiftRegs()
{
    compute_latch_connections();
    shiftreg_n::find_shiftregs(this);
}

void flat_module_t::dump_intersecting_nodes(node_t* n)
{
    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t* nd = it->second;
        const nodeset_t* s1 = nd->get_driven_latches();
        const nodeset_t* s2 = nd->get_driving_latches();
        if(s1->find(n) != s1->end() &&
                s2->find(n) != s2->end())
        {
            nd->dump(std::cout);
        }
    }
}

void flat_module_t::dump_driven_latches()
{
    for(unsigned i=0; i != latches.size(); i++) {
        node_t* n = latches[i];
        const nodeset_t* s = n->get_driven_latches();
        std::cout << "latch.driven:" << n->get_name() << " : " << *s << std::endl;
    }
    for(unsigned i=0; i != gates.size(); i++) {
        node_t* n = gates[i];
        const nodeset_t* s = n->get_driven_latches();
        std::cout << "gate.driven:" << n->get_name() << " : " << *s << std::endl;
    }
}

void flat_module_t::dump_driving_latches()
{
    for(unsigned i=0; i != latches.size(); i++) {
        node_t* n = latches[i];
        const nodeset_t* s = n->get_driving_latches();
        std::cout << "latch.driving:" << n->get_name() << " : " << *s << std::endl;
    }
    for(unsigned i=0; i != gates.size(); i++) {
        node_t* n = gates[i];
        const nodeset_t* s = n->get_driving_latches();
        std::cout << "gate.driving:" << n->get_name() << " : " << *s << std::endl;
    }
}

void flat_module_t::simpleFIFOAnalysis2()
{
    node_graph_t graph;
    int cnt=0;
    for(unsigned i=0; i != latches.size(); i++) {
        node_t* l = latches[i];
        for(node_t::fanout_iterator it = l->fanouts_begin(); it != l->fanouts_end(); it++) {
            node_t* n = *it;
            if(n->is_latch_gate() && n->get_si_input() != l) {

                assert(n->num_fanouts() == 1);
                node_t* l2 = *n->fanouts_begin();
                /*
                   if(l2->get_name() == "aaap18267") {
                   if(n->get_si_input()) {
                   std::cout << "si: " << n->get_si_input()->get_name() << std::endl;
                   } else {
                   std::cout << "si is NULL." << std::endl;
                   std::cout << "libelem:" << n->get_lib_elem()->get_name() << std::endl;
                   }
                   }
                   */
                graph.add_edge(l, l2);
                cnt += 1;
            }
        }
    }
    std::cout << "# of edges is " << cnt << std::endl;

    std::ofstream fout(options.simpleFIFOAnalysisOut.c_str(), std::fstream::app);
    for(unsigned i=0; i != latches.size(); i++) {
        node_t* n = latches[i];
        if(graph.num_edges_in(n) == 0 &&
                graph.num_edges_out(n) > 0)
        {
            nodeset_t set;
            graph.do_dfs(n, set);
            fout << "size: " << set.size() << "; latch-chain-set: ";
            dump_nodeset(fout, set);
            fout << std::endl;
        }
    }
    fout.close();
}

void flat_module_t::dump_nodeset(std::ostream& out, const nodeset_t& s)
{
    for(nodeset_t::const_iterator it = s.begin(); it != s.end(); it++) {
        node_t* n = *it;
        out << "Q:" << n->get_name () << "; inst: " << n->get_instance_name() << " ";
    }
}

void flat_module_t::simpleFIFOAnalysis()
{
    using namespace aggr;

    std::cout << "running simple FIFO analysis." << std::endl;

    input_provider_t* ipp = get_ipp(3);
    BDD mux21BDD = mux21(ipp);
    BDD imux21BDD = !mux21BDD;

    muxblock_list_t muxblocks;
    createMuxBlocks(mux21BDD,muxblocks);
    createMuxBlocks(imux21BDD,muxblocks);

    int edges = 0;
    for(unsigned i=0; i != muxblocks.size(); i++) {
        muxblock_t& a = muxblocks[i];
        for(unsigned j=0; j != muxblocks.size(); j++) {
            if(i == j) continue;
            muxblock_t& b = muxblocks[j];
            if(a.edge_exists_to(b)) {
                a.add_edge_out(&b);
                b.add_edge_in(&a);
                edges += 1;
            }
        }
    }

    muxblock_len_result_t len_res;
    muxblock_sel_result_t sel_res;
    for(unsigned i=0; i != muxblocks.size(); i++) {
        if(muxblocks[i].edges_in.size() == 0) {
            std::vector<muxblock_t*> path;
            muxblk_dfs(&muxblocks[i], path, len_res);
        }
        node_t* s = muxblocks[i].s;
        sel_res[s].push_back(&(muxblocks[i]));
    }

    std::ofstream fout(options.simpleFIFOAnalysisOut.c_str());
    for(muxblock_len_result_t::iterator it = len_res.begin(); it != len_res.end(); it++) {
        fout << "len: " << it->first << "; size: " << it->second.size() << std::endl;
        for(unsigned i=0; i != it->second.size(); i++) {
            std::vector<muxblock_t*>& mux = it->second[i];
            fout << mux << std::endl;
        }
    }
    std::map<int, int> cnts;
    for(muxblock_sel_result_t::iterator it = sel_res.begin(); it != sel_res.end(); it++) {
        fout << "common_sel: " << it->first->get_name() << " ";
        fout << "width: " << it->second.size() << "; " << it->second << std::endl;
        cnts[it->second.size()] += 1;
    }

    fout << "histogram of mux widths" << std::endl;
    for(std::map<int, int>::iterator it = cnts.begin(); it != cnts.end(); it++) {
        fout << "width: " << it->first << "; count: " << it->second << std::endl;
    }


    std::cout << "total number of mux+dff blocks: " << muxblocks.size() << std::endl;
    std::cout << "finished simple FIFO analysis." << std::endl;
}

std::ostream& operator<<(std::ostream& out, const flat_module_t::muxblock_t& m)
{
    out << "[sel:" << m.s->get_name() << " "
        << "a:" << m.a->get_name() << " "
        << "b:" << m.b->get_name() << " "
        << "mux_out:" << m.y->get_name() << " "
        << "mux_out_inst:" << m.y->get_instance_name() << " "
        << "dff_out:" << m.q->get_name() << " "
        << "dff_out_inst: " << m.q->get_instance_name() << "]";

    return out;
}

std::ostream& operator<<(std::ostream& out, const std::vector<flat_module_t::muxblock_t*>& list)
{
    for(unsigned i=0; i != list.size(); i++) {
        out << *list[i];
        if((i+1) != list.size()) {
            out << ", ";
        }
    }
    return out;
}

void flat_module_t::muxblk_dfs(muxblock_t* root, std::vector<muxblock_t*>& path, muxblock_len_result_t& res)
{
    path.push_back(root);
    if(root->edges_out.size() > 0) {
        for(unsigned i=0; i != root->edges_out.size(); i++) {
            muxblock_t* next = root->edges_out[i];
            muxblk_dfs(next, path, res);
        }
    } else {
        res[(int)path.size()].push_back(path);
    }
    path.pop_back();
}

void flat_module_t::createMuxBlocks(BDD& muxBDD, muxblock_list_t& l)
{
    using namespace aggr;

    fnInfo_t* muxInfo = getFunction(muxBDD);
    std::vector<uint8_t>& p = muxInfo->permutation;
    int sel_index = p[2];
    int a_index = p[0], b_index = p[1];
    std::set<kcover_t*>& covers = muxInfo->canonicalPtr->covers;

    for(std::set<kcover_t*>::iterator it = covers.begin(); it != covers.end(); it++) {
        kcover_t* kc = *it;
        node_t* y = kc->get_root();
        for(node_t::fanout_iterator fit = y->fanouts_begin(); fit != y->fanouts_end(); fit++) {
            node_t* fanout = *fit;
            if(fanout->is_latch_gate()) {
                assert(fanout->num_fanouts() == 1);
                node_t* latch = *(fanout->fanouts_begin());
                if(kc->is_leaf(latch)) {
                    bitslice_t bt(kc);
                    node_t* a = bt.xs[a_index];
                    node_t* b = bt.xs[b_index];
                    node_t* s = bt.xs[sel_index];
                    muxblock_t mux(a, b, s, y, latch);
                    l.push_back(mux);
                }
            }
        }
    }
}

void flat_module_t::markFrameBufferOutputs(markings_t& marks)
{
    nodelist_t rfs;
    for(unsigned i=0; i != macros.size(); i++) {
        node_t* n = macros[i];
        if(strcmp("RF2TCSG0064X032D1", n->get_lib_elem()->get_name()) == 0) {
            rfs.push_back(n);
        }
    }
    for(unsigned i=0; i != rfs.size(); i++) {
        node_t* n = rfs[i];
        for(unsigned j = 0; j != 32; j++) {
            char buf[6]; sprintf(buf, "Q%i", j);
            node_t* out = n->get_output_by_name(buf);
            assert(out != NULL);
            if(out->num_fanouts() == 1) {
                marks[out->get_index()] = true;
            }
        }
    }
}

void flat_module_t::analyzeFrameBuffer()
{
    nodelist_t rfs;
    for(unsigned i=0; i != macros.size(); i++) {
        node_t* n = macros[i];
        if(strcmp("RF2TCSG0064X032D1", n->get_lib_elem()->get_name()) == 0) {
            rfs.push_back(n);
        }
    }
    if(rfs.size() == 0) return;

    std::cout << "found " << rfs.size() << " regfiles." << std::endl;
    int cnt = 0;
    std::map<std::string, int> user_gates;

    nodeset_t selectors;
    markings_t fbouts(max_index(), false);

    for(unsigned i=0; i != rfs.size(); i++) {
        node_t* n = rfs[i];
        for(unsigned j = 0; j != 32; j++) {
            char buf[6]; sprintf(buf, "Q%i", j);
            node_t* out = n->get_output_by_name(buf);
            if(out == NULL) {
                std::cout << "Error for node: " << n->get_name() << "; output: " << out << std::endl;
            }
            assert(out != NULL);
            if(out->num_fanouts() != 1) {
                cnt += 1;
                //                std::cout << "rf bad node: " << out->get_name() << std::endl;
            } else {
                fbouts[out->get_index()] = true;

                node_t* user = *(out->fanouts_begin());
                const char* name = user->get_lib_elem()->get_name();
                if((strstr(name, "AOI22") == name) || (strstr(name, "AO22") == name)) {
                    selectors.insert(user);
                }
                std::string name_str(name);
                user_gates[name_str] += 1;
            }
        }
    }
    for(std::map<std::string, int>::iterator it = user_gates.begin(); it != user_gates.end(); it++) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
    std::cout << "# of nodes with >1 fanout: " << cnt << std::endl;
    std::cout << "# of selected nodes      : " << selectors.size() << std::endl;
    analyzeFrameBufferOutputs(selectors, fbouts);
}

void flat_module_t::analyzeFrameBufferOutputs(const nodeset_t& nodes, markings_t& fbouts)
{
    nodelist_t controlSignals;

    for(nodeset_t::const_iterator it = nodes.begin(); it != nodes.end(); it++) {
        node_t* n = *it;
        for(node_t::input_iterator jt = n->inputs_begin(); jt != n->inputs_end(); jt++) {
            node_t* inp = *jt;
            if(fbouts[inp->get_index()]) {
                continue;
            } else {
                if(inp->is_gate() && !inp->is_latch_gate()) {
                    controlSignals.push_back(inp);
                }
            }
        }
    }
    std::cout << "Found " << controlSignals.size () << " control signals." << std::endl;
    analyzeCommonInputs(controlSignals, false);

    nodelist_t decoder_outputs;
    for(unsigned i=0; i != modules.size(); i++) {
        using namespace aggr;
        id_module_t* m = modules[i];
        if(strcmp(m->get_type(), "decoder") == 0) {
            if(m->num_outputs() >= 768) {
                const id_module_t::nodelist_t& outs = *(m->get_outputs());
                decoder_outputs.resize(outs.size());
                std::copy(outs.begin(), outs.end(), decoder_outputs.begin());
                break;
            }
        }
    }
    std::cout << "# of decoder outputs: " << decoder_outputs.size() << std::endl;
    analyzeVGADecoder(fbouts, decoder_outputs);
}

void flat_module_t::analyzeVGADecoder(const markings_t& fbouts, const nodelist_t& decoder_outputs)
{
    markings_t marks(max_index(), false);
    int cnt=0;
    nodeset_t dec_out_set;
    for(unsigned i=0; i != decoder_outputs.size(); i++) {
        node_t* n = decoder_outputs[i];
        dec_out_set.insert(n);

        for(node_t::fanout_iterator it = n->fanouts_begin(); it != n->fanouts_end(); it++) {
            node_t* f = *it;
            if(f->is_gate() && !f->is_latch_gate()) {
                marks[f->get_index()] = true;
                cnt += 1;
            }
        }
    }
    std::cout << "marked " << cnt << " gates initially." << std::endl;
    cnt = 0;
    for(unsigned i=0; i != gates.size(); i++) {
        node_t* g = gates[i];
        if(g->num_inputs() == 0) continue;
        if(g->is_latch_gate()) continue;

        bool good = false;
        for(node_t::input_iterator it = g->inputs_begin(); it != g->inputs_end(); it++) {
            int gi = (*it)->get_index();
            if(marks[gi]) {
                good = true;
                break;
            }
        }
        if(good) {
            marks[g->get_index()] = true;
            cnt += 1;
        }
    }

    nodeset_t marked_gates;
    for(unsigned i=0; i != gates.size(); i++) {
        node_t* g = gates[i];
        if(marks[g->get_index()]) {
            marked_gates.insert(g);
        }
    }
    std::cout << "# of marked gates is: " << marked_gates.size() << std::endl;
    aggr::id_module_t* mod = create_module("framebuffer_read", marked_gates);

    const aggr::id_module_t::nodelist_t& inputs = *(mod->get_inputs());
    int fbcnt = 0, deccnt = 0, othercnt = 0;

    std::ostringstream otherstream;
    otherstream << "others: ";

    for(unsigned i=0; i != inputs.size(); i++) {
        node_t* n = inputs[i];
        if(fbouts[n->get_index()]) {
            assert(dec_out_set.find(n) == dec_out_set.end());
            fbcnt += 1;
        } else if(dec_out_set.find(n) != dec_out_set.end()) {
            deccnt += 1;
        } else {
            othercnt += 1;
            otherstream << n->get_name() << " ";
        }
    }
    std::ostringstream ostr;
    ostr << "frame buffer outputs : " << fbcnt
        << "; decoder outputs : " << deccnt
        << "; others : " << othercnt;
    std::string str(ostr.str());
    mod->add_comment(str);

    str = otherstream.str();
    mod->add_comment(str);
}

void flat_module_t::aggregate()
{
    using namespace aggr;

    // create_product_relations();
    std::cout << "running aggregation algorithms ... " << std::endl;

    identifyMuxes(this);
    identify21Muxes2(this);
    identify31Muxes(this);
    identify31Muxes2(this);
    identify41Muxes(this);
    identifyMuxes3(this);

    registerAnalysis();
    identifyRCAs(this);
    identify24Decoders(this);
    identify24Demuxes(this);
    identify38Decoders(this);
    identify38Demuxes(this);
    identify416Demuxes(this);
    identify416Decoders(this);
    identify532Decoders(this);
    identify664Decoders(this);
    identifyEqualityComparators(this);
    identifyGatingFuncs2(this);
    prop_n::findXorChains(this);
    prop_n::findAndChains(this);
    prop_n::findOrChains(this);
    if(!options.disableCgenTreeAggregation) {
        prop_n::findCgenTrees(this);
    }
    //findPopCnts(this);

    //TESTING
    //identifyALUXOR2(this);
    //identifyALUXNOR2(this);
    //shortestPath(this);

    std::cout << "finished aggregation algorithms." << std::endl;

    count_word_types(std::cout);
    if(enable_propagation) propagate_words();
    std::cout << "finished aggregation postprocessing." << std::endl;

}

void flat_module_t::registerAnalysis()
{
    using namespace aggr;

    modulelist_t newmods;
    for(unsigned i=0; i != modules.size(); i++) {
        aggr::id_module_t* mi = modules[i];
        if(strstr(mi->get_type(), "mux") == mi->get_type()) {
            aggr::id_module_t* newmod = mi->registerAnalysis(this);
            if(newmod) {
                newmods.push_back(newmod);
            }
        }
    }

    // group modules with the same ungrouped inputs.
    for(unsigned i=0; i != newmods.size(); i++) {
        newmods[i]->uf_init();
    }
    for(unsigned i=0; i != newmods.size(); i++) {
        id_module_t* mi = newmods[i];
        for(unsigned j=i+1; j != newmods.size(); j++) {
            id_module_t* mj = newmods[j];
            if(mi->uf_get_rep() != mj->uf_get_rep()) {
                if(mi->same_ungrouped_inputs(mj)) {
                    mi->uf_link(mj);
                }
            }
        }
    }
    typedef std::map<id_module_t*, modulelist_t> groups_t;
    groups_t groups;
    for(unsigned i=0; i != newmods.size(); i++) {
        groups[newmods[i]->uf_get_rep()].push_back(newmods[i]);
    }
    for(groups_t::iterator it = groups.begin(); it != groups.end(); it++) {
        modulelist_t& l = it->second;
        if(l.size() > 1) {
            id_module_t* m = aggr::merge_modules(l);
            modules.push_back(m);
            
            std::string selgrp("sel");
            ::nodelist_t* sel = m->get_inputs_in_group(selgrp);
            assert(sel != NULL);
            nodeset_t selset;
            std::copy(sel->begin(), sel->end(), std::inserter(selset, selset.begin()));

            modulelist_t mods;
            analyzeDecoders(selset, mods);
            for(unsigned i=0; i != mods.size(); i++) {
                add_module(mods[i]);
            }
        }
    }

    unsigned old_size = modules.size();
    modules.resize(old_size + newmods.size());
    std::copy(newmods.begin(), newmods.end(), modules.begin() + old_size);
    std::cout << "# of reg modules created: " << newmods.size() << std::endl;
}


void flat_module_t::count_word_types(std::ostream& out)
{
    int input_words = 0;
    int output_words = 0;
    for(wordlist_t::iterator it = words.begin(); it != words.end(); it++) {
        word_t* w = *it;
        if(w->is_input_word()) { input_words += 1; }
        if(w->is_output_word()) { output_words += 1; }
    }
    out << "# of input words : " << input_words << std::endl;
    out << "# of output words: " << output_words << std::endl;
    out << "# of words       : " << words.size() << std::endl;
}

void flat_module_t::interactive_propagator()
{
    using namespace std;

    while(true) {
        string line;

        cout << ">> ";
        getline(std::cin, line);

        istringstream iss(line);
        vector<string> tokens;
        copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter<vector<string> >(tokens));

        if(tokens.size() == 0) continue;

        if(tokens[0] == "quit") {
            break;
        } else if(tokens[0] == "fwdprop") {
            ip_fwdprop(tokens);
        } else {
            std::cout << "bad command: " << tokens[0] << std::endl;
        }
    }
}

void flat_module_t::ip_fwdprop(std::vector<std::string>& tokens)
{
    nodelist_t nodes;
    ip_get_nodes(tokens, nodes);
    if(nodes.size() == 0) {
        return;
    }

    word_t* nw = new word_t(false, word_t::FWD_PROPAGATION);
    for(unsigned k=0; k != nodes.size(); k++) {
        node_t* n = nodes[k];
        nw->add_bit(n, -1);
    }

    wordlist_t init_words;
    init_words.push_back(nw);

    int found = 0;
    int round = 1;
    while(true) {
        wordlist_t new_words;
        for(wordlist_t::iterator it = init_words.begin(); it != init_words.end(); it++) {
            word_t* w = *it;
            assert(w->size() > 0);
            propagate_forward(w, new_words);
        }

        found += new_words.size();
        printf("round %d: %d new words found.\n", round++, (int) new_words.size());

        if(new_words.size() == 0) break;
        else {
            init_words.resize(new_words.size());
            std::copy(new_words.begin(), new_words.end(), init_words.begin());
        }
        for(unsigned i=0; i != new_words.size(); i++) {
            std::cout << i << ": ";
            for(unsigned j=0; j != new_words[i]->size(); j++) {
                node_t* n = new_words[i]->get_bit(j);
                std::cout << n->get_name() << "/" << n->get_instance_name() << "/" << (n->get_lib_elem() ? n->get_lib_elem()->get_name() : "") << " ";
            }
            std::cout << std::endl;
        }
    }
}

void flat_module_t::ip_get_nodes(std::vector<std::string>& tokens, nodelist_t& nodes)
{
    if(tokens.size() == 4 && is_number(tokens[2]) && is_number(tokens[3])) {
        int n1 = atoi(tokens[2].c_str());
        int n2 = atoi(tokens[3].c_str());
        for(int i=n1; i <= n2; i++) {
            char buf[256];
            sprintf(buf, "%s[%d]", tokens[1].c_str(), i);
            node_t* n = get_node_by_name(buf);
            if(n == NULL) {
                std::cout << "error: can't find node: " << buf << std::endl;
                nodes.clear();
                return;
            }
            nodes.push_back(n);
        }
    } else {
        for(unsigned i=1; i < tokens.size(); i++) {
            node_t* n = get_node_by_name(tokens[i].c_str());
            if(n == NULL) {
                std::cout << "error: can't find node: " << tokens[i] << std::endl;
                nodes.clear();
                return;
            }
            nodes.push_back(n);
        }
    }
}

void flat_module_t::propagate_words()
{
    wordlist_t init_words(words);

    int round = 1;
    int found = 0;
    while(true) {
        wordlist_t new_words;
        for(wordlist_t::iterator it = init_words.begin(); it != init_words.end(); it++) {
            word_t* w = *it;
            assert(w->size() > 0);
            if(can_propagate_back(w)) {
                propagate_back(w, new_words);
            }
            propagate_forward(w, new_words);
        }
        round++;
        found += new_words.size();
        if(new_words.size() == 0) break;
        else {
            init_words.resize(new_words.size());
            std::copy(new_words.begin(), new_words.end(), init_words.begin());
        }
    }
    printf("%d rounds of propagation found %d new words.\n", round, found);
}

void flat_module_t::propagate_forward(const word_t* w, wordlist_t& new_words)
{
    assert(w->size() > 0);

    std::set<lib_elem_t*> fanout_types;
    createFanoutTypeIntersection(w, fanout_types);
    for(std::set<lib_elem_t*>::iterator it =  fanout_types.begin();
            it != fanout_types.end();
            it++)
    {
        lib_elem_t* l = *it;
        nodeset_t set;
        nodelist_t list;
        // ignore macros.
        if(l == NULL) return;

        for(word_t::const_iterator it = w->begin(); it != w->end(); it++) {
            node_t* n = *it;
            for(node_t::fanout_iterator jt = n->fanouts_begin(); jt != n->fanouts_end(); jt++) {
                node_t* f = *jt;
                if(f->get_lib_elem() == l) {
                    set.insert(f);
                    list.push_back(f);
                    break; // out of the fanout_iterator loop.
                }
            }
        }

        if(set.size() == list.size() && set.size() == w->size()) {
            create_fwdprop_words(w, list, new_words);
        }
    }
}

void flat_module_t::create_fwdprop_words(
        const word_t* w,
        nodelist_t& list,
        wordlist_t& new_words)
{
    assert(list.size() > 0);

    // let's first deal with the inputs.
    unsigned inpsz = list[0]->num_inputs();
    lib_elem_t* libelem = list[0]->get_lib_elem();

    for(unsigned i = 0; i != inpsz; i++) {
        nodeset_t set;
        nodelist_t list;
        if(libelem->is_symmetric_input(i)) continue;

        for(nodelist_t::iterator it = list.begin(); it != list.end(); it++) {
            node_t* n = *it;
            assert(n->get_lib_elem() == libelem);
            assert(n->num_inputs() == inpsz);
            node_t* inp = n->get_input(i);
            set.insert(inp);
            list.push_back(inp);
        }
        if(set.size() == list.size() && set.size() == w->size()) {
            word_t* nw = new word_t(w->is_ordered(), word_t::FWD_PROPAGATION);
            for(unsigned k=0; k != list.size(); k++) {
                node_t* n = list[k];
                int pos = w->is_ordered() ? k : -1;
                nw->add_bit(n, pos);
            }
            if(!similar_word_exists(word_htbl, nw)) {
                new_words.push_back(nw);
                add_word(nw);
            }
        }
    }

    // now create the "output" word.
    word_t* nw = new word_t(w->is_ordered(), word_t::FWD_PROPAGATION);
    for(unsigned k=0; k != list.size(); k++) {
        node_t* n = list[k];
        int pos = w->is_ordered() ? k : -1;
        nw->add_bit(n, pos);
    }
    if(!similar_word_exists(word_htbl, nw)) {
        new_words.push_back(nw);
        add_word(nw);
    }
}

void flat_module_t::createFanoutTypeIntersection(
        const word_t* w,
        std::set<lib_elem_t*>& fanoutTypeIntersection
        )
{
    unsigned pos = 0;
    // this loop creates a set which has the following properties:
    // 1. each element is of type lib_elem_t
    // 2. each lib_elem_t appears only once in the fanout of each bit.
    // 3. each lib_elem_t apperas in the fanouts of every bit of the word.
    for(word_t::const_iterator it = w->begin(); it != w->end(); it++, pos++) {
        node_t* b = w->get_bit(0);
        std::vector<lib_elem_t*> fanoutTypes;
        std::set<lib_elem_t*> validFanoutSet;

        b->get_fanout_types(fanoutTypes);
        getValidFanoutSet(fanoutTypes, validFanoutSet);

        if(pos == 0) {
            for(std::set<lib_elem_t*>::iterator it =  validFanoutSet.begin();
                    it != validFanoutSet.end();
                    it++)
            {
                lib_elem_t* l = *it;
                fanoutTypeIntersection.insert(l);
            }
        } else {
            std::set<lib_elem_t*> toDel;
            for(std::set<lib_elem_t*>::iterator it =  validFanoutSet.begin();
                    it != validFanoutSet.end();
                    it++)
            {
                lib_elem_t* l = *it;
                if(fanoutTypeIntersection.find(l) == fanoutTypeIntersection.end()) {
                    toDel.insert(l);
                }
            }
            for(std::set<lib_elem_t*>::iterator it =  fanoutTypeIntersection.begin();
                    it != fanoutTypeIntersection.end();
                    it++)
            {
                lib_elem_t* l = *it;
                if(validFanoutSet.find(l) == validFanoutSet.end()) {
                    toDel.insert(l);
                }
            }
            for(std::set<lib_elem_t*>::iterator it =  toDel.begin(); it != toDel.end(); it++)
            {
                lib_elem_t* l = *it;
                fanoutTypeIntersection.erase(l);
            }
        }
    }
}
void flat_module_t::getValidFanoutSet(std::vector<lib_elem_t*>& fanoutTypes,
        std::set<lib_elem_t*>& validFanoutSet)
{
    std::map<lib_elem_t*, int> counts;
    for(unsigned i=0; i != fanoutTypes.size(); i++) {
        lib_elem_t* l = fanoutTypes[i];
        counts[l] += 1;
    }
    for(std::map<lib_elem_t*, int>::iterator it =  counts.begin();
            it != counts.end();
            it++)
    {
        if(it->second == 1) {
            assert(validFanoutSet.find(it->first) == validFanoutSet.end());
            validFanoutSet.insert(it->first);
        }
    }
}

bool compare_words(const word_t* a, const word_t* b)
{
    for(unsigned i=0; (i < a->size() && i < b->size()); i++) {
        node_t* ai = a->get_bit(i);
        node_t* bi = b->get_bit(i);
        if(ai->get_name() < bi->get_name()) return true;
        else if(ai->get_name() > bi->get_name()) return false;
    }

    if(a->size() < b->size()) return true;
    else return false;
}

void flat_module_t::output_words(std::ostream& out)
{
    wordlist_t new_words;
    word_hashtable_t htbl;
    for(unsigned i=0; i != words.size(); i++) {
        word_t* w = words[i];
        if(options.dumpOnlyAdderWords) {
            if(w->getSrc() != word_t::RCA_SUM) continue;
        }

        if(!w->rf_word() && similar_word_exists(htbl, w)) {
            continue;
        } else {
            htbl[w->get_sign()].push_back(w);
            new_words.push_back(w);
        }
    }
    std::sort(new_words.begin(), new_words.end(), compare_words);
    std::cout << "# of compressed words: " << new_words.size() << std::endl;

    using boost::property_tree::ptree;
    using boost::property_tree::json_parser::write_json;
    
    ptree wl;
    int num_words = 0;
    for(unsigned i=0; i != new_words.size(); i++) {
        if(new_words[i]->size() >= 4 || new_words[i]->rf_word()) {
            word_t* w = new_words[i];

            num_words += 1;
            ptree word = w->get_ptree();

            wl.push_back(std::make_pair("", word));
        }
    }
    ptree words;
    words.add_child("words", wl);
    write_json(out,  words);
}


bool flat_module_t::can_propagate_back(const word_t* w) const
{
    std::set<node_t*> nodes;
    lib_elem_t* type = NULL;
    for(word_t::const_iterator it = w->begin(); it != w->end(); it++) {
        node_t* n= *it;
        if(n->is_macro_out() || n->is_macro()) {
            return false;
        }
        if(type == NULL) {
            type = n->get_lib_elem();
        } else {
            if(type != n->get_lib_elem()) return false;
        }
        nodes.insert(n);
    }
    if(nodes.size() != w->size()) return false;
    else return true;
}

void flat_module_t::propagate_back(const word_t* w, wordlist_t& new_words)
{
    typedef std::vector<nodeset_t> sets_t;
    typedef std::vector<nodelist_t> lists_t;

    assert(w->size() > 0);

    unsigned sz = w->get_bit(0)->num_inputs();
    lib_elem_t* libelem = w->get_bit(0)->get_lib_elem();

    sets_t sets(sz);
    lists_t lists(sz);

    for(word_t::const_iterator it = w->begin(); it != w->end(); it++) {
        node_t* node = *it;
        assert(node->num_inputs() == sz);
        assert(node->get_lib_elem() == libelem);
        int pos = 0;
        for(node_t::input_iterator inp_it = node->inputs_begin(); inp_it != node->inputs_end(); inp_it++) {
            node_t *inp_node = *inp_it;
            sets[pos].insert(inp_node);
            lists[pos].push_back(inp_node);
            pos += 1;
        }
    }

    for(unsigned i=0; i != sz; i++) {
        if(sets[i].size() == lists[i].size() && !libelem->is_symmetric_input(i)) {
            assert(lists[i].size() == w->size());
            word_t* nw = new word_t(w->is_ordered(), word_t::BACK_PROPAGATION);
            for(unsigned k=0; k != lists[i].size(); k++) {
                node_t* n = lists[i][k];
                int pos = w->is_ordered() ? k : -1;
                nw->add_bit(n, pos);
            }
            if(!similar_word_exists(word_htbl, nw)) {
                new_words.push_back(nw);
                add_word(nw);
            }
        }
    }
}

namespace {
    struct input_fullfn_wrapper_t : public input_provider_t
    {
        flat_module_t* module;
        node_t* node;
        std::map<node_t*, BDD>& varMap;
        bool memoize;
        int inputs;

        input_fullfn_wrapper_t(flat_module_t* module, node_t* node, std::map<node_t*, BDD>& var_map, bool memoize, int inputs)
            : module(module),
            node(node),
            varMap(var_map),
            memoize(memoize),
            inputs(inputs)
        {
        }

        virtual BDD inp(int index);
        virtual BDD one();
        virtual BDD zero();
    };

    BDD input_fullfn_wrapper_t::inp(int index)
    {
        node_t* input = node->get_input(index);
        return module->createFullFn(input, varMap, memoize, inputs);
    }

    BDD input_fullfn_wrapper_t::one()
    {
        return module->getMgr(inputs).bddOne();
    }
    BDD input_fullfn_wrapper_t::zero()
    {
        return module->getMgr(inputs).bddZero();
    }
}

BDD flat_module_t::createFullFn(node_t* node, std::map<node_t*, BDD>& varMap, bool memoize, int inputs)
{
    if(node->is_input() || node->is_latch() || node->is_macro() || node->is_macro_out()) {
        std::map<node_t*, BDD>::iterator it = varMap.find(node);
        if(it == varMap.end()) {
            int next = varMap.size();
            BDD var = getMgr(inputs).bddVar(next);
            varMap[node] = var;
            return var;
        } else {
            BDD var = it->second;
            return var;
        }
    } else {
        const bool enable_memos = false;
        if(enable_memos && memoize) {
            std::map<node_t*, BDD>::iterator it = varMap.find(node);
            if(it != varMap.end()) {
                return it->second;
            } else {
                input_fullfn_wrapper_t wrapper(this, node, varMap, memoize, inputs);
                BDD b = node->get_lib_elem()->getFn(&wrapper);
                varMap[node] = b;
                return b;
            }
        } else {
            if(varMap.find(node) != varMap.end()) {
                return varMap[node];
            }
            input_fullfn_wrapper_t wrapper(this, node, varMap, memoize, inputs);
            BDD b = node->get_lib_elem()->getFn(&wrapper);
            return b;
        }
    }
}

void flat_module_t::bddSweep()
{
    bdd_map_t vars;
    for(unsigned i=0; i != gates.size(); i++) {
        createFullFn(gates[i], vars, true, -1);
        printf("%6d/%8d\r", (int) i+1, (int) gates.size());
    }
    vars.clear();
    printf("\n");
}

void flat_module_t::readDFSInputs(const char* filename, nodeset_t& inputs, std::vector<node_t*>& outputs, std::string& moduleName)
{
    using namespace std;

    ifstream fin(filename);
    string line;
    moduleName = "userDefined";

    while(fin) {
        getline(fin, line);
        istringstream iss(line);

        vector<string> tokens;
        copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter<vector<string> >(tokens));

        // ignore empty lines.
        if(tokens.size() == 0) continue;
        // is this a line a list of inputs?
        if(tokens[0] == ".inputs")  {
            for(unsigned i=1; i != tokens.size(); i++) {
                node_t* n = get_node_by_name(tokens[i].c_str());
                inputs.insert(n);
            }
        } else if(tokens[0] == ".outputs") {
            for(unsigned i=1; i != tokens.size(); i++) {
                node_t* n = get_node_by_name(tokens[i].c_str());
                outputs.push_back(n);
            }
        } else if(tokens[0] == ".name") {
            if(tokens.size() != 2) {
                fprintf(stderr, "Only one name allowed.\n");
                exit(0);
            }
            moduleName = tokens[1];
        } else {
            fprintf(stderr, "ERROR: Unknown command: %s.\n", tokens[0].c_str());
            exit(0);
        }
    }
    fin.close();
}

bool compare_nodes(node_t* a, node_t* b)
{
    if(a->get_level() < b->get_level()) return true;
    if(a->get_level() > b->get_level()) return false;

    return strcmp(a->get_lib_elem()->get_name(), b->get_lib_elem()->get_name());
}

void flat_module_t::addUserDefinedModule(const char* filename)
{
    std::ifstream fin(filename);

    nodeset_t edges;
    nodeset_t visited;
    std::vector<node_t*> outputs;
    std::string moduleName;

    readDFSInputs(filename, edges, outputs, moduleName);
    std::cout << "User defined module read from: " << filename << std::endl;
    std::cout << "read " << edges.size() << " input(s)." << std::endl;
    std::cout << "read " << outputs.size() << " output(s)." << std::endl;
    std::cout << "module name: " << moduleName << std::endl;

    for(unsigned i=0; i != outputs.size(); i++) {
        node_t* start = outputs[i];
        // do the DFS.
        do_dfs(start, edges, visited);
    }
    using namespace aggr;
    id_module_t* mod = new id_module_t(moduleName.c_str(), id_module_t::UNSLICEABLE, id_module_t::USER_DEFINED);
    // add outputs.
    for(unsigned i=0; i != outputs.size(); i++) {
        node_t* n = outputs[i];
        mod->add_output(n);
    }

    // add inputs.
    for(nodeset_t::iterator it = edges.begin(); it != edges.end(); it++) {
        node_t* n = *it;
        if(visited.find(n) != visited.end()) {
            mod->add_input(n);
        }
    }
    mod->compute_internals();

    // FIXME: remove this ASAP
    // sneak in the debugging code here.
    {
        /*
           const nodeset_t& s = mod->get_internals();
           ::nodelist_t l;
           l.reserve(s.size());
           for(nodeset_t::const_iterator it = s.begin(); it != s.end(); it++) {
           l.push_back(*it);
           }
           std::sort(l.begin(), l.end(), compare_nodes);
           int level = -1;
           for(unsigned i=0; i != l.size(); i++) {
           if(l[i]->get_level() != level) {
           level = l[i]->get_level();
           std::cout << std::endl << "level: " << level << std::endl;
           }
           l[i]->dump(std::cout);
           }
           */
    }

    this->add_module(mod);
}

void flat_module_t::printPath(node_t* start, node_t* end)
{
    std::map<node_t*, bool> visited;
    if(!printPathImpl(start, end, visited)) {
        std::cout << "no path between " << start->get_name() << " and " << end->get_name() << std::endl;
    } else {
        start->dump(std::cout);
    }
}

bool flat_module_t::printPathImpl(node_t* start, node_t* end, std::map<node_t*, bool>& visited)
{
    if(start == end) {
        start->dump(std::cout);
        return true;
    }
    if(start->is_latch()) return false;

    if(visited.find(start) != visited.end()) return visited[start];
    for(node_t::input_iterator it = start->inputs_begin(); it != start->inputs_end(); it++) {
        node_t* n = *it;
        if(printPathImpl(n, end, visited)) {
            visited[start] = true;
        }
    }
    if(visited.find(start) == visited.end()) {
        visited[start] = false;
        return false;
    } else {
        start->dump(std::cout);
        return true;
    }
}

BDD flat_module_t::buildFns(node_t* start, nodeset_t& edges, std::map<node_t*, BDD>& fns)
{
    Cudd& mgr = getFullFnMgr();
    if(fns.find(start) != fns.end()) {
        return fns[start];
    } else if(edges.find(start) != edges.end()) {
        int var = fns.size();
        BDD bdd = mgr.bddVar(var);
        fns[start] = bdd;
        return bdd;
    } else if(start->is_input() || start->is_latch() || start->is_macro() || start->is_macro_out()) {
        fprintf(stderr, "Hit a primary input or latch at node: %s\n", start->get_name().c_str());
        exit(1);
        return mgr.bddZero();
    } else {
        for(node_t::input_iterator it = start->inputs_begin(); it != start->inputs_end(); it++) {
            node_t* n = *it;
            BDD bdd = buildFns(n, edges, fns);
            fns[n] = bdd;
        }
        input_fullfn_wrapper_t ipp(this, start, fns, false, -1);
        BDD bdd = start->get_lib_elem()->getFn(&ipp);
        fns[start] = bdd;
        return bdd;
    }
}

void flat_module_t::do_dfs(node_t* start, nodeset_t& edges, nodeset_t& visited)
{
    bool vfind = (visited.find(start) != visited.end());
    bool efind = (edges.find(start) != edges.end());
    if(vfind || efind) {
        if(efind && !vfind) visited.insert(start);
        return;
    } else {
        visited.insert(start);
        if(start->is_latch() || start->is_macro() || start->is_macro_out()) return;
        for(node_t::input_iterator it = start->inputs_begin(); it != start->inputs_end(); it++) {
            node_t* i = *it;
            do_dfs(i, edges, visited);
        }
    }
}

void flat_module_t::printFullFunctions(const char* filename)
{
    std::ifstream fin(filename);
    if(fin) {
        std::map<node_t*, BDD> varMap;
        BDD bdd;
        while(fin) {
            char input[256];

            fin >> input;

            if(strcmp(input, ".clear")==0) {
                varMap.clear();
                continue;
            } else if(strcmp(input, ".poscofactor") == 0) {
                fin >> input;
                node_t* c = get_node_by_name(input);
                if(c == NULL || varMap.find(c) == varMap.end()) {
                    fprintf(stderr, "ERROR: Can't find %s in variable map.\n", input);
                    return;
                }
                BDD b = varMap[c];
                BDD cf = bdd.Cofactor(b);
                std::cout << "+ve cofactor [" << input << "] = "; printBDD(stdout, cf);
                continue;
            } else if(strcmp(input, ".negcofactor") == 0) {
                fin >> input;
                node_t* c = get_node_by_name(input);
                if(c == NULL || varMap.find(c) == varMap.end()) {
                    fprintf(stderr, "ERROR: Can't find %s in variable map.\n", input);
                    return;
                }
                BDD b = !varMap[c];
                BDD cf = bdd.Cofactor(b);
                std::cout << "-ve cofactor [" << input << "] = "; printBDD(stdout, cf);
                continue;
            } else if(strcmp(input, ".poscofactors") == 0) {
                int num;
                fin >> num;
                BDD b = getFullFnMgr().bddOne();
                for(int i = 0; i != num; i++) {
                    fin >> input;
                    node_t* c = get_node_by_name(input);
                    if(c == NULL || varMap.find(c) == varMap.end()) {
                        fprintf(stderr, "ERROR: Can't find %s in variable map.\n", input);
                        return;
                    }
                    BDD x = varMap[c];
                    b = b & x;
                }
                BDD cf = bdd.Cofactor(b);
                std::cout << "+ve cofactors = "; printBDD(stdout, cf);
                continue;
            } else if(strcmp(input, ".negcofactors") == 0) {
                int num;
                fin >> num;
                BDD b = getFullFnMgr().bddOne();
                for(int i = 0; i != num; i++) {
                    fin >> input;
                    node_t* c = get_node_by_name(input);
                    if(c == NULL || varMap.find(c) == varMap.end()) {
                        fprintf(stderr, "ERROR: Can't find %s in variable map.\n", input);
                        return;
                    }
                    BDD x = !varMap[c];
                    b = b & x;
                }
                BDD cf = bdd.Cofactor(b);
                std::cout << "-ve cofactors = "; printBDD(stdout, cf);
                continue;
            } else if(strcmp(input, ".cofactors") == 0) {
                int num;
                fin >> num;
                BDD b = getFullFnMgr().bddOne();
                std::cout << "cofactors ";
                for(int i = 0; i != num; i++) {
                    fin >> input;
                    node_t* c = get_node_by_name(input+1);
                    if(c == NULL || varMap.find(c) == varMap.end()) {
                        fprintf(stderr, "ERROR: Can't find %s in variable map.\n", input+1);
                        return;
                    }

                    BDD x;
                    if(input[0] == '+') {
                        x = varMap[c];
                    } else if(input[0] == '-') {
                        x = !varMap[c];
                    } else {
                        fprintf(stderr, "Each variable must start with either '+' or '-' for"
                                "the positive and negative cofactor respectively.\n");
                        return;
                    }
                    std::cout << input[0];
                    b = b & x;
                }
                BDD cf = bdd.Cofactor(b);
                std::cout << " : "; printBDD(stdout, cf);
                continue;
            } else if(strcmp(input, ".allcofactors") == 0) {
                printAllCofactors(bdd, fin, varMap);
                continue;
            }

            node_t* n = get_node_by_name(input);
            if(n) {
                std::cout << std::endl;
                bdd = createFullFn(n, varMap, false, -1);
                std::cout << input << ": "; printBDD(stdout, bdd);
                printVarMap(varMap);
            } else {
                std::cerr << "Error: Unable to find node \"" << input << "\"." << std::endl;
                return;
            }
        }
    }
}

void flat_module_t::printAllCofactors(BDD& bdd, std::istream& in, std::map<node_t*, BDD>& varMap)
{
    int num;
    in >> num;
    std::vector<const char*> inputs;
    for(int i = 0; i != num; i++) {
        char input[256];
        in >> input;
        char* name = strdup(input);
        inputs.push_back(name);
    }
    int max = 1<<num;
    for(int i=0; i != max; i++) {
        BDD b = getFullFnMgr().bddOne();
        for(int j=0; j != num; j++) {
            node_t* n = get_node_by_name(inputs[j]);
            if(n == NULL) {
                fprintf(stderr, "Unable to find node in ckt: %s\n", inputs[j]);
                return;
            }
            if(varMap.find(n)==varMap.end()) {
                fprintf(stderr, "Unable to find node in map: %s\n", inputs[j]);
                return;
            }
            BDD x = varMap[n];
            int mask = 1 << j;
            if(i & mask) {
                b = b & x;
            } else {
                b = b & !x;
            }
        }
        BDD cf = bdd.Cofactor(b);
        std::cout << std::setw(5) << std::hex << i << " : "; printBDD(stdout, cf);
    }
}

void flat_module_t::printVarMap(std::map<node_t*, BDD> varMap)
{
    for(std::map<node_t*, BDD>::iterator it = varMap.begin();
            it != varMap.end();
            it++)
    {
        node_t* n = it->first;
        BDD& bdd = it->second;

        std::cout << std::setw(20) << n->get_name() << " : "; printBDD(stdout, bdd);
    }
}

node_t* flat_module_t::get_node_by_name(const char* name) const
{
    map_t::const_iterator it = map.find(name);
    if(it == map.end()) return NULL;
    else {
        return it->second;
    }
}

int flat_module_t::count_covered_gates()
{
    int cnt=0;
    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t* n = it->second;
        if(n->is_latch() || n->is_input() || n->is_latch_gate() || n->is_macro() || n->is_macro_out()) continue;
        if(n->count_modules() > 0) cnt += 1;
    }
    return cnt;
}

int flat_module_t::count_conflicting_gates()
{
    int cnt = 0;
    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t* n = it->second;
        if(n->is_latch() || n->is_input() || n->is_latch_gate() || n->is_macro() || n->is_macro_out()) continue;
        if(n->count_undominated_modules() > 1) cnt += 1;
    }
    return cnt;
}

int flat_module_t::get_module_count() const
{
    return modules.size();
}

int flat_module_t::get_undominated_module_count() const
{
    using namespace aggr;

    int cnt = 0;
    for(modulelist_t::const_iterator it = modules.begin(); it != modules.end(); it++) {
        id_module_t* mod = *it;
        if(!mod->is_dominated()) {
            cnt += 1;
        }
    }
    return cnt;
}

int flat_module_t::get_non_overlapping_module_count()
{
    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t* n = it->second;
        n->mark_overlapping_modules();
    }
    int cnt = 0;
    for(modulelist_t::iterator it = modules.begin(); it != modules.end(); it++) {
        aggr::id_module_t* m = *it;
        if(!m->is_overlapping2()) cnt += 1;
    }
    return cnt;
}

int flat_module_t::get_non_overlapping_undominated_module_count()
{
    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t* n = it->second;
        n->mark_overlapping_modules();
    }
    int cnt = 0;
    for(modulelist_t::iterator it = modules.begin(); it != modules.end(); it++) {
        aggr::id_module_t* m = *it;
        if(!m->is_dominated() && !m->is_overlapping()) cnt += 1;
    }
    return cnt;
}

void flat_module_t::create_matching_modules()
{
    using namespace aggr;
    for(modulelist_t::const_iterator it = modules.begin(); it != modules.end(); it++) {
        id_module_t* mod = *it;
        mod->create_new_modules();
    }
}

void flat_module_t::merge_modules(void)
{
    //printf("# of covered gates before merging:     %5d\n", count_covered_gates());
    //printf("# of conflicting gates before merging: %5d\n", count_conflicting_gates());
#if defined(MODULE_CONSTRAINTS) || defined(USE_GLPK)
    int pos = 1, tot = map.size();
    for(map_t::iterator it=map.begin(); it != map.end(); it++) {
        node_t* n = it->second;
        n->merge_modules();
        if(pos++ % 100 == 0) {
            printf("PROGRESS: %8d/%8d\r", pos, tot);
            fflush(stdout);
        }
    }
    printf("\n");
#endif

    cleanup_modules();
    if(options.eliminateOverlaps) {
#if defined(MODULE_CONSTRAINTS) || defined(USE_GLPK)
        bool compute_conflicts = true;
#else
        bool compute_conflicts = false;
#endif
        mark_conflicting_modules(compute_conflicts);
#ifdef USE_GLPK
        eliminate_overlaps_glpk();
#else
        if(options.sliceableILP) {
            eliminate_overlaps_sliceable();
        } else {
            eliminate_overlaps();
        }
#endif
        cleanup_modules();
        mark_conflicting_modules(true);
    }
    if(options.computeRepresentativeModules) {
        // removed the call to compute_repr and replaced
        // with this code that computes the pin names.
        for(unsigned i=0; i != modules.size(); i++) {
            modules[i]->computePinNames();
        }
    }

    aggr::cleanupAfterPinNameComputation();

    //printf("# of conflicting gates after merging:  %5d\n", count_conflicting_gates());
    //printf("# of covered gates afer merging:       %5d\n", count_covered_gates());
}

void flat_module_t::compute_sliceability()
{
    // FIXME: deal with the real work of handling the sliceable objects here.
    moduleset_t sliceables;
    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t* n = it->second;
        const moduleset_t& modules = *(n->get_modules());
        if(modules.size() <= 1) continue;
        else {
            for(moduleset_t::const_iterator it = modules.begin(); it != modules.end(); it++) {
                aggr::id_module_t* m = *it;
                if(m->getNumSlices() > 1) {
                    sliceables.insert(m);
                }
            }
        }
    }
    std::cout << "# of sliceable modules: " << sliceables.size() << std::endl;
    for(unsigned i=0; i != modules.size(); i++) {
        aggr::id_module_t* mi = modules[i];
        if(sliceables.find(mi) == sliceables.end()) {
            mi->setUnsliceable();
        }
    }
}

void flat_module_t::compute_repr()
{
    using namespace aggr;

    std::map<std::string, modulelist_t> module_groups;
    for(unsigned i=0; i != modules.size(); i++) {
        id_module_t* m = modules[i];
        std::string type(m->get_type());
        module_groups[type].push_back(m);
    }

    for(std::map<std::string, modulelist_t>::iterator it = module_groups.begin(); it != module_groups.end(); it++) {
        modulelist_t& ml = it->second;
        compute_repr(it->first, ml);
    }
}

void flat_module_t::compute_repr(const std::string& type, modulelist_t& ms)
{
    using namespace aggr;
    for(unsigned i=0; i != ms.size(); i++) {
        id_module_t* mi = ms[i];
        for(unsigned j=0; j < i; j++) {
            id_module_t* mj = ms[j];
            if(mi->get_repr() != mj->get_repr() && id_module_t::compare(mi, mj)) {
                id_module_t::link(mi, mj);
            }
        }
    }

    std::set<id_module_t*> reprset;
    for(unsigned i=0; i != ms.size(); i++) {
        id_module_t* mi = ms[i];
        id_module_t* mirep = mi->get_repr();
        reprset.insert(mirep);
    }
    std::cout << "type: " << type << "; initial: " << ms.size() << "; final: " << reprset.size() << std::endl;
}

void flat_module_t::cleanup_modules()
{
    using namespace aggr;
    modulelist_t new_modules;

    //std::cout << "-->> initial number of modules: " << modules.size() << std::endl;

    for(unsigned i=0; i != modules.size(); i++) {
        id_module_t* m = modules[i];
        if(!m->is_marked_bad() && !m->is_dominated()) {
            new_modules.push_back(m);
            m->clear_conflicting_modules();
            m->uf_init();
        } else {
            delete m;
        }
    }
    modules.resize(new_modules.size());
    //std::cout << "-->> final  number of modules: " << modules.size() << std::endl;
    std::copy(new_modules.begin(), new_modules.end(), modules.begin());
    renumberModules();
}

void flat_module_t::mark_conflicting_modules(bool compute_conflicts)
{
    using namespace aggr;
    if(compute_conflicts) {
        for(map_t::iterator it = map.begin(); it != map.end(); it++) {
            node_t* n = it->second;
            moduleset_t& modset = *(n->get_modules());

            for(moduleset_t::iterator it = modset.begin(); it != modset.end(); it++) {
                id_module_t* mi = *it;
                for(moduleset_t::iterator jt = modset.begin(); jt != modset.end(); jt++) {
                    id_module_t* mj = *jt;
                    if(mi != mj) {
                        mi->add_conflicting_module(mj);
                    }
                }
            }
        }
    }
    renumberModules();
}

void flat_module_t::renumberModules()
{
    using namespace aggr;
    for(unsigned i=0; i != modules.size(); i++) {
        id_module_t* m = modules[i];
        m->setNumber(i);
    }
}

void flat_module_t::glpk_create_constraints(std::vector< std::vector<int> >& constraints)
{
    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t* n = it->second;
        const moduleset_t& modset = *(n->get_modules());
        if(modset.size() <= 1) continue;
        else {
            std::vector<int> row;
            for(moduleset_t::const_iterator it = modset.begin(); it != modset.end(); it++) {
                int varNum = (*it)->moduleNum();
                assert(varNum < (int)modules.size() && varNum >= 0);
                row.push_back(varNum);
            }
            constraints.push_back(row);
        }
    }
}

void flat_module_t::eliminate_overlaps_glpk()
{
    std::cout << "CREATING THE GLPK MODEL." << std::endl;

    glp_prob *lp;
    lp = glp_create_prob();
    glp_set_prob_name(lp, "overlap");
    glp_set_obj_dir(lp, GLP_MAX);

    std::vector< std::vector<int> > cnst;
    glpk_create_constraints(cnst);

    // create the rows and columns.
    glp_add_cols(lp, modules.size());
    glp_add_rows(lp, cnst.size());

    printf("rows=%d, cols=%d\n", (int)cnst.size(), (int)modules.size());

    for(unsigned i=0; i != modules.size(); i++) {
        char name[16];

        // create and set properties of each variable.
        sprintf(name, "x%u", i);
        glp_set_col_name(lp, i+1, name);
        glp_set_col_kind(lp, i+1, GLP_BV);
        // GETCOEF
        glp_set_obj_coef(lp, i+1, modules[i]->num_internals());
    }

    for(unsigned i=0; i != cnst.size(); i++) {
        char name[16];

        // set the rhs of each constraint.
        sprintf(name, "c%u", i);
        glp_set_row_name(lp, i+1, name);
        glp_set_row_bnds(lp, i+1, GLP_UP, 0, 1);
    }


    // now is the painful part - create the constraint matrix.
    std::vector<int> ia_vec, ja_vec;
    for(unsigned i =0; i != cnst.size(); i++) {
        for(unsigned j=0; j != cnst[i].size(); j++) {
            ia_vec.push_back(i+1);
            ja_vec.push_back(cnst[i][j]+1);
        }
    }

    assert(ia_vec.size() == ja_vec.size());
    int N = ia_vec.size();
    printf("creating %d constraints.\n", N);

    int *ia = new int[N+1];
    int *ja = new int[N+1];
    double *ar = new double[N+1];
    for(int i=1; i <= N; i++) {
        ia[i] = ia_vec[i-1];
        ja[i] = ja_vec[i-1];
        assert(ia[i] >= 1 && ia[i] <= N);
        assert(ja[i] >= 1 && ja[i] <= N);
        ar[i] = 1;
    }
    glp_load_matrix(lp, N, ia, ja, ar);

    // glp_write_lp(lp, NULL, "overlap.lp");

    glp_iocp param;
    glp_init_iocp(&param);
    param.presolve = GLP_ON;
    param.mip_gap = 0.025; /* 2.5% tolerance for non-optimal solution. */
    glp_intopt(lp, &param);

    int cnt = 0;
    for(unsigned i=0; i != modules.size(); i++) {
        int v = (int) glp_mip_col_val(lp, i+1);
        assert(v == 0 || v == 1);
        if(v == 0) {
            modules[i]->set_bad();
            cnt += 1;
        }
    }
    std::cout << "killed " << cnt << " modules!" << std::endl;
}

#ifdef USE_CPLEX
void flat_module_t::eliminate_overlaps_sliceable()
{
    std::cout << "Creating the CPLEX model ..." << std::endl;

    compute_sliceability();

    IloEnv env;
    std::vector<int> solution_values;

    for(int solveiter = (options.minimizeModuleCount ? 0 : 1); solveiter < 2; solveiter++) {

        IloModel model(env);
        IloNumVarArray vars(env);
        IloRangeArray cnst(env);


        // create all the variables.
        int currentVar = 0;
        for(unsigned i=0; i != modules.size(); i++) {
            using namespace aggr;
            id_module_t* m = modules[i];

            int nextVar = m->setILPVars(currentVar)+1;
            assert(nextVar > currentVar);
            for(int j=currentVar; j != nextVar; j++) {
                char name[16];
                sprintf(name, "x%d", j);
                vars.add(IloNumVar(env, 0, 1, ILOINT, name));
            }
            currentVar = nextVar;
        }
        int ilpVars = currentVar;

        std::cout << "# of ILP Variables: " << ilpVars << std::endl;

        // create the objective function.
        IloObjective obj = (options.coverageTarget <= 0) ? IloMaximize(env) : IloMinimize(env);
        IloNumArray coeffs(env, ilpVars);
        for(unsigned i=0; i != modules.size(); i++) {
            using namespace aggr;
            id_module_t* m = modules[i];


            double wt=1;
            if(options.coverageTarget <= 0) {
                if(options.signalFlowAnalysis) {
                    // FIXME: assumes no more than 1e6 nodes in the ckt.
                    if(m->candidate()) { wt = 1.0; }
                    else { wt = 1e6; }
                }

                bool seq = strcmp(m->get_type(), "counter") == 0 || strcmp(m->get_type(), "shiftreg") == 0;
                if(seq) wt *= 50;

                // modify the objective function.
                for(int varIndex = m->getILPStartVar(); varIndex <= m->getILPEndVar(); varIndex++) {
                    int slice_index = varIndex - m->getILPStartVar();
                    coeffs[varIndex] = wt * m->getSliceSize(slice_index);
                    assert(varIndex >= 0 && varIndex < ilpVars);

                }
            } else {
                int start = m->getILPStartVar();
                int end = m->getILPEndVar();
                coeffs[start] = 1;
                for(int i = start+1; i <= end; i++) {
                    coeffs[i] = 0;
                }
            }
        }
        obj.setLinearCoefs(vars, coeffs);

        int numConflictConstraints = 0;
        int cnstPos = 0;

        if(options.coverageTarget > 0) {
            cnst.add(IloRange(env, options.coverageTarget, IloInfinity));
            for(unsigned i=0; i != modules.size(); i++) {
                using namespace aggr;
                id_module_t* m = modules[i];

                for(int varIndex = m->getILPStartVar(); varIndex <= m->getILPEndVar(); varIndex++) {
                    int slice_index = varIndex - m->getILPStartVar();
                    assert(varIndex >= 0 && varIndex < ilpVars);
                    cnst[cnstPos].setLinearCoef(vars[varIndex], m->getSliceSize(slice_index));
                }
            }
            cnstPos += 1;
        }

        for(map_t::iterator it = map.begin(); it != map.end(); it++) {
            node_t* n = it->second;
            const moduleset_t& modules = *(n->get_modules());
            if(modules.size() <= 1) continue;
            else {
                // rhs is <= 1
                cnst.add(IloRange(env, -IloInfinity, 1));
                // iterate throught the modules (i, j etc.) that this gate is
                // covered by and set lhs as x_i + x_j + ...
                for(moduleset_t::const_iterator it = modules.begin(); it != modules.end(); it++) {
                    int varNum = (*it)->getILPVar(n);
                    cnst[cnstPos].setLinearCoef(vars[varNum], 1);
                }
                // just added a constraint.
                numConflictConstraints += 1;
                cnstPos += 1;
            }
        }
        std::cout << "# of conflict constraints: " << numConflictConstraints << std::endl;

        // create the slice and size constraints.
        int numSliceConstraints = 0;
        int numSizeConstraints = 0;
        for(unsigned i=0; i != modules.size(); i++) {
            using namespace aggr;
            id_module_t* m = modules[i];
            if(m->getNumSlices() > 1) {
                int startVar = m->getILPStartVar();
                int endVar = m->getILPEndVar();
                for(int i=startVar+1; i <= endVar; i++) {
                    cnst.add(IloRange(env, 0, IloInfinity));
                    cnst[cnstPos].setLinearCoef(vars[i], -1);
                    cnst[cnstPos].setLinearCoef(vars[startVar], 1);
                    cnstPos += 1;
                    numSliceConstraints += 1;
                }
                cnst.add(IloRange(env, 0, IloInfinity));
                for(int i=startVar+1; i <= endVar; i++) {
                    cnst[cnstPos].setLinearCoef(vars[i], 1);
                }
                cnst[cnstPos].setLinearCoef(vars[startVar], -2);
                cnstPos += 1;
                numSizeConstraints += 1;
            }
        }
        std::cout << "# of slice constraints: " << numSliceConstraints << std::endl;
        std::cout << "# of size constraints : " << numSizeConstraints << std::endl;

        model.add(obj);
        model.add(cnst);

        IloCplex cplex(model);
        if(solveiter == 0) {
            cplex.exportModel("model0.lp");
        } else {
            cplex.exportModel("model1.lp");
        }

        if(solution_values.size() > 0) {
            std::cout << "Using the previous solution as a starting point!" << std::endl;

            assert((int) solution_values.size() == ilpVars);
            IloNumArray values(env, ilpVars);
            for(unsigned i=0; i != solution_values.size(); i++) {
                values[i] = solution_values[i];
            }
            cplex.addMIPStart(vars, values);
            // give up when within 5% of the optimal value.
            cplex.setParam(IloCplex::EpGap, 0.01); 
            // don't waste more than 300s (5 mins) looking for a better solution. 
            cplex.setParam(IloCplex::TiLim, 300); 
        }

        std::cout << "Solving the CPLEX model ..." << std::endl;
        if(!cplex.solve()) {
            std::cerr << "Error: Unable to solve the CPLEX model!" << std::endl;
            exit(1);
        }
        std::cout << "Status    : " << cplex.getStatus() << std::endl;
        std::cout << "Value     : " << cplex.getObjValue() << std::endl;

        // kill the "bad" modules or slices.
        IloNumArray vals(env);
        cplex.getValues(vals, vars);
        int cnt = 0;
        int pruned = 0, pruned_slices = 0;

#ifdef DEBUG_OVERLAP
        int pos=0;
        for(int i=0; i != ilpVars; i++, pos++) {
            std::cout << "x" << i << "=" << int(vals[i]) << " ";
            if(pos == 15) {
                std::cout << std::endl;
                pos = 0;
            }
        }
#endif
        this->achieved_coverage = 0;
        for(unsigned i=0; i != modules.size(); i++) {
            int startVar = modules[i]->getILPStartVar();
            int endVar   = modules[i]->getILPEndVar();
            int startVal = (int) vals[startVar];
            if(startVal != 0) {
                achieved_coverage += modules[i]->getSliceSize(0);
                if(endVar != startVar) {
                    for(int j=startVar+1; j <= endVar; j++) {
                        int val_j = (int) vals[j];

                        if(val_j == 1) {
                            achieved_coverage += modules[i]->getSliceSize(j-startVar);
                        }
                    }
                }
            }
        }

        bool coverageTargetSpecified = (options.coverageTarget > 0);
        if(solveiter == 0 && !coverageTargetSpecified) {
            options.coverageTarget = achieved_coverage; // options.mergeModules ? int(0.99 * achieved_coverage) : achieved_coverage;
            for(int i=0; i != ilpVars; i++) {
                solution_values.push_back(vals[i]);
            }
            continue;
        }

        // cnst[cnstPos].setLinearCoef(vars[varIndex], m->getSliceSize(slice_index));
        for(unsigned i=0; i != modules.size(); i++) {
            int startVar = modules[i]->getILPStartVar();
            int endVar   = modules[i]->getILPEndVar();

            int startVal = (int) vals[startVar];
            if(startVal == 0) {
                modules[i]->set_bad();
                cnt += 1;
            } else {

                if(endVar != startVar) {
                    std::vector<int> good_slices;
                    for(int j=startVar+1; j <= endVar; j++) {
                        int val_j = (int) vals[j];

                        if(val_j == 1) {
                            good_slices.push_back(j-startVar);
                        }
                    }
                    assert(modules[i]->getNumSlices() == endVar-startVar);
                    if(good_slices.size() > 0 && (int)good_slices.size() != modules[i]->getNumSlices()) {
                        pruned += 1;
                        pruned_slices += (modules[i]->getNumSlices() - good_slices.size());
                        modules[i]->pruneSlices(this, good_slices);
                    }
                }
            }
        }
        std::cout << "killed " << cnt << " modules!" << std::endl;
        std::cout << "pruned " << pruned << " modules!" << std::endl;
        std::cout << "pruned " << pruned_slices << " slices!" << std::endl;
        std::cout << "total " << modules.size() << " modules!" << std::endl;

        if(coverageTargetSpecified) break;
    }
    env.end();
}

void flat_module_t::eliminate_overlaps()
{
    std::cout << "CREATING THE CPLEX MODEL." << std::endl;

    IloEnv env;
    IloModel model(env);
    IloNumVarArray vars(env);
    IloRangeArray cnst(env);

    IloObjective obj = (options.coverageTarget <= 0) ? IloMaximize(env) : IloMinimize(env);
    // create all the variables.
    for(unsigned i=0; i != modules.size(); i++) {
        // create a new variable.
        vars.add(IloNumVar(env, 0, 1, ILOINT));
    }

    // create the objective function.
    for(unsigned i=0; i != modules.size(); i++) {
        using namespace aggr;
        id_module_t* m = modules[i];

        if(options.coverageTarget <= 0) {
            // modify the objective function.
            double wt=1;
            if(options.signalFlowAnalysis) {
                // FIXME: assumes no more than 1e6 nodes in the ckt.
                if(m->candidate()) { wt = 1.0; }
                else { wt = 1e6; }
            }
            obj.setLinearCoef(vars[i], wt * m->num_internals());
        } else {
            obj.setLinearCoef(vars[i], 1);
        }
    }

    // create the constraints
#ifdef MODULE_CONSTRAINTS
    for(unsigned i=0; i != modules.size(); i++) {
        using namespace aggr;
        id_module_t* m = modules[i];

        // set the right side of the constraint.
        cnst.add(IloRange(env, -IloInfinity, 1));

        // now create the left size of the constraint.
        cnst[i].setLinearCoef(vars[i], 1);
        const moduleset_t& conflicts = m->get_conflicting_modules();
        for(moduleset_t::iterator it = conflicts.begin(); it != conflicts.end(); it++) {
            aggr::id_module_t* mj = *it;
            cnst[i].setLinearCoef(vars[mj->moduleNum()], 1);
        }
    }
#else
    int numConstraints = 0;
    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t* n = it->second;
        const moduleset_t& modules = *(n->get_modules());
        if(modules.size() <= 1) continue;
        else {
            // rhs is <= 1
            cnst.add(IloRange(env, -IloInfinity, 1));
            // iterate throught the modules (i, j etc.) that this gate is
            // covered by and set lhs as x_i + x_j + ...
            for(moduleset_t::const_iterator it = modules.begin(); it != modules.end(); it++) {
                int varNum = (*it)->moduleNum();
                cnst[numConstraints].setLinearCoef(vars[varNum], 1);
            }
            // just added a constraint.
            numConstraints += 1;
        }
    }
#endif

    // add the final constraint.
    if(options.coverageTarget > 0) {
        int last = modules.size();
        cnst.add(IloRange(env, options.coverageTarget, IloInfinity));
        for(unsigned i=0; i != modules.size(); i++) {
            cnst[last].setLinearCoef(vars[i], modules[i]->num_internals());
        }
    }

    model.add(obj);
    model.add(cnst);

    std::cout << "SOLVING THE CPLEX MODEL." << std::endl;
    IloCplex cplex(model);
    // cplex.exportModel("model.lp");

    if(!cplex.solve()) {
        std::cerr << "Error: Unable to solve the CPLEX model!" << std::endl;
        exit(1);
    }

    std::cout << "Status    : " << cplex.getStatus() << std::endl;
    std::cout << "Value     : " << cplex.getObjValue() << std::endl;

    IloNumArray vals(env);
    cplex.getValues(vals, vars);
    int cnt = 0;
    for(unsigned i=0; i != modules.size(); i++) {
        int v = vals[i];
        assert(v == 0 || v == 1);
        if(v == 0) {
            modules[i]->set_bad();
            cnt += 1;
        }
    }
    std::cout << "killed " << cnt << " modules!" << std::endl;

    env.end();
}
#endif

const char* flat_module_t::get_type(const char* t)
{
    if(strcmp(t, "and2gate") == 0 ||
            strcmp(t, "or2gate") == 0 ||
            strcmp(t, "nand2gate") == 0 ||
            strcmp(t, "nor2gate") == 0 ||
            strcmp(t, "xnor2gate") == 0 ||
            strcmp(t, "xor2gate") == 0) {
        return "gating_func";
    } else if(
            strcmp(t, "andtree") == 0 ||
            strcmp(t, "ortree") == 0 ||
            strcmp(t, "xortree") == 0 ||
            strcmp(t, "cgentree") == 0) {
        return "logic_tree";
    } else if(strstr(t, "demux") == t)  {
        return "demux";
    } else if(strstr(t, "decoder") == t) {
        return "decoder";
    } else if(strstr(t, "mux21") == t) {
        return "mux21";
    } else if(strstr(t, "mux31") == t) {
        return "mux31";
    } else if(strstr(t, "mux41") == t) {
        return "mux41";
    } else if(strstr(t, "clktree") == t) {
        return "clktree";
    } else if(strstr(t, "eqCmp") == t ||
            strstr(t, "neqCmp") == t) {
        return "equality_cmp";
    } else if(strcmp(t, "ram") == 0) {
        return "ram";
    } else if(strcmp(t, "ripple_addsub") == 0) {
        return "rca";
    } else if(strcmp(t, "xbar") == 0) {
        return "xbar";
    } else if((strcmp(t, "upcounter") == 0) || (strcmp(t, "downcounter") == 0)) {
        return "counter";
    } else if(strcmp(t, "shiftregister") == 0) {
        return "shiftreg";
    } else {
        // std::cout << "unknown: " << t << std::endl;
        return "unknown";
    }
}

void flat_module_t::dump_summary(std::ostream& out)
{
    unsigned gates_cov=0, nodes_cov=0;
    unsigned gates_cov_cand=0, nodes_cov_cand=0;
    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t* n = it->second;
        if(!n->is_latch_gate()) {
            if(n->is_covered()) {
                if(n->is_gate()) gates_cov += 1;
                nodes_cov += 1;
            }
            if(n->is_covered_candidate()) {
                if(n->is_gate()) gates_cov_cand += 1;
                nodes_cov_cand += 1;
            }
        }
    }

    std::map<std::string, int> module_counts;
    std::map<std::string, int> module_gate_cover_counts;
    std::map<std::string, int> module_node_cover_counts;

    unsigned modules_cnt = 0;
    for(unsigned i=0; i != modules.size(); i++) {
        using namespace aggr;
        id_module_t* mi = modules[i];
        if(mi->is_dominated() || mi->is_marked_bad()) continue;

        modules_cnt += 1;

        std::string typ(mi->get_type());
        module_counts[typ] += 1;
        module_gate_cover_counts[typ] += mi->num_internal_gates();
        module_node_cover_counts[typ] += mi->num_internal_nodes();
    }

    out << "inputs                  " << std::setw(10) << inputs.size()     << std::endl;
    out << "outputs                 " << std::setw(10) << outputs.size()    << std::endl;
    out << "latches                 " << std::setw(10) << latches.size()    << std::endl;
    out << "gates                   " << std::setw(10) << num_real_gates()  << std::endl;
    out << "gates_covered           " << std::setw(10) << gates_cov         << std::endl;
    out << "nodes_covered           " << std::setw(10) << nodes_cov         << std::endl;
    out << "gates_covered_cand      " << std::setw(10) << gates_cov_cand    << std::endl;
    out << "nodes_covered_cand      " << std::setw(10) << nodes_cov_cand    << std::endl;
    out << "inferred_modules        " << std::setw(10) << modules_cnt       << std::endl;
    out << "achieved_coverage       " << std::setw(10) << achieved_coverage << std::endl;

    for(std::map<std::string, int>::iterator it = module_counts.begin(); it != module_counts.end(); it++) {
        const std::string& type = it->first;
        int modules = it->second;
        int gates = module_gate_cover_counts[type];
        int nodes = module_node_cover_counts[type];

        out << std::setw(10) << std::left << "module"
            << std::setw(20) << std::left << type 
            << std::setw(7) << modules 
            << std::setw(9) << gates 
            << std::setw(9) << nodes << std::endl;
    }

}

void flat_module_t::dump_coverage_info(std::ostream& out)
{
    out << "# of inputs  : " << std::setw(7) << inputs.size() << std::endl;
    out << "# of outputs : " << std::setw(7) << outputs.size() << std::endl;
    out << "# of latches : " << std::setw(7) << latches.size() << std::endl;
    out << "# of gates   : " << std::setw(7) << num_real_gates() << std::endl;
    // first count gates in each type of module.
    std::map<const char*, int, string_cmp_t> count_map, count_map2;
    for(unsigned i=0; i != modules.size(); i++) {
        aggr::id_module_t* m = modules[i];
        if(!m->is_dominated() && !m->is_marked_bad()) {
            const char* t = m->get_type();
            const char* s = get_type(t);
            count_map[t] += 1;
            count_map2[s] += 1;
        }
    }

    // count gates in each type of module.
    int covered = 0;
    int simple_covered = 0;
    int covered_once = 0;
    std::map<const char*, int, string_cmp_t> cover_map, cover_map2;
    for(nodelist_t::iterator it = gates.begin(); it != gates.end(); it++) {
        node_t* n = *it;
        if(n->is_latch_gate()) continue;

        std::map<const char*, int, string_cmp_t> cover_map_local, cml2;
        bool node_covered = false;
        int node_cover_count = 0;
        bool node_simple_covered = false;
        for(moduleset_t::iterator it = n->modules.begin(); it != n->modules.end(); it++) {
            aggr::id_module_t* m = *it;
            const char* t = m->get_type();
            const char* s = get_type(t);
            if(!m->is_dominated() && !m->is_marked_bad()) {
                cover_map_local[t] = 1;
                cml2[s] = 1;
            }
            if(!m->is_marked_bad()) {
                node_covered = true;
                if(!m->candidate()) {
                    node_simple_covered = true;
                    if(!m->is_dominated() && !m->is_marked_bad()) {
                        node_cover_count += 1;
                    }
                }
            }
        }
        if(node_covered) {
            covered += 1;
        }
        if(node_simple_covered) {
            simple_covered += 1;
        }
        if(node_cover_count == 1) {
            covered_once += 1;
        }
        for(std::map<const char*, int, string_cmp_t>::iterator it =  cover_map_local.begin();
                it != cover_map_local.end();
                it++)
        {
            const char* t = it->first;
            cover_map[t] += 1;
        }
        for(std::map<const char*, int, string_cmp_t>::iterator it =  cml2.begin();
                it != cml2.end();
                it++)
        {
            const char* s = it->first;
            cover_map2[s] += 1;
        }
    }

    out << "# module type           number of instances" << std::endl;
    // dump module counts.
    for(std::map<const char*, int, string_cmp_t>::iterator it =  count_map.begin();
            it != count_map.end();
            it++)
    {
        const char* t = it->first;
        out << "module: " << std::left << std::setw(20) << t << " count: " << std::setw(10) << it->second << std::endl;
    }
    out << std::endl << "# module type         number of gates covered" << std::endl;
    for(std::map<const char*, int, string_cmp_t>::iterator it =  cover_map.begin();
            it != cover_map.end();
            it++)
    {
        const char* t = it->first;
        out << "module: " << std::left << std::setw(20) << t << " cover: " << std::setw(10) << it->second << std::endl;
    }

    for(std::map<const char*, int, string_cmp_t>::iterator it = count_map2.begin(),
            jt = cover_map2.begin();
            it != count_map2.end() &&
            jt != cover_map2.end();
            it++,jt++)
    {
        const char* t = it->first;
        const char* s = jt->first;
        assert(strcmp(t, s) == 0);
        printf("%10s\t%5d\t%5d\n", t, it->second, jt->second);
    }
    count_word_types(out);
    out << std::endl;
    out << "# total number of modules                               : " << get_module_count() << std::endl;
    out << "# total number of non-overlapping modules               : " << get_non_overlapping_module_count() << std::endl;

    out << "# total number of undominated modules                   : " << get_undominated_module_count() << std::endl;
    out << "# total number of non-overlapping undominated modules   : " << get_non_overlapping_undominated_module_count() << std::endl;

    out << "# total gates covered                                   : " << covered << std::endl;
    out << "# redundant gates covered                               : " << get_covered_redundant_gate_count() << std::endl;
    out << "# total gates covered by one module                     : " << covered_once << std::endl;
    out << "# total gates covered without candidate modules         : " << simple_covered << std::endl;
    out << "total time taken                                        : " << timer.get_total_time() << std::endl;
}

int flat_module_t::get_covered_redundant_gate_count() const
{
    int cnt = 0;
    for(unsigned i=0; i != gates.size(); i++) {
        node_t* g = gates[i];
        if(g->num_fanouts() == 0) {
            node_t* o = g->get_orig();
            if(o != g && o->is_covered()) {
                if(!g->is_covered()) {
#ifdef DEBUG
                    std::cout << "name: " << g->get_name() << "; type: " << g->get_lib_elem()->get_name()
                        << "; orig: " << o->get_name() << "; type: "
                        << (o->get_lib_elem() ? o->get_lib_elem()->get_name() : "null")
                        << std::endl;
#endif
                    cnt += 1;
                }
            }
        }
    }
    return cnt;
}

void flat_module_t::get_latch_inputs(nodelist_t& outputs, nodelist_t& inputs) const
{
    unsigned sz = outputs.size();
    inputs.resize(sz);

    for(unsigned i=0; i != sz; i++) {
        node_t* n = outputs[i];
        node_t* d = get_latch_input(n);
        inputs[i] = d;
    }
}

node_t* flat_module_t::get_latch_input(node_t* q) const
{
    assert(q->is_latch());
    assert(q->get_module() == this);

    lib_elem_t* libelem = q->get_lib_elem();
    int d_index = libelem->get_input_index("D");
    assert(d_index != -1);
    if(d_index == -1) {
        printf("Error: Unable to find input of latch: '%s'\n", q->get_name().c_str());
        exit(1);
    }

    node_t* d = q->get_input(d_index);
    assert(d != NULL);
    if(d == NULL) {
        printf("Error: Unable to find input of latch: '%s'\n", q->get_name().c_str());
        exit(1);
    }
    return d;
}

bool int_BDD_pair_compare(const std::pair<int, BDD>& a, const std::pair<int, BDD>& b)
{
    return a.first < b.first;
}

bool check_latch(node_t* n, bool fanout_check)
{
    return n->is_latch() && (!fanout_check || n->num_fanouts() <= 4);
}

void flat_module_t::ramAnalysis(bool fanout_check)
{
    using namespace std;
    std::cout << "Starting RAM analysis ... " << std::endl;

    markings_t markings(max_index(), false);
    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        if(check_latch(it->second, fanout_check) ||
                (it->second->is_gate() && it->second->is_inverter() && it->second->get_input(0)->is_latch()) ||
                (it->second->is_gate() && it->second->is_buffer() && it->second->get_input(0)->is_latch()))
        {
            int idx = it->second->get_index();
            markings[idx] = true;
        }
    }
    markFrameBufferOutputs(markings);

    int iter=0;
    while(1) {
        unsigned cnt = 0;
        for(unsigned i=0; i != gates.size(); i++) {
            node_t *g = gates[i];
            if(!g->is_latch_gate() &&
                    !markings[g->get_index()] &&
                    has_marked_support(markings, g) &&
                    g->num_fanouts() == 1)
            {
                markings[g->get_index()] = true;
                cnt += 1;
            }
        }
        std::cout << "iteration: " << ++iter << "; markings: " << cnt << std::endl;
        if(!cnt) break;
    }

    markings_t covered(max_index(), false);
    for(int i=gates.size()-1; i >=0; i--) {
        node_t* g = gates[i];
        int idx = g->get_index();
        if(markings[idx] && !covered[idx]) {
            mux_node_check(fanout_check, g, markings, covered);
        }
        printf("%6d nodes remaining\r", i);
        fflush(stdout);
    }
    printf("\n");

    for(ram_map_t::iterator it = rams.begin(); it != rams.end(); it++)
    {
        ramlist_t& vec = it->second;

        for(unsigned i=0; i != vec.size(); i++) {
            ram_t* ri = vec[i];
            if(ri == NULL) continue;
            for(unsigned j=i+1; j != vec.size(); j++) {
                ram_t* rj = vec[j];
                if(rj == NULL) continue;
                if(ri->same_latches(rj)) {
                    std::copy(rj->rdaddr.begin(), rj->rdaddr.end(), std::inserter(ri->rdaddr, ri->rdaddr.end()));
                    std::copy(rj->outputs.begin(), rj->outputs.end(), std::inserter(ri->outputs, ri->outputs.begin()));
                    delete rj;
                    vec[j] = NULL;
                }
            }
        }
        ramlist_t newvec;
        for(unsigned i=0; i != vec.size(); i++) {
            if(vec[i]) newvec.push_back(vec[i]);
        }
        vec.resize(newvec.size());
        std::copy(newvec.begin(), newvec.end(), vec.begin());

        modulelist_t muxes;
        computeMUX21List(muxes);

        for(unsigned i=0; i != vec.size(); i++) {
            ram_t* r = vec[i];
            extendRAM(r, muxes);

            r->write_analysis();
            aggr::id_module_t* m = r->get_module();
            add_module(m);
            // now try to find muxes at the output of these rams.
        }
    }
    std::cout << "RAM analysis done." << std::endl;
}

void flat_module_t::computeMUX21List(modulelist_t& muxes)
{
    for(unsigned i=0; i != modules.size(); i++) {
        const char* t = modules[i]->get_type();
        if(strstr(t, "mux21") == t) {
            muxes.push_back(modules[i]);
        }
    }
}

void flat_module_t::extendRAM(ram_t* ram, modulelist_t& muxes)
{
    for(unsigned i=0; i != muxes.size(); i++) {
        std::vector<int> indices;
        if(muxCompatibleWithRAM(ram->outputs, muxes[i], indices)) {
            ram->extendRAM(muxes[i], indices);
            return;
        }
    }
}

void flat_module_t::ram_t::extendRAM(aggr::id_module_t* mux, std::vector<int>& indices)
{
    wordlist_t* wl = mux->get_word_outputs();
    assert(wl->size() == 1);
    word_t* w = (*wl)[0];

    std::cout << "outputs : " << outputs.size() << std::endl;
    std::cout << "rdaddr  : " << rdaddr.size() << std::endl;

    outputs.clear();
    for(unsigned i = 0; i != indices.size(); i++) {
        node_t* n = w->get_bit(indices[i]);
        outputs.insert(n);
    }
    std::string selgrp("sel");
    nodelist_t* nl = mux->get_inputs_in_group(selgrp);
    assert(nl->size() == 1);
    rdaddr.insert((*nl)[0]);

    std::cout << "outputs : " << outputs.size() << std::endl;
    std::cout << "rdaddr  : " << rdaddr.size() << std::endl;
}


bool flat_module_t::muxCompatibleWithRAM(const nodeset_t& nodes, aggr::id_module_t* mux, std::vector<int>& indices)
{
    std::map<int, int> index_counts;
    for(nodeset_t::const_iterator it = nodes.begin(); it != nodes.end(); it++) {
        node_t* n = *it;
        int idx = mux->get_word_input_index(n);
        if(idx == -1) {
            return false;
        } else {
            index_counts[idx] += 1;
        }
    }

    for(std::map<int,int>::iterator it =  index_counts.begin(); it != index_counts.end(); it++) {
        int idx = it->first;
        int cnt = it->second;
        if(cnt != 2) {
            return false;
        } else {
            indices.push_back(idx);
        }
    }
    if(nodes.size() != 2*indices.size()) {
        std::cout << "nodes   : " << nodes.size() << std::endl;
        std::cout << "indices : " << indices.size() << std::endl;
    }
    assert(nodes.size() == 2*indices.size());
    return true;
}

bool flat_module_t::ram_t::same_latches(ram_t* other)
{
    if(inputs.size() != other->inputs.size()) return false;
    return std::equal(inputs.begin(), inputs.end(), other->inputs.begin());
}

void flat_module_t::ram_t::addCovers(
        flat_module_t* module,
        BDD& mux,
        nodeset_t& ram_inputs,
        nodeset_t& covered_inputs,
        mux_cover_set_t& covers
        )
{
    using namespace aggr;

    fnInfo_t* inf = module->getFunction(mux);
    if(inf == NULL) return;

    std::vector<uint8_t>& p = inf->permutation;
    int s = p[2], a = p[0], b = p[1];
    for(kcoverset_t::iterator it = inf->canonicalPtr->covers.begin();
            it != inf->canonicalPtr->covers.end();
            it++)
    {
        kcover_t* kc = *it;
        node_t* r = kc->get_root();
        if(ram_inputs.find(r) != ram_inputs.end()) {
            covered_inputs.insert(r);
            bitslice_t bs(kc);
            mux_cover_t mc(kc->get_root(), bs.xs[s], bs.xs[a], bs.xs[b]);
            covers.insert(mc);
        } else {
            // std::cout << "(1) can't find: " << r->get_name() << std::endl;
        }
    }
}

void flat_module_t::ram_t::addCovers2(
        flat_module_t* module,
        nodeset_t& ram_inputs,
        nodeset_t& covered_inputs,
        mux_cover_set_t& covers
        )
{
    using namespace aggr;
    input_provider_t* ipp = get_ipp(4);
    BDD fn = !((ipp->inp(0) & !ipp->inp(1)) + (ipp->inp(2) & ipp->inp(3)));

    fnInfo_t* inf = module->getFunction(fn);
    if(inf == NULL) return;

    std::vector<uint8_t>& p = inf->permutation;
    int ai = p[0], bi = p[1], ci = p[2], di = p[3];
    inf = inf->canonicalPtr;
    for(kcoverset_t::iterator it = inf->covers.begin(); it != inf->covers.end(); it++) {
        kcover_t* kc = *it;
        node_t* r = kc->get_root();
        if(ram_inputs.find(r) == ram_inputs.end()) continue;
        if(covered_inputs.find(r) != covered_inputs.end()) continue;

        bitslice_t bs(kc);
        node_t* y = kc->get_root();

        node_t* a = bs.xs[ai];
        node_t* b = bs.xs[bi];
        node_t* c = bs.xs[ci];
        node_t* d = bs.xs[di];

        /*
           std::cout << "root: " << y->get_name() << "; a: " << a->get_name() << "; b: " << b->get_name()
           << "; c: " << c->get_name() << "; d: " << d->get_name();

           if(!b->is_latch()) {
           std::cout << " b::not_latch" << std::endl;
           continue;
           } else {
           std::cout << " b::is_latch" << std::endl;
           }
           */

        if(module->not_related(a, c)) {
            mux_cover_t mc(y, c, b, d);
            covered_inputs.insert(y);
            covers.insert(mc);
        }
        if(module->not_related(a, d)) {
            mux_cover_t mc(y, d, b, c);
            covered_inputs.insert(y);
            covers.insert(mc);
        }
    }
}

void flat_module_t::ram_t::write_analysis()
{
    using namespace aggr;

    nodeset_t ram_inputs;
    nodeset_t covered_inputs;

    int pos = 0;
    // std::cout << "inputs.size() = " << inputs.size() << std::endl;
    for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++, pos++) {
        node_t* n = *it;
        if(n->is_macro_out()) {
            return;
        }
        assert(n->is_latch());
        node_t* inp = n->get_input(0);
        if(options.ignoreScanInputs) {
            assert(inp->is_latch_gate());
            if(inp->get_si_input() != NULL) {
                assert(inp->get_se_input() != NULL);
                node_t* d_inp = inp->get_d_input();
                if(d_inp == NULL) {
                    // std::cout << "ignoring: " << inp->get_name() << "; instance: " << inp->get_instance_name() << std::endl;
                    // ignore latches that don't have D-inputs (for now anyway.)
                    continue;
                } else {
                    // std::cout << "latch: " << n->get_instance_name() << "; input: " << d_inp->get_instance_name()
                    //           << "; sz: " << ram_inputs.size() << "; pos: " << pos << std::endl;
                    ram_inputs.insert(d_inp);
                }
            } else {
                node_t* d_inp = inp->get_d_input();
                if(d_inp == NULL) {
                    // std::cout << "ignoring: " << inp->get_name() << "; instance: " << inp->get_instance_name() << std::endl;
                    // ignore latches that don't have D-inputs (for now anyway.)
                    continue;
                } else {
                    // std::cout << "latch: " << n->get_instance_name() << "; input: " << d_inp->get_instance_name()
                    //           << "; sz: " << ram_inputs.size() << "; pos: " << pos << std::endl;
                    ram_inputs.insert(d_inp);
                }
            }
        } else {
            ram_inputs.insert(inp);
        }
    }

    // std::cout << "ram_inputs: " << ram_inputs << std::endl;

    input_provider_t* e = get_ipp(3);
    BDD a = e->inp(0), b = e->inp(1), s = e->inp(2);
    BDD mux0 = (!s&a) + (s&b);
    BDD mux1 = (!s&a) + (s&!b);
    BDD mux2 = (!s&!a) + (s&b);

    mux_cover_set_t covers;
    addCovers(module, mux0, ram_inputs, covered_inputs, covers);
    addCovers(module, mux1, ram_inputs, covered_inputs, covers);
    addCovers(module, mux2, ram_inputs, covered_inputs, covers);
    addCovers2(module, ram_inputs, covered_inputs, covers);

    printf("\nFound %d inputs and %d covers.\n", (int) covered_inputs.size(), (int) covers.size());
    if(covered_inputs.size() != inputs.size()) {
        nodeset_t missing;
        std::set_difference(ram_inputs.begin(), ram_inputs.end(), covered_inputs.begin(), covered_inputs.end(), std::inserter(missing, missing.end()));
        // std::cout << "missing inputs: " << missing << std::endl;
        std::cout << "ram_inputs.size() = " << ram_inputs.size() << "; ";
        std::cout << "inputs.size() = " << inputs.size() << "; ";
        std::cout << "covered_inputs.size() = " << covered_inputs.size() << "; ";

        std::cout << "RAM write analysis: couldn't cover all inputs (" << inputs.size()
            << ") with muxes. Giving up!" << std::endl;
        return;
    }
    processCovers(covers);
}

bool flat_module_t::ram_t::processCovers(flat_module_t::ram_t::mux_cover_set_t& covers)
{
    nodeset_t sel_signals;
    selp_map_t selp;
    for(mux_cover_set_t::iterator it = covers.begin(); it != covers.end(); it++) {
        node_t* sel = it->sel;
        node_t* a = it->a;
        node_t* b = it->b;
        sel_signals.insert(sel);

        if(a->is_latch() && (a->get_input(0) == it->root || a->get_input(0)->get_d_input() == it->root)) {
            selp[sel] = 1;
            wrdata.insert(b);
        } else if(b->is_latch() && (b->get_input(0) == it->root || b->get_input(0)->get_d_input() == it->root)) {
            selp[sel] = 0;
            wrdata.insert(a);
        }
    }

    if(selp.size() != sel_signals.size()) {
        std::cout << "Couldn't determine polarity of all select signals. Giving up!" << std::endl;
        return false;
    }

    std::cout << "The total number of select signals is: " << sel_signals.size() << std::endl;
    std::cout << "RAM write data: " << wrdata << std::endl;


    nodeset_t cone;
    create_common_fanin_cone(sel_signals, cone);
    std::cout << "The size of the common fanin cone: " << cone.size() << "." << std::endl;
    if(cone.size() == 0) {
        return false;
    }

    nodeset_t inputs;
    for(nodeset_t::iterator it = sel_signals.begin(); it != sel_signals.end(); it++) {
        node_t* selsig = *it;
        if(inputs.size() == 0) {
            nodeset_t visited;
            find_inputs(selsig, cone, visited, inputs);
            expand_not_gates(inputs);
            std::cout << "decoder inputs: " << inputs << std::endl;
        } else {
            nodeset_t test_inputs;
            nodeset_t visited;
            find_inputs(selsig, cone, visited, test_inputs);
            expand_not_gates(test_inputs);
            if(!std::equal(test_inputs.begin(), test_inputs.end(), inputs.begin())) {
                std::cout << "these inputs: " << test_inputs << std::endl;
                std::cout << "those inputs: " << inputs << std::endl;
                std::cout << "Decoder inputs don't all have the same inputs." << std::endl;
                return false;
            }
        }
    }
    if( verify_decoder(sel_signals, inputs, selp) ) {
        std::cout << "Verification of decoder properties succeeded." << std::endl;
        std::copy(inputs.begin(), inputs.end(), std::inserter(wraddr, wraddr.end()));
        return true;
    } else {
        std::cout << "Verification of decoder properties failed." << std::endl;
        return false;
    }
}

void flat_module_t::expand_not_gates(nodeset_t& inputs)
{
    nodeset_t to_del;
    nodeset_t to_ins;

    for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
        node_t* n = *it;
        if(n->is_gate() && n->is_inverter()) {
            node_t* i = n->get_input(0);
            to_del.insert(n);
            to_ins.insert(i);
        }
    }
    for(nodeset_t::iterator it = to_del.begin(); it != to_del.end(); it++) {
        inputs.erase(*it);
    }
    std::copy(to_ins.begin(),to_ins.end(), std::inserter(inputs, inputs.end()));
}

bool flat_module_t::ram_t::verify_decoder(nodeset_t& sel_signals, nodeset_t& inputs, selp_map_t& selp)
{
    bdd_map_t vars;
    std::vector<BDD> outputs;

    Cudd& mgr = getFullFnMgr();
    BDD zero = mgr.bddZero();
    BDD one = mgr.bddOne();

    int i = 0;
    for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++, i++) {
        node_t* n = *it;
        if(n->get_name() == options.resetSignal) {
            vars[n] = (options.resetPolarity == 0) ? one : zero;
        } else {
            vars[n] = mgr.bddVar(i);
        }
    }

    unsigned sz = vars.size();
    for(nodeset_t::iterator it = sel_signals.begin(); it != sel_signals.end(); it++) {
        BDD o = module->createFullFn(*it, vars, false, -1);
        assert(vars.size() == sz);

        // can't always write or never write.
        if(o == zero || o == one) {
            printf("running into trouble with bdd for %s: \n", (*it)->get_name().c_str());
            printBDD(stdout, o);
            outputs.clear();
            return false;
        }
        assert(selp.find(*it) != selp.end());
        outputs.push_back(selp[*it] == 0 ? !o : o);
    }

    for(unsigned i=0; i != outputs.size(); i++) {
        BDD orgate = mgr.bddZero();
        for(unsigned j=0; j != outputs.size(); j++) {
            if(i != j) {
                orgate = orgate + outputs[j];
            }
        }
        BDD thisgate = outputs[i];
        BDD y = thisgate & orgate;
        if(y != zero) {
            std::cout << "orgate:"; printBDD(stdout, orgate);
            std::cout << "thisgate:"; printBDD(stdout, thisgate);
            std::cout << "y:"; printBDD(stdout, y);
            outputs.clear();
            return false;
        }
    }
    outputs.clear();
    return true;
}

void flat_module_t::analyzeCommonInputs()
{
    analyzeCommonInputs(gates, true);
}

void flat_module_t::analyzeCommonInputs(const nodelist_t& gates_in, bool update_support_rep)
{
    std::cout << "analyzing common inputs ... " << std::endl;

    node_wrapper_list_t nodes;
    for(unsigned i=0; i != gates_in.size(); i++) {
        node_wrapper_t n;
        n.node = gates_in[i];
        nodes.push_back(n);
    }
    for(unsigned i=0; i != nodes.size(); i++) {
        nodes[i].init();
    }

    for(unsigned i=0; i != nodes.size(); i++) {
        if(nodes[i].node->is_latch_gate()) continue;

        node_wrapper_t* a = &nodes[i];
        const nodeset_t& a_in = a->node->get_fanin_cone_inputs();
        for(unsigned j=0; j < i; j++) {
            node_wrapper_t* b = &nodes[j];
            if(find_set(a) != find_set(b)) {
                const nodeset_t& b_in = b->node->get_fanin_cone_inputs();
                if(a_in.size() == b_in.size()) {
                    if(std::equal(a_in.begin(), a_in.end(), b_in.begin())) {
                        link(a, b);
                    }
                }
            }
        }
        if((i % 100) == 0) {
            printf("PROGRESS %6d/%6d                      \r", i+1, (int)nodes.size());
            fflush(stdout);
        }
    }
    std::cout << std::endl << "done analyzing common inputs." << std::endl;

    typedef std::map<node_wrapper_t*, nodeset_t> group_t;
    group_t groups;
    for(unsigned i=0; i != nodes.size(); i++) {
        node_wrapper_t* rt = find_set(&nodes[i]);
        if(update_support_rep) {
            nodes[i].node->set_support_rep(rt->node);
        }
        groups[rt].insert(nodes[i].node);
    }

    //std::ofstream fout("groups.txt");
    for(group_t::iterator it = groups.begin(); it != groups.end(); it++) {
        if(it->second.size() >= 4) {
            //fout << it->second.size() << " : " << it->second << std::endl;
            modulelist_t mods;
            analyzeDecoders(it->second, mods);
            for(unsigned i=0; i != mods.size(); i++) {
                add_module(mods[i]);
            }
        }
    }
    //fout.close();
}

void flat_module_t::analyzeDecoders(const nodeset_t& nodes, modulelist_t& mods)
{
    nodeset_t cone;
    create_common_fanin_cone(nodes, cone);

    std::map<nodeset_t, nodeset_t> imp;
    for(nodeset_t::iterator it = nodes.begin(); it != nodes.end(); it++) {
        node_t* selsig = *it;

        nodeset_t inputs;
        nodeset_t visited;
        find_inputs(selsig, cone, visited, inputs);
        imp[inputs].insert(selsig);
    }

    for(std::map<nodeset_t, nodeset_t>::iterator it = imp.begin(); it != imp.end(); it++) {
        const nodeset_t& inputs = it->first;
        const nodeset_t& nodes = it->second;
        if(nodes.size() >= 4 && inputs.size() <= 32) {
            aggr::id_module_t* mod = analyzeCommonNodes(inputs, nodes);
            if(mod) mods.push_back(mod);
        }
    }
}

aggr::id_module_t* flat_module_t::analyzeCommonNodes(const nodeset_t& inputs, const nodeset_t& nodes)
{
    if(inputs.size() < 2) return NULL;

    Cudd& mgr = getFullFnMgr();
    bdd_map_t vars;

    int i = 0;
    for(nodeset_t::const_iterator it = inputs.begin(); it != inputs.end(); it++, i++) {
        node_t* n = *it;
        vars[n] = mgr.bddVar(i);
    }

    std::set<BDD, bdd_compare_t> funcs;
    
    unsigned sz = vars.size();
    // std::cout << "analyzing: " << nodes << std::endl;
    // std::cout << "inputs: " << inputs << std::endl;
    for(nodeset_t::const_iterator it = nodes.begin(); it != nodes.end(); it++) {
        // std::cout << "current node: " << (*it)->get_name() << std::endl;
        BDD o = createFullFn(*it, vars, false, -1);
        assert(vars.size() == sz);
        funcs.insert(o);
    }

    if(nodes.size() >= 4 &&
            (checkExclusivity(true, mgr, funcs) ||
             checkExclusivity(false, mgr, funcs)))
    {
        using namespace aggr;

        //std::cout << "found decoder with inputs: " << inputs.size() << " outputs: " << nodes.size() << std::endl;
        id_module_t* mod = new id_module_t("decoder", id_module_t::SLICEABLE, id_module_t::INFERRED);
        for(nodeset_t::iterator it = nodes.begin(); it != nodes.end(); it++) {
            mod->add_output(*it);
        }
        for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
            mod->add_input(*it);
        }
        mod->compute_internals();
        return mod;
    }
    return NULL;
}

bool flat_module_t::checkPopCnt(bool inv, int vars, Cudd& mgr, std::vector<BDD>& funcs)
{
    assert(vars > 1);
    BDD b0 = mgr.bddVar(0);
    for(int i = 1; i < vars; i++) {
        BDD vi = mgr.bddVar(i);
        b0 = b0.Xor(vi);
    }
    for(unsigned i=0; i != funcs.size(); i++) {
        if(funcs[i] == b0) {
            std::cout << std::endl << "FOUND A POPCNT B0: " << i << std::endl;
            return true;
        }
    }
    return false;
}

bool flat_module_t::checkExclusivity(bool polarity, Cudd& mgr, std::set<BDD, bdd_compare_t>& funcs)
{
    for(std::set<BDD, bdd_compare_t>::iterator it = funcs.begin(); it != funcs.end(); it++) {
        BDD or_func = mgr.bddZero();
        BDD bi = *it;
        for(std::set<BDD, bdd_compare_t>::iterator jt = funcs.begin(); jt != funcs.end(); jt++) {
            if(it != jt) {
                BDD bj = *jt;
                or_func = or_func + (polarity ? bj : !(bj));
            }
        }
        if((or_func & (polarity ? bi : !bi)) != mgr.bddZero()) {
            return false;
        }
    }
    return true;
}

void flat_module_t::create_common_fanin_cone(const nodeset_t& signals, nodeset_t& cone)
{
    bool first = true;
    for(nodeset_t::const_iterator it = signals.begin(); it != signals.end(); it++) {
        const nodeset_t& f = (*it)->get_fanin_cone();
        if(first) {
            for(nodeset_t::const_iterator it = f.begin(); it != f.end(); it++) {
                node_t* n = *it;
                cone.insert(n);
                for(node_t::input_iterator jt = n->inputs_begin(); jt != n->inputs_end(); jt++) {
                    node_t* inp = *jt;
                    cone.insert(inp);
                }
            }
        } else {
            nodeset_t new_cone;
            for(nodeset_t::const_iterator it = f.begin(); it != f.end(); it++) {
                node_t* n = *it;
                new_cone.insert(n);
                for(node_t::input_iterator jt = n->inputs_begin(); jt != n->inputs_end(); jt++) {
                    node_t* inp = *jt;
                    new_cone.insert(inp);
                }
            }
            /* now do inplace intersection. */
            inplace_intersection(cone, new_cone);
        }
        first = false;
    }
}

void inplace_intersection(nodeset_t& cone, const nodeset_t& new_cone)
{
    nodeset_t::iterator it1 = cone.begin();
    nodeset_t::iterator it2 = new_cone.begin();
    while ( (it1 != cone.end()) && (it2 != new_cone.end()) ) {
        if (*it1 < *it2) {
            cone.erase(it1++);
        } else if (*it2 < *it1) {
            ++it2;
        } else { // *it1 == *it2
            ++it1;
            ++it2;
        }
    }
    cone.erase(it1, cone.end());
}

aggr::id_module_t* flat_module_t::ram_t::get_module() const
{
    using namespace aggr;
    id_module_t* m = new id_module_t("ram", id_module_t::UNSLICEABLE, id_module_t::INFERRED);

    // CONSIDER: convert this to a word?
    std::ostringstream comm;

    comm << "rd addr: ";
    word_t* raw = new word_t(false, word_t::RF_RD_ADDR);
    for(nodeset_t::iterator it = rdaddr.begin(); it != rdaddr.end(); it++) {
        node_t* n = *it;
        raw->add_bit(n);
        comm << n->get_name() << " ";
    }
    raw = module->get_canonical_word(raw);
    module->add_word(raw);
    m->add_input_word(raw);

    comm << std::endl;

    comm << "  // latches: ";
    for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
        node_t* n = *it;
        comm << n->get_name() << " ";
        if(n->is_macro_out()) comm << " (rfout) ";
    }
    comm << std::endl;

    if(wraddr.size() == 0) {
        for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
            node_t* n = *it;
            m->add_input(n);
        }
    } else {
        comm << "  // wr data: ";
        word_t* wdw = new word_t(false, word_t::RF_WR_DATA);
        for(nodeset_t::iterator it = wrdata.begin(); it != wrdata.end(); it++) {
            node_t* n = *it;
            wdw->add_bit(n);
            comm << n->get_name() << " ";
        }
        wdw = module->get_canonical_word(wdw);
        module->add_word(wdw);
        m->add_input_word(wdw);
        comm << std::endl;

        comm << "  // wr addr: ";
        word_t* waw = new word_t(false, word_t::RF_WR_ADDR);
        for(nodeset_t::iterator it = wraddr.begin(); it != wraddr.end(); it++) {
            node_t* n = *it;
            waw->add_bit(n);
            comm << n->get_name() << " ";
        }
        waw = module->get_canonical_word(waw);
        module->add_word(waw);
        m->add_input_word(waw);
        comm << std::endl;
    }
    comm << "  // rd data: ";
    word_t* rdw = new word_t(false, word_t::RF_RD_DATA);
    for(::nodeset_t::const_iterator it = outputs.begin(); it != outputs.end(); it++) {
        node_t* n = *it;
        rdw->add_bit(n);
        comm << n->get_name() << " ";
    }
    rdw = module->get_canonical_word(rdw);
    module->add_word(rdw);
    m->add_output_word(rdw);
    comm << std::endl;

    std::string comm_str = comm.str();
    m->add_comment(comm_str);

    std::cout << "read data word: [" << rdw->size() << "] -- " << *rdw << std::endl;

    // deal with other inputs.

    if(wraddr.size() != 0) {
        for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
            node_t* l = *it;
            assert(l->is_latch());

            // deal with the clock inputs of the latches.
            node_t* clk = l->get_input(1);
            m->add_input(clk);

            node_t* g = l->get_input(0);
            for(node_t::input_iterator gt = g->inputs_begin(); gt != g->inputs_end(); gt++) {
                node_t* gi = *gt;
                if(gi == g->get_d_input()) {
                    continue;
                } else if(gi == g->get_si_input()) {
                    if(options.ignoreScanInputs) {
                        if(gi->is_gate() && !gi->is_latch_gate() && gi->is_inverter())
                        {
                            node_t* inp = gi->get_input(0);
                            if(inputs.find(inp) != inputs.end()) {
                                continue;
                            }
                        }
                        if(inputs.find(gi) == inputs.end()) {
                            m->add_input(gi);
                        }
                    } else {
                        continue;
                    }
                } else if(gi == g->get_se_input()) {
                    if(options.ignoreScanInputs) {
                        m->add_input(gi);
                    } else {
                        continue;
                    }
                } else {
                    m->add_input(gi);
                }
            }
        }
    }

    m->compute_internals();
    return m;
}

void flat_module_t::compute_cover_distance()
{
    std::vector<int> cover_distance(max_index(), INT_MAX);
    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t* n = it->second;
        int idx = n->get_index();
        if(n->is_covered()) {
            cover_distance[idx] = 0;
        }
    }

    bool changed = false;
    int iter=1;
    do {
        changed = false;
        for(map_t::iterator it = map.begin(); it != map.end(); it++) {
            node_t* n = it->second;
            if(!n->is_covered()) {
                int idx = n->get_index();
                int this_dist = cover_distance[idx];

                for(node_t::input_iterator jt = n->inputs_begin(); jt != n->inputs_end(); jt++) {
                    node_t* inp = *jt;
                    int inp_dist = cover_distance[inp->get_index()];
                    if(inp_dist != INT_MAX && (inp_dist + 1) < this_dist) {
                        this_dist = inp_dist + 1;
                        changed = true;
                    }
                }
                for(node_t::fanout_iterator jt = n->fanouts_begin(); jt != n->fanouts_end(); jt++) {
                    node_t* fout = *jt;
                    int fout_dist = cover_distance[fout->get_index()];
                    if(fout_dist != INT_MAX && (fout_dist + 1) < this_dist) {
                        this_dist = fout_dist + 1;
                        changed = true;
                    }
                }

                cover_distance[idx] = this_dist;
            }
        }
        printf("iteration #%3d of cover distance computation ...\r", iter++);
        fflush(stdout);
    } while(changed);

    std::map<int, int> hist;
    for(unsigned i=0; i != cover_distance.size(); i++) {
        hist[cover_distance[i]] += 1;
    }

    std::cout << "\nHistogram of distances from covered nodes." << std::endl;
    for(std::map<int, int>::iterator it = hist.begin(); it != hist.end(); it++) {
        std::cout << std::setw(6) << it->first << ":" << std::setw(8) << it->second << std::endl;
    }
}

bool flat_module_t::has_marked_support(markings_t& markings, node_t* n)
{
    for(node_t::input_iterator it = n->inputs_begin(); it != n->inputs_end(); it++)
    {
        node_t* inp = *it;
        if(markings[inp->get_index()]) return true;
    }
    return false;
}

void flat_module_t::trim_inputs(intnodelist_t& inputs, int& num_selects, int num_latches)
{
    nodeset_t sel_inputs;
    for(int i=0; i != num_selects; i++) {
        sel_inputs.insert(inputs[i].second);
    }
    std::vector<bool> redundant(num_selects, false);
    for(int i=0; i != num_selects; i++) {
        node_t* n = inputs[i].second;
        nodeset_t these_inputs;

        sel_inputs.erase(n);
        nodeset_t visited;
        find_inputs(n, sel_inputs, visited, these_inputs);
        if(std::includes(sel_inputs.begin(), sel_inputs.end(), these_inputs.begin(), these_inputs.end())) {
            redundant[i] = true;
        }
        sel_inputs.insert(n);
    }

    int new_num_selects = 0;
    intnodelist_t new_inputs;
    for(int i=0; i != num_selects; i++) {
        if(!redundant[i]) {
            new_num_selects += 1;
            new_inputs.push_back(inputs[i]);
        }
    }
    for(int i=0; i != num_latches; i++) {
        new_inputs.push_back(inputs[i+num_selects]);
    }

    inputs.resize(new_inputs.size());
    std::copy(new_inputs.begin(), new_inputs.end(), inputs.begin());
    num_selects = new_num_selects;

    /*
       std::cout << "old inputs:";
       for(unsigned i=0; i != inputs.size(); i++) {
       std::cout << inputs[i].second->get_name() << ": " << inputs[i].first << " ";
       }
       std::cout << std::endl;

       std::cout << "new inputs:";
       for(unsigned i=0; i != new_inputs.size(); i++) {
       std::cout << new_inputs[i].second->get_name() << ": " << new_inputs[i].first << " ";
       }
       std::cout << std::endl;
       */
}

void flat_module_t::reduce_inputs(node_t* node, intnodelist_t& inputs, int& num_selects, int num_latches)
{
    assert(num_selects > 0);

    nodelist_t sel_inputs;
    std::vector<nodeset_t> sel_fanin_cones(num_selects);

    // create the inputs array.
    sel_inputs.reserve(num_selects);
    for(int i=0; i != num_selects; i++) {
        sel_inputs.push_back(inputs[i].second);
    }
    // now compute fanin cones for each.
    for(int i=0; i != num_selects; i++) {
        node_t* n = sel_inputs[i];
        const nodeset_t& s = n->get_fanin_cone();

        add_set_with_inputs(sel_fanin_cones[i], s);
    }
    // now compute the intersection of these cones.
    nodeset_t common_fanin_cone;
    const nodeset_t& s0 = *sel_fanin_cones.begin();
    for(nodeset_t::const_iterator it = s0.begin(); it != s0.end(); it++) {
        node_t* n = *it;
        if(in_all_sets(sel_fanin_cones, n)) {
            common_fanin_cone.insert(n);
        }
    }

    if(common_fanin_cone.size() == 0) {
        return;
    }

    std::vector<nodeset_t> input_sets(num_selects);
    for(int i = 0; i != num_selects; i++) {
        nodeset_t visited;
        find_inputs(sel_inputs[i], common_fanin_cone, visited, input_sets[i]);
    }

    bool valid = true;
    unsigned sz = (input_sets.begin())->size();
    for(int i = 0; i != num_selects; i++) {
        if(input_sets[i].size() != sz) {
            valid = false;
            break;
        }
    }

    if(valid) {
        nodeset_t& p = *input_sets.begin();
        for(int i = 0; i != num_selects; i++) {
            nodeset_t& q = input_sets[i];
            if(!std::equal(p.begin(), p.end(), q.begin())) {
                valid = false;
                break;
            }
        }
    }

    if(valid && sz > 0) {
        intnodelist_t new_inputs;
        for(nodeset_t::iterator it = input_sets[0].begin(); it != input_sets[0].end(); it++) {
            node_t* inp = *it;
            new_inputs.push_back(intnode_t(0, inp));
        }
        for(int j=0; j != num_latches; j++) {
            new_inputs.push_back(inputs[j+num_selects]);
        }
        inputs.resize(new_inputs.size());
        std::copy(new_inputs.begin(), new_inputs.end(), inputs.begin());
        num_selects = input_sets[0].size();
    }
}

void flat_module_t::add_set_with_inputs(nodeset_t& out, const nodeset_t& in)
{
    for(nodeset_t::iterator it = in.begin(); it != in.end(); it++) {
        node_t* n = *it;
        out.insert(n);
        for(node_t::input_iterator it = n->inputs_begin(); it != n->inputs_end(); it++) {
            node_t* inp = *it;
            out.insert(inp);
        }
    }
}

void flat_module_t::find_inputs(node_t* n, const nodeset_t& cone, nodeset_t& visited, nodeset_t& inputs)
{
    if(visited.find(n) != visited.end()) return;
    if(cone.find(n) != cone.end() || n->is_latch() || n->is_input() || n->is_macro() || n->is_macro_out()) {
        inputs.insert(n);
    } else {
        visited.insert(n);
        for(node_t::input_iterator it = n->inputs_begin(); it != n->inputs_end(); it++) {
            node_t* inp = *it;
            find_inputs(inp, cone, visited, inputs);
        }
    }
}

bool flat_module_t::in_all_sets(std::vector<nodeset_t>& sets, node_t* n)
{
    for(unsigned i=0; i != sets.size(); i++) {
        if(sets[i].find(n) == sets[i].end()) return false;
    }
    return true;
}

void flat_module_t::compute_internals(node_t* node, intnodelist_t& inputs, nodeset_t& internals)
{
    if(is_node_present(inputs, node)) return;
    else {
        internals.insert(node);
        for(node_t::input_iterator it = node->inputs_begin(); it != node->inputs_end(); it++)
        {
            compute_internals(*it, inputs, internals);
        }
    }
}

bool flat_module_t::intnode_cmp_t::operator() (const flat_module_t::intnode_t& a, const flat_module_t::intnode_t& b) const
{
    int n1 = (int) check_latch(a.second, fanout_check) || (int) a.second->is_macro_out();
    int n2 = (int) check_latch(b.second, fanout_check) || (int) b.second->is_macro_out();
    if(n1 < n2) return true;
    else if(n1 > n2) return false;
    else {
        return (a.first < b.first);
    }
}

bool flat_module_t::mux_node_check(bool fanout_check, node_t* node, markings_t& markings, markings_t& covered)
{
    intnodelist_t inputs;

    do_mux_dfs(node, inputs, markings, 0, NULL);
    intnode_cmp_t cmp(fanout_check);
    std::sort(inputs.begin(), inputs.end(), cmp);

    int num_selects=0;
    int num_latches=0;

    for(unsigned i=0; i != inputs.size(); i++) {
        if(check_latch(inputs[i].second, fanout_check) || inputs[i].second->is_macro_out() ) {
            num_latches += 1;
        } else {
            assert(num_latches == 0);
            num_selects += 1;
        }
    }

    if(num_selects == num_latches) {
        // try to convert inputs to the undecoded form.
        reduce_inputs(node, inputs, num_selects, num_latches);
    }

    // trim inputs.
    trim_inputs(inputs, num_selects, num_latches);

    // trim based on size.
    int min_ram_size = 1 << (num_selects - 1);
    int max_ram_size = 1 << num_selects;
    if(num_latches < min_ram_size || num_latches > max_ram_size) {
        return false;
    }

    bdd_map_t vars;
    Cudd& mgr = getFullFnMgr();
    for(int i=0; i != num_selects+num_latches; i++) {
        vars[inputs[i].second] = mgr.bddVar(i);
    }

    if(num_selects == 0 || num_latches == 0) {
        vars.clear();
        return false;
    }
    // if num_latches is a power of two then
    // 2^num_selects should be num_latches.
    if((num_latches & (num_latches - 1)) == 0) {
        if((1 << num_selects) != num_latches) {
            return false;
        }
    }

    if(num_selects > 16) {
        vars.clear();
        return false;
    }
    if(num_selects > num_latches) {
        vars.clear();
        return false;
    }

    unsigned sz = vars.size();
    BDD fullfn = createFullFn(node, vars, false, -1);
    assert(sz == vars.size());

    markings_t latches(num_latches, false);

    int max = 1<<num_selects;
    bool valid = true;
    BDD k_one = mgr.bddOne();
    BDD k_zero = mgr.bddZero();
    for(int i=0; i != max; i++) {
        BDD b = mgr.bddOne();
        for(int j=0; j != num_selects; j++) {
            BDD x = mgr.bddVar(j);
            int mask = 1 << j;
            if(i & mask) { b = b & x; }
            else { b = b & !x; }
        }
        BDD cf = fullfn.Cofactor(b);
        if(cf == k_one || cf == k_zero) {
            continue;
        }
        if(cf.SupportSize() != 1) {
            // don't allow the output to depend on more than one input.
            valid = false;
            break;
        } else {
            BDD supp = cf.Support();
            bool set_flag = false;
            for(int j=0; j != num_latches; j++) {
                BDD vi = mgr.bddVar(j+num_selects);
                if((supp & vi) == vi) {
                    latches[j] = true;
                    set_flag = true;
                    break;
                }
            }
            if(!set_flag) {
                valid = false;
            }
        }
    }

    if(valid) {
        for(unsigned i=0; i != latches.size(); i++) {
            if(latches[i] == false) {
                // std::cout << "no route to " << inputs[i+num_selects].second->get_name() << std::endl;
                valid = false;
                break;
            }
        }
    }

    if(valid && num_latches >= 8) {
        add_ram(inputs, node, num_selects, num_latches);
        // we can destroy inputs now.
        inputs.clear();
        nodeset_t internals;
        do_mux_dfs(node, inputs, markings, 0, &internals);
        for(nodeset_t::iterator it = internals.begin(); it != internals.end(); it++) {
            node_t* n = *it;
            int idx = n->get_index();
            covered[idx] = true;
        }
    }
    vars.clear();
    return valid;
}

flat_module_t::ram_t::ram_t(intnodelist_t& inps, int num_sel, int num_lat, node_t* o)
{
    module = o->get_module();
    for(int i=0; i != (int)inps.size(); i++) {
        node_t* ni = inps[i].second;
        if(i < num_sel) {
            rdaddr.insert(ni);
        } else {
            inputs.insert(ni);
        }
    }
    outputs.insert(o);
}

bool flat_module_t::ram_t::same_inputs(intnodelist_t& inps, int num_sel, int num_lat) const
{
    assert((int)inps.size() == (num_sel + num_lat));
    if(num_sel != (int)rdaddr.size()) {
        // std::cout << "count different." << std::endl;
        // printf("selects: this=%5d that=%5d\n", (int) rdaddr.size(), (int) num_sel);
        return false;
    }
    bool rdaddr_match = true;
    for(int i=0; i != num_sel; i++) {
        node_t* ni = inps[i].second;
        if(rdaddr.find(ni) == rdaddr.end()) {
            // std::cout << "can't find rdaddr:" << ni->get_name() << std::endl;
            rdaddr_match = false;
            break;
        }
    }
    return rdaddr_match;
}
void flat_module_t::add_ram(intnodelist_t& inputs, node_t* output, int num_sel, int num_lat)
{
    // std::cout << "FOUND RAM: rdaddr: " << num_sel << "; latches: " << num_lat
    //           << "; node: " << output->get_name() << std::endl;

    intpair_t p(num_sel, num_lat);
    bool found = false;
    if(rams.find(p) != rams.end()) {
        ramlist_t& vec = rams[p];
        for(unsigned i=0; i != vec.size(); i++) {
            ram_t* r = vec[i];
            if(r->same_inputs(inputs, num_sel, num_lat)) {
                r->add_output(output);
                for(int j = num_sel; j != (int)inputs.size(); j++) {
                    node_t* nj = inputs[j].second;
                    r->add_input(nj);
                }
                found = true;
                break;
            }
        }
    }
    if(!found) {
        ram_t* r = new ram_t(inputs, num_sel, num_lat, output);
        rams[p].push_back(r);
    }
}

void flat_module_t::do_mux_dfs(node_t* node, intnodelist_t& inputs, markings_t& markings, int level, nodeset_t* internals)
{
    if(is_node_present(inputs, node)) return;
    int idx = node->get_index();
    if(markings[idx] && node->is_gate()) {
        if(internals != NULL) {
            internals->insert(node);
        }
        for(node_t::input_iterator it = node->inputs_begin(); it != node->inputs_end(); it++) {
            node_t* inp = *it;
            do_mux_dfs(inp, inputs, markings, level+1, internals);
        }
    } else {
        intnode_t p(level, node);
        inputs.push_back(p);
    }
}

bool flat_module_t::is_node_present(intnodelist_t& inputs, node_t* n)
{
    for(intnodelist_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
        if(it->second == n) return true;
    }
    return false;
}




bool BDD_int_int_pair_compare(const std::pair<BDD, std::pair<int, int> >& a, const std::pair<BDD, std::pair<int, int> >& b)
{
    return a.second.first > b.second.first;
}

void flat_module_t::partialFunctionAnalysis()
{

    FILE* fp = NULL;
    if(options.partialFunctionOutFile.size() > 0) {
        fp = fopen(options.partialFunctionOutFile.c_str(), "wt");
        if(fp == NULL) {
            fprintf(stderr,
                    "Unable to create file: %s.\n",
                    options.partialFunctionOutFile.c_str());
            exit(1);
        }
    } else {
        return;
    }
    for(flat_module_list_t::iterator fl = partialFuncModules.begin(); fl != partialFuncModules.end(); fl++)     {
        (*fl)->partialFunctionAnalysis(fp);

        for(std::vector<BDD>::iterator it = (*fl)->partialFuncBDDs.begin(); it != (*fl)->partialFuncBDDs.end(); it++) {
            printBDD(fp, *it);

            fnInfo_t* fi = getFunction(*it);
            if(fi != NULL) processLibElemMatch(fp, (*fl), 0, fi->canonicalPtr, 0);

            BDD ti = !*it;
            fnInfo_t* fin = getFunction(ti);
            if(fin != NULL) processLibElemMatch(fp, (*fl), 0, fin->canonicalPtr, 1);

        }
    }

    if(fp) {
        fclose(fp);
        fp = NULL;
    }

}

void flat_module_t::partialFunctionAnalysis(FILE* fp)
{

    kcover_analysis();
    dump_nodecounts(std::cout);
    typedef std::pair<int, int> gatefreq_t;
    typedef std::map<BDD, gatefreq_t, flat_module_t::bdd_compare_t> BDDfreq_map_t;
    BDDfreq_map_t gatefrequencies;
    typedef std::map<fnInfo_t*, int> countmap_t;
    countmap_t countMap;

    for(nodelist_t::iterator it = gates.begin(); it != gates.end(); it++) {
        //kcover_t* cov = (*it)->get_deepest();
        //if (cov == NULL) { continue; }

        kcoverlist_t* deepestCovers = (*it)->get_n_deepest(10);
        for(kcoverlist_t::iterator kc = deepestCovers->begin(); kc != deepestCovers->end(); kc++) {
            BDD fn = (*kc)->getFn(ipps[(*kc)->size()]);
            fnInfo_t* info = getFunction(fn)->canonicalPtr;
            countMap[info]++;
        }
        delete deepestCovers;
    }

    typedef std::vector<std::pair<int, fnInfo_t*> > sortablelist_t;
    sortablelist_t sortableList;
    for(countmap_t::iterator it = countMap.begin(); it != countMap.end(); it++) {
        std::pair<int, fnInfo_t*> p(it->second, it->first);
        sortableList.push_back(p);
    }

    std::sort(sortableList.begin(), sortableList.end());
    int i = 0;
    for(sortablelist_t::iterator it = sortableList.begin(); it != sortableList.end() && i < 10; it++, i++) {
        BDD fn = getBDDfromFn(it->second);
        partialFuncBDDs.push_back(fn);
    }

    /*for(bddset_t::iterator it = bdds.begin(); it != bdds.end(); it++) {
      const BDD& b = it->first;
      fnInfo_t& i = it->second;

      if(!i.isCanonical) continue;
      else {
      unsigned gateCount = 0;
      unsigned kcoverinstances = 0;
      for(kcoverset_t::iterator er = i.covers.begin(); er != i.covers.end(); er++) {
      (*er)->compute_internals((*er)->get_root());
      nodeset_t& internalnodes = (*er)->get_internals();
      gateCount += internalnodes.size();
      kcoverinstances++;
      internalnodes.clear();
      }
      gatefrequencies[b] = gatefreq_t(kcoverinstances, gateCount);
      }
      }

      typedef std::vector<std::pair<BDD, gatefreq_t> > sortablelist_t;
      sortablelist_t topFunctions;
      for(BDDfreq_map_t::iterator it = gatefrequencies.begin(); it != gatefrequencies.end(); it++) {
      topFunctions.push_back(*it);
      }

      std::sort(topFunctions.begin(), topFunctions.end(), BDD_int_int_pair_compare);
      int j = 0;
      for(sortablelist_t::iterator it = topFunctions.begin();
      it != topFunctions.end() && j < 10; it++, j++) {

    //fprintf(fp, "%d,  %d\n", it->second.first, it->second.second);
    partialFuncBDDs.push_back(it->first);
    //fp << it->second.first << "   " << it->second.second << std::endl;
    }   */
}

BDD flat_module_t::getBDDfromFn(fnInfo_t* info) {
    for(bddset_t::iterator it = bdds.begin(); it != bdds.end(); it++) {
        if (&it->second == info) return it->first;
    }
    assert(false);
}

void flat_module_t::signalFlowAnalysis()
{
    typedef std::pair<int, BDD> count_t;
    typedef std::vector<count_t> countlist_t;
    countlist_t counts;

    //Create unknown modules with four common signals
    aggr::simple_common_signal_enumerator_t enumerator( this, aggr::pred1_true );
    aggr::unknown_module_creator_t creator(
            this,
            "candidates_commonSignal",
            options.signalFlowAnalysisCommonSignalMinSize,
            aggr::id_module_t::CANDIDATE_COMMON_SIGNAL
            // aggr::id_module_t::CANDIDATE_COMMON_SIGNAL
            );
    // creator.create_partially_covered_modules = true;


    // spramod disabled this on 26 January 2013.
#if 0
    //Create unknown modules with four common singals
    aggr::common_signals_4_t enumerator4( this, aggr::pred4_true );
    aggr::unknown_module_creator_t creator4(
            this,
            "candidates_commonSignal4",
            options.signalFlowAnalysisCommonSignalMinSize,
            aggr::id_module_t::CANDIDATE_COMMON_SIGNAL
            );
#endif

    for(bddset_t::iterator it = bdds.begin(); it != bdds.end(); it++) {
        const BDD& b = it->first;
        fnInfo_t& i = it->second;

        if(!i.isCanonical) continue;
        else {
            counts.push_back(count_t(i.covers.size(), b));
            aggr::groupByCommonSignalBDD(
                    this,
                    b,
                    &enumerator,
                    &creator
                    );

#if 0
            aggr::groupByCommonSignalBDD(
                    this,
                    b,
                    &enumerator4,
                    &creator4
                    );
#endif
        }
    }
    std::sort(counts.begin(), counts.end(), int_BDD_pair_compare);
#if 0
    int c = 0;
    for(int i = counts.size()-1; i >= 0 && c <= 50; i--, c++) {
        std::cout << std::setw(8) << counts[i].first << ": ";
        printBDD(stdout, counts[i].second);
    }
#endif
}

void flat_module_t::conflictAnalysis()
{
    for(unsigned i=0; i != gates.size(); i++) {
        if(gates[i]->count_undominated_modules() > 1) {
            gates[i]->dump_modules(std::cout);
            gates[i]->dump(std::cout);
        }
    }
}

void flat_module_t::adjacencyAnalysis()
{
    std::map<std::string, int> counts;

    for(unsigned i=0; i != modules.size(); i++) {
        aggr::id_module_t* mi = modules[i];
        std::string str(mi->get_type());

        moduleset_t m;
        mi->get_input_modules(m);
        for(moduleset_t::iterator it = m.begin(); it != m.end(); it++) {
            aggr::id_module_t* mj = *it;
            std::string edge_str = str + "<-" + mj->get_type();
            counts[edge_str] += 1;
        }
    }

    std::map<std::string, int>::iterator it;
    for(it = counts.begin(); it != counts.end(); it++) {
        std::cout << std::setw(20) << it->first << " : " << it->second << std::endl;
    }
}

bool flat_module_t::check_mergeables(std::vector<mergeable_check_t>& checks, 
        const aggr::id_module_t* m1, 
        const aggr::id_module_t* m2,
        bool knownMergesOnly)
{
    if(!knownMergesOnly) {
        return !m1->is_seq() && !m2->is_seq();
    }
    for(unsigned i=0; i != checks.size(); i++) {
        if(checks[i].check(m1, m2)) return true;
    }
    return false;
}

namespace {
    struct output_group_checker_t {
        const aggr::id_module_t* module;
        const std::string& group;
        bool& valid;

        output_group_checker_t(const std::string& grp, const aggr::id_module_t* m, bool& val) 
            : module(m)
              , group(grp)
              , valid(val) 
        {
        }

        void operator() (node_t* n) {
            if(!module->is_input_in_group(group, n)) {
                valid = false;
            }
        }
    };
}

bool flat_module_t::mergeable_check_t::check_input_groups(
        const aggr::id_module_t* m1, 
        const aggr::id_module_t* m2) const
{
    bool valid = true;
    output_group_checker_t outchecker(input_group, m2, valid);
    m1->apply_on_all_outputs(outchecker);
    return valid;
}

void flat_module_t::mergeMuxes(bool kill_mods)
{
    using namespace aggr;

    mergeable_check_t mux2mux("mux", "mux", true, "");
    mergeable_check_t dec2mux("dec", "mux", false, "sel");
    mergeable_check_t demux2mux("demux", "mux", false, "sel");

    std::vector<mergeable_check_t> checks;
    checks.push_back(mux2mux);
    checks.push_back(dec2mux);
    checks.push_back(demux2mux);

    modulelist_array_t input_modules(max_index());
    modulelist_t merge_array(modules.size(), NULL);

    for(unsigned i=0; i != modules.size(); i++) {
        modules[i]->add_all_inputs(input_modules);
    }
    for(unsigned i=0; i != input_modules.size(); i++) {
        std::sort(input_modules[i].begin(), input_modules[i].end());
    }

    renumberModules();
    std::vector< moduleset_t > edges(modules.size());
    std::vector< moduleset_t > edges2(modules.size());

    for(unsigned i=0; i != modules.size(); i++) {
        assert((int) i == modules[i]->moduleNum());

        moduleset_t compats;
        modules[i]->compute_compatible_modules(input_modules, compats);

        for(moduleset_t::iterator it = compats.begin(); it != compats.end(); it++) {
            id_module_t* other = *it;
            if(other != modules[i] && check_mergeables(checks, modules[i], other, options.knownMergesOnly)) {
                edges[i].insert(other);
                assert(other->moduleNum() >= 0 && other->moduleNum() < (int) modules.size());
                edges[other->moduleNum()].insert(modules[i]);
            }
            if(!options.knownMergesOnly) {
                if(other != modules[i] && check_mergeables(checks, modules[i], other, true)) {
                    edges2[i].insert(other);
                    assert(other->moduleNum() >= 0 && other->moduleNum() < (int) modules.size());
                    edges2[other->moduleNum()].insert(modules[i]);
                }
            }
        }
    }

    create_merged_modules(edges, edges2, kill_mods);
    if(kill_mods) {
        cleanup_modules();
        renumberModules();
    }
}

void flat_module_t::create_merged_modules(std::vector< moduleset_t >& edges, std::vector< moduleset_t >& edges2, bool kill_mods)
{
    using namespace aggr;

    int merge = 0;

    std::vector<int> groups(modules.size(), -1);
    for(unsigned i=0; i != modules.size(); i++) {
        module_group_mark(groups, edges, i, i);
    }

    std::vector<int> groups2(modules.size(), -1);
    for(unsigned i=0; i != modules.size(); i++) {
        module_group_mark(groups2, edges2, i, i);
    }

    modulelist_t new_modules;
    std::vector< modulelist_t > module_groups(modules.size());
    std::vector< modulelist_t > module_groups2(modules.size());
    for(unsigned i=0; i != modules.size(); i++) {
        int grp = groups[modules[i]->moduleNum()];
        module_groups[grp].push_back(modules[i]);

        int grp2 = groups2[modules[i]->moduleNum()];
        module_groups2[grp2].push_back(modules[i]);
    }

    for(unsigned i=0; i != module_groups.size(); i++) {
        if(module_groups[i].size() > 1) {
            std::string name = (options.knownMergesOnly) ? "mux_merged" : "merged_module";
            id_module_t* newmod = merge_modulelist(name, module_groups[i], kill_mods);
            modules.push_back(newmod);
            merge += 1;
        }
        if(module_groups2[i].size() > 1) {
            std::string name("mux_merged");

            id_module_t* newmod = merge_modulelist(name, module_groups2[i], kill_mods);
            modules.push_back(newmod);
        }
    }

    std::cout << "created " << merge << " merged modules." << std::endl;
}
void flat_module_t::module_group_mark(std::vector<int>& groups, std::vector< moduleset_t >& edges, int i, int mark)
{
    assert(groups.size() == edges.size());
    assert(edges.size() == modules.size());
    assert(i >= 0 && i < (int) modules.size());
    assert(mark >= 0);

    if(groups[i] != -1) return;
    groups[i] = mark;
    const moduleset_t& ms = edges[i];
    for(moduleset_t::const_iterator it = ms.begin(); it != ms.end(); it++) {
        aggr::id_module_t* mod = *it;
        module_group_mark(groups, edges, mod->moduleNum(), mark);
    }
}

void flat_module_t::compute_repr(modulelist_t& links, std::vector< modulelist_t >& groups)
{
    assert(links.size() == modules.size());
    // initialize the union-find data structure with self-pointers.
    for(unsigned i=0; i != links.size(); i++) {
        if(links[i] == NULL) links[i] = modules[i];
    }
    // now compute the representative for each link.
    for(unsigned i=0; i != links.size(); i++) {
        links[i] = get_repr(links, i);
    }
    // now create the "groups".
    assert(groups.size() == modules.size());
    for(unsigned i=0; i != links.size(); i++) {
        if(modules[i] != links[i]) {
            int index = links[i]->moduleNum();
            groups[index].push_back(modules[i]);
            if(groups[index].size() == 1) {
                groups[index].push_back(links[i]);
            }
        }
    }
}

aggr::id_module_t* flat_module_t::get_repr(modulelist_t& links, unsigned module_index)
{
    assert(module_index < links.size());
    aggr::id_module_t* mthis = modules[module_index];
    aggr::id_module_t* mnext = links[module_index];
    if(mthis == mnext) return mthis;
    else {
        links[module_index] = get_repr(links, mnext->moduleNum());
        return links[module_index];
    }
}


namespace {
    struct node_marker_t {
        // types
        enum flag_t { INVALID=0, INPUT=1, OUTPUT=2 } flag;
        typedef std::map<node_t*, flag_t> map_t;

        map_t& map;

        // cons.
        node_marker_t(map_t& m) : flag(INVALID), map(m) {}

        // func.
        void operator() (node_t* n) {
            assert(flag != INVALID);
            map_t::iterator pos;
            if((pos = map.find(n)) == map.end()) {
                map[n] = flag;
            } else {
                int new_flag = (int) flag | (int) pos->second;
                map[n] = (flag_t) new_flag;
            }
        }
    };
}

namespace {
    struct input_grouper_t {
        aggr::id_module_t* mod;
        input_grouper_t(aggr::id_module_t* m) : mod(m) {}
        void operator() (const std::string& group_name, node_t* n) {
            if(mod->is_input(n)) {
                mod->set_input_group(group_name, n, false);
            }
        }
    };
}

aggr::id_module_t* flat_module_t::merge_modulelist(const std::string& name, modulelist_t& mods, bool kill_mods)
{
    assert(mods.size() >= 2);

    std::ostringstream ostr;
    ostr << "merged: ";
    for(unsigned i=0; i != mods.size(); i++) {
        std::cout << mods[i]->get_type() << "_mx" << mods[i]->moduleNum() << " ";
        ostr << mods[i]->get_type() << "_mx" << mods[i]->moduleNum() << " ";
    }
    std::string comment(ostr.str());
    std::cout << std::endl;

    // mark inputs and outputs.
    node_marker_t::map_t map;
    node_marker_t node_marker(map);
    for(unsigned i=0; i != mods.size(); i++) {
        node_marker.flag = node_marker_t::OUTPUT;
        mods[i]->apply_on_all_outputs(node_marker);

        node_marker.flag = node_marker_t::INPUT;
        mods[i]->apply_on_all_inputs(node_marker);
    }

    using namespace aggr;
    id_module_t* mod = new id_module_t(name.c_str(), id_module_t::SLICEABLE, id_module_t::INFERRED);

    // create inputs and outputs set.
    for(node_marker_t::map_t::iterator it = map.begin(); it != map.end(); it++) {
        if(it->second == node_marker_t::OUTPUT) {
            mod->add_output(it->first);
        }
        if(it->second == node_marker_t::INPUT) {
            mod->add_input(it->first);
        }
    }

    input_grouper_t ip_grouper(mod);
    for(unsigned i=0; i != mods.size(); i++) {
        mods[i]->apply_on_all_grouped_inputs(ip_grouper);
    }

    // compute internals.
    mod->compute_internals();
    mod->add_comment(comment);

    if(kill_mods) {
        for(unsigned i=0; i != mods.size(); i++) {
            mods[i]->set_bad();
        }
    }

    return mod;
}

void flat_module_t::fittingAnalysis()
{
    using namespace aggr;

    modulelist_array_t input_modules(max_index());
    for(unsigned i=0; i != modules.size(); i++) {
        modules[i]->add_all_inputs(input_modules);
    }
    for(unsigned i=0; i != input_modules.size(); i++) {
        std::sort(input_modules[i].begin(), input_modules[i].end());
    }

    std::cout << "-----------" << std::endl;
    std::cout << "FITTING MAP" << std::endl;
    std::cout << "-----------" << std::endl;
    std::map<std::string, int> fit_map;
    int extendable_cnt = 0;
    for(unsigned i=0; i != modules.size(); i++) {
        moduleset_t compats;
        modules[i]->compute_compatible_modules(input_modules, compats);

        if(modules[i]->can_structurally_extend()) {
            extendable_cnt += 1;
        }

        for(moduleset_t::iterator it = compats.begin(); it != compats.end(); it++) {
            using namespace std;
            string edge_str = string(modules[i]->get_type()) + "->" + string((*it)->get_type());
            fit_map[edge_str] += 1;
            std::cout << modules[i]->get_type() << "_mx" << modules[i]->moduleNum() << "->"
                << (*it)->get_type() << "_mx" << (*it)->moduleNum() << std::endl;
        }
    }
    std::cout << "extendable modules: " << extendable_cnt << std::endl;

    std::cout << "----------------" << std::endl;
    std::cout << "FITTING ANALYSIS" << std::endl;
    std::cout << "----------------" << std::endl;
    for(std::map<std::string, int>::iterator it = fit_map.begin(); it != fit_map.end(); it++) {
        std::cout << std::setw(60) << std::left << it->first << ":" << std::setw(6) << it->second << std::endl;
    }
}

void flat_module_t::intersect(moduleset_t& set, modulelist_t& list)
{
    moduleset_t::iterator it1 = set.begin();
    modulelist_t::iterator it2 = list.begin();
    while ( (it1 != set.end()) && (it2 != list.end()) ) {
        if (*it1 < *it2) {
            set.erase(it1++);
        } else if (*it2 < *it1) {
            ++it2;
        } else { // *it1 == *it2
            ++it1;
            ++it2;
        }
    }
    set.erase(it1, set.end());
}

void flat_module_t::create_product_relations()
{
    input_provider_t* e = get_ipp(2);
    BDD andBDD = e->inp(0) & e->inp(1);
    BDD nandBDD = !(e->inp(0) & e->inp(1));
    BDD orBDD = e->inp(0) | e->inp(1);
    BDD norBDD = !(e->inp(0) | e->inp(1));

    fnInfo_t* andI = getFunction(andBDD);
    if(andI) {
        create_product_relation(andI->canonicalPtr->covers, andRelation);
        search_products(andRelation);
        andRelation.clear();
    }
    fnInfo_t* nandI = getFunction(nandBDD);
    if(nandI) {
        create_product_relation(nandI->canonicalPtr->covers, nandRelation);
        search_products(nandRelation);
        nandRelation.clear();
    }
    fnInfo_t* orI = getFunction(orBDD);
    if(orI) {
        create_product_relation(orI->canonicalPtr->covers, orRelation);
        search_products(orRelation);
        orRelation.clear();
    }
    fnInfo_t* norI = getFunction(norBDD);
    if(norI) {
        create_product_relation(norI->canonicalPtr->covers, norRelation);
        search_products(norRelation);
        norRelation.clear();
    }
}

void flat_module_t::create_init_x(node_adj_list_t& relation, nodeset_t& x)
{
    x.clear();
    for(node_adj_list_t::iterator it =  relation.begin();
            it != relation.end();
            it++)
    {
        node_t* n = it->first;
        x.insert(n);
    }
}

void flat_module_t::create_init_y(node_adj_list_t& relation, node_t* x, nodeset_t& y)
{
    if(relation.find(x) == relation.end()) return;
    for(nodeset_t::iterator it = relation[x].begin(); it != relation[x].end(); it++) {
        node_t* n = *it;
        y.insert(n);
    }
}

bool flat_module_t::is_fully_connected(node_t* a, nodeset_t& bs, node_adj_list_t& relation)
{
    if(relation.find(a) == relation.end()) return false;
    for(nodeset_t::iterator it = bs.begin(); it != bs.end(); it++) {
        node_t* b = *it;
        if(relation[a].find(b) == relation[a].end()) return false;
    }
    return true;
}

bool flat_module_t::refine(nodeset_t& as, nodeset_t& bs, node_adj_list_t& relation)
{
    nodeset_t newa;
    unsigned sz = as.size();
    for(nodeset_t::iterator it = as.begin(); it != as.end(); it++) {
        node_t *a = *it;
        if(is_fully_connected(a, bs, relation)) {
            newa.insert(a);
        }
    }
    as.clear();
    std::copy(newa.begin(), newa.end(), std::inserter(as, as.end()));
    return as.size() != sz;
}

void flat_module_t::search_products(node_adj_list_t& relation)
{
    for(node_adj_list_t::iterator it = relation.begin(); it != relation.end(); it++) {
        node_t* n0 = it->first;
        nodeset_t xs;
        nodeset_t ys;

        create_init_x(relation, xs);
        create_init_y(relation, n0, ys);
        bool change = false;
        do {
            change = false;
            change = refine(xs, ys, relation) || change;
            change = refine(ys, xs, relation) || change;
        } while(change);
        if(xs.size() + ys.size() >= 16) {
            std::cout << "xs:" << xs << "; sz: " << xs.size() << std::endl;
            std::cout << "ys:" << ys << "; sz: " << ys.size() << std::endl;
        }
    }
}


void flat_module_t::create_product_relation(
        kcoverset_t& covers,
        node_adj_list_t& relation
        )
{
    for(kcoverset_t::iterator it = covers.begin(); it != covers.end(); it++) {
        kcover_t* kc = *it;
        assert(kc->numInputs() == 2);
        node_t* a = kc->at(0);
        node_t* b = kc->at(1);
        relation[a].insert(b);
        relation[b].insert(a);
    }
}

namespace xbar_n {

    group_t* new_group(node_t* gate, node_t* rep)
    {
        group_t* g = new group_t();
        g->gate = gate;
        g->rep_gate = rep;
        g->rank = 0;
        g->root = g;
        return g;
    }

    bool same_group(group_t* g1, group_t* g2)
    {
        node_t* n1 = g1->rep_gate;
        node_t* n2 = g2->rep_gate;
        for(node_t::input_iterator it = n1->inputs_begin(), jt = n2->inputs_begin();
                it != n1->inputs_end() && jt != n2->inputs_end();
                it++,jt++)
        {
            if(*it == *jt) {
                node_t* n = *it;
                if(n->num_fanouts() >= 16) {
                    return true;
                }
            }
        }
        return false;
    }

    void link(group_t* g1, group_t* g2)
    {
        if(g1->rank > g2->rank) {
            g2->root = g1;
        } else {
            g1->root = g2;
            if(g1->rank == g2->rank) {
                g2->rank += 1;
            }
        }
    }

    group_t* get_root(group_t* g)
    {
        if(g != g->root) {
            g->root = get_root(g->root);
        }
        return g->root;
    }
}

bool operator<(const nodeset_t& a, const nodeset_t& b)
{
    if(a.size() < b.size()) return true;
    else if(a.size() > b.size()) return false;
    else {
        nodeset_t::const_iterator ait= a.begin(), bit = b.begin();
        for(; ait != a.end() && bit != b.end(); ait++, bit++) {
            node_t* ai = *ait;
            node_t* bi = *bit;
            if(ai < bi) return true;
            else if(ai > bi) return false;
        }
    }
    return false;
}

void flat_module_t::readExternalModules(const std::string& filename)
{
    map_t imap;
    for(map_t::iterator it = map.begin(); it != map.end(); it++) {
        node_t* n = it->second;
        std::string iname = n->get_instance_name();
        if(iname.size()) {
            imap[iname] = n;
        }
    }

    std::ifstream in(filename.c_str());
    while(in) {
        std::string word;
        in >> word;
        if(word.size() == 0) continue;

        if(word == ".module") {
            std::string type;
            int numInternals;
            in >> type >> numInternals;
            nodeset_t internals;
            for(int i=0; i != numInternals; i++) {
                std::string internal;
                in >> internal;
                map_t::iterator pos = imap.find(internal);
                assert(pos != imap.end());
                node_t* n = pos->second;
                internals.insert(pos->second);
                if(n->is_latch()) {
                    internals.insert(n->get_input(0));
                }
                if(n->get_sibling()) {
                    internals.insert(n->get_sibling());
                }
                if(n->get_sibling_back()) {
                    internals.insert(n->get_sibling_back());
                }
            }
            aggr::id_module_t* m = create_id_module(type.c_str(), aggr::id_module_t::USER_DEFINED, internals);
            add_module(m);
        } else {
            assert(false);
        }
    }
}

