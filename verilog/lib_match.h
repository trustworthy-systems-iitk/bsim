#ifndef _LIBMATCH_
#define _LIBMATCH_

#include "cuddObj.hh"
#include <cuddInt.h>
#include <vector>
#include <set>
#include "signature.h"
#include "aggr.h"


struct BDDpair {
       BDD PERMUTED;
       BDD ORIGINAL;
       node_t* OUTPUT;
       aggr::id_module_t* MODULE;
public:
       BDDpair(BDD b, node_t* out, aggr::id_module_t* mod):
              PERMUTED(b),
              ORIGINAL(b),
              OUTPUT(out),
              MODULE(mod)
       {};
};

//struct BDDpair_orig_compare_t {
//       bool operator() (const BDDpair* one, const BDDpair& two) const {
//            return one.ORIGINAL.getNode() < two.ORIGINAL.getNode();
//       }
//};

set<BDDpair*>* create_cand_mod_bdds (flat_module_t& flat, aggr::id_module_t* cand_mod) {

//cout<<"create_cand_mod_bdds start"<<endl;
        set<BDDpair*>* bdd_set = new set<BDDpair*>;
bool b = cand_mod->is_seq(); //cout<<"is_seq = "<<b<<endl;
if (b) return bdd_set;

        nodelist_t* outputs = cand_mod->get_outputs();
        wordlist_t* word_outputs = cand_mod->get_word_outputs();
        nodelist_t* inputs = cand_mod->get_inputs();
        wordlist_t* word_inputs = cand_mod->get_word_inputs();
        std::map<node_t*, BDD> varMap;

        Cudd& fullMngr = flat.getFullFnMgr();

        int p = 0;
        
       for(unsigned i=0; i != inputs->size(); i++) {
            node_t* n = (*inputs)[i];
            if(varMap.find(n) == varMap.end()) {
                varMap[n] = fullMngr.bddVar(p++);
            }
        }
        // now words.
        for(unsigned i=0; i != word_inputs->size(); i++) {
            word_t* w = (*word_inputs)[i];
            for(word_t::iterator it = w->begin(); it != w->end(); it++) {
                node_t* n = *it;
                if(varMap.find(n) == varMap.end()) {
                    varMap[n] = fullMngr.bddVar(p++);
                }
            }
        }
	nodeset_t cone;
	for (nodelist_t::iterator inpit = inputs->begin(); inpit != inputs->end(); inpit++) cone.insert(*inpit);
	for (wordlist_t::iterator it = word_inputs->begin(); it != word_inputs->end(); it++) {
		for (word_t::iterator wit = (*it)->begin(); wit != (*it)->end(); wit++) cone.insert(*wit);
	}

        for(unsigned i=0; i != outputs->size(); i++) {
            node_t* n = (*outputs)[i];
            if(varMap.find(n) == varMap.end() && (n->get_type() != node_t::LATCH) ) {
		nodeset_t visited;
		nodeset_t inputs;
		//node_t nd = *n;
		cand_mod->find_inputs(n, cone, visited, inputs);
		//cout<<"inputs.size() = "<<inputs.size()<<endl;
		if (inputs.size()<20){
		    BDD b = cand_mod->createFullFunction(fullMngr, varMap, n); 
		}
            }

        }
        for(unsigned i=0; i != word_outputs->size(); i++) {
            //cout<<"FOR i = "<<i<<" , size = "<<word_outputs->size()<<endl;
            word_t* w = (*word_outputs)[i];
            //int m = 0;
	    for(word_t::iterator it = w->begin(); it != w->end(); it++) {
		//cout<<"word node m = "<<m<<" , size = "<<w->size()<<endl; m++;
		node_t* n = *it;
		if(varMap.find(n) == varMap.end() && (n->get_type() != node_t::LATCH) ) {
		    //cout<<"createfullfn word outp start , node_type = "<<n->get_type()<<endl;

		    nodeset_t visited;
		    nodeset_t inputs;
		    //node_t nd = *n;
		    cand_mod->find_inputs(n, cone, visited, inputs);
		    //cout<<"inputs.size() = "<<inputs.size()<<endl;
		    if (inputs.size()<20){
			BDD b = cand_mod->createFullFunction(fullMngr, varMap, n);
			//                    printBDD(stdout,b);
		    };
		    //cout<<"createfullfn word outp end"<<endl;
		}
	    }
	}

        //int k = 0;
	for (std::map<node_t*, BDD>::iterator it=varMap.begin() ; it != varMap.end(); it++) {//cout<<"k = "<<k<<" , size = "<<varMap.size()<<endl; k++;
	    if(cand_mod->is_output(it->first)) {
		if (it->second.SupportSize() > 1) {
		    BDD bdd = it->second;
		    BDDpair* bddpair = new BDDpair(bdd, it->first, cand_mod);
		    bdd_set->insert(bddpair);
		    //cout<<"l = "<<l<<" , sizebddset = "<<bdd_set->size()<<endl; 
		    //l++; printBDD(stdout,it->second);cout<<"map size = "<<varMap.size()<<endl;
		};
	    }
	};
        varMap.clear();
//        for (set<BDD, bdd_compare_t>::iterator its= bdd_set->begin(); its != bdd_set->end(); its++) { BDD b1 = *its; printBDD(stdout,b1);};
        //cout<<"create_cand_mod_bdds end"<<endl;
        for (set<BDDpair*>::iterator it = bdd_set->begin(); it != bdd_set->end(); it++) assert((*it)->ORIGINAL.SupportSize()>1);
        return bdd_set;
};



