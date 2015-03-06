#include <sstream>
#include "gae.h"
#include "main.h"
#include "kcover.h"
#include "flat_module.h"

namespace aggr {
    // comparator for nodelist types.
    bool nodelist_lt_t::operator() (const ::nodelist_t& a, const ::nodelist_t& b)
    {
        if(a.size() < b.size()) return true;
        else if(a.size() > b.size()) return false;
        else {
            for(unsigned i=0; i != a.size(); i++) {
                if(a[i] < b[i]) return true;
                else if(a[i] > b[i]) return false;
            }
            return false;
        }
    }

    common_signals_2_t::common_signals_2_t(
        flat_module_t* flat, 
        predicate2_t pred
    )
        :flat(flat)
        ,pred(pred)
        ,pos(-1)
    {
    }


    common_signals_3_t::common_signals_3_t(
        flat_module_t* flat, 
        predicate3_t pred
    )
        :flat(flat)
        ,pred(pred)
        ,pos(-1)
    {
    }

    common_signals_4_t::common_signals_4_t(
        flat_module_t* flat, 
        predicate4_t pred
    )
        :flat(flat)
        ,pred(pred)
        ,pos(-1)
    {
    }

    void common_signals_2_t::init(bitslice_t* slice)
    {
        pairs.clear();
        pos = 0;

        this->slice = slice;
        for(unsigned i=0; i != slice->xs.size(); i++) {
            node_t* a = slice->xs[i];
            for(unsigned j=i+1; j < slice->xs.size(); j++) {
                node_t* b = slice->xs[j];
                assert(a != b);
                if(pred(flat, a, i, b, j)) {
                    if(a < b) {
                        nodepair_t pair(a, b);
                        pairs.push_back(pair);
                    } else {
                        nodepair_t pair(b, a);
                        pairs.push_back(pair);
                    }
                }
            }
        }
    }

    void common_signals_3_t::init(bitslice_t* slice)
    {
        //assert(slice->xs.size() == 6);

        triples.clear();
        pos = 0;

        this->slice = slice;
        std::vector<node_t*> sortnodes;
        for(unsigned i=0; i != slice->xs.size(); i++) {
            node_t* a = slice->xs[i];
            for(unsigned j=i+1; j < slice->xs.size(); j++) {
                node_t* b = slice->xs[j];
                for(unsigned k=j+1; k < slice->xs.size(); k++) {
                    node_t* c = slice->xs[k];
                    assert(a != b);
                    if(pred(flat, a, i, b, j, c, k)) {
                        sortnodes.clear();
                        sortnodes.push_back(a);
                        sortnodes.push_back(b);
                        sortnodes.push_back(c);
                        std::sort(sortnodes.begin(), sortnodes.end()); 

                        nodetriple_t triple(sortnodes[2], sortnodes[1], sortnodes[0]);
                        triples.push_back(triple);
                    }
                }
            }
        }
    }

    void common_signals_4_t::init(bitslice_t* slice)
    {
        //assert(slice->xs.size() == 6);

        quads.clear();
        pos = 0;

        this->slice = slice;
        std::vector<node_t*> sortnodes;
        for(unsigned i=0; i != slice->xs.size(); i++) {
            node_t* a = slice->xs[i];
            for(unsigned j=i+1; j < slice->xs.size(); j++) {
                node_t* b = slice->xs[j];
                for(unsigned k=j+1; k < slice->xs.size(); k++) {
                    node_t* c = slice->xs[k];
                    for(unsigned l=k+1; l < slice->xs.size(); l++) {
                        node_t* d = slice->xs[l];
                        assert(a != b);
                        assert(a != c);
                        assert(a != d);
                        assert(b != c);
                        assert(b != d);
                        assert(c != d);

                        if(pred(flat, a, i, b, j, c, k, d, l)) {
                            sortnodes.clear();
                            sortnodes.push_back(a);
                            sortnodes.push_back(b);
                            sortnodes.push_back(c);
                            sortnodes.push_back(d);
                            std::sort(sortnodes.begin(), sortnodes.end()); 

                            nodequad_t quad(sortnodes[3], sortnodes[2], sortnodes[1], sortnodes[0]);
                            quads.push_back(quad);
                        }
                    }
                }
            }
        }
    }

    common_signals_2_t::~common_signals_2_t()
    {
    }

    common_signals_3_t::~common_signals_3_t()
    {
    }

    common_signals_4_t::~common_signals_4_t()
    {
    }

    bool common_signals_2_t::more()
    {
        assert(pos >= 0 && pos <= (int) pairs.size());

        if(pos == (int) pairs.size()) return false;
        else return true;
    }

