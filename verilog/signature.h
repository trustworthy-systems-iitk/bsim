#ifndef _SIGNATURE_
#define _SIGNATURE_

#include "cuddObj.hh"
#include <cuddInt.h>
#include <algorithm>
#include <vector>
#include <set>
#include <list>
#include "flat_module.h"

enum phase_t{
   NEG = -1,
   BIPOLAR = 0,
   POS = 1
};

struct variable{
   int VAR_IDX;
   vector<double>* SGNT;
   vector<double>* SGNT_LOCAL;
   phase_t PHASE;
public: 
     // ~variable(){cout<<"~~~~~variable"<<endl;};
};

struct bdd_compare_t {
   bool operator() (const BDD& one, const BDD& two) const {
        return one.getNode() < two.getNode();
   }
};

bool variable_sgnt_less_than (variable* lhs, variable* rhs){
     size_t lhsize = lhs->SGNT->size(), rhsize = rhs->SGNT->size();
     if (lhsize < rhsize) return TRUE;
     else if (lhsize > rhsize) return FALSE ;
     //lhsize == rhsize
     size_t i = 0;
     while (i < rhsize){
           if ((*lhs->SGNT)[i] == (*rhs->SGNT)[i]) i++;
           else return ((*lhs->SGNT)[i]<(*rhs->SGNT)[i]);
     };//while
     return FALSE;// all equal
};//end of variable_sgnt_less_than

bool variable_idx_less_than (variable* lhs, variable* rhs){
     return lhs->VAR_IDX < rhs->VAR_IDX;
};

bool variable_sgnt_less_than_local (variable* lhs, variable* rhs){  
     //size_t size = lhs->SGNT_LOCAL->size();
     if ((*lhs->SGNT_LOCAL)[0]<(*rhs->SGNT_LOCAL)[0]) return TRUE;
     else return FALSE;
};

bool variable_sgnt_equal (variable& lhs, variable& rhs){
     if (lhs.SGNT->size() != rhs.SGNT->size()) return FALSE;
     size_t size = lhs.SGNT->size();
     size_t i = 0;
     while (i < size){
           if ((*lhs.SGNT)[i] == (*rhs.SGNT)[i]) i++;
           else return FALSE;
     };//while
     return TRUE;
};

//queue<variable*> unique;
//queue<set<variable*>*> aliased;

class signature{
   vector<variable*>* _vars_unique;
   list<vector<variable*>*>* _vars_aliased;
public:
   signature (vector<variable*>* vars_unique, list<vector<variable*>*>* vars_aliased):
             _vars_unique(vars_unique),
             _vars_aliased(vars_aliased)
   { //cout<<"signature constractor start"<<endl;
     sort(_vars_unique->begin(), _vars_unique->end(), variable_sgnt_less_than);
     for (list<vector<variable*>*>::iterator it = _vars_aliased->begin(); it != _vars_aliased->end(); it++)
         sort ((*it)->begin(), (*it)->end(), variable_idx_less_than);
     //cout<<"signature constractor end"<<endl;
   };
   vector<variable*>* get_vars_unique() { return _vars_unique;};
   list<vector<variable*>*>* get_vars_aliased() { return _vars_aliased;};

};

bool signatures_equal (signature& lhs, signature& rhs){
     vector<variable*>* lhs_u = lhs.get_vars_unique();
     vector<variable*>* rhs_u = rhs.get_vars_unique();
     list<vector<variable*>*>* lhs_a = lhs.get_vars_aliased();
     list<vector<variable*>*>* rhs_a = rhs.get_vars_aliased();
     if (lhs_u->size() != rhs_u->size() || lhs_a->size() != rhs_a->size()) return FALSE;
     sort(lhs_u->begin(), lhs_u->end(), variable_sgnt_less_than);
     sort(rhs_u->begin(), rhs_u->end(), variable_sgnt_less_than);
     for (size_t i =0; i < lhs_u->size(); i++){
         if (!variable_sgnt_equal(*(*lhs_u)[i],*(*rhs_u)[i])) return FALSE;
     };
     vector<variable*> al_sgnt;
     for ( list<vector<variable*>*>::iterator j = lhs_a->begin(); j != lhs_a->end(); j++) 
         if ((*j)->size()){
            vector<variable*>* v =*j;
            al_sgnt.push_back((*v)[0]);
         };
     sort(al_sgnt.begin(),al_sgnt.end(), variable_sgnt_less_than);
     vector<variable*> al_sgnt_r;
     for ( list<vector<variable*>*>::iterator k = rhs_a->begin(); k != rhs_a->end(); k++) 
         if ((*k)->size()){
            vector<variable*>* v =*k;
            al_sgnt_r.push_back((*v)[0]);
         };
     sort(al_sgnt_r.begin(),al_sgnt_r.end(), variable_sgnt_less_than);

     for (size_t i =0; i < al_sgnt.size(); i++){
         if (!variable_sgnt_equal(*al_sgnt[i],*al_sgnt_r[i])) return FALSE;
     };
     return TRUE;
};

double minterm_count_of_cofactor (BDD& f, BDD& vars){
   BDD cofactor = f.Cofactor(vars);
   //cout<<"cofactor "; printBDD(stdout,cofactor);
   int support_size = cofactor.SupportSize();
   return cofactor.CountMinterm(support_size);
}