void lib_match(flat_module_t& f, vector<aggr::id_module_t*>* cand_modules){

     map<aggr::id_module_t*, set<BDDpair*>*> map_all_cand_mod_bdds;
     map<signature_BDDs*, map<BDDpair*, set<aggr::id_module_t*>*>*> map_lib_match;

     //cout<<"cand modules size = "<<cand_modules->size()<<endl;
     set<BDD, bdd_compare_t>* set_test = new set<BDD, bdd_compare_t>;
        int ch = 0;

     for(size_t i =0; i < cand_modules->size(); i++){
        //cout<<"i = "<<i<<", size = "<<cand_modules->size()<<endl;

        set<BDDpair*>* cand_mod_bdds = create_cand_mod_bdds (f, (*cand_modules)[i]);
        if (cand_mod_bdds->size()) ch++;
        if (ch == 1){
           //cout<<"populate set test size: "<<cand_mod_bdds->size()<<endl;
           //for (set<BDDpair*>::iterator testit = cand_mod_bdds->begin(); testit != cand_mod_bdds->end(); testit++) 
             //  { cout<<"next bdd"<<endl; printBDD(stdout, (*testit)->ORIGINAL); set_test->insert((*testit)->ORIGINAL);};
        };
        //if (ch == 1) {int kr; cin>>kr;};
        //cout<<"Error here?"<<endl;
        //for (set<BDD, bdd_compare_t>::iterator itb = cand_mod_bdds->begin(); itb != cand_mod_bdds->end(); itb++){
        //    cout<<"size = "<<cand_mod_bdds->size()<<endl; 
        //    BDD bdd = *itb;
        //    cout<<"bdd pass"<<endl; 
        //    printBDD(stdout,*itb);
        //    cout<<"next"<<endl;
        //};
        //cout<<"created can_mod_bdds, i = "<<i<<", cand_mod_size = "<<cand_modules->size()<<" , bdd set size = "<<cand_mod_bdds->size()<<" : ";
        if (cand_mod_bdds->size()){
           for (set<BDDpair*>::iterator it = cand_mod_bdds->begin(); it != cand_mod_bdds->end(); it++){
               //cout<<"size = "<<(*it)->ORIGINAL.SupportSize()<<" , ";
               assert((*it)->ORIGINAL.SupportSize()>1);
           };//cout<<endl;
           int max_vars_axcepted = 20;
           set<BDDpair*>* reordered_cand_mod_bdds = new set<BDDpair*>;
           for (set<BDDpair*>::iterator itb = cand_mod_bdds->begin(); itb != cand_mod_bdds->end(); itb++){
               if ((*itb)->ORIGINAL.SupportSize() <= max_vars_axcepted){
                  //for (set<BDD, bdd_compare_t>::iterator it = cand_mod_bdds->begin(); it != cand_mod_bdds->end(); it++){
                      //cout<<"for "<<endl;
                      //printBDD(stdout, (*itb)->ORIGINAL);
                      BDD cmb = (*itb)->ORIGINAL;
                      assert(cmb.SupportSize()>1);
                      bool print = FALSE;
                      if (ch == 1) print = FALSE;
                      BDD b = create_bdd_sgnt_order(cmb, print);
                      //cout<<"bdd size "<<b.SupportSize()<<endl;
                      assert(b.SupportSize()>1);
                      (*itb)->PERMUTED = b;
                      //if (i==0){
                      //   cout<<"original: "; printBDD(stdout, (*itb)->ORIGINAL);
                      //   cout<<"reordered: "; printBDD(stdout, b);
                      //};
                      reordered_cand_mod_bdds->insert(*itb);
                  //};
               };
           };
           if (!reordered_cand_mod_bdds->size()) delete(reordered_cand_mod_bdds);
           assert(map_all_cand_mod_bdds.find((*cand_modules)[i]) == map_all_cand_mod_bdds.end());
           map_all_cand_mod_bdds[(*cand_modules)[i]] = reordered_cand_mod_bdds;

        };
        delete(cand_mod_bdds);
     };
     //std::map<signature_BDDs&,vector<aggr::id_module_t*>* map;
     //cout<<"a"<<endl; int i; cin>>i;
     //cout<<"map_classify_lib_bdds size = "<<map_classify_lib_bdds->size()<<endl;
     //for (map<BDD, set<signature_BDDs*>*, bdd_compare_t>::iterator i = map_classify_lib_bdds->begin(); i != map_classify_lib_bdds->end(); i++) printBDD(stdout,i->first);
     //cout<<"print set_test size = "<<set_test->size()<<endl;
     //for (set<BDD, bdd_compare_t>::iterator bit = set_test->begin(); bit != set_test->end(); bit++) printBDD(stdout, *bit);

     //cout<<"lib_match_start"<<endl;
     set<signature_BDDs*>* lib_bdds = build_library_signatures(f, set_test);
     //cout<<"SIZESIGNBDD = "<<lib_bdds->size();
     //cout<<"lib_sgnts built"<<endl;
     map<BDD, set<signature_BDDs*>*, bdd_compare_t>* map_classify_lib_bdds = classified_library_bdds(lib_bdds); 
     //cout<<"lib_bdds classified size = "<<map_classify_lib_bdds->size()<<endl;
     //int ik; cin>>ik;     
     int max = 0;
     for (std::map<BDD, set<signature_BDDs*>*, bdd_compare_t>::iterator it = map_classify_lib_bdds->begin(); it != map_classify_lib_bdds->end(); it++){
         if (it->first.SupportSize()>max) max = it->first.SupportSize();
         //printBDD(stdout,it->first);
         //cout<<"- size = "<<it->second->size()<<endl;
     };
     //cout<<"max support size of lib = "<<max<<endl;
     //int n; cin>>n;



     for (map<aggr::id_module_t*, set<BDDpair*>*>::iterator it  = map_all_cand_mod_bdds.begin(); it != map_all_cand_mod_bdds.end(); it++){
         //cout<<it->first->get_name()<<endl;
         //cout<<"set bdd size = "<<it->second->size();
         for (set<BDDpair*>::iterator itbdd = it->second->begin(); itbdd != it->second->end(); itbdd++){
             //printBDD(stdout,(*itbdd)->PERMUTED);
             vector<BDD> vb; vb.push_back((*itbdd)->PERMUTED); vb.push_back(!(*itbdd)->PERMUTED);
             for (size_t i = 0; i<2; i++){ 
                 map<BDD, set<signature_BDDs*>*, bdd_compare_t>::iterator found_sgnt = map_classify_lib_bdds->find(vb[i]);
                 if (found_sgnt != map_classify_lib_bdds->end()){
                    //cout<<"FOUND!!"<<endl; printBDD(stdout, found_sgnt->first);
                    set<signature_BDDs*>* set_sgnt = found_sgnt->second;
	            for (set<signature_BDDs*>::iterator itsgnt = set_sgnt->begin(); itsgnt != set_sgnt->end(); itsgnt++){ 
                        map<signature_BDDs*, map<BDDpair*, set<aggr::id_module_t*>*>*>::iterator found_sgnt_matched = map_lib_match.find(*itsgnt);
                        if (found_sgnt_matched == map_lib_match.end()){
                           map<BDDpair*, set<aggr::id_module_t*>*>* bdd_belongs_to = new map<BDDpair*, set<aggr::id_module_t*>*>;
                           set<aggr::id_module_t*>* set_id_mod = new set<aggr::id_module_t*>;
                           set_id_mod->insert(it->first);
                           (*bdd_belongs_to)[*itbdd] = set_id_mod;
                           map_lib_match[*itsgnt] = bdd_belongs_to;
                        }else{
                           map<BDDpair*, set<aggr::id_module_t*>*>::iterator found_bdd_matched = found_sgnt_matched->second->find(*itbdd);
                           if (found_bdd_matched != found_sgnt_matched->second->end()){
                              found_bdd_matched->second->insert(it->first);
                           }else{
                              set<aggr::id_module_t*>* set_id_mod = new set<aggr::id_module_t*>;
                              set_id_mod->insert(it->first);
                              (*found_sgnt_matched->second)[*itbdd] = set_id_mod;
                           };
                        };
                    };
                 };
              };
         };
     };

     cout<<"map_lib_match size = "<<map_lib_match.size()<<endl;

     if (!map_lib_match.size()) cout<<"No Matches Found"<<endl;
     std::map<aggr::id_module_t*,std::set<BDDpair*>*> map_all_matched_slices;

     for (map<signature_BDDs*, map<BDDpair*, set<aggr::id_module_t*>*>*>::iterator it = map_lib_match.begin(); it != map_lib_match.end(); it++){
         signature_BDDs* sb = it->first;
         cout<<sb->MODULE_NAME<<endl;
         std::map<BDDpair*, set<aggr::id_module_t*>*>* map = it->second;
         for (std::map<BDDpair*, set<aggr::id_module_t*>*>::iterator mit = map->begin(); mit != map->end(); mit++){
             BDD b = mit->first->PERMUTED;

             node_t* out = mit->first->OUTPUT;
             aggr::id_module_t* mod = mit->first->MODULE;

             std::string module_name_str(sb->MODULE_NAME);
             mod->add_matched_node(out, module_name_str);

             //cout<<"permtd: "; printBDD(stdout, b);
             b = mit->first->ORIGINAL;
             //cout<<"orig: "; printBDD(stdout, b);
             set<aggr::id_module_t*>* set = mit->second;
             //cout<<"candidate_modules: "<<endl;
             for (std::set<aggr::id_module_t*>::iterator sit = set->begin(); sit != set->end(); sit++){
                 //cout<<(*sit)->get_name()<<endl;
                 std::map<aggr::id_module_t*,std::set<BDDpair*>*>::iterator it_map_slices;
                 it_map_slices = map_all_matched_slices.find(*sit);
                 if (it_map_slices != map_all_matched_slices.end()) it_map_slices->second->insert(mit->first);
                 else{
                     std::set<BDDpair*>* setbddpair = new std::set<BDDpair*>;
                     setbddpair->insert(mit->first);
                     map_all_matched_slices[*sit] = setbddpair;
                 }; 
             };
         };
     };

     
     if (map_lib_match.size()) cout<<"print all matched slices in cand modules"<<endl;
     for (map<aggr::id_module_t*,set<BDDpair*>*>::iterator itallslices = map_all_matched_slices.begin(); itallslices != map_all_matched_slices.end(); itallslices++){
         //cout<<itallslices->first->get_name()<<endl;
         for (set<BDDpair*>::iterator itpair = itallslices->second->begin(); itpair != itallslices->second->end(); itpair++){
             //cout<<"PERMUTED: "; printBDD(stdout, (*itpair)->PERMUTED);
             //cout<<"ORIGINAL: "; printBDD(stdout, (*itpair)->ORIGINAL);
         };
     };
     


     for (map<BDD, set<signature_BDDs*>*, bdd_compare_t>::iterator it = map_classify_lib_bdds->begin(); it != map_classify_lib_bdds->end(); it++){
         set<signature_BDDs*>* temp = it->second;
         for (set<signature_BDDs*>::iterator its = temp->begin(); its != temp->end(); its++) (*its)->BDDs->clear();
         temp->clear();
     };
     map_classify_lib_bdds->clear();

     for (map<aggr::id_module_t*, set<BDDpair*>*>::iterator it = map_all_cand_mod_bdds.begin(); it != map_all_cand_mod_bdds.end(); it++) it->second->clear();
     map_all_cand_mod_bdds.clear();

     for (map<signature_BDDs*, map<BDDpair*, set<aggr::id_module_t*>*>*>::iterator it = map_lib_match.begin(); it != map_lib_match.end(); it++) it->second->clear();
     map_lib_match.clear();
     for (set<signature_BDDs*>::iterator it = lib_bdds->begin(); it != lib_bdds->end(); it++) (*it)->BDDs->clear();
     lib_bdds->clear();

};


#endif
