#include "sat.h"
#include "node.h"
#include "simplify.h"
#include "core/Solver.h"
#include <iomanip>
#include <iostream>

namespace sat_n
{
    std::ostream& operator<<(std::ostream& out, const clause_t& c)
    {
        out << "(";
        unsigned pos = 0;
        for(clause_t::const_iterator it = c.begin(); it != c.end(); it++, pos += 1) {
            if(it->polarity == ZERO) {
                out << "!";
            }
            out << it->node->get_name();
            if(pos+1 != c.size()) {
                out << " + ";
            }
        }
        out << ")";
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const formula_t& f)
    {
        for(formula_t::const_iterator it = f.begin(); it != f.end(); it++) {
            out << *it;
        }
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const cnf_t& cnf)
    {
        if(cnf.constant) {
            out << "cnst:" << cnf.const_value;
        } else {
            out << cnf.formula;
        }
        return out;
    }

    bool incrementArray(int* array, int n)
    {
        assert(n > 0);
        int pos = 0;
        while(array[pos] == 1) {
            array[pos] = 0;
            pos = pos + 1;
            if(pos == n) break;
        }
        if(pos == n) return false;
        else {
            assert(array[pos] == 0);
            array[pos] = 1;
            return true;
        }
    }

    void dumpArray(std::ostream& out, int* array, int sz)
    {
        for(int i = 0; i != sz; i++) {
            out << array[i];
        }
    }

    int arrayValue(int* array, int sz)
    {
        int v=0;
        for(int i=0; i != sz; i++) {
            if(array[i]) {
                v |= (1 << i);
            }
        }
        return v;
    }

    void computeCovers(Cudd& mgr, BDD& bdd, simplify_n::cube_list_t& onset, simplify_n::cube_list_t& offset)
    {
        using namespace simplify_n;

        int numVars = bdd.SupportSize();
        int *support = new int[numVars];

        BDD b1 = mgr.bddOne();
        BDD b0 = mgr.bddZero();

        std::fill(support, support + numVars, 0);
        bool done = false;

        onset.clear();
        offset.clear();

        do {
            BDD r = bdd.Eval(support);
            int v = arrayValue(support, numVars);
            cube_t c(numVars, v);

            if(r == b1) {
                onset.push_back(c);
            } else {
                assert(r == b0);
                offset.push_back(c);
            }
            done = !incrementArray(support, numVars);
        } while(!done);

        simplify_n::simplify(onset);
        simplify_n::simplify(offset);

        delete [] support;

        assert(verifyCovers(mgr,  bdd, onset, offset));
    }

    bool verifyCovers(Cudd& mgr, BDD& bdd, simplify_n::cube_list_t& onset, simplify_n::cube_list_t& offset)
    {
        using namespace Minisat;

        assert(onset.size() > 0);
        assert(offset.size() > 0);

        int sz = offset[0].size();
        Lit y = mkLit(sz);

        // solver.
        Solver s;
        // create `sz' variables
        while(s.nVars() < (sz+1)) s.newVar();

        for(unsigned i=0; i != onset.size(); i++) {
            addSATClause(s, onset[i], y);
        }
        for(unsigned i=0; i != offset.size(); i++) {
            addSATClause(s, offset[i], ~y);
        }

        assert(bdd.SupportSize() == sz);
        int numVars = bdd.SupportSize();
        int *support = new int[numVars];

        BDD b1 = mgr.bddOne();
        BDD b0 = mgr.bddZero();

        std::fill(support, support + numVars, 0);
        bool done = false;

        do {
            vec<Lit> assump;
            for(int i=0; i != numVars; i++) {
                if(support[i] == 1) {
                    assump.push(mkLit(i));
                } else {
                    assert(support[i] == 0);
                    assump.push(~mkLit(i));
                }
            }

            BDD r = bdd.Eval(support);
            if(r == b1) {
                assump.push(mkLit(numVars));
            } else {
                assert(r == b0);
                assump.push(~mkLit(numVars));
            }
            bool sat = s.solve(assump);
            int v = arrayValue(support, numVars);
            if(!sat) {
                std::cout << std::setw(5) << v << " : FAILED" << std::endl;
                assert(false);
                return false;
            }
            done = !incrementArray(support, numVars);
        } while(!done);

        delete [] support;
        return true;
    }

