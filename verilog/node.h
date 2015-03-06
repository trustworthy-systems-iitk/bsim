#ifndef __NODE_H_DEFINED__
#define __NODE_H_DEFINED__

#include <algorithm>
#include <utility>
#include <vector>
#include <set>
#include <limits>
#include "ast.h"
#include "sat.h"
#include "aggr.h"
#include "symbol.h"
#include "library.h"
#include "common.h"
#include "kcover.h"
#include "cuddObj.hh"
#include "word.h"

class flat_module_t;


// module list - used in flat_module_t
typedef std::vector<aggr::id_module_t*> modulelist_t;
// module set - set of modules.
typedef std::set<aggr::id_module_t*> moduleset_t;

// word list - used in both flat_module_t and node_t.
typedef std::vector<word_t*> wordlist_t;
// iterator for wordlist.
typedef wordlist_t::iterator word_iterator;
// kcover set.
typedef std::set<kcover_t*> kcoverset_t;
// type of vector of nodes.
typedef std::vector<node_t*> nodelist_t;

// type of a list of word pointers.
typedef std::vector<word_t*> wordptr_list_t;
// type of the hashtable used to find duplicate words.
typedef std::map<word_t::sign_t, wordptr_list_t> word_hashtable_t;

struct fnInfo_t
{
    int numInputs;
    bool isCanonical;
    fnInfo_t* canonicalPtr;
    nodeset_t nodes;
    kcoverset_t covers;
    std::vector<uint8_t> permutation;

    fnInfo_t() { numInputs = -1; isCanonical = false; canonicalPtr = NULL; }
    fnInfo_t(int numInputs, std::vector<uint8_t>& perm, node_t* node, kcover_t* cover, fnInfo_t* canon)
        : numInputs(numInputs),
        permutation(perm)
    {
        if(node) {
            assert(cover);
            nodes.insert(node);
            covers.insert(cover);
            canon = this;
            isCanonical=true;
        } else {
            assert(cover == NULL);
            isCanonical=false;
            canonicalPtr = canon;
        }
    }

    std::vector<uint8_t>& getPerm() { return permutation; }

    ~fnInfo_t() {
    }
};

// type used to keep track of the library elements we need to spit out.
struct verilog_lib_t
{
    std::set<int> mux21;
    std::set<int> mux21i;
    std::set<int> mux41;
    std::set<int> mux41i;
    void dump(std::ostream& out);
};

struct decl_list_t {
public:
    struct index_t { int min0; int max0; int word_index; };

    typedef std::map<const char*, index_t, string_cmp_t> map_t;
    map_t inputs;
    map_t outputs;
    map_t wires;

    decl_list_t() {}
    ~decl_list_t();

private:
    void add_item(map_t& map, const char* name, int word_index);
    bool is_item(const map_t& map, const char*name) const;
    void dump_verilog_wire(std::ostream& out, const char* type, const char* wire, int min0, int max0, int word_index);
public:
    bool is_input(const char* name) const;
    bool is_output(const char* name) const;
    bool is_wire(const char* name) const;

    void add_input(const char* name, int word_index=-1);
    void add_output(const char* name, int word_index=-1);
    void add_wire(const char* name);
    void dump_verilog(std::ostream& out);
    void dump_instantiation(std::ostream& out, const char* moduleName, const char* instanceName, const aggr::id_module_t* mod);
};

struct bitslice_match_t
{
    flat_module_t* module;
    int output;
    int polarity;
    kcover_t* cover;

    bitslice_match_t(
        flat_module_t* f,
        int op,
        int pol,
        kcover_t* kc
    )
        : module(f)
        , output(op)
        , polarity(pol)
        , cover(kc)
    {
    }
};

std::ostream& operator<<(std::ostream& out, bitslice_match_t& m);


typedef std::vector<bitslice_match_t> matchlist_t;

class node_t {
public:
    enum type_t { INPUT, GATE, LATCH, MACRO, MACRO_OUT };

