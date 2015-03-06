#ifndef _FLAT_MODULE_H_DEFINED_
#define _FLAT_MODULE_H_DEFINED_

#include <algorithm>
#include <utility>
#include <vector>
#include <queue>
#include <set>
#include "ast.h"
#include "aggr.h"
#include "symbol.h"
#include "library.h"
#include "common.h"
#include "kcover.h"
#include "cuddObj.hh"
#include "word.h"
#include "node.h"

// global variables.
extern int enable_propagation;

// type of a list of nodes.
typedef std::vector<node_t*> nodelist_t;

//type of a list of flat modules.
typedef std::vector<flat_module_t*> flat_module_list_t;

void inplace_intersection(nodeset_t& cone, const nodeset_t& new_cone);
aggr::id_module_t* create_id_module(const char* name, aggr::id_module_t::type_t typ, nodeset_t& nodes);

bool operator<(const nodeset_t& a, const nodeset_t& b);

class flat_module_t {
protected:
    struct entry_t {
        stm_t::stm_type type;
    };

    struct ipp_t : public input_provider_t {
        Cudd* mgr;

        ipp_t(Cudd* mgr) : mgr(mgr) {}
        virtual ~ipp_t() {}

        virtual BDD inp(int i);
        virtual BDD one();
        virtual BDD zero();
    };

    struct bdd_compare_t {
        bool operator() (const BDD& one, const BDD& two) const {
            return one.getNode() < two.getNode();
        }
    };

    typedef symbol_table_t<entry_t> stbl_t;
    stbl_t symbols;

    // name of this module.
    const char* module_name;

    // map between node "names" (i.e. output signal names) and nodes.
    typedef std::map<std::string, node_t*> map_t;
    map_t map;

    typedef std::vector<bool> markings_t;

    typedef std::pair<int, node_t*> intnode_t;
    typedef std::vector<intnode_t> intnodelist_t;
    typedef std::map<node_t*, int> selp_map_t;

    struct intnode_cmp_t {
        bool fanout_check;
        intnode_cmp_t(bool fo) : fanout_check(fo) {}
        bool operator() (const flat_module_t::intnode_t& a, const flat_module_t::intnode_t& b) const;
    };


    void create_a2dff_module(markings_t& markings);
    void add_marked_fanins(node_t* n, nodeset_t& internals, markings_t& markings);

    struct ram_t {
        // types.
        struct mux_cover_t {
            node_t* root;
            node_t* sel;
            node_t* a;
            node_t* b;

            mux_cover_t(node_t* root, node_t* s, node_t *a, node_t *b)
                : root(root),
                  sel(s),
                  a(a),
                  b(b)
            {
            }
            bool operator<(const mux_cover_t& o) const {
                if(root < o.root) return true;
                if(root > o.root) return false;

                if(sel < o.sel) return true;
                if(sel > o.sel) return false;

                if(a < o.a) return true;
                if(a > o.a) return false;

                return b < o.b;
            }
        };
        typedef std::set<mux_cover_t> mux_cover_set_t;

        // variables.
        flat_module_t* module;
        nodeset_t rdaddr;
        nodeset_t inputs; // these are the latches.
        nodeset_t outputs;
        nodeset_t wraddr;
        nodeset_t wrdata;

        // constructor.
        ram_t(intnodelist_t& inputs, int num_sel, int num_lat, node_t* o);

        // function.
        bool same_inputs(intnodelist_t& inputs, int num_sel, int num_lat) const;
        bool same_latches(ram_t* other);
        void add_output(node_t* o) { outputs.insert(o); }
        void add_input(node_t* i) { inputs.insert(i); }
        aggr::id_module_t* get_module() const;

        void write_analysis();
        bool processCovers(mux_cover_set_t& covers);
        bool verify_decoder(nodeset_t& sel_signals, nodeset_t& inputs, selp_map_t& selp);
        void addCovers(flat_module_t* module, BDD& mux, nodeset_t& ram_inputs, nodeset_t& covered_inputs, mux_cover_set_t& covers);
        void addCovers2(flat_module_t* module, nodeset_t& ram_inputs, nodeset_t& covered_inputs, mux_cover_set_t& covers);
        void extendRAM(aggr::id_module_t* mux, std::vector<int>& indices);

