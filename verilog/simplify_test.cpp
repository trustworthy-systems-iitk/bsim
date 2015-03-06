#include <stdint.h>
#include <limits.h>
#include <stdio.h>
#include "library.h"

#include "simplify.h"
#include "core/SolverTypes.h"
#include "core/Solver.h"
#include "sat.h"

void sat_convert_main()
{
    using namespace sat_n;
    using namespace simplify_n;

    dummy_input_provider_t dip;
    BDD a = dip.cudd.bddVar(0);
    BDD b = dip.cudd.bddVar(1);
    BDD c = dip.cudd.bddVar(2);
    BDD d = dip.cudd.bddVar(3);

    BDD y = !((a&b)|c|d);
    cube_list_t onset;
    cube_list_t offset;

    computeCovers(dip.cudd, y, onset, offset);
    std::cout << "on-set  : " << onset << std::endl;
    std::cout << "off-set : " << offset << std::endl;
}

void solver_main()
{
    using namespace Minisat;

    Solver S;
    while(S.nVars() < 2) S.newVar();
    Lit a = mkLit(0);
    Lit b = mkLit(1);
    vec<Lit> c1, c2, c3, c4;

    c1.push(a);
    c1.push(b);

    c2.push(~a);
    c2.push(~b);

    c3.push(a);
    c3.push(~b);

    c4.push(~a);
    c4.push(b);

    S.addClause(c1);
    S.addClause(c2);
    bool r1 = S.solve();
    if(r1) { printf("Test 1 PASSED\n"); } 
    else { printf("Test 1 FAILED\n"); }

    S.addClause(c3);
    S.addClause(c4);
    bool r2 = (!S.solve());
    if(r2) { printf("Test 2 PASSED\n"); } 
    else { printf("Test 2 FAILED\n"); }
}

int simplify_main(int argc, char* argv[])
{
    using namespace simplify_n;

    std::vector<cube_t> terms;
    /*
    for(int i = 0; i != 8; i++) {
        cube_t m(3, i);
        std::cout << i << ": " << m << std::endl;
        terms.push_back(m);
    }
    for(int i = 0; i != 8; i++) {
        cube_t& m = terms[i];
        for(int j = 0; j != 8; j++) {
            cube_t& n = terms[j];
            if(distanceOne(m,n)) {
                std::cout << "distance one: " << m << " and " << n << std::endl;
                cube_t c(m, n);
                std::cout << "consensus   : " << c << std::endl;
                assert(c.contains(m));
                assert(c.contains(n));
            }
        }
    }
    */

    terms.clear();
    for(int i = 0; i != 8; i++) {
        int a = !!(i & 1);
        int b = !!(i & 2);
        int c = !!(i & 4);
        if(!((a & b) | c)) {
            terms.push_back(cube_t(3, i));
        }
    }
    std::cout << "initial  : " << terms << std::endl;
    simplify(terms);
    std::cout << "simplify : " << terms << std::endl;

    solver_main();
    sat_convert_main();

    return 0;
}

