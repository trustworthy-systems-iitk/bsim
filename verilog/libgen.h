#ifndef _LIBGEN_DEFINED
#define _LIBGEN_DEFINED
#include <algorithm>
#include <iostream>
#include <iterator>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <stdlib.h>
#include <time.h>

#include "flat_module.h"
#include "library.h"
#include "kcover.h"
#include "node.h"
#include "aggr.h"
#include "ast.h"


int generateLibElements(std::vector<string> filenames, flat_module_t* flat, string libname, bool use_fullfnmgr);
int generateFlatModules(std::vector<string> filenames, flat_module_t* flat, string libname);
int createBDDs(flat_module_t* flat, bool use_fullfnmgr=true);

#endif
