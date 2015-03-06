#include <vector>
#include <iostream>
#include <algorithm>
#include "main.h"
#include "node.h"
#include "kcover.h"

void kcover_t::add_node(node_t* n)
{
    internals.insert(n);
}

void kcover_t::add_leaf(node_t* n)
{
    if(std::find(leaves.begin(), leaves.end(), n) == leaves.end()) {
        leaves.push_back(n);
    }
}

kcover_t* kcover_t::simple_merge(kcover_t* other, node_t* root)
{
    int common_count = 0;
    for(leafset_t::iterator it  = leaves.begin();
                            it != leaves.end();
                            it++)
    {
        node_t *m = *it;
        if(other->is_node_present(m)) {
            common_count += 1;
        }
    }

    unsigned total_size = leaves.size() + other->leaves.size() - common_count;
    if(total_size > options.kcoverTh) { return NULL; }

    kcover_t* new_cover = new kcover_t(*this);
    for(leafset_t::iterator it = other->leaves.begin(); it != other->leaves.end(); it++) {
        node_t* n = *it;
        new_cover->add_leaf(n);
    }
    /* keep track of "parent" k-covers. */
    if(std::find(
            new_cover->parents.begin(), 
            new_cover->parents.end(), 
            other) == new_cover->parents.end())
    {
        new_cover->parents.push_back(other);
    }

    return new_cover;
}

kcover_t* kcover_t::simple_merge_internal(kcover_t* other, node_t* root)
{
    int common_count = 0;
    for(leafset_t::iterator it  = leaves.begin();
                            it != leaves.end();
                            it++)
    {
        node_t *m = *it;
        if(other->is_node_present(m) || other->is_node_internal(m))
            common_count += 1;
    }

    for(leafset_t::iterator it  = other->leaves.begin();
                            it != other->leaves.end();
                            it++)
    {
        node_t* m = *it;
        if(is_node_internal(m)) {
            assert(!is_node_present(m));
            common_count += 1;
        }
    }

    unsigned total_size = leaves.size() + other->leaves.size() - common_count;
    if(total_size > options.kcoverTh) { return NULL; }

    kcover_t* new_cover = new kcover_t(*this);
    for(nodeset_t::iterator it =  other->internals.begin();
                            it != other->internals.end();
                            it++)
    {
        node_t* n = *it;
        new_cover->internals.insert(n);
    }
    new_cover->internals.insert(root);

    leafset_t new_leaves;
    for(leafset_t::iterator it = leaves.begin(); it != leaves.end(); it++) {
        node_t* n = *it;
        if(!new_cover->is_node_internal(n) && 
           std::find(new_leaves.begin(), new_leaves.end(), n) == new_leaves.end())
        {
            new_leaves.push_back(n);
        }
    }
    for(leafset_t::iterator it = other->leaves.begin(); it != other->leaves.end(); it++) {
        node_t* n = *it;
        if(!new_cover->is_node_internal(n) && 
           std::find(new_leaves.begin(), new_leaves.end(), n) == new_leaves.end())
        {
            new_leaves.push_back(n);
        }
    }

    new_cover->leaves.resize(new_leaves.size());
    std::copy(new_leaves.begin(), new_leaves.end(), new_cover->leaves.begin());

    return new_cover;
}

bool kcover_t::is_node_present(node_t* n) const
{
    for(leafset_t::const_iterator it=leaves.begin(); it!=leaves.end(); it++) {
        if(*it == n) return true;
    }
    return false;
}

bool kcover_t::is_node_internal(node_t* n) const
{
    return (internals.find(n) != internals.end());
}

int kcover_t::get_node_index(node_t* n) 
{
    int idx = 0;
    for(leafset_t::iterator it=leaves.begin(); it!=leaves.end(); it++, idx++) {
        if(*it == n) { return idx; }
    }
    return -1;
}

int kcover_t::get_max_depth()
{
    int rt_level = root->get_level();
    int min_level = root->get_level();
    for(leafset_t::iterator it = leaves.begin(); it != leaves.end(); it++) {
        node_t *n = *it;
        if(n->get_level() < min_level) min_level = n->get_level();
    }

    return rt_level - min_level;
}

int kcover_t::get_min_depth()
{
    int rt_level = root->get_level();
    int max_level = 0;
    for(leafset_t::iterator it = leaves.begin(); it != leaves.end(); it++) {
        node_t* n = *it;
        if(n->get_level() > max_level) max_level = n->get_level();
    }
    return rt_level - max_level;
}

std::ostream& operator<<(std::ostream& out, kcover_t& kcov)
{
    out << kcov.root->get_name() << " : ";
    for(kcover_t::leafset_t::iterator it = kcov.leaves.begin(); it != kcov.leaves.end(); it++) {
        out << (*it)->get_name() << " ";
    }
    out << " { ";
    for(kcover_t::nodeset_t::iterator it = kcov.internals.begin(); it != kcov.internals.end(); it++) {
        out << (*it)->get_name() << " ";
    }
    out << " } ";
    return out;
}

