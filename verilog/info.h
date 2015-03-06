#ifndef _info_h
#define _info_h

#include "node.h"
#include "word.h"
#include <vector>
#include <set>
#include <assert.h>
#include <algorithm>
using namespace std;

/** Defines bit_count - a pair of pointers to bits of a word that had been hitted and number of hits. */
struct weight_bit_t {
    int WEIGHT;
    vector<int> BIT;

    weight_bit_t (size_t weight, int size):
       WEIGHT(weight)
       { BIT.clear(); BIT.resize(size,0);};

    void resize_bits (int size){
       BIT.clear(); BIT.resize(size,0);
    };

    size_t count_paths (){
       size_t count = 0;
       for(size_t i = 0; i< BIT.size(); i++) count = count + BIT[i];
       return count;
    };
};

/** Defines word_weight - a pair keeping pointer to a word and weight of this word, i.e. number of nodes on all paths to this word. */
struct word_weight_t {
    word_t* PWORD;
    size_t WEIGHT;
    vector<int> BIT;


    word_weight_t (word_t* p_word, int weight):
        PWORD(p_word),
        WEIGHT(weight)
    {BIT.clear(); BIT.resize(p_word->size(),0);};

    word_weight_t (word_t* p_word, int weight, vector<int>& bit):
        PWORD(p_word),
        WEIGHT(weight)
    { 
      BIT.clear(); 
      BIT.resize(p_word->size(),0); 
      for (size_t i = 0; i < BIT.size(); i++) BIT[i]=bit[i];
    };
};

/** Defines latch_weight - a pair keeping pointer to a latch and weight of this latch, i.e. number of nodes on all paths to this latch. */
struct latch_weight_t {
    node_t* PLATCH;
    size_t WEIGHT;
    bool is_subset;

    latch_weight_t (node_t* p_node, int weight):
        PLATCH(p_node),
        WEIGHT(weight),
        is_subset(FALSE)
    {};
};

/** Defines state of a node/word: 
    NEW - when the node/word has not been discovered yet, 
    MARKED - node/word has been discovered and is being processed at the moment (this will happen if there are cycles within words/nodes),
    VISITED - node/word has been discovered and processed. */

enum state_t { NEW, VISITED};


//struct latch_comp {
//  bool operator() (const latch_weight_t& lhs, const latch_weight_t& rhs) const
 // {
 //    info* l_info = (info*) lhs.PLATCH->get_info(), r_info = (info*) rhs.PLATCH->get_info();
 //    return (l_info->index() < r_info->index());
 // };
//};


class info {
public:

     static bool latch_comp (const latch_weight_t* lhs, const latch_weight_t* rhs){
        info* l_info = (info*) (lhs->PLATCH)->get_info(), * r_info = (info*) (rhs->PLATCH)->get_info();
        return (l_info->size_lw() < r_info->size_lw());
     };

     static bool latch_comp_n (node_t* lhs, node_t* rhs){
        info* l_info = (info*) lhs->get_info(), * r_info = (info*) rhs->get_info();
        return (l_info->size_lw() < r_info->size_lw());
     };
 
    void sort_lw () {
	sort(lw_pairs_begin(),lw_pairs_end(), latch_comp);
    };

    /** Vector for collecting weights of words. Used for linear complexity. */
    static vector<weight_bit_t> ww_collector;

    /** Vector for collecting weights of latches. Used for linear complexity. */
    static vector<size_t> lw_collector;

    typedef vector<word_weight_t> ww_list_t;
    typedef vector<latch_weight_t*> lw_list_t;
    typedef vector<node_t*> list_t;
    typedef std::set<aggr::id_module_t*> moduleset_t;

    set<node_t*>::iterator inp_begin() { return _inputs.begin(); };
    set<node_t*>::iterator inp_end() { return _inputs.end(); };
    ww_list_t::iterator ww_pairs_begin() { return _ww_pairs.begin(); };
    ww_list_t::iterator ww_pairs_end() { return _ww_pairs.end(); };
    list_t::iterator bit_begin() { return _list->begin(); };
    list_t::iterator bit_end() { return _list->end(); };

    lw_list_t::iterator lw_pairs_begin() { return _lw_pairs.begin(); };
    lw_list_t::iterator lw_pairs_end() { return _lw_pairs.end(); };

    const bool has_inputs () { return _inputs.empty(); };
    const bool has_input_words () { return _ww_pairs.empty(); }
    const bool has_modules () { return _modules.empty(); }
    const bool is_clear () { return (_inputs.empty() && _ww_pairs.empty()) && _modules.empty(); }

