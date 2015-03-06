#ifndef _AGGR_H_DEFINED_
#define _AGGR_H_DEFINED_

#include "cuddObj.hh"
#include <set>
#include <list>
#include <vector>
#include <iostream>
#include "library.h"
#include "utility.h"
#include "common.h"
#include "word.h"
#include <algorithm>

class node_t;
class kcover_t;
class flat_module_t;
class fnInfo_t;
class word_t;
class verilog_lib_t;
class decl_list_t;

// type of an array of nodelists.
namespace aggr {
    BDD mux21(input_provider_t* e);

    struct bdd_compare_t {
	    bool operator() (const BDD& one, const BDD& two) const {
		    return one.getNode() < two.getNode();
	    }
    };

    typedef std::map<BDD,node_t*, bdd_compare_t> bdd_node_map_t;

    struct nodelist_t 
    {
        std::vector<node_t*> nodes;

        // add node function.
        void add_node(node_t* n) { nodes.push_back(n); }
        // constructor.
        nodelist_t(node_t* a, node_t* b) {
            add_node(a);
            add_node(b);
        }

        // size function.
        unsigned size() const { return nodes.size(); }

        // comparison.
        bool operator<(const nodelist_t& other) const
        {
            assert(size() == other.size());
            for(unsigned i=0; i != size(); i++) {
                node_t* a = nodes[i];
                node_t* b = other.nodes[i];
                if(a < b) return true;
                if(a > b) return false;
            }
            return false;
        }
    };

    struct bitslice_t;
    typedef std::set<bitslice_t*> bitslice_set_t;

    struct bitslice_t
    {
    private:
        bitslice_t() {}
    public:
        // inputs.
        std::vector<node_t*> xs;
        // outputs.
        std::vector<node_t*> ys;
        // pointer to the "parter" bitslices (e.g., partners of carry bitslics are sum bitslices).
        bitslice_set_t partners;

        // some information required for the adder bitslices.

        // constructor.
        bitslice_t(kcover_t* kcov);
        // copy constructor.
        bitslice_t(const bitslice_t& other);

        // sort the inputs in ptr-order.
        void sort_inputs();

        // number of inputs.
        unsigned numInputs() const { return xs.size(); }

        // number of outputs.
        unsigned numOutputs() const { return ys.size(); }

        // ordering operation for throwing into a map.
        bool operator<(const bitslice_t& other);

        // equality test relation.
        bool operator==(const bitslice_t& other) const;

        // same inputs?
        bool same_inputs(const bitslice_t& other) const;

        bool has_input(const node_t* n) const;

        // similar inputs?
        bool not_related_inputs(const bitslice_t& other) const;

        bool inv_contained(const bitslice_t& other) const;

        // is the argument bitslice an input of this bitslice
        bool is_input(int output_index, bitslice_t* other) const;

        // merge these two k-covers.
        static bitslice_t* mergeSlices(bitslice_t* a, bitslice_t* b);

        void add_internals(node_t* n, std::set<node_t*>& internals) const;

        bool is_covered() const;
    };

    struct bitslice_chain_user_t 
    {
        virtual void use(std::list<bitslice_t*>& l) = 0;
    };

    class bitslice_graph_t
    {
    public:
        typedef std::map<bitslice_t*, bitslice_set_t> edge_list_t;
        typedef std::pair<bitslice_t*, bitslice_t*> bitslice_pair_t;
        edge_list_t edges_in;
        edge_list_t edges_out;
    public:
        bitslice_graph_t() {}
        void add_edge(bitslice_t* a, bitslice_t* b);
        void remove_edge(bitslice_t* a, bitslice_t* b);
        void remove_node(bitslice_t* a);
        unsigned num_edges_in(bitslice_t* bt) const {
            edge_list_t::const_iterator pos = edges_in.find(bt);
            if(pos == edges_in.end()) return 0;
            else {
                return pos->second.size();
            }
        }
        unsigned num_edges_out(bitslice_t* bt) const {
            edge_list_t::const_iterator pos = edges_out.find(bt);
            if(pos == edges_out.end()) return 0;
            else {
                return pos->second.size();
            }
        }

