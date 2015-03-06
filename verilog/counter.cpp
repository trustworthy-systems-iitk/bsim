#include <iostream>
#include <iomanip>
#include <sys/time.h>
#include "counter.h"
#include "main.h"
#include "node.h"
#include "sat.h"

void find_counters(nodelist_t& latches_in)
{
    using namespace counters_n;
    nodelist_t latches;
    for(nodelist_t::iterator it = latches_in.begin(); it != latches_in.end(); it++) {
        node_t* n = *it;
        if(!n->has_self_loop()) continue;
        if(should_ignore_latch(n)) continue;
        else {
            latches.push_back(n);
        }
    }

    //if(latches.size() == 0) return;

    //flat_module_t* m = latches[0]->get_module();

    // bit_select_1 
    // buff_4__2 
    // pc_buf_2 
    // pc_buf_3 
    // pc_buf_5 
    // pc_buf_10
    // pc_buf_13
    /*
    node_t* n0 = m->get_node_by_name("bit_select_1");
    node_t* n1 = m->get_node_by_name("buff_4__2");
    node_t* n2 = m->get_node_by_name("pc_buf_2");
    node_t* n3 = m->get_node_by_name("pc_buf_3");
    node_t* n4 = m->get_node_by_name("pc_buf_5");
    node_t* n5 = m->get_node_by_name("pc_buf_10");

    nodelist_t l;
    l.push_back(n0);
    l.push_back(n1);
    l.push_back(n2);
    l.push_back(n3);
    l.push_back(n4);
    l.push_back(n5);

    if(counters_n::is_counter(l)) {
        std::cout << "is_counter: " << l << std::endl;
    } else {
        std::cout << "notcounter: " << l << std::endl;
    }
    */
    

    {
        // MARK: fpu counter
        /*
        node_t* n0 = m->get_node_by_name("count_ready_0");
        node_t* n1 = m->get_node_by_name("count_ready_1");
        node_t* n2 = m->get_node_by_name("count_ready_2");
        node_t* n3 = m->get_node_by_name("count_ready_3");
        nodelist_t l;
        l.push_back(n0);
        l.push_back(n1);
        l.push_back(n2);
        l.push_back(n3);

        if(counters_n::is_counter(l)) {
            std::cout << "is_counter: " << l << std::endl;
        } else {
            std::cout << "notcounter: " << l << std::endl;
        }
        */
    }

    /*
    // TODO:
    // the second cofactor check fails for this counter.
    // we'll need to reverify after fixing the issue with
    // scan flip-flops.

    node_t* n0 = m->get_node_by_name("count_cycles_0");
    node_t* n1 = m->get_node_by_name("count_cycles_1");
    node_t* n2 = m->get_node_by_name("count_cycles_2");
    node_t* n3 = m->get_node_by_name("count_cycles_3");
    nodelist_t l;
    l.push_back(n0);
    l.push_back(n1);
    l.push_back(n2);
    l.push_back(n3);

    if(counters_n::is_counter(l)) {
        std::cout << "is_counter: " << l << std::endl;
    } else {
        std::cout << "notcounter: " << l << std::endl;
    }
    */

    // MARK1
    /*
    {
        node_t* n0 = m->get_node_by_name("cycle_0");
        node_t* n1 = m->get_node_by_name("cycle_1");
        nodelist_t l;
        l.push_back(n0);
        l.push_back(n1);
        if(counters_n::is_counter(l)) {
            std::cout << l << ": is a counter." << std::endl;
        } else {
            std::cout << l << ": not a counter." << std::endl;
        }
    }

    {
        node_t* n0 = m->get_node_by_name("acc_0");
        node_t* n1 = m->get_node_by_name("buff_4__0");
        nodelist_t l;
        l.push_back(n0);
        l.push_back(n1);
        if(counters_n::is_counter(l)) {
            std::cout << l << ": is a counter." << std::endl;
        } else {
            std::cout << l << ": not a counter." << std::endl;
        }
    }

    {
        node_t* n0 = m->get_node_by_name("re_count_0");
        node_t* n1 = m->get_node_by_name("re_count_1");
        node_t* n2 = m->get_node_by_name("re_count_2");
        node_t* n3 = m->get_node_by_name("re_count_3");
        nodelist_t l;
        l.push_back(n0);
        l.push_back(n1);
        l.push_back(n2);
        l.push_back(n3);
        if(counters_n::is_counter(l)) {
            std::cout << l << ": is a counter." << std::endl;
        } else {
            std::cout << l << ": not a counter." << std::endl;
        }
    }
    */

    // MARK3
    /*
    {
        node_t* n0 = m->get_node_by_name("dividend_reg_1");
        node_t* n1 = m->get_node_by_name("dividend_reg_52");
        node_t* n2 = m->get_node_by_name("dividend_reg_3");
        node_t* n3 = m->get_node_by_name("dividend_reg_5");
        nodelist_t l;
        l.push_back(n0);
        l.push_back(n1);
        l.push_back(n2);
        l.push_back(n3);
        if(counters_n::is_counter(l, true, false, 1)) {
            std::cout << l << ": is a counter." << std::endl;
        } else {
            std::cout << l << ": not a counter." << std::endl;
        }
    }
    */

    /*
    node_t* n0 = m->get_node_by_name("tr_count_0");
    node_t* n1 = m->get_node_by_name("tr_count_1");
    node_t* n2 = m->get_node_by_name("tr_count_2");
    node_t* n3 = m->get_node_by_name("tr_count_3");
    nodelist_t l;
    l.push_back(n0);
    l.push_back(n1);
    l.push_back(n2);
    l.push_back(n3);
    if(counters_n::is_counter(l)) {
        std::cout << l << ": is a counter." << std::endl;
    } else {
        std::cout << l << ": not a counter." << std::endl;
    }
    */

    //node_t* n4 = m->get_node_by_name("scon_4");
    /*
    nodelist_t l;
    l.push_back(n0);
    l.push_back(n1);
    //l.push_back(n2);
    //l.push_back(n3);
    //l.push_back(n4);
    if(counters_n::is_counter(l)) {
        std::cout << l << ": is a counter." << std::endl;
    } else {
        std::cout << l << ": not a counter." << std::endl;
    }

    // acc_0 buff_4__0 wr_data_r_0 pc_buf_1
    flat_module_t* m = latches[0]->get_module();
    node_t* n0 = m->get_node_by_name("acc_0");
    node_t* n1 = m->get_node_by_name("wr_data_r_0");
    // node_t* n2 = m->get_node_by_name("pc_buf_1");
    nodelist_t l;
    l.push_back(n0);
    l.push_back(n1);
    // l.push_back(n2);
    counters_n::is_counter(l);
    */
    
    // MARK: real code
    const unsigned MAX=INT_MAX;
    unsigned i=0;
    unsigned m=std::min<unsigned>(MAX,latches.size());
    counterlist_t counters;
    for(nodelist_t::iterator it = latches.begin(); it != latches.end(); it++) {
        node_t* n = *it;
        if(i++ >= MAX) break;

        (void) m;
        printf("PROGRESS2: %6d/%6d latches [count=%10d]\r", 
               i, m, counters_n::potential_counters_found); // n->get_name().c_str());
        fflush(stdout);

        counter_state_t cs(n);
        lengthen(cs, counters);
    }
    printf("\n");
    printf("# of potential counters found: %d\n", counters_n::potential_counters_found);
    printf("sz of counter list: %d\n", (int)counters.size());
    outputCounters(counters);


    /*
    node_t * n = m->get_node_by_name("re_count_0");
    counterlist_t counters;
    counter_state_t cs(n);
    lengthen(cs, counters);
    */

    /*
    node_t * n = m->get_node_by_name("cdone");
    counterlist_t counters;
    counter_state_t cs(n);
    lengthen(cs, counters);
    */

    /*
    node_t * n = m->get_node_by_name("count_ready_0");
    counterlist_t counters;
    counter_state_t cs(n);
    lengthen(cs, counters);
    */

    /*
    node_t * n = m->get_node_by_name("ex_desto[0]");
    counterlist_t counters;
    counter_state_t cs(n);
    lengthen(cs, counters);
    */

    // MARK2
    /*
    node_t * n = m->get_node_by_name("dividend_reg_1");
    counterlist_t counters;
    counter_state_t cs(n);
    lengthen(cs, counters);
    */
}