void generate_sgnt (variable* var_unique, variable* var_aliased, BDD& f, map<variable*,phase_t>& bipolars){
     //cout<<"generate_sgnt start"<<endl;
     Cudd* cudd_manager = f.manager();
     if (var_unique == NULL){
        BDD pos = cudd_manager->bddVar(var_aliased->VAR_IDX);
        double minterm_count_pos = minterm_count_of_cofactor(f, pos);
        BDD neg = !(cudd_manager->bddVar(var_aliased->VAR_IDX));
        double minterm_count_neg = minterm_count_of_cofactor(f, neg);

        //cout<<"minterm_count pos = "<<minterm_count_pos<<" , neg = "<<minterm_count_neg<<endl;
        //var_aliased->SGNT = new vector<int>;
        int minterm_count;
        phase_t phase;
        if (minterm_count_pos > minterm_count_neg) { 
           minterm_count = minterm_count_pos; phase = POS;
        }else if (minterm_count_pos < minterm_count_neg) {
           minterm_count = minterm_count_neg; phase = NEG;
        }else{ minterm_count = minterm_count_pos; phase = BIPOLAR; };
        var_aliased->SGNT->push_back(minterm_count);
        var_aliased->PHASE = phase;
        if (phase == BIPOLAR) bipolars[var_aliased] = BIPOLAR;
        //cout<<"gen_sgnt phase = "<<phase<<" , minterm_count = "<<minterm_count<<endl;
     }else{
        //cout<<"var unique IDX = "<<var_unique->VAR_IDX<<" , var_aliased IDX = "<<var_aliased->VAR_IDX<<endl;
        BDD var_alias_phased, var_unique_phased;
        if (var_unique->PHASE == NEG) var_unique_phased = !(cudd_manager->bddVar(var_unique->VAR_IDX));
        else var_unique_phased = cudd_manager->bddVar(var_unique->VAR_IDX);
        switch (var_aliased->PHASE)
        {
        case POS : var_alias_phased = cudd_manager->bddVar(var_aliased->VAR_IDX);
                   break;
        case NEG : var_alias_phased = !(cudd_manager->bddVar(var_aliased->VAR_IDX));
                   break;
        case BIPOLAR : break;
        };

        if (var_aliased->PHASE != BIPOLAR){ //cout<<"! bipolar"<<endl;
           assert(bipolars.find(var_aliased) == bipolars.end());
           BDD couple = var_alias_phased * var_unique_phased;
           double minterm_count_couple = minterm_count_of_cofactor(f, couple);
           var_aliased->SGNT_LOCAL->clear();
           var_aliased->SGNT_LOCAL->push_back(minterm_count_couple);
        }else{//cout<<"aliased bipolar"<<endl;
           BDD couple_pos = cudd_manager->bddVar(var_aliased->VAR_IDX) * var_unique_phased;
           BDD couple_neg = !(cudd_manager->bddVar(var_aliased->VAR_IDX)) * var_unique_phased;
           double minterm_count_couple_pos = minterm_count_of_cofactor(f, couple_pos);
           double minterm_count_couple_neg = minterm_count_of_cofactor(f, couple_neg);
           var_aliased->SGNT_LOCAL->clear();
           //cout<<"minterm_count_couple_pos = "<<minterm_count_couple_pos<<" , neg = "<<minterm_count_couple_neg<<endl;
           if (minterm_count_couple_pos == minterm_count_couple_neg) {
              var_aliased->SGNT_LOCAL->push_back(minterm_count_couple_pos);
              assert(bipolars.find(var_aliased) != bipolars.end());
           }else{
              if (minterm_count_couple_pos > minterm_count_couple_neg){
                 var_aliased->PHASE = POS;
                 var_aliased->SGNT_LOCAL->push_back(minterm_count_couple_pos);
              }else{
                 var_aliased->PHASE = NEG;
                 var_aliased->SGNT_LOCAL->push_back(minterm_count_couple_neg);
              }; 
              bipolars.erase(var_aliased);
           };
        };
     };
     //cout<<"vars unique size = "<<vars_unique->size()<<" , vars aliased size = "<<vars_aliased->size()<<endl;
     //cout<<"generate_sgnt end"<<endl;
};


void make_sgnt_global (variable* var){
     var->SGNT->push_back((*var->SGNT_LOCAL)[0]);
     var->SGNT_LOCAL->clear();
};

BDD normalize_vars (BDD& f){
     //cout<<"norm_Vars start"<<endl;
     //printBDD(stdout, f);
     int support_size = f.SupportSize();
     DdNode* node_f = f.getNode();
     Cudd* manager_cudd = f.manager();
     DdManager* manager = manager_cudd->getManager();
     int *supportInd = Cudd_SupportIndex(manager, node_f);
     int  nVars = Cudd_ReadSize(manager);
     int* perm = new int[nVars];
     int c = 0;
     for (int i = 0; i < nVars; i++){
         if (supportInd[i]) {
            perm[i] = c; c++;
         }else perm[i] = i;
         //cout<<"c = "<<c<<" , supportsize = "<<support_size<<", i = "<<i<<" nVars = "<<nVars<<endl;
     };
     assert(c == support_size );
     BDD b = f.Permute(perm);
     //printBDD(stdout, b);
     //cout<<"Norm Vars End"<<endl;
     return b;
};


