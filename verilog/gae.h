#ifndef _GAE_H_DEFINED_
#define _GAE_H_DEFINED_

#include "node.h"
#include "aggr.h"
#include "utility.h"
#include <map>
#include <algorithm>

namespace aggr {
    //// predicate typedefs. /////
    typedef bool (*predicate1_t) (flat_module_t* flat, node_t* a, int pa);
    typedef bool (*predicate2_t) (flat_module_t* flat, node_t* a, int pa, node_t* b, int pb);
    typedef bool (*predicate3_t) (flat_module_t* flat, node_t* a, int pa, node_t* b, int pb, node_t* c, int pc);
    typedef bool (*predicate4_t) (flat_module_t* flat, node_t* a, int pa, node_t* b, int pb, node_t* c, int pc, node_t* d, int pd);
     
    ///// comparator operator for vectors of nodes /////
    struct nodelist_lt_t
    {
        bool operator() (const ::nodelist_t& a, const ::nodelist_t& b);
    };

    //// abstract class to do something with a list of bitslices. ////
    struct bitslice_list_user_t
    {
        virtual void use(const ::nodelist_t& nodes, bitslice_list_t& list) = 0;
    };

    //// create a module from a list of bitslices. ////
    struct bitslice_module_creator_t : public bitslice_list_user_t
    {
    protected:
        flat_module_t* flat;
        const char* name;
        unsigned minSize;
        std::string selsig_name;
    public:
        bool dump_sizes;
        bitslice_module_creator_t(flat_module_t* module, const char* name, const char* selsig_name, unsigned minSize);
        virtual ~bitslice_module_creator_t();

        // Next two functions are a trial run
        virtual void traceTree(node_t& node, ::nodelist_t& inputs);
        virtual void controlSignalCreation(const ::nodelist_t& nodes);
        virtual void use(const ::nodelist_t& nodes, bitslice_list_t& list);
    };

    struct mux_creator_t : public bitslice_module_creator_t
    {
        mux_creator_t(flat_module_t* module, const char* name, const char* selsig, unsigned minSize) 
            : bitslice_module_creator_t(module, name, selsig, minSize)
        {
        }
        virtual ~mux_creator_t() {}
        virtual void use(const ::nodelist_t& nodes, bitslice_list_t& list);
    };

    struct bitslice_list_printer_t : public bitslice_list_user_t
    {
        virtual void use(const ::nodelist_t& nodes, bitslice_list_t& list);
    };

    //// abstract class for enumerating common signals ////
    struct common_signal_enumerator_t
    {
        virtual void init(bitslice_t* slice) = 0;
        virtual bool more() = 0;
        virtual void next(::nodelist_t& signals) = 0;
    };

    // take this function and group it by common signals.
    void groupByCommonSignal(
        flat_module_t* flat, 
        eval_fun_t func, 
        int num_inputs, 
        common_signal_enumerator_t* enumerator,
        bitslice_list_user_t* user
    );

    void groupByCommonSignalBDD(
        flat_module_t* flat,
        const BDD& bdd,
        common_signal_enumerator_t* enumerator,
        bitslice_list_user_t* user
    );

    //// RAM analysis user ////
    struct ram_analysis_user_t : public bitslice_list_user_t
    {
        enum { UNMARKED=0, OUTPUT_MARK=1, INTERNAL_MARK=2 };
        std::vector<int> markings;
        bool found_something;

        ram_analysis_user_t(unsigned marking_size) 
            : markings(marking_size, UNMARKED)
        {
            found_something = false;
        }

        virtual void use(const ::nodelist_t& nodes, bitslice_list_t& list);
        static void get_common_signals(const bitslice_list_t& list, ::nodelist_t& signals);
        static bool is_common(const node_t* s, const bitslice_list_t& list);
    };

    //// create an unknown module if the candidate looks "interesting". ////
    class unknown_module_creator_t : public bitslice_list_user_t 
    {
        flat_module_t* flat;
        char* name;
        unsigned minSize;
        id_module_t::type_t type;
    public:
        bool create_partially_covered_modules;

        unknown_module_creator_t(flat_module_t* flat,
                                 const char* name,
                                 unsigned minSize,
                                 id_module_t::type_t typ);
        ~unknown_module_creator_t();
        virtual void use(const ::nodelist_t& nodes, bitslice_list_t& list);
    };

    //// enumerate exactly one common signal ////
    class simple_common_signal_enumerator_t : public common_signal_enumerator_t
    {
        flat_module_t* flat;
        predicate1_t pred;
        bitslice_t* slice;
        int pos;
    public:
        simple_common_signal_enumerator_t(flat_module_t* flat, predicate1_t pred);

        virtual void init(bitslice_t* slice);
        virtual bool more();
        virtual void next(::nodelist_t& signals);
    };