        unsigned num_rdaddr() const { return rdaddr.size(); }
        unsigned num_inputs() const { return inputs.size(); }
        unsigned num_outputs() const { return outputs.size(); }

    };
    typedef std::vector<ram_t*> ramlist_t;

    typedef std::pair<int, int> intpair_t;
    typedef std::map<intpair_t, ramlist_t> ram_map_t;

    static void expand_not_gates(nodeset_t& inputs);
    static void create_common_fanin_cone(const nodeset_t& signals, nodeset_t& cone);

    void computeMUX21List(modulelist_t& muxes);
    void extendRAM(flat_module_t::ram_t* ram, modulelist_t& muxes);
    bool muxCompatibleWithRAM(const nodeset_t& nodes, aggr::id_module_t* mux, std::vector<int>& indices);

    ram_map_t rams;

    // list of nodes.
    nodelist_t inputs;
    nodelist_t gates;
    nodelist_t latches;
    nodelist_t outputs;
    nodelist_t macros;
    nodelist_t macro_outs;

    unsigned real_gates;

    // a count of the achieved coverage.
    int achieved_coverage;
    // a counter for generating fake wirenames.
    int wire_ctr;
    // a counter for generating fake instance names.
    int instance_ctr;

    // list of all the words here.
    wordlist_t words;
    // hashtable of the words: used to find duplicates.
    word_hashtable_t word_htbl;

    // list of flat modules for additional library elements
    flat_module_list_t libraryElements;

    //list of flat modules for additional partial function analysis
    flat_module_list_t partialFuncModules;
    std::vector<BDD> partialFuncBDDs; //BDDs of top partial functions to look for.

    // vector of BDDS corresponding to each output of flat module (for library elements)
    std::vector<BDD> outputBDDs;
    //...and for inputs of flat module (for library elements)
    std::vector<BDD> inputBDDs;

    // list of all identified modules.
    modulelist_t modules;

    const int debug;
    const int summarize_modules;
    ILibrary* lib;

    bool error;

    static Cudd fullMgr; // manager for the full functions.
    static std::vector<Cudd*> mgrs;
    static ipp_t* full_ipp;
    static std::vector<ipp_t*> ipps;

    typedef std::map<BDD, fnInfo_t, bdd_compare_t> bddset_t;
    bddset_t bdds;
    int prune_count;
    int cover_count;

    bool lcg_computed;

    // the sibling map is a map between module_inst_t pointers (from the AST)
    // and node_t pointers (in the  circuit graph). we will use this to fill
    // up the sibling field of the node_t structure.
    typedef std::map<module_inst_t*, node_t*> sibling_map_t;

    void summarize_instances(module_t* module);
    void parse_wire(stm_t* stm, wirename_t& w);
    void find_declarations(module_t* module);
    bool verify_module_inst(module_t* module);
    bool verify_module_inst(module_inst_t* mi);
    bool create_wirenames(module_t* module, map_t& port_map);
    void process_siblings(sibling_map_t& sibling_map, module_t* module);
    bool mark_outputs(module_t* module);
    bool create_nodeinputs(module_t* module);
    void create_nodelists();
    void create_latch_gates();
    void create_latch_gate(node_t* n, std::list<node_t*>& new_nodes);
    void replace_inputs(node_t* this_node, node_t* old_input, node_t* new_input);
    void rewrite_nodes();
    void create_fanouts();
    void create_words();
    void assign_indices();
    void rewriteInverters(FILE* fp);
    void rewriteBuffers(FILE* fp);
    void removeNOR2XB(FILE* fp);
    bool rewriteNodes(FILE* fp, node_t* orig_node, node_t* new_node);
    void removeDeadNodes(FILE* fp);
    void topo_sort();
    void mark_scan_enables();

