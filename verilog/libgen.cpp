#include <algorithm>
#include <iostream>
#include <iterator>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "flat_module.h"
#include "library.h"
#include "kcover.h"
#include "node.h"
#include "aggr.h"
#include "ast.h"
#include "libgen.h"
#include "main.h"

extern int yyparse();
extern FILE* yyin;
extern char* progname; 

int generateLibElements(std::vector<string> filenames, flat_module_t* flat, string libname, bool use_fullfnmgr) 
{
	std::vector<string>::iterator it;
	ILibrary* library = createLib(libname); 


	for (it=filenames.begin(); it <  filenames.end(); it++ ) {
		std::cout << "filename: " << it->c_str() << std::endl;
		yyin = fopen(it->c_str(), "rt");
		if (yyin == NULL) {
			fprintf(stderr, "File %s for additional library elements not found.\n", it->c_str());
			continue;
		}
		if(yyparse() == 0) {
			library->preprocess(*modules);	
			for (unsigned i=0; i != modules->size(); i++) {
				flat_module_t *libelem  = new flat_module_t(library, modules, i);
				if(!libelem->valid()) {
				       fprintf(stderr, "Invalid module: %s.\n", libelem->get_module_name());
					exit(1);
				}
				//Create BDD for libelem
				createBDDs(libelem, use_fullfnmgr);
				flat->addLibElem(libelem);
			}
			delete_modules(modules);
		}
	}


    const bool debug_dump_nodes = false;
    if(debug_dump_nodes) { 
	    flat_module_list_t* list = flat->get_libraryElements();
	    for (unsigned i=0; i != list->size(); i++) {
	    	(*list)[i]->dump_nodes(std::cout); //For testing!
	    }
    }
	
	return 1;
}

int generateFlatModules(std::vector<string> filenames, flat_module_t* flat, string libname) 
{
	std::vector<string>::iterator it;
	ILibrary* library = createLib(libname); 


	for (it=filenames.begin(); it <  filenames.end(); it++ ) {
		yyin = fopen(it->c_str(), "rt");
		if (yyin == NULL) {
			fprintf(stderr, "File %s for additional partial function modules not found.\n", it->c_str());
			continue;
		}
		if(yyparse() == 0) {
			library->preprocess(*modules);	
			for (unsigned i=0; i != modules->size(); i++) {
				flat_module_t *newMod  = new flat_module_t(library, modules, i);
				if(!newMod->valid()) {
				       fprintf(stderr, "Invalid module: %s.\n", newMod->get_module_name());
					exit(1);
				}
				//Add partial function module to main flat.
				flat->addPartialFuncModule(newMod);
			}
			delete_modules(modules);
		}
	}


    const bool debug_dump_nodes = false;
    if(debug_dump_nodes) { 
	    flat_module_list_t* list = flat->get_libraryElements();
	    for (unsigned i=0; i != list->size(); i++) {
	    	(*list)[i]->dump_nodes(std::cout); //For testing!
	    }
    }
	
	return 1;
}



int createBDDs(flat_module_t* flat, bool use_fullfnmgr) 
{
	nodelist_t* outputs = flat->get_outputs();
	nodelist_t* inputs = flat->get_inputs();
	
	int num_inputs = use_fullfnmgr ? -1 : (inputs->size() <= options.kcoverSize ? inputs->size() : -1);
	std::map<node_t*, BDD> varMap;
	for (unsigned i=0; i != outputs->size(); i++) {
		flat->addOutputBDD(flat->createFullFn((*outputs)[i], varMap, true, num_inputs));
	}
	for (unsigned i=0; i != inputs->size(); i++) {
		assert(varMap.find((*inputs)[i]) != varMap.end());
		flat->addInputBDD(varMap[(*inputs)[i]]);
	}

    const bool debug_print_BDDs = false;
    if(debug_print_BDDs) {
	    for (unsigned i=0; i != flat->num_outputs(); i++) {
	    	printBDD(stdout, flat->get_outputBDD(i));
	    }
	    for (unsigned i=0; i != flat->num_inputs(); i++) {
	    	printBDD(stdout, flat->get_inputBDD(i));
	    }
    }
    varMap.clear();
	
	return 1;
}
