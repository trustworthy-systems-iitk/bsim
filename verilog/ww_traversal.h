#ifndef _ww_traversal_h
#define _ww_traversal_h

#include "node.h"
#include "word.h"
#include "info.h"
#include "aggr.h"
#include <vector>
#include <set>
#include <map>

typedef std::vector<node_t*> nodelist_t;
typedef nodelist_t::iterator node_iterator;
typedef std::vector<word_t*> wordlist_t;
typedef wordlist_t::iterator word_iterator;
typedef std::vector<word_weight_t> ww_list_t;
typedef std::set<aggr::id_module_t*> moduleset_t;

enum traverse_mode { WW,WM };

std::vector<int> node_inds;
void print_vector (std::vector<int>& v, int bit_ind){
   cout<<endl;
   cout<<" bit "<<bit_ind<<" : ";
   for (std::vector<int>::iterator it = v.begin(); it != v.end(); it++){
      cout<<*it<<".";
   };
};

struct Wordinp{
   set<node_t*>* INP;
   set<word_t*>* INPWORD;
};

set<word_t*>* get_word_propagation_set (word_t* w){
//set<Wordinp*>* get_word_propagation_set (word_t* w){
    bool is_next_word = true;
    std::map<word_t*, set<node_t*>*> map;
    set<word_t*>* wordset = new set<word_t*>;
    wordset->insert(w);
    set<word_t*>::iterator word_it = wordset->begin();
    while (is_next_word && word_it != wordset->end()){

          nodelist_t bits = (*word_it)->get_input_list();
          for (node_iterator it = bits.begin(); it != bits.end(); it++){
              if (is_next_word){
                 for (node_iterator it_child = (*it)->inputs_begin(); it_child != (*it)->inputs_end(); it_child++){
                     node_t* node = *it_child;
                     if (node->num_words() != 0 || node->get_type() != node_t::LATCH){
                         wordlist_t* p_words = node->get_word_list();
                         for (word_iterator itw = p_words->begin(); itw != p_words->end(); itw++){
                              word_t* word = *itw;  
                              std::map<word_t*, std::set<node_t*>*>::iterator itmap = map.find(word);
                              if (itmap != map.end()){
                                 itmap->second->insert(node);
                              }else{
                                 std::set<node_t*>* set = new std::set<node_t*>;
                                 set->insert(node);
                                 map[*itw] = set;
                              };
                         };
                     }else{ is_next_word = FALSE; break;};
                 };
              }else break;
          };
          if (is_next_word){
             for (std::map<word_t*, set<node_t*>*>::iterator mit = map.begin(); mit != map.end(); mit++){
                 if (mit->second->size() == w->size()) wordset->insert(mit->first);
             };
          };
          word_it++;
    };
    return wordset;
};


