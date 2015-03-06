#ifndef __AST_H_DEFINED__
#define __AST_H_DEFINED__

#include <vector>
#include <stdint.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "symbol.h"
#include "common.h"
#include <list>

class node_t;

typedef std::list<stringlist_t> ast_wordlist_t;
extern ast_wordlist_t ast_words;

struct wirename_t {
    const char* name;
    char* full_name;
    int index;
    int index2;

    wirename_t(const char* name, int index=-1, int index2=-1) 
        : name(strdup(name)),
          full_name(static_cast<char*>(malloc(strlen(name)+32))),
          index(index),
          index2(index2)
    {
        print_name(full_name);
    }

    wirename_t(const wirename_t& w)
        : name(strdup(w.name)),
          full_name(strdup(w.full_name)),
          index(w.index),
          index2(w.index2)
    {
    }

    ~wirename_t();

    void print_name(char* buf)
    {
        if(index != -1) {
            if(index2 != -1) {
                sprintf(buf, "%s[%d][%d]", name, index, index2);
            } else {
                sprintf(buf, "%s[%d]", name, index);
            }
        } else {
            sprintf(buf, "%s", name);
        }
    }

    void add_index(int i)
    {
        if(index == -1) index = i;
        else if(index2 == -1) index2 = i;
        else assert(false);
        print_name(full_name);
    }
};

struct constant_t
{
    int bit_width;
    uint64_t value;
};

struct aggregate_t;

/* discriminated union of expression types. */

/* declare a list of expressions first. */

struct exp_t {
    const enum exp_type { WIRE, AGGR, CNST } type;
    union {
        wirename_t* wire;
        aggregate_t* aggr;
        constant_t* cnst;
    };
    exp_t(const exp_t& e);
    exp_t(wirename_t* wire_in) : type(WIRE) { wire = wire_in; }
    exp_t(aggregate_t* agg_in) : type(AGGR) { aggr = agg_in; }
    exp_t(constant_t cnst_in) : type(CNST) { cnst = new constant_t(cnst_in); }

    ~exp_t();
};

/* verilog {x, y .. } */
typedef std::vector<exp_t*> exp_list_t;
struct aggregate_t {
     exp_list_t* aggr;
     aggregate_t(exp_list_t* list) : aggr(list) {}
     aggregate_t(const aggregate_t& o) {
         aggr = new exp_list_t();
         for(exp_list_t::iterator i = o.aggr->begin(); i != o.aggr->end(); i++) {
             exp_t* e = *i;
             aggr->push_back(new exp_t(*e));
         }
     }

     exp_t* at(unsigned i) { return (*aggr)[i]; }
     unsigned size() const { return aggr->size(); }

     ~aggregate_t() {
         for(exp_list_t::iterator i  = aggr->begin();
                                  i != aggr->end();
                                  i++)
         {
             exp_t* e = *i;
             delete e;
         }
         delete aggr;
     }
};

struct wirebinding_t {
    wirename_t formal;
    exp_t* actual;
    bool enable_output;

    wirebinding_t(wirename_t formal, exp_t* actual)
        : formal(formal),
          actual(new exp_t(*actual)),
          enable_output(true)
    {
    }

    wirebinding_t(const wirebinding_t& o)
        : formal(o.formal),
          actual(new exp_t(*o.actual)),
          enable_output(o.enable_output)
    {
    }

    ~wirebinding_t() {
        delete actual;
    }
};

typedef std::vector<wirename_t> wire_list_t;
typedef std::vector<wirebinding_t> bindinglist_t;


struct module_inst_t 
{
    wirename_t* module;
    wirename_t* instance;
    bindinglist_t* bindings;

    module_inst_t* sibling;
    wirename_t* joint_module;
    bool suppress;
    node_t* node;

    module_inst_t(wirename_t* mod, wirename_t* inst, bindinglist_t* binds)
        : module(mod),
          instance(inst),
          bindings(binds),
          sibling(NULL),
          joint_module(NULL),
          suppress(false),
          node(NULL)

    {
    }

    ~module_inst_t() {
        delete module;
        delete instance;
        delete bindings;
        if(joint_module) delete joint_module;
    }

    bool has_port(const char* port)
    {
        for(bindinglist_t::iterator it = bindings->begin(); it != bindings->end(); it++) {
            wirebinding_t& wb = *it;
            if(strcmp(wb.formal.full_name, port) == 0) return true;
        }
        return false;
    }
};

struct assgn_t
{
    wirename_t* obj;
    exp_t* exp;

    assgn_t(wirename_t* o, exp_t* e) : obj(o), exp(e) {}
    ~assgn_t() {
        delete obj;
        delete exp;
    }
};

struct stm_t
{
    enum stm_type { INPUT,  OUTPUT, WIRE, MODULE_INST, ASSGN } type;
    union {
        wire_list_t* ids;
        module_inst_t* module_inst;
        assgn_t* assgn;
    };

    stm_t(stm_type t, wire_list_t* l) : type(t), ids(l) {
        assert(t == INPUT || t == OUTPUT || t == WIRE);
    }
    stm_t(module_inst_t* mod_inst) : type(MODULE_INST), module_inst(mod_inst) {}
    stm_t(assgn_t* a) : type(ASSGN), assgn(a) {}
    ~stm_t();
};

typedef std::vector<stm_t*> stm_list_t;

struct module_head_t
{
    const char* name;
    wire_list_t* inputs;

    module_head_t(const char* n, wire_list_t* i) : name(strdup(n)), inputs(i) {}
};

struct module_t
{
    const char* name;
    wire_list_t* inputs;
    stm_list_t* stms;

    module_t(module_head_t* mod_head, stm_list_t* s)
        : name(mod_head->name),
          inputs(mod_head->inputs),
          stms(s)
    {
    }

    int num_module_inst()
    {
        int cnt=0;
        for(stm_list_t::iterator it = stms->begin(); it != stms->end(); it++) {
            stm_t* stm = *it;
            if(stm->type == stm_t::MODULE_INST) {
                cnt+=1;
            }
        }
        return cnt;
    }

    ~module_t();
};

typedef std::vector<module_t*> module_list_t;
extern module_list_t* modules;

/* Output operators. */
std::ostream& operator<< (std::ostream& out, exp_t& exp);
std::ostream& operator<< (std::ostream& out, module_inst_t& mod_inst);
std::ostream& operator<< (std::ostream& out, wirename_t& wire);
std::ostream& operator<< (std::ostream& out, wire_list_t& list);
std::ostream& operator<< (std::ostream& out, bindinglist_t& list);
std::ostream& operator<< (std::ostream& out, assgn_t& a);
std::ostream& operator<< (std::ostream& out, stm_t& a);
std::ostream& operator<< (std::ostream& out, stm_list_t& a);
std::ostream& operator<< (std::ostream& out, module_t& a);
std::ostream& operator<< (std::ostream& out, module_list_t& a);

/* Functions */

constant_t get_const(const char* str);
wire_list_t* declare_objects(int n1, int n2, wire_list_t* ids);
void delete_modules(module_list_t* modules);

#endif // __AST_H_DEFINED__