namespace counters_n 
{
    unsigned potential_counters_found = 0;

    /*
        DEBUG: UPDOWN alfd25821 { amdn26455 amal26375 xxi16830 urc14640 ssp13327 szg13500 sxy13466 akuj25541 svh13397 sve13394 svw13412 svy13414 svz13415 atgy31276 swb13417 suc13366 tcb13573 szr13511 sxr13459 syq13484 svq13406 tcx13595 njad252827 njaf252829 njag252830 njah252831 njai252832 njaj252833 njak252834 njal252835 njam252836 njan252837 njao252838 njaq252840 njar252841 njyz253473 njat252843 njau252844 njav252845 njax252847 xfvp426441_16_ xfvp426441_17_ xfvp426441_26_ njrc253268 njrq253282 nkim253720 njrr253283 njse253296 njrs253284 njsf253297 nkhx253705 njsg253298 njsc253294 nkoi253872 njry253290 njsb253293 njrv253287 njsa253292 njrz253291 }
        DEBUG: UPDOWN amal26375 { alfd25821 amdn26455 xxi16830 urc14640 ssp13327 szg13500 sxy13466 akuj25541 svh13397 sve13394 svw13412 svy13414 svz13415 atgy31276 swb13417 suc13366 tcb13573 szr13511 sxr13459 syq13484 svq13406 tcx13595 njad252827 njaf252829 njag252830 njah252831 njai252832 njaj252833 njak252834 njal252835 njam252836 njan252837 njao252838 njaq252840 njar252841 njyz253473 njat252843 njau252844 njav252845 njax252847 xfvp426441_16_ xfvp426441_17_ xfvp426441_24_ njrc253268 njrq253282 nkim253720 njrr253283 njse253296 njrs253284 njsf253297 nkhx253705 njsg253298 njsc253294 nkoi253872 njry253290 njsb253293 njrv253287 njsa253292 njrz253291 }
        DEBUG: UPDOWN amdn26455 { alfd25821 amal26375 xxi16830 urc14640 ssp13327 szg13500 sxy13466 akuj25541 svh13397 sve13394 svw13412 svy13414 svz13415 atgy31276 swb13417 suc13366 tcb13573 szr13511 sxr13459 syq13484 svq13406 tcx13595 njad252827 njaf252829 njag252830 njah252831 njai252832 njaj252833 njak252834 njal252835 njam252836 njan252837 njao252838 njaq252840 njar252841 njyz253473 njat252843 njau252844 njav252845 njax252847 xfvp426441_16_ xfvp426441_17_ njzu253494 njrc253268 njrq253282 nkim253720 njrr253283 njse253296 njrs253284 njsf253297 nkhx253705 njsg253298 njsc253294 nkoi253872 njry253290 njsb253293 njrv253287 njsa253292 njrz253291 }
    */

