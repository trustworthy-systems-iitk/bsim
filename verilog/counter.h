#ifndef _COUNTER_H_DEFINED_
#define _COUNTER_H_DEFINED_

#include <map>
#include <vector>
#include <algorithm>
#include "node.h"
#include "flat_module.h"
#include "aggr.h"

void find_counters(nodelist_t& latches);

namespace counters_n 
{
    bool should_ignore_latch(node_t* n);
    bool is_driven_by(node_t* n, nodelist_t& latches);
    bool same_group(node_t* l1, node_t* l2);

    extern unsigned potential_counters_found;
    // debug.
    bool shd_dbg(node_t* n);

    struct counter_state_t {
        flat_module_t* flat;

        // the latches that comprise this counter.
        nodelist_t latches;

        // their inputs.
        nodelist_t latch_inputs;

        // latches which could possibly extend this counter.
        nodeset_t available_latches;

        // the common fanin cone of this counter.
        nodeset_t fanin_cone;

        // could this be an up counter?
        bool up;
        // could this be a down counter?
        bool down;

        // initialize the state so that we can start exploring from this one latch.
        counter_state_t(node_t* head);
        // initialize this state so that we add the "next" latch to the "other" state.
        counter_state_t(counter_state_t& other, node_t* next);

        // destructor.
        ~counter_state_t();
        // is there an edge from this node to one of the nodes in the list of latches.
        bool no_back_loops(node_t* n);
        // is this latch reachable (in 1-step) from all the latches in the list.
        bool is_fully_reachable(node_t* latch);
    };


    struct counter_t
    {
        nodelist_t latches;
        bool contained;
        bool up;
        bool down;
        int next;

        counter_t(counter_state_t& cs, bool up, bool down);
        counter_t(const counter_t& a);
        ~counter_t();

        bool contains(const node_t* n) const;
        bool contains(const counter_t& o) const;
        bool overlaps(const counter_t& other) const;
        bool same(const counter_t& other) const;

        void merge(const counter_t& other);

        friend std::ostream& operator<<(std::ostream& out, const counter_t& c);

        aggr::id_module_t* get_module();
    };
    typedef std::vector<counter_t> counterlist_t;
    std::ostream& operator<<(std::ostream& out, const counter_t& c);

    bool is_counter(nodelist_t& latches, bool up, bool down, const nodeset_t& common_cone, unsigned i);
    bool is_counter(nodelist_t& latches);
    std::ostream& operator<<(std::ostream& out, const counter_state_t& state);

    void lengthen(counter_state_t& state, counterlist_t& counters);
    void markContained(counterlist_t& cs);
    void outputCounters(counterlist_t& cs);

    bool fanin_contains_latches(node_t* n, const nodelist_t& latches);
    void prune_common_cone(nodeset_t& common_cone, const nodelist_t& latches);
    aggr::id_module_t* create_latch_module(nodelist_t& latches, const char* type);

    unsigned count_diff(const nodeset_t& a, const nodeset_t& b);
    unsigned count_diff(node_t* a, node_t* b);
}

#endif // _COUNTER_H_DEFINED