vector<variable*>* initialize_variables (BDD& f){
     //cout<<"initialize_variables start"<<endl;
     //int support_size = f.SupportSize();
     DdNode* node_f = f.getNode();
     Cudd* manager_cudd = f.manager();
     DdManager* manager = manager_cudd->getManager();
     int *supportInd = Cudd_SupportIndex(manager, node_f);
     int  nVars = Cudd_ReadSize(manager);
     //int* perm = new int[nVars];
     //int c = 0;
     //for (int i = 0; i < nVars; i++){
     //    if (supportInd[i]) {
     //       perm[i] = c; c++;
     //    };
     //    perm[i] = i;
     //};
     //cout<<"nVar = "<<nVars<<endl;
     vector<variable*>* vector_vars = new vector<variable*>;
     for (int i = 0; i < nVars; i++){
        if (supportInd[i]) {
           variable* var = new variable;
           var->VAR_IDX = i;
           var->SGNT =  new vector<double>;
           var->SGNT_LOCAL = new vector<double>;
           vector_vars->push_back(var);
        };
     };
     //cout<<"initialize_variables end"<<endl;
     return vector_vars;
};


void init_signature_classes (BDD& f, vector<variable*>& vector_vars, vector<variable*>& vars_unique, list<vector<variable*>*>& vars_aliased, map<variable*,phase_t>& bipolars){
     //cout<<"init_signature_classes start"<<endl;
     vector<variable*>::iterator it, curr, next;
     for (it = vector_vars.begin(); it != vector_vars.end(); it++)
         generate_sgnt (NULL, *it, f, bipolars);
     sort (vector_vars.begin(), vector_vars.end(), variable_sgnt_less_than);
     curr = vector_vars.begin();
     next = vector_vars.begin();
     //cout<<"nVar = "<<vector_vars.size();
     //printBDD(stdout, f);
     while (next != vector_vars.end()){
           int count = 0;
            while (next != vector_vars.end() && (*(*curr)->SGNT)[0] == (*(*next)->SGNT)[0]){
                  count++;
                  next++;
            };
            if (count == 1) { vars_unique.push_back(*curr); //cout<<"vars_unique_push"<<endl;
            }else {
                 vector<variable*>* new_vector = new vector<variable*>;
                 for (vector<variable*>::iterator it = curr; it != next; it++) new_vector->push_back(*it);
                 sort (new_vector->begin(), new_vector->end(), variable_sgnt_less_than);
                 vars_aliased.push_back(new_vector);
                 //cout<<"vars_aliased_pushed; new_vector->size =  "<<new_vector->size()<<endl;
            };
            curr = next;
     };
     //cout<<"vars_unique.size() = "<<vars_unique.size()<<endl;
     //cout<<"vars_aliased.size() = "<<vars_aliased.size()<<endl;
     //cout<<"init_signature_classes end"<<endl;
};

void split_sgnt_class (vector<variable*>& vars_unique, list<vector<variable*>*>& vars_aliased, list<vector<variable*>*>::iterator& vector_to_split_it){
     //cout<<"split_sgnt_class start"<<endl;
     vector<variable*>::iterator it, curr, next;
     vector<variable*>* vector_to_split = *vector_to_split_it;
     curr = vector_to_split->begin();
     next = vector_to_split->begin();
     if ((*(*vector_to_split->begin())->SGNT_LOCAL)[0] != (*(*vector_to_split->rbegin())->SGNT_LOCAL)[0]){
        while (next != vector_to_split->end()){
            int count = 0;
            while (next != vector_to_split->end() && (*(*curr)->SGNT_LOCAL)[0] == (*(*next)->SGNT_LOCAL)[0]){
                  count++;
                  next++;
            };
            if (count == 1){
               make_sgnt_global (*curr);
               vars_unique.push_back(*curr); //cout<<"unique push back idx = "<<(*curr)->VAR_IDX<<endl;
            }else{
                 vector<variable*>* new_vector = new vector<variable*>;
                 for (it = curr; it != next; it++){
                     make_sgnt_global(*it);
                     new_vector->push_back(*it);
                 };
                 vars_aliased.push_back(new_vector); //cout<<"aliased push back"<<endl;
            };
            curr = next;
       };
       vector_to_split_it = vars_aliased.erase(vector_to_split_it);
    };
    //for (vector<variable*>::iterator it = vars_unique.begin(); it != vars_unique.end(); it++) cout<<"idx = "<<(*it)->VAR_IDX; cout<<endl;
    //cout<<"vars_unique.size() = "<<vars_unique.size()<<endl;
    //cout<<"vars_aliased.size() = "<<vars_aliased.size()<<endl;
    //cout<<"split_sgnt_class end"<<endl;
};

