#include "word.h"
#include "node.h"


const int word_t::SIGN_PRIME = 257;
const char* word_sources[] = {
    "SIMPLE_WORD_ANALYSIS", 
    "KCOVER_ANALYSIS", 
    "RCA_SUM", 
    "RF_RD_ADDR", 
    "RF_RD_DATA", 
    "RF_WR_ADDR", 
    "RF_WR_DATA", 
    "BACK_PROPAGATION", 
    "FWD_PROPAGATION", 
    "NETLIST" 
};

word_t::word_t(bool o, source_t src)
    :ordered(o),
     source(src),
     index(-1)
{
    sign = sign_t(-1, 0);
    locked = false;
    input_word = false;
    output_word = false;
    module = NULL;
}

word_t::~word_t()
{
}

bool word_t::same_bits(const word_t& w) const
{
    if(size() != w.size()) return false;

    for(bitlist_t::const_iterator it =  w.begin();
                                  it != w.end();
                                  it++)
    {
        node_t* n = *it;
        if(!isBitPresent(n)) return false;
    }
    return true;
}

bool word_t::same_word(const word_t& w) const
{
    if(size() != w.size()) return false;

    // two words are the same iff they have the same bits and the same indices.
    // this is not a good form of uniqueness but it will have to do for now because the
    // module formation needs to be fixed otherwise.
    word_t::const_iterator it = begin();
    word_t::const_iterator jt = w.begin();

    for(; it != end() && jt != w.end(); it++, jt++) {
        node_t *n1 = *it;
        node_t* n2 = *jt;
        if(n1 != n2) return false;
    }

    return true;
}

void word_t::add_bit(node_t* b, int position)
{
    assert( (position == -1 && !ordered) || (position != -1 && ordered && position == (int)bits.size()) );
    assert(!isLocked());
    bits.push_back(b);
    invalidate_sign();
}

word_t::sign_t word_t::get_sign()
{
    if(sign_valid()) return sign;
    else {
        sign.first = size();
        sign.second = 0;
        for(unsigned i=0; i != size(); i++) {
            node_t* n = bits[i];
            sign.second = (sign.second + (uintptr_t)n) % SIGN_PRIME;
        }
        return sign;
    }
}

bool word_t::isBitPresent(node_t* b) const
{
    int idx = getBitIndex(b);
    return idx != -1;
}

int word_t::getBitIndex(node_t* b) const
{
    for(unsigned i=0; i != bits.size(); i++) {
        if(bits[i] == b) return (int) i;
    }
    return -1;
}

bool word_t::subsetOf(const word_t& other) const
{
    for(const_iterator it = begin(); it != end(); it++) {
        node_t* b = *it;
        if(!other.isBitPresent(b)) return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const word_t& w)
{
    out << "index:"<< w.get_index() << " ";
    for(word_t::const_iterator it = w.begin();
                               it != w.end();
                               it++)
    {
        node_t* n = *it;
        out << n->get_name() << " ";
    }
    return out;
}

bool compare_bit(const node_t* a, const node_t* b)
{
    return a->get_name() < b->get_name();
}

void word_t::dump(std::ostream& out)
{
    bitlist_t bits_copy(bits);
    std::sort(bits_copy.begin(), bits_copy.end(), compare_bit);
    for(unsigned i=0; i != bits_copy.size(); i++) {
        out << bits_copy[i]->get_name();
        if((i+1) != bits.size()) {
            out << ", ";
        }
    }
}

boost::property_tree::ptree word_t::get_ptree()
{
    using boost::property_tree::ptree;

    ptree word;
    word.put("tool", "bsim");
    if(module == NULL) {
        word.put("component",  "unknown" );
        word.put("instance", "unknown" );
    } else {
        word.put("component",  module->get_type() );
        word.put("instance",  module->get_name());
    }
    word.put("ordered", ordered);
    word.put("source", word_sources[source]);

    string orient;
    if(input_word) {
        orient += "input";
    }
    if(output_word) {
        orient += "output";
    }
    word.put("orientation", orient.size() == 0 ? "unknown" : orient.c_str());

    ptree signals;
    for(unsigned int i = 0; i != size(); i++) {
        ptree child;
        child.put("", get_bit(i)->get_name().c_str());
        signals.push_back(std::make_pair("", child));
    }

    word.add_child("signals", signals);
    return word;
}