    //FIELDS FOR SHORTEST PATH
    bool VISITED; //For shortest path algorithm
    float dist_to_source; //dist to source node
    node_t* previous; //pointer to previous node in optimal path

protected:
    // type that will hold the list of inputs.
    typedef std::vector<node_t*> inputlist_t;
    typedef std::list<node_t*> fanoutlist_t;
    typedef std::map<node_t*, bool> node_bool_map_t;
    typedef std::map<int, int> col_depth_map_t;
public:
    typedef inputlist_t::iterator input_iterator;
    typedef fanoutlist_t::iterator fanout_iterator;
    typedef fanoutlist_t::const_iterator const_fanout_iterator;
protected:
    // type of the node.
    type_t type;
    // name of this node.
    std::string name;
    // module instance name. (only valid for gates/latches)
    std::string instance;

    // MACRO related.

    // the macro node that contains the inputs.
    node_t* macro;
    // the list of outputs for this macro node.
    nodelist_t macro_outputs;
    // the list of "ports" this node has.
    nodelist_t ports;

    // is this node a primary output.
    bool output;

    // is this node "dead"
    bool dead;

    // list of inputs.
    inputlist_t inputs;
    // list of inputs in sorted of order of # of k-covers.
    inputlist_t k_inputs;
    // list of fanouts.
    fanoutlist_t fanouts;
    // list of words.
    wordlist_t words;
    // type of the library cell.
    lib_elem_t* lib_elem;
    // list of k-covers.
    kcoverlist_t kcovers;
    // list of bitslice matches.
    matchlist_t matches;
    // level for topo sort.
    int level;
    // the module this node belongs to.
    flat_module_t* const module;
    // the list of identified modules this node belongs to.
    moduleset_t modules;
    // the list of latches to which information flows from this node.
    nodeset_t driven_latches;
    // the list of latches from which information flows to this node.
    nodeset_t driving_latches;
    // the list of driving latches which have multiple paths coming in to this node.
    nodeset_t driving_latches_sp;
    // auxiliary pointer.
    void* aux;
    // index for this node: just a unique number >= 0. uniqueness
    // is defined wrt to a flat_module_t.
    int index;
    // is the CNF valid?
    bool cnf_valid;
    // the CNF for this gate.
    sat_n::cnf_t cnf;
    // the set of nodes in this gates fan-in cone.
    nodeset_t fanin_cone;
    // the set of inputs for the fanin cone.
    nodeset_t fanin_cone_inputs;
    // sibling node (used by 2-output gates).
    node_t* sibling;
    node_t* sibling_back;
    // the original name of the joint module.
    std::string joint_module;
    // if true, this node is not output in the verilog (used to suppress the "little" sibling).
    bool suppress_gate;
    // a bitmap that decides whether these inputs need to be printed.
    std::vector<bool> suppress_input_map;
    // if true, the output is not printed out in the verilog.
    bool suppress_output;
    // if true, this gate is sticking in front of a latch and we can ignore it for a lot of things.
    bool latch_gate;
    // if true, means this gate is part of a combinational loop!
    bool logic_loop;
    // the "color" of this node.
    int color;
    // the clk tree(s) this node belongs to.
    int clk_trees;
    // the resest tree(s) this node belongs to.
    int rst_trees;
    // the backward propagated reset trees.
    int rst2;
    // is this gate directly part of the clk tree?
    bool in_clktree;
    // is this gate directly part of the reset tree?
    bool in_rsttree;
    // is this node a scan-enable input to a flip-flop?
    bool is_scan_enable;

    // input and output distances.
    short input_distance;
    short output_distance;
    node_t* nearest_input;
    node_t* nearest_output;

    // maps colors to the depth of the node from input pins.
    col_depth_map_t col_depth_map;

    // a pointer to the node this was rewritten with.
    node_t* orig;