        void enumerateChains(bitslice_chain_user_t& user);
        void enumerateChains(bitslice_t* start, bitslice_chain_user_t& user, std::list<bitslice_t*>& l);
    };

    typedef std::vector<bitslice_t*> bitslice_list_t;

    typedef std::pair<bitslice_t*, bitslice_t*> adder_t; 
    typedef std::vector<adder_t> adderlist_t;

    struct carry_chain_t
    {
        typedef std::list< bitslice_t* > list_t;
        list_t slices;
        bool dominated;
        bool dead;

        carry_chain_t(bitslice_t* bt) {
            slices.push_back(bt);
            dominated=false;
            dead=false;
        }

        carry_chain_t(list_t& l) : slices(l)
        {
            dominated=false;
            dead=false;
        }

        bool addIfPossible(bitslice_t* bt);
        bool dominates(carry_chain_t* other);
        unsigned size() const { return slices.size(); }
        bool operator==(const carry_chain_t& other) const;
    };

    // output a bitslice list.
    std::ostream& operator<<(std::ostream& out, const bitslice_list_t& bits);
    // output a carry chain.
    std::ostream& operator<<(std::ostream& out, const carry_chain_t& cc);

    // stream output.
    std::ostream& operator<<(std::ostream& out, const bitslice_t& b);
    std::ostream& operator<<(std::ostream& out, const nodelist_t& b);

    // 2:1 muxes.
    void identifyMuxes(flat_module_t* module);
    void aggregateMuxes(flat_module_t* module, fnInfo_t* muxInfo, const char* muxName, int sel_index, int ai, int bi);

    struct sum_node_t {
        std::vector<node_t*> inputs;
        bitslice_t* slice;

        sum_node_t(bitslice_t* bs) : inputs(bs->xs), slice(bs) {
            assert(bs->xs.size() == 3 || bs->xs.size() == 2);
            assert(bs->ys.size() == 1);
            sort();
        }
        sum_node_t(node_t* a, node_t* b) {
            inputs.push_back(a);
            inputs.push_back(b);
            slice = NULL;
            sort();
        }
        sum_node_t(node_t* a, node_t* b, node_t* c) {
            inputs.push_back(a);
            inputs.push_back(b);
            inputs.push_back(c);
            slice = NULL;
            sort();
        }

        void sort() {
            std::sort(inputs.begin(), inputs.end());
        }


        bool operator<(const sum_node_t& other) const {
            if(inputs.size() < other.inputs.size()) return true;
            else if(inputs.size() > other.inputs.size()) return false;
            else {
                for(unsigned i=0; i != inputs.size(); i++) {
                    node_t* a = inputs[i];
                    node_t* b = other.inputs[i];
                    if(a < b) return true;
                    else if(a > b) return false;
                }
                return false;
            }
        }
    };
    typedef std::set<sum_node_t> sum_node_set_t;

    struct adder_creator_t : public bitslice_chain_user_t
    {
        flat_module_t* module;
        sum_node_set_t& sums;

        adder_creator_t(flat_module_t* mod, sum_node_set_t& s) 
            : module(mod)
            , sums(s) 
        {
        }
        virtual void use(std::list<bitslice_t*>& l);
    };


    // rippe carry adder.
    void identifyRCAs(flat_module_t* module);
    void findRCA(const char* s, std::vector<bitslice_t*>& sums, std::vector<bitslice_t*>& carrys);
    void findCarryChains(flat_module_t* module, std::vector<bitslice_t*>& carry, bitslice_list_t& sums);
    void findCarryChains2(flat_module_t* module, bitslice_list_t& carrys, bitslice_list_t& sums);
    void createAdderChain(carry_chain_t* cc, bitslice_list_t& sums, adderlist_t& adders);
    void createAdderChain2(carry_chain_t* cc, sum_node_set_t& sums, adderlist_t& adders);
    std::ostream& operator<<(std::ostream& out, adderlist_t& adders);
    void createAdderModule(flat_module_t* module, adderlist_t& adders);
    bool canJoinAdders(bitslice_t* a, bitslice_t* b);

