#ifndef _SIMRE_H_DEFINED_
#define _SIMRE_H_DEFINED_

#include "flat_module.h"
#include "node.h"
#include "cuddObj.hh"

int simre_main(int argc, char* argv[]);
void simre_usage(const char* prog);
int get_module_index(std::string& name);

void print_error(const char* s);

struct abstract_module_factory_t
{
    virtual flat_module_t* get_module(std::string& name) = 0;
};

struct sim_elem_t
{
public:
    enum type_t { BIT, WORD };
private:
    const type_t type;
    bool bit;
    std::vector<bool> bits;

    // position.
    int position;
    // (max value for position) + 1.
    const int max_position;
    // methods.
    bool bit_increment();
    bool word_increment();
public:
    sim_elem_t(int size);
    int get_bit(int i);
    bool increment();

    friend std::ostream& operator<<(std::ostream& out, sim_elem_t& s);
};

std::ostream& operator<<(std::ostream& out, sim_elem_t& s);

struct sim_info_t
{
    struct index_t {
        int index;
        int bit;
    };

    typedef std::pair<node_t*, int> sim_node_t;
    typedef std::vector<sim_node_t> sim_nodelist_t;
    typedef std::vector<sim_nodelist_t> sim_wordlist_t;
    typedef std::vector<sim_elem_t*> sim_elem_list_t;
    typedef std::map<node_t*, int> node2int_map_t;
    typedef std::map<int, node_t*> int2node_map_t;
    typedef std::map<int, index_t> int2index_map_t;
    typedef std::map<node_t*, BDD> bdd_map_t;
    typedef std::vector<BDD> bdd_list_t;
    typedef std::vector< std::vector<int> > var_list_t;

private:
    // module name.
    std::string moduleName;
    // flat_module pointer.
    flat_module_t* flat;

    // these are the "bit" inputs
    sim_nodelist_t inputs;
    // these are the "word" inputs
    sim_wordlist_t word_inputs;
    // list of simulation input values.
    sim_elem_list_t sim_elem_list;
    // these are the outputs. 
    // outputs have no distinction between "bit" outputs and "word" outputs.
    sim_nodelist_t outputs;

    // map from node to variable index.
    node2int_map_t node2int_map;
    // map from variable index to node.
    int2node_map_t int2node_map;
    // map from variable index to index inside the sim_elem;
    int2index_map_t int2index_map;
    // map from nodes to their BDDs
    bdd_map_t bdd_map;
    // list of BDDs for the output functions.
    bdd_list_t bdd_list;
    // indices of the nodes that each output depends on.
    var_list_t var_list;

    // dump methods
    void dump(std::ostream& out);
    void dump_word(std::ostream& out, sim_nodelist_t& s);
    // create BDD variables for the inputs.
    void createBDDVars();
    // create BDDs for the outputs.
    void createOutputBDDs();
    // create the support indices for this BDD.
    void createSupportIndices(BDD& b);
    // evalute all the outputs for the current state of the inputs.
    void evaluateOutputs(std::ostream& out);
    // evaluate the output at this index.
    int evaluateOutput(std::ostream& out, int index, int* support);
    // pretty print the inputs.
    void printInputs(std::ostream& out);
    // increment inputs: return false when done.
    bool incrementInputs();
public:
    // construct a sim info object from this file.
    sim_info_t(abstract_module_factory_t* factory, std::string& filename);
    ~sim_info_t();
    void simulate(std::ostream& out);

private:
    // private functions.
    void assert_flat_valid(const char* directive);
};

#endif // _SIMRE_H_DEFINED_
