#include "shiftreg.h"
#include "flat_module.h"
#include "counter.h"
#include <sstream>

namespace shiftreg_n
{
    int cnt = 0;

    bool is_shiftreg(nodelist_t& latch_out, nodelist_t& latch_in, unsigned j)
    {
        using namespace sat_n;

        assert(j >= 1);
        assert(j+1 < latch_out.size());
        assert(latch_out.size() == latch_in.size());

        unsigned i = j-1;
        unsigned k = j+1;

        node_t* dj = latch_in[j];
        node_t* dk = latch_in[k];

        const nodeset_t& sj = dj->get_fanin_cone();
        const nodeset_t& sk = dk->get_fanin_cone();

        if(sj.size() == 0 || sk.size() == 0) return false;

        // dj = !r & (se & qi + !se & qj) + s

        cofactor_map_t mij;
        mij[latch_out[i]] = 1;
        mij[latch_out[j]] = 0;

        satchecker_t sat1;

        sat1.addClauses(sj, mij);
        Minisat::Lit dj_lit = sat1.getLit(dj);

        cofactor_map_t mjk;
        mjk[latch_out[j]] = 1;
        mjk[latch_out[k]] = 0;

        sat1.renameGates(sk);
        sat1.addClauses(sk, mjk);
        Minisat::Lit dk_lit = sat1.getLit(dk);

        if(!sat1.areEquiv(dj_lit, dk_lit)) return false;

        mij[latch_out[i]] = 0;
        mij[latch_out[j]] = 1;

        satchecker_t sat2;
        sat2.addClauses(sj, mij);
        Minisat::Lit dj_lit2 = sat2.getLit(dj);

        mjk[latch_out[j]] = 0;
        mjk[latch_out[k]] = 1;

        sat2.renameGates(sk);
        sat2.addClauses(sk, mjk);
        Minisat::Lit dk_lit2 = sat2.getLit(dk);

        return (sat2.areEquiv(dj_lit2, dk_lit2));
    }

    bool shiftreg_t::identical(shiftreg_t& other)
    {
        using namespace sat_n;
        flat_module_t* module = latches[0]->get_module();

        if(size() != other.size()) return false;
        if(find_set() == other.find_set()) return true;

        node_t* q11 = latches[1];
        node_t* q21 = other.latches[1];
        node_t* q10 = latches[0];
        node_t* q20 = other.latches[0];
        node_t* d11 = module->get_latch_input(latches[1]);
        node_t* d21 = module->get_latch_input(other.latches[1]);

        const nodeset_t& s11 = d11->get_fanin_cone();
        const nodeset_t& s21 = d21->get_fanin_cone();
        assert(s11.size() != 0 && s21.size() != 0);

        satchecker_t sat1;

        cofactor_map_t m1;
        m1[q10] = 1;
        m1[q11] = 0;
        sat1.addClauses(s11, m1);
        Minisat::Lit d1_lit = sat1.getLit(d11);

        cofactor_map_t m2;
        m2[q20] = 1;
        m2[q21] = 0;
        sat1.renameGates(s21);
        sat1.addClauses(s21, m2);
        Minisat::Lit d2_lit = sat1.getLit(d21);

        if(!sat1.areEquiv(d1_lit, d2_lit)) return false;

        satchecker_t sat2;
        m1[q10] = 0;
        m1[q11] = 1;
        sat2.addClauses(s11, m1);
        d1_lit = sat2.getLit(d11);

        m2[q20] = 0;
        m2[q21] = 1;
        sat2.renameGates(s21);
        sat2.addClauses(s21, m2);
        d2_lit = sat2.getLit(d21);

        return sat2.areEquiv(d1_lit, d2_lit);
    }