void classify_signatures (BDD& f, vector<variable*>& vars_unique, list<vector<variable*>*>& vars_aliased, map<variable*,phase_t>& bipolars){
     //cout<<"classify_signatures start"<<endl;
     //cout<<"vars_unique size = "<<vars_unique.size()<<" , vars_aliased size = "<<vars_aliased.size()<<" , bipolars size = "<<bipolars.size()<<endl;
     //vector<variable*>::iterator curr = vars_unique.begin();
     size_t curr = 0;
     list<vector<variable*>*>::iterator last_aliased_vector;
     list<vector<variable*>*>::iterator it, it1;
     //int i = 0, j = 0;
//     while (curr != vars_unique.end()){ cout<<" unique var i = "<<i<<endl; i++;
     while (curr != vars_unique.size()){ //cout<<" unique var i = "<<i<<" , curr = "<<curr<<" , size = "<<vars_unique.size()<<endl; i++;
           //cout<<"vars_unique_IDX = "<<vars_unique[curr]->VAR_IDX<<endl;
           last_aliased_vector = vars_aliased.end();
           it = vars_aliased.begin(); //cout<<"*it = "<<*it<<" , *rbegin = "<<" - "<<*vars_aliased.rbegin()<<endl;
           while (it != last_aliased_vector){//cout<<"aliased var j = "<<j<<" , size = "<<vars_aliased.size()<<endl; cout<<" , vector size = "<<(*it)->size()<<endl; j++;
                 for (vector<variable*>::iterator var_it = (*it)->begin(); var_it != (*it)->end(); var_it++){
                     //cout<<"f"; 
                     generate_sgnt (vars_unique[curr], *var_it, f, bipolars);
                 };//cout<<"end for"<<endl;
                 sort ((*it)->begin(), (*it)->end(), variable_sgnt_less_than_local);
                 //cout<<"vars_unique.size() = "<<vars_unique.size();
                 //cout<<" , vars_aliased.size() = "<<vars_aliased.size()<<endl;
                 it1 = it; it1++;
                 //if (it1 == last_aliased_vector) cout<<"true"<<endl; else cout<<"false"<<endl;
                 split_sgnt_class (vars_unique, vars_aliased, it);
                 //cout<<"2 : ";   for (vector<variable*>::iterator it2 = vars_unique.begin(); it2 != vars_unique.end(); it2++) cout<<"idx = "<<(*it2)->VAR_IDX; cout<<endl;
                 it = it1;
                 //cout<<"*it = "<<*it<<" , end = "<<*last_aliased_vector<<endl;
           };

//           if ( it == last_aliased_vector){
//              cout<<"== case aliased var j = "<<j<<endl; j++;
//              for (vector<variable*>::iterator var_it = (*it)->begin(); var_it != (*it)->end(); var_it++){
//                  generate_sgnt (*curr, *var_it, f, bipolars);
//              };cout<<"end for"<<endl;
//              sort ((*it)->begin(), (*it)->end(), variable_sgnt_less_than_local);
//              split_sgnt_class (vars_unique, vars_aliased, it);
//           };
           //cout<<"3 : ";   for (vector<variable*>::iterator it2 = vars_unique.begin(); it2 != vars_unique.end(); it2++) cout<<"idx = "<<(*it2)->VAR_IDX; cout<<endl;

           curr++;
     };
     //cout<<"classify_signatures end"<<endl;
};



bool compare_signatures (signature& s1, signature& s2){
     vector<variable*>* s1_vars_unique = s1.get_vars_unique(), *s2_vars_unique = s2.get_vars_unique();
     list<vector<variable*>*>* s1_vars_aliased = s1.get_vars_aliased(), *s2_vars_aliased = s2.get_vars_aliased();
     if (s1_vars_unique->size() != s2_vars_unique->size() || s1_vars_aliased->size() != s2_vars_aliased->size()) return FALSE;
     list<vector<variable*>*>::iterator it_s1 = s1_vars_aliased->begin(), it_s2 = s2_vars_aliased->begin();
     while (it_s1 != s1_vars_aliased->end()){
           if ((*it_s1)->size() != (*it_s2)->size()) return FALSE;
           it_s1++; it_s2++;
     };
     size_t i = 0;
     size_t vars_unique_size = s1_vars_unique->size();
     //size_t vars_aliased_size = s1_vars_aliased->size();
     while (i < vars_unique_size){
	   if (!variable_sgnt_equal(*(*s1_vars_unique)[i],*(*s2_vars_unique)[i])) return FALSE;
     };
     it_s1 = s1_vars_aliased->begin(), it_s2 = s2_vars_aliased->begin();
     while (it_s1 != s1_vars_aliased->end()){
           size_t i = 0;
           while (i < (*it_s1)->size()){
                 if ((*it_s1)[i] == (*it_s2)[i]) i++;
                 else return FALSE; 
           };
           it_s1++; it_s2++;
     };
     return TRUE;
};


struct signature_BDDs{
       signature* SGNT;
       set<BDD, bdd_compare_t>* BDDs;
       const char* MODULE_NAME;
};

void process_unique_perm (vector<variable*>& vars_unique, int** perm, int& first_al){
     //cout<<"process_unique_perm start size = "<<vars_unique.size()<<endl;
     vector<int> idxs;
     for (size_t k = 0; k<vars_unique.size(); k++) { idxs.push_back(vars_unique[k]->VAR_IDX);};// cout<<" k = "<<k<<" , var_idx = "<<idxs[k]<<" , ";};cout<<endl;
     //sort(idxs.begin(), idxs.end());
     for (size_t i = 0; i < vars_unique.size(); i++){
         (*perm)[idxs[i]] = i;
         first_al++;
         //cout<<"perm["<<idxs[i]<<"]="<<(*perm)[idxs[i]]<<" ; ";
     }; //cout<<endl;
     //first_al = vars_unique.size();
     //cout<<"process_unique_perm end"<<endl;
};