    void compute_repr();
    void compute_repr(const std::string& type, modulelist_t& m);
    void compute_sliceability();

    void dump_intersecting_nodes(node_t* n);
    void dump_driven_latches();
    void dump_driving_latches();
    void markExclusive11s(kcover_t* cover, BDD& fn, input_provider_t* e);
    void createFunction(kcover_t* cover);
    void create_not_relation();
    void markNotRelation(BDD& x, BDD& notx);
    void match_lib_elems();
    bool create_simple_node(module_inst_t* mi, lib_elem_t* libelem, sibling_map_t& sibling_map);
    bool create_macro_node(module_inst_t* mi, lib_elem_t* libelem, sibling_map_t& sibling_map, map_t& port_map);
    bool get_outputs(module_inst_t* mi, lib_elem_t* libelem, stringlist_t& outputs);
    bool get_ports(module_inst_t* mi, lib_elem_t* libelem, stringlist_t& ports);
    bool handle_ports(module_t* module, map_t& port_map);
    bool create_inputs(module_inst_t* mi, lib_elem_t* libelem);
    bool create_macro_inputs(module_inst_t* mi, lib_elem_t* libelem);
    void mark_loop(nodelist_t& visited);
    void processLibElemMatch(
        FILE* fp,
        flat_module_t* module,
        int i,
        fnInfo_t* info,
        int polarity
    );
    void count_word_types(std::ostream& out);

    void add_ram(intnodelist_t& inputs, node_t* output, int num_sel, int num_lat);

    // Statistics about the k-covers.
    std::map<int, int> kcover_size_cnt;
    std::map<int, int> kcover_min_depth;
    std::map<int, int> kcover_max_depth;
    int numPhonyCovers;
    std::map<node_t*, node_t*> notMap;

    // Graph for identifying multipliers.
    typedef std::map<node_t*, nodeset_t> node_adj_list_t;
    node_adj_list_t andRelation;
    node_adj_list_t nandRelation;
    node_adj_list_t norRelation;
    node_adj_list_t orRelation;
    void create_product_relations();
    void create_product_relation(kcoverset_t& covers, node_adj_list_t& adj_list);
    static void create_init_x(node_adj_list_t& relation, nodeset_t& x);
    static void create_init_y(node_adj_list_t& relation, node_t* x, nodeset_t& y);
    static bool is_fully_connected(node_t* a, nodeset_t& bs, node_adj_list_t& relation);
    static bool refine(nodeset_t& as, nodeset_t& bs, node_adj_list_t& relation);
    static void search_products(node_adj_list_t& relation);


    typedef std::pair<node_t*, node_t*> node_pair_t;
    typedef std::set< node_pair_t > node_pair_set_t;
    node_pair_set_t exclusive11Relation;

    typedef std::map<node_t*, nodeset_t> not_relation_t;
    not_relation_t notRelation;

    void dump_kcover_info_file();
    void dump_fn_stats();
    void printAllCofactors(BDD& bdd, std::istream& in, std::map<node_t*, BDD>& varMap);
    void do_dfs(node_t* start, nodeset_t& edges, nodeset_t& visited);
    BDD buildFns(node_t* start, nodeset_t& edges, std::map<node_t*, BDD>& fns);

    void propagate_words();
    bool can_propagate_back(const word_t* w) const;
    void propagate_back(const word_t* w, wordlist_t& new_words);
    void propagate_forward(const word_t* w, wordlist_t& new_words);
    void getValidFanoutSet(std::vector<lib_elem_t*>& fanoutTypes,
                           std::set<lib_elem_t*>& validFanoutSet);
    void createFanoutTypeIntersection(const word_t* w, std::set<lib_elem_t*>& fanoutTypeIntersection);
    void create_fwdprop_words( const word_t* w, nodelist_t& list, wordlist_t& new_words );

    static bool has_marked_support(std::vector<bool>& markings, node_t* n);