    // a "representative" node that determines the equivalence class
    // that is computed based on the support.
    node_t* support_rep;

public:
    col_depth_map_t::iterator col_depth_begin() {return col_depth_map.begin(); };
    col_depth_map_t::iterator col_depth_end() {return col_depth_map.end(); };

    node_t(type_t type, const std::string& name, flat_module_t* module, lib_elem_t *libelem=NULL);
    ~node_t();

    static type_t get_type(lib_elem_t::type_t t);

    void morph_latch(node_t* d, lib_elem_t* le);

    void set_is_scan_enable() { is_scan_enable = true; }
    bool get_is_scan_enable() const { return is_scan_enable; }

    node_t* get_support_rep() const { return support_rep; }
    void set_support_rep(node_t* n) { support_rep = n; }

    void add_color(int c) {
        // colours must be a power of 2.
        assert((c&(c-1)) == 0);
        color |= c;
    }

    void set_orig(node_t* n) {
        assert(orig == NULL);
        assert(fanouts.size() == 0);
        orig = n;
    }

    node_t* get_orig() {
        if(orig == NULL) { return this; }
        else {
            // path compression.
            orig = orig->get_orig();
            return orig;
        }
    }


    void add_col_depth (int c, int d){
         assert(col_depth_map.find(c)==col_depth_map.end());
         col_depth_map[c] = d;
    }

    int is_colored(int c){
        //cout<<"is colored: node "<<this<<" , color "<<c<<" , ";
        if (col_depth_map.find(c)==col_depth_map.end()) {
           //cout<<"not colored"<<endl;
           return 0;
        };
        //cout<<col_depth_map[c]<<endl;
        return col_depth_map[c];
    }

    int is_colored() const {
        return color;
    }

    void add_clktree(int c) {
        assert((c&(c-1)) == 0);
        clk_trees |= c;
    }
    void set_clktrees(int c) {
        assert(clk_trees == 0);
        clk_trees = c;
    }
    int get_clktrees() const {
        return clk_trees;
    }

    void add_rsttree(int c) {
        assert((c&(c-1)) == 0);
        rst_trees |= c;
    }
    void set_rsttrees(int c) {
        //assert(rst_trees == 0);
        rst_trees = c;
    }
    int get_rsttrees() const {
        return rst_trees;
    }

    void set_rst2(int c) { rst2 = c; }
    int  get_rst2() const { return rst2; }


    void set_inclktree() { in_clktree = true; }
    bool get_inclktree() const { return in_clktree; }
    void set_inrsttree() { in_rsttree = true; }
    bool get_inrsttree() const { return in_rsttree; }

    int get_input_distance() const { return input_distance; }
    int get_output_distance() const { return output_distance; }
    void set_input_distance(int d, node_t* inp) {
        assert(d < input_distance);
        input_distance = d;
        nearest_input = inp;
    }
    node_t* get_nearest_input() const { return nearest_input; }
    void set_output_distance(int d, node_t* output) {
        assert(d < output_distance);
        output_distance = d;
        nearest_output = output;
    }
    node_t* get_nearest_output() const { return nearest_output; }

    bool is_dead() const { return dead; }
    void mark_dead() { assert(!dead); dead = true; }


    type_t get_type() const { return type; }
    const std::string& get_name() const { return name; }
    const char* get_lib_elem_name() const { return lib_elem->get_name(); }
    void set_instance_name(std::string& i) { instance = i; }
    const std::string& get_instance_name() const { return instance; }

    void add_input(node_t* inp, unsigned idx) {
        assert(idx == inputs.size());
        inputs.push_back(inp);
    }

    void add_macro_output(node_t* o) {
        assert(std::find(macro_outputs.begin(), macro_outputs.end(), o) == macro_outputs.end());
        macro_outputs.push_back(o);
    }

    node_t* get_macro_output(unsigned i) {
        assert(i < macro_outputs.size());
        return macro_outputs[i];
    }