    int number_of_inputs () { return _inputs.size();};
    int number_of_words_inputs () { return _ww_pairs.size();};
private:
    state_t   _state;
    ww_list_t _ww_pairs;
    list_t*   _list;
    set<node_t*> _inputs;
    lw_list_t _lw_pairs;
    moduleset_t _modules;
    //set<node_t*, latch_comp_n> _sup_latches;


    /** Add weight of the word in the static WWCollector. If the word is already added to the collector update its weight. */
    void add_weight (word_weight_t& ww, ww_list_t& temp_ww_pairs){
        info* winfo = (info*) ww.PWORD->get_info(); 
        int word_ind = winfo->index;
        if (ww_collector[word_ind].WEIGHT == 0) {
            ww_collector[word_ind].resize_bits(ww.PWORD->size());
            word_weight_t* ww0 = new word_weight_t(ww.PWORD,1);
            temp_ww_pairs.push_back(*ww0);
        };
        ww_collector[word_ind].WEIGHT = ww_collector[word_ind].WEIGHT + ww.WEIGHT;
        for (size_t bit = 0; bit <ww_collector[word_ind].BIT.size(); bit++)
            ww_collector[word_ind].BIT[bit] = ww_collector[word_ind].BIT[bit] + ww.BIT[bit];
    }


    /** Add weight of the latch in the static WWCollector. If the word is already added to the collector update its weight. */
    void add_weight (const latch_weight_t* lw, lw_list_t& temp_lw_pairs){
        info* linfo = (info*) (lw->PLATCH)->get_info(); 
        int latch_ind = linfo->index;
        if (lw_collector[latch_ind] == 0) {
            latch_weight_t* lw0 = new latch_weight_t(lw->PLATCH,1);
            temp_lw_pairs.push_back(lw0);
        };
        lw_collector[latch_ind] = lw_collector[latch_ind] + lw->WEIGHT;
    }



public:

    int index;

    info(list_t* list, int ind):
      _state(NEW),
      _list(list),
      index(ind)
    {}

    ~info () {}

    void clear_ww_pairs () { _ww_pairs.clear(); }
    void clear_ww () {
 	 _ww_pairs.clear();
	 _inputs.clear();
        _state = NEW;
    }
    void clear_wm () { 
	_modules.clear();
        _inputs.clear();
        _state = NEW;
    }
    void clear_lw_pairs () { _lw_pairs.clear(); }
    void clear_lw () {
	 _lw_pairs.clear();
         _inputs.clear();
         _state = NEW;
    }
    void set_visited () { _state = VISITED; }    
    void add_input(node_t* n) { _inputs.insert(n); }
    void set_new () { _state = NEW; }

    ww_list_t* get_ww_pairs () { return &_ww_pairs; }
    lw_list_t* get_lw_pairs () { return &_lw_pairs; }
    set<node_t*>* get_inputs() { return &_inputs; }

    //set<node_t*, latch_comp_n>* get_sup_latches() {return &_sup_latches;}

    size_t size_lw () { return _lw_pairs.size(); }

    void add_ww (word_weight_t ww) { _ww_pairs.push_back(ww); }

    void add_lw (latch_weight_t* lw) { _lw_pairs.push_back(lw); }

    void add_module (aggr::id_module_t* pmod) { _modules.insert(pmod); }

    moduleset_t* get_modules () { return &_modules; }

    /** Print word-weight vector. */
    void print_ww (){
       cout<<"PRINT WW: ";
       for (ww_list_t::iterator it = _ww_pairs.begin(); it != _ww_pairs.end(); it++){
           info* inf = (info*) (*it).PWORD->get_info();
           cout<<inf->index<<" (w="<<(*it).PWORD->size()<<") : "<<(*it).WEIGHT<<"  -  ";
           for (size_t i = 0; i < (*it).PWORD->size(); i++) 
              if ((*it).BIT[i]) cout<<i<<"("<<(*it).BIT[i]<<") , ";
	   cout<<endl;
       };
    }

    /** Print latch-weight vector. */
    void print_lw (node_t* n){
       cout<<"PRINT LW: "<< n->get_name()<<" : ";
      
       for (lw_list_t::iterator it = _lw_pairs.begin(); it != _lw_pairs.end(); it++){
           //info* inf = (info*) (*it).PLATCH->get_info();
           cout<<(*it)->PLATCH->get_name()<<" , ";
       };
    }


   void print_collector () {
       cout<<"print collector :"<<endl;
       for (size_t i = 0; i != ww_collector.size(); i++){
	   cout<<i<<" : "<<ww_collector[i].WEIGHT<<"  -  ";
           for (size_t bit = 0; bit != ww_collector[i].BIT.size(); bit++) 
		if (ww_collector[i].BIT[bit]) cout<<bit<<"("<<ww_collector[i].BIT[bit]<<") , ";
       };
      cout<<endl;
   }