    bool shd_dbg(node_t* n) {
        // rd addr: alfd25821 amdn26455 amal26375 
        if(n->get_name() == "alfd25821") { return true; } 
        else if(n->get_name() == "amdn26455") { return true; }
        else if(n->get_name() == "amal26375") { return true; }
        else { return false; }
    }


    bool should_ignore_latch(node_t* n)
    {
        std::string name(n->get_name());
        if( (options.caLatchesToIgnore.find(name) != options.caLatchesToIgnore.end()) ) {
            return true;
        } else {
            return (n->is_covered_by_seq());
        }
    }

    counter_state_t::counter_state_t(node_t* head)
    {
        up = true;
        down = true;

        latches.resize(1);
        latch_inputs.resize(1);

        flat = head->get_module();
        latches[0] = head;
        latch_inputs[0] = flat->get_latch_input(head);

        const nodeset_t* s = head->get_driven_latches();
        for(nodeset_t::const_iterator it = s->begin(); it != s->end(); it++)
        {
            node_t* n = *it;
            if(n == head) continue;
            if(should_ignore_latch(n)) continue;
            if(!same_group(head, n)) continue;
            if(count_diff(n, head) >= 16) continue;
            available_latches.insert(n);
        }
    }

    counter_state_t::counter_state_t(counter_state_t& other, node_t* next)
    {
        up = false;
        down = false;

        assert(other.available_latches.find(next) != other.available_latches.end());

        // initialize the list of latches.
        latches.resize(other.latches.size() + 1);
        std::copy(other.latches.begin(), other.latches.end(), latches.begin());
        latches[other.latches.size()] = next;

        // get the driven latches for this node.
        const nodeset_t* s = next->get_driven_latches();

        // initialize available nodes set.
        for(nodeset_t::const_iterator it  = s->begin(); 
                                      it != s->end();
                                      it++)
        {
            node_t* n = *it;
            if(std::find(latches.begin(), latches.end(), n) == latches.end() && 
               other.is_fully_reachable(n) &&
               n->has_self_loop() &&
               same_group(n, next) &&
               count_diff(n, next) < 16 &&
               !should_ignore_latch(n))
            {
                available_latches.insert(n);
            }
        }

    }

    counter_state_t::~counter_state_t()
    {
    }

    bool same_group(node_t* l1, node_t* l2)
    {
        if(l1->get_clktrees() != l2->get_clktrees()) return false;
        return (abs((int)l1->get_rsttrees() - (int)l2->get_rsttrees()) <= 1);
    }

    bool counter_state_t::is_fully_reachable(node_t* latch)
    {
        for(unsigned i=0; i != latches.size(); i++) {
            node_t* n = latches[i];
            const nodeset_t* s = n->get_driven_latches();
            if(s->find(latch) == s->end()) return false;
        }
        return true;
    }

    bool counter_state_t::no_back_loops(node_t* n)
    {
        const nodeset_t* s = n->get_driven_latches();
        for(unsigned i=0; i != latches.size(); i++) {
            if(s->find(latches[i]) != s->end()) {
                return false;
            }
        }
        return true;
    }

