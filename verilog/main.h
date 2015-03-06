#ifndef _MAIN_H_DEFINED_
#define _MAIN_H_DEFINED_

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <set>

#include "common.h"

#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

struct bsim_options_t
{
    bool simpleWordAnalysis;
    unsigned int kcoverSize;
    unsigned int kcoverTh;
    bool aggregation;
    bool disableCgenTreeAggregation;

    int maxTopoSortIterations;

    std::string fullFnScript;

    // handling scrambled netlists.
    bool addFakeConnections;

    bool candidateModulesBound;
    bool candidateWordToModule;
    bool candidateLibMatch;
    bool coloringWithDepth;
    bool latchAnalysis;
    bool ramAnalysis;
    bool partialFunctionAnalysis;
    bool signalFlowAnalysis;
    int  signalFlowAnalysisCommonSignalMinSize;
    bool conflictAnalysis;
    bool rewriteBuffers;
    int  resetPolarity;
    bool createUnknownInputs;
    bool simpleFIFOAnalysis;
    bool ignoreScanInputs;
    bool renameWiresInModule;
    bool createClkModule;
    bool dumpGateComments;
    bool removeDeadNodes;
    bool computeDistances;
    bool analyzeCommonInputs;
    bool interactivePropagator;
    bool bigModules;
    bool eliminateOverlaps;
    bool sliceableILP;
    bool minimizeModuleCount;
    int  coverageTarget;
    bool analyzeFrameBuffer;
    bool computeRepresentativeModules;
    bool mergeModules;
    bool mergeModules2;
    bool knownMergesOnly;

    struct reset_module_t
    {
        int color;
        std::string name;
    };
    typedef std::vector<reset_module_t> reset_module_list_t;
    reset_module_list_t reset_modules;

	std::string vcdFileName;
	std::string refFileName;
    std::string circuitGraphOutFile;
    std::string verilogOutFile;
    std::string summaryOutFile;
    std::string candidateOutFile;
    std::string bitsliceMatchFile;
    std::string partialFunctionOutFile;
    std::string resetSignal;
    std::string simpleFIFOAnalysisOut;
    std::string treeSummaryOut;
    std::string a2dffInfo;
    std::string rewriteLog;
    std::string dumpWords;
    bool dumpOnlyAdderWords;
    std::string removeNOR2XB;
    std::string externalModuleFile;

    std::string testCktTechLib;
    std::string libCktsTechLib;
    std::string instanceDump;
    std::vector<std::string> userDefinedModules;
    std::vector<std::string> libraryElementFiles;
    std::vector<std::string> bitsliceFiles;
    std::vector<std::string> partialFuncModules;
    std::vector<std::string> clockTreeRoots;
    std::vector<std::string> resetTreeRoots;
    std::vector<std::string> undrivenSignals;
    stringlist_t ignorePins;
    std::vector<stringlist_t> pinGroups;
    std::vector<stringlist_t> backProps;
    int backPropLevels;
    unsigned minMultibitElementSize;

    bool counterAnalysis;
    bool wordTrace;
    bool freqWordDetect;
    bool freqBasedNC;
    bool NCextract;
    bool shiftregAnalysis;
    std::set<std::string> caLatchesToIgnore;
    unsigned caMaxCounterSize;

    bool enablePropagation;