    void common_signals_2_t::next(::nodelist_t& signals)
    {
        assert(pos >= 0 && pos <= (int) pairs.size());

        signals.clear();
        if(pos == (int) pairs.size()) return;
        else {
            signals.resize(2);
            signals[0] = pairs[pos].first;
            signals[1] = pairs[pos].second;

            pos += 1;
        }
    }

    bool common_signals_3_t::more()
    {
        assert(pos >= 0 && pos <= (int) triples.size());

        if(pos == (int) triples.size()) return false;
        else return true;
    }

    void common_signals_3_t::next(::nodelist_t& signals)
    {
        assert(pos >= 0 && pos <= (int) triples.size());

        signals.clear();
        if(pos == (int) triples.size()) return;
        else {
            signals.resize(3);
            signals[0] = triples[pos].a;
            signals[1] = triples[pos].b;
	    signals[2] = triples[pos].c;
            pos += 1;
        }
    }

    bool common_signals_4_t::more()
    {
        assert(pos >= 0 && pos <= (int) quads.size());

        if(pos == (int) quads.size()) return false;
        else return true;
    }

    void common_signals_4_t::next(::nodelist_t& signals)
    {
        assert(pos >= 0 && pos <= (int) quads.size());

        signals.clear();
        if(pos == (int) quads.size()) return;
        else {
            signals.resize(4);
            signals[0] = quads[pos].a;
            signals[1] = quads[pos].b;
	    signals[2] = quads[pos].c;
	    signals[3] = quads[pos].d;
            pos += 1;
        }
    }


    bool pred1_true(flat_module_t* flat, node_t* a, int pa)
    {
        return true;
    }

    bool pred2_true(flat_module_t* flat, node_t* a, int pa, node_t* b, int pb)
    {
        return true;
    }

    bool pred3_true(flat_module_t* flat, node_t* a, int pa, node_t* b, int pb, node_t* c, int pc)
    {
	return true;
    }

    bool pred4_true(flat_module_t* flat, node_t* a, int pa, node_t* b, int pb, node_t* c, int pc, node_t* d, int pd)
    {
	return true;
    }

    simple_common_signal_enumerator_t::simple_common_signal_enumerator_t(
        flat_module_t* flat,
        predicate1_t pred
    )   : flat(flat)
        , pred(pred)
        , slice(NULL)
        , pos(-1)
    {
    }

    void simple_common_signal_enumerator_t::init(bitslice_t* slice)
    {
        this->slice = slice;
        this->pos = 0;
    }

    bool simple_common_signal_enumerator_t::more()
    {
        return (slice != NULL) && (pos != -1) && (pos < (int) slice->xs.size());
    }

    void simple_common_signal_enumerator_t::next(::nodelist_t& signals)
    {
        assert(more());

        signals.resize(1);
        signals[0] = slice->xs[pos];
        pos += 1;
    }

    void groupByCommonSignalBDD(
        flat_module_t* flat,
        const BDD& bdd,
        common_signal_enumerator_t* enumerator,
        bitslice_list_user_t* user
    )
    {
        typedef std::map< ::nodelist_t, bitslice_list_t> groups_t;
        groups_t groups;
        bitslice_list_t slices;

        fnInfo_t* fi = flat->getFunction(bdd);
        if(fi == NULL) return;

        fi = fi->canonicalPtr;
        for(kcoverset_t::iterator it = fi->covers.begin(); it != fi->covers.end(); it++)
        {
            kcover_t* kc = *it;
            if(kc->get_root()->is_latch_gate()) continue;

            bitslice_t* bt = new bitslice_t(kc);
            slices.push_back(bt);

            enumerator->init(bt);
            ::nodelist_t list;
            for(; enumerator->more(); ) {
                enumerator->next(list);
                groups[list].push_back(bt);
            }
        }

        for(groups_t::iterator it = groups.begin(); it != groups.end(); it++) {
            const ::nodelist_t& ns = it->first;
            bitslice_list_t& l = it->second;
            user->use(ns, l);
        }

        deleteSlices(slices);
    }

    void groupByCommonSignal(
        flat_module_t* flat, 
        eval_fun_t func, 
        int num_inputs, 
        common_signal_enumerator_t* enumerator,
        bitslice_list_user_t* user
    )
    {
        input_provider_t* e = flat->get_ipp(num_inputs);
        const BDD bdd = func(e);
        groupByCommonSignalBDD(flat, bdd, enumerator, user);
    }

    bitslice_module_creator_t::bitslice_module_creator_t(
        flat_module_t* module, 
        const char* name,
        const char* selsigname,
        unsigned minSize
    )
        :flat(module)
        ,name(strdup(name))
        ,minSize(minSize)
        ,selsig_name(selsigname)
        ,dump_sizes(false)
    {
    }