    bool mux_node_check(bool fanout_check, node_t* node, markings_t& markings, markings_t& covered);
    void do_mux_dfs(node_t* node, intnodelist_t& inputs, markings_t& markings, int level, nodeset_t* internals);
    static bool is_node_present(intnodelist_t& inputs, node_t* n);

    void reduce_inputs(node_t* node, intnodelist_t& inputs, int& num_selects, int num_latches);
    void trim_inputs(intnodelist_t& inputs, int& num_selects, int num_latches);

    static void compute_internals(node_t* node, intnodelist_t& inputs, nodeset_t& internals);
    bool in_all_sets(std::vector<nodeset_t>& sets, node_t* n);
    static void add_set_with_inputs(nodeset_t& out, const nodeset_t& in);

    void ip_fwdprop(std::vector<std::string>& tokens);
    void ip_get_nodes(std::vector<std::string>& tokens, nodelist_t& nodes);

    struct mergeable_check_t
    {
        std::string first;
        std::string second;
        bool compare_output_size;
        std::string input_group;

        mergeable_check_t(std::string f, std::string s, bool cmp_outsize, const char* inpgrp)
          : first(f)
          , second(s)
          , compare_output_size(cmp_outsize)
          , input_group(inpgrp)
        {
        }

        bool check(const aggr::id_module_t* m1, const aggr::id_module_t* m2) const
        {
            const char* t1 = m1->get_type();
            const char* t2 = m2->get_type();
            return 
               strstr(t1, first.c_str()) == t1 &&
               strstr(t2, second.c_str()) == t2 &&
               (!compare_output_size || (m1->total_outputs() == m2->total_outputs())) &&
               (input_group.size() == 0 || check_input_groups(m1, m2));
        }

        bool check_input_groups(const aggr::id_module_t* m1, const aggr::id_module_t* m2) const;
    };

    bool check_mergeables(std::vector<mergeable_check_t>& checks, 
                          const aggr::id_module_t* m1, 
                          const aggr::id_module_t* m2,
                          bool knownMergesOnly);
    aggr::id_module_t* merge_modulelist(const std::string& name, modulelist_t& mods, bool kill_mods);
    void create_merged_modules(std::vector< moduleset_t >& edges, std::vector< moduleset_t >& edges2, bool kill_mods);

    void module_group_mark(std::vector<int>& groups, std::vector< moduleset_t >& edges, int i, int mark);
    void compute_repr(modulelist_t& links, std::vector< modulelist_t >& groups);
    aggr::id_module_t* get_repr(modulelist_t& links, unsigned module_index);
    void renumberModules();
public:
    typedef std::map<node_t*, BDD> bdd_map_t;

    void compute_latch_connections();
    flat_module_t(ILibrary* library, module_list_t* modules, int index);
    ~flat_module_t();

    operator bool() const { return !error; }
    bool valid() const { return !error; }

    const char* get_module_name() const { return module_name; }

    void dump_verilog(std::ostream& out, std::ostream& libOut);
    void dump_nodes(std::ostream& out);
    void test_jan23();
    void test_mar8();
    void find_loops();
    void do_loop_dfs(node_t* n, node_t* root, nodeset_t& visited, nodelist_t& path, nodelist_t& best_path);

    unsigned num_real_gates() const { return real_gates; }

    void interactive_propagator();
    void readExternalModules(const std::string& filename);
    void output_words(std::ostream& out);

    //Add modules for partial function analysis
    void addPartialFuncModule(flat_module_t* newMod) { partialFuncModules.push_back(newMod); }
    //Return BDD matching fnInfo_t*
    BDD getBDDfromFn(fnInfo_t* info);


    //Add library elements method (re: library elements)
    void addLibElem(flat_module_t* newElement) { libraryElements.push_back(newElement); }

    //Get number of library elements (re: library elements)
    unsigned num_libraryElements() { return libraryElements.size(); }

    //Get flat module of ith library element (re: library elements)
    flat_module_t* get_libraryElement(int index) { return libraryElements[index]; }