void process_alias_perm (list<vector<variable*>*>& vars_aliased, int** perm, int first_al){
     //cout<<"process_alias_perm start"<<endl;
     size_t i = 0;
     vector<int> aliased_idxs;
     for (list<vector<variable*>*>::iterator lit = vars_aliased.begin(); lit != vars_aliased.end(); lit++){
         for (vector<variable*>::iterator vit = (*lit)->begin(); vit != (*lit)->end(); vit++) aliased_idxs.push_back((*vit)->VAR_IDX);
     };
     //cout<<"aliased idxs ";
     //for (size_t j = 0; j < aliased_idxs.size(); j++) cout<<aliased_idxs[j]<<" , "; cout<<endl;
     //for (list<vector<variable*>*>::iterator it = vars_aliased.begin(); it != vars_aliased.end(); it++){
         //for (vector<variable*>::iterator it_var = (*it)->begin(); it_var != (*it)->end(); it_var++){
             //perm[end+i] = (*it_var)->VAR_IDX;
            // assert(i<aliased_idxs.size());
             //perm[aliased_idxs[i] = (*it_var)->VAR_IDX;
             //cout<<"perm["<<end+i<<"]="<<perm[end+i]<<" ; ";
            // cout<<"perm["<<aliased_idxs[i]<<"]="<<perm[aliased_idxs[i]]<<" ; ";
            // i++;
         //};
     //end = end+i;
 //    };cout<<endl;
     for (list<vector<variable*>*>::iterator it = vars_aliased.begin(); it != vars_aliased.end(); it++){
         for (vector<variable*>::iterator it_var = (*it)->begin(); it_var != (*it)->end(); it_var++){
             //for (size_t i = 0; i < aliased_idxs.size(); i++){
                 (*perm)[aliased_idxs[i]]=first_al;
                 //cout<<"perm["<<aliased_idxs[i]<<"] = "<<(*perm)[aliased_idxs[i]]<<" ,";
              //};
                 i++; first_al++;
         };
      };//cout<<endl;
      //cout<<"process_alias_perm end"<<endl;
};


void init_bipolar_phases (map<variable*,phase_t>& bipolars){
     //cout<<"init_bipolar_phases start"<<endl;
     for (map<variable*,phase_t>::iterator it = bipolars.begin();it != bipolars.end(); it++) it->second = POS;
     //cout<<"init_bipolar_phases end"<<endl;
};

bool nextPhase (map<variable*,phase_t>& bipolars) {
  //cout<<"nextPhase start"<<endl; 
  map<variable*,phase_t>::iterator it = bipolars.begin();
  //cout<<"bipolars.size = "<<bipolars.size();
  while (it != bipolars.end()) {
    if ( it->second == NEG) {  // sanam a[k] gasulia boloshi
       //cout<<"was pos change to neg"<<endl;
       it->second = POS; it++;
    } else {//cout<<"was neg change to pos"<<endl; 
       it->second = NEG; break;};
  }; // a-shi dagenerirda morigi permut 
  //cout<<"nextPhase end"<<endl;
  return it != bipolars.end();  // tu k = nA mashin kvela phase amoturulia
};

bool nextTotalPermut (list<vector<variable*>*>& vars_aliased){
     //cout<<"nextTotalPermut start"<<endl;
     list<vector<variable*>*>::iterator it = vars_aliased.begin();
     if (!vars_aliased.size()) return FALSE;
     //cout<<"begin.size = "<<(*it)->size()<<endl;
     //for (vector<variable*>::iterator itv = (*it)->begin(); itv != (*it)->end(); itv++) cout<<(*itv)->VAR_IDX<<" , ";cout<<endl;
     while (it != vars_aliased.end() && !next_permutation((*it)->begin(), (*it)->end(), variable_idx_less_than)){
           it++; //cout<<"it++"<<endl;
     };
     //cout<<"nextTotalPermut end"<<endl;
     return it != vars_aliased.end();
};