    bitslice_module_creator_t::~bitslice_module_creator_t()
    {
        free((void*)name);
    }

    void bitslice_module_creator_t::traceTree(node_t& node, ::nodelist_t& inputs)
    {
        if (node.is_latch() || node.is_input()) {
            inputs.push_back(&node);
            return;
        }
        else {
            for(unsigned i=0; i !=node.num_inputs(); i++) {
                traceTree(*node.get_input(i), inputs);
            }
        }
        return;
    }
    void bitslice_module_creator_t::controlSignalCreation(const ::nodelist_t& nodes)
    {
        for(unsigned i=0; i!= nodes.size(); i++) {
            ::nodelist_t inputs;
            id_module_t* mod = new id_module_t("candidate_controlSignal", id_module_t::SLICEABLE, id_module_t::CANDIDATE_CONTROL_SIGNAL);
            traceTree(*nodes[i], inputs);

            if((inputs.size() < 3) || (inputs.size() > 100)) {
                delete mod;
                return;
            }
            else {
                for(unsigned j=0; j != inputs.size(); j++) 
                    mod->add_input(inputs[j]);
                mod->add_output(nodes[i]);
                mod->compute_internals();
                flat->add_module(mod);
            }

        }
    }

    static std::map<int, int> size_map;
    void bitslice_module_creator_t::use(const ::nodelist_t& nodes, bitslice_list_t& list)
    {
        if(list.size() < minSize) return;
        int size=0;

        id_module_t* mod = new id_module_t(name, id_module_t::SLICEABLE, id_module_t::INFERRED);
        word_t* w = new word_t(false, word_t::KCOVER_ANALYSIS);

        for(unsigned i=0; i != nodes.size(); i++) {
            mod->add_input(selsig_name, nodes[i], false);
        }

        // add the outputs first.
        for(unsigned i=0; i != list.size(); i++) {
            bool ok = true;
            for(unsigned y=0; y != list[i]->ys.size(); y++) {
                node_t* n = list[i]->ys[y];
                if(n->is_latch_gate()) {
                    ok = false;
                    break;
                }
                // mod->add_output(n);
                w->add_bit(n);
            }
            if(!ok) continue;
            size += 1;
        }

        if(w->size() == 0) { 
            delete mod;
            return;
        } else {
            // FIXME: add word to flat_module_t
            mod->add_output_word(w);
            w->get_bit(0)->get_module()->add_word(w);
        }

        // now add the inptus.
        for(unsigned i=0; i != list.size(); i++) {
            bool ok = true;
            for(unsigned y=0; y != list[i]->ys.size(); y++) {
                node_t* n = list[i]->ys[y];
                if(n->is_latch_gate()) {
                    ok = false;
                    break;
                }
            }
            if(!ok) continue;
            size += 1;
            for(unsigned x=0; x != list[i]->xs.size(); x++) {
                node_t* n = list[i]->xs[x];
                if(!mod->is_output(n)) {
                    mod->add_input(n);
                }
            }
        }


        mod->compute_internals();
        flat->add_module(mod);
        if(dump_sizes) {
            size_map[size] += 1;
            if(size > 1000) {
                std::cout << "huge module: " << mod->get_name() << std::endl;
            }
        }
        //Trial Run!
        //controlSignalCreation(nodes);
    }

    void bitslice_list_printer_t::use(const ::nodelist_t& nodes, bitslice_list_t& list)
    {
        std::cout << "list: " << list << std::endl;
    }

    BDD mux21ao22(input_provider_t* e)
    {
        return (e->inp(0) & e->inp(1)) + (e->inp(2) & e->inp(3));
    }

    BDD mux21oa22(input_provider_t* e)
    {
        return (e->inp(0) | e->inp(1)) & (e->inp(2) | e->inp(3));
    }


    BDD mux21aoi22(input_provider_t* e)
    {
        return !((e->inp(0) & e->inp(1)) + (e->inp(2) & e->inp(3)));
    }

    BDD mux21oai22(input_provider_t* e)
    {
        return !((e->inp(0) | e->inp(1)) & (e->inp(2) | e->inp(3)));
    }

    void identify21Muxes2(flat_module_t* flat)
    {
        std::cout << "identifying 2:1 muxes (2)." << std::endl;
        if(options.kcoverSize < 4) return;

        common_signals_2_t cs2(flat, pred2_true);

        bitslice_module_creator_t bsc1(flat, "mux21_ao22", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, mux21ao22, 4, &cs2, &bsc1);

        bitslice_module_creator_t bsc2(flat, "mux21i_aoi22", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, mux21aoi22, 4, &cs2, &bsc2);

        bitslice_module_creator_t bsc3(flat, "mux21_oa22", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, mux21oa22, 4, &cs2, &bsc3);

        bitslice_module_creator_t bsc4(flat, "mux21i_oai22", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, mux21oai22, 4, &cs2, &bsc4);
    }