    //// enumerate exactly 2 common signals ////
    struct common_signals_2_t : public common_signal_enumerator_t
    {
        common_signals_2_t(flat_module_t* flat, predicate2_t pred);
        ~common_signals_2_t();

        virtual void init(bitslice_t* slice);
        virtual bool more();
        virtual void next(::nodelist_t& signals);
    private:
        bitslice_t* slice;
        flat_module_t* flat;
        predicate2_t pred;
        int pos;

        std::vector<nodepair_t> pairs;
    };

    //// enumerate exactly 3 common signals ////
    struct common_signals_3_t : public common_signal_enumerator_t
    {
        common_signals_3_t(flat_module_t* flat, predicate3_t pred);
        ~common_signals_3_t();

        virtual void init(bitslice_t* slice);
        virtual bool more();
        virtual void next(::nodelist_t& signals);
    private:
        bitslice_t* slice;
        flat_module_t* flat;
        predicate3_t pred;
        int pos;

        std::vector<nodetriple_t> triples;
    };

    //// enumerate exactly 4 common signals ////
    struct common_signals_4_t : public common_signal_enumerator_t
    {
        common_signals_4_t(flat_module_t* flat, predicate4_t pred);
        ~common_signals_4_t();

        virtual void init(bitslice_t* slice);
        virtual bool more();
        virtual void next(::nodelist_t& signals);
    private:
        bitslice_t* slice;
        flat_module_t* flat;
        predicate4_t pred;
        int pos;

        std::vector<nodequad_t> quads;
    };

    //// predicates that return true ////
    bool pred1_true(flat_module_t* flat, node_t* a, int pa);
    bool pred2_true(flat_module_t* flat, node_t* a, int pa, node_t* b, int pb);
    bool pred3_true(flat_module_t* flat, node_t* a, int pa, node_t* b, int pb, node_t* c, int pc);
    bool pred4_true(flat_module_t* flat, node_t* a, int pa, node_t* b, int pb, node_t* c, int pc, node_t* d, int pd);
    // more aggregation functions
    void identify21Muxes2(flat_module_t* flat);
    void identify31Muxes2(flat_module_t* flat);   
    void identifyMuxes3(flat_module_t* flat);
    void identifyGatingFuncs2(flat_module_t* flat);
 
    void identifyALUXOR2(flat_module_t* flat);
    void identifyALUXNOR2(flat_module_t* flat);

    namespace prop_n
    {
        struct bitslice_i 
        {
            virtual unsigned num_inputs() = 0;
            virtual node_t* get_input(unsigned i) = 0;
            virtual node_t* get_output() = 0;
            virtual ~bitslice_i() {}
        };

        typedef std::vector<bitslice_i*> bitslice_list_t;

        struct bitslice_list_user_i
        {
            virtual void use(bitslice_list_t& l) {};
            virtual void resetTree(bitslice_i* r) {}
            virtual void useTreeNode(bitslice_i* bs) {}
        };

        void findPropagationChains(bitslice_list_t& l, bitslice_list_user_i& user);

        typedef std::set<bitslice_i*> bitslice_set_t;
        typedef std::map<node_t*, bitslice_set_t> bitslice_map_t;
        typedef std::map<bitslice_i*, bitslice_set_t> inputs_map_t;

        void do_dfs(
                bitslice_i* pbs, 
                inputs_map_t& edges, 
                bitslice_list_t& l, 
                bitslice_set_t& visited,
                bitslice_list_user_i& user
        );


        struct generic_slice_t : public bitslice_i
        {
            bitslice_t bitslice;
            virtual unsigned num_inputs() { return bitslice.xs.size(); }
            virtual node_t* get_input(unsigned i) { return bitslice.xs[i]; }
            virtual node_t* get_output() { return bitslice.ys[0]; }

            generic_slice_t(bitslice_t& bs) : bitslice(bs) {}
            virtual ~generic_slice_t() {}
        };

        struct module_creator_t : public bitslice_list_user_i
        {
            std::string name;
            flat_module_t* module;
            node_t* root;
            nodeset_t internals;
            int internalMinSize;

            module_creator_t(std::string& n, flat_module_t* m, int iminSize) 
                : name(n), module(m), root(NULL), internalMinSize(iminSize) 
            {
            }
            void do_dfs(node_t* n, nodeset_t& inputs);

            virtual void resetTree(bitslice_i* r);
            virtual void useTreeNode(bitslice_i* bs);
        };

        typedef std::pair<eval_fun_t, unsigned> fn_t;
        typedef std::vector<fn_t> fnlist_t;

        void findChains(std::string& name, fnlist_t& list, flat_module_t* module, int iminSize);
        void findXorChains(flat_module_t* module);
        void findAndChains(flat_module_t* module);
        void findOrChains(flat_module_t* module);
        void findCgenTrees(flat_module_t* module);

        void destroy_list(bitslice_list_t& l);
    }

    //TEST!
    void identifyPossibleMultiplies(flat_module_t* flat);
}
#endif