    void addSATClause(Minisat::Solver& solver, simplify_n::cube_t& cube, Minisat::Lit result)
    {
        using namespace Minisat;
        using namespace simplify_n;

        vec<Lit> clause;
        for(unsigned i=0; i != cube.size(); i++) {
            Lit l = mkLit(i);
            switch(cube.bit(i))
            {
                case k1:
                    clause.push(~l);
                    break;
                case k0:
                    clause.push(l);
                    break;
                default:
                    break;
            }
        }
        clause.push(result);
        solver.addClause(clause);
    }

    void createClause(const simplify_n::cube_t& cube, node_t* node, lit_t& result, clause_t& clause)
    {
        assert(clause.size() == 0);
        clause.clear();
        for(unsigned i=0; i != cube.size(); i++) {
            using namespace simplify_n;
            switch(cube.bit(i)) {
                case k1:
                    // note inversion of polarity.
                    clause.push_back(lit_t(node->get_input(i), ZERO));
                    break;
                case k0:
                    // note inversion of polarity.
                    clause.push_back(lit_t(node->get_input(i), ONE));
                    break;
                default:
                    break;
            }
        }
        clause.push_back(result);
    }

    bool satchecker_t::doesLitExist(node_t* n) const
    {
        return node_map.find(n) != node_map.end();
    }

    Minisat::Lit satchecker_t::getLit(node_t* n)
    {
        using namespace Minisat;

        node_map_t::iterator pos = node_map.find(n);
        if(pos == node_map.end()) {
            int var = getNewVar();
            node_map[n] = mkLit(var);
        }
        return node_map[n];
    }

    void satchecker_t::renameLit(node_t* n, Minisat::Lit& oldLit, Minisat::Lit& newLit)
    {
        using namespace Minisat;

        // ring out the old.
        assert(doesLitExist(n));
        oldLit = getLit(n);

        // ring in the new.
        newLit = mkLit(getNewVar());
        node_map[n] = newLit;
    }

    void satchecker_t::renameGates(const nodeset_t& nodes)
    {
        using namespace Minisat;

        for(nodeset_t::const_iterator it = nodes.begin(); it != nodes.end(); it++) {
            node_t* n = *it;
            if(doesLitExist(n)) {
                Lit a, b;
                renameLit(n, a, b);
            }
        }
    }


    int satchecker_t::getNewVar()
    {
        int var=last_var;
        while(S.nVars() < (var+1)) S.newVar();
        last_var += 1;
        return var;
    }

    Minisat::Lit satchecker_t::getNewLit()
    {
        using namespace Minisat;

        int var = getNewVar();
        return mkLit(var);
    }

    satchecker_t::satchecker_t()
    {
        last_var = 0;
    }

    satchecker_t::~satchecker_t()
    {
    }

    void satchecker_t::addCNF(cnf_t& cnf, cofactor_map_t& cfmap)
    {
        if(cnf.constant) {
            if(cnf.const_value == 1) {
                S.addClause(getLit(cnf.node));
            } else {
                assert(cnf.const_value == 0);
                S.addClause(~getLit(cnf.node));
            }
        } else {
            for(formula_t::iterator it = cnf.formula.begin(); it != cnf.formula.end(); it++) {
                clause_t& c = *it;
                if(!isSat(c, cfmap)) {
                    addClause(c, cfmap);
                }
            }
        }
        {
            /*
            using namespace Minisat;
            std::cout << "cnf:  " << cnf << std::endl;
            std::cout << "cft:  " << cfmap << std::endl;
            std::cout << "nodemap:" << std::endl << node_map;
            */
            /*
            std::cout << "=================================" << std::endl;
            */
        }

    }

    bool satchecker_t::addClause (Minisat::Lit p)
    {
        return S.addClause(p);
    }

    bool satchecker_t::addClause (Minisat::Lit p, Minisat::Lit q)
    {
        return S.addClause(p, q);
    }

    bool satchecker_t::addClause (Minisat::Lit p, Minisat::Lit q, Minisat::Lit r)
    {
        return S.addClause(p, q, r);
    }

    bool satchecker_t::isSat(clause_t& c, cofactor_map_t& cfmap)
    {
        for(clause_t::iterator it = c.begin(); it != c.end(); it++) {
            lit_t& l = *it;
            cofactor_map_t::iterator pos = cfmap.find(l.node);

            if(pos == cfmap.end()) {
                continue;
            } else {
                int value = pos->second;
                if((value == 1 && l.polarity == ONE) ||
                   (value == 0 && l.polarity == ZERO))
                {
                    return true;
                }
            }
        }
        return false;
    }