BDD create_bdd_sgnt_order (BDD& f0, bool print){
     if (print) cout<<"create_bdd_sgnt_order start"<<endl;
     //printBDD(stdout, f0);
     Cudd* cudd_manager = f0.manager();
     DdManager* manager = cudd_manager->getManager();
     int nVars = Cudd_ReadSize(manager);
     BDD f = f0;
     f = normalize_vars(f);
     //printBDD(stdout, f);
     vector<variable*>* vector_vars = initialize_variables (f);
     vector<variable*>* vars_unique = new vector<variable*>;
     list<vector<variable*>*>* vars_aliased = new list<vector<variable*>*>;
     map<variable*,phase_t>* bipolars = new map<variable*,phase_t>;
     init_signature_classes (f, *vector_vars, *vars_unique, *vars_aliased, *bipolars);
     classify_signatures (f, *vars_unique, *vars_aliased, *bipolars);
     if (print){
        cout<<"SET order ------------- ";
        cout<<"vars unique idxs: ";
        for (vector<variable*>::iterator vit = vars_unique->begin(); vit != vars_unique->end(); vit++) cout<<(*vit)->VAR_IDX<<" - "<<(*vit)->PHASE<<" ,"; cout<<endl;
        for (list<vector<variable*>*>::iterator it = vars_aliased->begin(); it != vars_aliased->end(); it++){
            cout<<(*it)->size()<<" : ";
            for (vector<variable*>::iterator vit = (*it)->begin(); vit != (*it)->end(); vit++) cout<<(*vit)->VAR_IDX<<" - "<<(*vit)->PHASE<<" , ";
            cout<<" | ";
        };
        cout<<"bipolars "<<bipolars->size()<<" : ";
        for (map<variable*,phase_t>::iterator it = bipolars->begin(); it != bipolars->end(); it++) cout<<it->first->VAR_IDX<<" , "; cout<<endl;
     };
     signature* sgnt = new signature(vars_unique, vars_aliased);
     //int* perm = new int[vector_vars->size()];

     int* perm = new int[nVars];
     for (int i = 0; i <nVars; i++) perm[i] = i;
     int first_al = 0;
     vector<int> aliased_idxs;
     for (list<vector<variable*>*>::iterator lit = vars_aliased->begin(); lit != vars_aliased->end(); lit++){
         for (vector<variable*>::iterator vit = (*lit)->begin(); vit != (*lit)->end(); vit++) aliased_idxs.push_back((*vit)->VAR_IDX);
     };
     sort (aliased_idxs.begin(), aliased_idxs.end());
     if (print){
        cout<<"vars unique idxs: ";
        for (vector<variable*>::iterator vit = vars_unique->begin(); vit != vars_unique->end(); vit++) cout<<(*vit)->VAR_IDX<<" , "; cout<<endl;
        cout<<"vector aliased idxs = ";
        for (vector<int>::iterator it = aliased_idxs.begin(); it != aliased_idxs.end(); it++) cout<<*it<<" ,";cout<<endl;
        for (int i = 0; i <nVars; i++) cout<<"perm["<<i<<"] = "<<perm[i]<<" ,"; cout<<endl;
     };
     process_unique_perm (*vars_unique, &perm, first_al);
     process_alias_perm(*vars_aliased, &perm, first_al);
     if (print) { cout<<"final : "; for (int i = 0; i <nVars; i++) cout<<"perm["<<i<<"] = "<<perm[i]<<" ,"; cout<<endl;};
     BDD b = f;
      if (print) printBDD(stdout, b);
     b = b.Permute(perm);
     if (print) {cout<<"b permuted  = "; printBDD(stdout,b);}
     for (vector<variable*>::iterator vit = vector_vars->begin(); vit != vector_vars->end(); vit++){
         //cout<<(*vit)->VAR_IDX<<" - "<<(*vit)->PHASE<<" ,";
         if ((*vit)->PHASE == NEG){
            int idx = (*vit)->VAR_IDX;
            BDD neg = !(cudd_manager->bddVar(idx));
            //cout<<"NegVar compose start: "<<idx<<endl;
            b = b.Compose(neg, idx); //cout<<"b..: "; printBDD(stdout,b);
            //cout<<"NegVar compose end"<<endl;
          };
      };
      //cout<<"testing b should be recalc: ";printBDD(stdout, b);
     //init_bipolar_phases (*bipolars);
     //for (vector<variable*>::iterator vit = vector_vars->begin(); vit != vector_vars->end(); vit++){
     //    if ((*vit)->PHASE == NEG){
     //       int idx = (*vit)->VAR_IDX;
     //       BDD neg = !(cudd_manager->bddVar(idx));
     //       b = b.Compose(neg, idx);
     //    };
     //};
     if (print) {cout<<"b permuted, recalculated: "; printBDD(stdout, b);};
     delete(sgnt);
     if (print) cout<<"create_bdd_sgnt_order end"<<endl;
     return b;
};