    BDD mux21ao221(input_provider_t* e)
    {
        return (e->inp(0) & e->inp(1)) + (e->inp(2) & e->inp(3)) + (e->inp(4));
    }


    BDD mux21aoi221(input_provider_t* e)
    {
        return !( (e->inp(0) & e->inp(1)) + (e->inp(2) & e->inp(3)) + (e->inp(4)) );
    }

    BDD mux21oa221(input_provider_t* e)
    {
        return (e->inp(0) | e->inp(1)) & (e->inp(2) | e->inp(3)) & (e->inp(4));
    }


    BDD mux21oai221(input_provider_t* e)
    {
        return !( (e->inp(0) | e->inp(1)) & (e->inp(2) | e->inp(3)) & (e->inp(4)) );
    }

    void identifyMuxes3(flat_module_t* flat)
    {
        std::cout << "identifying 5-input muxes." << std::endl;

        if(options.kcoverSize < 5) return;

        common_signals_2_t cs2(flat, pred2_true);

        bitslice_module_creator_t bsc1(flat, "mux21_ao221", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, mux21ao221, 5, &cs2, &bsc1);

        bitslice_module_creator_t bsc2(flat, "mux21i_aoi221", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, mux21aoi221, 5, &cs2, &bsc2);

        bitslice_module_creator_t bsc3(flat, "mux21_oa221", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, mux21oa221, 5, &cs2, &bsc3);

        bitslice_module_creator_t bsc4(flat, "mux21i_oai221", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, mux21oai221, 5, &cs2, &bsc4);
    }


    BDD mux31ao222(input_provider_t* e)
    {
        return (e->inp(0) & e->inp(1)) + (e->inp(2) & e->inp(3)) + (e->inp(4) & e->inp(5));
    }

    BDD mux31oa222(input_provider_t* e)
    {
        return (e->inp(0) | e->inp(1)) & (e->inp(2) | e->inp(3)) & (e->inp(4) | e->inp(5));
    }

    BDD mux31aoi222(input_provider_t* e)
    {
        return !((e->inp(0) & e->inp(1)) + (e->inp(2) & e->inp(3)) + (e->inp(4) & e->inp(5)));
    }

    BDD mux31oai222(input_provider_t* e)
    {
        return !((e->inp(0) | e->inp(1)) & (e->inp(2) | e->inp(3)) & (e->inp(4) | e->inp(5)));
    }

    void identify31Muxes2(flat_module_t* flat)
    {
        std::cout << "identifying 3:1 muxes (2)." << std::endl;

        if(options.kcoverSize < 6) return;

        common_signals_3_t cs3(flat, pred3_true);

        bitslice_module_creator_t bsc1(flat, "mux31_ao222", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, mux31ao222, 6, &cs3, &bsc1);

        bitslice_module_creator_t bsc2(flat, "mux31i_aoi222", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, mux31aoi222, 6, &cs3, &bsc2);

        bitslice_module_creator_t bsc3(flat, "mux31_oa222", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, mux31oa222, 6, &cs3, &bsc3);

        bitslice_module_creator_t bsc4(flat, "mux31i_oai222", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, mux31oai222, 6, &cs3, &bsc4);
    }

    BDD and2(input_provider_t* e) { return e->inp(0) & e->inp(1); }
    BDD or2(input_provider_t* e) { return e->inp(0) + e->inp(1); }
    BDD nand2(input_provider_t* e) { return !(e->inp(0) & e->inp(1)); }
    BDD nor2(input_provider_t* e) { return !(e->inp(0) + e->inp(1)); }
    BDD xor2(input_provider_t* e) { return e->inp(0) ^ e->inp(1); }
    BDD xnor2(input_provider_t* e) { return !(e->inp(0) ^ e->inp(1)); }

    void identifyGatingFuncs2(flat_module_t* flat)
    {
        std::cout << "identify gating functions [2]." <<  std::endl;

        if(options.kcoverSize < 2) return;
        simple_common_signal_enumerator_t cs(flat, pred1_true);

        bitslice_module_creator_t bsc_and(flat, "and2gate", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, and2, 2, &cs, &bsc_and);

        bitslice_module_creator_t bsc_or(flat, "or2gate", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, or2, 2, &cs, &bsc_or);

        bitslice_module_creator_t bsc_nand(flat, "nand2gate", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, nand2, 2, &cs, &bsc_nand);

        bitslice_module_creator_t bsc_nor(flat, "nor2gate", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, nor2, 2, &cs, &bsc_nor);

        bitslice_module_creator_t bsc_xor(flat, "xor2gate", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, xor2, 2, &cs, &bsc_xor);

        bitslice_module_creator_t bsc_xnor(flat, "xnor2gate", "sel", options.minMultibitElementSize);
        groupByCommonSignal(flat, xnor2, 2, &cs, &bsc_xnor);
    }

