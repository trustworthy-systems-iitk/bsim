#include <algorithm>
#include <iostream>
#include <iterator>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <stdlib.h>
#include <time.h>

#include "flat_module.h"
#include "library.h"
#include "kcover.h"
#include "simre.h"
#include "node.h"
#include "aggr.h"
#include "ast.h"

extern int yyparse();
extern FILE* yyin;
const bool verbose = false;

struct module_factory_t : public abstract_module_factory_t
{
    ILibrary* lib;
    module_factory_t(ILibrary* lib) : lib(lib) {}
    virtual flat_module_t* get_module(std::string& name);
};

flat_module_t* module_factory_t::get_module(std::string& name)
{
    int index = get_module_index(name);
    if(index == -1) {
        printf("Error can't find module '%s'.\n", name.c_str());
        exit(1);
    }
    flat_module_t *flat = new flat_module_t(lib, modules, index);
    return flat;
}

void run_bdd_test()
{
    Cudd mgr;
    BDD a = mgr.bddVar(0);
    BDD b = mgr.bddVar(1);
    BDD c = mgr.bddVar(2);
    BDD d = mgr.bddVar(3);
    BDD fn = (a ^ b ^ c ^ d) + (a&b);
    std::cout << "satisfy count of f: " << fn.SatisfyCount(fn.SupportSize()) << std::endl;
}

int simre_main(int argc, char* argv[])
{
    using namespace std;

    vector<string> sim_inputfiles;
    int bdd_test = 0;

    char c;
    while ((c = getopt (argc, argv, "hm:b")) != -1) {
        switch (c) {
            case 'h':
                simre_usage(argv[0]);
                return 0;
                break;
            case 'b':
                bdd_test = 1;
                break;
            case 'm':
                string module_name(optarg);
                sim_inputfiles.push_back(module_name);
                break;
        }
    }

    if(bdd_test == 1) {
        run_bdd_test();
        return 0;
    }

    if(optind == argc || optind != argc-1) {
        simre_usage(argv[0]);
        return 1;
    }

    yyin = fopen(argv[optind], "rt");
    if(yyin == NULL) {
        perror(argv[optind]);
        return 1;
    }
    if(yyparse() == 0) {
        flat_module_t::setup_cudd();

        string libname("12soi");
        ILibrary* library = createLib(libname);
        library->init();
        library->preprocess(*modules);
        module_factory_t factory(library);

        for(unsigned i = 0; i != sim_inputfiles.size(); i++) {
            sim_info_t sim(&factory, sim_inputfiles[i]);
            sim.simulate(std::cout);
        }
        if(sim_inputfiles.size() == 0) {
            printf("Error: no simulation input files specified.\n");
            return 1;
        }

        delete_modules(modules);
        delete library;
        flat_module_t::destroy_cudd();
    }
    return 0;
}

int get_module_index(std::string& name)
{
    for(unsigned i=0; i != modules->size(); i++) {
        module_t* m = (*modules)[i];
        if(name == m->name) { 
            return i;
        }
    }
    return -1;
}

void simre_usage(const char* prog)
{
    std::cout << "Usage: " << prog << " [options] <input.v>" << std::endl;
    std::cout << "   -h             : this message." << std::endl;
    std::cout << "   -m <inputfile> : input file with info on what to simulate. [repeat -m for multiple input files.]" << std::endl;
    std::cout << "   -b             : run BDD test." << std::endl;
}

void print_error(const char* s)
{
    printf("error: %s\n", s);
    exit(1);
}

