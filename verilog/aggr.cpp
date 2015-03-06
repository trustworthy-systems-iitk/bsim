#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <string.h>
#include <limits>

#include "cuddObj.hh"

#include "aggr.h"
#include "main.h"
#include "node.h"
#include "word.h"
#include "flat_module.h"
#include "utility.h"

#include <boost/lexical_cast.hpp>

namespace aggr {
    std::vector<BDD> decFns[MAX_DECODER_SIZE], decnegFns[MAX_DECODER_SIZE];

    int id_module_t::moduleCounter = 0;
    const std::string id_module_t::typeStrings[] =
    { 
        "USER_DEFINED",
        "INFERRED",
        "CANDIDATE_WORD_BOUND", 
        "CANDIDATE_MODULE_BOUND",
        "CANDIDATE_COMMON_SIGNAL",
        "CANDIDATE_CONTROL_SIGNAL",
        "CANDIDATE_COLORED"
    }; 

    id_module_t::~id_module_t() {
        free((void*)type);
        free(name);
        for(nodeset_t::iterator it = internals.begin(); it != internals.end(); it++) {
            node_t* n = *it;
            n->remove_module(this);
        }
        for(unsigned i=0; i != word_inputs.size(); i++) {
            word_inputs[i]->set_module(NULL);
        }
        for(unsigned i=0; i != word_outputs.size(); i++) {
            word_outputs[i]->set_module(NULL);
        }
    }

    void id_module_t::add_all_inputs(modulelist_array_t& inps)
    {
        for(nodelist_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
            node_t* n = *it;
            std::vector<id_module_t*>& nl = inps[n->get_index()];
            if(std::find(nl.begin(), nl.end(), this) == nl.end()) {
                nl.push_back(this);
            }
        }
        for(wordlist_t::iterator it = word_inputs.begin(); it != word_inputs.end(); it++) {
            word_t* w = *it;
            for(unsigned i=0; i != w->size(); i++) {
                node_t* n = w->get_bit(i);
                std::vector<id_module_t*>& nl = inps[n->get_index()];
                if(std::find(nl.begin(), nl.end(), this) == nl.end()) {
                    nl.push_back(this);
                }
            }
        }
    }

    void id_module_t::compute_compatible_modules(modulelist_array_t& inps, moduleset_t& compats)
    {
        assert(compats.size() == 0);
        bool first = true;
        for(nodelist_t::iterator it = outputs.begin(); it != outputs.end(); it++) {
            node_t* n = *it;
            std::vector<id_module_t*>& nl = inps[n->get_index()];
            if(first) {
                std::copy(nl.begin(), nl.end(), std::inserter(compats, compats.end()));
                first = false;
            } else {
                flat_module_t::intersect(compats, nl);
            }
            if(compats.size() == 0) return;
        }
        for(wordlist_t::iterator it = word_outputs.begin(); it != word_outputs.end(); it++) {
            word_t* w = *it;
            for(unsigned i=0; i != w->size(); i++) {
                node_t* n = w->get_bit(i);
                std::vector<id_module_t*>& nl = inps[n->get_index()];
                if(first) {
                    std::copy(nl.begin(), nl.end(), std::inserter(compats, compats.end()));
                    first = false;
                } else {
                    flat_module_t::intersect(compats, nl);
                }
            }
            if(compats.size() == 0) return;
        }
    }

    bool id_module_t::can_structurally_extend()
    {
        nodelist_t output_nodes;
        nodelist_t output_fanouts;

        for(nodelist_t::iterator it = outputs.begin(); it != outputs.end(); it++) {
            node_t* n = *it;
            if(n->num_fanouts() != 1) return false;
            output_nodes.push_back(n);
        }

        for(wordlist_t::iterator it = word_inputs.begin(); it != word_inputs.end(); it++) {
            word_t* w = *it;
            for(unsigned i=0; i != w->size(); i++) {
                node_t* n = w->get_bit(i);
                if(n->num_fanouts() != 1) return false;
                output_nodes.push_back(n);
            }
        }

        if(output_nodes.size() == 0) return false;

        for(unsigned i=0; i != output_nodes.size(); i++) {
            node_t* n = output_nodes[i];
            assert(n->num_fanouts() == 1);
            node_t* nf = *(n->fanouts_begin());
            if(nf->is_gate() && !nf->is_latch_gate() && !nf->is_covered()) {
                output_fanouts.push_back(nf);
            } else {
                return false;
            }
        }
        if(output_nodes.size() != output_fanouts.size()) return false;

        std::ostringstream ostr; 
        ostr << "can structurally extend: " << output_fanouts;
        std::string str(ostr.str());
        this->add_comment(str);

        return true;
    }

    void id_module_t::createBDDMap(fn_map_t& mapIn, bdd_node_map_t& mapOut)
    {
        for(nodelist_t::iterator it = inputs.begin(); it != 
                inputs.end(); it++) {
            node_t* n = *it;
            assert(mapIn.find(n) != mapIn.end());
            BDD v = mapIn[n];
            assert(mapOut.find(v) == mapOut.end() || 
                    (mapOut.find(v)->second == n));
            mapOut[v] = n;
        }
        for(wordlist_t::iterator it = word_inputs.begin(); it != 
                word_inputs.end(); it++)
        {
            word_t* w = *it;
            for(nodelist_t::iterator jt = w->begin(); jt != w->end(); 
                    jt++) {
                node_t* n = *jt;
                assert(mapIn.find(n) != mapIn.end());
                BDD v = mapIn[n];
                assert(mapOut.find(v) == mapOut.end() || 
                        (mapOut.find(v)->second == n));
                mapOut[v] = n;
            }
        }
    }

        
    void id_module_t::add_slice(bitslice_t* bt)
    {
        for(unsigned i=0; i != bt->ys.size(); i++) {
            add_output(bt->ys[i]);
        }
        for(unsigned i=0; i != bt->xs.size(); i++) {
            add_input(bt->xs[i]);
        }
    }

    void id_module_t::add_input(node_t* i)
    {
        if(std::find(inputs.begin(), inputs.end(), i) == inputs.end()) {
            inputs.push_back(i);
        }
    }

    void id_module_t::add_input(const std::string& name, node_t* n, bool allow_dup) {
        if(std::find(inputs.begin(), inputs.end(), n) == inputs.end()) {
            inputs.push_back(n);
        }
        set_input_group(name, n, allow_dup);
    }

    void id_module_t::set_input_group(const std::string& name, node_t* n, bool allow_dup) 
    {
        if(allow_dup || 
           std::find( node_groups[name].begin(), node_groups[name].end(), n) == node_groups[name].end()) 
        {
            node_groups[name].push_back(n);
        }
    }

    bool id_module_t::is_input_in_group(const std::string& group, node_t* n) const
    {
        node_groups_t::const_iterator pos = node_groups.find(group);
        if(pos == node_groups.end()) return false;
        else {
            const nodelist_t& nl = pos->second;
            return (std::find(nl.begin(), nl.end(), n) != nl.end());
        }
    }

    void id_module_t::add_input_word(word_t* w)
    {
        assert(std::find(word_inputs.begin(), word_inputs.end(), w) == word_inputs.end());
        assert(w->get_index() != -1);
        word_inputs.push_back(w);
        w->set_module(this);
        if(!candidate()) {
            w->set_input_word();
        }
    }
    void id_module_t::add_input_module(id_module_t* m)
    {
        module_inputs.insert(m);
    }
    void id_module_t::add_output_word(word_t* w)
    {
        assert(std::find(word_outputs.begin(), word_outputs.end(), w) == word_outputs.end());
        word_outputs.push_back(w);
        w->set_module(this);
        if(!candidate()) {
            w->set_output_word();
        }
    }

    void id_module_t::add_output(node_t* o, fnInfo_t* inf)
    {
        if(std::find(outputs.begin(), outputs.end(), o) == outputs.end()) {
            outputs.push_back(o);
        }
    }

    void id_module_t::link(id_module_t* a, id_module_t* b)
    {
        if(a->level > b->level) {
            b->repr = a;
        } else if(b->level > a->level) {
            a->repr = b;
        } else {
            a->repr = b;
            b->level += 1;
        }
    }

    id_module_t* id_module_t::get_repr()
    {
        if(repr != this) {
            repr = repr->get_repr();
        }
        return repr;
    }

    bool id_module_t::compare(id_module_t* left, id_module_t* right)
    {
        const char* ltype = left->get_type();
        const char* rtype = right->get_type();

        assert(strcmp(ltype, rtype) == 0);
        const char* t=ltype;
        if(strcmp(t, "and2gate") == 0   || strcmp(t, "or2gate") == 0    ||
           strcmp(t, "nand2gate") == 0  || strcmp(t, "nor2gate") == 0   ||
           strcmp(t, "xnor2gate") == 0  || strcmp(t, "xor2gate") == 0) 
        {
            assert(left->num_outputs() == 0);
            assert(left->word_outputs.size() == 1);

            unsigned int left_outsize = left->word_outputs[0]->size();
            unsigned int right_outsize = right->word_outputs[0]->size();

            if(left->num_inputs() == right->num_inputs() && 
               left_outsize == right_outsize &&
               left->num_inputs() == left_outsize + 1) 
            {
                return true; 
            } else { 
                return false; 
            }
        } else if(strcmp(t, "andtree") == 0 || strcmp(t, "ortree") == 0) {
            if(left->num_inputs() == right->num_inputs()) {
                assert(left->num_outputs() == 1);
                assert(right->num_outputs() == 1);
                return true;
            } else {
                return false;
            }
        }

        return false;
    }

    bool id_module_t::needs_grouping() const
    {
        if(!options.computeRepresentativeModules) return false;
        return pin_map.size() > 0;
    }

    std::string id_module_t::get_input_port_name(int word, int bit)
    {
        const char* t = get_type();
        if(strcmp(t, "and2gate") == 0   || strcmp(t, "or2gate") == 0    ||
           strcmp(t, "nand2gate") == 0  || strcmp(t, "nor2gate") == 0   ||
           strcmp(t, "xnor2gate") == 0  || strcmp(t, "xor2gate") == 0) 
        {
            assert(word == -1);
            assert(bit < (int) num_inputs());
            if(bit == 0) {
                return std::string("A");
            } else {
                return std::string("B") + boost::lexical_cast<std::string>(bit-1);
            }
        } else if(strcmp(t, "andtree") == 0 || strcmp(t, "ortree") == 0) {
            assert(word == -1);
            assert(bit < (int) num_inputs());
            return std::string("X") + boost::lexical_cast<std::string>(bit);
        }
        // shouldn't get here.
        assert(false);
    }

    std::string id_module_t::get_output_port_name(int word, int bit)
    {
        const char* t = get_type();
        if(strcmp(t, "and2gate") == 0   || strcmp(t, "or2gate") == 0    ||
           strcmp(t, "nand2gate") == 0  || strcmp(t, "nor2gate") == 0   ||
           strcmp(t, "xnor2gate") == 0  || strcmp(t, "xor2gate") == 0) 
        {
            assert(word == 0);
            return std::string("Y") + boost::lexical_cast<std::string>(bit);
        } else if(strcmp(t, "andtree") == 0 || strcmp(t, "ortree") == 0) {
            assert(word == -1);
            assert(bit == 0);
            return std::string("Y");
        }
        // shouldn't get here.
        assert(false);
    }

    void id_module_t::compute_internals_private()
    {
        //if(strcmp(get_type(), "ripple_addsub") == 0) { std::cout << "really initial internals: " << internals << std::endl; }
        nodeset_t outnodes;
        for(nodelist_t::iterator it = outputs.begin(); it != outputs.end(); it++) {
            node_t* o = *it;
            outnodes.insert(o);
        }
        for(wordlist_t::iterator it = word_outputs.begin(); it != word_outputs.end(); it++) {
            word_t* w = *it;
            for(word_t::iterator it = w->begin(); it != w->end(); it++) {
                node_t* n = *it;
                outnodes.insert(n);
            }
        }

        //if(strcmp(get_type(), "ripple_addsub") == 0) { std::cout << "initial internals: " << internals << std::endl; }
        for(nodeset_t::iterator it = outnodes.begin(); it != outnodes.end(); it++) {
            node_t* o = *it;
            internals.insert(o);
            for(node_t::input_iterator it = o->inputs_begin(); 
                                       it != o->inputs_end();
                                       it++)
            {
                node_t* n = *it;
                compute_internals(n);
                //if(strcmp(get_type(), "ripple_addsub") == 0) { std::cout << "internals after: " << n->get_name() << " : " << internals << std::endl; }
            }
        }
        //if(strcmp(get_type(), "ripple_addsub") == 0) { std::cout << "final internals: " << internals << std::endl; }
    }

    void id_module_t::compute_internals(bool update)
    {
        compute_internals_private();
        if(update) {
            update_nodes();
        }
        mark_bad();
    }

    void id_module_t::compute_internals2()
    {
        compute_internals_private();
        update_nodes();
        add_sibling_gates();
        mark_bad();
    }

    void id_module_t::mark_bad()
    {
        // no module must have the "latch gates" at the outputs either include the
        // whole latch or leave out the gate itself.
        for(nodelist_t::iterator it = outputs.begin(); it != outputs.end(); it++) {
            node_t *n = *it;
            if(n->is_latch_gate()) {
                is_bad = true;
                std::cout << "mark bad due to output1: " << n->get_name() << std::endl;
                break;
            }
        }
        for(wordlist_t::iterator it = word_outputs.begin(); it != word_outputs.end(); it++) {
            if(is_bad) break;
            word_t* w = *it;
            for(word_t::iterator jt = w->begin(); jt != w->end(); jt++) {
                node_t* n = *jt;
                if(n->is_latch_gate()) {
                    is_bad = true;
                    std::cout << "mark bad due to output2: " << n->get_name() << std::endl;
                    break;
                }
            }
        }
        if(is_bad) return;

        for(nodeset_t::iterator it = internals.begin();
                                it != internals.end();
                                it++)
        {
            node_t* n = *it;
            node_t* s = n->get_sibling_back();
            if(s != NULL && internals.find(s) == internals.end()) {
                 std::string str("trouble is coming to this module.");
                // add_comment(str);
                 std::cout << get_name() << " : " << "mark bad due to internal1: " << n->get_name() 
                           << "; type: " << get_type() << std::endl;
                is_bad = true;
                return;
            }
            s = n->get_sibling();
            if(s != NULL && internals.find(s) == internals.end()) {
                 std::cout << get_name() << " : " << "mark bad due to internal2: " << n->get_name() 
                           << "; s: " << s->get_name() << "; type: " << get_type() << std::endl;
                is_bad = true;
                return;
            }
        }


        if(!candidateWordBound()) return;

        for(nodeset_t::iterator it =  internals.begin();
                                it != internals.end();
                                it++)
        {
            node_t* n = *it;
            if(n->is_latch()) is_bad = true;
        }
    }

    void id_module_t::add_comment(std::string& c) { 
        if(c.size() > 120) {
            std::vector<std::string> lines;
            split_lines(lines, c, 120);
            for(unsigned i=0; i != lines.size(); i++) {
                comments.push_back(lines[i]);
            }
        } else {
            comments.push_back(c); 
        }
    }

    void id_module_t::add_sibling_gates()
    {
        added_siblings = true;
        for(nodeset_t::iterator it = internals.begin();
                                it != internals.end();
                                it++)
        {
            node_t* n = *it;

            if(!n->is_gate()) continue;
            if(n->is_latch_gate()) continue;

            node_t* s = n->get_sibling_back();
            if(s != NULL && internals.find(s) == internals.end()) {
                internals.insert(s);
                continue;
            }
            s = n->get_sibling();
            if(s != NULL && internals.find(s) == internals.end()) {
                internals.insert(s);
                continue;
            }
        }
    }

    void id_module_t::compute_inputmap(node_t* n)
    {   
        if(inputmap.find(n) != inputmap.end()) return;
        else if(is_input(n)) {
            inputmap[n].insert(n);
        } else {
            inputmap[n] = nodeset_t();
            for(node_t::input_iterator it = n->inputs_begin();
                                       it != n->inputs_end();
                                       it++)
            {
                node_t* i = *it;
                compute_inputmap(i);
                for(nodeset_t::iterator jt = inputmap[i].begin(); jt != inputmap[i].end(); jt++) {
                    node_t* ii = *jt;
                    inputmap[n].insert(ii);
                }
            }
        }
    }

    const nodeset_t& id_module_t::get_inputmap(node_t* n) 
    {
        compute_inputmap(n);
        return inputmap[n];
    }

    int id_module_t::getNumSlices()
    {
        // std::cout << "getNumSlices [" << moduleNum() << "] = ";
        if(sliceable == UNSLICEABLE) {
            // std::cout << "unsliceable" << std::endl;
            return 1;
        } else if(sliceable == SLICEABLE) {
            // std::cout << numSlices << " (already sliced); sliceable=" << sliceable << std::endl;
            return numSlices;
        } else {
            assert(sliceable == NEEDS_SLICE_CHECK);
            int slices =  computeNumSlices();
            // std::cout << slices << " (sliced now); sliceable=" << sliceable << std::endl;
            return slices;
        }
    }

    bool id_module_t::checkOutputBit(node_t* n, unsigned& lastSz)
    {
        compute_inputmap(n);
        unsigned sz = inputmap[n].size();
        if(lastSz == UINT_MAX) {
            lastSz = sz;
        } else {
            if(sz != lastSz) {
                return false;
            }
        }
        return true;
    }

    int id_module_t::getILPVar(node_t* n) {
        if(sliceable == SLICEABLE) {
            slice_index_map_t::iterator pos = slice_index_map.find(n);
            if(pos == slice_index_map.end()) {
                std::cout << "node: " << n->get_name() << std::endl;
                dumpSliceIndexMap();
                dump_structural_verilog(type, std::cout, std::cout);
            }
            assert(pos != slice_index_map.end());
            int var = pos->second + getILPStartVar();
            assert( var >= getILPStartVar() && var <= getILPEndVar() );
            return var;
        } else {
            assert(sliceable == UNSLICEABLE);
            assert(getILPStartVar() == getILPEndVar());
            return getILPStartVar();
        }
    }

    void id_module_t::updateSliceIndex(node_t* n, int index)
    {
        nodeset_t outputInternalSet;
        compute_internals(n, outputInternalSet);
        for(nodeset_t::iterator it = outputInternalSet.begin(); it != outputInternalSet.end(); it++) {
            node_t* g = *it;
            slice_index_map_t::iterator pos = slice_index_map.find(g);
            if(pos == slice_index_map.end()) {
                slice_index_map[g] = index;
            } else {
                assert(pos->second != index);
                slice_index_map[g] = 0;
            }
        }
    }

    void id_module_t::dumpSliceIndexMap()
    {
        std::ostringstream out;
        for(slice_index_map_t::iterator it = slice_index_map.begin(); it != slice_index_map.end(); it++) {
            out << it->first->get_name() << ":" << it->second << "; ";
        }
        std::string str(out.str());
        add_comment(str);
    }

    void id_module_t::pruneSlices(flat_module_t* flat, const std::vector<int>& good_slices)
    {
        assert(!is_bad);

#ifdef DEBUG_OVERLAP
        std::ostringstream out;
        for(unsigned i=0; i != good_slices.size(); i++) { out << good_slices[i] << " "; }
        out << "; original outs: " << outputs;
        std::string comment("pruned module; good slices: " + out.str());
        add_comment(comment);
#endif

        // output pruning.
        std::vector<node_t*> new_outputs;
        if(outputs.size() != 0) {
            assert(word_outputs.size() == 0);
            assert(good_slices.size() != outputs.size());

            for(unsigned i=0; i != good_slices.size(); i++) {
                int index = good_slices[i]-1;
                assert(index >= 0 && index < (int)outputs.size());
                new_outputs.push_back(outputs[index]);
            }
            assert(new_outputs.size() == good_slices.size());
            outputs.resize(new_outputs.size());
            std::copy(new_outputs.begin(), new_outputs.end(), outputs.begin());
        } else {
            assert(word_outputs.size() == 1);
            word_t* nw = new word_t(false, word_outputs[0]->getSrc());
            for(unsigned i=0; i != good_slices.size(); i++) {
                int index=good_slices[i]-1;
                assert(index >= 0 && index < (int)word_outputs[0]->size());
                nw->add_bit(word_outputs[0]->get_bit(index), -1);
            }
            flat->add_word(nw);
            word_outputs[0] = nw;
            word_outputs[0]->set_module(this);
        }

        // input pruning.
        nodeset_t good_inputs;
        compute_good_inputs(good_inputs);
        prune_inputs(good_inputs);

        clear_internals();
        compute_internals_private();
        update_nodes();
        if(added_siblings) {
            add_sibling_gates();
        }
        mark_bad();
        assert(!is_bad);

    }

    void id_module_t::compute_good_inputs(nodeset_t& good_inputs)
    {
        for(unsigned i=0; i != outputs.size(); i++) {
            node_t* o = outputs[i]; 
            compute_inputmap(o);
            std::copy(inputmap[o].begin(), inputmap[o].end(), 
                      std::inserter(good_inputs, good_inputs.end()));
        }
        for(unsigned i=0; i != word_outputs.size(); i++) {
            for(unsigned j=0; j != word_outputs[i]->size(); j++) {
                node_t* o = word_outputs[i]->get_bit(j); 
                compute_inputmap(o);
                std::copy(inputmap[o].begin(), inputmap[o].end(), 
                          std::inserter(good_inputs, good_inputs.end()));
            }
        }
        reset_inputmaps();
    }