    bool is_macro_output(node_t* o) const {
        return (std::find(macro_outputs.begin(), macro_outputs.end(), o) != macro_outputs.end());
    }

    void set_macro(node_t* m) {
        macro = m;
    }

    void add_port(node_t* o) {
        assert(std::find(ports.begin(), ports.end(), o) == ports.end());
        ports.push_back(o);
    }

    bool in_logic_loop() const { return logic_loop; }
    void set_logic_loop() { logic_loop = true; }

    void mark_output() { output = true; }
    bool is_output() const { return output; }

    void set_latch_gate() { latch_gate = true; }
    bool is_latch_gate() const { return latch_gate; }

    lib_elem_t* get_lib_elem() { return lib_elem; }
    bool is_latch() const { return type == LATCH; }
    bool is_input() const { return type == INPUT; }
    bool is_gate() const { return type == GATE; }
    bool is_macro() const { return type == MACRO; }
    bool is_macro_out() const { return type == MACRO_OUT; }

    bool is_buffer() const { return lib_elem && lib_elem->is_buffer(); }
    bool is_inverter() const { return lib_elem && lib_elem->is_inverter(); }

    // input manipulation.
    unsigned num_inputs() const { return inputs.size(); }
    unsigned num_macro_outputs() const { return macro_outputs.size(); }
    input_iterator inputs_begin() { return inputs.begin(); }
    input_iterator inputs_end() { return inputs.end(); }
    node_t* get_input(int i) { assert((unsigned) i < inputs.size() && i >= 0); return inputs[i]; }
    node_t* get_first_fanout() { assert( fanouts.size() > 0); return fanouts.front(); }
    void set_input(int i, node_t* n) {
        assert((unsigned)i < inputs.size() && i >= 0);
        inputs[i] = n;
    }
    bool has_input(const node_t* n) const {
        return std::find(inputs.begin(), inputs.end(), n) != inputs.end();
    }

    input_iterator k_inputs_begin() { return k_inputs.begin(); }
    input_iterator k_inputs_end() { return k_inputs.end(); }
    inputlist_t* get_input_list() { return &inputs; }
    moduleset_t* get_modules () {return &modules; };
    aggr::id_module_t* get_covering_module();
    bool is_covered() const;
    bool is_covered_by_seq() const;
    bool is_covered_candidate() const;

    // fanout manipulation.
    unsigned num_fanouts() const { return fanouts.size(); }
    unsigned num_real_fanouts() const;
    fanout_iterator fanouts_begin() { return fanouts.begin(); }
    fanout_iterator fanouts_end() { return fanouts.end(); }
    const_fanout_iterator fanouts_begin() const { return fanouts.begin(); }
    const_fanout_iterator fanouts_end() const { return fanouts.end(); }
    void add_fanout(node_t* out) {
        if(std::find(fanouts.begin(), fanouts.end(), out) == fanouts.end()) {
            fanouts.push_back(out);
        }
    }
    void remove_fanout(node_t* out) {
        fanout_iterator pos = std::find(fanouts.begin(), fanouts.end(), out);
        assert(pos != fanouts.end());
        fanouts.erase(pos);
    }
    void fanouts_clear() {
        fanouts.clear();
    }

    void get_fanout_types(std::vector<lib_elem_t*>& fanoutTypes);

    // getting inputs by names.
    node_t* get_input_by_name(const char* name);
    // getting outputs by names.
    node_t* get_output_by_name(const char* name);

    // word manipulation.
    unsigned num_words() const { return words.size(); }
    word_iterator words_begin() { return words.begin(); }
    word_iterator words_end() { return words.end(); }

    int get_index() const { return index; }
    void set_index(int i) { index = i; }

    static int count_clauses(const nodeset_t& cone);
    void dump_gate_cnf(std::ostream& out, std::map<node_t*, int>& mapping);
    void dump_cnf(std::ostream& out);
    sat_n::cnf_t& get_cnf();
    const nodeset_t& get_fanin_cone();
    const nodeset_t& get_fanin_cone_inputs();
private:
    void add_word(word_t* w) {
        if(std::find(words.begin(), words.end(), w) == words.end()) {
            words.push_back(w);
        }
    }
    void compute_cnf();
    void compute_fanin_cone(node_t* n);

