#ifndef _SHIFTREG_H_DEFINED_
#define _SHIFTREG_H_DEFINED_

#include "sat.h"
#include "node.h"
#include "flat_module.h"

namespace shiftreg_n
{
    struct shiftreg_t {
        bool contained;
        nodelist_t latches;

        // for the union-find DS.
        shiftreg_t* root;
        int rank;

        shiftreg_t(nodelist_t& l) 
            : contained(false), 
              latches(l), 
              root(this),
              rank(0)
        {
        }

        shiftreg_t(const shiftreg_t& other)
            : contained(other.contained),
              latches(other.latches),
              root(other.root),
              rank(0)
        {
        }


        bool contains(const shiftreg_t& other) const;
        bool identical(shiftreg_t& other);
        unsigned size() const { return latches.size(); }

        // union find.
        void link(shiftreg_t& other);
        shiftreg_t* find_set();
    };

    aggr::id_module_t* create_module(nodelist_t& latches);

    typedef std::map<shiftreg_t*, nodelist_t> root_map_t;
    std::ostream& operator<<(std::ostream& out, const shiftreg_t& sr);

    typedef std::vector<shiftreg_t> shiftreg_list_t;

    // find all shift registers in the design.
    void find_shiftregs(flat_module_t* module);

    // test function.
    void shiftreg_test(flat_module_t* module);

    // is this set of latches a shift register?
    bool is_shiftreg(nodelist_t& latch_outputs, nodelist_t& latch_inputs, unsigned j);
    
    // can we ignore this latch?
    bool should_ignore_latch(node_t* n);

    // explore this path.
    void explore(flat_module_t* module, shiftreg_list_t& shiftregs, nodelist_t& latch_out, nodelist_t& latch_in);

    // does n drive any of the latches in latch_out
    bool drives(node_t* n, const nodelist_t& latch_out);

    bool onepath(node_t* a, node_t* b);

}

#endif
