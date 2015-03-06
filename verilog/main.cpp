#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include "ast.h"
#include "structv.tab.hh"
#include "flat_module.h"
#include "node.h"
#include "library.h"
#include "kcover.h"
#include "aggr.h"
#include "ww_traversal.h"
#include "signature.h"
#include "lib_match.h"
#include "simre.h"
#include <time.h>
#include "libgen.h"
#include "main.h"
#include "vcd.h"
#include "simplify.h"
#include "timer.h"
#include "coloring.h"

#include <sys/time.h>
#include <sys/resource.h>

extern int yyparse();
extern FILE* yyin;

bsim_options_t options;

int bsim_main(int argc, char* argv[]);
void bsim_usage(const char* prog);
int bmtest_main(int argc, char* argv[]);
int dumpcnf_main(int argc, char* argv[]);

int main(int argc, char* argv[])
{

    char* progname_full = strdup(argv[0]);
    char* progname = basename(progname_full);

    int r = 0;
    if(strcmp(progname, "simre") == 0) {
        r = simre_main(argc, argv);
    } else if(strcmp(progname, "simplify_test") == 0) {
        r = simplify_main(argc, argv);
    } else if(strcmp(progname, "bmtest") == 0) {
        r = bmtest_main(argc, argv);
    } else if(strcmp(progname, "dumpcnf") == 0) {
        r = dumpcnf_main(argc, argv);
    } else {
        r = bsim_main(argc, argv);
    }
    free(progname_full);
    return r;
}

int bmtest_main(int argc, char* argv[])
{
    if(argc != 2) {
        std::cerr << "Syntax error." << std::endl;
        std::cerr << "  Usage: " << argv[0] << " <netlist.v>" << std::endl;
        return 1;
    }

    // try to open the verilog file.
    yyin = fopen(argv[1], "rt");
    if(yyin == NULL) {
        perror(argv[optind]);
        return 1;
    }

    if(yyparse() == 0) {
        // initialize CUDD.
        flat_module_t::setup_cudd();

        // init tech library.
        std::string lib("12soi");
        ILibrary* library = createLib(lib);
        library->init();

        if(modules->size() != 1) {
            std::cout << "The file you have specified is not flat ";
            std::cout << "(i.e., it has more than one module.)" << std::endl;
            return 1;
        }
        library->preprocess(*modules);
        flat_module_t *flat_ptr = new flat_module_t(library, modules, -1);
        flat_module_t& flat = *flat_ptr;

        if(flat) {
            flat.count_unate_vars();
        }

        delete flat_ptr;
        flat_module_t::destroy_cudd();
        delete library;
    }
    return 0;
}