void DFS (node_t* start, size_t depth, int bit_ind, set<node_t*>& all_nodes_traversed){
//cout<<"start node: "<<start<<endl;
    info* info_start = (info*) start->get_info();
    //assert(info_start->get_state() != VISITED);
    if (info_start->get_state() == VISITED) return; 
    //ww_list_t* ww = info_start->get_ww_pairs (); //cout<<"ww: "<<ww->size()<<endl;
    //set<node_t*>* inp = info_start->get_inputs(); //cout<<"inp: "<<inp->size()<<endl;
    assert(info_start->is_clear());
    all_nodes_traversed.insert(start);
    node_iterator it_begin = start->inputs_begin(), it_end = start->inputs_end();
    int i = 1;
    for (node_iterator it = it_begin; it != it_end; it++){
        node_inds.push_back(i);
        node_t* current = *it;
        //cout<<"current node "<<current<<endl;
        if (current->num_words() == 0 && current->get_type() != node_t::LATCH){
            if (current->get_type() != node_t::INPUT){
               depth++;
               DFS(current, depth, bit_ind, all_nodes_traversed);
               depth--;
            }else{//cout<<"input"<<endl;
               info_start->add_input(current);
            };
        }else{
            if (current->num_words() != 0){ //cout<<"inp word"<<endl;
                wordlist_t* p_words = current->get_word_list();
                info* inf = (info*) current->get_info();
                for (word_iterator itw = p_words->begin(); itw != p_words->end(); itw++){
                    vector<int> vbit ((*itw)->size());
                    (*itw)->bit_ind(current, vbit);
                    word_weight_t ww ((*itw), 1, vbit);
                    inf->add_ww(ww);
                };
                //inf->set_visited();
            }else{//cout<<"latch"<<endl;
                    info_start->add_input(current);
            };
        };
        node_inds.pop_back();
        i++;
    };
    info_start->set_visited();
    if (start->num_inputs() != 0) { info_start->unite_ww();};// cout<<"start unite called "<<start<<endl;};
    //if (info_start->get_state() == VISITED) cout<<"VISITED"<<endl; else cout<<"not visited"<<endl;
    if (info_start->has_input_words()){//cout<<"start "<<start<<" got cleared"<<endl;
       info_start->clear_inputs(); 
       info_start->add_input(start);
    };
}


void ww_traversal (flat_module_t& flat){
   wordlist_t* words = flat.get_words();
   set<node_t*> all_nodes_traversed;
   for (word_iterator it = words->begin(); it != words->end(); it++){
     if ((*it)->is_input_word()){
       all_nodes_traversed.clear();
       info* info_word = (info*) (*it)->get_info();
       //cout<<"--------------------------------------------------------------"<<endl;
       //cout<<"Word Ind: "<<info_word->index<<", word width : "<<(*it)->size()<<endl;
       //cout<<"--------------------------------------------------------------"<<endl;
       nodelist_t inp_list = (*it)->get_input_list();
       int bit_ind = 1;
       for (node_iterator inp_it = inp_list.begin(); inp_it != inp_list.end(); inp_it++){
            //cout<<"next child of the bit "<<*inp_it<<endl;
            info* inf = (info*) (*inp_it)->get_info();
            if (inf->get_state() != VISITED && !inf->is_clear()){
               assert((*inp_it)->num_words()!=0);
               inf->clear_ww_pairs();
               inf->clear_inputs();
               //inf->set_new();
            };
            DFS(*inp_it,1,bit_ind, all_nodes_traversed);
            bit_ind++;
       };
       info_word->set_visited();
       info_word->unite_ww();
       for (set<node_t*>::iterator it = all_nodes_traversed.begin(); it != all_nodes_traversed.end(); it++) { info* inf = (info*) (*it)->get_info(); inf->clear_ww();};
       //info_word->clear_bits_ww();
       //info_word->print_ww();
     };
   };

}

bool is_in_module (node_t* node, moduleset_t& identified_modules){
     moduleset_t* modules = node->get_modules();
     //cout<<"( "<<node->get_name()<<" SZ="<<modules->size()<<"{ ";
     using namespace aggr;
     bool found = false;
     for (moduleset_t::iterator it_m = modules->begin(); it_m != modules->end(); it_m++){
         aggr::id_module_t::type_t type = (*it_m)->get_moduleType();
         //cout<<type<<" ";
         if (type == id_module_t::USER_DEFINED || type == id_module_t::INFERRED) {
             identified_modules.insert(*it_m);
             found = true;
         }
     }
//cout<<"} : "<<found<<" ) ";
//cout<<"found = "<<found<<endl;
     return found;
}


