import sys
import os
from collections import namedtuple

import numpy as np
import matplotlib.pyplot as plt

# ls results/*_target*.summary | grep -v itag1b | grep -v voting_machine | grep -v 12SOI_router | xargs echo python scripts/parse-summary-plot-overlap.py
# python scripts/parse-summary-plot-overlap.py results/12SOI_evoter_synthesis_flat_nooverlap_target_0.65.summary results/12SOI_evoter_synthesis_flat_nooverlap_target_0.6.summary results/12SOI_evoter_synthesis_flat_nooverlap_target_0.75.summary results/12SOI_evoter_synthesis_flat_nooverlap_target_0.7.summary results/12SOI_evoter_synthesis_flat_nooverlap_target_0.85.summary results/12SOI_evoter_synthesis_flat_nooverlap_target_0.8.summary results/12SOI_evoter_synthesis_flat_nooverlap_target_0.95.summary results/12SOI_evoter_synthesis_flat_nooverlap_target_0.975.summary results/12SOI_evoter_synthesis_flat_nooverlap_target_0.9.summary results/12SOI_evoter_synthesis_flat_nooverlap_target_1.summary results/ae18_core_flat_nooverlap_target_0.65.summary results/ae18_core_flat_nooverlap_target_0.6.summary results/ae18_core_flat_nooverlap_target_0.75.summary results/ae18_core_flat_nooverlap_target_0.7.summary results/ae18_core_flat_nooverlap_target_0.85.summary results/ae18_core_flat_nooverlap_target_0.8.summary results/ae18_core_flat_nooverlap_target_0.95.summary results/ae18_core_flat_nooverlap_target_0.975.summary results/ae18_core_flat_nooverlap_target_0.9.summary results/ae18_core_flat_nooverlap_target_1.summary results/cpu8080_flat_nooverlap_target_0.65.summary results/cpu8080_flat_nooverlap_target_0.6.summary results/cpu8080_flat_nooverlap_target_0.75.summary results/cpu8080_flat_nooverlap_target_0.7.summary results/cpu8080_flat_nooverlap_target_0.85.summary results/cpu8080_flat_nooverlap_target_0.8.summary results/cpu8080_flat_nooverlap_target_0.95.summary results/cpu8080_flat_nooverlap_target_0.975.summary results/cpu8080_flat_nooverlap_target_0.9.summary results/cpu8080_flat_nooverlap_target_1.summary results/mips_16_core_top_flat_nooverlap_target_0.65.summary results/mips_16_core_top_flat_nooverlap_target_0.6.summary results/mips_16_core_top_flat_nooverlap_target_0.75.summary results/mips_16_core_top_flat_nooverlap_target_0.7.summary results/mips_16_core_top_flat_nooverlap_target_0.85.summary results/mips_16_core_top_flat_nooverlap_target_0.8.summary results/mips_16_core_top_flat_nooverlap_target_0.95.summary results/mips_16_core_top_flat_nooverlap_target_0.975.summary results/mips_16_core_top_flat_nooverlap_target_0.9.summary results/mips_16_core_top_flat_nooverlap_target_1.summary results/oc8051_top_flat_nooverlap_target_0.65.summary results/oc8051_top_flat_nooverlap_target_0.6.summary results/oc8051_top_flat_nooverlap_target_0.75.summary results/oc8051_top_flat_nooverlap_target_0.7.summary results/oc8051_top_flat_nooverlap_target_0.85.summary results/oc8051_top_flat_nooverlap_target_0.8.summary results/oc8051_top_flat_nooverlap_target_0.95.summary results/oc8051_top_flat_nooverlap_target_0.975.summary results/oc8051_top_flat_nooverlap_target_0.9.summary results/oc8051_top_flat_nooverlap_target_1.summary results/Open8_CPU_flat_nooverlap_target_0.65.summary results/Open8_CPU_flat_nooverlap_target_0.6.summary results/Open8_CPU_flat_nooverlap_target_0.75.summary results/Open8_CPU_flat_nooverlap_target_0.7.summary results/Open8_CPU_flat_nooverlap_target_0.85.summary results/Open8_CPU_flat_nooverlap_target_0.8.summary results/Open8_CPU_flat_nooverlap_target_0.95.summary results/Open8_CPU_flat_nooverlap_target_0.975.summary results/Open8_CPU_flat_nooverlap_target_0.9.summary results/Open8_CPU_flat_nooverlap_target_1.summary results/risc_fpu_flat2_nooverlap_target_0.65.summary results/risc_fpu_flat2_nooverlap_target_0.6.summary results/risc_fpu_flat2_nooverlap_target_0.75.summary results/risc_fpu_flat2_nooverlap_target_0.7.summary results/risc_fpu_flat2_nooverlap_target_0.85.summary results/risc_fpu_flat2_nooverlap_target_0.8.summary results/risc_fpu_flat2_nooverlap_target_0.95.summary results/risc_fpu_flat2_nooverlap_target_0.975.summary results/risc_fpu_flat2_nooverlap_target_0.9.summary results/risc_fpu_flat2_nooverlap_target_1.summary results/router_flat_nooverlap_target_0.65.summary results/router_flat_nooverlap_target_0.6.summary results/router_flat_nooverlap_target_0.75.summary results/router_flat_nooverlap_target_0.7.summary results/router_flat_nooverlap_target_0.85.summary results/router_flat_nooverlap_target_0.8.summary results/router_flat_nooverlap_target_0.95.summary results/router_flat_nooverlap_target_0.975.summary results/router_flat_nooverlap_target_0.9.summary results/router_flat_nooverlap_target_1.summary
# python scripts/parse-summary-plot-overlap.py results/ae18_core_flat_nooverlap_target_* results/oc8051_top_flat_nooverlap_target_*

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