    void lengthen(counter_state_t& state, counterlist_t& counters)
    {
        if(state.available_latches.size() > 0) {
            if(state.latches.size() <= options.caMaxCounterSize) {
                for(nodeset_t::iterator it = state.available_latches.begin();
                                        it != state.available_latches.end();
                                        it++)
                {
                    counter_state_t new_state(state, *it);
                    if(state.up && state.down) {
                        if(is_counter(new_state.latches, true, false, new_state.fanin_cone, new_state.latches.size()-1)) {
                            counter_t c(new_state, true, false);
                            counters.push_back(c);
                            potential_counters_found += 1;
                            new_state.up = true;
                            new_state.down = false;
                            lengthen(new_state, counters);
                        }
                        if(is_counter(new_state.latches, false, true, new_state.fanin_cone, new_state.latches.size()-1)) {
                            counter_t c(new_state, false, true);
                            counters.push_back(c);
                            potential_counters_found += 1;
                            new_state.up = false;
                            new_state.down = true;
                            lengthen(new_state, counters);
                        }
                    } else {
                        assert(state.up || state.down);
                        if(is_counter(new_state.latches, state.up, state.down, new_state.fanin_cone, new_state.latches.size()-1)) {
                            counter_t c(new_state, state.up, state.down);
                            counters.push_back(c);
                            potential_counters_found += 1;
                            new_state.up = state.up;
                            new_state.down = state.down;
                            lengthen(new_state, counters);
                        }
                    }
                }
            } else {
                // pass
            }
        }
    }

    std::ostream& operator<<(std::ostream& out, const counter_state_t& state)
    {
        unsigned last = state.latches.size() - 1;
        unsigned pos = 0;

        if(state.up) out << "UP";
        if(state.down) out << "DOWN";
        out << " ";

        for(nodelist_t::const_iterator it  = state.latches.begin(); 
                                       it != state.latches.end();
                                       it++)
        {
            node_t* n = *it;
            out << n->get_name();
            if(pos++ != last) {
                out << " ";
            }
        }
        out << " { ";
        for(nodeset_t::const_iterator it =  state.available_latches.begin();
                                      it != state.available_latches.end();
                                      it++)
        {
            node_t* n = *it;
            out << n->get_name() << " ";
        }
        out << "}";
        return out;
    }

    bool is_driven_by(node_t* n, nodelist_t& latches)
    {
        const nodeset_t* s = n->get_driving_latches();
        for(unsigned i=0; i < latches.size(); i++) {
            node_t* n = latches[i];
            if(s->find(n) != s->end()) return true;
        }
        return false;
    }

    void get_truncated_fanin_cone(node_t* n, const nodeset_t& common_cone, nodeset_t& trunc_cone)
    {
        nodeset_t inputs;

        trunc_cone.clear();
        flat_module_t::find_inputs(n, common_cone, trunc_cone, inputs);
    }

    bool is_upcounter_t0(nodelist_t& latch_out, nodelist_t& latch_in, const nodeset_t& common_cone, unsigned i)
    {
        using namespace sat_n;
        node_t* d0 = latch_in[0];
        node_t* di = latch_in[i];

        const nodeset_t& s0 = d0->get_fanin_cone();
        const nodeset_t& si = di->get_fanin_cone();
        /*
        nodeset_t s0;
        nodeset_t si;

        get_truncated_fanin_cone(d0, common_cone, s0);
        get_truncated_fanin_cone(di, common_cone, si);
        */

        /*
        std::cout << "latches: " << latch_out << std::endl;
        std::cout << "common: " << common_cone << std::endl;
        std::cout << "s0: " << s0 << std::endl;
        std::cout << "si: " << si << std::endl;
        */

        if(s0.size() == 0 || si.size() == 0) return false;
        cofactor_map_t m0, mi;
        for(unsigned j=0; j < i; j++) {
            mi[latch_out[j]] = 1;
        }
        mi[latch_out[i]] = 0;
        m0[latch_out[0]] = 0;

        satchecker_t sat1;
        sat1.addClauses(s0, m0);

        sat1.renameGates(si);
        sat1.addClauses(si, mi);

        if(sat1.areEquiv(d0, di)) {
            return true;
        } else {
            return false;
        }
    }

    bool is_upcounter_t1(nodelist_t& latch_out, nodelist_t& latch_in, const nodeset_t& common_cone, unsigned i)
    {

        using namespace sat_n;
        node_t* d0 = latch_in[0];
        node_t* di = latch_in[i];

        const nodeset_t& s0 = d0->get_fanin_cone();
        const nodeset_t& si = di->get_fanin_cone();

        if(s0.size() == 0 || si.size() == 0) return false;
        cofactor_map_t m0, mi;
        for(unsigned j=0; j < i; j++) {
            mi[latch_out[j]] = 1;
        }
        mi[latch_out[i]] = 1;
        m0[latch_out[0]] = 1;

        satchecker_t sat1;
        sat1.addClauses(s0, m0);

        sat1.renameGates(si);
        sat1.addClauses(si, mi);

        if(sat1.areEquiv(d0, di)) {
            return true;
        } else {
            return false;
        }
    }

