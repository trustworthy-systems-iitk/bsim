import sys
import os
from collections import namedtuple

import numpy as np
import matplotlib.pyplot as plt

# python scripts/parse-summary-v2.py results/router_flat.summary results/12SOI_evoter_synthesis_flat.summary results/Open8_CPU_flat.summary results/cpu8080_flat.summary results/ae18_core_flat.summary results/mips_16_core_top_flat.summary results/oc8051_top_flat.summary results/risc_fpu_flat2.summary

def get_module_type(mtyp):
    if mtyp.startswith('mux'): return 'mux'
    elif mtyp.startswith('decoder'): return 'dec'
    elif mtyp.startswith('demux'): return 'dmx'
    elif mtyp.endswith('gate'): return 'gf'
    elif mtyp == 'ram': return 'ram'
    elif mtyp == 'counter': return 'cnt'
    elif mtyp == 'upcounter': return 'cnt'
    elif mtyp == 'downcounter': return 'cnt'
    elif mtyp == 'shiftreg': return 'sr'
    elif mtyp.startswith('eqCmp') or mtyp.startswith('neqCmp'): return 'eq'
    elif mtyp == 'ripple_addsub': return 'a/s'
    elif mtyp.endswith('tree'): return 'lt'
    elif mtyp.startswith('clktree'): return 'misc'
    elif mtyp.startswith('reg'): return 'reg'
    elif mtyp.startswith('framebuffer_read'): return 'misc'
    else: 
        print mtyp
        assert False

groups = ['a/s', 'dec', 'dmx', 'eq', 'gf', 'mux', 'lt', 'ram', 'sr', 'cnt', 'reg']

name_dict = {
    'risc_fpu_flat2':'RISC FPU',
    'router_flat':'router',
    '12SOI_evoter_synthesis_flat':'eVoter',
    'Open8_CPU_flat':'Open8',
    'cpu8080_flat':'cpu8080',
    'ae18_core_flat':'ae18',
    'mips_16_core_top_flat':'MIPS16',
    'oc8051_top_flat':'oc8051',
    'risc_fpu_flat2':'RISC FPU',
    'itag1b_bsim_i2c':'I2C',
    'itag1b_bsim_memctrl':'MemCtrl',
    'itag1b_bsim_spi':'SPI',
    'itag1b_bsim_uart':'UART',
    'itag1b_bsim_vga':'VGA',
    'itag1b_bsim_arm':'ARM',
    'itag1b_bsim_svd':'SVD'
}

class summary_file:
    Module = namedtuple('Module', ['name', 'count', 'gates', 'nodes'])

    def __init__(self, name, fileobj):
        self.name = name
        if self.name in name_dict:
            self.name = name_dict[self.name]
        
        self.modules = {}
        for line in fileobj:
            self.parse_line(line.split())
        self.compute_groups()

    def compute_groups(self):
        self.groups = {}
        for g in groups:
            self.groups[g] = (0,0)
        for m in self.modules:
            gt = get_module_type(m)
            (c, g) = self.groups[gt]
            c += self.modules[m].count
            g += self.modules[m].gates
            self.groups[gt] = (c, g)

    def parse_line(self, words):
        if words[0] == 'module':
            mtyp = words[1]
            mcnt = int(words[2])
            mgates = int(words[3])
            mnodes = int(words[4])
            assert mtyp not in self.modules
            self.modules[mtyp] = summary_file.Module(name=mtyp, count=mcnt, gates=mgates, nodes=mnodes)
        else:
            assert len(words) == 2
            try:
                self.__dict__[words[0]] = int(words[1])
            except ValueError:
                self.__dict__[words[0]] = float(words[1])

    def print_info_table(self):
        data = ' & '.join([('%d' % i) for i in [self.inputs, self.outputs, self.gates, self.latches]])
        print self.name, '&', data, '\\\\'

    def print_result_table(self):
        gi1 = ['%d' % i for i in [self.gates, self.latches]] 
        gi2 = ['%d' % self.groups[g][0] for g in groups] 
        gi3 = ['%.0f\\%%' % self.coverage()] + ['%.0fs' % self.cpu_time] + ['%.1f' % self.maxrss]

        print ' & '.join([self.name] + gi1 + gi2 + gi3) + '\\\\'

    def coverage(self):
        return float(self.gates_covered) / float(self.gates) * 100