    void id_module_t::prune_inputs(const nodeset_t& good_inputs) 
    {
        nodelist_t new_inputs;
        for(unsigned i=0; i != inputs.size(); i++) {
            if(good_inputs.find(inputs[i]) != good_inputs.end()) {
                new_inputs.push_back(inputs[i]);
            }
        }
        inputs.resize(new_inputs.size());
        std::copy(new_inputs.begin(), new_inputs.end(), inputs.begin());

        for(unsigned i=0; i != word_inputs.size(); i++) {
            word_inputs[i] = prune_word(good_inputs, word_inputs[i]);
        }
    }

    word_t* id_module_t::prune_word(const nodeset_t& good_inputs, word_t* w)
    {
        word_t* nw = new word_t(false, w->getSrc());
        for(unsigned i=0; i != w->size(); i++) {
            node_t* n = w->get_bit(i);
            if(good_inputs.find(n) != good_inputs.end()) {
                nw->add_bit(n, -1);
            }
        }
        if(w->size() == nw->size()) {
            delete nw;
            return w;
        } else {
            nw->set_module(this);
            return nw;
        }
    }

    void id_module_t::updateSliceSizes()
    {
        assert(sliceable == SLICEABLE);
        slice_sizes.resize(numSlices+1);
        for(slice_index_map_t::iterator it = slice_index_map.begin(); it != slice_index_map.end(); it++) {
            slice_sizes[it->second] += 1;
        }
    }

    int id_module_t::computeNumSlices()
    {
        assert(sliceable == NEEDS_SLICE_CHECK);
        assert((outputs.size() == 0 && word_outputs.size() == 1) || 
               (outputs.size() > 0 && word_outputs.size() == 0) );

        nodelist_t outs;
        bool good = true;
        if(outputs.size() > 0) {
            numSlices = outputs.size();
            unsigned lastSz = UINT_MAX;
            for(unsigned i=0; i != outputs.size(); i++) {
                node_t* n = outputs[i];
                if(!checkOutputBit(n, lastSz)) {
                    good = false;
                    break;
                }
            }
        } else {
            numSlices = word_outputs[0]->size();
            unsigned lastSz = UINT_MAX;
            for(unsigned i=0; i != word_outputs[0]->size(); i++) {
                node_t* n = word_outputs[0]->get_bit(i);
                if(!checkOutputBit(n, lastSz)) {
                    good = false;
                    break;
                }
            }
        }
        reset_inputmaps();
        if(good && numSlices > 1) {
            sliceable = SLICEABLE;
            if(outputs.size() > 0) {
                for(unsigned i=0; i != outputs.size(); i++) {
                    updateSliceIndex(outputs[i], i+1);
                }
            } else {
                for(unsigned i=0; i != word_outputs[0]->size(); i++) {
                    updateSliceIndex(word_outputs[0]->get_bit(i), i+1);
                }
            }
            updateSliceSizes();
#ifdef DEBUG_OVERLAP
            dumpSliceIndexMap();
#endif
        } else {
            sliceable = UNSLICEABLE;
            numSlices = 1;
        }
        return numSlices;
    }



    void id_module_t::compute_internals(node_t* o)
    {
        compute_internals(o, internals);
    }

    // This version adds the internals to the set that is passed in.
    void id_module_t::compute_internals(node_t* o, nodeset_t& internalset)
    {
        if(is_input(o) || internalset.find(o) != internalset.end()) { return; }

        internalset.insert(o);
        for(node_t::input_iterator it = o->inputs_begin(); 
                                   it != o->inputs_end();
                                   it++)
        {
            node_t* n = *it;
            compute_internals(n, internalset);
        }
    }


void id_module_t::find_inputs(node_t* n, const nodeset_t& cone, nodeset_t& visited, nodeset_t& inputs)
{
    if(visited.find(n) != visited.end()) return;
    if(cone.find(n) != cone.end()) {
        inputs.insert(n);
    } else {
        visited.insert(n);
        for(node_t::input_iterator it = n->inputs_begin(); it != n->inputs_end(); it++) {
            node_t* inp = *it;
            find_inputs(inp, cone, visited, inputs);
        }
    }
}


    void id_module_t::update_nodes()
    {
        for(nodeset_t::iterator it = internals.begin(); it != internals.end(); it++) {
            node_t* n = *it;
            n->add_module(this);
        }
    }

    void id_module_t::clear_internals()
    {
        for(nodeset_t::iterator it = internals.begin(); it != internals.end(); it++) {
            node_t* n = *it;
            n->remove_module(this);
        }
        internals.clear();
    }

    bool id_module_t::is_input(node_t* n) const
    {
        if(std::find(inputs.begin(), inputs.end(), n) != inputs.end()) return true;
        else return is_word_input(n);
    }

    int id_module_t::get_word_input_index(node_t* n) const
    {
        for(wordlist_t::const_iterator it = word_inputs.begin();
                it != word_inputs.end();
                it++)
        {
            int idx;
            if((idx = (*it)->getBitIndex(n)) != -1) return idx;
        }
        return -1;
    }

    unsigned id_module_t::count_noninferred_gates() const
    {
        if(non_inferred_gates != -1) return non_inferred_gates;

        unsigned sum=0;
        for(nodeset_t::iterator it =  internals.begin();
                                it != internals.end();
                                it++)
        {
            node_t* n = *it;
            sum = sum + (n->is_noninferred() ? 1 : 0);
        }
        non_inferred_gates =  sum;
        return non_inferred_gates;
    }


    bool id_module_t::is_output(node_t* n) const
    {
        if(std::find(outputs.begin(), outputs.end(), n) != outputs.end()) return true;
        else {
            for(wordlist_t::const_iterator it = word_outputs.begin();
                                           it != word_outputs.end();
                                           it++)
            {
                if((*it)->isBitPresent(n)) return true;
            }
        }
        return false;
    }

    bool id_module_t::contains(const id_module_t* other) const
    {
        if(other->internals.size() > internals.size()) return false;
        for(nodeset_t::iterator it = other->internals.begin(); it != other->internals.end(); it++) {
            node_t* n = *it;
            if(internals.find(n) == internals.end()) return false;
        }
        return true;
    }

    void id_module_t::mark_internals(std::vector<bool>& marks) const
    {
        for(nodeset_t::const_iterator it = internals.begin(); it != internals.end(); it++) {
            node_t* n = *it;
            marks[n->get_index()] = true;
        }
    }

    void id_module_t::mark_inputs(std::vector<bool>& marks) const
    {
        for(nodelist_t::const_iterator it = inputs.begin(); it != inputs.end(); it++) {
            node_t* n = *it;
            marks[n->get_index()] = true;
        }
        for(wordlist_t::const_iterator it = word_inputs.begin(); it != word_inputs.end(); it++) {
            word_t *w = *it;
            for(unsigned j=0; j != w->size(); j++) {
                node_t* n = w->get_bit(j);
                marks[n->get_index()] = true;
            }
        }
    }

    void id_module_t::dump_verilog(std::ostream& out, std::ostream& lib, verilog_lib_t* vlib)
    {
#ifdef DUMP_MUX_BEHAVIORAL
        if(strcmp(type, "mux21") == 0) {
            dump_mux21(out, "mux21", vlib);
        } else if(strcmp(type, "mux21i") == 0) {
            dump_mux21(out, "mux21i", vlib);
        } else if(strcmp(type, "mux41") == 0) {
            dump_mux41(out, "mux41", vlib);
        } else if(strcmp(type, "mux41i") == 0) {
            dump_mux41(out, "mux41i", vlib);
        } else {
            dump_structural_verilog(type, out, lib);
        }
#else
        dump_structural_verilog(type, out, lib);
#endif
    }

    void id_module_t::create_varname_set(stringset_t& names)
    {
        names.clear();

        if(!needs_grouping()) {
            for(unsigned i=0; i != inputs.size(); i++) {
                node_t* n = inputs[i];
                names.insert(n->get_name());
            }
            for(unsigned i=0; i != outputs.size(); i++) {
                node_t* n = outputs[i];
                names.insert(n->get_name());
            }
            for(unsigned i=0; i != word_inputs.size(); i++) {
                word_t* w = word_inputs[i];
                for(unsigned j=0; j != w->size(); j++) {
                    node_t* n = w->get_bit(j);
                    names.insert(n->get_name());
                }
            }
            for(unsigned i=0; i != word_outputs.size(); i++) {
                word_t* w = word_outputs[i];
                for(unsigned j=0; j != w->size(); j++) {
                    node_t* n = w->get_bit(j);
                    names.insert(n->get_name());
                }
            }
            for(nodeset_t::iterator it = internals.begin(); it != internals.end(); it++) {
                node_t* n = *it;
                names.insert(n->get_name());
            }
        }
    }

    std::string rename_var(
        const std::string& old, 
        stringset_t& names1, 
        stringset_t& names2
    )
    {
        std::string name(old);
        if(varname_has_indices(old)) {
            // replace '[' and ']' with '_'
            for(unsigned i=0; i != name.size(); i++) {
                if(name[i] == '[' || name[i] == ']') {
                    name[i] = '_';
                }
            }
            // now uniquify.
            while( (names1.find(name) != names1.end()) ||
                   (names2.find(name) != names2.end()))
            {
                name = name + "_";
            }
        }
        // return.
        return name;
    }

    void uniquify(std::string& name, stringset_t& names1, stringset_t& names2)
    {
        while(names1.find(name) != names1.end() ||
              names2.find(name) != names2.end())
        {
            name += "_";
        }
    }

    void renamer_t::operator() (node_t* n) {
        id_module_t::pin_map_t::iterator pos;
        assert((pos = mod->pin_map.find(n)) != mod->pin_map.end());
        map[n->get_name()] = pos->second;
        rev[pos->second] = n->get_name();
    }

    void id_module_t::rename_inputs(stringset_t& names, stringset_t& new_names,
                                    stringmap_t& map, stringmap_t& rev)
    {
        renamer_t renamer(this, map, rev);
        apply_on_all_inputs(renamer);
    }

    void id_module_t::rename_outputs(stringset_t& names, stringset_t& new_names,
                                     stringmap_t& map, stringmap_t& rev)
    {
        renamer_t renamer(this, map, rev);
        apply_on_all_outputs(renamer);
    }

    void id_module_t::create_rename_map( stringset_t& names, stringmap_t& map, stringmap_t& rev )
    {
        using namespace std;
        stringset_t new_names;
        for(stringset_t::iterator it = names.begin(); it != names.end(); it++) {
            const string& o = *it;
            const string& n = rename_var(o, names, new_names);
            if(n != o) {
                map[o] = n;
                rev[n] = o;
                new_names.insert(n);
            }
        }
        if(needs_grouping()) {
            rename_inputs(names, new_names, map, rev);
            rename_outputs(names, new_names, map, rev);
        }
    }

    bool varname_has_indices(const std::string& s)
    {
        for(unsigned i=0; i != s.size(); i++) {
            if(s[i] == '[' || s[i] == ']') {
                assert(s[i] == '[');
                return true;
            }
        }
        return false;
    }

    void id_module_t::add_decls(decl_list_t& decls)
    {
        stringset_t names;
        if(options.renameWiresInModule) {
            create_varname_set(names);
        }

        create_rename_map(names, rename_map, rev_rename_map);

        for(unsigned i=0; i != inputs.size(); i++) {
            node_t* n = inputs[i];
            const char* name = get_renaming(n->get_name()).c_str();
            decls.add_input(name);
        }
        for(unsigned i=0; i != outputs.size(); i++) {
            node_t* n = outputs[i];
            const char* name = get_renaming(n->get_name()).c_str();
            decls.add_output(name);
        }
        for(unsigned i=0; i != word_inputs.size(); i++) {
            word_t* w = word_inputs[i];
            for(unsigned j=0; j != w->size(); j++) {
                node_t* n = w->get_bit(j);
                const char* name = get_renaming(n->get_name()).c_str();
                decls.add_input(name);
            }
        }
        for(unsigned i=0; i != word_outputs.size(); i++) {
            word_t* w = word_outputs[i];
            for(unsigned j=0; j != w->size(); j++) {
                node_t* n = w->get_bit(j);
                const char* name = get_renaming(n->get_name()).c_str();
                decls.add_output(name);
            }
        }

        if(!needs_grouping()) {
            for(nodeset_t::iterator it = internals.begin(); it != internals.end(); it++) {
                node_t* n = *it;
                const char* name = get_renaming(n->get_name()).c_str();
                if(!(decls.is_input(name) || decls.is_output(name) || n->is_latch_gate())) {
                    node_t* sib = n->get_sibling();
                    decls.add_wire(name);

                    if(sib && !sib->is_latch_gate()) { 
                        const char* sib_name = get_renaming(sib->get_name()).c_str();
                        decls.add_wire(sib_name);
                    }
                }
            }
        }
    }

    void id_module_t::dump_inputmap(std::ostream& out, node_t* n)
    {
        if(inputmap.find(n) == inputmap.end()) return;
        if(inputmap[n].size()) {
            out << "  // BSIM@inputs: ";
            for(nodeset_t::iterator it = inputmap[n].begin(); it != inputmap[n].end(); it++) {
                node_t* i = *it;
                out << i->get_name() << " ";
            }
            out << std::endl;
        }
    }

    void id_module_t::get_input_modules(moduleset_t& mods)
    {
        for(unsigned i=0; i != inputs.size(); i++) {
            node_t* n = inputs[i];
            id_module_t* m = n->get_covering_module();
            if(m != NULL) {
                mods.insert(m);
            }
        }
        for(unsigned i=0; i != word_inputs.size(); i++) {
            word_t* w = word_inputs[i];
            for(unsigned j=0; j != w->size(); j++) {
                node_t* n = w->get_bit(j);
                id_module_t* m = n->get_covering_module();
                if(m != NULL) {
                    mods.insert(m);
                }
            }
        }
    }
    
    namespace {
        struct add_group_comments_t {
            id_module_t* mod;
            add_group_comments_t(id_module_t* m) : mod(m) {}
            void operator() (const std::string& grp, const id_module_t::nodelist_t& nl) {
                std::ostringstream ostr;
                ostr << "group: " << grp << "; nodes: " << nl;
                std::string str(ostr.str());
                mod->add_comment(str);
            }
        };
    }

    unsigned id_module_t::num_internal_gates() const
    {
        unsigned cnt=0;
        for(nodeset_t::const_iterator it = internals.begin(); it != internals.end(); it++) {
            node_t* n= *it;
            if(n->is_gate() && !n->is_latch_gate()) {
                cnt += 1;
            }
        }
        return cnt;
    }

    unsigned id_module_t::num_internal_nodes() const
    {
        unsigned cnt=0;
        for(nodeset_t::const_iterator it = internals.begin(); it != internals.end(); it++) {
            node_t* n= *it;
            if(!n->is_latch_gate()) {
                cnt += 1;
            }
        }
        return cnt;
    }

    void id_module_t::dump_comments(std::ostream& out)
    {
        out << std::endl;

        out << "  // moduleNum: x" << moduleNum() << std::endl;
        out << "  // type: " << get_type() << std::endl;
        out << "  // conflicting modules: ";
        for(moduleset_t::iterator it = conflicting_modules.begin(); it != conflicting_modules.end(); it++) {
            id_module_t* m = *it;
            out << m->get_type() << "_" << m->get_name() << " ";
        }
        out << std::endl;

        out << "  // number of bit inputs: " << num_inputs() << std::endl;
        out << "  // number of bit outputs: " << num_outputs() << std::endl;
        out << "  // number of word inputs: " << word_inputs.size() << std::endl;
        out << "  // number of word outputs: " << word_outputs.size() << std::endl;
        out << "  // number of internal gates: " << num_internals() << std::endl;
        out << "  // number of non-inferred gates: " << count_noninferred_gates() << std::endl;
        out << "  // ILP vars: " << getILPStartVar() << "/" << getILPEndVar() << std::endl;
        for(unsigned i=0; i != word_inputs.size(); i++) {
            std::ostringstream cstr;
            cstr << "input word of size " << word_inputs[i]->size() << " : " << *word_inputs[i];
            std::string cs = cstr.str();
            add_comment(cs);
        }

        for(unsigned i=0; i != word_outputs.size(); i++) {
            std::ostringstream cstr;
            cstr << "output word of size " << word_outputs[i]->size() << " : " << *word_outputs[i];
            std::string cs = cstr.str();
            add_comment(cs);
        }

        add_group_comments_t ag(this);
        apply_on_all_input_groups(ag);

        out << "  // type: " << typeStrings[moduleType] << std::endl;
        out << "  // marked bad? " << (is_bad ? "yes" : "no") << std::endl;
        for(commentList_t::iterator it = comments.begin(); it != comments.end(); it++) {
            out << "  // " << *it << std::endl;
        }
        out << std::endl;
    }

    void id_module_t::dump_structural_verilog(const char* type, std::ostream& vout, std::ostream& lout)
    {
        char *moduleName = new char[strlen(type) + 128];
        id_module_t* rep = get_repr();
        if(needs_grouping()) {
            sprintf(moduleName, "%s", type);
        } else {
            sprintf(moduleName, "%s_mx%d", type, rep->moduleNumber);
        }

        decl_list_t decls;
        this->add_decls(decls);
        if(rep == this && !needs_grouping()) {
            lout << "module " << moduleName << " ";
            decls.dump_verilog(lout);
            dump_comments(lout);
            for(nodeset_t::iterator it = internals.begin(); it != internals.end(); it++) {
                node_t* n = *it;
                dump_inputmap(lout, n);
                n->dump_verilog_defn(lout, false, dump_annotation, (dump_annotation ? annotation_module_name.c_str() : NULL), this);
            }
            lout << "endmodule" << std::endl;
        }

        // now dump module instantiation.
        // first the header
        vout << "  /*CASIO@<instance>" << std::endl;
        vout << "     <id>" << name << "</id>" << std::endl;
        vout << "     <module>" << moduleName << "</module>" << std::endl;
        vout << "     <compmap>"; dumpGates(vout, name); vout << "</compmap>" << std::endl;
        vout << "     <creationtool>bsim</creationtool>" << std::endl;
        vout << "     <confidence>0.60</confidence>" << std::endl;
        vout << "     <flag></flag>" << std::endl;
        vout << "     <wiremap></wiremap>" << std::endl;
        vout << "     </instance>" << std::endl;
        vout << "   */" << std::endl;

        decls.dump_instantiation(vout, moduleName, name, this);
        delete [] moduleName;
    }

    void id_module_t::dumpGates(std::ostream& out, const char* name)
    {
        int count=1;
        int sz = internals.size();
        nodeset_t dump;
        for(nodeset_t::iterator it = internals.begin(); it != internals.end(); it++) {
            node_t* n = *it;
            if(n->is_suppress_gate() || n->is_latch_gate() || n->is_input() || n->is_macro_out()) continue;
            else dump.insert(n);
        }

        for(nodeset_t::iterator it = dump.begin(); it != dump.end(); it++) {
            bool last = (count == sz);
            out << "F" << count++ << ":" << (*it)->get_instance_name();
            if(!last) {
                out << ", ";
            }
        }
    }

    void id_module_t::dump_mux41(std::ostream& out, const char* modulename, verilog_lib_t* vlib)
    {
        assert(word_inputs.size() == 4);
        assert(inputs.size() == 2);

        unsigned n = word_inputs[0]->size();
        assert(word_inputs[1]->size() == n);
        assert(word_inputs[2]->size() == n);
        assert(word_inputs[3]->size() == n);

        if(strcmp(modulename, "mux41i") == 0) {
            vlib->mux41i.insert(n);
        } else {
            assert(strcmp(modulename, "mux41") == 0);
            vlib->mux41.insert(n);
        }

        char fullName[256];
        sprintf(fullName, "%s_%d_%s", modulename, n, "casio1b");

        out << "  /*CASIO@<instance>" << std::endl;
        out << "     <id>" << name << "</id>" << std::endl;
        out << "     <module>" << fullName << "</module>" << std::endl;
        out << "     <compmap>"; dumpGates(out, name); out << "</compmap>" << std::endl;
        out << "     <creationtool>bsim</creationtool>" << std::endl;
        out << "     <confidence>0.85</confidence>" << std::endl;
        out << "     <flag></flag>" << std::endl;
        out << "     <wiremap></wiremap>" << std::endl;
        out << "     </instance>" << std::endl;
        out << "   */" << std::endl;

        out << "  " << fullName << " " << name << " ( ";
        out << ".s0 (" << inputs[0]->get_name() << "), ";
        out << ".s1 (" << inputs[1]->get_name() << "), ";
        out << ".a ("; dump_verilog_word(out, word_inputs[0]); out << "), ";
        out << ".b ("; dump_verilog_word(out, word_inputs[1]); out << "), ";
        out << ".c ("; dump_verilog_word(out, word_inputs[2]); out << "), ";
        out << ".d ("; dump_verilog_word(out, word_inputs[3]); out << "), ";
        out << ".y ("; dump_verilog_word(out, word_outputs[0]); out << ")";
        out << " );" << std::endl;
    }