int bsim_main(int argc, char* argv[])
{
    timer.start("init");
    char c;
    while ((c = getopt (argc, argv, "hwav:c:k:K:f:s:mS:lI:U:p:MCL:E:T:o:")) != -1) {
        switch (c) {
            case 'h':
                bsim_usage(argv[0]);
                return 0;
                break;
            case 'o':
                options.readOptions(optarg);
                break;
        }
    }

    // check if we got a test article.
    if(optind == argc || optind != argc-1) {
        bsim_usage(argv[0]);
        return 1;
    }

    // try to open the verilog file.
    yyin = fopen(argv[optind], "rt");
    if(yyin == NULL) {
        perror(argv[optind]);
        return 1;
    }

    options.renameFiles(argv[optind]);
    if(yyparse() == 0) {
        // initialize CUDD.
        flat_module_t::setup_cudd();

        // init tech library.
        ILibrary* library = createLib(options.testCktTechLib);
        library->init();

        if(modules->size() != 1) {
            std::cout << "The file you have specified is not flat ";
            std::cout << "(i.e., it has more than one module.)" << std::endl;
            return 1;
        }

        // create the flat module.
        std::cout << "# of module instantiations: " << (*modules)[0]->num_module_inst() << std::endl;


        // create the flat module.
        library->preprocess(*modules);
        flat_module_t *flat_ptr = new flat_module_t(library, modules, -1);
        flat_module_t& flat = *flat_ptr;

        if(flat) {
            flat.dump_nodecounts(std::cout);
            delete_modules(modules);

            // flat.test_mar8();
            // flat.find_loops();

            if(options.interactivePropagator) {
                flat.interactive_propagator();
            }

            if(options.libraryElementFiles.size() > 0) {
                generateLibElements(options.libraryElementFiles,
                                    &flat,
                                    options.libCktsTechLib, true);
            }

            if(options.bitsliceFiles.size() > 0) {
                generateLibElements(options.bitsliceFiles,
                                    &flat,
                                    options.libCktsTechLib, false);
            }

            if(options.partialFuncModules.size() > 0) {
                generateFlatModules(options.partialFuncModules,
                        &flat,
                        options.libCktsTechLib);
            }
            timer.stop("init");

            if(options.externalModuleFile.size() > 0) {
                flat.readExternalModules(options.externalModuleFile);
            }

            if(options.analyzeCommonInputs) {
                flat.analyzeCommonInputs();
            }

            if(options.instanceDump.size()) {
                std::string inputfile = options.instanceDump + ".inputs.txt";
                std::string outputfile = options.instanceDump + ".outputs.txt";
                std::string instancefile = options.instanceDump + ".instance.txt";

                std::ofstream inp(inputfile.c_str());
                std::ofstream out(outputfile.c_str());
                std::ofstream inst(instancefile.c_str());

                flat.dumpInstanceNames(inp, out, inst);

                inp.close();
                out.close();
                inst.close();
            }

            if(options.clockTreeRoots.size() > 0) {
                assert(options.clockTreeRoots.size() < 32);
                for(unsigned i=0; i != options.clockTreeRoots.size(); i++) {
                    std::string& s = options.clockTreeRoots[i];
                    node_t* n = flat.get_node_by_name(s.c_str());
                    if(n != NULL) {
                        node_t* m = NULL;
                        // check if it is an I/O pad.
                        if(flat.is_pad_input(n)) {
                            m = flat.get_pad_port(n);
                            if(m == NULL) {
                                std::cerr << "ERROR: Unable to find pad output for '" << s << "'." << std::endl;
                                continue;
                            }
                        } else {
                            m = n;
                        }

                        assert(m != NULL);
                        // create the CLK module if necessary.
                        if(options.createClkModule) {
                            flat.tree_bfs("clktree", n->get_name().c_str(), m);
                        }
                        // mark the tree.
                        flat.mark_tree(m, (1 << i), flat_module_t::MARK_CLKTREE);
                    } else {
                        std::cerr << "ERROR: Unknown signal '" << s << "'." << std::endl;
                    }
                }
            }

            if(options.resetTreeRoots.size() > 0) {
                assert(options.resetTreeRoots.size() < 32);
                for(unsigned i=0; i != options.resetTreeRoots.size(); i++) {
                    std::string& s = options.resetTreeRoots[i];
                    node_t* n = flat.get_node_by_name(s.c_str());
                    if(n != NULL) {
                        node_t* m = NULL;
                        if(flat.is_pad_input(n)) {
                            m = flat.get_pad_port(n);
                            if(m == NULL) {
                                std::cerr << "ERROR: Unable to find pad output for '" << s << "'." << std::endl;
                                continue;
                            }
                        } else {
                            m = n;
                        }
                        assert(m != NULL);
                        flat.mark_tree(m, (1 << i), flat_module_t::MARK_RSTTREE);
                    } else {
                        std::cerr << "ERROR: Unknown signal '" << s << "'." << std::endl;
                    }
                }
            }

            if(options.treeSummaryOut.size()) {
                std::ofstream fout(options.treeSummaryOut.c_str());
                fout << "initial markings" << std::endl;
                fout << "----------------" << std::endl;
                flat.summarize_tree_info(fout);
                fout.close();
            }

            flat.propagateTreeMarks();

            if(options.treeSummaryOut.size()) {
                std::ofstream fout(options.treeSummaryOut.c_str(), ios::app);
                fout << "propagated markings" << std::endl;
                fout << "-------------------" << std::endl;
                flat.summarize_tree_info(fout);
                fout.close();
            }

            for(unsigned i=0; i != options.reset_modules.size(); i++) {
                flat.createModuleFromResetTree(options.reset_modules[i].color, options.reset_modules[i].name);
            }

            if(options.a2dffInfo.size()) {
                std::ofstream fout(options.a2dffInfo.c_str());
                flat.analyze_a2dff(fout);
                fout.close();
            }

            flat.color_analysis();
            flat.pinBackProp();

            if(options.simpleWordAnalysis) {
                flat.simple_word_analysis();
            }

            timer.start("bsim");

            if(options.kcoverSize) {
                flat.kcover_analysis();
            }


            for(unsigned i = 0; i != options.userDefinedModules.size(); i++) {
                flat.addUserDefinedModule(options.userDefinedModules[i].c_str());
            }

            if(options.aggregation) {
                if(options.kcoverSize) {
                    enable_propagation = options.enablePropagation;
                    flat.aggregate();

                    if(options.mergeModules) {
                        flat.mergeMuxes(false);
                    }
                    std::cout << "all aggregation algorithms done!" << std::endl;

                } else {
                    fprintf(stderr, "Can't run aggregation without running k-cover analysis first.\n");
                    return 1;
                }
            }
            timer.stop("bsim");

            if(options.dumpOnlyAdderWords && options.dumpWords.size()) {
                std::ofstream fout(options.dumpWords.c_str());
                flat.output_words(fout);
                fout.close();
            }

            timer.start("vga_fb");
            if(options.analyzeFrameBuffer) {
                flat.analyzeFrameBuffer();
            }
            timer.stop("vga_fb");

            timer.start("simple_fifo");
            if(options.simpleFIFOAnalysis) {
                flat.simpleFIFOAnalysis();
                flat.simpleFIFOAnalysis2();
            }
            timer.stop("simple_fifo");

            if(options.fullFnScript.size()) {
                flat.printFullFunctions(options.fullFnScript.c_str());
            }

            if(options.ramAnalysis) {
                flat.ramAnalysis(false);
                flat.ramAnalysis(true);
            }

            // do shift reg analysis after ram analysis.
            if(options.shiftregAnalysis) {
                timer.start("shiftregs");
                flat.findShiftRegs();
                timer.stop("shiftregs");
            }

            // we want to do counter analysis after
            // RAM and shift reg analysis because we don't consider
            // the latches already covered for the counters
            if(options.counterAnalysis) {
                timer.start("counter");
                flat.findCounters(options.vcdFileName);
                timer.stop("counter");
            }
            if(options.wordTrace) {
                std::cout << "Starting word trace..." << std::endl;
                flat.traceWords(options.vcdFileName);
                //flat.traceParc();
                std::cout << "Word trace ends" << std::endl;
            }


            if(options.NCextract) {
                std::cout << "Starting NC circuit..." << std::endl;
                flat.get_NC_circuit(options.vcdFileName);
                std::cout << "NC ends" << std::endl;
            }

            if(options.signalFlowAnalysis) {
                if(!options.kcoverSize) {
                    fprintf(stderr, "Need k-cover analysis to be enabled for signal flow analysis.\n");
                    return 1;
                } else {
                    flat.signalFlowAnalysis();
                }
            }

            if(options.partialFunctionAnalysis) {
                if(!options.kcoverSize) {
                    fprintf(stderr, "Need k-cover analysis to be enabled for partial function analysis.\n");
                    return 1;
                } else {
                    flat.partialFunctionAnalysis();
                }
            }

            if(options.conflictAnalysis) {
                if(!options.kcoverSize || !options.aggregation) {
                    fprintf(stderr, "Need k-cover analysis and aggregation for conflict analysis.\n");
                } else {
                    flat.conflictAnalysis();
                }
            }


            if (options.coloringWithDepth){
               std::cout<<"COLORING WITH DEPTH"<<endl;
               coloring_with_depth(flat);
            };


            if(options.candidateModulesBound || options.latchAnalysis){
                if(options.aggregation || options.simpleWordAnalysis) {
                    clock_t init, final;
                    init=clock();
                    init_info (flat);
                    if (options.latchAnalysis){
                        std::cout<<"LATCHING"<<std::endl;
                        lw_traversal (flat);
                        clear_nodes  (flat);
                    }
                    if(options.aggregation || options.simpleWordAnalysis){
                        std::cout<<"WORD_ANALYSIS"<<std::endl;
                        ww_traversal (flat);
                        vector<aggr::id_module_t*>* cand_modules_WW = dump_modules (flat, WW);
                        if (options.candidateLibMatch){
                           std::cout<<"CandidateLibMatch"<<std::endl;
                           lib_match (flat, cand_modules_WW);
                           flat.create_matching_modules();
                        };
                        if (options.candidateWordToModule){
                            std::cout<<"WORD TO MODULES TRIVERSAL"<<std::endl;
                            wm_traversal (flat);
                            dump_modules (flat, WM);
                        }
                    }
                    free_info (flat);
                    final=clock()-init;
                    cout << (double)final / ((double)CLOCKS_PER_SEC)<<endl;
                } else {
                    fprintf(stderr, "Can't run unknown module analysis without aggregation or simple word analysis.\n");
                    return 1;
                }
            }


            // now we can do the merge modules thing.
            // this is where the ilp-based overlap resolution happens.
            flat.merge_modules();

            if(options.mergeModules2) {
                flat.mergeMuxes(true);
            }

            if(options.candidateOutFile.size()) {
                std::ofstream fout(options.candidateOutFile.c_str());
                if(fout) {
                    flat.dump_unknown_modules(fout);
                } else {
                    fprintf(stderr, "Error opening file for writing: %s\n", options.candidateOutFile.c_str());
                    return 1;
                }
            }

            if(options.circuitGraphOutFile.size()) {
                std::ofstream fout(options.circuitGraphOutFile.c_str());
                flat.dump_nodes(fout);
            }

            if(options.verilogOutFile.size()) {
                char* vfile = static_cast<char*>(malloc(options.verilogOutFile.size() + 4));
                strcpy(vfile, options.verilogOutFile.c_str());
                strcat(vfile, ".v");

                char* libvfile = static_cast<char*>(malloc(options.verilogOutFile.size() + 16));
                strcpy(libvfile, options.verilogOutFile.c_str());
                strcat(libvfile, ".library.v");

                std::ofstream fout(vfile);
                std::ofstream libFout(libvfile);
                flat.dump_verilog(fout, libFout);

                free(vfile);
                free(libvfile);
            }

            if(options.summaryOutFile.size()) {
                std::ofstream fout(options.summaryOutFile.c_str());
                // flat.dump_coverage_info(fout);
                flat.dump_summary(fout);
                fout.close();
            }

            if(options.dumpWords.size() && !options.dumpOnlyAdderWords) {
                std::ofstream fout(options.dumpWords.c_str());
                flat.output_words(fout);
                fout.close();
            }
            // flat.compute_cover_distance();
            // flat.adjacencyAnalysis();
        } else {
            printf("Errors found while parsing module. Exiting now!\n");
            return 1;
        }

        timer.start("cleanup");
        delete flat_ptr;
        flat_module_t::destroy_cudd();
        delete library;
        timer.stop("cleanup");
        timer.report(std::cout);
        std::cout << "# of sat queries    :" << sat_n::satQueryCount << std::endl;

        if(options.summaryOutFile.size()) {
            using namespace std;
            ofstream fout(options.summaryOutFile.c_str(), ios_base::app);

            struct rusage usage;
            getrusage(RUSAGE_SELF, &usage);
            double t = usage.ru_utime.tv_sec + usage.ru_utime.tv_usec*1e-6 +
                       usage.ru_stime.tv_sec + usage.ru_stime.tv_usec*1e-6;

            fout << "sat_queries             " << std::setw(10) << sat_n::satQueryCount << std::endl;
            fout << "cpu_time                " << std::setw(10) << t << std::endl;
            fout << "maxrss                  " << std::setw(10) << (double) usage.ru_maxrss / 1048576.0 << std::endl;
            fout.close();
        }
    }

    return 0;
}