    void fill_vector(node_t* x, std::vector<node_t*>& xs);
    void find_matching_sums(sum_node_set_t& sums, bitslice_t* carry);

    // 4:1 muxes.
    void identify41Muxes(flat_module_t* module);
    void aggregate41Muxes(flat_module_t* module, fnInfo_t* info, const char* name);

    // 3:1 muxes
    void identify31Muxes(flat_module_t* module);
    void aggregate31Muxes(flat_module_t* module, fnInfo_t* info, const char* name);

    // utility;
    void deleteSlices(std::vector<bitslice_t*>& slices);
    bool repeatedInputs(std::vector<bitslice_t*>& bits);
    void printCovers(const char* prefix, std::set<kcover_t*>& covers);
    void addCovers(std::set<kcover_t*>& covers, std::vector<bitslice_t*>& slices);
    void printBanner(const char* s);

    // create a word and return it.
    word_t* createInputWord(flat_module_t* module, std::vector<bitslice_t*>& bits, int pos, int th);
    word_t* createOutputWord(flat_module_t* module, std::vector<bitslice_t*>& bits, int pos, int th);


    typedef std::map<std::string, std::string> stringmap_t;

    class id_module_t;
    typedef std::vector< std::vector<id_module_t*> > modulelist_array_t;

    class id_module_t
    {
    public:
        typedef std::vector<node_t*> nodelist_t;
        typedef std::vector<word_t*> wordlist_t;
        typedef std::vector<int> taglist_t;
        typedef std::set<node_t*> nodeset_t;
        typedef std::map<std::string, nodelist_t> node_groups_t;
        typedef std::map<node_t*, std::string> pin_map_t;
        typedef std::set<aggr::id_module_t*> moduleset_t;
        enum type_t { USER_DEFINED, INFERRED, CANDIDATE_WORD_BOUND, CANDIDATE_MODULE_BOUND, CANDIDATE_COMMON_SIGNAL, CANDIDATE_CONTROL_SIGNAL, CANDIDATE_COLORED }; 
        enum sliceable_t { UNSLICEABLE=0, SLICEABLE=1, NEEDS_SLICE_CHECK=2 };
        static const std::string typeStrings[];
        typedef std::set<node_t*> matched_node_t;

    private:
        typedef std::map<node_t*, nodeset_t> node_inputmap_t;
        typedef std::vector< std::string > commentList_t;

        type_t moduleType; 
        nodelist_t inputs;
        node_groups_t node_groups;
        pin_map_t pin_map;
        wordlist_t word_inputs;

        nodelist_t outputs;
        wordlist_t word_outputs;

        nodeset_t internals;
        matched_node_t matched_nodes;
        std::set<std::string> libtypes;
        moduleset_t module_inputs;
        moduleset_t conflicting_modules;
        node_inputmap_t inputmap;
        id_module_t* dominator;
        bool is_bad;
        bool dump_annotation;
        std::string annotation_module_name;
        commentList_t comments;

        void compute_internals(node_t* n);
        void compute_internals(node_t* n, nodeset_t& internals);
        void compute_internals_private();
        void mark_bad();
        void compute_inputmap(node_t* n);
        void add_slice(bitslice_t* bt);
        void dump_inputmap(std::ostream& out, node_t* n);
        void dumpSliceIndexMap();

        void prune_inputs(const nodeset_t& good_inputs);
        word_t* prune_word(const nodeset_t& good_inputs, word_t* w);
        void compute_good_inputs(nodeset_t& good_inputs);

        static int moduleCounter;
        const char* type;
        int moduleNumber;
        char* name;
        bool overlapping;
        bool overlapping2;
        mutable int non_inferred_gates;
        stringmap_t rename_map;
        stringmap_t rev_rename_map;

        id_module_t* repr;
        int level;
        mutable /*sorry*/ int needs_grouping_memo;
        sliceable_t sliceable;
        int numSlices;