std::vector<const char*> path;
void DFS_wm (node_t* start, size_t depth, int bit_ind, set<node_t*>& all_nodes_traversed, bool is_bit){
//path.push_back(start->get_name()); 

        moduleset_t im;
        assert(!is_in_module(start,im)||is_bit);

cout<<is_bit<<"_"<<is_in_module(start,im)<<"@"<<start->get_name()<<" , "; 
//for (size_t i = 0; i < path.size(); i++) cout<<path[i]<<" , "; cout<<endl;
//const char* rd = "rd", *not_rd = "NOT_rd";
    info* info_start = (info*) start->get_info();
//is_bit = FALSE;
//if (strcmp (start->get_name(),rd) == 0 || strcmp (start->get_name(), not_rd) == 0) {cout<<"node = "<<start->get_name()<<" type "<<start->get_type()<<", inputs: "<<endl;
//           for (set<node_t*>::iterator inp_it = info_start->inp_begin(); inp_it != info_start->inp_end(); inp_it++) 
//           cout<<(*inp_it)->get_name()<<" , "; cout<<endl;};
    if (info_start->get_state() == VISITED) { cout<<"VISITED , ";//if (strcmp (start->get_name(),rd) == 0 || strcmp (start->get_name(), not_rd) == 0) cout<<"RETURN! "<<endl; 
return; }; 
all_nodes_traversed.insert(start);
    //info_start->clear_inputs();
    info_start->set_visited();
    node_iterator it_begin = start->inputs_begin(), it_end = start->inputs_end();
    //int i = 1;
    for (node_iterator it = it_begin; it != it_end; it++){
        //node_inds.push_back(i);
	moduleset_t identified_modules;
        node_t* current = *it;
//if (strcmp (start->get_name(),rd) == 0 || strcmp (start->get_name(), not_rd) == 0) cout<<"node = "<<start->get_name()<<" - current = "<<current->get_name()<<" type: "<<current->get_type()<<endl;
       if ( !is_in_module (current, identified_modules) ){
           if (current->get_type() != node_t::INPUT){
               depth++;
               DFS_wm(current, depth, bit_ind, all_nodes_traversed, FALSE);
               depth--;
            }else{//if (strcmp (start->get_name(),rd) == 0 || strcmp (start->get_name(), not_rd) == 0) cout<<" INPUT "<<endl;
		cout<<"crntINP: "<<current->get_name()<<" ; ";
               info_start->add_input(current);
            };
        }else{
            cout<<"crntMOD: "<<current->get_name()<<" . ";
            for (moduleset_t::iterator it_m = identified_modules.begin(); it_m != identified_modules.end(); it_m++){
                    info* inf = (info*) current->get_info();
                    inf->add_module(*it_m);
                    //cout<<"module: "<<(*it_m)->moduleNum()<<" , ";
            };//if (strcmp (start->get_name(),rd) == 0 || strcmp (start->get_name(), not_rd) == 0) cout<<"is_in_module "<<endl;
            info_start->add_input(current);
        };
        //node_inds.pop_back();
        //i++;
    };
    //info_start->set_visited();
    if (start->num_inputs() != 0) info_start->unite_wm();
//if (strcmp (start->get_name(),rd) == 0 || strcmp (start->get_name(), not_rd) == 0){
//cout<<"node: "<<start->get_name()<<" , after unite : ";
//         for (set<node_t*>::iterator inp_it = info_start->inp_begin(); inp_it != info_start->inp_end(); inp_it++) 
//           cout<<(*inp_it)->get_name()<<" , "; cout<<endl;
//};
//cout<<" unite res for "<<start->get_name()<<" : ";
//for (set<node_t*>::iterator i = info_start->inp_begin(); i != info_start->inp_end(); i++) cout<<(*i)->get_name()<<" , "; cout<<endl;
}