   void print_collector_lw () {
       cout<<"print collector lw :"<<endl;
       for (size_t i = 0; i != lw_collector.size(); i++){
           cout<<i<<" : "<<lw_collector[i]<<" , ";
       };
      cout<<endl;
   }

    void clear_inputs () { _inputs.clear();}

    void clear_bits_ww() {
         for (list_t::iterator it = _list->begin(); it != _list->end(); it++){
             info* inf = (info*) (*it)->get_info();
             inf->clear_ww();
         };
    }

    void clear_bits_wm() {
         for (list_t::iterator it = _list->begin(); it != _list->end(); it++){
             info* inf = (info*) (*it)->get_info();
             inf->clear_wm();
         };
    }



    /** Unites word-weight vectors of all nodes/bits. */
    void unite_ww (){
        assert(_state == VISITED);
	ww_list_t temp_ww_pairs;
        for (list_t::iterator it = _list->begin(); it != _list->end(); it++){
           info* inf = (info*) (*it)->get_info();
	   _inputs.insert (inf->inp_begin(), inf->inp_end());
           ww_list_t* ww_list = inf->get_ww_pairs();
           for (ww_list_t::iterator itww = ww_list->begin(); itww != ww_list->end(); itww++) add_weight (*itww, temp_ww_pairs);
        };

        _ww_pairs.clear();
        for (ww_list_t::iterator it = temp_ww_pairs.begin(); it != temp_ww_pairs.end(); it++){
            info* inf = (info*) (*it).PWORD->get_info();
	    assert (ww_collector[inf->index].WEIGHT != 0);
            (*it).WEIGHT = ww_collector[inf->index].WEIGHT + ww_collector[inf->index].count_paths();
            for (size_t bit = 0; bit < ww_collector[inf->index].BIT.size();bit++) (*it).BIT[bit] = ww_collector[inf->index].BIT[bit];
            _ww_pairs.push_back((*it));
            ww_collector[inf->index].WEIGHT = 0;
            ww_collector[inf->index].resize_bits((*it).PWORD->size());
        };
    }



bool is_in_module (node_t* node){
     moduleset_t* modules = node->get_modules();
     using namespace aggr;
     bool found = false;
     for (moduleset_t::iterator it_m = modules->begin(); it_m != modules->end(); it_m++){
         aggr::id_module_t::type_t type = (*it_m)->get_moduleType();
         if (type == id_module_t::USER_DEFINED || type == id_module_t::INFERRED) {
             found = true;
         }
     }
//cout<<"found = "<<found<<endl;
     return found;
}


    /** Unites sets of identified modules of all nodes/bits, for word-module triversal. */
    void unite_wm (){
        assert(_state == VISITED);
       //_modules.clear();
//cout<<" unite_wm start : ";
        for (list_t::iterator it = _list->begin(); it != _list->end(); it++){
            info* inf = (info*) (*it)->get_info();
	    assert(inf->get_state() == VISITED || (*it)->get_type() == node_t::INPUT || is_in_module(*it));
//cout<<inf->get_inputs()->size()<<" , ";
	    _inputs.insert (inf->inp_begin(), inf->inp_end());
            for (set<node_t*>::iterator inp_it = inf->inp_begin(); inp_it != inf->inp_end(); inp_it++) assert((*inp_it)->get_type() == node_t::INPUT || is_in_module(*inp_it));
            moduleset_t* mods = inf->get_modules();
            for (moduleset_t::iterator it_m = mods->begin(); it_m != mods->end(); it_m++) _modules.insert(*it_m);
        };
//cout<<endl;
    }


    /** Unites latch-weight vectors of all nodes/bits. */
    void unite_lw (){
        assert(_state == VISITED);
        lw_list_t temp_lw_pairs;
        for (list_t::iterator it = _list->begin(); it != _list->end(); it++){
           info* inf = (info*) (*it)->get_info();
           _inputs.insert (inf->inp_begin(), inf->inp_end());
           lw_list_t* lw_list = inf->get_lw_pairs();
           for (lw_list_t::iterator itlw = lw_list->begin(); itlw != lw_list->end(); itlw++) add_weight (*itlw, temp_lw_pairs);
        };

        _lw_pairs.clear();
        for (lw_list_t::iterator it = temp_lw_pairs.begin(); it != temp_lw_pairs.end(); it++){
            info* inf = (info*) ((*it)->PLATCH)->get_info();
            assert (lw_collector[inf->index] != 0);
            (*it)->WEIGHT = lw_collector[inf->index];// + lw_collector[inf->index].count_paths();
            _lw_pairs.push_back((*it));
            lw_collector[inf->index] = 0;
        };
    }





    /** Get state. */
    state_t get_state () {
        return _state;
    }

};
vector<weight_bit_t> info::ww_collector;
vector<size_t> info::lw_collector;

#endif