        typedef std::map<node_t*, int> slice_index_map_t;
        slice_index_map_t slice_index_map;
        std::vector<int> slice_sizes;
        int ilpStartVar;
        int ilpEndVar;
        bool added_siblings;

        // sorry.
        mutable int is_seq_memo;

    public:

        nodelist_t* get_inputs(){ return &inputs;};
        nodelist_t* get_outputs(){ return &outputs;};
        wordlist_t* get_word_outputs(){ return &word_outputs;};
        wordlist_t* get_word_inputs(){ return &word_inputs;};

        void add_all_inputs(modulelist_array_t& nodelist);

        template<class func_t> void apply_on_all_inputs(func_t& func) const {
            for(nodelist_t::const_iterator it = inputs.begin(); it != inputs.end(); it++) {
                node_t* n = *it;
                func(n);
            }
            for(wordlist_t::const_iterator it = word_inputs.begin(); it != word_inputs.end(); it++) {
                word_t* w = *it;
                for(unsigned i=0; i != w->size(); i++) {
                    node_t* n = w->get_bit(i);
                    func(n);
                }
            }
        }

        template<class func_t> void apply_on_all_outputs(func_t& func) const {
            for(nodelist_t::const_iterator it = outputs.begin(); it != outputs.end(); it++) {
                node_t* n = *it;
                func(n);
            }
            for(wordlist_t::const_iterator it = word_outputs.begin(); it != word_outputs.end(); it++) {
                word_t* w = *it;
                for(unsigned i=0; i != w->size(); i++) {
                    node_t* n = w->get_bit(i);
                    func(n);
                }
            }
        }

        template<class func_t> void apply_on_all_grouped_inputs(func_t& func) const {
            for(node_groups_t::const_iterator it = node_groups.begin(); it != node_groups.end(); it++)
            {
                const nodelist_t& nl = it->second;
                const std::string& group_name(it->first);
                for(nodelist_t::const_iterator it = nl.begin(); it != nl.end(); it++) {
                    node_t* n = *it;
                    func(group_name, n);
                }
            }
        }

        template<class func_t> void apply_on_all_input_groups(func_t& func) const {
            for(node_groups_t::const_iterator it = node_groups.begin(); it != node_groups.end(); it++)
            {
                const nodelist_t& nl = it->second;
                const std::string& group_name(it->first);
                func(group_name, nl);
            }
        }

        void compute_compatible_modules(modulelist_array_t& inps, moduleset_t& compats);
        bool can_structurally_extend();

        int moduleNum() const { return moduleNumber; }
        void setNumber(int num) { moduleNumber = num; }

        void get_input_modules(moduleset_t& mods);

        void add_sibling_gates();
        void set_bad() { is_bad = true; }

        void enable_annotation(std::string& mod_name) { 
            dump_annotation = true; 
            annotation_module_name = mod_name;
        }

        void add_matched_node(node_t*n, std::string& name ) { 
            matched_nodes.insert(n); 
            libtypes.insert(name);
        }
        void create_new_modules();

        void find_inputs(node_t* n, const nodeset_t& cone, nodeset_t& visited, nodeset_t& inputs);

        const nodeset_t& get_inputmap(node_t* n);
        void reset_inputmap(node_t* n) { 
            if(inputmap.find(n) != inputmap.end()) {
                inputmap[n].clear();
                inputmap.erase(n);
            }
        }
        void reset_inputmaps() {
            inputmap.clear();
        }

        void add_conflicting_module(id_module_t* m) {
            conflicting_modules.insert(m);
        }
        void clear_conflicting_modules() {
            conflicting_modules.clear();
        }

        const moduleset_t& get_conflicting_modules() const {
            return conflicting_modules;
        }