baseConfig = """
<BSIMConfig>
  <wordTrace>false</wordTrace>
  <analyzeFrameBuffer>false</analyzeFrameBuffer>
  <eliminateOverlaps>true</eliminateOverlaps>
  <coverageTarget>$target</coverageTarget>
  <createClkModule>false</createClkModule>
  <dumpGateComments>false</dumpGateComments>
  <a2dffInfo />
  <rewriteLog />
  <removeDeadNodes>true</removeDeadNodes>
  <ignorePins />
  <computeDistances>false</computeDistances>
  <analyzeCommonInputs>true</analyzeCommonInputs>
  <instanceDump />
  <interactivePropagator>false</interactivePropagator>
  <dumpWords/>
  <bigModules>false</bigModules>
  <noPads>true</noPads>
  <removeNOR2XB />
  <countPaths>false</countPaths>
  <createUnknownInputs>false</createUnknownInputs>
  <simpleWordAnalysis>false</simpleWordAnalysis>
  <ignoreScanInputs>false</ignoreScanInputs>
  <renameWiresInModule>true</renameWiresInModule>
  <computeRepresentativeModules>
  false</computeRepresentativeModules>
  <wordTrace>false</wordTrace>
  <NCextract>false</NCextract>
  <mergeModules>true</mergeModules>
  <kcoverSize>6</kcoverSize>
  <kcoverTh>6</kcoverTh>
  <aggregation>true</aggregation>
  <simpleFIFOAnalysis>false</simpleFIFOAnalysis>
  <simpleFIFOAnalysisOut />
  <treeSummaryOut />
  <fullFunctionScript />
  <candidateModulesBound>false</candidateModulesBound>
  <latchAnalysis>false</latchAnalysis>
  <candidateWordToModule>false</candidateWordToModule>
  <candidateLibMatch>false</candidateLibMatch>
  <coloringWithDepth>false</coloringWithDepth>
  <circuitGraphOut>ckt</circuitGraphOut>
  <verilogOutFile>results/verilog/$$name_nooverlap_target_${suffix}</verilogOutFile>
  <summaryOutFile>results/$$name_nooverlap_target_${suffix}.summary</summaryOutFile>
  <candidateOutFile />
  <partialFunctionOutFile />
  <counterAnalysis>true</counterAnalysis>
  <counterAnalysisMaxCounterSize>8</counterAnalysisMaxCounterSize>
  <counterAnalysisLatchesToIgnore />
  <shiftregAnalysis>true</shiftregAnalysis>
  <enablePropagation>false</enablePropagation>
  <minMuxSize>4</minMuxSize>
  <partialFunctionAnalysis>false</partialFunctionAnalysis>
  <signalFlowAnalysis>false</signalFlowAnalysis>
  <conflictAnalysis>false</conflictAnalysis>
  <rewriteBuffers>true</rewriteBuffers>
  <ramAnalysis>true</ramAnalysis>
  <resetSignal>rst</resetSignal>
  <resetPolarity>0</resetPolarity>
  <vcdFileName />
  <undrivenSignals />
  <clockTreeRoots />
  <resetTreeRoots />
  <resetTreeModules />
  <pinGroups />
  <backPropagationLevels>10</backPropagationLevels>
  <backPropagation />
  <userDefinedModules />
  <testCktTechLib>12soi</testCktTechLib>
  <libCktsTechLib>12soi</libCktsTechLib>
  <bitsliceMatchFile />
  <libraryElements />
  <bitslices />
  <partialFuncModules />
  <freqBasedNC>false</freqBasedNC>
  <freqWordDetect>false</freqWordDetect>
  <sliceableILP>true</sliceableILP>
  <dumpOnlyAdderWords>true</dumpOnlyAdderWords>
  <minimizeModuleCount>false</minimizeModuleCount>
</BSIMConfig>
"""
#    <verilogOutFile>results/verilog/$$name_nooverlap_target_${suffix}</verilogOutFile>
#    <summaryOutFile>results/$$name_nooverlap_target_${suffix}.summary</summaryOutFile>
#    <coverageTarget>$target</coverageTarget>

from string import Template

def getGates(filename):
    with open(filename, 'rt') as fileobj: 
        sf = summary_file(filename, fileobj)
        return sf.achieved_coverage

gates = getGates(sys.argv[1])
print gates
cfgTemplate = Template(baseConfig)
points = [0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 0.975, 1]
for p in points:
    suffix = '%g' % p
    target = '%d' % int(p * gates)
    filename = 'configs/test_ov_%s.xml' % suffix

    mapping = { 'suffix': suffix, 'target':target }

    print 'creating', filename, '...'
    cfg = cfgTemplate.substitute(mapping)
    with open(filename, 'wt') as fileobj:
        print >> fileobj, cfg