    bool is_upcounter_t2(nodelist_t& latch_out, nodelist_t& latch_in, const nodeset_t& common_cone, unsigned i)
    {

        using namespace sat_n;

        node_t* di = latch_in[i];
        const nodeset_t& si = di->get_fanin_cone();
        if(si.size() == 0) return false;

        // set the last latch output to 1.
        cofactor_map_t cfi;
        cfi[latch_out[i]] = 1;
        
        // add the clauses representing the full function.
        satchecker_t sat;
        sat.addClauses(si, cfi);

        // now create a clause which says at least one of the remaining latch outputs is zero.
        clause_t restr_clause;
        for(unsigned pos=0; pos < i; pos++) {
            lit_t l(latch_out[pos], ZERO);
            restr_clause.push_back(l);
        }
        sat.addClause(restr_clause, cfi);

        // now we want to rename the d input.
        Minisat::Lit old_di, new_di;
        sat.renameLit(di, old_di, new_di);

        // and then we rename the qi's.
        for(unsigned pos=0; pos < i; pos++) {
            Minisat::Lit old_lit, new_lit;
            sat.renameLit(latch_out[pos], old_lit, new_lit);
        }

        // now add the clauses again.
        sat.addClauses(si, cfi);
        sat.addClause(restr_clause, cfi);

        // now check if these two are the same.
        if(sat.areEquiv(old_di, new_di)) {
            return true;
        } else {
            return false;
        }
    }
    
    bool is_upcounter_t3(nodelist_t& latch_out, nodelist_t& latch_in, const nodeset_t& common_cone, unsigned i)
    {

        using namespace sat_n;

        if(i < 2) return true;

        // part 1: handle d_i
        node_t* di = latch_in[i];
        const nodeset_t& si = di->get_fanin_cone();

        if(si.size() == 0) return false;

        // set the last latch output to 1.
        cofactor_map_t cfi;
        cfi[latch_out[i]] = 1;
        
        // add the clauses representing the full function.
        satchecker_t sat;
        sat.addClauses(si, cfi);

        // now create a clause which says at least one of the remaining latch outputs is zero.
        clause_t restr_clause_i;
        for(unsigned pos=0; pos < i; pos++) {
            lit_t l(latch_out[pos], ZERO);
            restr_clause_i.push_back(l);
        }
        sat.addClause(restr_clause_i, cfi);

        // part 2: handle d_j | j = i-1
        unsigned j = i-1;
        node_t* dj = latch_in[j];
        const nodeset_t& sj = dj->get_fanin_cone();
        if(sj.size() == 0) return false;

        // set the last latch output to 1.
        cofactor_map_t cfj;
        cfj[latch_out[j]] = 1;
        
        // first rename the Qs coz we want to allow them to have different values.
        for(unsigned pos=0; pos < i; pos++) {
            Minisat::Lit old_lit, new_lit;
            sat.renameLit(latch_out[pos], old_lit, new_lit);
        }

        // add the clauses representing the full function.
        sat.addClauses(sj, cfj);
        
        // now create a clause which says at least one of the remaining latch outputs is zero.
        clause_t restr_clause_j;
        for(unsigned pos=0; pos < j; pos++) {
            lit_t l(latch_out[pos], ZERO);
            restr_clause_j.push_back(l);
        }
        sat.addClause(restr_clause_j, cfj);

        // now check if these two are the same.
        if(sat.areEquiv(di, dj)) {
            return true;
        } else {
            return false;
        }
    }

    bool is_upcounter_t4(nodelist_t& latch_out, nodelist_t& latch_in, const nodeset_t& common_cone, unsigned i)
    {
        using namespace sat_n;
        using namespace Minisat;

        node_t* di = latch_in[i];
        const nodeset_t& si = di->get_fanin_cone();
        if(si.size() == 0) return false;

        satchecker_t sat;

        // add the clauses representing Di with cofactor 11...10.
        cofactor_map_t cfi;
        for(unsigned j=0; j < i; j++) {
            cfi[latch_out[j]] = 1;
        }
        cfi[latch_out[i]] = 0;
        sat.addClauses(si, cfi);

        // rename Di.
        Lit lit_di0, lit_di1;
        sat.renameLit(di, lit_di0, lit_di1);

        // add the clauses representing Di with cofactor 11...11.
        cfi[latch_out[i]] = 1;
        sat.addClauses(si, cfi);

        // now add the OR of these two.
        // first need a new literal from the solver.
        Lit lit_y = sat.getNewLit();
        sat.addClause(lit_di0, ~lit_y);
        sat.addClause(lit_di1, ~lit_y);
        sat.addClause(~lit_di0, ~lit_di1, lit_y);

        // rename Di yet again.
        Lit lit_di2, lit_di3;
        sat.renameLit(di, lit_di2, lit_di3);
        assert(lit_di2 == lit_di1);

        // reset the cofactor map.
        cfi.clear();
        cfi[latch_out[i]] = 1;
        sat.addClauses(si, cfi);

        // now add the constraint clause (!q0 + !q1 + ... + !q{i-1})
        clause_t restr_clause;
        for(unsigned pos=0; pos < i; pos++) {
            lit_t l(latch_out[pos], ZERO);
            restr_clause.push_back(l);
        }
        sat.addClause(restr_clause, cfi);

        // now check that lit_di3 == lit_y
        return sat.areEquiv(lit_di3, lit_y);
    }