        id_module_t(const char* type, sliceable_t sl, type_t mType = INFERRED)
            : moduleType(mType),
              type(strdup(type)),
              moduleNumber(moduleCounter++),
              name(static_cast<char*>(malloc(16)))
        {
            overlapping = false;
            overlapping2 = false;
            dominator = this;
            sprintf(name, "mx%d", moduleNumber);
            is_bad = false;
            dump_annotation = false;
            non_inferred_gates = -1;

            repr = this;
            level = 0;
            needs_grouping_memo = -1;
            sliceable = (sl == SLICEABLE ? NEEDS_SLICE_CHECK : UNSLICEABLE);
            numSlices = 1;

            ilpStartVar = -1;
            ilpEndVar = -1;
            added_siblings = false;

            is_seq_memo = -1;
        }

        int getILPStartVar() const { return ilpStartVar; }
        int getILPEndVar() const { return ilpEndVar; }
        int getILPVar(node_t* n);
        int getSliceSize(int i) {
            if(sliceable == UNSLICEABLE) {
                assert(i == 0);
                return num_internals();
            } else {
                assert(sliceable == SLICEABLE);
                assert(i < (int)slice_sizes.size() && i >= 0);
                return slice_sizes[i];
            }
        }

        int setILPVars(int start) {
            ilpStartVar = start;
            int slices = getNumSlices();
            ilpEndVar = start + (slices == 1 ? 0 : slices);
            return ilpEndVar;
        }
        void setUnsliceable() {
            if(!(sliceable == UNSLICEABLE  || sliceable == NEEDS_SLICE_CHECK)) {
                std::cout << "sliceable: " << sliceable << std::endl;
                std::cout << "type: " << get_type() << std::endl;
                std::cout << *this << std::endl;
                std::cout << "num internals: " << num_internals() << std::endl;
            }

            assert(sliceable == UNSLICEABLE  || sliceable == NEEDS_SLICE_CHECK);
            sliceable = UNSLICEABLE;
        }

        // union-find: init.
        void uf_init() {
            level = 0;
            repr = this;
        }

        // union-find: link
        void uf_link(id_module_t* that) {
            if(this->level > that->level) {
                that->repr = this;
            } else {
                this->repr = that;
                if(this->level == that->level) {
                    that->level += 1;
                }
            }
        }

        // union-find: find-set
        id_module_t* uf_get_rep() {
            if(this->repr != this) {
                this->repr = this->repr->uf_get_rep();
            }
            return this->repr;
        }

        void pruneSlices(flat_module_t* flat, const std::vector<int>& good_slices);

        int getNumSlices();
        int computeNumSlices();

        static void link(id_module_t* a, id_module_t* b);
        id_module_t* get_repr();

        ~id_module_t();

        bool userDefined() const { return moduleType == USER_DEFINED; }
        bool inferred() const { return moduleType == INFERRED; }
        bool candidateModuleBound() const { return moduleType == CANDIDATE_MODULE_BOUND; }
        bool candidateWordBound() const { return moduleType == CANDIDATE_WORD_BOUND; }
        bool candidateCommonSignal() const { return moduleType == CANDIDATE_COMMON_SIGNAL; }
        bool candidateColored() const {return moduleType == CANDIDATE_COLORED; }
        bool isDominateable() const {
            return !candidate();
        }

        bool candidate() const { 
            return candidateModuleBound() || 
                   candidateWordBound() ||
                   candidateCommonSignal();
        }

        type_t get_moduleType() const { return moduleType; }

        const char* get_type() const { return type; }

        void add_input(node_t* n);
        void add_input(const std::string& group_name, node_t* n, bool allow_dup);
        void set_input_group(const std::string& group_name, node_t* n, bool allow_dup);
        bool is_input_in_group(const std::string& group, node_t* n) const;
        nodelist_t* get_inputs_in_group(const std::string& group) {
            node_groups_t::iterator pos = node_groups.find(group);
            if(pos != node_groups.end()) {
                return &(pos->second);
            } else {
                return NULL;
            }
        }

