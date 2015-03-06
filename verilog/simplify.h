#ifndef _SIMPLIFY_H_DEFINED_
#define _SIMPLIFY_H_DEFINED_

#include <stdint.h>
#include <assert.h>
#include <vector>
#include <set>
#include <iostream>

int simplify_main(int argc, char* argv[]);

namespace simplify_n
{
    typedef std::set<int> intset_t;

    // definitions for zero, one and X.
    enum { k0 = 0, k1 = 1, kX = 2 };

    // structure represents cubes.
    struct cube_t
    {
        std::vector<int8_t> vars;
        std::set<int> onset;
        bool essential;

        // constructor: create a cube with 'n' vars from
        // with the bits set to the value in 'value'. for e.g.,
        // if vars = 2 and value = 1, then the cube a & !b is
        // created.
        cube_t(int n, int value);

        // create the consensus of two cubes
        cube_t(const cube_t& m1, const cube_t& m2);

        void mark_essential() { essential = true; }
        bool is_essential() const { return essential; }

        const unsigned size() const { return vars.size(); }

        int8_t bit(int i) const { assert(i < (int) size()); return vars[i]; }

        bool contains(const cube_t& m) const;

        bool covers(int i) const;

        unsigned get_uncovered_count(intset_t& covered) const;

        bool operator==(const cube_t& m) const;
    };

    // list of cubes.
    typedef std::vector<cube_t> cube_list_t;

    // does this term exist in the list already?
    bool exists(cube_list_t& ms, cube_t& m);

    // are the two cubes at a distance of one from each other?
    bool distanceOne(const cube_t& m1, const cube_t& m2);

    // dump a cube to a stream.
    std::ostream& operator<<(std::ostream& out, const cube_t& m);
    std::ostream& operator<<(std::ostream& out, const cube_list_t& m);

    // simplify a list of cubes.
    void simplify(cube_list_t& cubes);

    // iterated consensus to get primes
    void iterated_consensus(cube_list_t& terms);
}

#endif