    void id_module_t::dump_mux21(std::ostream& out, const char* modulename, verilog_lib_t* vlib)
    {
        assert(word_inputs.size() == 2);
        assert(inputs.size() == 1);

        unsigned n = word_inputs[0]->size();
        assert(word_inputs[1]->size() == n);

        if(strcmp(modulename, "mux21i") == 0) {
            vlib->mux21i.insert(n);
        } else {
            assert(strcmp(modulename, "mux21") == 0);
            vlib->mux21.insert(n);
        }

        char fullName[256];
        sprintf(fullName, "%s_%d_%s", modulename, n, "casio1b");

        out << "  /*CASIO@<instance>" << std::endl;
        out << "     <id>" << name << "</id>" << std::endl;
        out << "     <module>" << fullName << "</module>" << std::endl;
        out << "     <compmap>"; dumpGates(out, name); out << "</compmap>" << std::endl;
        out << "     <creationtool>bsim</creationtool>" << std::endl;
        out << "     <confidence>0.85</confidence>" << std::endl;
        out << "     <flag></flag>" << std::endl;
        out << "     <wiremap></wiremap>" << std::endl;
        out << "     </instance>" << std::endl;
        out << "   */" << std::endl;

        out << "  " << fullName << " " << name << " ( ";
        out << ".s (" << inputs[0]->get_name() << "), ";
        out << ".a ("; dump_verilog_word(out, word_inputs[0]); out << "), ";
        out << ".b ("; dump_verilog_word(out, word_inputs[1]); out << "), ";
        out << ".y ("; dump_verilog_word(out, word_outputs[0]); out << ")";
        out << " );" << std::endl;
    }
    
    void id_module_t::dump_verilog_word(std::ostream& out, word_t* word)
    {
        int sz = word->size();
        int p = 0;
        out << "{ ";
        for(word_t::reverse_iterator it = word->rbegin(); it != word->rend(); it++, p++) {
            node_t* n = *it;
            out << n->get_name();
            if(p+1 != sz) {
                out << ", ";
            }
        }
        out << "}";
    }

    BDD id_module_t::getProduct(int num, int vars, Cudd& mgr)
    {
        BDD product = mgr.bddOne();
        for(int i = 0; i != vars; i++) {
            int bit = num & (1 << i);
            BDD var = mgr.bddVar(i);
            if(bit) {
                product = product & var;
            } else {
                product = product & !var;
            }
        }
        return product;
    }

    void id_module_t::doRAMReadAnalysis()
    {
        Cudd cudd;
        fn_map_t map;

        createFullFunctions(cudd, map);
        for(unsigned j=0; j != outputs.size(); j++) {
            node_t* n = outputs[j];
            assert(map.find(n) != map.end());
            std::cout << "node: " << n->get_name() << std::endl;
            for(int i = 0; i != 256; i++) {
                BDD o = map[n];
                BDD b = getProduct(i, 8, cudd);
                std::cout << i << " : "; printBDD(stdout, o.Cofactor(b));
            }
        }
    }

    void id_module_t::printFullFunctions()
    {
        Cudd cudd;
        fn_map_t map;

        createFullFunctions(cudd, map);
        for(unsigned i = 0; i != word_outputs.size(); i++) {
            word_t *w = word_outputs[i];
            for(word_t::iterator it = w->begin(); it != w->end(); it++) {
                node_t* n = *it;
                assert(map.find(n) != map.end());
                std::cout << n->get_name() << " :: "; printBDD(stdout, map[n]);
            }
        }
        for(unsigned i=0; i != outputs.size(); i++) {
            node_t* n = outputs[i];
            assert(map.find(n) != map.end());
            std::cout << n->get_name() << " :: "; printBDD(stdout, map[n]);
        }
    }

    void id_module_t::dumpGates(std::ostream& out)
    {
        for(nodeset_t::iterator it = internals.begin(); it != internals.end(); it++) {
            node_t* n = *it;
            n->dump(out);
        }
    }

    // TODO: do we need this function?
    void id_module_t::compareAdderBDDs()
    {
        assert(strcmp(get_type(), "ripple_addsub") == 0);
        assert(word_outputs.size() == 1 && word_inputs.size() == 2);

        Cudd cudd;
        fn_map_t struct_map;
        fn_map_t fun_map;

        createFullFunctions(cudd, struct_map);
        createAdderBDDs(cudd, fun_map, struct_map);

        word_t* w = word_outputs[0];
        for(word_t::iterator it = w->begin(); it != w->end(); it++) {
            node_t* n = *it;
            assert(struct_map.find(n) != struct_map.end());
            assert(fun_map.find(n) != fun_map.end());
            std::cout << n->get_name() << " <<strct>> "; printBDD(stdout, struct_map[n]);
            std::cout << n->get_name() << " <<funct>> "; printBDD(stdout, fun_map[n]);
            // bool match = (struct_map[n] == fun_map[n]) || (struct_map[n] == !fun_map[n]);
            BDD f = struct_map[n];
            BDD g = fun_map[n];
            BDD d = f.Xor(g);
            std::cout << n->get_name() << " ^^ " ; printBDD(stdout, d);
            std::cout << std::endl;
        }
    }

    void id_module_t::createAdderBDDs(Cudd& cudd, fn_map_t& map, fn_map_t& struct_map)
    {
        assert(strcmp(get_type(), "ripple_addsub") == 0);
        assert(map.size() == 0);

        // bddvar ptr.
        unsigned p = 0;

        // get carry in.
        BDD cin;
        if(inputs.size() == 0) {
            cin = cudd.bddZero();
        } else if(inputs.size() == 1) {
            cin = cudd.bddVar(p++);
            node_t *n = inputs[0];
            map[n] = cin;
        } else {
            assert(false);
        }

        std::vector<BDD> a;
        std::vector<BDD> b;

        // now create the input words.
        assert(word_inputs.size() == 2);
        assert(word_inputs[0]->size() == word_inputs[1]->size());

        word_t *w1 = word_inputs[0];
        word_t *w2 = word_inputs[1];
        for(unsigned i=0; i != w1->size(); i++) {
            node_t* n = w1->get_bit(i);
            if(map.find(n) == map.end()) {
                map[n] = cudd.bddVar(p++);
            }
            a.push_back(map[n]);
        }
        for(unsigned i=0; i != w2->size(); i++) {
            node_t* n = w2->get_bit(i);
            if(map.find(n) == map.end()) {
                map[n] = cudd.bddVar(p++);
            }
            b.push_back(map[n]);
        }

        // now create the output words.
        assert(word_outputs.size() == 1);
        assert(word_outputs[0]->size() == word_inputs[0]->size());
        word_t* y = word_outputs[0];
        for(unsigned i=0; i != y->size(); i++) {
            node_t* n = y->get_bit(i);
            assert(map.find(n) == map.end());

            BDD ai = a[i];
            BDD bi = b[i];
            BDD ci = cin;

            BDD sum = ai.Xor(bi.Xor(ci));
            BDD carry = (ai & bi) | (bi & ci) | (ci & ai);

            map[n] = sum;
            cin = carry;
        }
    }

    namespace {
        struct input_bdd_creator_t {
            Cudd& mgr;
            id_module_t::fn_map_t& var_map;
            id_module_t::fn_rev_map_t& rev_map;

            input_bdd_creator_t(Cudd& cudd, id_module_t::fn_map_t& m1, id_module_t::fn_rev_map_t& m2)
                : mgr(cudd)
                , var_map(m1)
                , rev_map(m2)
            { }

            void operator() (node_t* n) {
                assert(var_map.size() == rev_map.size());

                id_module_t::fn_map_t::iterator it = var_map.find(n);
                if(it == var_map.end()) {
                    BDD bi = mgr.bddVar(var_map.size());
                    var_map[n] = bi;
                    rev_map[bi] = n;
                }
            }
            void operator() (const std::string& group, node_t* n)
            {
                this->operator() (n);
            }
        };

        struct output_bdd_creator_t {
            id_module_t* module;
            Cudd& cudd;
            id_module_t::fn_map_t& map;
            std::vector<node_t*>& output_nodes;
            std::vector<BDD>& output_bdds;

            output_bdd_creator_t(
                id_module_t* mod, 
                Cudd& mgr, 
                id_module_t::fn_map_t& map, 
                std::vector<node_t*>& nl, 
                std::vector<BDD>& bl
            )
                : module(mod)
                , cudd(mgr)
                , map(map)
                , output_nodes(nl)
                , output_bdds(bl)
            {}

            void operator() (node_t* n)
            {
                BDD b = module->createFullFunction(cudd, map, n); 
                output_nodes.push_back(n);
                output_bdds.push_back(b);
            }
        };

    }

    void id_module_t::createFullFunctions(
        Cudd& cudd, 
        fn_map_t& var_map, 
        fn_rev_map_t& rev_map, 
        std::vector<node_t*>& outputs,
        std::vector<BDD>& bdds
    )
    {
        input_bdd_creator_t icreat(cudd, var_map, rev_map);

        // create BDDs for all the inputs.
        apply_on_all_grouped_inputs(icreat);
        apply_on_all_inputs(icreat);

        // now for the outputs.
        output_bdd_creator_t ocreat(this, cudd, var_map, outputs, bdds);
        apply_on_all_outputs(ocreat);
    }

    node_t* id_module_t::evalBDD(
        Cudd& mgr, 
        fn_map_t& map, 
        fn_rev_map_t& rev, 
        nodelist_t& inps, 
        int value, 
        BDD& out)
    {
        BDD cube = mgr.bddOne();
        for(unsigned i = 0; i != inps.size(); i++) {
            assert(map.find(inps[i]) != map.end());

            int bit_i = value & (1 << i);
            if(bit_i) {
                cube = cube & (map[inps[i]]);
            } else {
                cube = cube & (!map[inps[i]]);
            }
        }

        BDD result = out.Cofactor(cube);
        if(result.SupportSize() == 1) {
            BDD res = result.Support();
            assert(rev.find(res) != rev.end());
            return rev[res];
        } else {
            return NULL;
        }
    }

    void id_module_t::muxAnalysis()
    {
        std::string sel("sel");

        node_groups_t::iterator sel_iter = node_groups.find(sel);
        assert(sel_iter != node_groups.end());
        nodelist_t& selnodes = sel_iter->second;

        Cudd cudd;
        fn_map_t var_map;
        fn_rev_map_t rev_map;
        std::vector<node_t*> output_nodes;
        std::vector<BDD> output_fns;

        createFullFunctions(cudd, var_map, rev_map, output_nodes, output_fns);
        assert(output_nodes.size() == output_fns.size());


        int max_count = 1 << (selnodes.size());

        nodeset_t covered_inputs;
        for(int i = 0; i < max_count; i++) {
            for(unsigned j=0; j != output_nodes.size(); j++) {
                node_t* inp_ij = evalBDD( cudd, var_map, rev_map, selnodes, i, output_fns[j]);
                if(inp_ij != NULL) {
                    covered_inputs.insert(inp_ij);
                }
            }
        }
        std::copy(selnodes.begin(), selnodes.end(), std::inserter(covered_inputs, covered_inputs.end()));
        std::cout << "# of covered inputs: " << covered_inputs.size() << std::endl;
        std::cout << "# of total   inputs: " << total_inputs() << std::endl;

        var_map.clear();
        rev_map.clear();
        output_fns.clear();
    }

    void id_module_t::createFullFunctions(Cudd& cudd, fn_map_t& map)
    {
        assert(map.size() == 0);
        // initialize the map
        unsigned p = 0;
        // first non-word inputs.
        for(unsigned i=0; i != inputs.size(); i++) {
            node_t* n = inputs[i];
            if(map.find(n) == map.end()) {
                map[n] = cudd.bddVar(p++);
            }
        }
        // now words.
        for(unsigned i=0; i != word_inputs.size(); i++) {
            word_t* w = word_inputs[i];
            for(word_t::iterator it = w->begin(); it != w->end(); it++) {
                node_t* n = *it;
                if(map.find(n) == map.end()) {
                    map[n] = cudd.bddVar(p++);
                }
            }
        }
        // now iterate through the outputs and create functions for each.
        for(unsigned i=0; i != outputs.size(); i++) {
            node_t* n = outputs[i];
            if(map.find(n) == map.end()) {
                BDD b = createFullFunction(cudd, map, n); 
                map[n] = b;
            }
        }
        for(unsigned i=0; i != word_outputs.size(); i++) {
            word_t* w = word_outputs[i];
            for(word_t::iterator it = w->begin(); it != w->end(); it++) {
                node_t* n = *it;
                if(map.find(n) == map.end()) {
                    BDD b = createFullFunction(cudd, map, n);
                    map[n] = b;
                }
            }
        }
    }

    struct module_input_provider_t : public input_provider_t
    {
        Cudd& cudd;
        id_module_t* module;
        id_module_t::fn_map_t& map;
        node_t* node;

        module_input_provider_t(Cudd& cudd, id_module_t* module, id_module_t::fn_map_t& map, node_t* node)
            : cudd(cudd),
              module(module),
              map(map),
              node(node)
        {
        }

        virtual BDD inp(int index)
        {
            assert(index < (int) node->num_inputs());
            node_t* n = node->get_input(index);
            if(map.find(n) != map.end()) { return map[n]; }
            else {
                BDD b = module->createFullFunction(cudd, map, n);
                map[n] = b;
                return b;
            }
        }
        virtual BDD one()
        {
            return cudd.bddOne();
        }

        virtual BDD zero()
        {
            return cudd.bddZero();
        }
    };

    void id_module_t::add_inputset(nodeset_t& cone) const
    {
        for (nodelist_t::const_iterator inpit = inputs.begin(); inpit != inputs.end(); inpit++) {
            cone.insert(*inpit);
        }
        for (wordlist_t::const_iterator it = word_inputs.begin(); it != word_inputs.end(); it++) {
            for (word_t::const_iterator wit = (*it)->begin(); wit != (*it)->end(); wit++) {
                cone.insert(*wit);
            }
        }
    }