        void add_input_word(word_t* w);
        void add_input_module(id_module_t* m);
        void add_inputset(nodeset_t& cone) const;
        void add_output(node_t* n, fnInfo_t* inf=NULL);
        void add_output_word(word_t* w);
        void compute_internals(bool update=true);
        void compute_internals2();
        bool is_marked_bad() const { return is_bad; }
        void update_nodes();
        void clear_internals();
        void mark_overlapping() { overlapping = true; }
        void mark_overlapping2() { overlapping2 = true; }
        bool is_overlapping() const { return overlapping; }
        bool is_overlapping2() const { return overlapping2; }
        void add_comment(std::string& c); 

        const std::string& get_renaming(const std::string& s) const {
            stringmap_t::const_iterator pos = rename_map.find(s);
            if(pos != rename_map.end()) return pos->second;
            else return s;
        }
        const std::string& get_rev_renaming(const std::string& s) const {
            stringmap_t::const_iterator pos = rev_rename_map.find(s);
            if(pos != rev_rename_map.end()) return pos->second;
            else return s;
        }
        const std::string get_rev_renaming(const char* s) const {
            std::string cppstr(s);
            return get_rev_renaming(cppstr);
        }

        static bool compare(id_module_t* left, id_module_t* right);
        std::string get_input_port_name(int word, int bit);
        std::string get_output_port_name(int word, int bit);
        bool needs_grouping() const;
        void rename_inputs(stringset_t& names, stringset_t& new_names,
                           stringmap_t& map, stringmap_t& rev);
        void rename_outputs(stringset_t& names, stringset_t& new_names,
                            stringmap_t& map, stringmap_t& rev);

        void create_rename_map( stringset_t& names, stringmap_t& map, stringmap_t& rev );


        // compute pin names for the inputs and outputs.
        void computePinNames();
    private:
        void dump_mux21(std::ostream& out, const char* name, verilog_lib_t* vlib);
        void dump_mux41(std::ostream& out, const char* name, verilog_lib_t* vlib);
        void add_decls(decl_list_t& decls);
        BDD getProduct(int num, int vars, Cudd& mgr);

        // check the sliceability of this output bit.
        bool checkOutputBit(node_t* n, unsigned& lastSz);
        // update the slice index for the internals of each output node.
        void updateSliceIndex(node_t* n, int index);
        void updateSliceSizes();

        // functions to compute the pin names.
        void computePinNamesDecoders();
        void computePinNamesMuxes(int index);
        void computePinNamesDemux();
        void computeDecoderBDDs(int sz, Cudd& mgr);
        int computeDemuxedVar(int vars, std::vector<unsigned>& outs);
        BDD computeCube(int sz, int value, Cudd& mgr);
    public:

        unsigned num_inputs() const { return inputs.size(); }
        unsigned num_word_inputs() const { return word_inputs.size(); }
        unsigned num_outputs() const { return outputs.size(); }
        unsigned num_word_outputs() const { return word_outputs.size(); }
        unsigned num_internals() const { return internals.size(); }
        unsigned total_inputs() const;
        unsigned total_outputs() const;
        unsigned num_internal_gates() const;
        unsigned num_internal_nodes() const;

        void set_dominator(id_module_t* d) { dominator = d; }
        id_module_t* get_dominator() const { return dominator; }
        bool is_dominated() const { return dominator != this; }

        unsigned count_noninferred_gates() const;

        bool is_input(node_t* n) const;
        bool is_word_input(node_t* n) const { return get_word_input_index(n) != -1; };
        int get_word_input_index(node_t* n) const;
        bool is_output(node_t* n) const;

        friend std::ostream& operator<<(std::ostream& out, const id_module_t& mod);

        // the name of this module.
        const char* get_name() { return name; }
        // return the internal nodes.
        nodeset_t& get_internals() { return internals; }
        // is this node an internal node?
        bool is_internal(node_t* n) const { return internals.find(n) != internals.end(); }
        // mark nodes inside this module.
        void mark_internals(std::vector<bool>& marks) const;
        // mark inputs of this module.
        void mark_inputs(std::vector<bool>& marks) const;

        // does this module contain another.
        bool contains(const id_module_t* other) const;