sim_info_t::sim_info_t(abstract_module_factory_t* factory, std::string& filename)
    : flat(NULL)
{
    using namespace std;

    int inputGroup = 0;
    int outputGroup = 0;
    ifstream fin(filename.c_str());
    while(fin) {
        std::string line;
        getline(fin, line);

        // split the line into tokens
        istringstream iss(line);
        vector<string> tokens;
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter<vector<string> >(tokens));

        // continue on an empty line.
        if(tokens.size() == 0) continue;
        // now handle the different commands.
        else if(tokens[0] == ".module") {
            if(tokens.size() != 2) {
                print_error("syntax error in .module directive.");
            }
            moduleName = tokens[1];
            flat = factory->get_module(moduleName);
        } else if(tokens[0] == ".inputs") {
            // input list.
            assert_flat_valid(".inputs");
            for(unsigned i=1; i != tokens.size(); i++) {
                node_t* n = flat->get_node_by_name(tokens[i].c_str());
                if(n == NULL) {
                    printf("error: unable to find node '%s'.\n", n->get_name().c_str());
                    exit(1);
                }
                sim_node_t s(n, inputGroup);
                inputs.push_back(s);
            }
            inputGroup += 1;
        } else if(tokens[0] == ".word_input") {
            // word input list.
            assert_flat_valid(".word_input");
            sim_nodelist_t w;
            for(unsigned i=1; i != tokens.size(); i++) {
                node_t* n = flat->get_node_by_name(tokens[i].c_str());
                if(n == NULL) {
                    printf("error: unable to find node '%s'.\n", n->get_name().c_str());
                    exit(1);
                }
                sim_node_t s(n, inputGroup);
                w.push_back(s);
            }
            // creates a copy of the the whole list, but nevermind.
            word_inputs.push_back(w);
            inputGroup += 1;
        } else if(tokens[0] == ".outputs") {
            assert_flat_valid(".outputs");
            for(unsigned i=1; i != tokens.size(); i++) {
                node_t* n = flat->get_node_by_name(tokens[i].c_str());
                if(n == NULL) {
                    printf("error: unable to find node '%s'.\n", n->get_name().c_str());
                    exit(1);
                }
                sim_node_t s(n, outputGroup);
                outputs.push_back(s);
            }
            outputGroup += 1;
        } else {
            printf("error: unknown directive: '%s'.\n", tokens[0].c_str());
            exit(1);
        }
    }

    // now initalize the sim_elem_list 
    for(unsigned i=0; i != inputs.size(); i++) {
        sim_elem_t* s = new sim_elem_t(1);
        sim_elem_list.push_back(s);
    }
    for(unsigned i=0; i != word_inputs.size(); i++) {
        sim_elem_t* s = new sim_elem_t(word_inputs[i].size());
        sim_elem_list.push_back(s);
    }
}

void sim_info_t::assert_flat_valid(const char* directive)
{
    if(flat == NULL) {
        printf("error: .module directive must be used before a '%s' directive occurs.\n", directive);
        exit(1);
    }
}

void sim_info_t::simulate(std::ostream& out)
{
    dump(out);
    // now create BDD variables for the input nodes.
    createBDDVars();
    // now create BDD full functions for each output node.
    createOutputBDDs();
    bool incr = false;
    do {
        // print inputs.
        printInputs(out);
        // now evaluate outputs.
        evaluateOutputs(out);
        // move on to the next.
        incr = incrementInputs();
    } while(incr);
}

bool sim_info_t::incrementInputs()
{
    int p;
    for(p = sim_elem_list.size()-1; p >= 0; p--) {
        if(!sim_elem_list[p]->increment()) break;
    }
    if(p == -1) {
        return false;
    } else {
        return true;
    }
}

void sim_info_t::evaluateOutputs(std::ostream& out)
{
    int g = 0;
    // create the support.
    unsigned sz = bdd_map.size();
    int* support = new int[sz];
    for(unsigned i=0; i != sz; i++) {
        index_t idx = int2index_map[i];
        support[i] = sim_elem_list[idx.index]->get_bit(idx.bit);
    }
    // now evaluate each BDD and print output.
    for(unsigned i=0; i != bdd_list.size(); i++) {
        int o = evaluateOutput(out, i, support);
        int h = outputs[i].second;
        if(g != h) {
            g = h;
            out << " ";
        }
        out << o;
    }
    out << std::endl;
    // delete support.
    delete [] support;
}

int sim_info_t::evaluateOutput(std::ostream& out, int i, int* support)
{
    BDD bdd = bdd_list[i];
    BDD result = bdd.Eval(support);
    if(result == flat->getFullFnMgr().bddOne()) {
        return 1;
    } else if(result == flat->getFullFnMgr().bddZero()) {
        return 0;
    } else {
        assert(false);
        return -1;
    }
}

void sim_info_t::createBDDVars()
{
    int index = 0;
    for(unsigned i=0; i != inputs.size(); i++) {
        node_t* n = inputs[i].first;

        int2node_map[index] = n;
        node2int_map[n] = index;
        int2index_map[index].index = i;
        int2index_map[index].bit = 0;
        bdd_map[n] = flat->getFullFnMgr().bddVar(index);
        std::cout << "index:" << index << "; node: " << n->get_name() << std::endl;

        index += 1;
    }

    for(unsigned i=0; i != word_inputs.size(); i++) {
        for(unsigned j=0; j != word_inputs[i].size(); j++) {
            node_t* n = word_inputs[i][j].first;

            int2node_map[index] = n;
            node2int_map[n] = index;
            int2index_map[index].index = i+inputs.size();
            int2index_map[index].bit = j;
            bdd_map[n] = flat->getFullFnMgr().bddVar(index);

            index += 1;
        }
    }
}

void sim_info_t::createOutputBDDs()
{
    unsigned init_size = bdd_map.size();

    for(unsigned i=0; i != outputs.size(); i++) {
        node_t* n = outputs[i].first;
        BDD b = flat->createFullFn(n, bdd_map, false, -1);
        bdd_list.push_back(b);
        createSupportIndices(b);
    }
    if(bdd_map.size() != init_size) {
        std::cout << "Error: Either all inputs were not specified in the input sim file," << std::endl
                  << "       or the circuit contains sequential elements." << std::endl;
        exit(1);
    }
}