groups = ['a/s', 'dec', 'dmx', 'eq', 'gf', 'mux', 'lt', 'ram', 'sr', 'cnt', 'reg', 'misc']

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
    'itag1b_bsim_svd':'SVD',
    'itag1b_clean':'BigSoC'
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

    def avg_module_size(self):
        return float(self.achieved_coverage) / float(self.inferred_modules)

# name, gates, latch, a/s, dec, dm, eq, gf, mux, lt, ram, sr, cnt, reg, cov, time, mem

def prettify(filename):
    return filename.replace('results/', '').replace('.summary', '')

def get_group(filename):
    filename = filename.replace('results/', '')
    return filename[:filename.find('_nooverlap')]

def get_target(filename):
    p1 = filename.find('target_')

    assert p1 != -1
    filename = filename[p1:]

    assert filename.find('_') != -1
    filename = filename[filename.find('_')+1:]

    assert filename.find('.summary') != -1
    filename = filename[:filename.find('.summary')]

    return float(filename)

group_data = {}
for arg in sys.argv[1:]:
    grp = get_group(arg)
    if grp not in group_data:
        group_data[grp] = []

    with open(arg, 'rt') as fileobj:
        sumfile = summary_file(prettify(arg), fileobj)
        group_data[grp].append((get_target(arg), sumfile.coverage(), sumfile.avg_module_size()))
        print grp, get_target(arg), sumfile.coverage(), sumfile.avg_module_size()

fig = plt.figure(figsize=(8,3))
plt.subplots_adjust(bottom=0.2)
colors = ['ro-', 'b^-']
i = 0
plts = []
legs = []
for g in group_data:
    group_data[g].sort()
    results = group_data[g]

    coverage = np.array([x[1] for x in results])
    modules_size = np.array([x[2] for x in results])
    plts.append(plt.plot(modules_size, coverage, colors[i], linewidth=3))
    legs.append(name_dict[g])
    i += 1

plt.ylabel('Coverage (% of gates)')
plt.xlabel('Average size of inferred modules')
plt.grid(True)
print legs
#plt.legend((plts[0][0], plts[1][0]), tuple(legs))
plt.savefig('coverage-tradeoff.pdf')



#coverage = np.array([x[0] for x in results])
#modules = np.array([x[1] for x in results])
#
#print coverage
#print modules
#
#plt.plot(modules, coverage, 'ko-', linewidth=3)
#plt.ylabel('Coverage (% of gates)')
#plt.xlabel('Number of inferred modules')
#
#plt.show()