        // dump a verilog module instantiation for this.
        void dump_verilog(std::ostream& out, std::ostream& lib, verilog_lib_t* vlib);
        // dump this word.
        void dump_verilog_word(std::ostream& out, word_t* word);
        // dump the gates inside this component.
        void dumpGates(std::ostream& out, const char* name);
        // dump some informational comments about this module.
        void dump_comments(std::ostream& out);

        struct bdd_compare_t {
            bool operator() (const BDD& one, const BDD& two) const {
                return one.getNode() < two.getNode();
            }
        };

        typedef std::map<node_t*, BDD> fn_map_t;
        typedef std::map<BDD, node_t*, bdd_compare_t> fn_rev_map_t;

        // create full functions.
        void createFullFunctions(Cudd& cudd, fn_map_t& map);

        // create full functions while keeping track of the reverse mappings from bdds to nodes.
        void createFullFunctions(
            Cudd& cudd, 
            fn_map_t& var_map, 
            fn_rev_map_t& rev_map, 
            std::vector<node_t*>& outputs,
            std::vector<BDD>& bdds
        );
        // mux analysis.
        void muxAnalysis();
        node_t* evalBDD(Cudd& mgr, fn_map_t& map, fn_rev_map_t& rev, nodelist_t& inps, int value, BDD& out);

        // create full functions for a particular word.
        BDD createFullFunction(Cudd& cudd, fn_map_t& map, node_t* n);

        void createBDDMap(fn_map_t& mapIn, bdd_node_map_t& mapOut);
        bool is_seq() const;
        // print the full functions.
        void printFullFunctions();
        // do a specific type of analysis that helps us identify RAM decode logic.
        void doRAMReadAnalysis();
        // create the BDDs for adders.
        void createAdderBDDs(Cudd& cudd, fn_map_t& map, fn_map_t& struct_map);
        // create the BDDs for the adders from the circuit as well as what we expect them to be and compare.
        void compareAdderBDDs();
        // dump all the gates in this module.
        void dumpGates(std::ostream& out);
        // dump structural verilog.
        void dump_structural_verilog(const char* type, std::ostream& vout, std::ostream& lout);

        // create a set containing all the variable names used in this module.
        void create_varname_set(stringset_t& names);

        friend class renamer_t;
        friend bool findPopCnts(flat_module_t* flat, id_module_t* mod, std::vector<node_t*>& outnodes);

        id_module_t* registerAnalysis(flat_module_t* flat);
        void get_ungrouped_inputs(nodeset_t& ungrouped) const;
        bool same_ungrouped_inputs(id_module_t* other) const;
    };

    struct renamer_t  {
        id_module_t* mod;
        stringmap_t& map; 
        stringmap_t& rev;
        renamer_t(id_module_t* m, stringmap_t& m1, stringmap_t& m2) 
            : mod(m), map(m1), rev(m2)
        {}

        void operator() (node_t* n);
    };

    bool varname_has_indices(const std::string& s);
    std::string rename_var(
        const std::string& old, 
        stringset_t& names1, 
        stringset_t& names2
    );
    // create a map with mappings from old to new names.
    std::ostream& operator<<(std::ostream& out, const id_module_t& mod);

    // 2 to 4 decoders.
    typedef std::pair<node_t*, node_t*> nodepair_t;
    inline nodepair_t createNodePair(node_t* a, node_t* b)
    {
        if(a < b) return nodepair_t(a,b);
        else return nodepair_t(b,a);
    }

    typedef struct decoder24Info_t {
        bitslice_t* slice;
        int tag;
    } demux24Info_t;

    typedef std::vector<decoder24Info_t> decoder24list_t;
    typedef std::map<nodepair_t, decoder24list_t> decoder24map_t; 

    void add24Decoders(fnInfo_t* info, int tag, decoder24map_t& decs);
    void identify24Decoders(flat_module_t* module);

    typedef std::vector<demux24Info_t> demux24list_t;
    typedef std::map<nodetriple_t, demux24list_t> demux24map_t;

    void add24Demuxes(fnInfo_t* info, int tag, demux24map_t& decs);
    void identify24Demuxes(flat_module_t* module);