	//PROBABLY WON'T WORK BUT HERE IT GOES:

    BDD multiplyNAND(input_provider_t* e)
    {
        return !(e->inp(0) & e->inp(1));
    }

    void identifyPossibleMultiplies(flat_module_t* flat)
    {
        if(options.kcoverSize < 2) return;

        simple_common_signal_enumerator_t cs1(flat, pred1_true);

        unknown_module_creator_t umc1(flat, "possible_multiply_chain", options.minMultibitElementSize, aggr::id_module_t::CANDIDATE_COMMON_SIGNAL);
        groupByCommonSignal(flat, multiplyNAND, 2, &cs1, &umc1);
    }

    BDD aluXOR2a(input_provider_t* e)
    {
        return ((e->inp(0) ^ e->inp(1)) & (e->inp(2) & e->inp(3) & e->inp(4) & e->inp(5)));
    }

    BDD aluXOR2b(input_provider_t* e)
    {
        return ((e->inp(0) ^ e->inp(1)) & (!e->inp(2) & e->inp(3) & e->inp(4) & e->inp(5)));
    }

    BDD aluXOR2c(input_provider_t* e)
    {
        return ((e->inp(0) ^ e->inp(1)) & (!e->inp(2) & !e->inp(3) & e->inp(4) & e->inp(5)));
    }

    BDD aluXOR2d(input_provider_t* e)
    {
        return ((e->inp(0) ^ e->inp(1)) & (!e->inp(2) & !e->inp(3) & !e->inp(4) & e->inp(5)));
    }

    BDD aluXOR2e(input_provider_t* e)
    {
        return ((e->inp(0) ^ e->inp(1)) & (!e->inp(2) & !e->inp(3) & !e->inp(4) & !e->inp(5)));
    }

    BDD aluXOR2i(input_provider_t* e)
    {
        return !((e->inp(0) ^ e->inp(1)) & (e->inp(2) & e->inp(3) & e->inp(4) & e->inp(5)));
    }

    void identifyALUXOR2(flat_module_t* flat)
    {
        if(options.kcoverSize < 4) return;

        common_signals_4_t cs4(flat, pred4_true);

        //Let's just assume that the ALU XOR operation should be as big as a MUX, at least) Can create own user later.
        bitslice_module_creator_t bsc1(flat, "ALU_XOR2", "fun", options.minMultibitElementSize);
        groupByCommonSignal(flat, aluXOR2a, 6, &cs4, &bsc1);
        groupByCommonSignal(flat, aluXOR2b, 6, &cs4, &bsc1);
        groupByCommonSignal(flat, aluXOR2c, 6, &cs4, &bsc1);
        groupByCommonSignal(flat, aluXOR2d, 6, &cs4, &bsc1);
        groupByCommonSignal(flat, aluXOR2e, 6, &cs4, &bsc1);
    }

    BDD aluXNOR2a(input_provider_t* e)
    {
        return (!(e->inp(0) ^ e->inp(1)) & (e->inp(2) & e->inp(3) & e->inp(4) & e->inp(5)));
    }

    BDD aluXNOR2b(input_provider_t* e)
    {
        return (!(e->inp(0) ^ e->inp(1)) & (!e->inp(2) & e->inp(3) & e->inp(4) & e->inp(5)));
    }

    BDD aluXNOR2c(input_provider_t* e)
    {
        return (!(e->inp(0) ^ e->inp(1)) & (!e->inp(2) & !e->inp(3) & e->inp(4) & e->inp(5)));
    }

    BDD aluXNOR2d(input_provider_t* e)
    {
        return (!(e->inp(0) ^ e->inp(1)) & (!e->inp(2) & !e->inp(3) & !e->inp(4) & e->inp(5)));
    }

    BDD aluXNOR2e(input_provider_t* e)
    {
        return (!(e->inp(0) ^ e->inp(1)) & (!e->inp(2) & !e->inp(3) & !e->inp(4) & !e->inp(5)));
    }

    BDD aluXNOR2i(input_provider_t* e)
    {
        return !(!(e->inp(0) ^ e->inp(1)) & (e->inp(2) & e->inp(3) & e->inp(4) & e->inp(5)));
    }