    bsim_options_t()
    {
		vcdFileName = "";
		refFileName = "";
        circuitGraphOutFile = "";
        simpleWordAnalysis = false;

        kcoverSize = 6;
        kcoverTh = kcoverSize;
        aggregation = true;
        disableCgenTreeAggregation = false;
        maxTopoSortIterations = 200;

        fullFnScript = "";

        addFakeConnections = false;

        candidateModulesBound = false;
        latchAnalysis = false;
        candidateWordToModule = false;
        candidateLibMatch = false;
        coloringWithDepth = false;
        ramAnalysis = false;
        partialFunctionAnalysis = false;
        signalFlowAnalysis = false;
        signalFlowAnalysisCommonSignalMinSize = 8;
        conflictAnalysis = false;
        rewriteBuffers = false;

        verilogOutFile = "";
        summaryOutFile = "";
        candidateOutFile = "";
        bitsliceMatchFile = "";
        partialFunctionOutFile = "";
        resetSignal = "";
        resetPolarity = 0;
        createUnknownInputs = false;
        simpleFIFOAnalysis = false;
        ignoreScanInputs = false;
        renameWiresInModule = true;
        createClkModule = false;
        dumpGateComments = false;
        removeDeadNodes = false;
        computeDistances = false;
        analyzeCommonInputs = false;
        interactivePropagator = false;
        bigModules = false;
        eliminateOverlaps = false;
        minimizeModuleCount = true;
        sliceableILP = true;
        coverageTarget = -1;
        analyzeFrameBuffer = false;
        computeRepresentativeModules = false;
        mergeModules = false;
        mergeModules2 = false;
        knownMergesOnly = true;

        simpleFIFOAnalysisOut = "";
        treeSummaryOut = "";
        a2dffInfo = "";
        rewriteLog = "";
        dumpWords = "";
        dumpOnlyAdderWords = false;
        removeNOR2XB = "";

        testCktTechLib = "12soi";
        libCktsTechLib = "12soi";
        instanceDump = "";

        enablePropagation = false;

        minMultibitElementSize = 4;
        counterAnalysis = false;
        wordTrace = false;
    		freqWordDetect = false;
    		freqBasedNC = false;
        NCextract = false;
        backPropLevels = 0;
        shiftregAnalysis = false;
        caMaxCounterSize = 8;
    }

