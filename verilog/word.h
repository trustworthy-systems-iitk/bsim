#ifndef __WORD_H_DEFINED__
#define __WORD_H_DEFINED__

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <set>

#include <assert.h>
#include <stdint.h>

#include <boost/property_tree/ptree.hpp>

class node_t;

namespace aggr {
    class id_module_t;
}

class word_t {
private:
    static const int SIGN_PRIME;
protected:
    typedef std::vector<node_t*> bitlist_t;
public:
    typedef std::pair<int, uintptr_t> sign_t;
    typedef bitlist_t::iterator iterator;
    typedef bitlist_t::const_iterator const_iterator;
    typedef bitlist_t::reverse_iterator reverse_iterator;
protected:
    const bool ordered;
    bitlist_t bits;
    void* aux;
    sign_t sign;
public:
    enum source_t { SIMPLE_WORD_ANALYSIS, KCOVER_ANALYSIS, RCA_SUM, RF_RD_ADDR, RF_RD_DATA, RF_WR_ADDR, RF_WR_DATA, BACK_PROPAGATION, FWD_PROPAGATION, NETLIST };
private:
    source_t source;
    int index;
    bool locked;
    aggr::id_module_t* module;


    void invalidate_sign() { sign = sign_t(-1, 0); }
    bool sign_valid() const { return sign.first != -1; }

    bool input_word;
    bool output_word;
public:
    word_t(bool order, source_t src);
    ~word_t();

    bool rf_word() const {
        return source == RF_RD_ADDR ||
	       source == RF_RD_DATA ||
	       source == RF_WR_DATA ||
	       source == RF_WR_ADDR;
    }
    // do both these words have the same bits.
    bool same_bits (const word_t& w) const;

    void set_module(aggr::id_module_t* m) { module = m; }
    aggr::id_module_t* get_module() const { return module; }

    // do both these words have the same order.
    bool same_word (const word_t& w) const;

    // is this an ordered word?
    bool is_ordered() const { return ordered; }

    // number of bits.
    unsigned size() const { return bits.size(); }

    // add a bit to this node.
    void add_bit(node_t* b, int position=-1);

    void set_input_word() { input_word = true; }
    void set_output_word() { output_word = true; }
    bool is_input_word() const { return input_word; }
    bool is_output_word() const { return output_word; }

    // get bit list.
    std::vector<node_t*>& get_input_list() {
        return bits; 
    }

    void bit_ind (node_t* n, std::vector<int>& v) {
      size_t i = 0;
      for (iterator it = begin(); it != end(); it++) {
         if (*it == n) v[i] = 1;
         i++;
      }
    }


    // iterator access.
    iterator begin() { return bits.begin(); }
    iterator end() { return bits.end(); }
    const_iterator begin() const { return bits.begin(); }
    const_iterator end() const { return bits.end(); }
    reverse_iterator rbegin() { return bits.rbegin(); }
    reverse_iterator rend() { return bits.rend(); }

    // is this bit present in this word.
    bool isBitPresent(node_t* b) const;

    // get the bit's index.
    int getBitIndex(node_t* b) const;

    // is this word an (improper) subset of another.
    bool subsetOf(const word_t& other) const;


    // set info structure.
    void set_info (void* info){ aux = info; }

    // get info structure.
    void* get_info() { return aux; }

    source_t getSrc() const { return source; }
    void setSrc(source_t s) { source = s; }

    void set_index(int ind) { index = ind; }
    int get_index() const { return index; }

    node_t* get_bit(int index) const { return bits[index]; }

    // methods for managing the hash table of bits in the flat module.

    // get this words "signature"
    sign_t get_sign();

    // lock this word.
    bool isLocked() const { return locked; }
    void setLocked() { locked = true; }

    void dump(std::ostream& out);

    friend std::ostream& operator<<(std::ostream& out, const word_t& w);

    // this returns a property tree representing this word.
    // we will use this property tree when dumping JSON.
    boost::property_tree::ptree get_ptree();
};

std::ostream& operator<<(std::ostream& out, const word_t& w);
#endif // __WORD_H_DEFINED__