    bool is_downcounter_t0(nodelist_t& latch_out, nodelist_t& latch_in, const nodeset_t& common_cone, unsigned i)
    {

        using namespace sat_n;
        node_t* d0 = latch_in[0];
        node_t* di = latch_in[i];

        const nodeset_t& s0 = d0->get_fanin_cone();
        const nodeset_t& si = di->get_fanin_cone();

        if(s0.size() == 0 || si.size() == 0) return false;
        cofactor_map_t m0, mi;
        for(unsigned j=0; j < i; j++) {
            mi[latch_out[j]] = 0;
        }
        mi[latch_out[i]] = 0;
        m0[latch_out[0]] = 0;

        satchecker_t sat1;
        sat1.addClauses(s0, m0);

        sat1.renameGates(si);
        sat1.addClauses(si, mi);

        return sat1.areEquiv(d0, di);
    }


    bool is_downcounter_t1(nodelist_t& latch_out, nodelist_t& latch_in, const nodeset_t& common_cone, unsigned i)
    {

        using namespace sat_n;
        node_t* d0 = latch_in[0];
        node_t* di = latch_in[i];

        const nodeset_t& s0 = d0->get_fanin_cone();
        const nodeset_t& si = di->get_fanin_cone();

        if(s0.size() == 0 || si.size() == 0) return false;
        cofactor_map_t m0, mi;
        for(unsigned j=0; j < i; j++) {
            mi[latch_out[j]] = 0;
        }
        mi[latch_out[i]] = 1;
        m0[latch_out[0]] = 1;

        satchecker_t sat1;
        sat1.addClauses(s0, m0);

        sat1.renameGates(si);
        sat1.addClauses(si, mi);

        return sat1.areEquiv(d0, di);
    }

    bool is_downcounter_t2(nodelist_t& latch_out, nodelist_t& latch_in, const nodeset_t& common_cone, unsigned i)
    {

        using namespace sat_n;

        node_t* di = latch_in[i];
        const nodeset_t& si = di->get_fanin_cone();
        if(si.size() == 0) return false;

        // set the last latch output to 1.
        cofactor_map_t cfi;
        cfi[latch_out[i]] = 1;
        
        // add the clauses representing the full function.
        satchecker_t sat;
        sat.addClauses(si, cfi);

        // now create a clause which says at least one of the remaining latch outputs is zero.
        clause_t restr_clause;
        for(unsigned pos=0; pos < i; pos++) {
            lit_t l(latch_out[pos], ONE);
            restr_clause.push_back(l);
        }
        sat.addClause(restr_clause, cfi);

        // now we want to rename the d input.
        Minisat::Lit old_di, new_di;
        sat.renameLit(di, old_di, new_di);

        // and then we rename the qi's.
        for(unsigned pos=0; pos < i; pos++) {
            Minisat::Lit old_lit, new_lit;
            sat.renameLit(latch_out[pos], old_lit, new_lit);
        }

        // now add the clauses again.
        sat.addClauses(si, cfi);
        sat.addClause(restr_clause, cfi);

        // now check if these two are the same.
        return sat.areEquiv(old_di, new_di);
    }
    
    bool is_downcounter_t3(nodelist_t& latch_out, nodelist_t& latch_in, const nodeset_t& common_cone, unsigned i)
    {

        using namespace sat_n;

        if(i < 2) return true;

        // part 1: handle d_i
        node_t* di = latch_in[i];
        const nodeset_t& si = di->get_fanin_cone();
        if(si.size() == 0) return false;

        // set the last latch output to 1.
        cofactor_map_t cfi;
        cfi[latch_out[i]] = 1;
        
        // add the clauses representing the full function.
        satchecker_t sat;
        sat.addClauses(si, cfi);

        // now create a clause which says at least one of the remaining latch outputs is zero.
        clause_t restr_clause_i;
        for(unsigned pos=0; pos < i; pos++) {
            lit_t l(latch_out[pos], ONE);
            restr_clause_i.push_back(l);
        }
        sat.addClause(restr_clause_i, cfi);

        // part 2: handle d_j | j = i-1
        unsigned j = i-1;
        node_t* dj = latch_in[j];
        const nodeset_t& sj = dj->get_fanin_cone();
        if(sj.size() == 0) return false;

        // set the last latch output to 1.
        cofactor_map_t cfj;
        cfj[latch_out[j]] = 1;
        
        // first rename the Qs coz we want to allow them to have different values.
        for(unsigned pos=0; pos < i; pos++) {
            Minisat::Lit old_lit, new_lit;
            sat.renameLit(latch_out[pos], old_lit, new_lit);
        }

        // add the clauses representing the full function.
        sat.addClauses(sj, cfj);
        
        // now create a clause which says at least one of the remaining latch outputs is zero.
        clause_t restr_clause_j;
        for(unsigned pos=0; pos < j; pos++) {
            lit_t l(latch_out[pos], ONE);
            restr_clause_j.push_back(l);
        }
        sat.addClause(restr_clause_j, cfj);

        // now check if these two are the same.
        return sat.areEquiv(di, dj);
    }
    