    void readOptions(const char* filename)
    {
        using namespace boost::algorithm;
        using namespace std;
        using boost::property_tree::ptree;
        ptree pt;

        read_xml(filename, pt);

		//read vcdfile name
		vcdFileName = pt.get<string>("BSIMConfig.vcdFileName");	

		//read vcdfile name
		refFileName = pt.get<string>("BSIMConfig.refFileName");	
        
        // enable circuit graph output?
        circuitGraphOutFile = pt.get<string>("BSIMConfig.circuitGraphOut");
        trim(circuitGraphOutFile);

        // enable simple word analysis?
        simpleWordAnalysis = pt.get<bool>("BSIMConfig.simpleWordAnalysis");

        // k-cover related.
        kcoverSize = pt.get<int>("BSIMConfig.kcoverSize");
        kcoverTh = pt.get<int>("BSIMConfig.kcoverTh");
        aggregation = pt.get<bool>("BSIMConfig.aggregation");
        disableCgenTreeAggregation = pt.get<bool>("BSIMConfig.disableCgenTreeAggregation");

        // number of iterations in the topological sort.
        maxTopoSortIterations = pt.get<int>("BSIMConfig.maxTopoSortIterations");

        // whether to add "fake" connections to the scrambled netlist.
        addFakeConnections = pt.get<bool>("BSIMConfig.addFakeConnections");

        // full functions (for counter analysis).
        fullFnScript = pt.get<string>("BSIMConfig.fullFunctionScript");
        trim(fullFnScript);

        // module analysis.
        candidateModulesBound = pt.get<bool>("BSIMConfig.candidateModulesBound");
        latchAnalysis = pt.get<bool>("BSIMConfig.latchAnalysis");
        candidateWordToModule = pt.get<bool>("BSIMConfig.candidateWordToModule");
        candidateLibMatch = pt.get<bool>("BSIMConfig.candidateLibMatch");
        backPropLevels = pt.get<int>("BSIMConfig.backPropagationLevels");

        //coloring with depth analysis
        coloringWithDepth = pt.get<bool>("BSIMConfig.coloringWithDepth");

        // output files
        verilogOutFile = pt.get<string>("BSIMConfig.verilogOutFile");
        summaryOutFile = pt.get<string>("BSIMConfig.summaryOutFile");
        candidateOutFile = pt.get<string>("BSIMConfig.candidateOutFile");
        bitsliceMatchFile = pt.get<string>("BSIMConfig.bitsliceMatchFile");
        partialFunctionOutFile = pt.get<string>("BSIMConfig.partialFunctionOutFile");
        resetSignal = pt.get<string>("BSIMConfig.resetSignal");
        simpleFIFOAnalysisOut = pt.get<string>("BSIMConfig.simpleFIFOAnalysisOut");
        treeSummaryOut = pt.get<string>("BSIMConfig.treeSummaryOut");
        a2dffInfo = pt.get<string>("BSIMConfig.a2dffInfo");
        rewriteLog = pt.get<string>("BSIMConfig.rewriteLog");
        dumpWords = pt.get<string>("BSIMConfig.dumpWords");
        dumpOnlyAdderWords = pt.get<bool>("BSIMConfig.dumpOnlyAdderWords");
        removeNOR2XB = pt.get<string>("BSIMConfig.removeNOR2XB");
        externalModuleFile = pt.get<string>("BSIMConfig.externalModuleFile");

        trim(vcdFileName);
        trim(refFileName);
        trim(verilogOutFile);
        trim(summaryOutFile);
        trim(candidateOutFile);
        trim(bitsliceMatchFile);
        trim(partialFunctionOutFile);
        trim(resetSignal);
        trim(simpleFIFOAnalysisOut);
        trim(treeSummaryOut);
        trim(a2dffInfo);
        trim(rewriteLog);
        trim(dumpWords);

        resetPolarity = pt.get<int>("BSIMConfig.resetPolarity");
        createUnknownInputs = pt.get<bool>("BSIMConfig.createUnknownInputs");
        simpleFIFOAnalysis = pt.get<bool>("BSIMConfig.simpleFIFOAnalysis");
        ignoreScanInputs = pt.get<bool>("BSIMConfig.ignoreScanInputs");
        renameWiresInModule = pt.get<bool>("BSIMConfig.renameWiresInModule");
        createClkModule  = pt.get<bool>("BSIMConfig.createClkModule");
        dumpGateComments = pt.get<bool>("BSIMConfig.dumpGateComments");
        removeDeadNodes = pt.get<bool>("BSIMConfig.removeDeadNodes");
        computeDistances = pt.get<bool>("BSIMConfig.computeDistances");
        analyzeCommonInputs = pt.get<bool>("BSIMConfig.analyzeCommonInputs");
        interactivePropagator = pt.get<bool>("BSIMConfig.interactivePropagator");
        bigModules = pt.get<bool>("BSIMConfig.bigModules");
        eliminateOverlaps = pt.get<bool>("BSIMConfig.eliminateOverlaps");
        minimizeModuleCount = pt.get<bool>("BSIMConfig.minimizeModuleCount");
        sliceableILP = pt.get<bool>("BSIMConfig.sliceableILP");
        coverageTarget = pt.get<int>("BSIMConfig.coverageTarget");
        analyzeFrameBuffer = pt.get<bool>("BSIMConfig.analyzeFrameBuffer");
        computeRepresentativeModules = pt.get<bool>("BSIMConfig.computeRepresentativeModules");
        mergeModules = pt.get<bool>("BSIMConfig.mergeModules");
        mergeModules2 = pt.get<bool>("BSIMConfig.mergeModules2");
        knownMergesOnly = pt.get<bool>("BSIMConfig.knownMergesOnly");

        // some misc. options.
        enablePropagation = pt.get<bool>("BSIMConfig.enablePropagation");
        counterAnalysis = pt.get<bool>("BSIMConfig.counterAnalysis");
        wordTrace = pt.get<bool>("BSIMConfig.wordTrace");
        freqBasedNC = pt.get<bool>("BSIMConfig.freqBasedNC");
        freqWordDetect = pt.get<bool>("BSIMConfig.freqWordDetect");
        NCextract = pt.get<bool>("BSIMConfig.NCextract");
        shiftregAnalysis = pt.get<bool>("BSIMConfig.shiftregAnalysis");
        minMultibitElementSize = pt.get<int>("BSIMConfig.minMultibitElementSize");
        partialFunctionAnalysis = pt.get<bool>("BSIMConfig.partialFunctionAnalysis");
        signalFlowAnalysis = pt.get<bool>("BSIMConfig.signalFlowAnalysis");
        ramAnalysis = pt.get<bool>("BSIMConfig.ramAnalysis");
        conflictAnalysis = pt.get<bool>("BSIMConfig.conflictAnalysis");
        rewriteBuffers = pt.get<bool>("BSIMConfig.rewriteBuffers");

        testCktTechLib = pt.get<string>("BSIMConfig.testCktTechLib");
        libCktsTechLib = pt.get<string>("BSIMConfig.libCktsTechLib");

        instanceDump = pt.get<string>("BSIMConfig.instanceDump");
        trim(instanceDump);

        // list of user defined modules.
        ptree ptUDMs = pt.get_child("BSIMConfig.userDefinedModules");
        for(ptree::iterator it = ptUDMs.begin(); it != ptUDMs.end(); it++) {
            string& data = it->second.data();
            trim(data);
            userDefinedModules.push_back(data);
        }

        // list of library files to match with.
        ptree ptLibFiles = pt.get_child("BSIMConfig.libraryElements");
        for(ptree::iterator it = ptLibFiles.begin(); it != ptLibFiles.end(); it++) {
            string& data = it->second.data();
            trim(data);
            libraryElementFiles.push_back(data);
        }
        //list of bitslice files to match
        ptree ptBitsliceFiles = pt.get_child("BSIMConfig.bitslices");
        for(ptree::iterator it = ptBitsliceFiles.begin(); it != ptBitsliceFiles.end(); it++) {
            string& data = it->second.data();
            trim(data);
            bitsliceFiles.push_back(data);
        }

        ptree ptResetTreeModules = pt.get_child("BSIMConfig.resetTreeModules");
        for(ptree::iterator it = ptResetTreeModules.begin(); it != ptResetTreeModules.end(); it++) {
            string& data = it->second.data();
            std::vector<std::string> strs;
            boost::split(strs, data, boost::is_any_of(":\t "));
            if(strs.size() != 2) {
                std::cerr << "Warning! Ignoring illformed string for tag: " << it->first << std::endl;
            } else {
                int resetNum = atoi(strs[0].c_str());
                reset_module_t rsm = { resetNum, strs[1] };
                reset_modules.push_back(rsm);
            }
        }

        ptree ptPartialFuncModules = pt.get_child("BSIMConfig.partialFuncModules");
        for(ptree::iterator it = ptPartialFuncModules.begin(); it != ptPartialFuncModules.end(); it++) {
            string& data = it->second.data();
            trim(data);
            partialFuncModules.push_back(data);
        }

        ptree ptClkTreeRoots = pt.get_child("BSIMConfig.clockTreeRoots");
        for(ptree::iterator it = ptClkTreeRoots.begin(); it != ptClkTreeRoots.end(); it++) {
            string& data = it->second.data();
            trim(data);
            clockTreeRoots.push_back(data);
        }

        ptree ptRstTreeRoots = pt.get_child("BSIMConfig.resetTreeRoots");
        for(ptree::iterator it = ptRstTreeRoots.begin(); it != ptRstTreeRoots.end(); it++) {
            string& data = it->second.data();
            trim(data);
            resetTreeRoots.push_back(data);
        }
        ptree ptUndrivenSignals = pt.get_child("BSIMConfig.undrivenSignals");
        for(ptree::iterator it = ptUndrivenSignals.begin(); it != ptUndrivenSignals.end(); it++) {
            string& data =it->second.data();
            trim(data);
            undrivenSignals.push_back(data);
        }
        ptree ptIgnorePins = pt.get_child("BSIMConfig.ignorePins");
        for(ptree::iterator it = ptIgnorePins.begin(); it != ptIgnorePins.end(); it++) {
            string& data = it->second.data();
            trim(data);
            ignorePins.push_back(data);
        }

        ptree ptPinGroups = pt.get_child("BSIMConfig.pinGroups");
        for(ptree::iterator it = ptPinGroups.begin(); it != ptPinGroups.end(); it++) {
            string& data = it->second.data();
            istringstream sin(data);

            stringlist_t signals;
            while(sin) {
                string sig;
                sin >> sig;
                trim(sig);
                if(sig.size() > 0) {
                    signals.push_back(sig);
                }
            }
            pinGroups.push_back(signals);
        }
        ptree ptBackProps = pt.get_child("BSIMConfig.backPropagation");
        for(ptree::iterator it = ptBackProps.begin(); it != ptBackProps.end(); it++) {
            string& data = it->second.data();
            istringstream sin(data);

            stringlist_t pins;
            while(sin) {
                string sig;
                sin >> sig;
                trim(sig);
                if(sig.size() > 0) {
                    pins.push_back(sig);
                }
            }
            backProps.push_back(pins);
        }

        if(counterAnalysis) {
            caMaxCounterSize = pt.get<unsigned>("BSIMConfig.counterAnalysisMaxCounterSize");
            string ignoreList = pt.get<string>("BSIMConfig.counterAnalysisLatchesToIgnore");
            istringstream iss(ignoreList);

            while(iss) {
                string latch;
                iss >> latch;
                if(latch.size()) {
                    caLatchesToIgnore.insert(latch);
                }
            }
            std::cout << "COUNTER ANALYSIS: Ignoring " << caLatchesToIgnore.size() << " latches." << std::endl;
        }
    }