void sim_info_t::createSupportIndices(BDD& b)
{
    if(verbose) { printf("creating support for: "); printBDD(stdout, b); }
    std::vector<int> vec;

    BDD support = b.Support();
    if(verbose) { printf("the support is: "); printBDD(stdout, support); }

    for(unsigned i=0; i != bdd_map.size(); i++) {
        BDD vi = flat->getFullFnMgr().bddVar(i);
        assert(int2node_map.find(i) != int2node_map.end());
        if((support & vi) == support) {
            vec.push_back((int)i);
            if(verbose) { printf("added var: %d\n", i); }
        }
    }
    var_list.push_back(vec);
}

void sim_info_t::printInputs(std::ostream& out)
{
    int g = 0;
    for(unsigned i=0; i != inputs.size(); i++) {
        int h = inputs[i].second;
        if(h != g) {
            out << " ";
            g = h;
        }
        node_t* n = inputs[i].first;
        int pos = node2int_map[n];
        index_t index = int2index_map[pos];
        out << *(sim_elem_list[index.index]);
    }
    unsigned p = inputs.size();
    for(unsigned i=0; i != word_inputs.size(); i++) {
        sim_elem_t* s = sim_elem_list[i+p];
        out << " " << *s;
    }
    out << "\t";
}

void sim_info_t::dump(std::ostream& out)
{
    for(unsigned i=0; i != inputs.size(); i++) {
        out << "input [" << std::setw(5) << i << "]: " << std::setw(20) 
            << inputs[i].first->get_name() << std::setw(5) << inputs[i].second 
            << std::endl;
    }
    for(unsigned i=0; i != word_inputs.size(); i++) {
        out << "word  [" << std::setw(5) << i << "]: ";
        dump_word(out, word_inputs[i]);
        out << std::endl;
    }
    for(unsigned i=0; i != outputs.size(); i++) {
        out << "output [" << std::setw(5) << i << "]: " << std::setw(20) 
            << outputs[i].first->get_name() << std::setw(5) << outputs[i].second 
            << std::endl;
    }
}

void sim_info_t::dump_word(std::ostream& out, sim_nodelist_t& s)
{
    for(unsigned i=0; i != s.size(); i++) {
        out << s[i].first->get_name();
        if(i+1 != s.size()) out << " ";
    }
}

sim_info_t::~sim_info_t()
{
    for(unsigned i=0; i != sim_elem_list.size(); i++) {
        delete sim_elem_list[i];
    }

    bdd_map.clear();
    bdd_list.clear();

    delete flat;
}

sim_elem_t::sim_elem_t(int size)
    : type(size == 1 ? BIT : WORD)
    , bit(false)
    , position(0)
    , max_position(2*size+2)
{
    assert(size > 0);
    if(type == WORD) {
        bits.resize(size, false);
    }
}

int sim_elem_t::get_bit(int i)
{
    if(type == BIT) {
        assert(i == 0);
        return bit ? 1 : 0;
    } else {
        assert(type == WORD);
        assert(i < (int)bits.size());
        return bits[i] ? 1 : 0;
    }
}

std::ostream& operator<<(std::ostream& out, sim_elem_t& s)
{
    if(s.type == sim_elem_t::BIT) {
        out << (s.bit ? 1 : 0);
    } else {
        assert(s.type == sim_elem_t::WORD);
        for(unsigned i=0; i != s.bits.size(); i++) {
            out << (s.bits[i] ? 1 : 0);
        }
    }
    return out;
}

bool sim_elem_t::increment()
{
    if(type == BIT) {
        return bit_increment();
    } else {
        assert(type == WORD);
        return word_increment();
    }
}

bool sim_elem_t::bit_increment()
{
    assert(type == BIT);

    bit = !bit;
    return !bit;
}

bool sim_elem_t::word_increment()
{
    assert(type == WORD);

    position = position+1;
    if(position == max_position) position = 0;
    assert(position < max_position);

    int n = bits.size();
    if(position == 0) {
        std::fill(bits.begin(), bits.end(), false);
    } else if(position >= 1 && position <= n) {
        std::fill(bits.begin(), bits.end(), false);
        int one_pos = position - 1;
        bits[one_pos] = true;
    } else if(position >= n+1 && position <= 2*n) {
        std::fill(bits.begin(), bits.end(), true);
        int zero_pos = position - (n+1);
        bits[zero_pos] = false;
    } else {
        assert(position == 2*n+1);
        std::fill(bits.begin(), bits.end(), true);
    }
    return position == 0;
}