    struct renamer_t {
        const aggr::id_module_t* mod;
        renamer_t(const aggr::id_module_t* m) : mod(m) {}
        const std::string& get_renaming(const std::string& s) {
            if(mod == NULL) return s;
            else return mod->get_renaming(s);
        }
    };
public:
    wordlist_t* get_word_list() { return &words; }

    // write this out as verilog statements.
    void dump_verilog_decl(std::ostream& out);
    void dump_verilog_defn(std::ostream& out, bool comments, bool dump_annotation, const char* annotation_mod_name, const aggr::id_module_t* mod);
    static void dump_masked_strings(std::ostream& out, int mask, stringlist_t& strings);
    void dump_verilog_comment(std::ostream& out);
    void dump_annotation(std::ostream& out, bool dump_annotation, const char* annotation_mod_name);
    void dump_sibling_bindings(std::ostream& out, renamer_t& r);

    // dump this node to the stream.
    void dump(std::ostream& out);
    void dump_modules(std::ostream& out);

    // get all the k-covers of this node.
    kcoverlist_t& get_kcovers();
    void compute_covers(kcover_t* cov, unsigned pos);

    //get deepest k-cover of this node.
    kcover_t* get_deepest();
    kcoverlist_t* get_n_deepest(unsigned n);

    // update level for topological sort.
    bool update_level();
    // get level.
    int get_level() const { return level; }

    // rewrite this nodes inputs to use new_node instead of orig_node
    bool rewrite_inputs(node_t* old, node_t* newt);

    flat_module_t* get_module() const { return module; }

    // set info structure.
    void set_info (void* p_info){
         aux = p_info;
    }

    // get info structure.
    void* get_info() {
        return (void*)aux;
    }

    // add this module
    void add_module(aggr::id_module_t* m) { modules.insert(m); }
    void remove_module(aggr::id_module_t* m) { assert(modules.find(m) != modules.end()); modules.erase(m); }

    // count modules.
    int count_modules() const;
    // count undominated modules.
    int count_undominated_modules() const;
    // is this gate non-inferred
    // (i.e., is one of the modules cover it an inferred module?
    bool is_noninferred() const;
    // mark modules which are overlapping.
    void mark_overlapping_modules();
    // merge modules.
    void merge_modules(void);

    // update the list of driven latches.
    // return true if a new latch was added.
    bool update_driven_latches(bool update_macros);
    // update the list of driving latches.
    // return true if a new latch was added.
    bool update_driving_latches(bool update_macros);

    void compute_driving_latches_mp();

    // accessor functions for driven
    const nodeset_t* get_driven_latches() const { return &driven_latches; }
    // and driving latches.
    const nodeset_t* get_driving_latches() const { return &driving_latches; }
    const nodeset_t* get_driving_latches_sp() const { return &driving_latches_sp; }

    unsigned num_driving_latches_sp() const { return driving_latches_sp.size(); }

    // is this other latch driven by this latch?
    bool is_driven_latch(node_t* n) const {
        return driven_latches.find(n) != driven_latches.end();
    }
    // is this other latch driving this latch?
    bool is_driving_latch(node_t* n) const {
        return driving_latches.find(n) != driving_latches.end();
    }
    bool is_driving_latch_mp(node_t* n) const {
        return driving_latches_sp.find(n) == driving_latches_sp.end();
    }
    bool is_driving_latch_sp(node_t* n) const {
        return driving_latches_sp.find(n) != driving_latches_sp.end();
    }

    bool has_self_loop() const {
        return driven_latches.find((node_t*)this) != driven_latches.end();
    }