signature_BDDs* create_all_bdds_sgnt (BDD& f){
     //cout<<"create all bdds sgnt"<<endl;
     //printBDD(stdout,f);
     Cudd* cudd_manager = f.manager();
     DdManager* manager = cudd_manager->getManager();
     int nVars = Cudd_ReadSize(manager);
    // Cudd mgr;
     //BDD a1 = cudd_manager->bddVar(0);
     //BDD b1 = cudd_manager->bddVar(1);
     //BDD x0 = cudd_manager->bddVar(0);
     //BDD x1 = cudd_manager->bddVar(1);
     //BDD x2 = cudd_manager->bddVar(2);
     //BDD x3 = cudd_manager->bddVar(3);
     //BDD x4 = cudd_manager->bddVar(4);
     //BDD x5 = cudd_manager->bddVar(5);
     //int* p1 = new int[nVars];
     //for (int i = 0; i <nVars; i++) p1[i] = i;
     //BDD a = mgr.bddVar(0);
     //BDD b = mgr.bddVar(1);
     //BDD bxor1 = a1 + !b1;
     //BDD bxor2 = !(a1 * (b1 + (x2 + (x3 + (x4 + (x5))))) + !a1 * ((b1 + (x3 + (x4 + (x5)))) + !b1));
     //BDD bxor2 = !(x0 * (x1 + (x2 + (x3 + (x4 + (x5))))) + !x0 * ((x2 + (x3 + (x4 + (x5)))) + !x1));
     //BDD b3 = f;
     //BDD b4 = bxor2;
     //BDD bxor = a + !b;
     //DdManager* manager1 = mgr.getManager();
     //int ms = Cudd_ReadSize(manager1);
     //p1[0] = 1; p1[1] = 0;
     //cout<<"bxor2: ";printBDD(stdout,bxor2);
     //bxor2 = bxor2.Permute(p1);
     //cout<<"b3: ";printBDD(stdout,b3);
     //cout<<"b4: ";printBDD(stdout,b4);
     //printBDD(stdout,bxor1);
     //bxor1 = bxor1.Permute(p1); f = f.Permute(p1); b3 = b3.Permute(p1); b4 = b4.Permute(p1); cout<<"1,0"<<endl;
     //printBDD(stdout,bxor1);cout<<"bxor2:";
     //printBDD(stdout,bxor2);
     //cout<<"b3: ";printBDD(stdout,b3);
     //cout<<"b4: ";printBDD(stdout,b4);
//printBDD(stdout,f);
     //p1[0] = 1; p1[1] = 0;
     //bxor1 = bxor1.Permute(p1);cout<<"0,1"<<endl;
     //printBDD(stdout,bxor1); 
//     int* p = new int[ms];
//     for (int i = 2; i < ms; i++) p[i]=i;

    //int* p = new int[3];

     //p[0] = 1; p[1] = 0; p[2] = 0;
     //printBDD(stdout,bxor); 
     //bxor = bxor.Permute(p);
     //printBDD(stdout,bxor);
     //p[0] = 1; p[1] = 0; p[2] = 1;
//cout<<"ms = "<<ms<<" p = "<<endl;
//for (int i = 0; i <ms; i++) cout<<"p["<<i<<"] = "<<p[i]<<" , "; cout<<endl;

     //bxor = bxor.Permute(p);
     //printBDD(stdout,bxor); 
     vector<variable*>* vector_vars = initialize_variables (f);
     vector<variable*>* vars_unique = new vector<variable*>;
     list<vector<variable*>*>* vars_aliased = new list<vector<variable*>*>;
     map<variable*,phase_t>* bipolars = new map<variable*,phase_t>;
     //size_t nVars = vector_vars->size();
     //cout<<"nVar = "<<nVars<<endl;
     init_signature_classes (f, *vector_vars, *vars_unique, *vars_aliased, *bipolars);
     classify_signatures (f, *vars_unique, *vars_aliased, *bipolars);
     //cout<<"SET ";
     //   cout<<"vars unique idxs: ";
     //   for (vector<variable*>::iterator vit = vars_unique->begin(); vit != vars_unique->end(); vit++) cout<<(*vit)->VAR_IDX<<" - "<<(*vit)->PHASE<<" ,"; cout<<endl;
     //for (list<vector<variable*>*>::iterator it = vars_aliased->begin(); it != vars_aliased->end(); it++){
     //    cout<<(*it)->size()<<" : ";
     //    for (vector<variable*>::iterator vit = (*it)->begin(); vit != (*it)->end(); vit++) cout<<(*vit)->VAR_IDX<<" - "<<(*vit)->PHASE<<" , ";
     //    cout<<" | ";
     //};
     //cout<<"bipolars "<<bipolars->size()<<" : ";
     //for (map<variable*,phase_t>::iterator it = bipolars->begin(); it != bipolars->end(); it++) cout<<it->first->VAR_IDX<<" , "; cout<<endl;


     signature* sgnt = new signature(vars_unique, vars_aliased);
     signature_BDDs* sgnt_bdds = new signature_BDDs;
     int* perm = new int[nVars];
     for (int i = 0; i <nVars; i++) perm[i] = i;
     int first_al = 0;
     vector<int> aliased_idxs;
     for (list<vector<variable*>*>::iterator lit = vars_aliased->begin(); lit != vars_aliased->end(); lit++){
         for (vector<variable*>::iterator vit = (*lit)->begin(); vit != (*lit)->end(); vit++) aliased_idxs.push_back((*vit)->VAR_IDX);
     };
     sort (aliased_idxs.begin(), aliased_idxs.end());
     //cout<<"vector aliased idxs = ";
     //for (vector<int>::iterator it = aliased_idxs.begin(); it != aliased_idxs.end(); it++) cout<<*it<<" ,";cout<<endl;
     //for (int i = 0; i <nVars; i++) cout<<"perm["<<i<<"] = "<<perm[i]<<" ,"; cout<<endl;
     process_unique_perm (*vars_unique, &perm, first_al);
     process_alias_perm(*vars_aliased, &perm, first_al);
     //for (int i = 0; i <nVars; i++) cout<<"perm["<<i<<"] = "<<perm[i]<<" ,"; cout<<endl;
     set<BDD, bdd_compare_t>* bdds = new set<BDD, bdd_compare_t>;
     //cout<<"bdd to permute :"; printBDD(stdout, f);
     while (nextTotalPermut (*vars_aliased)){
           //cout<<"nextTotalPermute true"<<endl;
           for (list<vector<variable*>*>::iterator lit = vars_aliased->begin(); lit != vars_aliased->end(); lit++){
               for (vector<variable*>::iterator vit = (*lit)->begin(); vit != (*lit)->end(); vit++) aliased_idxs.push_back((*vit)->VAR_IDX);
            }; 
          BDD b = f;
           //cout<<"f = ";
           //printBDD(stdout,f);
           process_alias_perm (*vars_aliased, &perm, first_al);
           //cout<<"perm : ";
           //for (int p = 0; p < nVars; p++){
               //cout<<perm[p]<<" , ";
           //};cout<<endl; 
           b = f.Permute(perm);
           //cout<<"b permuted = ";
           //printBDD(stdout,b);
           for (vector<variable*>::iterator vit = vector_vars->begin(); vit != vector_vars->end(); vit++){
               //cout<<(*vit)->VAR_IDX<<" - "<<(*vit)->PHASE<<" ,";
               if ((*vit)->PHASE == NEG){
                  int idx = (*vit)->VAR_IDX;
                  BDD neg = !(cudd_manager->bddVar(idx));
                  //cout<<"NegVar compose start: "<<idx<<endl;
                  b = b.Compose(neg, idx); //cout<<"b..: "; printBDD(stdout,b);
                  //cout<<"NegVar compose end"<<endl;
               };
           };
           init_bipolar_phases (*bipolars);
           //cout<<"bipolars size = "<<bipolars->size()<<endl;
           //printBDD(stdout, b);
           if (!bipolars->size()) {/*cout<<"permuted recalculated: "; printBDD(stdout, b);*/ bdds->insert(b);};
           while (nextPhase(*bipolars)){//make bipolars' permutation of phases
                 //cout<<"nextPhase true"<<endl;
                 BDD c = b; //cout<<"c=b = ";printBDD(stdout,c);
                 for (map<variable*,phase_t>::iterator it = bipolars->begin(); it != bipolars->end(); it++){
                     if (it->second == NEG){
                        int idx = (it->first)->VAR_IDX;
                        BDD neg = !(cudd_manager->bddVar(idx));
                        //cout<<"phase compose start: "<<idx<<endl;
                        c = c.Compose(neg, idx); //cout<<"c: "; printBDD(stdout,c);
                        //cout<<"phase compose end"<<endl;
                     };
                 };
                 //cout<<"permuted recalculated bdd: "<<endl; printBDD(stdout,c);
                 bdds->insert(c);

           };
           //cout<<"nextPhase false"<<endl;
     };
     //cout<<"nextTotalPermute false"<<endl;
     sgnt_bdds->SGNT = sgnt;
     sgnt_bdds->BDDs = bdds;
     sgnt_bdds->MODULE_NAME = NULL;
     delete(perm);

     //cout<<" SET OF PERM BDDS"<<endl; for (set<BDD,bdd_compare_t>::iterator it = bdds->begin(); it != bdds->end(); it++) {cout<<"SET: "; printBDD(stdout,*it);};
     //cout<<"SET: ==================="<<endl;
     return sgnt_bdds;
};