    //Return pointer to list of library elements (re: library elements)
    flat_module_list_t* get_libraryElements() { return &libraryElements; }

    //Add BDD for output and input of module (re: library elements)
    void addOutputBDD(BDD newBDD) { outputBDDs.push_back(newBDD); }
    void addInputBDD(BDD newBDD) { inputBDDs.push_back(newBDD); }

    //Get BDD for ith input/output (re: library elements)
    BDD get_outputBDD(int index) { return outputBDDs[index]; }
    BDD get_inputBDD(int index) { return inputBDDs[index]; }

    node_t* get_output(int i) { return outputs[i]; }
    node_t* get_input(int i) { return inputs[i]; }

    //Get BDD list for outputs and inputs (re: library elements)
    std::vector<BDD>* get_outputBDDs() { return &outputBDDs; }
    std::vector<BDD>* get_inputBDDs() {return &inputBDDs; }

    void add_word(word_t* w);

    word_t* get_canonical_word(word_t* w);

    bool similar_word_exists(word_hashtable_t& htbl, word_t* w) const;

    bool word_exists(word_t* w) const;

    void add_module(aggr::id_module_t* m) { modules.push_back(m); }

    void simple_word_analysis();

    void bdd_sweep();

    void kcover_analysis();

    void dump_nodecounts(std::ostream& out);

    wordlist_t* get_words () { return &words; };

    nodelist_t* get_latches () { return &latches; };

    nodelist_t* get_inputs () {return &inputs; };

    nodelist_t* get_gates () {return &gates; };

    nodelist_t* get_outputs () {return &outputs; };

    nodelist_t* get_macros () {return &macros; };

    nodelist_t* get_macro_outs () {return &macro_outs; };

    unsigned num_latches() const { return latches.size(); }
    unsigned num_inputs() const { return inputs.size(); }
    unsigned num_gates() const { return gates.size(); }
    unsigned num_outputs() const { return outputs.size(); }

    void count_unate_vars();
    void compute_cover_distance();

    void dist_bfs_input(node_t* inp);
    void dist_bfs_output(node_t* inp);
    void compute_distances();
    void analyze_a2dff(std::ostream& out);
    void create_xbar_module(const nodelist_t& nands, std::map<node_t*, int>& ins, std::map<node_t*, int>& outs);
    static void xbar_dfs(node_t* n, markings_t& marks, nodeset_t& visited, nodeset_t& inputs);
    static node_t* get_latch_input2(node_t* latch_out);
    static int count_included_fanouts(node_t* n, aggr::id_module_t* mod);
    bool verify_xbar(const nodeset_t& inputs, const nodeset_t& outputs, std::map<node_t*, int>& uses);
    bool verify_xbar_node(Cudd& mgr, bdd_map_t& vars, node_t* out, int num_selects);
    void process_xbar_output_group(aggr::id_module_t* mod, const nodeset_t& bits);


    void analyze_axi_nand();
    void process_axi_nand4s(nodelist_t& nands);
    static int get_rsttrees(const nodeset_t& set);
    static int get_fanout_rst(const nodeset_t& set);
    static void dump_node_int_map(std::ostream& out, const std::map<node_t*, int>& m);
    static void dump_node_int_map2(std::ostream& out, const std::map<node_t*, int>& m);

    void dump_dot_name(std::ostream& out, node_t* n, std::map<node_t*, nodelist_t>& ffmap);
    void dump_dot_node(std::ostream& out, node_t* n, std::map<node_t*, nodelist_t>& ffmap);

    static void mark_fanouts(node_t* n, markings_t& marks);

    // do a breadth first search to find the clk tree.
    void tree_bfs(const char* moduleName, const char* signalName, node_t* clk);
    typedef enum { MARK_CLKTREE, MARK_RSTTREE } marking_t;
    void mark_tree(node_t* clk, int clk_index, marking_t type);
    void summarize_tree_info(std::ostream& out);
    void count_tree_info(nodelist_t& nodes, std::map<int, int>& clk_cnt, std::map<int, int>& rst_cnt);
    void dump_tree_info(std::map<int, int>& clk_cnt, std::map<int, int>& rst_cnt, const char* label, std::ostream& out);
    void propagateTreeMarks();

