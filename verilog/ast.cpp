#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "ast.h"
#include "structv.tab.hh"

ast_wordlist_t ast_words;

/* this is the parse root. */
module_list_t* modules;

constant_t get_const(const char* str)
{
    char* dup = strdup(str);
    char* p1 = strchr(dup, '\'');
    assert(p1 != NULL);
    *p1 = '\0';

    constant_t c;
    c.bit_width = atoi(dup);
    p1++;
    c.value = 0;
    if(*p1 == 'b') {
        p1++;
        for(;*p1; p1++) {
            assert(*p1 == '0' || *p1 == '1');
            c.value = 2*c.value + (*p1 - '0');
        }
    } else if(*p1 == 'd') {
        p1++;
        c.value = atoi(p1);
    } else {
        assert(false);
    }
    free(dup);
    return c;
}


std::ostream& operator<<(std::ostream& out, wire_list_t& list)
{
    for(unsigned i=0; i != list.size(); i++) {
        out << list[i];
        if(i != list.size() - 1) out << ", ";
    }
    return out;
}

std::ostream& operator<< (std::ostream& out, bindinglist_t& list)
{
    for(unsigned i=0; i != list.size(); i++) {
        wirebinding_t& b = (list)[i];
        assert (b.formal.index == -1);

        out << (".");
        out << (b.formal);
        out << ("(");
        out << *(b.actual);
        out << (")");

        if(i != list.size() - 1) out << (", ");
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, wirename_t& wire)
{
    char *buf = new char[strlen(wire.name)+32];
    wire.print_name(buf);
    out << buf;
    delete [] buf;
    return out;
}


std::ostream& operator<< (std::ostream& out, exp_t& exp) 
{
    switch (exp.type) {
        case exp_t::WIRE: out << *(exp.wire); break;
        case exp_t::AGGR: 
            out << "{ ";
            for(unsigned i=0; i != exp.aggr->size(); i++)  {
                out << *(exp.aggr->at(i));
                if(i != exp.aggr->size() - 1) out << ", ";
            }
            out << " }";
            break;
        case exp_t::CNST: {
            char *buf = new char[1024];
            sprintf(buf, "%d'd%lu", exp.cnst->bit_width, exp.cnst->value); 
            out << buf;
            delete [] buf;
            break;
        }
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, module_inst_t& mod_inst)
{
    out << *(mod_inst.module);
    out << " ";
    out << *(mod_inst.instance);
    out << " (";
    out << *(mod_inst.bindings);
    out << ")";

    return out;
}

std::ostream& operator<<(std::ostream& out, assgn_t& a)
{
    out << "assign " << *(a.obj) << " = " << *(a.exp);
    return out;
}

wire_list_t* declare_objects(int n1, int n2, wire_list_t* ids)
{
    stringlist_t word;
    wire_list_t* wires = new wire_list_t();
    if(n1 <= n2) {
        for(int j=0; j != (int) ids->size(); j++) {
            for(int i=n1; i <= n2; i++) {
                wirename_t& w = (*ids)[j];
                wirename_t v(w.name, w.index, w.index2);
                v.add_index(i);
                wires->push_back(v);
                word.push_back(std::string(v.full_name));
            }
        }
    } else {
        for(int j=0; j != (int) ids->size(); j++) {
            for(int i=n1; i >= n2; i--) {
                wirename_t& w = (*ids)[j];
                wirename_t v(w.name, w.index, w.index2);
                v.add_index(i);
                wires->push_back(v);
                word.push_back(std::string(v.full_name));
            }
        }
    }
    ast_words.push_back(word);
    return wires;
}

std::ostream& operator<<(std::ostream& out, stm_t& s)
{
    switch(s.type) {
        case stm_t::INPUT: out << "input " << *s.ids << ";"; break;
        case stm_t::OUTPUT: out << "output " << *s.ids << ";"; break;
        case stm_t::WIRE: out << "wire " << *s.ids << ";"; break;
        case stm_t::MODULE_INST: out << *s.module_inst << ";"; break;
        case stm_t::ASSGN: out << *s.assgn << ";"; break;
        default: assert(false);
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, stm_list_t& s)
{
    for(stm_list_t::iterator i=s.begin(); i != s.end(); i++) {
        out << "  " << **i << std::endl;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, module_t& m)
{
    std::cout << "module " << m.name << "(" << *m.inputs << ")" << std::endl;
    std::cout << *m.stms << "endmodule" << std::endl;
    return out;
}

std::ostream& operator<<(std::ostream& out, module_list_t& ids)
{
    for(module_list_t::iterator i = ids.begin(); i != ids.end(); i++) {
        std::cout << std::endl << **i << std::endl;
    }
    return out;
}

// Destructors.
module_t::~module_t()
{
    free(const_cast<char*>(name));
    delete (inputs);
    for(stm_list_t::iterator i =  stms->begin(); 
                             i != stms->end();
                             i++)
    {
        delete *i;
    }
    delete (stms);
}

wirename_t::~wirename_t()
{
    assert(name);
    assert(strlen(name));
    free(const_cast<char*>(name));
    free(full_name);
}

stm_t::~stm_t()
{
    switch(type)
    {
        case INPUT:
        case OUTPUT:
        case WIRE:
            delete ids;
            break;
        case MODULE_INST:
            delete module_inst;
            break;
        case ASSGN:
            delete assgn;
            break;
        default:
            assert(false);
            break;
    }
}

exp_t::exp_t(const exp_t& e) : type(e.type) {
    switch(type) {
        case WIRE: wire = new wirename_t(*e.wire); break;
        case AGGR: aggr = new aggregate_t(*e.aggr); break;
        case CNST: cnst = new constant_t(*e.cnst); break;
        default: assert(false);
    }
}
exp_t::~exp_t() {
    switch(type) {
        case WIRE: delete wire; break;
        case AGGR: delete aggr; break;
        case CNST: delete cnst; break;
        default: assert(false); break;
    }
}

void delete_modules(module_list_t* modules)
{
    for(module_list_t::iterator i  = modules->begin(); 
            i != modules->end();
            i++)
    {
        delete *i;
    }
    delete modules;
}