int dumpcnf_main(int argc, char* argv[])
{
    if(argc != 4) {
        std::cout << "Syntax error." << std::endl;
        std::cout << "Usage: " << argv[0] << " <verilog> <node> <filename>" << std::endl;
        return 1;
    }

    yyin = fopen(argv[1], "rt");
    if(yyin == NULL) {
        perror(argv[1]);
        return 1;
    }

    if(yyparse() == 0) {
        // initialize CUDD.
        flat_module_t::setup_cudd();

        // init tech library.
        std::string lib("12soi");
        ILibrary* library = createLib(lib);
        library->init();

        if(modules->size() != 1) {
            std::cout << "The file you have specified is not flat ";
            std::cout << "(i.e., it has more than one module.)" << std::endl;
            return 1;
        }

        // create the flat module.
        library->preprocess(*modules);
        flat_module_t flat(library, modules, -1);
        if(flat) {
            if(strcmp(argv[2], "(all)")== 0) {
                for(unsigned i=0; i != flat.num_outputs(); i++) {
                    node_t* n = flat.get_output(i);
                    if(!n->is_input()) {
                        std::string filename = argv[3] + std::string("_out_") + n->get_name() + std::string(".cnf");
                        std::cout << "creating: " << filename << std::endl;
                        std::ofstream fout(filename.c_str());
                        n->dump_cnf(fout);
                        fout.close();
                    }
                }
            } else {
                node_t* n = flat.get_node_by_name(argv[2]);
                if(n == NULL) {
                    std::cerr << "Unable to find node: " << argv[2] << std::endl;
                    return 1;
                }

                std::ofstream out(argv[3]);
                if(!out) {
                    std::cerr << "Unable to open file: " << argv[3] << std::endl;
                    return 1;
                }
                n->dump_cnf(out);
                out.close();
                std::cout << "successfully dumped cnf to file: " << argv[3] << std::endl;
            }
        }
    }
    return 0;
}


void bsim_usage(const char* prog)
{
    std::cout << "Usage: " << prog << " [options] <input.v>" << std::endl;
    std::cout << "   -h                     : this message." << std::endl;
    std::cout << "   -o <xml config file>   : configuration file to use." << std::endl;
}
