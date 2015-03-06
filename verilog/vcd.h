#ifndef _VCD_H_DEFINED_
#define _VCD_H_DEFINED_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "node.h"
#include "flat_module.h"

namespace vcd_n
{
    struct vcd_signal_t;
        class vcd_file_t;

    struct bit_t
    {
        typedef std::map<unsigned, int> times_t;
        times_t values;

        vcd_signal_t* signal;
        int index;

        int get_value(unsigned time);
        void add_value(unsigned time, int value);
    };

    typedef std::vector<bit_t> bitvector_t;
    typedef std::vector<bit_t*> bitptr_list_t;
    typedef std::vector<int> intvec_t;

    struct wordGraph_t
    {
        const node_t * root;
        int bitno;
        int toggleno;
        int pushedsubno;

        std::vector<wordGraph_t *> subnodes;
        wordGraph_t(node_t * n, int bno, vcd_file_t * vcdf,intvec_t& cnts,intvec_t& flgs,std::map<node_t*, int> & ref_nodebitnomap);
    };

    std::ostream& dump(std::ostream& out, bitvector_t& bits);
    std::ostream& operator<<(std::ostream& out, intvec_t& iv);

    struct vcd_signal_t
    {
        const std::string name;
        const std::string symbol;

        const int max_index;
        const int min_index;

        bitvector_t bits;

        vcd_signal_t(std::string& n, std::string& s, int maxi, int mini);
        unsigned size() const { return max_index-min_index + 1; }
        void record_change(unsigned time, std::string& valstr);
    };

    std::ostream& operator<<(std::ostream& out, const vcd_signal_t& sig);

    typedef std::vector<vcd_signal_t*> vcd_signal_list_t;

    int get_min_index(std::string& str);
        int get_vector_index(std::string& str);
        bool is_new_vcd(std::string& str);
    int get_max_index(std::string& str);
    bool is_time(const std::string& s);
    int get_time(const std::string& s);

    bool is_value(char c);
    bool is_base_letter(char c);
    int get_value(char c);

    class vcd_file_t
    {
    public:
        typedef std::map<std::string, vcd_signal_list_t> signal_map_t;
        typedef std::pair<int, int> intpair_t;
        typedef std::map<intpair_t, int> wtmap_t;
        //typedef std::map <int, node_t*> children_t;
        typedef std::vector<nodelist_t> paths;
        bitptr_list_t bits;
        unsigned initial_time;
        unsigned final_time;
        unsigned step;

        // fill up this vector with signal values a particular time.
        void read_vector(intvec_t& vec, unsigned time);

    private:
        std::string filename;
        vcd_signal_list_t signals;
        signal_map_t signal_map;
                intvec_t togglecnts;

        // dump all the signals we've found.
        void dump_signals(std::ostream& out);

        // process a variable declaration.
        void process_vars(std::vector<std::string>& vars, std::string& scope);
        // process a scope declaration.
        void process_scope(std::vector<std::string>& vars, std::vector<std::string>& scopes);

        // return a string representing the current scope.
        static std::string get_scope(std::vector<std::string>& scopes);

        // update all the signals which share this symbol.
        static void update_all(vcd_signal_list_t& l, unsigned t, std::string& v);

        // update counter weights.
        void update_wtmap_counter(
            intvec_t& v1,
            intvec_t& v2,
            wtmap_t& wtmap,
            intvec_t& cnts
        );

        //get the children of a node
        int get_children(int i, node_t* ptrnode_i, paths& pths, intvec_t& cnts, std::map<int,node_t*> & ref_bitnonodemap, flat_module_t* fm);

        //create paths
        void get_paths (intvec_t& ref_cnts, paths& ref_pths, std::map<int,node_t*> & ref_bitnonodemap, flat_module_t* fm);

        //get node name
        char* get_node_name(bit_t* b);

        //get node by bit
        node_t* get_node_by_bit(bit_t* b, flat_module_t* fm);
        //check node connectivity in a given path
        bool check_connectivity(nodelist_t& nl);

    public:
        // constructor.
        vcd_file_t(std::string& filename);
        // destructor.
        ~vcd_file_t();

        // analyze for counters.
        void counter_analysis();
        void counter_analysis2(flat_module_t* fm);
        void traceWordsinVCD(flat_module_t* fm, nodelist_t& inputs);
        //given a node name return the corresponding index in 'bits' variable
        int get_bitno(node_t * n);
        void prepare_NCinfo(std::map<node_t*, int> & ref_nodebitnomap,intvec_t& v1, flat_module_t* fm);
				void get_nodebitnomap(std::map<node_t*, int> & ref_nodebitnomap,flat_module_t* fm);
				void get_bitnonodemap(std::map<int, node_t*> & ref_bitnonodemap,flat_module_t* fm);
        //update toogle numbers
        void update_toogle_counter(
            intvec_t& v1,
            intvec_t& v2,
            intvec_t& cnts
        );

        void update_toogle_counter_wout_x(
            intvec_t& v1,
            intvec_t& v2,
            intvec_t& cnts
        );
        void get_transitions(
                intvec_t& stpcnts,
                intvec_t& trcnts,
                intvec_t& keys,
                intvec_t& freeze,
                int lmttr,
                int thrshld,
                intvec_t& v1,
                intvec_t& v2,
                const int primes [98]
        );
    };

}

#endif // _VCD_H_DEFINED