    bool is_counter(nodelist_t& latch_out, bool up, bool down, const nodeset_t& common_cone, unsigned i)
    {
        if(latch_out.size() == 0) return false;
        assert(latch_out.size() >= 2);

        // find that flat module.
        flat_module_t *module = latch_out[0]->get_module();

        nodelist_t latch_in;
        module->get_latch_inputs(latch_out, latch_in);
        
        if(up && is_upcounter_t0(latch_out, latch_in, common_cone, i) && 
                 is_upcounter_t1(latch_out, latch_in, common_cone, i) &&
                 is_upcounter_t2(latch_out, latch_in, common_cone, i) && 
                 is_upcounter_t3(latch_out, latch_in, common_cone, i))
        {
            return true;
        }

        if(down && is_downcounter_t0(latch_out, latch_in, common_cone, i) && 
                   is_downcounter_t1(latch_out, latch_in, common_cone, i) &&
                   is_downcounter_t2(latch_out, latch_in, common_cone, i) &&
                   is_downcounter_t3(latch_out, latch_in, common_cone, i)) 
        {
            return true;
        }

        return false;
    }

    bool is_counter(nodelist_t& latch_out)
    {
        // compute latch inputs.
        nodelist_t latch_in;
        flat_module_t *module = latch_out[0]->get_module();
        module->get_latch_inputs(latch_out, latch_in);

        // compute common cone.
        nodeset_t common_cone(latch_in[0]->get_fanin_cone());
        for(unsigned i=1; i != latch_out.size(); i++) {
            node_t* li = latch_in[i];
            inplace_intersection(common_cone, li->get_fanin_cone());
        }
        prune_common_cone(common_cone, latch_out);

        for(unsigned i=1; i != latch_out.size(); i++) {
            if(!is_counter(latch_out, true, true, common_cone, i)) {
                return false;
            }
        }
        assert(latch_out.size() >= 2);
        return true;
    }

    counter_t::counter_t(counter_state_t& cs, bool up, bool down)
        : latches(cs.latches)
        , contained(false)
        , up(up)
        , down(down)
        , next(-1)
    {
        assert((up && !down) || (!up && down));
    }

    counter_t::~counter_t() {}

    counter_t::counter_t(const counter_t& a)
        : latches(a.latches)
        , contained(false)
        , up(a.up)
        , down(a.down)
        , next(-1)
    {}

    bool counter_t::contains(const node_t* n) const
    {
        nodelist_t::const_iterator pos = std::find(latches.begin(), latches.end(), n);
        return (pos != latches.end());
    }

    bool counter_t::contains(const counter_t& o) const
    {
        for(nodelist_t::const_iterator it = o.latches.begin(); it != o.latches.end(); it++) {
            node_t* n = *it;
            if(!contains(n)) return false;
        }
        return true;
    }

    bool counter_t::overlaps(const counter_t& other) const
    {
        if(other.latches.size() < 2) return false;
        if(other.latches.size() > latches.size()) return false;

        for(unsigned i=0; i < other.latches.size()-1; i++) {
            node_t* li = other.latches[i];
            if(latches[i+1+latches.size()-other.latches.size()] != li) return false;
        }
        return true;
    }

    void counter_t::merge(const counter_t& other)
    {
#if 0
        if(!overlaps(other)) {
            std::cout << "this:" << *this << std::endl;
            std::cout << "that:" << other << std::endl;
        }
        assert(overlaps(other));
#endif

        assert(other.latches.size() >= 2);
        unsigned last_index = other.latches.size() - 1;
        if(std::find(latches.begin(), latches.end(), other.latches[last_index]) == latches.end()) {
            latches.push_back(other.latches[last_index]);
        }
    }

    std::ostream& operator<<(std::ostream& out, const counter_t& c)
    {
        out << "counter:" << c.latches;
        return out;
    }

    void markContained(counterlist_t& cs)
    {
        for(unsigned i=0; i != cs.size(); i++) {
            cs[i].contained = false;
        }
        for(unsigned i=0; i != cs.size(); i++) {
            counter_t& ci = cs[i];
            for(unsigned j=0; j != cs.size(); j++) {
                counter_t& cj = cs[j];
                if(i != j && ci.contains(cj)) {
                    cj.contained = true;
                }
            }
        }
    }