    void identifyALUXNOR2(flat_module_t* flat)
    {
        if(options.kcoverSize < 4) return;

        common_signals_4_t cs4(flat, pred4_true);

        //Let's just assume that the ALU XNOR operation should be as big as a MUX, at least) Can create own user later.
        bitslice_module_creator_t bsc1(flat, "ALU_XNOR2", "fun", options.minMultibitElementSize);
        groupByCommonSignal(flat, aluXNOR2a, 6, &cs4, &bsc1);
        groupByCommonSignal(flat, aluXNOR2b, 6, &cs4, &bsc1);
        groupByCommonSignal(flat, aluXNOR2c, 6, &cs4, &bsc1);
        groupByCommonSignal(flat, aluXNOR2d, 6, &cs4, &bsc1);
        groupByCommonSignal(flat, aluXNOR2e, 6, &cs4, &bsc1);
    }

    void ram_analysis_user_t::use(const ::nodelist_t& nodes, bitslice_list_t& list)
    {
        if(list.size() < 32) return;

        ::nodelist_t commons;
        get_common_signals(list, commons);

        bool valid = true;
        for(bitslice_list_t::iterator it = list.begin(); it != list.end(); it++) {
            bitslice_t* bs = *it;
            for(unsigned i=0; i != bs->numInputs(); i++) {
                node_t* inp = bs->xs[i];
                int idx = inp->get_index();
                if(markings[idx] & OUTPUT_MARK) continue;
                if(std::find(commons.begin(), commons.end(), inp) != commons.end())
                    continue;
                valid = false;
                break;
            }
            if(!valid) break;
        }

        if(valid) {
            nodeset_t internals;
            std::cout << list << std::endl;
            for(bitslice_list_t::iterator it = list.begin(); it != list.end(); it++) {
                bitslice_t* bs = *it;
                bs->add_internals(bs->ys[0], internals);
                int yi = bs->ys[0]->get_index();
                node_t* y = bs->ys[0];
                std::cout << y->num_fanouts() << " ";
                if((markings[yi] & OUTPUT_MARK) == 0) {
                    found_something = true;
                }
                markings[yi] |= OUTPUT_MARK;
            }
            std::cout << std::endl;
            for(nodeset_t::iterator it = internals.begin(); it != internals.end(); it++) {
                node_t* n = *it;
                int i = n->get_index();
                markings[i] |= INTERNAL_MARK;
            }
        }
    }

    void ram_analysis_user_t::get_common_signals(
        const bitslice_list_t& list, 
        ::nodelist_t& signals
    )
    {
        assert(list.size() > 0);
        bitslice_t* bs = *list.begin();

        signals.clear();
        for(unsigned i=0; i != bs->numInputs(); i++) {
            node_t* n = bs->xs[i];
            if(is_common(n, list)) {
                signals.push_back(n);
            }
        }
    }

    bool ram_analysis_user_t::is_common(const node_t* s, const bitslice_list_t& list)
    {
        for(bitslice_list_t::const_iterator it = list.begin(); it != list.end(); it++) {
            const bitslice_t* bs = *it;
            if(!bs->has_input(s)) return false;
        }
        return true;
    }

    unknown_module_creator_t::unknown_module_creator_t (
        flat_module_t* flat,
        const char* name,
        unsigned minSize,
        id_module_t::type_t t
    )
        : flat(flat)
        , name(strdup(name))
        , minSize(minSize)
        , type(t)
    {
        create_partially_covered_modules = false;
    }

    unknown_module_creator_t::~unknown_module_creator_t()
    {
        free(name);
    }

    void unknown_module_creator_t::use(const ::nodelist_t& nodes, bitslice_list_t& list_in)
    {
        if(list_in.size() < minSize) return;

        bitslice_list_t list;
        for(unsigned i=0; i != list_in.size(); i++) {
            if(!list_in[i]->is_covered() && !list_in[i]->ys[0]->is_latch_gate()) {
                list.push_back(list_in[i]);
            }
        }

        if(list.size() < minSize) return;

        id_module_t* mod = new id_module_t(name, id_module_t::SLICEABLE, type);
        for(unsigned i=0; i != list.size(); i++) {
            for(unsigned y=0; y != list[i]->ys.size(); y++) {
                node_t* n = list[i]->ys[y];
                mod->add_output(n);
            }
        }
        for(unsigned i=0; i != list.size(); i++) {
            for(unsigned x=0; x != list[i]->xs.size(); x++) {
                node_t* n = list[i]->xs[x];
                if(!mod->is_output(n)) {
                    mod->add_input(n);
                }
            }
        }
        if(mod->num_outputs() < minSize) {
            delete mod;
        } else {
            mod->compute_internals();
            std::ostringstream ostr;
            ostr << "common signal(s): "; ::operator<<(ostr, nodes);
            std::string str = ostr.str();
            mod->add_comment(str);
            mod->update_nodes();
            flat->add_module(mod);
        }
    }