    void createModuleFromResetTree(int color, std::string& name);

    struct col_t {
        node_t* n;
        int col;
        int dir;

        col_t(node_t* node, int color, int direction)
            : n(node), col(color), dir(direction)
        {
        }
    };

    struct col_depth_t {
        node_t* n;
        int col;
        int dpt;
        int dir;

        col_depth_t(node_t* node, int color, int depth, int direction)
            : n(node), col(color), dpt(depth), dir(direction)
        {
        }
    };

    void glpk_create_constraints(std::vector< std::vector<int> >& constraints);
    void color_analysis();
    void pinBackProp();
    void backProp(std::queue< std::pair<node_t*, int> >& bfsq);
    void create_color_modules();
    static int get_color(int c);
    aggr::id_module_t* create_module(const char* name, const nodeset_t& gates);

    void printPath(node_t* start, node_t* end);
    bool printPathImpl(node_t* start, node_t* end, std::map<node_t*, bool>& visited);

    int get_count(BDD bdd);

    static Cudd& getMgr(int idx) {
        if(idx == -1) {
            return getFullFnMgr();
        } else {
            assert(idx >= 0 && idx < (int) ipps.size());
            return *mgrs[idx];
        }
    }

    static input_provider_t* get_ipp(int idx) {
        if(idx == -1) {
            return full_ipp;
        } else {
            assert(idx >= 0 && idx < (int) ipps.size());
            return ipps[idx];
        }
    }

    void incr_prune_count() { prune_count += 1; }
    void incr_cover_count() { cover_count += 1; }

    fnInfo_t* getFunction(const BDD& fn);

    void findCounters(std::string& filename);
    void traceWords(std::string& filename);
    void get_NC_circuit(std::string& vfn);
        //check_connectivity within combinational logic through one direction
        bool check_connectivity(node_t* nf, node_t* ni,nodeset_t& visited);
        bool check_latch_connectivity(node_t* nf, node_t* ni,nodeset_t& visited);
        bool check_fout_connectivity_with_set(node_t* ni, nodelist_t & nset, nodeset_t& visited, nodeset_t & ndpth);
        bool check_fout_1st_connectivity_with_set(node_t* ni, nodelist_t & nset);
        bool check_inp_connectivity_with_set(node_t* nf, nodelist_t & nset, nodeset_t& visited, nodeset_t & ndpth);
        bool check_inp_1st_connectivity_with_set(node_t* nf, nodelist_t & nset);
        void aggregate_nodes(node_t * n, nodelist_t& nset, nodeset_t& checked, nodelist_t * inputs, nodelist_t * outputs, nodeset_t & drivennodes, nodeset_t & drivingnodes);
        void get_connected_nodes(nodeset_t & nset, nodelist_t * inputs, std::vector<nodeset_t> & v_sets,bool include_latch);
        void narrow_connected_set(std::vector<nodeset_t> & v_sets, std::vector<nodelist_t> & v_lists);
        void inp_narrow_connected_set(std::vector<nodeset_t> & v_sets, std::vector<nodelist_t> & v_lists);
        void full_narrow_connected_set(std::vector<nodeset_t> & v_sets, std::vector<nodelist_t> & v_lists);
        typedef std::pair<node_t*, node_t*> nodepair_t;
		void get_module_givenIO(nodeset_t& outputs, nodeset_t& inputs,std::map<node_t*, int>& chklist, char * chr, nodeset_t& ilatches, bool ignore_latch);
		bool check_node(node_t* n, std::map<node_t*, int>& chklist,nodeset_t& visited,nodeset_t& inps, nodeset_t& outs,nodeset_t& ilatches, bool ignore_latch);
		bool get_intermediate_latches(node_t* n, std::map<node_t*, int>& chklist,nodeset_t& visited,nodeset_t& inps, nodeset_t& outs);
		void reduce_extra_inputs(node_t* n, nodeset_t& inps1, nodeset_t& inps_visited);
    void findShiftRegs();