    // 3:8 decoders.
    typedef struct decoder38Info_t {
        bitslice_t* slice;
        int tag;
    } demux38Info_t;

    typedef std::vector<decoder38Info_t> decoder38list_t;
    typedef std::map<nodetriple_t, decoder38list_t> decoder38map_t; 

    void add38Decoders(fnInfo_t* info, int tag, decoder38map_t& decs);
    void identify38Decoders(flat_module_t* module);

    // 3:8 demuxes.
    typedef std::vector<demux38Info_t> demux38list_t;
    typedef std::map<nodequad_t, demux38list_t> demux38map_t; 

    void add38Demuxes(fnInfo_t* info, int tag, demux38map_t& decs);
    void identify38Demuxes(flat_module_t* module);

    // 4:16 decoders.
    typedef struct decoder416Info_t {
        bitslice_t* slice;
        int tag;
    } demux416Info_t;

    typedef std::vector<decoder416Info_t> decoder416list_t;
    typedef std::map<nodequad_t, decoder416list_t> decoder416map_t; 

    void add416Decoders(fnInfo_t* info, int tag, decoder416map_t& decs);
    void identify416Decoders(flat_module_t* module);

    // 4:16 demuxes.
    typedef std::vector<demux416Info_t> demux416list_t;
    typedef std::map<node5tup_t, demux416list_t> demux416map_t;

    void add416Demuxes(fnInfo_t* info, int tag, demux416map_t& decs);
    void identify416Demuxes(flat_module_t* module);

    // 5:32 decoders
    typedef struct decoder532Info_t {
	bitslice_t* slice;
	int tag;
    } demux532Info_t;
	
    typedef std::vector<decoder532Info_t> decoder532list_t;
    typedef std::map<node5tup_t, decoder532list_t> decoder532map_t;

    void add532Decoders(fnInfo_t* info, int tag, decoder532map_t& decs);
    void identify532Decoders(flat_module_t* module);

    // 6:64 decoders.
    typedef struct decoder664Info_t {
	bitslice_t* slice;
	int tag;
    } demux664Info_t;
    
    typedef std::vector<decoder664Info_t> decoder664list_t;
    typedef std::map<node_tuple_t, decoder664list_t> decoder664map_t;

    void add664Decoders(fnInfo_t* info, int tag, decoder664map_t& decs);
    void identify664Decoders(flat_module_t* module);
	
    // comparators.
    void identifyEqualityComparators(flat_module_t* module);
    void identifyEqualityComparators2(flat_module_t* module);

    void shortestPath(flat_module_t* module);

    class id_module_output_inserter_t {
        id_module_t* mod;
    public:
        id_module_output_inserter_t(id_module_t* m) : mod(m) {}
        void operator() (node_t* n) { mod->add_output(n); }
    };

    class id_module_input_inserter_t {
        id_module_t* mod;
    public:
        id_module_input_inserter_t(id_module_t* m) : mod(m) {}
        void operator() (node_t* n) { mod->add_input(n); }
    };

#define MAX_DECODER_SIZE 7 
    extern std::vector<BDD> decFns[MAX_DECODER_SIZE], decnegFns[MAX_DECODER_SIZE];
    void cleanupAfterPinNameComputation(); 
    unsigned removeBit(unsigned bit, unsigned numBits, unsigned val);

    struct mux_selinfo_t {
        const char* type;
        int signals[8];
    };

    typedef std::map< node_t*, std::vector<node_t*> > popcnt_groups_t;

    void computePopCountBDDs(Cudd& mgr, int n, std::vector<BDD>& bits);
    void findPopCnts(flat_module_t* mod);
    bool findPopCnts(flat_module_t* flat, id_module_t* mod, std::vector<node_t*>& outnodes);

    extern std::map<int, std::vector<BDD> > popcnt_bdd_cache;
    std::vector<BDD>& get_popcnt_bdds(flat_module_t* flat, int n);
    void clear_popcnt_bdd_cache();

    id_module_t* merge_modules(std::vector<id_module_t*>& mods);
}

#endif // _AGGR_H_DEFINED_