/** Traversal from words to identified modules.*/
void wm_traversal (flat_module_t& flat){
   wordlist_t* words = flat.get_words();
//   for (word_iterator it = words->begin(); it != words->end(); it++){
   set<node_t*> all_nodes_traversed;
   for (word_iterator it = words->begin(); it != words->end(); it++){
	if ((*it)->is_input_word()){
		all_nodes_traversed.clear();
      		info* info_word = (info*) (*it)->get_info();
       		cout<<"--------------------------------------------------------------"<<endl;
       		cout<<"Word Ind: "<<info_word->index<<", word width : "<<(*it)->size()<<endl;
       		cout<<"--------------------------------------------------------------"<<endl;
                cout<<"inputs: "<<info_word->get_inputs()->size()<<" modules: "<<info_word->get_modules()->size()<<endl;
       		nodelist_t inp_list = (*it)->get_input_list();
       		int bit_ind = 1;
       		for (node_iterator inp_it = inp_list.begin(); inp_it != inp_list.end(); inp_it++){
            	  //info* inf = (info*) (*inp_it)->get_info();
            	  //inf->clear_wm();
            	  //inf->set_new();
moduleset_t* mods = (*inp_it)->get_modules();cout<<"bit :"<<(*inp_it)->get_name()<<" : ";
for (moduleset_t::iterator i = mods->begin(); i != mods->end(); i++) cout<<(*i)->moduleNum()<<" - "<<(*i)->get_type()<<" , ";
cout<<endl;
}
                for (node_iterator inp_it = inp_list.begin(); inp_it != inp_list.end(); inp_it++){
cout<<"..................Next Bit................."<<endl;
//int k; cin>>k;
path.clear();
            	  DFS_wm(*inp_it,1,bit_ind, all_nodes_traversed, TRUE);
            	  bit_ind++;
//                  cout<<(*inp_it)->get_name()<<" , ";
        	};
	        info_word->set_visited();
	        info_word->unite_wm();
                cout<<"inputs: "<<info_word->get_inputs()->size()<<" modules: "<<info_word->get_modules()->size()<<endl;
//cout<<" unite res for word index: "<<info_word->index<<" : ";
//for (set<node_t*>::iterator i = info_word->inp_begin(); i != info_word->inp_end(); i++) cout<<(*i)->get_name()<<" , "; cout<<endl;
//moduleset_t* mods = info_word->get_modules(); cout<<"modules hitted : ";
//for (moduleset_t::iterator it_m = mods->begin(); it_m != mods->end(); it_m++) cout<<(*it_m)->moduleNum()<<" , "; cout<<endl;
//		info_word->clear_bits_wm();
cout<<"all nodes traversed: "<<all_nodes_traversed.size()<<endl;
                for (set<node_t*>::iterator it = all_nodes_traversed.begin(); it != all_nodes_traversed.end(); it++) { info* inf = (info*) (*it)->get_info(); inf->clear_wm();};

       		//info_word->print_ww();
         };
   };
//cout<<"LATCHES OUT"<<endl;
//   nodelist_t* latches = flat.get_latches();
//   for (node_iterator it = latches->begin(); it != latches->end(); it++){
//       cout<<(*it)->get_name()<<" , ";
//   };cout<<endl;

}


void DFS (node_t* start, size_t depth){
    info* info_start = (info*) start->get_info();
    if (info_start->get_state() == VISITED) return;
    node_iterator it_begin = start->inputs_begin(), it_end = start->inputs_end();
    int i = 1;
    for (node_iterator it = it_begin; it != it_end; it++){
        node_inds.push_back(i);
        node_t* current = *it;
        if (current->get_type() != node_t::LATCH){
            if (current->get_type() != node_t::INPUT){
               depth++;
               DFS(current, depth);
               depth--;
            }else{
               info_start->add_input(current);
            };
        }else{
            latch_weight_t* lw = new latch_weight_t(current, 1);
            info* inf = (info*) current->get_info();
            inf->add_lw(lw);
        };
        node_inds.pop_back();
        i++;
    };
    info_start->set_visited();
    if (start->num_inputs() != 0) info_start->unite_lw();
    //info_start->print_lw();
}