set<signature_BDDs*>* build_library_signatures (flat_module_t& flat_module, set<BDD, bdd_compare_t>* set_test){
     set<signature_BDDs*>* sgnt_bdds_all = new set<signature_BDDs*>; 
     flat_module_list_t* library_modules =  flat_module.get_libraryElements();
     for (flat_module_list_t::iterator it = library_modules->begin(); it != library_modules->end(); it++){
         //cout<<"for build library signatures"<<endl;
         vector<BDD>* bdds = (*it)->get_outputBDDs();
         //for (vector<BDD>::iterator it_bdd = bdds->begin(); it_bdd != bdds->end(); it_bdd++){
         //    printBDD(stdout, *it_bdd);
         //};
         for (vector<BDD>::iterator it_bdd = bdds->begin(); it_bdd != bdds->end(); it_bdd++){
             signature_BDDs* sgnt_bdds = create_all_bdds_sgnt (*it_bdd);
             //cout<<"all_bdds created"<<endl;
             sgnt_bdds->MODULE_NAME = (*it)->get_module_name();
             sgnt_bdds_all->insert(sgnt_bdds);
             //cout<<"for--"<<endl;
         };
     };
     //cout<<"set_test size = "<<set_test->size()<<endl;
     for (set<BDD,bdd_compare_t>::iterator bit = set_test->begin(); bit != set_test->end(); bit++){
         BDD b = *bit;
         b = normalize_vars(b);
         signature_BDDs* sgnt_bdds = create_all_bdds_sgnt (b);
         //cout<<"create_all_bdds for: "; printBDD(stdout,*bit);
         //cout<<"normalized "; printBDD(stdout, b);
         //int kr; cin>>kr;
         //cout<<"all_bdds created"<<endl;
         sgnt_bdds->MODULE_NAME = "testm";
         sgnt_bdds_all->insert(sgnt_bdds);
     };
     return sgnt_bdds_all;
};

map<BDD, set<signature_BDDs*>*, bdd_compare_t>* classified_library_bdds (set<signature_BDDs*>* lib_sgnt){
//cout<<"classified_library_bdds start"<<endl;
     std::map<BDD, set<signature_BDDs*>*, bdd_compare_t>* map = new std::map<BDD, set<signature_BDDs*>*, bdd_compare_t>;
     for (set<signature_BDDs*>::iterator it = lib_sgnt->begin(); it != lib_sgnt->end(); it++){
         for (set<BDD, bdd_compare_t>::iterator itbdd = (*it)->BDDs->begin(); itbdd != (*it)->BDDs->end(); itbdd++){
             //printBDD(stdout,*itbdd);
             if (map->find(*itbdd) == map->end()){ //cout<<"not there ";
                std::set<signature_BDDs*>* set = new std::set<signature_BDDs*>;
                set->insert(*it);
                (*map)[*itbdd] = set;
             }else{//cout<<"there ";
                assert((*map)[*itbdd]->find(*it) == (*map)[*itbdd]->end());
                (*map)[*itbdd]->insert(*it);
             };
         };
     };
     //for (std::map<BDD, set<signature_BDDs*>*, bdd_compare_t>::iterator it = map->begin(); it != map->end(); it++){
         //printBDD(stdout,it->first);
         //cout<<"- size = "<<it->second->size()<<endl;
     //};
//cout<<"classified_library_bdds end"<<endl;
//int i; cin>>i;
     return map;
};

#endif
