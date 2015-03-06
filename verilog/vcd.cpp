#include "vcd.h"
#include "common.h"
#include "counter.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>

#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace vcd_n
{
    int bit_t::get_value(unsigned t)
    {
        times_t::iterator pos = values.lower_bound(t);
        if(pos == values.end()) return -1;
        else return pos->second;
    }

    void bit_t::add_value(unsigned t, int v)
    {
        values[t] = v;
    }

    std::ostream& dump(std::ostream& out, unsigned time, bitvector_t& bits)
    {
        for(unsigned i=0; i != bits.size(); i++) {
            int v = bits[i].get_value(time);
            if(v == -1) {
                out << "x";
            } else {
                assert(v == 0 || v == 1);
                out << v;
            }
        }
        return out;
    }

    std::ostream& operator<<(std::ostream& out, intvec_t& iv)
    {
        for(unsigned i=0; i != iv.size(); i++) {
            out << (iv[i] == 0 ? '0' :
                    iv[i] == 1 ? '1' : 'x');
        }
        return out;
    }

    vcd_signal_t::vcd_signal_t(std::string& n, std::string& s, int maxi, int mini)
        : name(n)
        , symbol(s)
        , max_index(maxi)
        , min_index(mini)
    {
        bits.resize(size());
        for(unsigned i=0; i != bits.size(); i++) {
            bits[i].signal = this;
            bits[i].index = min_index+i;
        }
    }

    void vcd_signal_t::record_change(unsigned time, std::string& valstr)
    {
        if(is_value(valstr[0])) {
            assert(valstr.size() == 1);
            assert(size() == 1);
            bits[0].add_value(time, get_value(valstr[0]));
        } else {
            assert(is_base_letter(valstr[0]));
            if(tolower(valstr[0]) == 'r') {
                // just ignore these signals for now.
            } else {
                assert(tolower(valstr[0]) == 'b');
                std::string data = valstr.substr(1);
                std::reverse(data.begin(), data.end());
                int last_value = -1;
                for(unsigned i=0; i < data.size(); i++) {
                    char c = data[i];
                    last_value = get_value(c);
                    bits[i].add_value(time, last_value);
                }
                // left-extend (section 18.2.2 in IEEE 1364-2001 Version C)
                for(unsigned i=data.size(); i < bits.size(); i++) {
                    bits[i].add_value(time, last_value == -1 ? -1 : 0);
                }
            }
        }
    }

    bool is_time(const std::string& s)
    {
        if(s.size() < 2) return false;
        if(s[0] != '#') return false;
        for(unsigned i=1; i != s.size(); i++) {
            if(!isdigit(s[i])) return false;
        }
        return true;
    }

    int get_time(const std::string& s)
    {
        assert(is_time(s));

        std::string sub = s.substr(1);
        return atoi(sub.c_str());
    }

    bool is_value(char c)
    {
        return ((tolower(c) == 'x') || (tolower(c) == 'z') ||
                (tolower(c) == '1') || (tolower(c) == '0'));
    }

    bool is_base_letter(char c)
    {
        return (tolower(c) == 'b') || (tolower(c) == 'r');
    }

    int get_value(char c)
    {
        if(c == '0') return 0;
        else if(c == '1') return 1;
        else return -1;
    }

    std::ostream& operator<<(std::ostream& out, const vcd_signal_t& sig)
    {
        out << "name:" << sig.name << " ";
        out << "sym:" << sig.symbol << " ";
        out << "[" << sig.max_index << ":" << sig.min_index << "]";
        return out;
    }

    int get_max_index(std::string& str)
    {
        assert(str[0] == '[');
        unsigned end = str.find(':');
        assert(end != std::string::npos);
        std::string num_str = str.substr(1, end);
        return atoi(num_str.c_str());
    }

    int get_min_index(std::string& str)
    {
        assert(str[0] == '[');
        unsigned end = str.find(':');
        assert(end != std::string::npos);
        std::string num_str = str.substr(end+1);
        return atoi(num_str.c_str());
    }

    int get_vector_index(std::string& str)
    {
        assert(str[0] == '[');
        unsigned end = str.find(']');
        assert(end != std::string::npos);
        std::string num_str = str.substr(1, end-1);
        return atoi(num_str.c_str());
    }

    bool is_new_vcd(std::string& str)
    {
        unsigned end = str.find(':');
        //std::cout << str << std::endl;
        //std::cout << "end " << end << std::endl;
        //std::cout << "str end " << std::string::npos << std::endl;
        return (end > str.size());
    }

    vcd_file_t::vcd_file_t(std::string& filename)
        : filename(filename)
    {
        using namespace std;

        ifstream in(filename.c_str());
        vector<string> words;
        vector<string> scopes;
        std::string scope_string = get_scope(scopes);

        if(!in) {
            fprintf(stderr, "unable to open file: %s\n", filename.c_str());
            exit(1);
        }

        bool in_var = false;
        bool in_scope = false;
        bool end_def = false;
        unsigned current_time = 0;
        initial_time = UINT_MAX;
        int gcd_step = -1;

        while(in) {
            string word;
            in >> word;
            // enter in_var, when we read $var.
            if(word == "$var") {
                assert(!in_var && !in_scope);
                in_var = true;
            }
            // enter in_scope when we read $scope.
            if(word == "$scope") {
                assert(!in_scope && !in_var);
                in_scope = true;
            }
            if(in_var || in_scope) {
                words.push_back(word);
            }
            // exit when we read $end
            if(word == "$end") {
                if(in_var) {
                    process_vars(words, scope_string);
                    words.clear();
                    in_var = false;
                    assert(!in_scope);
                }
                if(in_scope) {
                    process_scope(words, scopes);
                    scope_string = get_scope(scopes);

                    words.clear();
                    in_scope = false;
                    assert(!in_var);
                }
            }
            if(word == "$upscope") {
                assert(!in_scope && !in_var);
                scopes.pop_back();
                scope_string = get_scope(scopes);
            }

            if(word == "$enddefinitions") {
                assert(!end_def);
                end_def = true;
            }
            if(end_def) {
                if(is_time(word)) {
                    unsigned last_time = current_time;
                    current_time = get_time(word);
                    unsigned step = current_time - last_time;
                    if(gcd_step == -1) gcd_step = step;
                    else gcd_step = gcd(gcd_step, step);

                    if(current_time < initial_time) {
                        initial_time = current_time;
                    }
                }
                if(is_value(word[0])) {
                    std::string val = word.substr(0, 1);
                    std::string sym = word.substr(1);
                    if(signal_map.find(sym) == signal_map.end()) {
                        fprintf(stderr, "Unable to find symbol:%s in the map.\n", sym.c_str());
                        exit(1);
                    }
                    update_all(signal_map[sym], current_time, val);
                } else if(is_base_letter(word[0])) {
                    std::string sym;
                    in >> sym;
                    if(signal_map.find(sym) == signal_map.end()) {
                        fprintf(stderr, "Unable to find symbol:%s in the map.\n", sym.c_str());
                        exit(1);
                    }
                    update_all(signal_map[sym], current_time, word);
                }
            }
        }
        final_time = current_time;
        this->step = gcd_step;

        const bool debug_dump_signals = false;
        if(debug_dump_signals) {
            dump_signals(std::cout);
            std::cout << "final time: " << final_time << std::endl;
            std::cout << "gcd step  : " << this->step << std::endl;
            std::cout << "# of bits : " << bits.size() << std::endl;
            vcd_signal_list_t& l = signal_map[std::string("F&")];
            vcd_signal_t* s = l[0];
            std::cout << *s << std::endl;
            for(unsigned i=0; i < final_time; i += gcd_step) {
                std::cout << std::setw(10) << i << ":";
                dump(std::cout, i, s->bits) << std::endl;
            }
        }
    }

    void vcd_file_t::process_vars(std::vector<std::string>& vars, std::string& scope)
    {
        assert(vars.size() == 7 || vars.size() == 6);
        assert(vars[0] == "$var");

        std::string name = scope + "." + vars[4];

        unsigned size = atoi(vars[2].c_str());
        vcd_signal_t* sig = NULL;

        //New version //
        if(vars.size() == 7){
            if(is_new_vcd(vars[5])){
            //if(true){
                int idx = get_vector_index(vars[5]);
                sig = new vcd_signal_t(name, vars[3], idx, idx);
            }
            else{
            // Previous version //
                //if(size == 1) {
                //sig = new vcd_signal_t(name, vars[3], 0, 0);
                //} else {
                int mini = get_min_index(vars[5]); //TODO
                int maxi = get_max_index(vars[5]);
                if(mini > maxi) std::swap(mini, maxi);
                sig = new vcd_signal_t(name, vars[3], maxi, mini);
                //}
                //std::cout <2 "inside previous" << std::endl;
            }
        }
        else{
            sig = new vcd_signal_t(name, vars[3], 0, 0);
        }
        assert(sig->size() == size);
        signals.push_back(sig);
        signal_map[sig->symbol].push_back(sig);
        for(unsigned i=0; i != sig->bits.size(); i++) {
            bit_t* b = &(sig->bits[i]);
            bits.push_back(b);
        }
    }

    void vcd_file_t::process_scope(std::vector<std::string>& scope, std::vector<std::string>& scopes)
    {
        assert(scope.size() == 4);
        assert(scope[0] == "$scope");
        assert(scope[3] == "$end");
        scopes.push_back(scope[2]);
    }

    std::string vcd_file_t::get_scope(std::vector<std::string>& scopes)
    {
        std::string s = "";
        for(unsigned i=0; i != scopes.size(); i++) {
            s += scopes[i];
            if(i+1 != scopes.size()) {
                s += ".";
            }
        }
        return s;
    }

    void vcd_file_t::update_all(vcd_signal_list_t& l, unsigned t, std::string& v)
    {
        for(vcd_signal_list_t::iterator it = l.begin(); it != l.end(); it++) {
            (*it)->record_change(t, v);
        }
    }

    void vcd_file_t::dump_signals(std::ostream& out)
    {
        for(vcd_signal_list_t::iterator it = signals.begin();
                                        it != signals.end();
                                        it++)
        {
            vcd_signal_t* sig = *it;
            out << *sig << std::endl;
        }
    }

    vcd_file_t::~vcd_file_t()
    {
        for(vcd_signal_list_t::iterator it = signals.begin(); it != signals.end(); it++) {
            vcd_signal_t* sig = *it;
            delete sig;
        }
        signals.clear();
    }

    void vcd_file_t::read_vector(intvec_t& vec, unsigned time)
    {
        assert(vec.size() == bits.size());
        for(unsigned i=0; i != bits.size(); i++) {
            int v = bits[i]->get_value(time);
            vec[i] = v;
        }
    }

    void vcd_file_t::counter_analysis()
    {
        intvec_t v1(bits.size(), -1);
        intvec_t v2(bits.size(), -1);
        intvec_t& vlast = v1;
        intvec_t& vcurr = v2;
        wtmap_t wtmap;
        intvec_t cnts(bits.size(), 0);

        read_vector(vlast, initial_time);
        std::cout << "initial time : " << initial_time << std::endl;
        std::cout << "final time   : " << final_time << std::endl;
        std::cout << "step         : " << step << std::endl;

        for(unsigned t=initial_time + step; t <= final_time; t += step) {
            read_vector(vcurr, t);
            update_wtmap_counter(vlast, vcurr, wtmap, cnts);
            printf("TIME:%7d\r", (int)t); fflush(stdout);

            intvec_t& vtmp = vlast;
            vlast = vcurr;
            vcurr = vtmp;
        }
        printf("\n");

        double ratio_max = -1e100;
        int a=-1, b=-1;
        for(wtmap_t::iterator it = wtmap.begin(); it != wtmap.end(); it++) {
            const intpair_t& p = it->first;
            double count = it->second;
            double scale = cnts[p.first];

            double ratio = count / scale;
            if(ratio >= ratio_max) {
                ratio_max = ratio;
                a = p.first;
                b = p.second;
            }
            if(ratio >= 0.99) {
                std::cout << "a:" << bits[a]->signal->name << "[" << bits[a]->index << "]" << " and ";
                std::cout << "b:" << bits[b]->signal->name << "[" << bits[b]->index << "]" << std::endl;
            }
        }
        std::cout << a << "," << b << ": " << ratio_max << std::endl;
        std::cout << "cnt: " << wtmap[intpair_t(a,b)] << std::endl;
        std::cout << "scl: " << cnts[a] << std::endl;
        std::cout << "a:" << bits[a]->signal->name << "[" << bits[a]->index << "]" << std::endl;
        std::cout << "b:" << bits[b]->signal->name << "[" << bits[b]->index << "]" << std::endl;
        std::cout << "size of wtmap: " << wtmap.size() << std::endl;
    }

    void vcd_file_t::update_wtmap_counter(
        intvec_t& v1,
        intvec_t& v2,
        wtmap_t& wtmap,
        intvec_t& cnts
    )
    {
        assert(v1.size() == v2.size());

        intvec_t ups;
        intvec_t tgs;

        for(unsigned i=0; i != v1.size(); i++) {
            if(v1[i] == 1 && v2[i] == 0) {
                ups.push_back(i);
            }
            if(v1[i] != v2[i]) {
                tgs.push_back(i);
            }
        }

        for(unsigned i=0; i != ups.size(); i++) {
            int a = ups[i];
            cnts[a] += 1;
            for(unsigned j=0; j != tgs.size(); j++) {
                int b = tgs[j];
                if(a != b) {
                    wtmap[intpair_t(a, b)] += 1;
                }
            }
        }
    }

    void vcd_file_t::prepare_NCinfo(std::map<node_t*, int> & ref_nodebitnomap,intvec_t& v1, flat_module_t * fm)
    {
				get_nodebitnomap(ref_nodebitnomap,fm);
        read_vector(v1, 100800);
    }

    void vcd_file_t::get_nodebitnomap(std::map<node_t*, int> & ref_nodebitnomap,flat_module_t * fm)
    {
        std::ofstream fl("map.out");
        for(unsigned i=0; i != bits.size(); i++) {
            node_t * nd = get_node_by_bit(bits[i], fm);
            if (nd != NULL){
                fl << "nd: "<<nd->get_name() << std::endl;
                ref_nodebitnomap[nd] = i;
                fl << "mapped bno: "<<ref_nodebitnomap[nd] << std::endl;
            }
        }
        std::cout << "node-bitno map is DONE." << std::endl;
        fl.close();
    }

    void vcd_file_t::get_bitnonodemap(std::map<int,node_t*> & ref_bitnonodemap,flat_module_t * fm)
    {
        std::ofstream fl("map_inverted.out");
        for(unsigned i=0; i != bits.size(); i++) {
            node_t * nd = get_node_by_bit(bits[i], fm);
            if (nd != NULL){
                fl << "nd: "<<nd->get_name() << std::endl;
                ref_bitnonodemap[i] = nd;
                fl << "mapped bno: "<<i << std::endl;
            }
        }
        std::cout << "bitno-node map is DONE." << std::endl;
        fl.close();
    }


    void vcd_file_t::traceWordsinVCD(flat_module_t* fm,nodelist_t& inputs)
    {
        intvec_t v1(bits.size(), -1);
        intvec_t v2(bits.size(), -1);
        intvec_t& vlast = v1; //intvec_t *vlast = &v1
        intvec_t& vcurr = v2;
        intvec_t cnts(bits.size(), 0);
        intvec_t& ref_cnts = cnts;
        std::map<node_t*, int> nodebitnomap;
        std::map<node_t*, int> & ref_nodebitnomap = nodebitnomap;
        std::vector<wordGraph_t*> pstack;

        //create folder
        size_t found = filename.find_last_of("/");
        std::string flname = filename.substr(found+1);
        std::string fname = flname.substr(0,flname.size()-4);
        std::cout << "filename "<< fname << std::endl;
        mkdir(fname.c_str(), 755);

        std::cout << filename.substr(0,found+1).c_str() << std::cout;
        std::ofstream fl((fname + "/map.out").c_str());
        std::ofstream fl2((fname  + "/bits.out").c_str());
        std::ofstream fl3((fname + "/toggles.out").c_str());


        read_vector(vlast, initial_time);
        std::cout << "initial time : " << initial_time << std::endl;
        std::cout << "final time   : " << final_time << std::endl;

        //TODO: final_time will be fixed
        for(unsigned t=initial_time + step; t <= final_time; t += step) {
            read_vector(vcurr, t);
            update_toogle_counter(vlast, vcurr, ref_cnts);
            printf("TIME:%7d\r", (int)t); fflush(stdout);

            intvec_t& vtmp = vlast;
            vlast = vcurr;
            vcurr = vtmp;
        }
        printf("\n");

        for(unsigned i=0; i != bits.size(); i++) {
            fl2 << "bits:" << bits[i]->signal->name << "[" << bits[i]->index << "]" << std::endl;
            fl2 << "bits:" << bits[i]->signal->max_index << " " << bits[i]->signal->min_index << std::endl;
            fl3 << bits[i]->signal->name << "[" << bits[i]->index << "] : " << ref_cnts[i] << std::endl;
        }

        fl2.close();
        fl3.close();

        //
        for(unsigned i=0; i != bits.size(); i++) {
            node_t * nd = get_node_by_bit(bits[i], fm);
            if (nd != NULL){
                fl << "nd: "<<nd->get_name() << std::endl;
                ref_nodebitnomap[nd] = i;
                fl << "mapped bno: "<<ref_nodebitnomap[nd] << std::endl;
            }
        }
        std::cout << "node-bitno map is DONE." << std::endl;
        fl.close();

        //node_t * nn = (*fm).get_node_by_name("ctrl_n415");
        //for(node_t::fanout_iterator it = nn->get_first_fanout()->fanouts_begin(); it != nn->get_first_fanout()->fanouts_end(); it++) {
            //node_t* element = *it;
            //std::cout << "child of ctrl_n415" << element->get_name() <<std::endl;
        //}

        //print input names
        for(unsigned i=0; i != inputs.size(); i++) {
            node_t* n = inputs[i];
            std::cout << "name: " << n->get_name()<< std::endl;
        }

        //TODO: need to be fixed to iterate over the output word bits
        //TODO: need to be fixed to iterate over the input word bits
        for(unsigned k=0; k!=34; k++){
            wordGraph_t* wgph = NULL;

            char buffer[50];
            sprintf (buffer, "branches%d.out",k);
            std::string fn = buffer;

            std::ofstream fl4((fname + "/" +fn).c_str());
            //node_t* n = ( (*fm).get_pad_port(inputs[k+2]) );
            //node_t* n = inputs[k+2]; //for dmem stall
            //node_t* n = inputs[k+39]; //for imem stall
            node_t* n = inputs[k+12]; //for imem ssc
            intvec_t flgs(bits.size(), 0);
            intvec_t& ref_flgs = flgs;

            //debugging prints
            //std::cout << "MEM_DATA_INp[0]: "<< std::endl;
            //std::cout << "get_pad_port() : " << n->get_name().c_str() << std::endl;

            int bno = ref_nodebitnomap[n];
            wgph = new wordGraph_t(n,bno,this, ref_cnts, ref_flgs, ref_nodebitnomap);

            std::cout << "Tree Done: "<< k << std::endl;

            //DONE: print tree
            pstack.push_back(wgph);
            //std::cout << "Stack size " << pstack.size() << std::endl;
            while(pstack.size()>0){
                //std::cout << "Stack size " <<  pstack[pstack.size()-1]->subnodes.size()  << std::endl;
                if(pstack[pstack.size()-1]->subnodes.size() == 0){
                    for(unsigned i = 0; i<pstack.size();i++){
                        //if(i==0){
                            //fl4 << pstack[i]->root->get_name().c_str() << "/";
                        //}
                        //if(pstack[i]->root->is_latch()){
                            fl4 << pstack[i]->root->get_name().c_str() << "/";
                        //}
                    }
                    wordGraph_t *topObj = pstack[pstack.size()-1];
                    pstack.pop_back();
                    delete topObj;
                    fl4 << "" << std::endl;
                }
                if(pstack.size()>0){
                    if(pstack[pstack.size()-1]->pushedsubno < int(pstack[pstack.size()-1]->subnodes.size()) ){
                        pstack[pstack.size()-1]->pushedsubno++;
                        pstack.push_back(pstack[pstack.size()-1]->subnodes[pstack[pstack.size()-1]->pushedsubno-1]);
                    }
                    else{
                        wordGraph_t *topObj = pstack[pstack.size()-1];
                        pstack.pop_back();
                        delete topObj;
                    }
                }
            }
            fl4.close();
        }
    }

    //sig = new vcd_signal_t(name, vars[3], maxi, mini);
    wordGraph_t::wordGraph_t(node_t* n, int bno, vcd_file_t* vcdf, intvec_t& ref_cnts,intvec_t& ref_flgs,std::map<node_t*, int> & ref_nodebitnomap)
    : root(n)
    {
        //find bitno
        bitno = bno;
        //find toggleno
        toggleno = ref_cnts[bitno];
        pushedsubno = 0;
        //const nodeset_t* s1 = n->get_driven_latches();
        //debugging prints
        //printf("\n");
        //std::cout << "node : " << n->get_name().c_str() << std::endl;
        //std::cout << "bitno: " << bitno << std::endl;
        //std::cout << "toggle no: " << toggleno << std::endl;

        //printf("\n");
        ref_flgs[bitno] = 1;

        //DONE: the algorithm is too slow
        //if((n->get_name().compare("cok2402") ==0) || (n->get_name().compare("bigr41409")==0) ){
        for(node_t::fanout_iterator it = n->fanouts_begin(); it != n->fanouts_end(); it++) {
        //for(nodeset_t::iterator it = (*s1).begin(); it != (*s1).end(); it++) {// node.h nin icerisinde
            node_t* element ;
            if((*it)->is_latch_gate()){
                element = (*it)->get_first_fanout();
                //std::cout << (*it)->get_name()<< " is latch gate" <<std::endl;
            }
            else{
                element = (*it);
            }
            //std::cout << "elmnt name before filtering "<< element->get_name().c_str()  << std::endl;
            int sbno = ref_nodebitnomap[element];
            //int sbno = vcdf->get_bitno(element);
            //std::cout << "elmnt bitno before filtering "<< sbno  << std::endl;

            if ( (ref_flgs[sbno]==0) && (ref_cnts[sbno] != 0) && (std::abs(toggleno-ref_cnts[sbno])<3 ) ){  // 3 is the threshold
                //std::cout << "driven latch of " <<  n->get_name().c_str() << ": "  << element->get_name().c_str()  << std::endl;
                wordGraph_t* wgph = NULL;
                wgph = new wordGraph_t(element,sbno,vcdf,ref_cnts,ref_flgs,ref_nodebitnomap);
                subnodes.push_back(wgph);
            }
        } //}

    }

    int vcd_file_t::get_bitno(node_t * n){
        std::string nname = n->get_name();
        //std::cout << "Name: "<< nname << std::endl;
        for(unsigned i = 0; i<bits.size();i++){
            std::string bname = bits[i]-> signal -> name;
            size_t found = bname.find_last_of(".");
            std::string bitname = bname.substr(found+1);
            if(bitname.compare(nname)==0){
                return i;
            }
        }
        return -1;
    }

    node_t* vcd_file_t::get_node_by_bit(bit_t* b, flat_module_t* fm){
        std::string str = b-> signal -> name;
        //if(str.compare(0,28, "TestRISC.DUT.I1.risc_fpu_I1.")==0){
        size_t found = str.find_last_of(".");
        std::string ss = str.substr(found+1);
        std::string indxstr;
        std::stringstream out;
        std::string s1;

        out << (b->index);
        indxstr = out.str();
        s1  = ss+"["+indxstr+"]";
        char *a0 = new char[s1.size()+1];
        char *a1 = new char[ss.size()+1];

        a0[s1.size()]=0;
        memcpy(a0,s1.c_str(), s1.size());

        if(b->index > 0){
            return (*fm).get_node_by_name(a0);
        }
        else{
            if((*fm).get_node_by_name(a0) != NULL){
                return (*fm).get_node_by_name(a0);
            }
            else{
                a1[ss.size()]=0;
                memcpy(a1,ss.c_str(), ss.size());
                return (*fm).get_node_by_name(a1);
            }
        }
    }

    void vcd_file_t::counter_analysis2(flat_module_t* fm)
    {
        intvec_t v1(bits.size(), -1);
        intvec_t v2(bits.size(), -1);
        intvec_t& vlast = v1; //intvec_t *vlast = &v1
        intvec_t& vcurr = v2;
        intvec_t cnts(bits.size(), 0);
        intvec_t& ref_cnts = cnts;
        paths pths;
        paths& ref_pths = pths;

        read_vector(vlast, initial_time);
        std::cout << "initial time : " << initial_time << std::endl;
        std::cout << "final time   : " << final_time << std::endl;

        for(unsigned t=initial_time + step; t <= final_time; t += step) {
            read_vector(vcurr, t);
            update_toogle_counter(vlast, vcurr, ref_cnts);
            printf("TIME:%7d\r", (int)t); fflush(stdout);

            intvec_t& vtmp = vlast;
            vlast = vcurr;
            vcurr = vtmp;
        }
        printf("\n");

        //double toogle_max = 1;
        std::map<int,node_t*>  ref_bitnonodemap1;
        std::map<int,node_t*> & ref_bitnonodemap = ref_bitnonodemap1;
        get_bitnonodemap(ref_bitnonodemap,fm);

        //print toogle counts
        std::ofstream cfl("ctransition.txt");
        for(unsigned i=0; i != v1.size(); i++) {
          cfl << "bits:" << bits[i]->signal->name << "[" << bits[i]->index << "] :"<< ref_cnts[i]<< std::endl;
        }

        std::cout << "Inside counter_analysis2()" <<std::endl;
        get_paths(ref_cnts, ref_pths, ref_bitnonodemap,fm);

        //print paths
        for (unsigned iter1 = 0; iter1 < ref_pths.size(); iter1++){
            for(unsigned iter2 = 0; iter2 < (ref_pths[iter1]).size(); iter2++){
                 std::cout << "counter:" << iter1 << " "<< (ref_pths[iter1])[iter2]->get_name() << std::endl;
            }
        }

    }

    void vcd_file_t::get_paths (intvec_t& ref_cnts, paths& ref_pths, std::map<int,node_t*> & ref_bitnonodemap, flat_module_t* fm){
        intvec_t flgs(bits.size(), 0);
        nodelist_t tmp;
        std::cout << "Inside get_paths()" <<std::endl;
        //i as start node
        for (unsigned i=72858; i< 72861; i++){
              node_t* ptrnode_i = ref_bitnonodemap[i];

              if ( (ptrnode_i!= NULL) && ((*ptrnode_i).is_latch()) ){
                  tmp.push_back(ptrnode_i);
                  ref_pths.push_back(tmp);
                  tmp.clear();
                  std::cout << "not null node_i:" << i << ": " << ptrnode_i->get_name() << std::endl;
                  if ((get_children(i, ptrnode_i, ref_pths, ref_cnts, ref_bitnonodemap,fm)) == 0 ) {
                      ref_pths.pop_back();
                  }
              }
          
        } 
    }

    int vcd_file_t::get_children(int i, node_t* ptrnode_i, paths& pths, intvec_t& cnts, std::map<int,node_t*> & ref_bitnonodemap, flat_module_t* fm){
        intvec_t chldr;
        nodelist_t cpy;
        int fchldr = 0;
        int nchldr = 0;

          node_t* ptrnode_j;
          //std::cout << "not null node_i:" << ch_name << std::endl;

          for (unsigned j=72858; j!= 72861; j++){
                ptrnode_j = ref_bitnonodemap[j];
                /*if(ptrnode_j && ptrnode_i->get_name() == "rclo[0]" && ptrnode_j->get_name() == "rclo[1]") {
                        std::cout << "hi" << std::endl;
                }*/
                std::cout << "in get_children: " << ptrnode_j->get_name() << std::endl;
                //std::cout << (*ptrnode_j).is_driving_latch(ptrnode_i) << std::endl;
                //std::cout << (*ptrnode_i).is_driving_latch(ptrnode_j) << std::endl;
                if((ptrnode_j != NULL) && ((*ptrnode_j).is_latch()) ){
                    if ( (abs(cnts[j] - cnts[i]/2)<3) && (cnts[i] != 0) && (cnts[j] != 0)
                        && (cnts[i] != cnts[j])  && (*ptrnode_j).is_driving_latch(ptrnode_i)){
                        chldr.push_back(j);
                        std::cout << "Flag checked:" << ptrnode_j->get_name() << std::endl;///////////
                    }
                }
            
          }

          if (chldr.size() != 0){
            std::cout << "Children number:" << chldr.size() << std::endl;/////////
            if (chldr.size()>1) {
              cpy = (pths)[(pths).size()-1];
            }
            for (unsigned it =0; it<chldr.size(); it++){
                ptrnode_j = ref_bitnonodemap[chldr[it]]; //TWICE

                (pths)[pths.size()-1].push_back(ptrnode_j);

                if (  (check_connectivity((pths)[pths.size()-1]))
                      && counters_n::is_counter(  (pths)[pths.size()-1]  ) ) {

                    if (fchldr == 1) {
                        (pths).push_back(cpy);
                    }
                    std::cout << "Counter found:" << ptrnode_j->get_name() << std::endl;///////////

                    get_children(chldr[it], ptrnode_j, pths, cnts, ref_bitnonodemap, fm);
                    fchldr = 1;
                    nchldr = nchldr +1;

                } else {

                    (pths)[pths.size()-1].pop_back();
                      //fchldr = 0;
                }
                  /*if (( 0 < rchldr) && (it != (chldr.size()-1))) {
                      fchldr = 0;
                      (pths).push_back(cpy);
                  }*/
            }
              //(pths).pop_back();
          }
        return nchldr;
    }

    char* vcd_file_t::get_node_name(bit_t* b){
        std::string str = b-> signal -> name;
        //if(str.compare(0,23, "oc8051_tb.oc8051_top_1.")==0){//uncommented changed for parc
        //if(str.compare(0,28, "TestRISC.DUT.I1.risc_fpu_I1.")==0){
          size_t found = str.find_last_of(".");
          std::string ss = str.substr(found+1);
            std::string indxstr;
            std::stringstream out;
            out << (b->index);
            indxstr = out.str();

          std::string s  = ss+"["+indxstr+"]";
          char *a = new char[s.size()+1];
          a[s.size()]=0;
          memcpy(a,s.c_str(), s.size());
          //std::cout << a << std::endl;

          return a;
        /*} 
        else{
            return NULL;
        }*/
    }

    bool vcd_file_t::check_connectivity(nodelist_t& nl){
        for (unsigned i = 0; i < nl.size(); i++){
            for (unsigned j = i; j < nl.size(); j++ ) {
                if (! (*(nl[i])).is_driven_latch(nl[j])) {
                    std::cout << "return false" << std::endl;
                    return false;
                }
            }
        }
        std::cout << "return true" << std::endl;
        return true;
    }

    void vcd_file_t::update_toogle_counter(
        intvec_t& v1,
        intvec_t& v2,
        intvec_t& cnts
    )
    {
        assert(v1.size() == v2.size());

        for(unsigned i=0; i != v1.size(); i++) {
            if(v1[i] != v2[i]) {
                cnts[i]+= 1;
            }
        }

    }

    void vcd_file_t::update_toogle_counter_wout_x(
        intvec_t& v1,
        intvec_t& v2,
        intvec_t& cnts
    )
    {
        assert(v1.size() == v2.size());

        for(unsigned i=0; i != v1.size(); i++) {
            int v2s = 0;
            int v1s = 0;
            if ((v2[i]==1) ||(v2[i]==0) ){
                v2s = v2[i];
            }
            if ((v1[i]==1) ||(v1[i]==0) ){
                v1s = v1[i];
            }
            if((v1s != v2s) ) {
                cnts[i]+= 1;
            }
        }

    }

    void vcd_file_t::get_transitions(
        intvec_t& stpcnts,
        intvec_t& trcnts,
        intvec_t& keys,
        intvec_t& freeze,
        int lmttr,
        int thrshld,
        intvec_t& v1,
        intvec_t& v2,
        const int primes [98] 
    )
    {
        assert(v1.size() == v2.size());

        for(unsigned i=0; i != v1.size(); i++) {
            //std::cout << i << std::endl;
            if(freeze[i]<1){
                if(stpcnts[i]<=(thrshld)){
                    if((v1[i] != v2[i]) ) {
                        //std::cout << i << std::endl;
                        if( stpcnts[i]>(thrshld*0.75) ){
                            if(v1[i]>0){
                                keys[i] *= primes[trcnts[i]]*v1[i];
                            }
                            trcnts[i] += 1;
                        }
                        else{
                            if(trcnts[i]>=lmttr){
                                freeze[i] = 1;
                            }
                            if(freeze[i]!=1){
                                trcnts[i] = 0;
                                keys[i] = 1;
                            }
                        }
                        stpcnts[i] = 0;
                    }
                    else{
                        stpcnts[i] += 1;
                    }
                }
                else{
                    stpcnts[i] = 0;
                    if(v1[i]>0){
                        keys[i] *= primes[trcnts[i]]*v1[i];
                    }
                    trcnts[i] += 1;
                }
            }
        }

    }

}