void lw_traversal (flat_module_t& flat){
   nodelist_t* latches = flat.get_latches();
   for (node_iterator it = latches->begin(); it != latches->end(); it++){
       info* info_latch = (info*) (*it)->get_info();
       cout<<"--------------------------------------------------------------"<<endl;
       cout<<"Latch Ind: "<<info_latch->index<<endl;
       cout<<"--------------------------------------------------------------"<<endl;
       info_latch->clear_lw_pairs();
       info_latch->set_new();
       DFS((*it),1);
       info_latch->print_lw((*it));
       //info_latch->clear_latch();
       //info_latch->print_lw((*it));
   };
}

void clear_nodes (flat_module_t& flat){
   nodelist_t* inputs = flat.get_inputs();
   nodelist_t* gates = flat.get_gates();
   nodelist_t* latches = flat.get_latches();
   wordlist_t* words = flat.get_words();
   for (node_iterator it = inputs->begin(); it != inputs->end(); it++){
       info* inf = (info*) (*it)->get_info();
       inf->set_new();
       inf->clear_inputs();
   };

   for (node_iterator it = gates->begin(); it != gates->end(); it++){
       info* inf = (info*) (*it)->get_info();
       inf->set_new();
       inf->clear_inputs();
   };

   for (node_iterator it = latches->begin(); it != latches->end(); it++){
       info* inf = (info*) (*it)->get_info();
       inf->set_new();
       inf->clear_inputs();
   };

   for (word_iterator it = words->begin(); it != words->end(); it++){
       info* inf = (info*) (*it)->get_info();
       inf->set_new();
       inf->clear_inputs();
   };
}

void init_module (aggr::id_module_t& module, word_t* inp_w, traverse_mode type){
     info* inf_inp_w = (info*) inp_w->get_info();
     for (std::set<node_t*>::iterator it_n = inf_inp_w->inp_begin(); it_n != inf_inp_w->inp_end(); it_n++) { module.add_input((*it_n)); }; 
     if (type == WW){
        for (ww_list_t::iterator it_ww = inf_inp_w->ww_pairs_begin(); it_ww != inf_inp_w->ww_pairs_end(); it_ww++) module.add_input_word((*it_ww).PWORD);
     };
     if (type == WM){
        for (moduleset_t::iterator it_m = inf_inp_w->get_modules()->begin(); it_m != inf_inp_w->get_modules()->end(); it_m++) module.add_input_module (*it_m);
     };
     module.add_output_word (inp_w);
     module.compute_internals();
}

void init_module_propagated (aggr::id_module_t& module, word_t* inp_w, std::set<word_t*>& inputs, traverse_mode type){
     info* inf_inp_w = (info*) inp_w->get_info();
     for (std::set<node_t*>::iterator it_n = inf_inp_w->inp_begin(); it_n != inf_inp_w->inp_end(); it_n++) { module.add_input((*it_n)); }; 
     if (type == WW){
        cout<<"size inp initializing module all zeros = "<<inputs.size();
       
        //for (ww_list_t::iterator it_ww = inf_inp_w->ww_pairs_begin(); it_ww != inf_inp_w->ww_pairs_end(); it_ww++) module.add_input_word((*it_ww).PWORD);
        for (std::set<word_t*>::iterator it = inputs.begin(); it != inputs.end(); it++) {module.add_input_word(*it);};
     };
     module.add_output_word (inp_w);
     module.compute_internals();
}

std::set<std::set<word_t*>*>* init_next_propagated (std::vector<size_t>& its, word_t* inp_w){
        info* inf_inp_w = (info*) inp_w->get_info();
cout<<"init next prop"<<endl;
        std::set<std::set<word_t*>*>* all_propagated_words = new std::set<std::set<word_t*>*>;
size_t s = 1;
        for (ww_list_t::iterator it_ww = inf_inp_w->ww_pairs_begin(); it_ww != inf_inp_w->ww_pairs_end(); it_ww++){
            std::set<word_t*>* propagated_words = get_word_propagation_set ((*it_ww).PWORD);
            cout<<propagated_words->size()<<" , ";
            s = s*propagated_words->size();
            if (!propagated_words->empty()) if (s<100) all_propagated_words->insert(propagated_words);
        };cout<<endl;
        for (std::set<std::set<word_t*>*>::iterator i = all_propagated_words->begin(); i != all_propagated_words->end(); i++) its.push_back(0);
        return all_propagated_words;
};