    void satchecker_t::addClause(clause_t& c, cofactor_map_t& cfmap)
    {
        using namespace Minisat;

        vec<Lit> mc;

        for(clause_t::iterator it = c.begin(); it != c.end(); it++) {
            lit_t& l = *it;
            cofactor_map_t::iterator pos = cfmap.find(l.node);

            if(pos == cfmap.end()) {
                Lit ml = l.polarity == ZERO ? ~getLit(l.node) : getLit(l.node);
                mc.push(ml);
            } else {
                int value = pos->second;
                if((value == 0 && l.polarity == ONE) ||
                   (value == 1 && l.polarity == ZERO))
                {
                    continue;
                } else {
                    // can't get here because this condition should have been
                    // covered by isSat.
                    assert(false);
                }
            }
        }
        /*
        std::cout << "clause: " << c << std::endl;
        std::cout << "adding: ";
        for(int i=0; i != mc.size(); i++) {
            if(sign(mc[i])) std::cout << "!";
            std::cout << var(mc[i]) << " ";
        }
        std::cout << std::endl;
        */
        S.addClause(mc);
    }

    void satchecker_t::addClauses(const nodeset_t& nodes, cofactor_map_t& cfmap) 
    {
        for(nodeset_t::const_iterator it = nodes.begin(); it != nodes.end(); it++) {
            node_t* n = *it;
            cnf_t& cnf = n->get_cnf();
            addCNF(cnf, cfmap);
        }
    }

    bool satchecker_t::areEquiv(node_t* a, node_t* b)
    {
        using namespace Minisat;

        assert(node_map.find(a) != node_map.end());
        assert(node_map.find(b) != node_map.end());

        Lit x = node_map[a];
        Lit y = node_map[b];

        return areEquiv(x, y);
    }

    bool satchecker_t::areInvEquiv(node_t* a, node_t* b)
    {
        using namespace Minisat;

        assert(node_map.find(a) != node_map.end());
        assert(node_map.find(b) != node_map.end());

        Lit x = node_map[a];
        Lit y = ~node_map[b];

        return areEquiv(x, y);
    }

    unsigned satQueryCount=0;
    bool satchecker_t::areEquiv(Minisat::Lit& x, Minisat::Lit& y)
    {
        using namespace Minisat;
        satQueryCount += 1;

        S.addClause(x, y);
        S.addClause(~x, ~y);

        vec<Lit> assumps;
        if(S.solveLimited(assumps) == l_False) return true;
        else return false;
    }

    bool satchecker_t::isSAT(Minisat::Lit y)
    {
        using namespace Minisat;
        satQueryCount += 1;

        vec<Lit> assumps;
        assumps.push(y);
        return S.solve(assumps);
    }

    void satchecker_t::dump()
    {
        using namespace Minisat;
        std::cout << "===================================" << std::endl;
        vec<Lit> assumps;
        S.toDimacs(stdout, assumps);
        std::cout << "===================================" << std::endl;
    }

    std::ostream& operator<<(std::ostream& out, const cofactor_map_t& map)
    {
        for(cofactor_map_t::const_iterator it = map.begin(); it != map.end(); it++) {
            out << it->first->get_name() << ":" << it->second << " ";
        }
        return out;
    }

    std::ostream& operator<<(std::ostream& out, const satchecker_t::node_map_t& nm)
    {
        for(satchecker_t::node_map_t::const_iterator it = nm.begin(); it != nm.end(); it++) {
            out << it->first->get_name() << ":" << Minisat::var(it->second) << std::endl;
        }
        return out;
    }

    Minisat::Lit satchecker_t::addFunc(
        node_t* root,
        const nodeset_t& nodes, 
        cofactor_map_t& cfmap, 
        const nodeset_t* inputs,
        node_map_t* input_map
    )
    {
        assert(nodes.find(root) != nodes.end());

        // clear any old definitions.
        renameGates(nodes);
        addClauses(nodes, cfmap);

        // find the inputs.
        if(inputs != NULL) {
            assert(input_map != NULL);
            for(nodeset_t::const_iterator it = inputs->begin(); it != inputs->end(); it++) {
                node_t* inp = *it;
                assert(doesLitExist(inp));
                Minisat::Lit l = getLit(inp);
                (*input_map)[inp] = l;
            }
        }
        // find the root literal.
        assert(doesLitExist(root));
        Minisat::Lit root_lit = getLit(root);
        // get rid of all the definitions made here!
        renameGates(nodes);
        return root_lit;
    }
}