    namespace prop_n
    {
        void findPropagationChains(bitslice_list_t& l, bitslice_list_user_i& user)
        {
            // insert all slices into a set.
            bitslice_map_t slices;
            for(unsigned i=0; i != l.size(); i++) {
                node_t* o = l[i]->get_output();
                slices[o].insert(l[i]);
            }

            // now create the edges between the nodes.
            inputs_map_t edges;
            for(unsigned i=0; i != l.size(); i++) {
                bitslice_i* bs = l[i];
                for(unsigned i=0; i != bs->num_inputs(); i++) {
                    node_t* n = bs->get_input(i);
                    bitslice_map_t::iterator it = slices.find(n);
                    if(it != slices.end()) {
                        bitslice_set_t& s = it->second;
                        // insert all the nodes which have this output node into the edge
                        // node set of this bitslice_i
                        std::copy(s.begin(), s.end(), std::inserter(edges[bs], edges[bs].end()));
                    }
                }
            }

            // now all we need to do is a dfs starting from every node that doesn't
            // have edge coming in.
            bitslice_set_t have_edges_in;
            for(inputs_map_t::iterator it = edges.begin(); it != edges.end(); it++) {
                for(bitslice_set_t::iterator jt = it->second.begin(); jt != it->second.end(); jt++) {
                    bitslice_i* pbs = *jt;
                    have_edges_in.insert(pbs);
                }
            }
            for(unsigned i=0; i != l.size(); i++) {
                bitslice_i *pbs = l[i];
                if(have_edges_in.find(pbs) == have_edges_in.end()) {
                    bitslice_list_t l;
                    bitslice_set_t visited;
                    l.push_back(pbs);
                    user.resetTree(pbs);
                    do_dfs(pbs, edges, l, visited, user);
                }
            }
            user.resetTree(NULL);
        }

        void do_dfs(
                bitslice_i* pbs, 
                inputs_map_t& edges, 
                bitslice_list_t& l, 
                bitslice_set_t& visited,
                bitslice_list_user_i& user
            )
        {
            user.useTreeNode(pbs);

            bool found = false;
            inputs_map_t::iterator pos = edges.find(pbs);
            if(pos != edges.end()) {
                bitslice_set_t& s= pos->second;
                for(bitslice_set_t::iterator it = s.begin(); it != s.end(); it++) {
                    bitslice_i *bs = *it;
                    if(visited.find(bs) == visited.end()) {
                        l.push_back(bs);
                        do_dfs(bs, edges, l, visited, user);
                        l.pop_back();
                        found = true;
                        visited.insert(bs);
                    }
                }
            }
            if(!found) {
                user.use(l);
            }
        }
        
        void module_creator_t::resetTree(bitslice_i* r)
        {
            if(internals.size() > 0) {
                assert(root != NULL);

                if(!root->is_latch_gate() && (int)internals.size() >= internalMinSize) {
                    nodeset_t inputs;
                    do_dfs(root, inputs);

                    if(inputs.size() >= options.minMultibitElementSize) {
                        id_module_t* mod = new id_module_t(name.c_str(), id_module_t::UNSLICEABLE, id_module_t::INFERRED);
                        mod->add_output(root);
                        for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
                            node_t* n = *it;
                            mod->add_input(n);
                        }
                        mod->compute_internals();
                        module->add_module(mod);
                    }
                }
            }

            if(r != NULL) root = r->get_output();
            else root = NULL;

            internals.clear();
        }

        void module_creator_t::do_dfs(node_t* n, nodeset_t& inputs)
        {
            if(internals.find(n) == internals.end()) {
                inputs.insert(n);
                return;
            }

            for(node_t::input_iterator it = n->inputs_begin(); it != n->inputs_end(); it++) {
                node_t *inp = *it;
                do_dfs(inp, inputs);
            }
        }

        void module_creator_t::useTreeNode(bitslice_i* r)
        {
            generic_slice_t* gbs = dynamic_cast<generic_slice_t*>(r);
            bitslice_t& bs = gbs->bitslice;
            bs.add_internals(bs.ys[0], internals);
        }

        void destroy_list(bitslice_list_t& l)
        {
            for(unsigned i=0; i != l.size(); i++) {
                delete l[i];
            }
        }