bool next_propagated (std::vector<size_t>& its, std::set<word_t*>& inputs, std::set<std::set<word_t*>*>* all_propagated_words, word_t* inp_w){
        inputs.clear();
        //for (std::set<std::set<word_t*>*>::iterator i = all_propagated_words->begin(); i != all_propagated_words->end(); i++) {
        //    for (std::set<word_t*>::iterator w = (*i)->begin(); w != (*i)->end(); w++) {cout<<"HERE "<<endl; cout<<(*w)->size()<<endl;};
        //};
        std::set<std::set<word_t*>*>::iterator itset = all_propagated_words->begin();
        size_t itset_i = 0;
        while (itset != all_propagated_words->end()){
              //std::set<word_t*>::iterator itemp = its[itset_i], iend = (*itset)->end()--;
              if (its[itset_i] != (*itset)->size()-1){
                 //std::set<word_t*>::iterator temp = its[itset_i]; temp++; cout<<"-----"<<endl; cout<<(*temp)->size()<<endl;
                 its[itset_i]++; //cout<<"LATER there "<<itset_i<<endl; cout<<(*(itsetits[itset_i]))->size()<<endl; 
                 break;
              }else{
                 its[itset_i] = 0; itset++; itset_i++; //cout<<"LATER HERE "<<itset_i<<endl; //cout<<(*its[itset_i])->size()<<endl;
              };
        };
        if (itset != all_propagated_words->end()){
           std::set<std::set<word_t*>*>::iterator it = all_propagated_words->begin();
           //cout<<"inputs set: "<<(*itset)->size()<<" : ";
           for (size_t i = 0; i < all_propagated_words->size(); i++) { //cout<<"adding "<<endl; 
               std::set<word_t*>::iterator i_beg = (*it)->begin();
               for (size_t k = 0; k < its[i]; k++) i_beg++;
               inputs.insert(*i_beg);
               cout<<its[i]<<" ,";
               it++;
           };cout<<" : inputs set size = "<<inputs.size()<<endl;
        };
        return itset != all_propagated_words->end();
};


vector<aggr::id_module_t*>* dump_modules (flat_module_t& flat, traverse_mode type) {
      vector<aggr::id_module_t*>* cand_modules = new vector<aggr::id_module_t*>;
      wordlist_t* words = flat.get_words();
      bool propagation = false;
      for (word_iterator it = words->begin(); it != words->end(); it++){
       info* inf = (info*) (*it)->get_info();
       if (type == WW){
          if ( !inf->has_input_words()){
             if (propagation){
             std::vector<size_t> its;
             std::set<std::set<word_t*>*>* all_propagated_words = init_next_propagated(its, (*it));
             set<word_t*> inputs;
             for (std::set<std::set<word_t*>*>::iterator itw = all_propagated_words->begin(); itw != all_propagated_words->end(); itw++) 
                 inputs.insert((*(*itw)->begin()));
             int count = 0;
             do{
                   cout<<"count = "<<count<<" : "<<endl;
           	   aggr::id_module_t* module = new aggr::id_module_t("candidate_word_bound", aggr::id_module_t::UNSLICEABLE, aggr::id_module_t::CANDIDATE_WORD_BOUND);
                   init_module_propagated (*module, *it, inputs, WW);
    	           //init_module (*module, (*it), WW);
	           flat.add_module(module);
                   cand_modules->push_back(module);
                   cout<<endl;
                   //cout<<"module: "<<module->moduleNum()<<": "<<endl;
                   //cout<<"number of inputs: "<<inf->number_of_inputs()<<" ; number of internals: "<<module->num_internals()<<" ; number of input words: "<< inf->number_of_words_inputs()<<endl;
                   count++;
             }while (next_propagated(its, inputs, all_propagated_words, (*it)));
             }else{
                   aggr::id_module_t* module = new aggr::id_module_t("candidate_word_bound", aggr::id_module_t::UNSLICEABLE, aggr::id_module_t::CANDIDATE_WORD_BOUND);
                   init_module (*module, (*it), WW);
                   flat.add_module(module);
                   cand_modules->push_back(module);
                   //cout<<endl;
                   //cout<<"module: "<<module->moduleNum()<<": "<<endl;
                   //cout<<"number of inputs: "<<inf->number_of_inputs()<<" ; number of internals: "<<module->num_internals()<<" ; number of input words: "<< inf->number_of_words_inputs()<<endl;

             };
          };
       };
       if (type == WM){
	  if (!inf->has_modules()){
//cout<<"module dump word index: "<<inf->index<<endl;
             aggr::id_module_t* module = new aggr::id_module_t("candidate_module_bound", aggr::id_module_t::UNSLICEABLE, aggr::id_module_t::CANDIDATE_MODULE_BOUND);
             init_module (*module, (*it), WM);
             flat.add_module(module);
          };
       };

      };
      return cand_modules;
}