    void renameFiles(const char* netlistFilename)
    {
        char* fullPath = strdup(netlistFilename);
        char* filename = basename(fullPath);
        char* dotPos = strrchr(fullPath, '.');
        if(dotPos != NULL) {
            *dotPos = '\0';
        }

        std::string namePhrase("$name");
        std::string netlist(filename);

        circuitGraphOutFile = replace(circuitGraphOutFile, namePhrase, netlist);
        verilogOutFile = replace(verilogOutFile, namePhrase, netlist);
        summaryOutFile = replace(summaryOutFile, namePhrase, netlist);
        candidateOutFile = replace(candidateOutFile, namePhrase, netlist);
        bitsliceMatchFile = replace(bitsliceMatchFile, namePhrase, netlist);
        partialFunctionOutFile = replace(partialFunctionOutFile, namePhrase, netlist);
        simpleFIFOAnalysisOut = replace(simpleFIFOAnalysisOut, namePhrase, netlist);
        treeSummaryOut = replace(treeSummaryOut, namePhrase, netlist);
        a2dffInfo = replace(a2dffInfo, namePhrase, netlist);
        rewriteLog = replace(rewriteLog, namePhrase, netlist);
        instanceDump = replace(instanceDump, namePhrase, netlist);
        dumpWords = replace(dumpWords, namePhrase, netlist);

        free(fullPath);
    }

    static std::string replace(std::string& in, std::string& phrase, std::string& repl)
    {
        size_t pos = in.find(phrase);
        if(pos == std::string::npos) {
            return in;
        } else {
            std::string out(in);
            out.replace(pos, phrase.size(), repl);
            std::cout << "in: " << in << "; out: " << out << std::endl;
            return out;
        }
    }
};

extern bsim_options_t options;

#endif // _MAIN_H_DEFINED_