    bool counter_t::same(const counter_t& other) const
    {
        if(latches.size() != other.latches.size()) return false;

        for(unsigned i=0; i != latches.size(); i++) {
            node_t* l1i = latches[i];
            node_t* l2i = other.latches[i];
            if(l1i != l2i) return false;
        }
        return true;
    }

    void addOverlaps(counterlist_t& cs)
    {
        for(unsigned i=0; i != cs.size(); i++) {
            counter_t& ci = cs[i];
            for(unsigned j=0; j != cs.size(); j++) {
                counter_t& cj = cs[j];
                if(i != j && ci.overlaps(cj)) {
                    ci.next = j;
                }
            }
        }

        for(unsigned i=0; i != cs.size(); i++) {
            counter_t& ci = cs[i];
            int next = ci.next;
#if 0
            if(next != -1) {
                std::cout << "old ctr: " << ci << std::endl;
            }
#endif
            while(next != -1) {
                assert(next < (int) cs.size());
                ci.merge(cs[next]);
                next = cs[next].next;
            }
#if 0
            std::cout << "new ctr: " << ci << std::endl;
#endif
        }
    }

    bool existsInList(counterlist_t& newlist, counter_t& ci)
    {
        for(unsigned i=0; i != newlist.size(); i++) {
            if(newlist[i].same(ci)) {
                return true;
            }
        }
        return false;
    }

    void elimDups(counterlist_t& cs)
    {
        counterlist_t newlist;
        for(unsigned i=0; i != cs.size(); i++) {
            if(!existsInList(newlist, cs[i])) {
                newlist.push_back(cs[i]);
            }
        }
        cs.clear();
        std::back_insert_iterator< counterlist_t > it(cs);
        std::copy(newlist.begin(), newlist.end(), it);
    }

    void outputCounters(counterlist_t& cs)
    {
        addOverlaps(cs);
        elimDups(cs);
        markContained(cs);
        std::cout << "uncontained counters:" << std::endl;
        std::cout << "---------------------" << std::endl;
        for(unsigned i=0; i != cs.size(); i++) {
            if(!cs[i].contained) {
                std::cout << cs[i] << std::endl;
                aggr::id_module_t* mod = cs[i].get_module();
                flat_module_t* flatmod = cs[i].latches[0]->get_module();
                flatmod->add_module(mod);

            }
        }
    }

    bool fanin_contains_latches(node_t* n, const nodelist_t& latches)
    {
        for(nodelist_t::const_iterator it = latches.begin(); it != latches.end(); it++) {
            node_t* li = *it;
            if(n->is_driving_latch(li)) return true;
        }
        return false;
    }

    void prune_common_cone(nodeset_t& common_cone, const nodelist_t& latches)
    {
        nodeset_t::iterator it = common_cone.begin();
        while(it != common_cone.end()) {
            node_t* n = *it;
            if(fanin_contains_latches(n, latches)) {
                common_cone.erase(it++);
            } else {
                it++;
            }
        }
    }

    unsigned count_diff(const nodeset_t& a, const nodeset_t& b)
    {
        std::vector<node_t*> diff;
        std::back_insert_iterator< std::vector<node_t*> > bit(diff);

        std::set_difference(a.begin(), a.end(), b.begin(), b.end(), bit);
        std::set_difference(b.begin(), b.end(), a.begin(), a.end(), bit);

        return diff.size();
    }

    unsigned count_diff(node_t* a, node_t* b)
    {
        return count_diff(*a->get_driving_latches(), *b->get_driving_latches());
    }

    aggr::id_module_t* create_latch_module(nodelist_t& latches, const char* type)
    {
        assert(latches.size() >= 2);

        flat_module_t *module = latches[0]->get_module();

        nodelist_t latch_in;
        module->get_latch_inputs(latches, latch_in);

        aggr::id_module_t* mod = new aggr::id_module_t(
            type,
            aggr::id_module_t::UNSLICEABLE,
            aggr::id_module_t::INFERRED
        );

        nodeset_t inputs;
        for(unsigned i=0; i != latches.size(); i++) {
            node_t* l = latches[i];
            mod->add_output(l);
            inputs.insert(l->get_input(1)); // clock.

            node_t* li = latch_in[i];
            for(node_t::input_iterator it = li->inputs_begin(); it != li->inputs_end(); it++) {
                node_t* inp = *it;
                inputs.insert(inp);
            }
        }

#if 0
        // code to print out "distances" between latches.
        std::cout << type << " : ";
        for(unsigned i=1; i != latches.size(); i++) {
            int d = count_diff(latches[0], latches[i]);
            std::cout << d << " ";
        }
        std::cout << std::endl;
#endif

        for(nodeset_t::iterator it = inputs.begin(); it != inputs.end(); it++) {
            node_t* i = *it;
            mod->add_input(i);
        }
        mod->compute_internals();
        return mod;
    }

    aggr::id_module_t* counter_t::get_module()
    {
        return create_latch_module(latches, "counter");
    }
}