void init_info (flat_module_t& flat){
   nodelist_t* inputs = flat.get_inputs();
   nodelist_t* gates = flat.get_gates();
   nodelist_t* latches = flat.get_latches();
   wordlist_t* words = flat.get_words();
  
   weight_bit_t wb (0,0);

   info::ww_collector.clear();
   info::ww_collector.resize(words->size(),wb);

   info::lw_collector.clear();
   info::lw_collector.resize(latches->size(),0);

   for (node_iterator it = inputs->begin(); it != inputs->end(); it++){
       info* inf = new info ((*it)->get_input_list(), 0);
       (*it)->set_info(inf);
       //info* info_start = (info*) (*it)->get_info();
       //cout<<" input "<<(*it)<<" info "<<info_start<<endl;
   };

   for (node_iterator it = gates->begin(); it != gates->end(); it++){
       info* inf = new info ((*it)->get_input_list(), 0);
       (*it)->set_info((void*)inf);
       //info* info_start = (info*) (*it)->get_info();
       //cout<<" gate "<<(*it)<<" info "<<info_start<<endl;

   };

   int latch_count = 0;
   for (node_iterator it = latches->begin(); it != latches->end(); it++){
       info* inf = new info ((*it)->get_input_list(), latch_count);
       (*it)->set_info(inf);
       //info* info_start = (info*) (*it)->get_info();
      //cout<<"latch "<<(*it)<<" ind "<<latch_count<<" info "<<info_start<<endl; 
      latch_count++;
   };

   int word_count = 0;
   for (word_iterator it = words->begin(); it != words->end(); it++){
       info* inf = new info (&(*it)->get_input_list(), word_count);
       (*it)->set_info(inf);
       word_count++;
   };

}

void free_info (flat_module_t& flat){
   nodelist_t* inputs = flat.get_inputs();
   nodelist_t* gates = flat.get_gates();
   nodelist_t* latches = flat.get_latches();
   wordlist_t* words = flat.get_words();

   for (node_iterator it = inputs->begin(); it != inputs->end(); it++){
       info* inf =  (info*) (*it)->get_info();
       delete inf;
   };

   for (node_iterator it = gates->begin(); it != gates->end(); it++){
       info* inf =  (info*) (*it)->get_info();
       delete inf;
   };

   for (node_iterator it = latches->begin(); it != latches->end(); it++){
       info* inf =  (info*) (*it)->get_info();
       delete inf;
   };

   for (word_iterator it = words->begin(); it != words->end(); it++){
       info* inf =  (info*) (*it)->get_info();
       delete inf;
   };

}

#endif