    void find_shiftregs(flat_module_t* module)
    {
        //shiftreg_test(module);
        //return;

        shiftreg_list_t shiftregs;
        nodelist_t& latches = *module->get_latches();
        int pos = 1, total = latches.size();
        for(nodelist_t::iterator it = latches.begin(); it != latches.end(); it++, pos++) {
            node_t* n = *it;
            if(n->is_covered_by_seq()) continue;

            printf("PROGRESS1 [%6d/%6d]: %30s:%6d:%7d\r", pos, total, n->get_name().c_str(), n->num_driving_latches_sp(), (int)shiftregs.size());
            fflush(stdout);

            nodelist_t latch_out;
            nodelist_t latch_in;
            latch_out.push_back(n);
            latch_in.push_back(module->get_latch_input(n));

            const nodeset_t& out_nodes = *n->get_driven_latches();
            for(nodeset_t::const_iterator jt = out_nodes.begin(); jt != out_nodes.end(); jt++) {
                node_t* nxt = *jt;
                if(!should_ignore_latch(nxt) && !drives(nxt, latch_out) && 
                    onepath(n, nxt) && 
                    counters_n::same_group(n, nxt) &&
                    counters_n::count_diff(n, nxt) <= 256) {
                    latch_out.push_back(nxt);
                    latch_in.push_back(module->get_latch_input(nxt));

                    explore(module, shiftregs, latch_out, latch_in);

                    latch_out.pop_back();
                    latch_in.pop_back();
                }
            }
        }

        for(unsigned i=0; i != shiftregs.size(); i++) {
            shiftregs[i].contained = false;
            shiftregs[i].rank = 0;
            shiftregs[i].root = &(shiftregs[i]);
        }

        printf("\n");
        for(unsigned i=0; i != shiftregs.size(); i++) {
            for(unsigned j=0; j != shiftregs.size(); j++) {
                if(i != j && shiftregs[i].contains(shiftregs[j])) {
                    shiftregs[j].contained = true;
                }
            }
        }
        // merge identical slices.
        for(unsigned i=0; i != shiftregs.size(); i++) {
            if(!shiftregs[i].contained) {
                for(unsigned j=0; j != shiftregs.size(); j++) {
                    if(!shiftregs[j].contained) {
                        if(i != j && shiftregs[i].identical(shiftregs[j])) {
                            shiftregs[i].link(shiftregs[j]);
                        }
                    }
                }
            }
        }
        // create a list of latches in each shiftreg.
        root_map_t root_map;
        for(unsigned i=0; i != shiftregs.size(); i++) {
            if(!shiftregs[i].contained) {
                shiftreg_t* rt = shiftregs[i].find_set();
                for(unsigned j=0; j != shiftregs[i].latches.size(); j++) {
                    root_map[rt].push_back(shiftregs[i].latches[j]);
                }
            }
        }

        for(root_map_t::iterator it = root_map.begin(); it != root_map.end(); it++) {
            nodelist_t& latches = it->second;
            aggr::id_module_t* mod = counters_n::create_latch_module(latches, "shiftreg");
            module->add_module(mod);
        }

        std::cout << "# of shift registers: " << root_map.size() << std::endl;
    }

    void shiftreg_t::link(shiftreg_t& other)
    {
        if(rank < other.rank) {
            root = other.root;
        } else {
            other.root = root;
            if(other.rank == rank) {
                rank += 1;
            }
        }
    }

    shiftreg_t* shiftreg_t::find_set()
    {
        if(root != this) {
            root = root->find_set();
        }
        return root;
    }

    std::ostream& operator<<(std::ostream& out, const shiftreg_t& sr)
    {
        out << "shiftreg:" << sr.latches;
        return out;
    }

    bool onepath(node_t* a, node_t* b)
    {
        assert(b->is_driving_latch(a));
        return !(b->is_driving_latch_mp(a));
    }

    void explore(flat_module_t* module, shiftreg_list_t& shiftregs, nodelist_t& latch_out, nodelist_t& latch_in)
    {
        cnt += 1;
        assert(latch_out.size() == latch_in.size());

        node_t *n = latch_out[latch_out.size()-1];
        const nodeset_t& out_nodes = *n->get_driven_latches();
        for(nodeset_t::const_iterator jt = out_nodes.begin(); jt != out_nodes.end(); jt++) {
            node_t* nxt = *jt;
            if(!drives(nxt, latch_out) && 
               !should_ignore_latch(nxt) && 
               onepath(n, nxt) && 
               counters_n::same_group(n, nxt) &&
               counters_n::count_diff(n, nxt) <= 16
               ) 
            {
                latch_out.push_back(nxt);
                latch_in.push_back(module->get_latch_input(nxt));

                if(is_shiftreg(latch_out, latch_in, latch_out.size() - 2)) {
                    shiftreg_t sr(latch_out);
                    shiftregs.push_back(sr);
                    explore(module, shiftregs, latch_out, latch_in);
                }

                latch_out.pop_back();
                latch_in.pop_back();
            }
        }
    }

    bool drives(node_t* n, const nodelist_t& latch_out) 
    {
        for(nodelist_t::const_iterator it = latch_out.begin(); it != latch_out.end(); it++) {
            node_t* l_i = *it;
            if(n->is_driven_latch(l_i)) return true;
        }
        return false;
    }

    bool should_ignore_latch(node_t* n)
    {
        return (n->is_covered_by_seq());
    }


    void shiftreg_test(flat_module_t* module)
    {
        node_t* n1 = module->get_node_by_name("i_hdu_pcarray_10__31_");
        //node_t* n2 = module->get_node_by_name("i_hdu_pcarray_9__31_");
        //node_t* n3 = module->get_node_by_name("i_hdu_pcarray_8__31_");

        kcoverlist_t& kcovs = module->get_latch_input(n1)->get_kcovers();
        for(kcoverlist_t::iterator it = kcovs.begin(); it != kcovs.end(); it++) {
            std::cout << **it << std::endl;
        }

        /*
        nodelist_t latch_outs;
        latch_outs.push_back(n1);
        latch_outs.push_back(n2);
        latch_outs.push_back(n3);

        nodelist_t latch_ins;
        module->get_latch_inputs(latch_outs, latch_ins);

        bool result = is_shiftreg(latch_outs, latch_ins, 1);
        std::cout << "is_shiftreg " << latch_outs << ": ";
        std::cout << (result ? "yes" : "no") << std::endl;
        */
    }

    bool shiftreg_t::contains(const shiftreg_t& other) const
    {
        for(nodelist_t::const_iterator it = other.latches.begin(); it != other.latches.end(); it++) {
            node_t* n = *it;
            if(std::find(latches.begin(), latches.end(), n) == latches.end()) {
                return false;
            }
        }
        return true;
    }
}
