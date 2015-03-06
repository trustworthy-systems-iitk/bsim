#ifndef __LIBRARY_H_DEFINED__
#define __LIBRARY_H_DEFINED__

#include <set>
#include <map>
#include "ast.h"
#include "common.h"
#include "simplify.h"
#include "cuddObj.hh"

struct input_provider_t {
    virtual BDD inp(int index) = 0;
    virtual BDD one() = 0;
    virtual BDD zero() = 0;

    virtual ~input_provider_t() {}
};

struct dummy_input_provider_t : public input_provider_t {
    Cudd cudd;
    BDD inp(int index) { return cudd.bddVar(index); }
    BDD one() { return cudd.bddOne(); }
    BDD zero() { return cudd.bddZero(); }
};


typedef BDD (*eval_fun_t)(input_provider_t*);
typedef std::set<int> intset_t;

class node_t;
class flat_module_t;
class lib_elem_t;

struct ILibrary 
{
    // types.
    typedef std::map<const char*, lib_elem_t*, string_cmp_t> map_t;
    // map between library element names and lib_elem_t pointers.
    map_t blocks;

    // initialize the library.
    virtual void init() = 0;
    // munge the modules before they are parsed.
    virtual void preprocess(module_list_t& modules) = 0;
    // get the library element representing this module type.
    lib_elem_t* get_elem(const char* str);
    // rewrite this node if necessary. default implementation does nothing.
    virtual void rewrite_node(node_t* n, flat_module_t* mod) {}
    // get the "replacement" flop for this seq element (i.e., return one of 
    // BSIM_DFF, BSIM_DFFN, BSIM_LAT or BSIM_LATN.
    virtual lib_elem_t* get_replacement(lib_elem_t* ff);
    // destroy.
    virtual ~ILibrary() {}
};

class lib_elem_t {
public:
    // Gates and latches should be self-explanatory. A block is something
    // which has a bunch of ports and we don't really know which of these
    // are inputs or outputs.
    enum type_t { GATE, LATCH, MACRO };
protected:
    // types.
    struct cover_t
    {
        bool constant;
        int const_value;
        simplify_n::cube_list_t onset;
        simplify_n::cube_list_t offset;
    };

    // typedefs.
    typedef std::map<const char*, int, string_case_cmp_t> string_map_t;
    typedef std::set<int> index_set_t;
    typedef std::map<eval_fun_t, intset_t*> symm_inputs_memo_t;
    typedef std::map<eval_fun_t, bool> is_buffer_flag_t;
    typedef std::map<eval_fun_t, bool> is_inverter_flag_t;
    typedef std::map<eval_fun_t, bool> is_nor2xb_flag_t;
    typedef std::map<eval_fun_t, cover_t> cover_memo_t;

    static symm_inputs_memo_t symm_inputs_memo;
    static cover_memo_t cover_memo;
    static is_buffer_flag_t is_buffer_flag;
    static is_inverter_flag_t is_inverter_flag;
    static is_nor2xb_flag_t is_nor2xb_flag;

    // variables
    const char*  name;
    eval_fun_t   eval_fn;
    string_map_t inputs;
    string_map_t outputs;
    string_map_t ports;
    index_set_t  inputIdx;
    index_set_t  outputIdx;
    index_set_t  portIdx;
    type_t       type;
    intset_t     symm_inputs;
    int d_index;
    int si_index;
    int se_index;

    // functions.
    void register_input  (const char* i, int index);
    void register_output (const char* o, int index);
    void register_port   (const char* p, int index);

    lib_elem_t(const char* name, eval_fun_t e, type_t type);
    void compute_input_symmetry();
    void compute_covers();
    void is_buffer_compute();
    void is_inverter_compute();
    void is_nor2xb_compute();
    const cover_t& get_cover() const;
    void verify_satisfy_count();
public:
    static lib_elem_t* create_block_ex(const char* name, int n_inputs, int n_outputs, int n_ports, ...);
    static lib_elem_t* create_lib_elem(const char* name, bool seq, int n_inputs, int n_outputs, eval_fun_t e, ...);
    static void create_seq_elem(ILibrary::map_t& blocks, const char* name, bool seq, int n_inputs, int n_outputs, eval_fun_t e, ...);
    virtual ~lib_elem_t();

    const char* get_name() const { return name; }
    bool is_input(const char* i);
    bool is_output(const char* o);
    bool is_port(const char* p);

    int  get_input_index(const char* i);
    int  get_output_index(const char* o);
    int  get_port_index(const char* p);

    const char* get_input_name(int i);
    const char* get_output_name(int i);
    const char* get_port_name(int i);
    bool has_input(const char* n) const;
    bool has_output(const char* n) const;

    bool is_seq() const { return type == LATCH; }
    bool is_gate() const { return type == GATE; }
    bool is_macro() const { return type == MACRO; }
    type_t get_type() const { return type; }

    void set_dindex(int i) { d_index = i; }
    int get_dindex() const { return d_index; }
    void set_si_index(int i) { si_index = i; }
    int get_si_index() const { return si_index; }
    void set_se_index(int i) { se_index = i; }
    int get_se_index() const { return se_index; }

    // CNF functions
    bool is_constant() const { return get_cover().constant; }
    int get_const_value() const { return get_cover().const_value; }
    const simplify_n::cube_list_t& get_onset() const { return get_cover().onset; }
    const simplify_n::cube_list_t& get_offset() const { return get_cover().offset; }

    unsigned num_inputs() const { return inputs.size(); }
    unsigned num_outputs() const { return outputs.size(); }
    unsigned num_ports() const { return ports.size(); }
    unsigned num_connections() const { return inputs.size() + outputs.size() + ports.size(); }

    eval_fun_t get_eval_fun() const { return eval_fn; }
    BDD getFn(input_provider_t* ipp) { return eval_fn(ipp); }
    bool is_symmetric_input(int i) const { return symm_inputs.find(i) != symm_inputs.end(); }
    bool is_buffer() const { 
        assert(is_buffer_flag.find(eval_fn) != is_buffer_flag.end());
        return is_buffer_flag[eval_fn];
    }
    bool is_inverter() const {
        assert(is_inverter_flag.find(eval_fn) != is_inverter_flag.end());
        return is_inverter_flag[eval_fn];
    }
    bool is_nor2xb() const {
        assert(is_nor2xb_flag.find(eval_fn) != is_nor2xb_flag.end());
        return is_nor2xb_flag[eval_fn];
    }

    void getMissingInputs(module_inst_t* mi, std::vector<std::string>& missing_inputs);
    void getMissingOutputs(module_inst_t* mi, std::vector<std::string>& missing_outputs);
    void getMissingPorts(module_inst_t* mi, std::vector<std::string>& missing_ports);
};

ILibrary* createLib(std::string& name);

#endif // __LIBRARY_H_DEFINED__