    void aggregate();
    void registerAnalysis();
    void analyzeFrameBuffer();
    void analyzeFrameBufferOutputs(const nodeset_t& nodes, markings_t& fbouts);
    void analyzeVGADecoder(const markings_t& fbouts, const nodelist_t& decoder_outputs);

    void simpleFIFOAnalysis();
    void simpleFIFOAnalysis2();

    struct muxblock_t
    {
        node_t* a;
        node_t* b;
        node_t* s;
        node_t* y;
        node_t* q;

        std::vector<muxblock_t*> edges_out;
        std::vector<muxblock_t*> edges_in;

        muxblock_t(node_t* a, node_t* b, node_t* s, node_t* y, node_t* q)
            : a(a), b(b), s(s), y(y), q(q)
        {
        }

        bool edge_exists_to(const muxblock_t& mux) const
        {
            return (mux.a == y || mux.b == y || mux.a == q || mux.b == q);
        }

        void add_edge_out(muxblock_t* b) { edges_out.push_back(b); }
        void add_edge_in(muxblock_t* b) { edges_in.push_back(b); }
    };

    typedef std::vector<muxblock_t> muxblock_list_t;
    typedef std::vector< std::vector<muxblock_t*> > muxblock_list_list_t;
    typedef std::map<int, muxblock_list_list_t> muxblock_len_result_t;
    typedef std::map< node_t*, std::vector<muxblock_t*> > muxblock_sel_result_t;

    void createMuxBlocks(BDD& muxBDD, muxblock_list_t& l);
    void muxblk_dfs(muxblock_t* root, std::vector<muxblock_t*>& path, muxblock_len_result_t& res);

    BDD createFullFn(node_t* root, bdd_map_t& varMap, bool memoize, int inputs);
    void bddSweep();
    static Cudd& getFullFnMgr() { return fullMgr; }

    void readDFSInputs(const char* filename, nodeset_t& edges, std::vector<node_t*>& outputs, std::string& moduleName);
    void addUserDefinedModule(const char* filename);
    void printFullFunctions(const char* filename);
    void printVarMap(std::map<node_t*, BDD> varMap);

    node_t* get_node_by_name(const char* name) const;
    bool not_related(node_t* x, node_t* y) const;
    const nodeset_t* get_not_related(node_t* x) const;
    bool exclusive11_related(node_t* a, node_t* b) const;

    node_t* get_pad_port(node_t* n) const;
    bool is_pad_input(node_t* n) const;

    // count the number of gates covered by some module.
    int count_covered_gates();
    // count the number of undominated modules
    int get_undominated_module_count() const;
    // count the total number of modules.
    int get_module_count() const;
    // count the number of non-overlapping modules.
    int get_non_overlapping_module_count();
    // count the number of non-overlapping undominated modules.
    int get_non_overlapping_undominated_module_count();
    // count the number of gates present in more than one module.
    int count_conflicting_gates();
    // create modules matching library elements.
    void create_matching_modules();
    // find dominated modules.
    void merge_modules(void);
    // delete all bogus modules.
    void cleanup_modules(void);
#ifdef USE_CPLEX
    // eliminate overlaps using the ILP solver.
    void eliminate_overlaps();
    // a "sliceable" version of eliminate overlaps.
    void eliminate_overlaps_sliceable();
#endif
    // eliminate overlaps using the GLPK ILP solver.
    void eliminate_overlaps_glpk();
    // mark conflicting modules.
    void mark_conflicting_modules(bool compute_conflicts);
    // dump coverage information to a file.
    void dump_coverage_info(std::ostream& out);
    void dump_summary(std::ostream& out);
    const char* get_type(const char* t);
    // dump unknown modules to a verilog file.
    void dump_unknown_modules(std::ostream& out);