    void id_module_t::create_new_modules()
    {
        if(matched_nodes.size() == 0) return;

        nodeset_t cone;
        add_inputset(cone);

        nodeset_t inputs;
        nodeset_t outputs;
        std::string name;
        flat_module_t *flat = NULL;
        for(matched_node_t::iterator it = matched_nodes.begin(); it != matched_nodes.end(); it++)
        {
            node_t* n = *it;
            if(flat == NULL) {
                flat = n->get_module();
            } else {
                assert(flat == n->get_module());
            }

            nodeset_t visited;
            find_inputs(n, cone, visited, inputs);
            outputs.insert(n);

        }
        for(std::set<std::string>::iterator jt = libtypes.begin(); jt != libtypes.end(); jt++) {
            const std::string& s = *jt;
            name += "_" + s;
        }
        id_module_t* mod  = new id_module_t(("libmatch" + name).c_str(), id_module_t::UNSLICEABLE, INFERRED);
        for(nodeset_t::iterator it = outputs.begin(); it != outputs.end(); it++) {
            mod->add_output(*it);
        }
        for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
            mod->add_input(*it);
        }
        mod->compute_internals();
        flat->add_module(mod);
    }

    bool id_module_t::is_seq() const
    {
        if(is_seq_memo == -1) {
            is_seq_memo = 0;
            for(nodeset_t::const_iterator it = internals.begin(); it != internals.end(); it++) {
                node_t* n = *it;
                if(n->is_latch()) {
                    is_seq_memo = 1;
                    break;
                }
            }
        }
        return is_seq_memo;
    }

    BDD id_module_t::createFullFunction(Cudd& cudd, fn_map_t& map, node_t* n)
    {
        if(map.find(n) != map.end()) {
            return map[n];
        } else {
            module_input_provider_t mipp(cudd, this, map, n);
            BDD b = n->get_lib_elem()->getFn(&mipp);
            map[n] = b;
            return b;
        }
    }

    unsigned id_module_t::total_outputs() const {
        unsigned sum = outputs.size();
        for(unsigned i=0; i != word_outputs.size(); i++) {
            sum += word_outputs[i]->size();
        }
        return sum;
    }

    unsigned id_module_t::total_inputs() const {
        unsigned sum = inputs.size();
        for(unsigned i=0; i != word_inputs.size(); i++) {
            sum += word_inputs[i]->size();
        }
        return sum;
    }

    bitslice_t::bitslice_t(kcover_t* kcov)
    {
        std::vector<uint8_t>& perm = kcov->getPerm();
        for(unsigned i=0; i != kcov->numInputs(); i++) {
            for(unsigned j=0; j != kcov->numInputs(); j++) {
                if(perm[j] == i) {
                    xs.push_back(kcov->at(j));
                }
            }
        }
        ys.push_back(kcov->get_root());
    }

    bitslice_t::bitslice_t(const bitslice_t& other)
        : xs (other.xs),
          ys (other.ys)
    {
    }

    bool bitslice_t::operator<(const bitslice_t& other)
    {
        assert(xs.size() == other.xs.size());
        assert(ys.size() == other.ys.size());

        for(unsigned i=0; i != xs.size(); i++) {
            node_t* a = xs[i];
            node_t* b = other.xs[i];
            if(a < b) return true;
            else if(a > b) return false;
        }
        return false;
    }

    bool bitslice_t::operator==(const bitslice_t& other) const
    {
        if(xs.size() != other.xs.size()) return false;
        if(ys.size() != other.ys.size()) return false;

        for(unsigned i=0; i != xs.size(); i++) {
            node_t* a = xs[i];
            node_t* b = other.xs[i];
            if(a != b) return false;
        }
        for(unsigned i=0; i != ys.size(); i++) {
            node_t* a = ys[i];
            node_t* b = other.ys[i];
            if(a != b) return false;
        }
        return true;
    }

    void bitslice_t::sort_inputs()
    {
        std::sort(xs.begin(), xs.end());
    }

    bool bitslice_t::same_inputs(const bitslice_t& other) const
    {
        if(xs.size() != other.xs.size()) return false;
        else {
            for(unsigned i=0; i != xs.size(); i++) {
                node_t* a = xs[i];
                std::vector<node_t*>::const_iterator pos = std::find(
                    other.xs.begin(), 
                    other.xs.end(), 
                    a
                );
                if(pos == other.xs.end()) return false;
            }
            return true;
        }
    }

    bool bitslice_t::has_input(const node_t* n) const
    {
        for(unsigned i=0; i != xs.size(); i++) {
            if(n == xs[i]) return true;
        }
        return false;
    }

    bool bitslice_t::not_related_inputs(const bitslice_t& other) const
    {
        if(xs.size() != other.xs.size()) return false;
        else {
            for(unsigned i=0; i != xs.size(); i++) {
                node_t* a = xs[i];
                bool found = false;
                for(unsigned j=0; j != other.xs.size(); j++) {
                    node_t* b = other.xs[j];
                    if(a == b || a->get_module()->not_related(a, b)) {
                        found = true;
                        break;
                    }
                }
                if(!found) return false;
            }
            return true;
        }
    }

    bool bitslice_t::inv_contained(const bitslice_t& other) const
    {
        if(xs.size() != other.xs.size()) return false;
        else {
            for(unsigned i=0; i != xs.size(); i++) {
                node_t* a = xs[i];
                bool found = false;
                for(unsigned j=0; j != other.xs.size(); j++) {
                    node_t* b = other.xs[j];
                    if(a == b || (b->is_gate() && b->is_inverter() && b->get_input(0) == a)) {
                        found = true;
                        break;
                    }
                }
                if(!found) return false;
            }
            for(unsigned i=0; i != ys.size(); i++) {
                node_t* a = ys[i];
                node_t* b = other.ys[i];
                if(a != b) return false;
            }
            return true;
        }
    }

    bool bitslice_t::is_input(int output_index, bitslice_t* bt) const
    {
        for(unsigned i=0; i != xs.size(); i++) {
            node_t* a = xs[i];
            if(a == bt->ys[output_index]) return true;
        }
        return false;
    }

    bitslice_t* bitslice_t::mergeSlices(bitslice_t* a, bitslice_t* b)
    {
        a->sort_inputs();
        b->sort_inputs();

        if(!a->same_inputs(*b)) return NULL;
        else {
            assert(a->ys.size() == b->ys.size());
            assert(a->ys.size() == 1);
            assert(a->ys[0] != b->ys[0]);

            bitslice_t* m = new bitslice_t();
            for(unsigned i=0; i != a->xs.size();  i++) {
                node_t* n = a->xs[i];
                m->xs.push_back(n);
            }
            m->ys.push_back(a->ys[0]);
            m->ys.push_back(b->ys[0]);
            return m;
        }
    }

    void bitslice_t::add_internals(node_t* n, std::set<node_t*>& internals) const
    {
        if(std::find(xs.begin(), xs.end(), n) != xs.end()) return;
        else if(internals.find(n) != internals.end()) return;
        else {
            internals.insert(n);
            for(node_t::input_iterator it=n->inputs_begin(); it != n->inputs_end(); it++) {
                node_t* i = *it;
                add_internals(i, internals);
            }
        }
    }

    bool bitslice_t::is_covered() const
    {
        nodeset_t internals;
        for(unsigned i=0; i != ys.size(); i++) {
            add_internals(ys[i], internals);
        }
        for(nodeset_t::iterator it = internals.begin(); it != internals.end(); it++) {
            node_t* n = *it;
            if(n->is_covered()) return true;
        }
        return false;
    }

    // TODO: replace this with calls to the i12soi defns.
    BDD mux21(input_provider_t* e)
    {
            BDD s = e->inp(2);
            BDD a = e->inp(0);
            BDD b = e->inp(1);
            return ((!s & a) | (s & b));
    }

    BDD mux4(input_provider_t* e) {
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
    BDD mux4i(input_provider_t* e) {
        return !mux4(e);
    }


    BDD mux3(input_provider_t* e) {
        BDD a = e->inp(0);
        BDD b = e->inp(1);
        BDD c = e->inp(2);

        BDD s0 = e->inp(3);
        BDD s1 = e->inp(4);

        return (!s1 & !s0 & a) |
               (!s1 &  s0 & b) |
               (s1  & !s0 & c);
    }
    BDD mux3i(input_provider_t* e) {
        return !mux3(e);
    }


    BDD mux31b(input_provider_t* e) { 
	BDD a = e->inp(0);
	BDD b = e->inp(1);
	BDD c = e->inp(2);
	BDD s0 = e->inp(3);
	BDD s1 = e->inp(4);

	return (  s1 & c ) |
		( !s1 & s0 & b) |
		( !s1 & !s0 & a);	
    }

    BDD mux31bi(input_provider_t* e) {
	return !mux31b(e);
    }
 

    BDD mux31c(input_provider_t* e) { 
	BDD a = e->inp(0);
	BDD b = e->inp(1);
	BDD c = e->inp(2);
	BDD s0 = e->inp(3);
	BDD s1 = e->inp(4);

	return (  !s1 & c ) |
		( s1 & s0 & b) |
		( s1 & !s0 & a);	
    }

    BDD mux31ci(input_provider_t* e) {
	return !mux31c(e);
    }

    void identify31Muxes(flat_module_t* module)
    {
        std::cout << "identifying 3:1 muxes." << std::endl;
        if(options.kcoverSize < 5) return;
        input_provider_t* ipp5 = module->get_ipp(5);
        if(ipp5 == NULL) return;

        BDD mux31bdd = mux3(ipp5);
        BDD mux31ibdd = mux3i(ipp5);
        BDD mux31bbdd = mux31b(ipp5);
        BDD mux31bibdd = mux31bi(ipp5);
        BDD mux31cbdd = mux31c(ipp5);
        BDD mux31cibdd = mux31ci(ipp5);


        aggregate31Muxes(module, module->getFunction(mux31cbdd), "mux31c");
        aggregate31Muxes(module, module->getFunction(mux31cibdd), "mux31ci");
        aggregate31Muxes(module, module->getFunction(mux31bbdd), "mux31b");
        aggregate31Muxes(module, module->getFunction(mux31bibdd), "mux31bi");
        aggregate31Muxes(module, module->getFunction(mux31bdd), "mux31");
        aggregate31Muxes(module, module->getFunction(mux31ibdd), "mux31i");
    }
    
    void aggregate31Muxes(flat_module_t* module, fnInfo_t* info, const char* name)
    {
        if(info == NULL) {
            return;
        }

        std::vector<uint8_t>& p = info->permutation;
        assert(p.size() == 5);
        int s0pos = p[3];
        int s1pos = p[4];

        assert(s0pos != -1 && s1pos != -1);
        std::map< nodelist_t, std::vector<bitslice_t*> > muxMap;
        std::vector<bitslice_t*> muxes;
        std::set<kcover_t*>& covers = info->canonicalPtr->covers;
        for(std::set<kcover_t*>::iterator it = covers.begin(); it != covers.end(); it++) {
            kcover_t* cover = *it;
            if(cover->get_root()->is_latch_gate()) continue;
            bitslice_t* bt = new bitslice_t(cover);
            muxes.push_back(bt);
            node_t* s0 = bt->xs[s0pos];
            node_t* s1 = bt->xs[s1pos];
            nodelist_t nl(s0, s1);
            muxMap[nl].push_back(bt);
        }
        for(std::map< nodelist_t, std::vector<bitslice_t*> >::iterator it = muxMap.begin();
                                                                    it != muxMap.end();
                                                                    it++)
        {
            const nodelist_t& nl = it->first;
            if(it->second.size() >= options.minMultibitElementSize) {
                word_t *w1, *w2, *w3, *w4;
                module->add_word(w1 = createInputWord(module, it->second, p[0], 1));
                module->add_word(w2 = createInputWord(module, it->second, p[1], 1));
                module->add_word(w3 = createInputWord(module, it->second, p[2], 1));
                module->add_word(w4 = createOutputWord(module, it->second, 0, 1));

                if(w1 && w2 && w3 && w4) {
                    id_module_t* mod = new id_module_t(name, id_module_t::SLICEABLE);
                    mod->add_input("sel", nl.nodes[0], false);
                    mod->add_input("sel", nl.nodes[1], false);
                    mod->add_input_word(w1);
                    mod->add_input_word(w2);
                    mod->add_input_word(w3);
                    mod->add_output_word(w4);
                    mod->compute_internals();
                    module->add_module(mod);
                }
            }
        }

        deleteSlices(muxes);
    }

    void identify41Muxes(flat_module_t* module)
    {
        std::cout << "identifying 4:1 muxes." << std::endl;

        if(options.kcoverSize < 6) return;
        input_provider_t* ipp6 = module->get_ipp(6);
        if(ipp6 == NULL) return;

        BDD mux41bdd = mux4(ipp6);
        BDD imux41bdd = mux4i(ipp6);

        aggregate41Muxes(module, module->getFunction(mux41bdd), "mux41");
        aggregate41Muxes(module, module->getFunction(imux41bdd), "mux41i");
    }

    void aggregate41Muxes(flat_module_t* module, fnInfo_t* info, const char* name)
    {
        if(info == NULL) return;

        std::vector<uint8_t>& p = info->permutation;
        assert(p.size() == 6);
        int s0pos = p[4];
        int s1pos = p[5];

        assert(s0pos != -1 && s1pos != -1);
        std::map< nodelist_t, std::vector<bitslice_t*> > muxMap;
        std::vector<bitslice_t*> muxes;
        std::set<kcover_t*>& covers = info->canonicalPtr->covers;
        for(std::set<kcover_t*>::iterator it = covers.begin(); it != covers.end(); it++) {
            kcover_t* cover = *it;
            if(cover->get_root()->is_latch_gate()) continue;
            bitslice_t* bt = new bitslice_t(cover);
            muxes.push_back(bt);
            node_t* s0 = bt->xs[s0pos];
            node_t* s1 = bt->xs[s1pos];
            nodelist_t nl(s0, s1);
            muxMap[nl].push_back(bt);
        }
        for(std::map< nodelist_t, std::vector<bitslice_t*> >::iterator it = muxMap.begin();
                                                                    it != muxMap.end();
                                                                    it++)
        {
            const nodelist_t& nl = it->first;
            if(it->second.size() >= options.minMultibitElementSize) {
                word_t *w1, *w2, *w3, *w4, *w5;
                module->add_word(w1 = createInputWord(module, it->second, p[0], 1));
                module->add_word(w2 = createInputWord(module, it->second, p[1], 1));
                module->add_word(w3 = createInputWord(module, it->second, p[2], 1));
                module->add_word(w4 = createInputWord(module, it->second, p[3], 1));
                module->add_word(w5 = createOutputWord(module, it->second, 0, 1));

                if(w1 && w2 && w3 && w4 && w5) {
                    id_module_t* mod = new id_module_t(name, id_module_t::SLICEABLE);
                    mod->add_input("sel", nl.nodes[0], false);
                    mod->add_input("sel", nl.nodes[1], false);
                    mod->add_input_word(w1);
                    mod->add_input_word(w2);
                    mod->add_input_word(w3);
                    mod->add_input_word(w4);
                    mod->add_output_word(w5);
                    mod->compute_internals();
                    module->add_module(mod);
                }
            }
        }

        deleteSlices(muxes);
    }

    void deleteSlices(std::vector<bitslice_t*>& slices)
    {
        for(unsigned i=0; i != slices.size(); i++) {
            delete slices[i];
        }
        slices.clear();
    }

    BDD decoder24i00(input_provider_t* e) { return !e->inp(0) & !e->inp(1); }
    BDD decoder24i01(input_provider_t* e) { return !e->inp(0) &  e->inp(1); }
    BDD decoder24i10(input_provider_t* e) { return  e->inp(0) & !e->inp(1); }
    BDD decoder24i11(input_provider_t* e) { return  e->inp(0) &  e->inp(1); }

    void add24Decoders(fnInfo_t* info, int tag, decoder24map_t& decs)
    {
        if(info == NULL) return;
        fnInfo_t* i = info->canonicalPtr;
        int finalTag = tag;
        for(kcoverset_t::iterator it = i->covers.begin(); it != i->covers.end(); it++) {
            kcover_t* kc = *it;
            bitslice_t* bt = new bitslice_t(kc);
            assert(bt->xs.size() == 2);
            if(tag == 1) {
                if(kc->getPi(0) == 0 && kc->getPi(1) == 1) { finalTag = tag; }
                else if(kc->getPi(0) == 1 && kc->getPi(1) == 0) { finalTag = tag+1; }
                else { assert(false); }
            }
            decoder24Info_t dec = { bt, finalTag };
            nodepair_t inputs = createNodePair(bt->xs[0], bt->xs[1]);
            decs[inputs].push_back(dec);
        }
    }

    BDD decoder38i0(input_provider_t* e) { return !e->inp(0) & !e->inp(1) & !e->inp(2); }
    BDD decoder38i1(input_provider_t* e) { return !e->inp(0) & !e->inp(1) &  e->inp(2); }
    BDD decoder38i2(input_provider_t* e) { return !e->inp(0) &  e->inp(1) &  e->inp(2); }
    BDD decoder38i3(input_provider_t* e) { return  e->inp(0) &  e->inp(1) &  e->inp(2); }

    void add38Decoders(fnInfo_t* info, int tag, decoder38map_t& decs)
    {
        if(info == NULL) return;
        fnInfo_t *i = info->canonicalPtr;
        for(kcoverset_t::iterator it = i->covers.begin(); it != i->covers.end(); it++) {
            kcover_t* kc = *it;
            bitslice_t* bt = new bitslice_t(kc);
            assert(bt->xs.size() == 3);
            decoder38Info_t dec = { bt, tag };
            nodetriple_t inputs(bt->xs);
            decs[inputs].push_back(dec);
        }
    }

    BDD decoder416i0(input_provider_t* e) { return !e->inp(0) & !e->inp(1) & !e->inp(2) & !e->inp(3); }
    BDD decoder416i1(input_provider_t* e) { return !e->inp(0) & !e->inp(1) & !e->inp(2) &  e->inp(3); }
    BDD decoder416i2(input_provider_t* e) { return !e->inp(0) & !e->inp(1) &  e->inp(2) &  e->inp(3); }
    BDD decoder416i3(input_provider_t* e) { return !e->inp(0) &  e->inp(1) &  e->inp(2) &  e->inp(3); }
    BDD decoder416i4(input_provider_t* e) { return  e->inp(0) &  e->inp(1) &  e->inp(2) &  e->inp(3); }

    void add416Decoders(fnInfo_t* info, int tag, decoder416map_t& decs)
    {
        if(info == NULL) return;
        fnInfo_t *i = info->canonicalPtr;
        for(kcoverset_t::iterator it = i->covers.begin(); it != i->covers.end(); it++) {
            kcover_t* kc = *it;
            bitslice_t* bt = new bitslice_t(kc);
            assert(bt->xs.size() == 4);
            decoder416Info_t dec = { bt, tag };
            nodequad_t inputs(bt->xs);
            decs[inputs].push_back(dec);
        }
    }

    BDD decoder532i0(input_provider_t* e) {return !e->inp(0) & !e->inp(1) & !e->inp(2) & !e->inp(3) & !e->inp(4); }
    BDD decoder532i1(input_provider_t* e) {return !e->inp(0) & !e->inp(1) & !e->inp(2) & !e->inp(3) &  e->inp(4); }
    BDD decoder532i2(input_provider_t* e) {return !e->inp(0) & !e->inp(1) & !e->inp(2) &  e->inp(3) &  e->inp(4); }
    BDD decoder532i3(input_provider_t* e) {return !e->inp(0) & !e->inp(1) &  e->inp(2) &  e->inp(3) &  e->inp(4); }
    BDD decoder532i4(input_provider_t* e) {return !e->inp(0) &  e->inp(1) &  e->inp(2) &  e->inp(3) &  e->inp(4); }
    BDD decoder532i5(input_provider_t* e) {return  e->inp(0) &  e->inp(1) &  e->inp(2) &  e->inp(3) &  e->inp(4); }



    void add532Decoders(fnInfo_t* info, int tag, decoder532map_t& decs)
    {
	if(info == NULL) return;
	fnInfo_t *i = info->canonicalPtr;
	for (kcoverset_t::iterator it = i->covers.begin(); it != i->covers.end(); it++) {
		kcover_t* kc = *it;
		bitslice_t* bt = new bitslice_t(kc);
		assert(bt->xs.size() == 5);
		decoder532Info_t dec = { bt, tag };
		node5tup_t inputs(bt->xs);
		decs[inputs].push_back(dec);

	}
    } 


    BDD decoder664i0(input_provider_t* e) {return !e->inp(0) & !e->inp(1) & !e->inp(2) & !e->inp(3) & !e->inp(4) & !e->inp(5); }
    BDD decoder664i1(input_provider_t* e) {return !e->inp(0) & !e->inp(1) & !e->inp(2) & !e->inp(3) & !e->inp(4) &  e->inp(5); }
    BDD decoder664i2(input_provider_t* e) {return !e->inp(0) & !e->inp(1) & !e->inp(2) & !e->inp(3) &  e->inp(4) &  e->inp(5); }
    BDD decoder664i3(input_provider_t* e) {return !e->inp(0) & !e->inp(1) & !e->inp(2) &  e->inp(3) &  e->inp(4) &  e->inp(5); }
    BDD decoder664i4(input_provider_t* e) {return !e->inp(0) & !e->inp(1) &  e->inp(2) &  e->inp(3) &  e->inp(4) &  e->inp(5); }
    BDD decoder664i5(input_provider_t* e) {return !e->inp(0) &  e->inp(1) &  e->inp(2) &  e->inp(3) &  e->inp(4) &  e->inp(5); }
    BDD decoder664i6(input_provider_t* e) {return  e->inp(0) &  e->inp(1) &  e->inp(2) &  e->inp(3) &  e->inp(4) &  e->inp(5); }



    void add664Decoders(fnInfo_t* info, int tag, decoder664map_t& decs)
    {
	if(info == NULL) return;
	fnInfo_t *i = info->canonicalPtr;
	for (kcoverset_t::iterator it = i->covers.begin(); it != i->covers.end(); it++) {
		kcover_t* kc = *it;
		bitslice_t* bt = new bitslice_t(kc);
		assert(bt->xs.size() == 6);
		decoder664Info_t dec = { bt, tag };
		node_tuple_t inputs(bt->xs);
		decs[inputs].push_back(dec);

	}
    } 

    void add24Demuxes(fnInfo_t* info, int tag, demux24map_t& demuxes)
    {
        if(info == NULL) return;
        fnInfo_t* i = info->canonicalPtr;
        for(kcoverset_t::iterator it = i->covers.begin(); it != i->covers.end(); it++) {
            kcover_t* kc = *it;
            bitslice_t* bt = new bitslice_t(kc);
            demux24Info_t dmx = { bt, tag };
            assert(bt->xs.size() == 3);
            nodetriple_t inputs(bt->xs);
            if(tag == 0) {
                demuxes[inputs].push_back(dmx);
            } else {
                if(demuxes.find(inputs) != demuxes.end()) {
                    demuxes[inputs].push_back(dmx);
                } else {
                    delete bt;
                }
            }
        }
    }

    BDD demux38i0(input_provider_t* e) { return !e->inp(1) & !e->inp(2) & !e->inp(3) & e->inp(0); }
    BDD demux38i1(input_provider_t* e) { return !e->inp(1) & !e->inp(2) &  e->inp(3) & e->inp(0); }
    BDD demux38i2(input_provider_t* e) { return !e->inp(1) &  e->inp(2) &  e->inp(3) & e->inp(0); }
    BDD demux38i3(input_provider_t* e) { return  e->inp(1) &  e->inp(2) &  e->inp(3) & e->inp(0); }

    void add38Demuxes(fnInfo_t* info, int tag, demux38map_t& demuxes)
    {
        if(info == NULL) return;
        fnInfo_t* i = info->canonicalPtr;
        for(kcoverset_t::iterator it = i->covers.begin(); it != i->covers.end(); it++) {
            kcover_t* kc = *it;
            bitslice_t* bt = new bitslice_t(kc);
            demux38Info_t dmx = { bt, tag };
            assert(bt->xs.size() == 4);
            nodequad_t inputs(bt->xs);
            if(tag == 0) {
                demuxes[inputs].push_back(dmx);
            } else {
                if(demuxes.find(inputs) != demuxes.end()) {
                    demuxes[inputs].push_back(dmx);
                } else {
                    delete bt;
                }
            }
        }
    }

    BDD demux416i0(input_provider_t* e) { return !e->inp(1) & !e->inp(2) & !e->inp(3) & !e->inp(4) & e->inp(0); }
    BDD demux416i1(input_provider_t* e) { return !e->inp(1) & !e->inp(2) & !e->inp(3) &  e->inp(4) & e->inp(0); }
    BDD demux416i2(input_provider_t* e) { return !e->inp(1) & !e->inp(2) & !e->inp(3) &  e->inp(4) & e->inp(0); }
    BDD demux416i3(input_provider_t* e) { return !e->inp(1) & !e->inp(2) &  e->inp(3) &  e->inp(4) & e->inp(0); }
    BDD demux416i4(input_provider_t* e) { return  e->inp(1) &  e->inp(2) &  e->inp(3) &  e->inp(4) & e->inp(0); }

    void add416Demuxes(fnInfo_t* info, int tag, demux416map_t& demuxes)
    {
        if(info == NULL) return;
        fnInfo_t* i = info->canonicalPtr;
        for(kcoverset_t::iterator it = i->covers.begin(); it != i->covers.end(); it++) {
            kcover_t* kc = *it;
            bitslice_t* bt = new bitslice_t(kc);
            demux416Info_t dmx = { bt, tag };
            assert(bt->xs.size() == 5);
            node5tup_t inputs(bt->xs);
            if(tag == 0) {
                demuxes[inputs].push_back(dmx);
            } else {
                if(demuxes.find(inputs) != demuxes.end()) {
                    demuxes[inputs].push_back(dmx);
                } else {
                    delete bt;
                }
            }
        }
    }
    namespace {
        bool compareDecoder24Info(const decoder24Info_t& d1, const decoder24Info_t& d2)
        {
            return d1.tag < d2.tag;
        }
        bool findTag(decoder24list_t& dlist, int tag) {
            for(unsigned i=0; i != dlist.size(); i++) {
                if(dlist[i].tag == tag) return true;
            }
            return false;
        }
    }

    void identify24Decoders(flat_module_t* module)
    {
        std::cout << "identifying 2:4 decoders." << std::endl;

        if(options.kcoverSize < 2) return;
        input_provider_t* ipp = module->get_ipp(2);

        BDD d00 = decoder24i00(ipp);
        BDD d01 = decoder24i01(ipp);
        BDD d10 = decoder24i10(ipp);
        BDD d11 = decoder24i11(ipp);
        BDD d00n = !decoder24i00(ipp);
        BDD d01n = !decoder24i01(ipp);
        BDD d10n = !decoder24i10(ipp);
        BDD d11n = !decoder24i11(ipp);
        
        fnInfo_t* i00 = module->getFunction(d00);
        fnInfo_t* i01 = module->getFunction(d01);
        fnInfo_t* i10 = module->getFunction(d10);
        fnInfo_t* i11 = module->getFunction(d11);
        fnInfo_t* i00n = module->getFunction(d00n);
        fnInfo_t* i01n = module->getFunction(d01n);
        fnInfo_t* i10n = module->getFunction(d10n);
        fnInfo_t* i11n = module->getFunction(d11n);

        decoder24map_t decoders;
        add24Decoders(i00, 0, decoders);
        add24Decoders(i00n, 1, decoders);
        add24Decoders(i01, 2, decoders);
        add24Decoders(i01n, 3, decoders);
        assert(i01 == NULL || i10 == NULL || i01->canonicalPtr == i10->canonicalPtr);
        assert(i01n == NULL || i10n == NULL || i01n->canonicalPtr == i10n->canonicalPtr);
        add24Decoders(i11, 4, decoders);
        add24Decoders(i11n, 5, decoders);

        for(decoder24map_t::iterator it =  decoders.begin();
                                   it != decoders.end();
                                   it++)
        {
            const nodepair_t& p = it->first;
            node_t* a = p.first;
            node_t* b = p.second;

            std::sort(it->second.begin(), it->second.end(), compareDecoder24Info);

            int f = 0;
            for(int i=0; i <= 2; i++) {
                  f += ((findTag(it->second, 2*i) ? 1 : 0) || (findTag(it->second, 2*i+1) ? 1 : 0));
            }

            // create a module and shove into the list of identified modules.
            if(f >= 2) {
                id_module_t* mod = new id_module_t("decoder24", id_module_t::SLICEABLE);
                mod->add_input("sel", a, false);
                mod->add_input("sel", b, false);
                for(decoder24list_t::iterator jt = it->second.begin(); jt != it->second.end(); jt++) {
                    mod->add_output(jt->slice->ys[0]);
                }
                mod->compute_internals();
                module->add_module(mod);
            }
        }
        for(decoder24map_t::iterator it = decoders.begin(); it != decoders.end(); it++) {
            decoder24list_t& l = it->second;
            for(unsigned i=0; i != l.size(); i++) {
                delete l[i].slice;
            }
        }
    }

    mux_selinfo_t mux_selinfo[] = {
        { "mux21", {0, 1, -1} },
        { "mux21i", {0, 1, -1} },
        { "mux21_ao22", {1, 2, -1} },
        { "mux21_aoi22", {1, 2, -1} },
        { "mux21_oa22", {1, 2, -1} },
        { "mux21_oai22", {1, 2, -1} }
    };

    void id_module_t::computePinNames()
    {
        pin_map.clear();

        if(strcmp(type, "decoder24") == 0  || strcmp(type, "decoder38") == 0 ||
           strcmp(type, "decoder416") == 0 || strcmp(type, "decoder532") == 0 ||
           strcmp(type, "decoder664") == 0)
        {
            computePinNamesDecoders();
        }
        else if(strstr(type, "demux") == type) {
            computePinNamesDemux();
        } else {
            int numTypes = sizeof(mux_selinfo) / sizeof(mux_selinfo[0]);
            for(int i=0; i != numTypes; i++) {
                if(strcmp(mux_selinfo[i].type, type) == 0) {
                    computePinNamesMuxes(i);
                    break;
                }
            }
        }
    }

    void id_module_t::computePinNamesMuxes(int index)
    {
        assert(strcmp(mux_selinfo[index].type, type) == 0);
        std::string sel("sel");

        node_groups_t::iterator sel_iter = node_groups.find(sel);
        assert(sel_iter != node_groups.end());
        nodelist_t& selnodes = sel_iter->second;

        Cudd cudd;
        fn_map_t var_map;
        fn_rev_map_t rev_map;
        std::vector<node_t*> output_nodes;
        std::vector<BDD> output_fns;

        createFullFunctions(cudd, var_map, rev_map, output_nodes, output_fns);
        assert(output_nodes.size() == output_fns.size());


        nodeset_t covered_inputs;
        int sel_i;
        for(int i = 0; (sel_i = mux_selinfo[index].signals[i]) != -1; i++) {
            char inputname[16];
            for(unsigned j=0; j != output_nodes.size(); j++) {
                node_t* inp_ij = evalBDD( cudd, var_map, rev_map, selnodes, sel_i, output_fns[j]);
                assert(inp_ij != NULL);
                sprintf(inputname, "%c%d", (char)('A'+(int)i), (int)j);
                pin_map[inp_ij] = std::string(inputname);
            }
        }

        for(unsigned j=0; j != output_nodes.size(); j++) {
            char outputname[16];
            sprintf(outputname, "Y%d", (int)j);
            pin_map[output_nodes[j]] = std::string(outputname);
        }

        for(unsigned i=0; i != selnodes.size(); i++) {
            char selname[16];
            sprintf(selname, "S%d", i);
            pin_map[selnodes[i]] = std::string(selname);
        }

        var_map.clear();
        rev_map.clear();
        output_fns.clear();
    }


    void id_module_t::computePinNamesDecoders()
    {
        assert(strcmp(type, "decoder") != 0 && strstr(type, "decoder") == type);

        assert(num_inputs() > 0);
        assert(num_inputs() < MAX_DECODER_SIZE);

        flat_module_t *flat = inputs[0]->get_module();
        Cudd& cudd = flat->getFullFnMgr();

        int sz = num_inputs();
        assert(decFns[sz].size() == decnegFns[sz].size());
        if(decFns[sz].size() == 0) computeDecoderBDDs(sz, cudd);
        assert(decFns[sz].size() > 0 && decnegFns[sz].size() == decFns[sz].size());


        fn_map_t var_map;
        fn_rev_map_t rev_map;
        std::vector<node_t*> output_nodes;
        std::vector<BDD> output_fns;

        createFullFunctions(cudd, var_map, rev_map, output_nodes, output_fns);
        assert(output_nodes.size() == output_fns.size());
        char outputname[16];
        for(unsigned i=0; i != output_nodes.size(); i++) {
            bool found = false;
            for(unsigned j=0; j != decFns[sz].size(); j++) {
                if(output_fns[i] == decFns[sz][j]) {
                    found = true;
                    sprintf(outputname, "Y%d", i);
                    break;
                } else if(output_fns[i] == decnegFns[sz][j]) {
                    found = true;
                    sprintf(outputname, "YN%d", i);
                    break;
                }
            }
            assert(found);
            pin_map[output_nodes[i]] = std::string(outputname);
        }
        for(unsigned i=0; i != inputs.size(); i++) {
            char inputname[16];
            sprintf(inputname, "X%d", i);
            pin_map[inputs[i]] = std::string(inputname);
        }

        output_fns.clear();
        var_map.clear();
        rev_map.clear();
    }

    void id_module_t::computePinNamesDemux()
    {
        assert(strstr(type, "demux") == type);

        assert(num_inputs() > 0);
        assert(num_inputs() < MAX_DECODER_SIZE);

        flat_module_t *flat = inputs[0]->get_module();
        Cudd& cudd = flat->getFullFnMgr();

        int sz = num_inputs();
        assert(decFns[sz].size() == decnegFns[sz].size());
        if(decFns[sz].size() == 0) computeDecoderBDDs(sz, cudd);
        assert(decFns[sz].size() > 0 && decnegFns[sz].size() == decFns[sz].size());

        fn_map_t var_map;
        fn_rev_map_t rev_map;
        std::vector<node_t*> output_nodes;
        std::vector<BDD> output_fns;

        createFullFunctions(cudd, var_map, rev_map, output_nodes, output_fns);
        assert(output_nodes.size() == output_fns.size());
        std::vector<unsigned>  outputs_index;
        std::vector<bool> outputs_neg;
        for(unsigned i=0; i != output_nodes.size(); i++) {
            bool found = false;
            for(unsigned j=0; j != decFns[sz].size(); j++) {
                bool neg = false;
                if(output_fns[i] == decFns[sz][j] || (neg = (output_fns[i] == decnegFns[sz][j]))) {
                    found = true;
                    outputs_index.push_back(j);
                    outputs_neg.push_back(neg);
                    break;
                }
            }
            assert(found);
        }

        // compute the var that's being demultiplexed.
        int var = computeDemuxedVar(sz, outputs_index);
        if(var == -1) { set_bad(); return; }
        /*
        if(var == -1) {
            for(unsigned i=0; i != output_nodes.size(); i++) {
                std::cout << "node: " << output_nodes[i]->get_name() << "; "
                          << "index: " << outputs_index[i] << "; ";
                          
                printBDD(stdout, output_fns[i]);
            }
            return;
        }
        */

        assert(outputs_index.size() == output_nodes.size());
        for(unsigned i=0; i != output_nodes.size(); i++) {
            int idx = outputs_index[i];
            assert(idx >= 0 && idx < (1 << (sz-1)));
            char outname[16];
            if(outputs_neg[i]) {
                sprintf(outname, "Y%d", idx);
            } else {
                sprintf(outname, "Y%d", idx);
            }
            pin_map[output_nodes[i]] = std::string(outname);
        }

        // FIXME: remove the next four lines when done.
        std::ostringstream ostr;
        ostr << "demux var: " << var;
        std::string str(ostr.str());
        add_comment(str);

        assert(var >=0 && var < (int) inputs.size());
        int pos = 0;
        for(unsigned i=0; i != inputs.size(); i++) {
            char inputname[16];
            if((int) i == var) {
                sprintf(inputname, "A");
            } else {
                sprintf(inputname, "B%d", pos++);
            }
            pin_map[inputs[i]] = std::string(inputname);
        }

        output_fns.clear();
        var_map.clear();
        rev_map.clear();
    }

    unsigned removeBit(unsigned bit, unsigned numBits, unsigned val)
    {
        int pos = 0;
        unsigned new_val = 0;
        for(unsigned i=0; i != numBits; i++) {
            if(i == bit) continue;
            else {
                int in_mask = 1 << i;
                int out_mask = ((in_mask & val) != 0) ? (1 << pos) : 0;
                new_val = new_val | out_mask;
                pos ++;
            }
        }
        return new_val;
    }

    int id_module_t::computeDemuxedVar(int vars, std::vector<unsigned>& outs)
    {
        int demuxVar = -1;
        for(int i = 0; i < vars; i++) {
            bool good = true;
            unsigned mask = 1 << i;
            for(std::vector<unsigned>::iterator it = outs.begin(); it != outs.end(); it++) {
                unsigned num = *it;
                if((mask & num) == 0) {
                    good = false;
                    break;
                }
            }
            if(good) {
                demuxVar = i;
                break;
            }
        }
        if(demuxVar == -1) { return demuxVar; }

        for(unsigned i = 0; i != outs.size(); i++) {
            outs[i] = removeBit(demuxVar, vars, outs[i]);
        }

        return demuxVar;
    }

    void id_module_t::computeDecoderBDDs(int sz, Cudd& mgr)
    {
        assert(sz > 0 && sz < MAX_DECODER_SIZE);
        int max = 1 << sz;

        for(int i=0; i != max; i++) {
            BDD ci = computeCube(sz, i, mgr);
            decFns[sz].push_back(ci);
            decnegFns[sz].push_back(!ci);
        }
    }

    void cleanupAfterPinNameComputation() 
    {
        std::cout << "cleaning up" << std::endl;

        for(unsigned i=0; i != MAX_DECODER_SIZE; i++) {
            decFns[i].clear();
        }
        for(unsigned i=0; i != MAX_DECODER_SIZE; i++) {
            decnegFns[i].clear();
        }

        std::cout << "finished cleaning up" << std::endl;
    }

    BDD id_module_t::computeCube(int sz, int value, Cudd& mgr)
    {
        BDD c = mgr.bddOne();
        for(int i = 0; i != sz; i++) {
            int mask = 1 << i;
            BDD vi = mgr.bddVar(i);
            if( (mask & value) == 0 ) {
                c = c & !vi;
            } else {
                c = c & vi;
            }
        }
        return c;
    }

    namespace {
        bool compareDecoder38Info(const decoder38Info_t& d1, const decoder38Info_t& d2)
        {
            return d1.tag < d2.tag;
        }
        bool findTag(decoder38list_t& dlist, int tag) {
            for(unsigned i=0; i != dlist.size(); i++) {
                if(dlist[i].tag == tag) return true;
            }
            return false;
        }
    }

    void identify38Decoders(flat_module_t* module)
    {
        std::cout << "identifying 3:8 decoders." << std::endl;

        if(options.kcoverSize < 3) return;
        input_provider_t* ipp = module->get_ipp(3);

        BDD d0 = decoder38i0(ipp); BDD d0n = !decoder38i0(ipp);
        BDD d1 = decoder38i1(ipp); BDD d1n = !decoder38i1(ipp);
        BDD d2 = decoder38i2(ipp); BDD d2n = !decoder38i2(ipp);
        BDD d3 = decoder38i3(ipp); BDD d3n = !decoder38i3(ipp);
        
        fnInfo_t* i0 = module->getFunction(d0); fnInfo_t* i0n = module->getFunction(d0n);
        fnInfo_t* i1 = module->getFunction(d1); fnInfo_t* i1n = module->getFunction(d1n);
        fnInfo_t* i2 = module->getFunction(d2); fnInfo_t* i2n = module->getFunction(d2n);
        fnInfo_t* i3 = module->getFunction(d3); fnInfo_t* i3n = module->getFunction(d3n);

        decoder38map_t decoders;
        add38Decoders(i0, 0, decoders); add38Decoders(i0n, 0, decoders);
        add38Decoders(i1, 1, decoders); add38Decoders(i1n, 1, decoders);
        add38Decoders(i2, 2, decoders); add38Decoders(i2n, 2, decoders);
        add38Decoders(i3, 3, decoders); add38Decoders(i3n, 3, decoders);

        for(decoder38map_t::iterator it = decoders.begin();
                                     it != decoders.end();
                                     it++)
        {
            const nodetriple_t& p = it->first;
            node_t* a = p.a;
            node_t* b = p.b;
            node_t* c = p.c;

            std::sort(it->second.begin(), it->second.end(), compareDecoder38Info);

            int f0 = findTag(it->second, 0) ? 1 : 0;
            int f1 = findTag(it->second, 1) ? 1 : 0;
            int f2 = findTag(it->second, 2) ? 1 : 0;
            int f3 = findTag(it->second, 3) ? 1 : 0;
            int f = f0 + f1 + f2 + f3;

            // create a module and shove into the list of identified modules.
            if(f >= 2) {
                id_module_t* mod = new id_module_t("decoder38", id_module_t::SLICEABLE);
                mod->add_input("sel", a, false);
                mod->add_input("sel", b, false);
                mod->add_input("sel", c, false);
                for(decoder38list_t::iterator jt = it->second.begin(); jt != it->second.end(); jt++) {
                    mod->add_output(jt->slice->ys[0]);
                }
                mod->compute_internals();
                module->add_module(mod);
            }
        }
        for(decoder38map_t::iterator it = decoders.begin(); it != decoders.end(); it++) {
            decoder38list_t& l = it->second;
            for(unsigned i=0; i != l.size(); i++) {
                delete l[i].slice;
            }
        }
    }

    namespace {
        bool compareDecoder416Info(const decoder416Info_t& d1, const decoder416Info_t& d2)
        {
            return d1.tag < d2.tag;
        }
        bool findTag(decoder416list_t& dlist, int tag) {
            for(unsigned i=0; i != dlist.size(); i++) {
                if(dlist[i].tag == tag) return true;
            }
            return false;
        }
    }

    void identify416Decoders(flat_module_t* module)
    {
        std::cout << "identify 4:16 decoders: " << std::endl;

        if(options.kcoverSize < 4) return;
        input_provider_t* ipp = module->get_ipp(4);

        BDD d0 = decoder416i0(ipp); BDD d0n = !decoder416i0(ipp);
        BDD d1 = decoder416i1(ipp); BDD d1n = !decoder416i1(ipp);
        BDD d2 = decoder416i2(ipp); BDD d2n = !decoder416i2(ipp);
        BDD d3 = decoder416i3(ipp); BDD d3n = !decoder416i3(ipp);
        BDD d4 = decoder416i4(ipp); BDD d4n = !decoder416i4(ipp);
        
        fnInfo_t* i0 = module->getFunction(d0); fnInfo_t* i0n = module->getFunction(d0n);
        fnInfo_t* i1 = module->getFunction(d1); fnInfo_t* i1n = module->getFunction(d1n);
        fnInfo_t* i2 = module->getFunction(d2); fnInfo_t* i2n = module->getFunction(d2n);
        fnInfo_t* i3 = module->getFunction(d3); fnInfo_t* i3n = module->getFunction(d3n);
        fnInfo_t* i4 = module->getFunction(d4); fnInfo_t* i4n = module->getFunction(d4n);

        decoder416map_t decoders;
        add416Decoders(i0, 0, decoders); add416Decoders(i0n, 0, decoders);
        add416Decoders(i1, 1, decoders); add416Decoders(i1n, 1, decoders);
        add416Decoders(i2, 2, decoders); add416Decoders(i2n, 2, decoders);
        add416Decoders(i3, 3, decoders); add416Decoders(i3n, 3, decoders);
        add416Decoders(i4, 4, decoders); add416Decoders(i4n, 4, decoders);

        for(decoder416map_t::iterator it = decoders.begin();
                                      it != decoders.end();
                                      it++)
        {
            const nodequad_t& p = it->first;
            node_t* a = p.a;
            node_t* b = p.b;
            node_t* c = p.c;
			node_t* d = p.d;

            std::sort(it->second.begin(), it->second.end(), compareDecoder416Info);

            int f0 = findTag(it->second, 0) ? 1 : 0;
            int f1 = findTag(it->second, 1) ? 1 : 0;
            int f2 = findTag(it->second, 2) ? 1 : 0;
            int f3 = findTag(it->second, 3) ? 1 : 0;
            int f = f0 + f1 + f2 + f3;

            // create a module and shove into the list of identified modules.
            if(f >= 2) {
                id_module_t* mod = new id_module_t("decoder416", id_module_t::SLICEABLE);
                mod->add_input("sel", a, false);
                mod->add_input("sel", b, false);
                mod->add_input("sel", c, false);
				mod->add_input("sel", d, false);
                for(decoder416list_t::iterator jt = it->second.begin(); jt != it->second.end(); jt++) {
                    mod->add_output(jt->slice->ys[0]);
                }
                mod->compute_internals();
                module->add_module(mod);
            }
        }
        for(decoder416map_t::iterator it = decoders.begin(); it != decoders.end(); it++) {
            decoder416list_t& l = it->second;
            for(unsigned i=0; i != l.size(); i++) {
                delete l[i].slice;
            }
        }
    }
    namespace {
	bool compareDecoder532Info(const decoder532Info_t& d1, const decoder532Info_t& d2)
	{
		return d1.tag < d2.tag;
	}
	bool findTag(decoder532list_t& dlist, int tag) {
		for(unsigned i=0; i != dlist.size(); i++) {
			if(dlist[i].tag == tag) return true;
		}
		return false;
	}
    }

    void identify532Decoders(flat_module_t* module)
    {
        std::cout << "identify 5:32 decoders: " <<  std::endl;

        if(options.kcoverSize < 5) return;
        input_provider_t* ipp = module->get_ipp(5);

        BDD d0 = decoder532i0(ipp); BDD d0n = !decoder532i0(ipp);
        BDD d1 = decoder532i1(ipp); BDD d1n = !decoder532i1(ipp);
        BDD d2 = decoder532i2(ipp); BDD d2n = !decoder532i2(ipp);
        BDD d3 = decoder532i3(ipp); BDD d3n = !decoder532i3(ipp);
        BDD d4 = decoder532i4(ipp); BDD d4n = !decoder532i4(ipp);
        BDD d5 = decoder532i5(ipp); BDD d5n = !decoder532i5(ipp);

        fnInfo_t* i0 = module->getFunction(d0); fnInfo_t* i0n = module->getFunction(d0n);
        fnInfo_t* i1 = module->getFunction(d1); fnInfo_t* i1n = module->getFunction(d1n);
        fnInfo_t* i2 = module->getFunction(d2); fnInfo_t* i2n = module->getFunction(d2n);
        fnInfo_t* i3 = module->getFunction(d3); fnInfo_t* i3n = module->getFunction(d3n);
        fnInfo_t* i4 = module->getFunction(d4); fnInfo_t* i4n = module->getFunction(d4n);
        fnInfo_t* i5 = module->getFunction(d5); fnInfo_t* i5n = module->getFunction(d5n);

        decoder532map_t decoders;
        add532Decoders(i0, 0, decoders); add532Decoders(i0n, 0, decoders);
        add532Decoders(i1, 1, decoders); add532Decoders(i1n, 1, decoders);
        add532Decoders(i2, 2, decoders); add532Decoders(i2n, 2, decoders);
        add532Decoders(i3, 3, decoders); add532Decoders(i3n, 3, decoders);
        add532Decoders(i4, 4, decoders); add532Decoders(i4n, 4, decoders);
        add532Decoders(i5, 5, decoders); add532Decoders(i5n, 5, decoders);

        for(decoder532map_t::iterator it = decoders.begin(); it != decoders.end(); it++ ) {
            const node5tup_t& p = it->first;
            node_t* a = p.a;
            node_t* b = p.b;
            node_t* c = p.c;
            node_t* d = p.d;
            node_t* e = p.e;

            std::sort(it->second.begin(), it->second.end(), compareDecoder532Info); 

            int f0 = findTag(it->second, 0) ? 1 : 0;
            int f1 = findTag(it->second, 1) ? 1 : 0;
            int f2 = findTag(it->second, 2) ? 1 : 0;
            int f3 = findTag(it->second, 3) ? 1 : 0;
            int f4 = findTag(it->second, 4) ? 1 : 0;
            int f = f0 + f1 + f2 + f3 + f4;

            //Create a module
            if (f >= 2) {
                id_module_t* mod = new id_module_t("decoder532", id_module_t::SLICEABLE); 
                mod->add_input("sel", a, false);
                mod->add_input("sel", b, false);
                mod->add_input("sel", c, false);
                mod->add_input("sel", d, false);
                mod->add_input("sel", e, false);

                for(decoder532list_t::iterator jt = it->second.begin(); jt != it->second.end(); jt++) { 
                    mod->add_output(jt->slice->ys[0]);
                }
                mod->compute_internals();
                module->add_module(mod);

            }
        }
        for (decoder532map_t::iterator it = decoders.begin(); it != decoders.end(); it++) {
            decoder532list_t& l = it->second;
            for(unsigned i=0; i != l.size(); i++) {
                delete l[i].slice;
            }
        }
    }


    namespace {
	bool compareDecoder664Info(const decoder664Info_t& d1, const decoder664Info_t& d2)
	{
		return d1.tag < d2.tag;
	}
	bool findTag(decoder664list_t& dlist, int tag) {
		for(unsigned i=0; i != dlist.size(); i++) {
			if(dlist[i].tag == tag) return true;
		}
		return false;
	}
    }

    void identify664Decoders(flat_module_t* module)
    {
        std::cout << "identify 6:64 decoders: " <<  std::endl;

        if(options.kcoverSize < 6) return;
        input_provider_t* ipp = module->get_ipp(6);

        BDD d0 = decoder664i0(ipp); BDD d0n = !decoder664i0(ipp);
        BDD d1 = decoder664i1(ipp); BDD d1n = !decoder664i1(ipp);
        BDD d2 = decoder664i2(ipp); BDD d2n = !decoder664i2(ipp);
        BDD d3 = decoder664i3(ipp); BDD d3n = !decoder664i3(ipp);
        BDD d4 = decoder664i4(ipp); BDD d4n = !decoder664i4(ipp);
        BDD d5 = decoder664i5(ipp); BDD d5n = !decoder664i5(ipp);
        BDD d6 = decoder664i6(ipp); BDD d6n = !decoder664i6(ipp);

        fnInfo_t* i0 = module->getFunction(d0); fnInfo_t* i0n = module->getFunction(d0n);
        fnInfo_t* i1 = module->getFunction(d1); fnInfo_t* i1n = module->getFunction(d1n);
        fnInfo_t* i2 = module->getFunction(d2); fnInfo_t* i2n = module->getFunction(d2n);
        fnInfo_t* i3 = module->getFunction(d3); fnInfo_t* i3n = module->getFunction(d3n);
        fnInfo_t* i4 = module->getFunction(d4); fnInfo_t* i4n = module->getFunction(d4n);
        fnInfo_t* i5 = module->getFunction(d5); fnInfo_t* i5n = module->getFunction(d5n);
        fnInfo_t* i6 = module->getFunction(d6); fnInfo_t* i6n = module->getFunction(d6n);

        decoder664map_t decoders;

        add664Decoders(i0, 0, decoders); add664Decoders(i0n, 0, decoders);
        add664Decoders(i1, 1, decoders); add664Decoders(i1n, 1, decoders);
        add664Decoders(i2, 2, decoders); add664Decoders(i2n, 2, decoders);
        add664Decoders(i3, 3, decoders); add664Decoders(i3n, 3, decoders);
        add664Decoders(i4, 4, decoders); add664Decoders(i4n, 4, decoders);
        add664Decoders(i5, 5, decoders); add664Decoders(i5n, 5, decoders);
        add664Decoders(i6, 6, decoders); add664Decoders(i6n, 6, decoders);

        for(decoder664map_t::iterator it = decoders.begin(); it != decoders.end(); it++ ) {
            const node_tuple_t& p = it->first;
            node_t* a = p.nodes[0];
            node_t* b = p.nodes[1];
            node_t* c = p.nodes[2];
            node_t* d = p.nodes[3];
            node_t* e = p.nodes[4];
            node_t* f = p.nodes[5];

            std::sort(it->second.begin(), it->second.end(), compareDecoder664Info); 

            int f0 = findTag(it->second, 0) ? 1 : 0;
            int f1 = findTag(it->second, 1) ? 1 : 0;
            int f2 = findTag(it->second, 2) ? 1 : 0;
            int f3 = findTag(it->second, 3) ? 1 : 0;
            int f4 = findTag(it->second, 4) ? 1 : 0;
            int f5 = findTag(it->second, 5) ? 1 : 0;
            int fx = f0 + f1 + f2 + f3 + f4 + f5;

            //Create a module
            if (fx >= 2) {
                id_module_t* mod = new id_module_t("decoder664", id_module_t::SLICEABLE); 
                mod->add_input("sel", a, false);
                mod->add_input("sel", b, false);
                mod->add_input("sel", c, false);
                mod->add_input("sel", d, false);
                mod->add_input("sel", e, false);
                mod->add_input("sel", f, false);

                for(decoder664list_t::iterator jt = it->second.begin(); jt != it->second.end(); jt++) { 
                    mod->add_output(jt->slice->ys[0]);
                }
                mod->compute_internals();
                module->add_module(mod);

            }
        }
        for (decoder664map_t::iterator it = decoders.begin(); it != decoders.end(); it++) {
            decoder664list_t& l = it->second;
            for(unsigned i=0; i != l.size(); i++) {
                delete l[i].slice;
            }
        }

    }

    BDD demux24i0(input_provider_t* e) { return e->inp(0) & !e->inp(1) & !e->inp(2); }
    BDD demux24i1(input_provider_t* e) { return e->inp(0) & !e->inp(1) &  e->inp(2); }
    BDD demux24i2(input_provider_t* e) { return e->inp(0) &  e->inp(1) & !e->inp(2); }
    BDD demux24i3(input_provider_t* e) { return e->inp(0) &  e->inp(1) &  e->inp(2); }

    void identify24Demuxes(flat_module_t* module)
    {
        std::cout << "identifying 2:4 demuxes." << std::endl;

        if(options.kcoverSize < 3) return;
        input_provider_t* ipp = module->get_ipp(3);

        BDD d0 = demux24i0(ipp); BDD d0n = !demux24i0(ipp);
        BDD d1 = demux24i1(ipp); BDD d1n = !demux24i1(ipp);
        BDD d2 = demux24i2(ipp); BDD d2n = !demux24i2(ipp);
        BDD d3 = demux24i3(ipp); BDD d3n = !demux24i3(ipp);

        fnInfo_t* i0 = module->getFunction(d0); fnInfo_t* i0n = module->getFunction(d0);
        fnInfo_t* i1 = module->getFunction(d1); fnInfo_t* i1n = module->getFunction(d1);
        fnInfo_t* i2 = module->getFunction(d2); fnInfo_t* i2n = module->getFunction(d2);
        fnInfo_t* i3 = module->getFunction(d3); fnInfo_t* i3n = module->getFunction(d3);

        assert(i2 == NULL || i1 == NULL || i1->canonicalPtr == i2->canonicalPtr);
        assert(i2n == NULL || i1 == NULL || i1->canonicalPtr == i2n->canonicalPtr);
        demux24map_t demuxes;
        add24Demuxes(i0, 0, demuxes); add24Demuxes(i0n, 0, demuxes);
        add24Demuxes(i1, 1, demuxes); add24Demuxes(i1n, 1, demuxes);
        add24Demuxes(i3, 3, demuxes); add24Demuxes(i3n, 3, demuxes);

        for(demux24map_t::iterator it = demuxes.begin(); it != demuxes.end(); it++) {
            demux24list_t& l = it->second;
            std::map<int,int> tagCounts;
            for(unsigned i=0; i != l.size(); i++) {
                demux24Info_t& d = l[i];
                tagCounts[d.tag] += 1;
            }
            int f = (tagCounts[0] > 0) + (tagCounts[1] > 0) + (tagCounts[3] > 0);
            bool found = f >= 2;
            if(found) {
                id_module_t* mod = new id_module_t("demux24", id_module_t::SLICEABLE);
                mod->add_input("sel", it->first.a, false);
                mod->add_input("sel", it->first.b, false);
                mod->add_input("sel", it->first.c, false);
                for(unsigned i=0; i != l.size(); i++) {
                    demux24Info_t& d = l[i];
                    mod->add_output(d.slice->ys[0]);
                }
                mod->compute_internals();
                mod->computePinNames();
                if(mod->is_marked_bad()) delete mod;
                else {
                    module->add_module(mod);
                }
            }
        }
        for(demux24map_t::iterator it = demuxes.begin(); it != demuxes.end(); it++) {
            demux24list_t& l = it->second;
            for(unsigned i=0; i != l.size(); i++) {
                delete l[i].slice;
            }
        }
    }

    void identify38Demuxes(flat_module_t* module)
    {
        std::cout << "identifying 3:8 demuxes." << std::endl;

        if(options.kcoverSize < 4) return;
        input_provider_t* ipp = module->get_ipp(4);

        BDD d0 = demux38i0(ipp); BDD d0n = !demux38i0(ipp);
        BDD d1 = demux38i1(ipp); BDD d1n = !demux38i1(ipp);
        BDD d2 = demux38i2(ipp); BDD d2n = !demux38i2(ipp);
        BDD d3 = demux38i3(ipp); BDD d3n = !demux38i3(ipp);

        fnInfo_t* i0 = module->getFunction(d0); fnInfo_t* i0n = module->getFunction(d0n);
        fnInfo_t* i1 = module->getFunction(d1); fnInfo_t* i1n = module->getFunction(d1n);
        fnInfo_t* i2 = module->getFunction(d2); fnInfo_t* i2n = module->getFunction(d2n);
        fnInfo_t* i3 = module->getFunction(d3); fnInfo_t* i3n = module->getFunction(d3n);

        demux38map_t demuxes;
        add38Demuxes(i0, 0, demuxes); add38Demuxes(i0n, 0, demuxes);
        add38Demuxes(i1, 1, demuxes); add38Demuxes(i1n, 1, demuxes);
        add38Demuxes(i2, 2, demuxes); add38Demuxes(i2n, 2, demuxes);
        add38Demuxes(i3, 3, demuxes); add38Demuxes(i3n, 3, demuxes);

        for(demux38map_t::iterator it = demuxes.begin(); it != demuxes.end(); it++) {
            demux38list_t& l = it->second;
            std::map<int,int> tagCounts;
            for(unsigned i=0; i != l.size(); i++) {
                demux38Info_t& d = l[i];
                tagCounts[d.tag] += 1;
            }
            int f = (tagCounts[0] > 0) + (tagCounts[1] > 0) + (tagCounts[2] > 0) + (tagCounts[3] > 0);
            bool found = f >= 2;
            if(found) {
                id_module_t* mod = new id_module_t("demux38", id_module_t::SLICEABLE);
                mod->add_input("sel", it->first.a, false);
                mod->add_input("sel", it->first.b, false);
                mod->add_input("sel", it->first.c, false);
                mod->add_input("sel", it->first.d, false);
                for(unsigned i=0; i != l.size(); i++) {
                    demux38Info_t& d = l[i];
                    mod->add_output(d.slice->ys[0]);
                }
                mod->compute_internals();
                mod->computePinNames();
                if(mod->is_marked_bad()) delete mod;
                else {
                    module->add_module(mod);
                }
            }
        }
        for(demux38map_t::iterator it = demuxes.begin(); it != demuxes.end(); it++) {
            demux38list_t& l = it->second;
            for(unsigned i=0; i != l.size(); i++) {
                delete l[i].slice;
            }
        }
    }

    void identify416Demuxes(flat_module_t* module)
    {
        std::cout << "identifying 4:16 demuxes." << std::endl;

        if(options.kcoverSize < 5) return;
        input_provider_t* ipp = module->get_ipp(5);

        BDD d0 = demux416i0(ipp); BDD d0n = !demux416i0(ipp);
        BDD d1 = demux416i1(ipp); BDD d1n = !demux416i1(ipp);
        BDD d2 = demux416i2(ipp); BDD d2n = !demux416i2(ipp);
        BDD d3 = demux416i3(ipp); BDD d3n = !demux416i3(ipp);
        BDD d4 = demux416i4(ipp); BDD d4n = !demux416i4(ipp);
        fnInfo_t* i0 = module->getFunction(d0); fnInfo_t* i0n = module->getFunction(d0);
        fnInfo_t* i1 = module->getFunction(d1); fnInfo_t* i1n = module->getFunction(d1);
        fnInfo_t* i2 = module->getFunction(d2); fnInfo_t* i2n = module->getFunction(d2);
        fnInfo_t* i3 = module->getFunction(d3); fnInfo_t* i3n = module->getFunction(d3);
        fnInfo_t* i4 = module->getFunction(d4); fnInfo_t* i4n = module->getFunction(d4);

        demux416map_t demuxes;
        add416Demuxes(i0, 0, demuxes); add416Demuxes(i0n, 0, demuxes);
        add416Demuxes(i1, 1, demuxes); add416Demuxes(i1n, 1, demuxes);
        add416Demuxes(i2, 2, demuxes); add416Demuxes(i2n, 2, demuxes);
        add416Demuxes(i3, 3, demuxes); add416Demuxes(i3n, 3, demuxes);
        add416Demuxes(i4, 4, demuxes); add416Demuxes(i4n, 4, demuxes);

        for(demux416map_t::iterator it = demuxes.begin(); it != demuxes.end(); it++) {
            demux416list_t& l = it->second;
            std::map<int,int> tagCounts;
            for(unsigned i=0; i != l.size(); i++) {
                demux416Info_t& d = l[i];
                tagCounts[d.tag] += 1;
            }
            int f = (tagCounts[0] > 0) + (tagCounts[1] > 0) + (tagCounts[2] > 0) + (tagCounts[3] > 0) + (tagCounts[4] > 0);
            bool found = f >= 2;
            if(found) {
                id_module_t* mod = new id_module_t("demux416", id_module_t::SLICEABLE);
                mod->add_input("sel", it->first.a, false);
                mod->add_input("sel", it->first.b, false);
                mod->add_input("sel", it->first.c, false);
                mod->add_input("sel", it->first.d, false);
                mod->add_input("sel", it->first.e, false);

                for(unsigned i=0; i != l.size(); i++) {
                    demux416Info_t& d = l[i];
                    mod->add_output(d.slice->ys[0]);
                }
                mod->compute_internals();
                mod->computePinNames();
                if(mod->is_marked_bad()) delete mod;
                else {
                    module->add_module(mod);
                }
            }
        }
        for(demux416map_t::iterator it = demuxes.begin(); it != demuxes.end(); it++) {
            demux416list_t& l = it->second;
            for(unsigned i=0; i != l.size(); i++) {
                delete l[i].slice;
            }
        }
    }

    void identifyMuxes(flat_module_t* module)
    {
        std::cout << "identifying 2:1 muxes. " << std::endl;
        if(options.kcoverSize < 3) return;
        input_provider_t* ipp = module->get_ipp(3);
        BDD mux21BDD = mux21(ipp);
        BDD imux21BDD = !mux21BDD;

        fnInfo_t* muxInfo = module->getFunction(mux21BDD);
        if(muxInfo != NULL) {
            std::vector<uint8_t>& p = muxInfo->permutation;
            int sel_index = p[2]; assert(sel_index != -1);
            aggregateMuxes(module, muxInfo, "mux21", sel_index, p[0], p[1]);
        }

        muxInfo = module->getFunction(imux21BDD);
        if(muxInfo != NULL) {
            std::vector<uint8_t>& p = muxInfo->permutation;
            int sel_index = p[2]; assert(sel_index != -1);
            aggregateMuxes(module, muxInfo, "mux21i", sel_index, p[0], p[1]);
        }
    }

    void aggregateMuxes(flat_module_t* module, fnInfo_t* muxInfo, const char* muxName, int sel_index, int ai, int bi)
    {
        std::set<kcover_t*>& covers = muxInfo->canonicalPtr->covers;
        // list of all muxes.
        std::vector<bitslice_t*> muxes;
        // map between mux-selects and muxes.
        std::map< node_t*, std::vector<bitslice_t*> > sel_map;

        for(std::set<kcover_t*>::iterator it = covers.begin(); it != covers.end(); it++) {
            kcover_t* cover = *it;
            if(cover->get_root()->is_latch_gate()) continue;
            bitslice_t* bt = new bitslice_t(cover);
            muxes.push_back(bt);
            sel_map[bt->xs[sel_index]].push_back(bt);
        }

        for(std::map< node_t*, std::vector<bitslice_t*> >::iterator it = sel_map.begin();
                                                                    it != sel_map.end();
                                                                    it++)
        {
            node_t* sel = it->first;
            if(it->second.size() >= options.minMultibitElementSize) {
                word_t *w1, *w2, *w3;

                module->add_word(w1 = createInputWord(module, it->second, ai, 1));
                module->add_word(w2 = createInputWord(module, it->second, bi, 1));
                module->add_word(w3 = createOutputWord(module, it->second, 0, 1));
                
                if(w1 && w2 && w3) {
                    id_module_t* mod = new id_module_t(muxName, id_module_t::SLICEABLE);
                    std::string str_c("s");
                    mod->add_input_word(w1);
                    mod->add_input_word(w2);
                    mod->add_input("sel", sel, false);
                    mod->add_output_word(w3);
                    mod->compute_internals();
                    module->add_module(mod);
                }
            }
        }

        deleteSlices(muxes);
    }

    word_t* createInputWord(flat_module_t* module, std::vector<bitslice_t*>& bits, int pos, int th)
    {
        word_t* w = new word_t(false, word_t::KCOVER_ANALYSIS);
        for(std::vector<bitslice_t*>::iterator it = bits.begin();
                                               it != bits.end();
                                               it++)
        {
            bitslice_t* bt = *it;
            assert(pos < (int)bt->xs.size());
            node_t* n = bt->xs[pos];
            w->add_bit(n);
        }
        if((int)w->size() >= th) { return module->get_canonical_word(w); }
        else { delete w; return NULL; }
    }

    word_t* createOutputWord(flat_module_t* module, std::vector<bitslice_t*>& bits, int pos, int th)
    {
        word_t* w = new word_t(false, word_t::KCOVER_ANALYSIS);
        for(std::vector<bitslice_t*>::iterator it = bits.begin();
                                               it != bits.end();
                                               it++)
        {
            bitslice_t* bt = *it;
            assert(pos < (int)bt->ys.size());
            node_t* n = bt->ys[pos];
            w->add_bit(n);
        }
        if((int)w->size() >= th) { return module->get_canonical_word(w); }
        else { delete w; return NULL; }
    }

    BDD fa_sum(input_provider_t* e)
    {
        BDD a = e->inp(0);
        BDD b = e->inp(1);
        BDD c = e->inp(2);
        return a ^ b ^ c;
    }
    BDD fa_sumb(input_provider_t* e)
    {
        BDD a = !e->inp(0);
        BDD b = e->inp(1);
        BDD c = e->inp(2);
        return a ^ b ^ c;
    }

    BDD fa_carry(input_provider_t* e)
    {
        BDD a = e->inp(0);
        BDD b = e->inp(1);
        BDD c = e->inp(2);
        return (a&b) | (b&c) | (c&a);
    }
    BDD fa_carryb(input_provider_t* e)
    {
        BDD a = !e->inp(0);
        BDD b = e->inp(1);
        BDD c = e->inp(2);
        return (a&b) | (b&c) | (c&a);
    }

    BDD ha_sum(input_provider_t* e)
    {
        BDD a = e->inp(0);
        BDD b = e->inp(1);
        return a^b;
    }

    BDD ha_carry(input_provider_t* e)
    {
        BDD a = e->inp(0);
        BDD b = e->inp(1);
        return a&b;
    }

    void identifyRCAs(flat_module_t* module)
    {
        std::cout << "identifying ripple carry adders/subtractors." << std::endl;

        if(options.kcoverSize < 3) return;
        input_provider_t* ipp = module->get_ipp(3);
        BDD sumBDD = fa_sum(ipp);
        BDD isumBDD = !sumBDD;

        BDD carryBDD = fa_carry(ipp);
        BDD icarryBDD = !carryBDD;
        BDD carrybBDD = fa_carryb(ipp);
        BDD icarrybBDD = !carrybBDD;

        if(options.kcoverSize < 2) return;
        input_provider_t* ipp2 = module->get_ipp(2);
        BDD hasumBDD = ha_sum(ipp2);
        BDD ihasumBDD = !hasumBDD;
        BDD hacarryBDD = ha_carry(ipp2);
        BDD ihacarryBDD = !hacarryBDD;

        std::vector<bitslice_t*> sums;
        std::vector<bitslice_t*> carrys;

        // dump the list of sum covers (for debugging).

        // create the list of sum covers.
        if(module->getFunction(sumBDD)) addCovers(module->getFunction(sumBDD)->covers, sums);
        if(module->getFunction(isumBDD)) addCovers(module->getFunction(isumBDD)->covers, sums);
        // half-adder sums.
        if(module->getFunction(hasumBDD)) addCovers(module->getFunction(hasumBDD)->covers, sums);
        if(module->getFunction(ihasumBDD)) addCovers(module->getFunction(ihasumBDD)->covers, sums);

        // create the list of carry covers.
        if(module->getFunction(carryBDD)) addCovers(module->getFunction(carryBDD)->covers, carrys);
        if(module->getFunction(icarryBDD)) addCovers(module->getFunction(icarryBDD)->covers, carrys);
        if(module->getFunction(carrybBDD)) addCovers(module->getFunction(carrybBDD)->covers, carrys);
        if(module->getFunction(icarrybBDD)) addCovers(module->getFunction(icarrybBDD)->covers, carrys);
        // half-adder carrys.
        if(module->getFunction(hacarryBDD)) addCovers(module->getFunction(hacarryBDD)->covers, carrys);
        if(module->getFunction(ihacarryBDD)) addCovers(module->getFunction(ihacarryBDD)->covers, carrys);

        // findCarryChains(module, carrys, sums);
        findCarryChains2(module, carrys, sums);

        deleteSlices(sums);
        deleteSlices(carrys);
        sums.clear();
        carrys.clear();
    }

    void fill_vector(node_t* x, std::vector<node_t*>& xs)
    {
        flat_module_t* mod = x->get_module();
        const nodeset_t* s = mod->get_not_related(x);
        xs.push_back(x);
        if(s != NULL) {
            for(nodeset_t::const_iterator it = s->begin(); it != s->end(); it++) {
                xs.push_back(*it);
            }
        }
    }

    void find_matching_sums(sum_node_set_t& sums, bitslice_t* carry)
    {
        assert(carry->xs.size() == 3 || carry->xs.size() == 2);
        assert(carry->ys.size() == 1);

        std::vector<node_t*> as, bs, cs;
        fill_vector(carry->xs[0], as);
        fill_vector(carry->xs[1], bs);
        if(carry->xs.size() == 3) fill_vector(carry->xs[2], cs);

        for(unsigned i=0; i != as.size(); i++) {
            node_t* a = as[i];
            for(unsigned j=0; j != bs.size(); j++) {
                node_t* b = bs[j];
                if(carry->xs.size() == 3) {
                    for(unsigned k=0; k != cs.size(); k++) {
                        node_t* c = cs[k];
                        sum_node_t s(a, b, c);
                        sum_node_set_t::iterator pos;
                        if((pos = sums.find(s)) != sums.end()) {
                            bitslice_t* sum = pos->slice;
                            carry->partners.insert(sum);
                        }
                    }
                } else {
                    sum_node_t s(a, b);
                    sum_node_set_t::iterator pos = sums.find(s);
                    if(pos != sums.end()) {
                        bitslice_t* sum = pos->slice;
                        carry->partners.insert(sum);
                    }
                }
            }
        }
    }

    void findCarryChains2(flat_module_t* module, bitslice_list_t& carrys, bitslice_list_t& sums)
    {
        std::cout << "creating the mapping between carry and sum bitslices ..." << std::endl;
        sum_node_set_t sumset;
        for(unsigned i=0; i != sums.size(); i++) {
            bitslice_t* s = sums[i];
            sum_node_t sumnode(s);
            sumset.insert(sumnode);
        }
        for(unsigned i=0; i != carrys.size(); i++) {
            find_matching_sums(sumset, carrys[i]);
        }

        std::cout << "creating the bitslice graph ... " << std::endl;
        // TODO: fix this by making removing bitslices if 
        bitslice_graph_t graph;
        for(unsigned i=0; i != carrys.size(); i++) {
            bitslice_t* bt = carrys[i];
            if(bt->partners.size() == 0) continue;
            for(unsigned j=0; j != carrys.size(); j++) {
                bitslice_t* ct = carrys[j];
                if(ct->partners.size() == 0) continue;
                if(i != j && canJoinAdders(bt, ct)) {
                    graph.add_edge(bt, ct);
                }
            }
        }

        bitslice_set_t nodes2remove;
        for(bitslice_graph_t::edge_list_t::iterator it = graph.edges_out.begin(); 
                                                    it != graph.edges_out.end();
                                                    it++)
        {
            bitslice_t* na = it->first;
            for(bitslice_graph_t::edge_list_t::iterator jt = graph.edges_out.begin(); 
                                                        jt != graph.edges_out.end();
                                                        jt++)
            {
                bitslice_t* nb = jt->first;
                if(na == nb) continue;
                if(na->inv_contained(*nb)) {
                    const bitslice_set_t& sa = it->second;
                    const bitslice_set_t& sb = jt->second;
                    if(sa.size() == sb.size() &&
                       std::equal(sa.begin(), sa.end(), sb.begin()))
                    {
                        // std::cout << "eliminating: " << *nb << " due to " << *na << std::endl;
                        nodes2remove.insert(nb);
                    }
                }
            }
        }
        std::cout << "number of nodes to eliminate: " << nodes2remove.size() << std::endl;
        for(bitslice_set_t::iterator it = nodes2remove.begin(); it != nodes2remove.end(); it++) {
            bitslice_t* node = *it;
            graph.remove_node(node);
        }

        std::cout << "now enumerating the chains ... " << std::endl;
        adder_creator_t creator(module, sumset);
        graph.enumerateChains(creator);
        std::cout << "done with the ripple carry adder analysis!" << std::endl;
    }


    void findCarryChains(flat_module_t* module, bitslice_list_t& carrys, bitslice_list_t& sums)
    {
        std::list< carry_chain_t* > cclist;
        for(unsigned i=0; i != carrys.size(); i++) {
            bitslice_t* bt = carrys[i];
            cclist.push_back(new carry_chain_t(bt));
        }
        
        while(true) {
            bool change = false;
            for(std::list< carry_chain_t* >::iterator it = cclist.begin();
                                                      it != cclist.end();
                                                      it++)
            {
                carry_chain_t* cc = *it;

                if(cc->dead) continue;
                bool thisChanged = false;

                for(unsigned i=0; i != carrys.size(); i++) {
                    bitslice_t* bt = carrys[i];
                    bool added = cc->addIfPossible(bt);
                    change = added || change;
                    thisChanged = added || thisChanged;
                }
                if(!thisChanged) cc->dead = true;
            }
            if(!change) break;
        }

        for(std::list< carry_chain_t* >::iterator jt = cclist.begin();
                jt != cclist.end();
                jt++)
        {
            carry_chain_t* jcc = *jt;
            bool dup = false;
            for(std::list< carry_chain_t* >::iterator it = cclist.begin();
                    it != jt;
                    it++)
            {
                carry_chain_t* icc = *it;
                if(*icc == *jcc) {
                    dup = true;
                    break;
                }
            }
            if(!dup && jcc->slices.size() >= 4) {
                adderlist_t carrys;
                createAdderChain(jcc, sums, carrys);
                if(carrys.size() >= 4) {
                    createAdderModule(module, carrys);
                }
            }
        }

        for(std::list< carry_chain_t* >::iterator jt = cclist.begin();
                                                  jt != cclist.end();
                                                  jt++)
        {
            carry_chain_t* cc = *jt;
            delete cc;
        }
    }

    void adder_creator_t::use(std::list<bitslice_t*>& l)
    {
        if(l.size() < 4) return;

        carry_chain_t cc(l);
        adderlist_t adders;
        createAdderChain2(&cc, sums, adders);
        if(adders.size() >= 4) {
            createAdderModule(module, adders);
        }
    }

    void createAdderModule(flat_module_t* module, adderlist_t& adders)
    {
        id_module_t* mod = new id_module_t("ripple_addsub", id_module_t::UNSLICEABLE);
        word_t* a = new word_t(true, word_t::KCOVER_ANALYSIS);
        word_t* b = new word_t(true, word_t::KCOVER_ANALYSIS);
        word_t* s = new word_t(true, word_t::RCA_SUM);

        node_t *carry_in = NULL;
        for(unsigned i=0; i != adders.size(); i++) {
            bitslice_t* carry = i > 0 ? adders[i-1].first : NULL;
            node_t* prev_carry = carry ? carry->ys[0] : NULL;

            unsigned p = 0;
            bitslice_t* sum = adders[i].second;
            for(unsigned x=0; x != sum->xs.size(); x++) {
                node_t* sb = sum->xs[x];
                if(!( prev_carry && (prev_carry == sb || module->not_related(prev_carry, sb)) )) {
                    node_t* inp = sb;
                    if(p == 0 || p == 1) {
                        if(sb->is_gate() && !sb->is_latch_gate() && sb->is_inverter()) {
                            node_t* inv_inp = sb->get_input(0);
                            if(/*carry=adders[i].first*/adders[i].first->has_input(inv_inp)) {
                                inp = inv_inp;
                            }
                        }
                    }

                    if(p == 0) {
                        a->add_bit(inp, i);
                    } else if(p == 1) {
                        b->add_bit(inp, i);
                    } else {
                        // DEBUG
                        if(i != 0) {
                            std::cout << "prev carry node: " << prev_carry->get_name() << std::endl;
                            std::cout << "bad node: " << sb->get_name() << std::endl;
                        }
                        assert(i == 0);
                        assert(carry_in == NULL);
                        carry_in = sb;
                        mod->add_input(carry_in);
                    }
                    p = p + 1;
                }
            }
            s->add_bit(sum->ys[0], i);
        }
        int last = adders.size()-1;
        mod->add_output(adders[last].first->ys[0]);
        module->add_word(a = module->get_canonical_word(a));
        module->add_word(b = module->get_canonical_word(b));
        module->add_word(s = module->get_canonical_word(s));

        //std::cout << "a: " << *a << std::endl;
        //std::cout << "b: " << *b << std::endl;
        mod->add_input_word(a);
        mod->add_input_word(b);
        mod->add_output_word(s);

        // Q. what's the deal with this code below?  why did I write it?!
        // A. the reason this is exists because the carry and sum chains are "mismatched" in some RCAs.
        //    so we manually add the carry chains into the internals.
        for(unsigned i=0; i != adders.size(); i++) {
            bitslice_t* carry = adders[i].first;
            //std::cout << "slice: " << *carry << std::endl;

            carry->add_internals(carry->ys[0], mod->get_internals());
            if(i != adders.size()-1) {
                //std::cout << "inserting1: " << carry->ys[0]->get_name() << std::endl;
                mod->get_internals().insert(carry->ys[0]);
            }
            for(unsigned j=0; j != carry->xs.size(); j++) {
                node_t* n = carry->xs[j];
                if(!mod->is_input(n)) {
                    //std::cout << "inserting2: " << n->get_name() << std::endl;
                    mod->get_internals().insert(n);
                }
            }
        }

        mod->compute_internals();
        module->add_module(mod);
    }

    std::ostream& operator<<(std::ostream& out, adderlist_t& adders) 
    {
        out << "adder::" << adders.size() << " ";
        for(unsigned i=0; i != adders.size(); i++) {
            bitslice_t* carry = adders[i].first;
            bitslice_t* sum = adders[i].second;
            out << "[" << *carry << ", " << *sum << "] ";
        }
        return out;
    }

    void createAdderChain(carry_chain_t* cc, bitslice_list_t& sums, adderlist_t& adders)
    {
        int cnt = 0;
        for(carry_chain_t::list_t::iterator it = cc->slices.begin();
                                            it != cc->slices.end();
                                            it++)
        {
            bitslice_t* carry = *it;
            bool found = false;
            for(unsigned i=0; i != sums.size(); i++) {
                bitslice_t* sum = sums[i];
                if(sum->not_related_inputs(*carry)) {
                    // TODO: what if there are multiple sum nodes that match.
                    found = true;
                    adder_t adder(carry, sum);
                    adders.push_back(adder);
                    break;
                }
            }
            if(found) cnt += 1;
            else break;
        }
    }

    void createAdderChain2(carry_chain_t* cc, sum_node_set_t& sums, adderlist_t& adders)
    {
        for(carry_chain_t::list_t::iterator it = cc->slices.begin();
                                            it != cc->slices.end();
                                            it++)
        {
            bitslice_t* carry = *it;
            if(carry->partners.size() > 0) {
                bitslice_t* s1 = *carry->partners.begin();
                adder_t adder(carry, s1);
                adders.push_back(adder);
            }
            else break;
        }
    }

    std::ostream& operator<<(std::ostream& out, const bitslice_list_t& bits)
    {
        out << "{ ";
        for(unsigned i=0; i != bits.size(); i++) {
            bitslice_t* bt = bits[i];
            out << *bt << " ";
        }
        out << "}";
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const carry_chain_t& cc)
    {
        out << "carrychain:: ";
        for(carry_chain_t::list_t::const_iterator i = cc.slices.begin();
                                                  i != cc.slices.end();
                                                  i++)
        {
            bitslice_t* bt = *i;
            out << *bt << " ";
        }
        return out;
    }

    void bitslice_graph_t::add_edge(bitslice_t* a, bitslice_t* b)
    {
        edges_out[a].insert(b);
        edges_in[b].insert(a);
    }

    void bitslice_graph_t::remove_edge(bitslice_t* a, bitslice_t* b)
    {
        assert(edges_out.find(a) != edges_out.end() && edges_out[a].find(b) != edges_out[a].end());
        assert(edges_in.find(b) != edges_in.end() && edges_in[b].find(a) != edges_in[b].end());
        edges_out[a].erase(b);
        edges_in[b].erase(a);
    }

    void bitslice_graph_t::remove_node(bitslice_t* a)
    {
        typedef std::vector<bitslice_pair_t> bitslice_pair_list_t;
        bitslice_pair_list_t edges_to_remove;

        if(edges_out.find(a) != edges_out.end()) {
            const bitslice_set_t& out = edges_out[a];
            for(bitslice_set_t::const_iterator it = out.begin(); it != out.end(); it++) {
                bitslice_pair_t p(a, *it);
                edges_to_remove.push_back(p);
            }
        }
        if(edges_in.find(a) != edges_in.end()) {
            const bitslice_set_t& in = edges_in[a];
            for(bitslice_set_t::const_iterator it = in.begin(); it != in.end(); it++) {
                bitslice_pair_t p(*it, a);
                edges_to_remove.push_back(p);
            }
        }

        for(unsigned i=0; i != edges_to_remove.size(); i++) {
            bitslice_t* a = edges_to_remove[i].first;
            bitslice_t* b = edges_to_remove[i].second;
            remove_edge(a, b);
        }
        if(edges_out.find(a) != edges_out.end()) {
            const bitslice_set_t& out = edges_out[a];
            assert(out.size() == 0);
            edges_out.erase(a);
        }
        if(edges_in.find(a) != edges_in.end()) {
            const bitslice_set_t& in = edges_in[a];
            assert(in.size() == 0);
            edges_in.erase(a);
        }
    }

    bool canJoinAdders(bitslice_t* a, bitslice_t* b)
    {
        if(b->numInputs() == 2) return false;
        return (b->is_input(0, a));
    }

    void bitslice_graph_t::enumerateChains(bitslice_chain_user_t& user)
    {
        int total = edges_out.size();
        int pos = 1;
        for(edge_list_t::iterator it = edges_out.begin(); it != edges_out.end(); it++)
        {
            bitslice_t* n = it->first;
            if(num_edges_in(n) == 0) {
                std::list<bitslice_t*> l;
                l.push_back(n);
                enumerateChains(n, user, l);
            }
            printf("PROGRESS: %6d/%6d\r", pos++, total);
            fflush(stdout);
        }
    }

    void bitslice_graph_t::enumerateChains(
        bitslice_t* start, 
        bitslice_chain_user_t& user, 
        std::list<bitslice_t*>& l
    )
    {
        if(num_edges_out(start) > 0) {
            edge_list_t::iterator pos = edges_out.find(start);
            bitslice_set_t& s = pos->second;
            for(bitslice_set_t::iterator it = s.begin(); it != s.end(); it++) {
                bitslice_t* bs = *it;
                l.push_back(bs);
                enumerateChains(bs, user, l);
                l.pop_back();
            }
        } else {
            user.use(l);
        }
    }

    bool carry_chain_t::addIfPossible(bitslice_t* bt) 
    {
        assert(bt->numInputs() == 2 || bt->numInputs() == 3);
        if(bt->numInputs() == 2) return false;

        list_t::reverse_iterator last_it = slices.rbegin();
        bitslice_t* last = *last_it;
        if(bt->is_input(0, last)) {
            assert(std::find(slices.begin(), slices.end(), bt) == slices.end());
            slices.push_back(bt);
            return true;
        }

        list_t::iterator first_it = slices.begin();
        bitslice_t* first = *first_it;
        if(first->is_input(0, bt) && bt->numInputs() <= first->numInputs()) {
            assert(std::find(slices.begin(), slices.end(), bt) == slices.end());
            slices.push_front(bt);
            return true;
        }

        return false;
    }

    bool carry_chain_t::dominates(carry_chain_t* other) 
    {
        // can't dominate yourself
        if(this == other) return false;

        for(list_t::iterator it = other->slices.begin();
                             it != other->slices.end();
                             it++)
        {
            bitslice_t* bt = *it;
            if(std::find(slices.begin(), slices.end(), bt) == slices.end()) {
                return false;
            }
        }
        return true;
    }

    bool carry_chain_t::operator==(const carry_chain_t& other) const 
    {
        if(slices.size() != other.slices.size()) return false;

        for(list_t::const_iterator 
                             it = other.slices.begin(),
                             jt = slices.begin();
                             it != other.slices.end() && jt != slices.end();
                             it++,jt++)
        {
            bitslice_t& bi = **it;
            bitslice_t& bj = **jt;
            if(!(bi == bj)) return false;
        }
        return true;
    }

    void printCovers(const char* prefix, std::set<kcover_t*>& covers)
    {
        for(std::set<kcover_t*>::iterator it = covers.begin();
                                          it != covers.end();
                                          it++)
        {
            kcover_t* cover = *it;
            std::cout << prefix << *cover << std::endl;
        }
    }

    void addCovers(std::set<kcover_t*>& covers, std::vector<bitslice_t*>& slices)
    {
        for(std::set<kcover_t*>::iterator it = covers.begin();
                                          it != covers.end();
                                          it++)
        {
            kcover_t* cover = *it;
            bitslice_t* bt = new bitslice_t(cover);
            bt->sort_inputs();
            slices.push_back(bt);
        }
    }

    std::ostream& operator<<(std::ostream& out, const bitslice_t& b)
    {
        for(unsigned i=0; i != b.ys.size(); i++) {
            out << b.ys[i]->get_name();
            if(i != b.ys.size()-1) out << ", ";
        }
        out << " = f(";
        for(unsigned i=0; i != b.xs.size(); i++) {
            out << b.xs[i]->get_name();
            if(i != b.xs.size()-1) out << ", ";
        }
        out << ")";
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const id_module_t::nodelist_t& b)
    {
        for(unsigned i=0; i != b.size(); i++) {
            out << b[i]->get_name() << " ";
        }
        return out;
    }


    std::ostream& operator<<(std::ostream& out, const id_module_t& mod)
    {
        // assert(!mod.is_dominated());
        out << ".module index:" << mod.moduleNumber << " type:" << mod.type << std::endl;
        if(mod.inputs.size()) {
            out << "  .input ";
            for(unsigned i = 0; i != mod.inputs.size(); i++) {
                node_t* n = mod.inputs[i];
                out << n->get_name() << " ";
            }
            out << std::endl;
        }

        if(mod.outputs.size()) {
            out << "  .output ";
            for(unsigned i = 0; i != mod.outputs.size(); i++) {
                node_t* n = mod.outputs[i];
                out << n->get_name() << " ";
            }
            out << std::endl;
        }

        for(unsigned i=0; i != mod.word_inputs.size(); i++) {
            word_t* w = mod.word_inputs[i];
            out << "  .word_input ";
            out << *w << std::endl;
        }

        for(unsigned i=0; i != mod.word_outputs.size(); i++) {
            word_t* w= mod.word_outputs[i];
            out << "  .word_output ";
            out << *w << std::endl;
        }

        for(moduleset_t::iterator it = mod.module_inputs.begin(); it != mod.module_inputs.end(); it++) {
            id_module_t* m = (*it);
            out << "  .module_input ";
            out << "index: " << m->moduleNumber <<std::endl;
        }

        if(mod.internals.size()) {
            out << "  .gates ";
            for(id_module_t::nodeset_t::iterator 
                    it=mod.internals.begin();
                    it!=mod.internals.end();
                    it++)
            {
                node_t* n = *it;
                out << n->get_name() << " ";
            }
            out << std::endl;
        }

        out << ".endmodule " << std::endl;

        return out;
    }

    BDD xorFn(input_provider_t* e) 
    {
        return e->inp(0) ^ e->inp(1);
    }
    BDD xnorFn(input_provider_t* e) 
    {
        return !(e->inp(0) ^ e->inp(1));
    }

    fnInfo_t* getAnds(flat_module_t* module, int size)
    {
        input_provider_t* ipp = module->get_ipp(size);
        BDD b = ipp->one();
        for(int i=0; i < size; i++) {
            b = b & ipp->inp(i);
        }
        fnInfo_t* inf = module->getFunction(b);
        if(inf == NULL) return NULL;
        else return inf->canonicalPtr;
    }

    fnInfo_t* getNands(flat_module_t* module, int size)
    {
        input_provider_t* ipp = module->get_ipp(size);
        BDD b = ipp->one();
        for(int i=0; i < size; i++) {
            b = b & ipp->inp(i);
        }
        b = !b;
        fnInfo_t* inf = module->getFunction(b);
        if(inf == NULL) return NULL;
        else return inf->canonicalPtr;
    }

    fnInfo_t* getOrs(flat_module_t* module, int size)
    {
        input_provider_t* ipp = module->get_ipp(size);
        BDD b = ipp->zero();
        for(int i=0; i < size; i++) {
            b = b + ipp->inp(i);
        }
        fnInfo_t* inf = module->getFunction(b);
        if(inf == NULL) return NULL;
        else return inf->canonicalPtr;
    }

    fnInfo_t* getNors(flat_module_t* module, int size)
    {
        input_provider_t* ipp = module->get_ipp(size);
        BDD b = ipp->zero();
        for(int i=0; i < size; i++) {
            b = b + ipp->inp(i);
        }
        b = !b;
        fnInfo_t* inf = module->getFunction(b);
        if(inf == NULL) return NULL;
        else return inf->canonicalPtr;
    }

    void addRoots(kcoverset_t& covers, nodeset_t& nodes)
    {
        for(kcoverset_t::iterator it = covers.begin(); it != covers.end(); it++) {
            kcover_t* kc = *it;
            node_t* rt = kc->get_root();
            nodes.insert(rt);
        }
    }

    bool isSupportedCover(kcover_t* kc, nodeset_t& support)
    {
        for(unsigned i=0; i != kc->numInputs(); i++) {
            node_t* n = kc->at(i);
            if(support.find(n) == support.end()) return false;
        }
        return true;
    }

    void addEqInputs(kcover_t* kc, kcoverset_t& eqCovs, id_module_t* id)
    {
        for(unsigned i=0; i != kc->numInputs(); i++) {
            node_t* n = kc->at(i);
            for(kcoverset_t::iterator it = eqCovs.begin(); it != eqCovs.end();it++) {
                kcover_t* eqCov = *it;
                if(eqCov->get_root() == n) {
                    for(unsigned j=0; j != eqCov->size(); j++) {
                        id->add_input(eqCov->at(j));
                    }
                }
            }
        }
    }

    void createEqModules(
        flat_module_t* module, 
        nodeset_t& support, 
        kcoverset_t& covers, 
        const char* name, 
        kcoverset_t& eqCovs
    )
    {
        for(kcoverset_t::iterator it = covers.begin(); it != covers.end(); it++) {
            kcover_t* kc = *it;
            if(isSupportedCover(kc, support)) {
                id_module_t* id = new id_module_t(name, id_module_t::UNSLICEABLE);
                id->add_output(kc->get_root());
                addEqInputs(kc, eqCovs, id);
                id->compute_internals();
                module->add_module(id);
            }
        }
    }

    void identifyEqualityComparators(flat_module_t* module)
    {
        std::cout << "identify equality comparators." << std::endl;
        
        using namespace std;
        if(options.kcoverSize < 2) return;
        input_provider_t* ipp = module->get_ipp(2);

        BDD xnorBDD = xnorFn(ipp);
        fnInfo_t *infoXnor = module->getFunction(xnorBDD);
        if(infoXnor == NULL) return;

        nodeset_t xnorRoots;
        fnInfo_t *ixnor = infoXnor->canonicalPtr;
        addRoots(ixnor->covers, xnorRoots);

        for(unsigned i=2; i <= options.kcoverSize; i++) {
            fnInfo_t* inf = getAnds(module, i);
            if(inf) {
                char name[16];
                sprintf(name, "eqCmp%d", i);
                createEqModules(module, xnorRoots, inf->covers, name, ixnor->covers);
            }
            inf = getNands(module, i);
            if(inf) {
                char name[16];
                sprintf(name, "neqCmp%d", i);
                createEqModules(module, xnorRoots, inf->covers, name, ixnor->covers);
            }
        }

        BDD xorBDD = xorFn(ipp);
        fnInfo_t *infoXor = module->getFunction(xorBDD);
        if(infoXor == NULL) return;

        nodeset_t xorRoots;
        fnInfo_t* ixor = infoXor->canonicalPtr;
        addRoots(ixor->covers, xorRoots);

        for(unsigned i=2; i <= options.kcoverSize; i++) {
            fnInfo_t* inf = getNors(module, i);
            if(inf) {
                char name[16];
                sprintf(name, "eqCmp%d", i);
                createEqModules(module, xorRoots, inf->covers, name, ixor->covers);
            }
            inf = getOrs(module, i);
            if(inf) {
                char name[16];
                sprintf(name, "neqCmp%d", i);
                createEqModules(module, xorRoots, inf->covers, name, ixor->covers);
            }
        }
    }

    void identifyEqualityComparators2(flat_module_t* module)
    {
        if(options.kcoverSize < 2) return;
        input_provider_t* e = module->get_ipp(2);
        BDD beq = !(e->inp(0) ^ e->inp(1));
        BDD blt = !e->inp(0) & e->inp(1);
        BDD bgt = e->inp(0) & !e->inp(1);

        fnInfo_t* ieq = module->getFunction(beq);
        if(ieq == NULL) return;

        bitslice_list_t eqs;
        for(kcoverset_t::iterator it =  ieq->covers.begin();
                                  it != ieq->covers.end();
                                  it++)
        {
            kcover_t* kc = *it;
            bitslice_t* bt = new bitslice_t(kc);
            eqs.push_back(bt);
        }

        fnInfo_t* ilt = module->getFunction(blt);
        bool ltCanonical = ilt->canonicalPtr == ilt;

        bitslice_list_t lts;
        bitslice_list_t gts;

        fnInfo_t* icmp = ilt->canonicalPtr;
        for(kcoverset_t::iterator it =  icmp->covers.begin();
                                  it != icmp->covers.end();
                                  it++)
        {
            kcover_t* kc = *it;
            std::vector<uint8_t>& p = kc->getPerm();
            bitslice_t* bt = new bitslice_t(kc);

            bool found = false;
            for(unsigned i=0; i != eqs.size(); i++) {
                if(eqs[i]->same_inputs(*bt)) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                delete bt;
                continue;
            }

            if(ltCanonical) {
                if(p[0] == 0) {
                    lts.push_back(bt);
                } else {
                    gts.push_back(bt);
                }
            } else {
                if(p[0] == 0) {
                    gts.push_back(bt);
                } else {
                    lts.push_back(bt);
                }
            }
        }

        printf("eqs.size() = %d\n", (int) eqs.size());
        printf("lts.size() = %d\n", (int) lts.size());
        printf("gts.size() = %d\n", (int) gts.size());

        deleteSlices(eqs);
        deleteSlices(lts);
        deleteSlices(gts);
    }

    bool shortestPathComp(node_t* x, node_t* y) {
        return x->get_dist_to_source() > y->get_dist_to_source();
    }

    void shortestPath(flat_module_t* module){
        std::string source ("VGA_PIXLp[11]");
        printf("Finding shortest path from %s...\n", source.c_str());	
        node_t* n = module->get_node_by_name(source.c_str());
        node_t* target = NULL;

        n->set_dist_to_source(0.); //Source node is 0 away from itself
        std::vector<node_t*> nodesToVisit; //All possible nodes to visit (discovered along way via inputs)

        nodesToVisit.push_back(n);

        while (!nodesToVisit.empty()) {
            //Take node with smallest distance!
            std::sort(nodesToVisit.begin(), nodesToVisit.end(), shortestPathComp);
            n = nodesToVisit.back();
            nodesToVisit.pop_back(); //remove from the list we're creating
            n->VISITED = true; //mark as visited

            //INSERT END CONDITION HERE


            if (n == NULL) return;
            for(node_t::input_iterator it = n->inputs_begin(); 
                    it != n->inputs_end();
                    it++)
            {
                if ((*it)->VISITED == false) {
                    nodesToVisit.push_back(*it);
                    float alt = n->get_dist_to_source() + 1;
                    if (alt < (*it)->get_dist_to_source()) {
                        (*it)->set_dist_to_source(alt);
                        (*it)->set_previous_node(n);

                        if (!(*it)->is_input() && !(*it)->is_macro_out()) {
                            if (strcmp((*it)->get_lib_elem_name(), "RF2TCSG0064X032D1") == 0) {
                                printf("Found frame buffer\n");
                                target = (*it);
                                break;
                            }
                        }
                    }
                }
            }

            if (target != NULL) break;
            //if (n->is_input()) continue;
            //if (n->is_latch()) continue;
            //if (n->is_macro()) continue;
            //if (n->is_macro_out()) continue;
            //if (!n->is_input() && !n->is_macro_out())
            //printf("%s\n", n->get_lib_elem_name());
        }

        //Trace back from target to get entire path.
        node_t* traverse = target;
        while (traverse->previous != NULL) {
            if(!traverse->is_input() && !traverse->is_macro_out())
                printf("%s\n", traverse->get_lib_elem_name());
            traverse = traverse->previous;
        }
    }

    int countOnes(int num) {
        int c = 0;
        while(num != 0) {
            c += 1;
            num = num & (num-1);
        }
        return c;
    }

    BDD getCube(Cudd& mgr, int vars, int num) {
        BDD cb = mgr.bddOne();
        for(int pos = 0; pos != vars; pos++) {
            int mask = 1 << pos;
            if(num & mask) {
                cb = cb & mgr.bddVar(pos);
            } else {
                cb = cb & !mgr.bddVar(pos);
            }
        }
        return cb;
    }

    // return the number of bits required to represent this number
    // 1 --> 1 
    // 2 --> 2 
    // 3 --> 2 
    // ..
    // 4 --> 3
    // 5 --> 3
    // ...
    // 8 --> 4
    // 9 --> 4
    // ...
    // 16 --> 5
    int getBitsReq(int n) {
        assert(n != 0);
        assert(n < INT_MAX/2);
        for(int i=0; i != 32; i++) {
            int ls = 1 << i;
            if((ls+ls) > n) {
                return i+1;
            }
        }
        assert(false);
        return -1;
    }

    // take the cubes that add up to this result value and them to the positions
    // which have a 1-bit in the result. 
    void addCubes(int numResultBits, int result, std::vector<BDD>& fns, BDD& cubesum)
    {
        for(int pos = 0; pos != numResultBits; pos++) {
            int mask = 1 << pos;
            if(result & mask) {
                fns[pos] = fns[pos] + cubesum;
            }
        }
    }

    void computePopCountBDDs(Cudd& mgr, int n, std::vector<BDD>& result)
    {
        assert(n >= 2 && n <= 16384);

        int max = 1 << n;
        std::map<int, std::vector<BDD> > cubes;
        for(int num = 0; num < max; num++) {
            int ones = countOnes(num);
            cubes[ones].push_back(getCube(mgr, n, num));
        }
        int numResultBits = getBitsReq(n);
        result.clear();
        for(int i = 0; i != numResultBits; i++) {
            result.push_back(mgr.bddZero());
        }
        for(std::map<int, std::vector<BDD> >::iterator it = cubes.begin(); it != cubes.end(); it++) {
            BDD cubesum = mgr.bddZero();
            for(std::vector<BDD>::iterator jt = it->second.begin(); jt != it->second.end(); jt++) {
                cubesum = cubesum + *jt;
            }
            addCubes(numResultBits, it->first, result, cubesum);
        }
        cubes.clear();
    }

    void findPopCnts(flat_module_t* flat)
    {
        if(!options.analyzeCommonInputs) return;

        modulelist_t xortrees;
        popcnt_groups_t groups;
        flat_module_t::markings_t marks(flat->max_index(), false);

        for(unsigned i=0; i != flat->modules.size(); i++) {
            id_module_t* m = flat->modules[i];
            if(strcmp(m->get_type(), "xortree") == 0) {
                xortrees.push_back(m);
                node_t* op = *((m->get_outputs())->begin());
                node_t* rep = op->get_support_rep();
                marks[rep->get_index()] = true;
            }
        }

        std::cout << "found " << xortrees.size() << " xortree modules. " << std::endl;

        for(unsigned i=0; i != flat->gates.size(); i++) {
            node_t* gi = flat->gates[i];
            node_t* rep = gi->get_support_rep();
            if(marks[rep->get_index()]) {
                groups[rep].push_back(gi);
            }
        }

        std::cout << "created " << groups.size() << " popcnt groups. " << std::endl;

        int cnt = 0; 
        for(unsigned i=0; i != xortrees.size(); i++) {
            id_module_t* m = xortrees[i];
            assert(m->num_outputs() == 1);
            if(m->num_inputs() >= 16) continue;

            node_t* op = *((m->get_outputs())->begin());
            node_t* rep = op->get_support_rep();
            
            popcnt_groups_t::iterator pos = groups.find(rep);
            assert(pos != groups.end());
            if(pos->second.size() > 1) {
                if(findPopCnts(flat, m, pos->second)) { cnt += 1; }
            }
            if(i % 10 == 0) {
                printf("%7d/%7d\r", (int)i, (int)xortrees.size());
                fflush(stdout);
            }
        }
        printf("\n");
        clear_popcnt_bdd_cache();
        std::cout << "found " << cnt << " popcnt modules." << std::endl;
    }

    struct create_vars_t {
        Cudd& mgr;
        id_module_t* m;
        std::map<node_t*, BDD>& vars;

        create_vars_t(Cudd& cudd, id_module_t* mod, std::map<node_t*, BDD>& v) 
          : mgr(cudd), m(mod), vars(v) 
        {}
        void operator() (node_t* n) {
            if(vars.find(n) == vars.end()) {
                vars[n] = mgr.bddVar(vars.size());
            }
        }
    };

    std::map<int, std::vector<BDD> > popcnt_bdd_cache;

    std::vector<BDD>& get_popcnt_bdds(flat_module_t* flat, int n) {
        if(popcnt_bdd_cache.find(n) == popcnt_bdd_cache.end()) {
            computePopCountBDDs(flat->getFullFnMgr(), n, popcnt_bdd_cache[n]);
        }
        return popcnt_bdd_cache[n];
    }

    void clear_popcnt_bdd_cache()
    {
        for(std::map<int, std::vector<BDD> >::iterator it = popcnt_bdd_cache.begin(); 
            it != popcnt_bdd_cache.end(); it++) 
        {
            it->second.clear();
        }
        popcnt_bdd_cache.clear();
    }

    bool findPopCnts(flat_module_t* flat, id_module_t* mod, std::vector<node_t*>& outnodes)
    {
        assert(mod->num_outputs() == 1);

        std::vector<BDD>& bdds = get_popcnt_bdds(flat, mod->num_inputs());

        std::vector<node_t*> outs;
        outs.resize(bdds.size(), NULL);
        std::vector<bool> out_neg(bdds.size(), false);

        if(outnodes.size() <= 1) {
            return false;
        }

        for(unsigned i=0; i != outnodes.size(); i++) {
            std::map<node_t*, BDD> vars;
            create_vars_t cv(flat->getFullFnMgr(), mod, vars);
            mod->apply_on_all_inputs(cv);

            unsigned old_size = vars.size();
            BDD outfn = flat->createFullFn(outnodes[i], vars, false, -1);
            if(vars.size() == old_size) {
                BDD outfn_neg = !outfn;
                for(std::map<node_t*, BDD>::iterator it = vars.begin(); it != vars.end(); it++) {
                    node_t* n = it->first;
                    BDD v_n = it->second;
                    BDD not_v_n = !v_n;
                    vars[n] = not_v_n;
                }
                BDD neginp_outfn = flat->createFullFn(outnodes[i], vars, false, -1);
                BDD neginp_outfn_neg = !neginp_outfn;

                for(unsigned j=0; j != bdds.size(); j++) {
                    bool inv = false;
                    if(bdds[j] == outfn || (inv = (bdds[j] == outfn_neg)) ||
                       bdds[j] == neginp_outfn || (inv = (bdds[j] == neginp_outfn_neg))) 
                    {
                        outs[j] = outnodes[i];
                        out_neg[j] = inv;
                        break;
                    }
                }
            }
            vars.clear();
        }

        // TODO: update pin map here!
        if(outs.size() >= 2 && outs[0] != NULL && outs[1] != NULL) {
            id_module_t* newmod = new id_module_t("popcnt", id_module_t::SLICEABLE, id_module_t::INFERRED);
            for(unsigned i=0; i != outs.size() && outs[i] != NULL; i++) {
                newmod->add_output(outs[i]);
            }
            for(unsigned i=0; i != mod->inputs.size(); i++) {
                newmod->add_input(mod->inputs[i]);
            }
            newmod->compute_internals();
            flat->add_module(newmod);
            return true;
        }
        return false;
    }

    struct reg_analysis_check_outputs_t {
        ::nodelist_t& outs;
        id_module_t* mod;
        reg_analysis_check_outputs_t(::nodelist_t& o, id_module_t* m) 
            : outs(o), mod(m) 
        {}

        void operator() (node_t* out) {
            if(out->num_fanouts() == 1) {
                node_t* fout = *(out->fanouts_begin());
                if(fout->is_latch_gate()) {
                    assert(fout->num_fanouts() == 1);
                    node_t* latch = *(fout->fanouts_begin());
                    if(mod->is_input(latch)) {
                        std::string selgrp("sel");
                        if(!mod->is_input_in_group(selgrp, latch)) {
                            outs.push_back(out);
                        }
                    }
                }
            }
        }
    };

    struct reg_analysis_input_creator_t {
        id_module_t* mod;
        const nodeset_t& good_inputs;

        reg_analysis_input_creator_t(id_module_t* m, const nodeset_t& gi)
            : mod(m), good_inputs(gi)
        {}

        void operator() (const std::string& grp, node_t* n) {
            if(good_inputs.find(n) != good_inputs.end() && !mod->is_input(n)) {
                mod->add_input(grp, n, false);
            }
        }
        void operator() (node_t* ni) {
            if(good_inputs.find(ni) != good_inputs.end() && !mod->is_input(ni)) {
                mod->add_input(ni);
            }
        }
    };

    id_module_t* id_module_t::registerAnalysis(flat_module_t* flat)
    {
        assert(strstr(type, "mux") == type);

        ::nodelist_t outs;
        reg_analysis_check_outputs_t checker(outs, this);
        apply_on_all_outputs(checker);

        if(outs.size() >= 2) {
            nodeset_t good_inputs;
            nodelist_t latches;

            for(unsigned i=0; i != outs.size(); i++) {
                node_t* out = outs[i];
                assert(out->num_fanouts() == 1);

                node_t* lg = *(out->fanouts_begin());
                assert(lg);
                assert(lg->is_latch_gate());
                assert(lg->num_fanouts() == 1);

                node_t* lat = *(lg->fanouts_begin());
                assert(lat);
                assert(lat->is_latch());

                latches.push_back(lat);

                const nodeset_t& slice_inputs = get_inputmap(out);
                std::copy(slice_inputs.begin(), 
                          slice_inputs.end(), 
                          std::inserter(good_inputs, good_inputs.end()));
            }

            assert(latches.size() == outs.size());
            id_module_t* mod = new id_module_t("reg", id_module_t::SLICEABLE, id_module_t::INFERRED);
            std::string auxgrp("aux");

            for(unsigned i=0; i != latches.size(); i++) {
                good_inputs.erase(latches[i]);
                mod->add_output(latches[i]);
                for(node_t::input_iterator it = latches[i]->inputs_begin(); it != latches[i]->inputs_end(); it++) {
                    node_t* n = *it;
                    if(n->is_latch_gate()) {
                        for(node_t::input_iterator jt = n->inputs_begin(); jt != n->inputs_end(); jt++) {
                            node_t* n2 = *jt;
                            if(inputmap.find(n2) == inputmap.end()) {
                                mod->add_input(auxgrp, n2, false);
                            } 
                        }
                    } else if(inputmap.find(n) == inputmap.end()) {
                        mod->add_input(auxgrp, n, false);
                    } 
                }
            }

            reg_analysis_input_creator_t ic(mod, good_inputs);
            apply_on_all_grouped_inputs(ic);
            apply_on_all_inputs(ic);

            mod->compute_internals();

            unsigned n1 = num_internals() + 2*latches.size();
            unsigned n2 = mod->num_internals();
            assert(n2 <= n1);
            reset_inputmaps();
            return mod;
        }

        return NULL;
    }

    struct get_ungrouped_nodes_t {
        nodeset_t& grouped_nodes;
        nodeset_t& ungrouped_nodes;
        get_ungrouped_nodes_t (nodeset_t& g, nodeset_t& u) 
            : grouped_nodes(g), ungrouped_nodes(u) 
        {}
        void operator() (const std::string& grp, node_t* n) {
            grouped_nodes.insert(n);
        }
        void operator() (node_t* n) {
            if(grouped_nodes.find(n) == grouped_nodes.end()) {\
                ungrouped_nodes.insert(n);
            }
        }
    };

    void id_module_t::get_ungrouped_inputs(nodeset_t& ungrouped) const
    {
        nodeset_t grouped;
        get_ungrouped_nodes_t gun(grouped, ungrouped);
        apply_on_all_grouped_inputs(gun);
        apply_on_all_inputs(gun);
    }

    bool id_module_t::same_ungrouped_inputs(id_module_t* other) const
    {
        nodeset_t ug_this;
        nodeset_t ug_other;

        this->get_ungrouped_inputs(ug_this);
        other->get_ungrouped_inputs(ug_other);

        return (std::equal(ug_this.begin(), ug_this.end(), ug_other.begin()));
    }

    struct merger_add_inputs_t {
        id_module_t* mod;
        nodeset_t inputs;
        merger_add_inputs_t(id_module_t* m) : mod(m) {}

        void operator() (const std::string& grp, node_t* n) {
            mod->add_input(grp, n, false);
            inputs.insert(n);
        }
        void operator() (node_t* n) {
            if(inputs.find(n) == inputs.end()) {
                assert(!mod->is_input(n));
                mod->add_input(n); 
                inputs.insert(n);
            }
        }
    };

    struct merger_add_outputs_t {
        id_module_t* mod;
        merger_add_outputs_t(id_module_t* m) : mod(m) {}

        void operator() (node_t* n) {
            mod->add_output(n); 
        }
    };

    id_module_t* merge_modules(std::vector<id_module_t*>& mods)
    {
        id_module_t* m = new id_module_t("reg_group", id_module_t::UNSLICEABLE, id_module_t::INFERRED);
        merger_add_inputs_t ai(m);
        merger_add_outputs_t ao(m);

        for(unsigned i=0; i != mods.size(); i++) {
            mods[i]->apply_on_all_grouped_inputs(ai);
            mods[i]->apply_on_all_inputs(ai);
            mods[i]->apply_on_all_outputs(ao);
        }
        m->compute_internals();
        return m;
    }
}