BDD kcover_t::getFn(input_provider_t* ipp)
{
    return getFn(ipp, root);
}

BDD kcover_t::getCktFn(input_provider_t* ipp)
{
    return getCktFn(ipp, root);
}

BDD kcover_t::getFn(input_provider_t* ipp, node_t* node)
{
    int index;

    if((index = get_node_index(node)) != -1) {
        return ipp->inp(index);
    } else {
        assert(node->get_type() == node_t::GATE);
        input_provider_wrapper_t iwrap (node, this, ipp);
        BDD result =  node->get_lib_elem()->getFn(&iwrap);
        return result;
    }
}

BDD kcover_t::getCktFn(input_provider_t* ipp, node_t* node)
{
    int kcover_index;
    if((kcover_index = get_node_index(node)) != -1) {
        int node_index = at(kcover_index)->get_index();
        return ipp->inp(node_index);
    } else {
        assert(node->get_type() == node_t::GATE);
        ckt_inp_provider_t iwrap (node, this, ipp);
        BDD result =  node->get_lib_elem()->getFn(&iwrap);
        return result;
    }
}


BDD input_provider_wrapper_t::inp(int index)
{
    node_t *inp_i = node->get_input(index);
    return kcover->getFn(ipp, inp_i);
}

BDD ckt_inp_provider_t::inp(int index)
{
    node_t* inp_i = node->get_input(index);
    return kcover->getCktFn(ipp, inp_i);
}

BDD input_provider_wrapper_t::one()
{
    return ipp->one();
}

BDD input_provider_wrapper_t::zero()
{
    return ipp->zero();
}

bool kcover_t::prune(input_provider_t* ipp)
{
    BDD bdd = getFn(ipp);

    if(bdd.SupportSize() != (int) size()) {
        assert(internals.size() == 0);

        for(unsigned i = 0; i != parents.size(); i++) {
            nodeset_t& s = parents[i]->get_internals();
            for(nodeset_t::iterator it = s.begin(); it != s.end(); it++) {
                node_t* n = *it;
                internals.insert(n);
            }
            internals.insert(root);
        }
        prune_leaves();
        return true;
    } else {
        return false;
    }
    
}

int kcover_t::pruneLeaves(BDD& fn, input_provider_t* e)
{
    printf("\n");
    std::cout << "cover size: " << size() << std::endl;
    std::cout << "bdd is: "; printBDD(stdout, fn);
    std::cout << "doesn't depend on: ";

    std::set<int> missingInputs;
    for(unsigned i=0; i != size(); i++) {
        BDD vi = e->inp(i);
        if(fn.Cofactor(vi) == fn.Cofactor(!vi)) {
            std::cout << i << " ";
            missingInputs.insert(i);
        }
    }
    std::cout << std::endl;
    std::cout << "old cover: " << *this << std::endl;;
    leafset_t new_leaves;
    new_leaves.reserve(leaves.size());
    // rearrange the leaves.
    for(unsigned i=0; i != size(); i++) {
        if(missingInputs.find(i) == missingInputs.end()) {
            new_leaves.push_back(leaves[i]);
        }
    }
    for(unsigned i=0; i != size(); i++) {
        if(missingInputs.find(i) != missingInputs.end()) {
            new_leaves.push_back(leaves[i]);
        }
    }
    std::copy(new_leaves.begin(), new_leaves.end(), leaves.begin());
    std::cout << "new cover: " << *this << std::endl;;

    significantInputs = missingInputs.size();
    return significantInputs;
}

void kcover_t::prune_leaves()
{
    assert(internals.size() > 0);
    leafset_t new_leaves;
    for(leafset_t::iterator it = leaves.begin(); it != leaves.end(); it++) {
        node_t* n = *it;
        if(!is_node_internal(n)) {
            new_leaves.push_back(n);
        }
    }
    leaves.resize(new_leaves.size());
    std::copy(new_leaves.begin(), new_leaves.end(), leaves.begin());
}

kcover_t::nodeset_t& kcover_t::get_internals()
{
    if(internals.size() > 0) return internals;
    else {
        compute_internals(root);
        return internals;
    }
}

void kcover_t::compute_internals(node_t* root)
{
    if(size() == 0 || is_node_present(root)) return;
    else {
        internals.insert(root);
        for(node_t::input_iterator it  = root->inputs_begin(); 
                                   it != root->inputs_end();
                                   it++)
        {
            node_t *inp = *it;
            compute_internals(inp);
        }
    }
}

bool kcover_t::operator==(const kcover_t& other) const
{
    if(size() !=  other.size()) return false;
    else {
        for(leafset_t::const_iterator it = other.leaves.begin(); it != other.leaves.end(); it++) {
            node_t* l = (*it);
            if(!is_node_present(l)) return false;
        }
        return true;
    }
}
