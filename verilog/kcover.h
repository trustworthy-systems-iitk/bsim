#ifndef __KCOVER_H_DEFINED__
#define __KCOVER_H_DEFINED__

#include <set>
#include <vector>
#include <iostream>
#include <assert.h>
#include "library.h"

class kcover_t;
class node_t;
struct input_provider_t;

typedef std::vector<kcover_t*> kcoverlist_t;

class kcover_t {
public:
    typedef std::vector<node_t*> leafset_t;
    typedef std::set<node_t*> nodeset_t;
protected:
    leafset_t leaves;
    nodeset_t internals;
    kcoverlist_t parents;
    node_t* root;
    int significantInputs;

    BDD getFn(input_provider_t* ipp, node_t* n);
    BDD getCktFn(input_provider_t* ipp, node_t* n);
    std::vector<uint8_t> *perm;
    void* canonical;

    void add_node(node_t* n);
public:
    kcover_t(node_t* rt) { root = rt; significantInputs = -1; }
    kcover_t(const kcover_t& o) 
        : leaves(o.leaves), 
          internals(o.internals),
          parents(o.parents),
          root(o.root)
    {
        significantInputs = -1;
    }

    ~kcover_t() {}

    node_t* get_root() { return root; }
    void add_leaf(node_t* n);


    kcover_t* simple_merge(kcover_t* nkc, node_t *root);
    kcover_t* simple_merge_internal(kcover_t* nkc, node_t *root);
    bool is_node_present(node_t* n) const;
    bool is_node_internal(node_t* n) const;
    int get_node_index(node_t* n);
    bool is_leaf(node_t* n) const { return std::find(leaves.begin(), leaves.end(), n) != leaves.end(); }

    nodeset_t& get_internals();
    void compute_internals(node_t* root);
    void prune_leaves();

    bool prune(input_provider_t* ipp);
    unsigned size() const { return leaves.size(); }
    unsigned numInputs() const {
        if(significantInputs == -1) return size();
        else return significantInputs;
    }

    node_t* at(int i) { return leaves[i]; }

    friend std::ostream& operator<<(std::ostream& out, kcover_t& kcov);

    BDD getFn(input_provider_t* ipp);
    BDD getCktFn(input_provider_t* ipp);
    int pruneLeaves(BDD& fn, input_provider_t* e);

    friend class input_provider_wrapper_t;
    friend class ckt_inp_provider_t;

    void setPerm(std::vector<uint8_t>& p) { perm = &p; }
    std::vector<uint8_t>& getPerm() const { assert(perm); return *perm; }
    int getPi(int i) { assert(i < (int) perm->size()); return (*perm)[i]; }
    bool isPerm(std::vector<uint8_t>& p) {
        if(p.size() != perm->size()) return false;
        else {
            for(unsigned i=0; i != p.size(); i++) {
                if(p[i] != (*perm)[i]) return false;
            }
            return true;
        }
    }
    void setCanonical(void* canon) { canonical = canon; }
    void* getCanonical() { return canonical; }

    int get_min_depth();
    int get_max_depth();

    bool operator==(const kcover_t& other) const;
};

std::ostream& operator<<(std::ostream& out, kcover_t& kcov);


struct input_provider_wrapper_t : public input_provider_t 
{
    node_t* node;
    kcover_t* kcover;
    input_provider_t* ipp;

    input_provider_wrapper_t(node_t* node, kcover_t* kcover, input_provider_t* ipp)
        : node(node),
          kcover(kcover),
          ipp(ipp)
    {
    }

    virtual BDD inp(int index);
    virtual BDD one();
    virtual BDD zero();
};

struct ckt_inp_provider_t : public input_provider_wrapper_t
{
    ckt_inp_provider_t(node_t* n, kcover_t* kc, input_provider_t* e)
        : input_provider_wrapper_t(n, kc, e)
    {
    }
    virtual BDD inp(int index);
};

#endif
