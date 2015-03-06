#ifndef _SAT_H_DEFINED_
#define _SAT_H_DEFINED_

#include <map>
#include <vector>
#include <iostream>
#include "core/Solver.h"
#include "simplify.h"
#include "cuddObj.hh"

class node_t;
// type of sets of nodes.
typedef std::set<node_t*> nodeset_t;

namespace sat_n
{

    // ONE means non-inverted.
    // ZERO means inverted.
    enum polarity_t { ZERO, ONE };
    struct lit_t
    {
        node_t* node;
        polarity_t polarity;

        lit_t(node_t *n, polarity_t p)
            : node(n)
            , polarity(p)
        {
        }
    };
    typedef std::vector<lit_t> clause_t;
    typedef std::vector<clause_t> formula_t;

    void createClause(const simplify_n::cube_t& cube, node_t* node, lit_t& result, clause_t& clause);

    struct cnf_t
    {
        bool constant;
        int const_value;
        node_t* node;

        formula_t formula;
        cnf_t() : constant(false), const_value(-1), node(NULL) {}
    };

    std::ostream& operator<<(std::ostream& out, const clause_t& c);
    std::ostream& operator<<(std::ostream& out, const formula_t& f);
    std::ostream& operator<<(std::ostream& out, const cnf_t& cnf);

    typedef std::map<node_t*, int> cofactor_map_t;
    std::ostream& operator<<(std::ostream& out, const cofactor_map_t& map);

    class satchecker_t
    {
    public:
        // types.
        typedef std::map<node_t*, Minisat::Lit> node_map_t;
    protected:

        // data.
        Minisat::Solver S;
        node_map_t node_map;
        int last_var;

        // methods.
        int getNewVar();

        bool isSat(clause_t& c, cofactor_map_t& cfmap);

    public:
        // construction and destruction.
        satchecker_t();
        ~satchecker_t();
        
        // does this literal exist?
        bool doesLitExist(node_t* n) const;
        // Return the Minisat literal for this node.
        Minisat::Lit getLit(node_t* n);
        // Returns a "new" Minisat literal.
        Minisat::Lit getNewLit();
        // rename this node to have a new literal (returning the old and new literals one).
        void renameLit(node_t* n, Minisat::Lit& oldLit, Minisat::Lit& newLit);
        // rename all the gates inside this set to have new literals.
        void renameGates(const nodeset_t& nodes);

        // Add a unit clause to the solver. 
        bool addClause (Minisat::Lit p);
        // Add a binary clause to the solver. 
        bool addClause (Minisat::Lit p, Minisat::Lit q);
        // Add a ternary clause to the solver. 
        bool addClause (Minisat::Lit p, Minisat::Lit q, Minisat::Lit r);

        // add this clause to the solver.
        void addClause(clause_t& c, cofactor_map_t& cfmap);
        // add this formula to the solver.
        void addCNF(cnf_t& cnf, cofactor_map_t& cfmap);
        // check if these two nodes are the same function.
        bool areEquiv(node_t* a, node_t* b);
        // check if a = !b.
        bool areInvEquiv(node_t* a, node_t* b);
        // check if these two literals are equivalent.
        bool areEquiv(Minisat::Lit& x, Minisat::Lit& y);
        // chekc if the literal is satisfiable.
        bool isSAT(Minisat::Lit l);
        // add clauses.
        void addClauses(const nodeset_t& nodes, cofactor_map_t& cfmap);

        // This adds the fanin cone of this function, renames all the internal
        // nodes. Returns the output literal. It also returns a map of the
        // literals representing the inputs.
        Minisat::Lit addFunc(node_t* root, const nodeset_t& nodes, cofactor_map_t& cfmap, const nodeset_t* inputs, node_map_t* input_map);

        void dump();
    };

    extern unsigned satQueryCount;

    // boolean increment an array.
    // returns false when all 1s.
    bool incrementArray(int* array, int sz);

    // dump an array to a stream.
    void dumpArray(std::ostream& out, int* array, int sz);

    // convert array to integer.
    int arrayValue(int* array, int sz);

    // Compute a near-minimal cover for the on-set and off-set.
    void computeCovers(Cudd& mgr, BDD& bdd, simplify_n::cube_list_t& onset, simplify_n::cube_list_t& offset);

    // Verify that the cover for the on-set and off-set do in fact represent the BDD.
    bool verifyCovers(Cudd& mgr, BDD& bdd, simplify_n::cube_list_t& onset, simplify_n::cube_list_t& offset);

    // Add a clause to the solver.
    void addSATClause(Minisat::Solver& solver, simplify_n::cube_t& cube, Minisat::Lit result);

    // Debugging function to dump the mapping between nodes and variables.
    std::ostream& operator<<(std::ostream& out, const satchecker_t::node_map_t& nm);
}

#endif