        void findChains(std::string& name, fnlist_t& list, flat_module_t* module, int iminSize)
        {
            bitslice_list_t l;
            for(unsigned i=0; i != list.size(); i++) {
                unsigned s=list[i].second;
                if(s > options.kcoverSize) continue; // silently ignore.

                eval_fun_t e = list[i].first;
                input_provider_t* ipp = module->get_ipp(s);
                BDD b = e(ipp);

                fnInfo_t* info = module->getFunction(b);
                if(info != NULL) {
                    fnInfo_t* i = info->canonicalPtr;
                    for(kcoverset_t::iterator it = i->covers.begin(); it != i->covers.end(); it++) {
                        kcover_t* kc = *it;
                        if(kc->get_root()->is_latch_gate() || 
                           kc->get_root()->get_sibling() ||
                           kc->get_root()->get_sibling_back()) continue;

                        bitslice_t bs(kc);
                        generic_slice_t* gs =new generic_slice_t(bs);
                        l.push_back(gs);
                    }
                }
            }
            // std::cout << "size of the matching bitslice slice list:" << l.size() << std::endl;
            module_creator_t mp(name, module, iminSize);
            findPropagationChains(l, mp);
            destroy_list(l);
        }

        BDD xor2(input_provider_t* e) { return e->inp(0) ^ e->inp(1); }
        BDD xnor2(input_provider_t* e) { return !(e->inp(0) ^ e->inp(1)); }
        BDD xnor3(input_provider_t* e) { return !(e->inp(0) ^ e->inp(1) ^ e->inp(2)); }
        BDD xor3(input_provider_t* e) { return (e->inp(0) ^ e->inp(1) ^ e->inp(2)); }

        void findXorChains(flat_module_t* module)
        {
            std::cout << "identify xor chains." <<  std::endl;

            fnlist_t l;

            l.push_back(fn_t(xor2, 2));
            l.push_back(fn_t(xnor2, 2));
            l.push_back(fn_t(xor3, 3));
            l.push_back(fn_t(xnor3, 3));

            std::string name("xortree");

            findChains(name, l, module, 4);
        }

        BDD and2(input_provider_t* e) { return e->inp(0) & e->inp(1); }
        BDD and3(input_provider_t* e) { return e->inp(0) & e->inp(1) & e->inp(2); }
        BDD and4(input_provider_t* e) { return e->inp(0) & e->inp(1) & e->inp(2) & e->inp(3); }
        BDD and5(input_provider_t* e) { return e->inp(0) & e->inp(1) & e->inp(2) & e->inp(3) & e->inp(4); }
        BDD and6(input_provider_t* e) { return e->inp(0) & e->inp(1) & e->inp(2) & e->inp(3) & e->inp(4) & e->inp(5); }

        void findAndChains(flat_module_t* module)
        {
            std::cout << "identify and chains." <<  std::endl;

            fnlist_t l;
            l.push_back(fn_t(and2, 2));
            l.push_back(fn_t(and3, 3));
            l.push_back(fn_t(and4, 4));
            l.push_back(fn_t(and5, 5));
            l.push_back(fn_t(and6, 6));

            std::string name("andtree");
            findChains(name, l, module, 8);
        }

        BDD or2(input_provider_t* e) { return e->inp(0) | e->inp(1); }
        BDD or3(input_provider_t* e) { return e->inp(0) | e->inp(1) | e->inp(2); }
        BDD or4(input_provider_t* e) { return e->inp(0) | e->inp(1) | e->inp(2) | e->inp(3); }
        BDD or5(input_provider_t* e) { return e->inp(0) | e->inp(1) | e->inp(2) | e->inp(3) | e->inp(4); }
        BDD or6(input_provider_t* e) { return e->inp(0) | e->inp(1) | e->inp(2) | e->inp(3) | e->inp(4) | e->inp(5); }

        void findOrChains(flat_module_t* module)
        {
            std::cout << "identify or chains." <<  std::endl;

            fnlist_t l;
            l.push_back(fn_t(or2, 2));
            l.push_back(fn_t(or3, 3));
            l.push_back(fn_t(or4, 4));
            l.push_back(fn_t(or5, 5));
            l.push_back(fn_t(or6, 6));

            std::string name("ortree");
            findChains(name, l, module, 8);
        }

        BDD cgen(input_provider_t* e) { 
            BDD a = e->inp(0);
            BDD b = e->inp(1);
            BDD c = e->inp(2);
            return (a&b) + (b&c) + (c&a);
        }

        BDD cgeni(input_provider_t* e) {
            BDD a = e->inp(0);
            BDD b = e->inp(1);
            BDD c = e->inp(2);
            return !((a&b) + (b&c) + (c&a));
        }

        void findCgenTrees(flat_module_t* module)
        {
            std::cout << "identify cgen trees." <<  std::endl;

            fnlist_t l;
            l.push_back(fn_t(cgen, 3));
            l.push_back(fn_t(cgeni, 3));

            std::string name("cgentree");
            findChains(name, l, module, 4);
        }
    }
}