    // get the latch inputs for a list that contains the latch outputs.
    int get_covered_redundant_gate_count() const;
    void get_latch_inputs(nodelist_t& out, nodelist_t& in) const;
    node_t* get_latch_input(node_t* q) const;

    // partial function analysis: count frequency of functions in circuit and
    // number of gates covered by function
    void partialFunctionAnalysis();
    void partialFunctionAnalysis(FILE* fp);

    node_pair_t createSymmPair(node_t* a, node_t* b) {
        if(a < b) return node_pair_t(a, b);
        else return node_pair_t(b, a);
    }

    struct node_wrapper_t {
        node_t* node;
        node_wrapper_t* root;
        int rank;

        void init() {
            root = this;
            rank = 0;
        }
    };

    typedef std::vector<node_wrapper_t> node_wrapper_list_t;

    void link(node_wrapper_t* a, node_wrapper_t* b) {
        if(a->rank > b->rank) {
            b->root = a;
        } else if(b->rank > a->rank) {
            a->root = b;
        } else {
            a->root = b;
            b->rank += 1;
        }
    }

    node_wrapper_t* find_set(node_wrapper_t* x)
    {
        if(x != x->root) {
            x->root = find_set(x->root);
        }
        return x->root;
    }

    void analyzeCommonInputs(const nodelist_t& gates_in, bool update_support_rep);
    void analyzeCommonInputs();
    void dumpInstanceNames(std::ostream& input_file, std::ostream& output_file, std::ostream& instance_file);
    void dump_to_file(std::ostream& out, const std::set<std::string>& set);
    void analyzeDecoders(const nodeset_t& nodes, modulelist_t& mods);
    static bool checkExclusivity(bool polarity, Cudd& mgr, std::set<BDD, bdd_compare_t>& funcs);
    static bool checkPopCnt(bool inv, int vars, Cudd& mgr, std::vector<BDD>& funcs);
    aggr::id_module_t* analyzeCommonNodes(const nodeset_t& inputs, const nodeset_t& nodes);

    // signal flow analysis: look for "unknown" but commonly occuring
    // bitslices which have one of the following two patterns:
    //  a. common signals.
    //  b. signal propagation.
    void signalFlowAnalysis();
    void ramAnalysis(bool fanout_check);
    void markFrameBufferOutputs(markings_t& marks);
    void memoryReadAnalysis();
    // conflict analysis: look at what gates ended up in multiple modules.
    void conflictAnalysis();
    void adjacencyAnalysis();
    void fittingAnalysis();
    void mergeMuxes(bool kill_mods);

    static void setup_cudd();
    static void destroy_cudd();

    // functions used by the node rewriter in the library.
    std::string gen_wirename();
    std::string gen_instancename();
    void add_node(node_t* node);

    unsigned max_index() const;
    static void intersect(moduleset_t& set, modulelist_t& list);

    // finds inputs of a node
    static void find_inputs(node_t* n, const nodeset_t& cone, nodeset_t& visited, nodeset_t& inputs);
    static void dump_nodeset(std::ostream& out, const nodeset_t& s);

    friend void aggr::findPopCnts(flat_module_t* mod);
    friend bool aggr::findPopCnts(flat_module_t* flat, id_module_t* mod, std::vector<node_t*>& outnodes);
};

namespace xbar_n {
    struct group_t {
        node_t* gate;
        node_t* rep_gate;
        group_t* root;
        int rank;
    };

    group_t* new_group(node_t* gate, node_t* rep);
    bool same_group(group_t* g1, group_t* g2);
    void link(group_t* g1, group_t* g2);
    group_t* get_root(group_t* g);
};

std::ostream& operator<<(std::ostream& out, const flat_module_t::muxblock_t& mux);
std::ostream& operator<<(std::ostream& out, const std::vector<flat_module_t::muxblock_t*>& list);

bool check_latch(node_t* n, bool fanout_check);

#endif // _FLAT_MODULE_H_DEFINED_