    void add_match(
        flat_module_t* mod,
        int output,
        int polarity,
        kcover_t* kc
    )
    {
        bitslice_match_t m(mod, output, polarity, kc);
        matches.push_back(m);
    }

    matchlist_t::iterator matches_begin() { return matches.begin(); }
    matchlist_t::const_iterator matches_begin() const { return matches.begin(); }

    matchlist_t::iterator matches_end() { return matches.end(); }
    matchlist_t::const_iterator matches_end() const { return matches.end(); }

    void set_sibling(node_t* s) { sibling = s; sibling->set_sibling_back(this); }
    node_t* get_sibling() const { return sibling; }
    void set_sibling_back(node_t* s) { sibling_back = s; }
    node_t* get_sibling_back() const { return sibling_back; }

    void set_joint_module(const char* s) {
        if(s) {
            joint_module = std::string(s);
        }
    }
    const char* get_joint_module() const { return joint_module.c_str(); }

    void set_suppress_gate() { suppress_gate = true; }
    bool is_suppress_gate() const { return suppress_gate; }

    void set_suppress_output() { suppress_output = true; }
    bool is_suppress_output() const { return suppress_output; }

    void set_suppress_input_map(int idx) {
        assert(idx < (int) suppress_input_map.size());
        assert(idx >= 0);
        suppress_input_map[idx] = true;
    }
    bool is_suppress_input_map(int idx) const { return suppress_input_map[idx]; }

    node_t* get_d_input() {
        int idx = get_lib_elem()->get_dindex();
        if(idx == -1) return NULL;
        else return get_input(idx);
    }
    node_t* get_si_input() {
        int idx = get_lib_elem()->get_si_index();
        if(idx == -1) return NULL;
        else return get_input(idx);
    }
    node_t* get_se_input() {
        int idx = get_lib_elem()->get_se_index();
        if(idx == -1) return NULL;
        else return get_input(idx);
    }

    //Helper functions for shortest path
    void set_previous_node(node_t* n) {
	previous = n;
    }

    void set_dist_to_source(float dist) {
	dist_to_source = dist;
    }

    float get_dist_to_source() { return dist_to_source; }

private:
    // helper functions.
    bool is_duplicate(kcover_t* kc) const;

    // need this for kcover_analysis.
    friend class flat_module_t;
    friend class kcover_t;
};

struct node_graph_t
{
    typedef std::map<node_t*, nodeset_t> edgeset_t;
    edgeset_t edges_out;
    edgeset_t edges_in;

    void add_edge(node_t* a, node_t* b) {
        edges_out[a].insert(b);
        edges_in[b].insert(a);
    }

    int num_edges_in(node_t* n) {
        edgeset_t::iterator it = edges_in.find(n);
        if(it == edges_in.end()) return 0;
        else return it->second.size();
    }

    int num_edges_out(node_t* n) {
        edgeset_t::iterator it = edges_out.find(n);
        if(it == edges_out.end()) return 0;
        else return it->second.size();
    }

    void do_dfs(node_t* start, nodeset_t& nodes) {
        edgeset_t::iterator pos = edges_out.find(start);
        nodes.insert(start);

        if(pos != edges_out.end()) {
            nodeset_t& edges_out = pos->second;
            for(nodeset_t::iterator it = edges_out.begin(); it != edges_out.end(); it++) {
                node_t* n = *it;
                if(nodes.find(n) == nodes.end()) {
                    do_dfs(n, nodes);
                }
            }
        }
    }
};

struct toposort_cmp
{
    bool operator() (node_t* l, node_t* r) {
        return l->get_level() < r->get_level();
    }
};

struct kcover_cnt_cmp
{
    bool operator() (node_t* l, node_t* r) {
        return l->get_kcovers().size() < r->get_kcovers().size();
    }
};

std::ostream& operator<<(std::ostream& out, const nodeset_t& s);
std::ostream& operator<<(std::ostream& out, const nodelist_t& b);

#endif // __NODE_H_DEFINED__
