import sys
import os
from collections import namedtuple
import itertools

import numpy as np
import matplotlib.pyplot as plt

# python scripts/parse-summary-v2.py results/router_flat.summary results/12SOI_evoter_synthesis_flat.summary results/Open8_CPU_flat.summary results/cpu8080_flat.summary results/ae18_core_flat.summary results/mips_16_core_top_flat.summary results/oc8051_top_flat.summary results/risc_fpu_flat2.summary

def get_module_type2(mtyp):
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

comb_groups = ['a/s', 'dec', 'dmx', 'eq', 'gf', 'mux', 'lt']
seq_groups = ['ram', 'sr', 'cnt', 'reg']

def get_module_type(mtyp):
    if mtyp.startswith('mux'): return 'mux'
    elif mtyp.startswith('decoder'): return 'dec'
    elif mtyp.startswith('demux'): return 'dec'
    elif mtyp.endswith('gate'): return 'gf'
    elif mtyp == 'ram': return 'ram'
    elif mtyp == 'counter': return 'others'
    elif mtyp == 'upcounter': return 'others'
    elif mtyp == 'downcounter': return 'others'
    elif mtyp == 'shiftreg': return 'others'
    elif mtyp.startswith('eqCmp') or mtyp.startswith('neqCmp'): return 'others'
    elif mtyp == 'ripple_addsub': return 'a/s'
    elif mtyp.endswith('tree'): return 'others'
    elif mtyp.startswith('clktree'): return 'misc'
    elif mtyp.startswith('reg'): return 'reg'
    elif mtyp.startswith('framebuffer_read'): return 'misc'
    elif mtyp.startswith('merged_module'): return 'misc'
    else: 
        print mtyp
        assert False

groups = ['mux', 'gf', 'ram', 'reg', 'a/s', 'dec', 'others']
group_names = {
    'a/s': 'adders/subtracters',
    'dec': 'decoders and demuxes',
    'gf': 'gating functions',
    'mux': 'muxes',
    'ram': 'rf',
    'reg': 'multibit regs',
    'others': 'others',
    'unknown': 'not covered'
}

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
        self.compute_groups2()

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

    def compute_groups2(self):
        self.comb_groups = {}
        self.seq_groups = {}
        for g in comb_groups:
            self.comb_groups[g] = (0,0)
        for g in seq_groups:
            self.seq_groups[g] = (0,0)

        for m in self.modules:
            gt = get_module_type2(m)
            if gt in comb_groups:
                (c, g) = self.comb_groups[gt]
                c += self.modules[m].count
                g += self.modules[m].gates
                self.comb_groups[gt] = (c, g)
            elif gt in seq_groups:
                (c, g) = self.seq_groups[gt]
                c += self.modules[m].count
                g += self.modules[m].gates
                self.seq_groups[gt] = (c, g)
            else:
                print gt
                assert False

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

    def print_comb_table(self):
        gi1 = ['%d' % i for i in [self.gates, self.latches]] 
        gi2 = ['%d' % self.comb_groups[g][0] for g in comb_groups] 
        gi3 = ['%.0f\\%%' % self.coverage()] + ['%.0fs' % self.cpu_time]
        grp = [self.name] + gi1 + gi2 + gi3
        print ' & '.join(grp) + '\\\\'

    def print_seq_table(self):
        gi1 = ['%d' % i for i in [self.gates, self.latches]] 
        gi2 = ['%d' % self.seq_groups[g][0] for g in seq_groups] 
        gi3 = ['%.0f\\%%' % self.coverage()] + ['%.0fs' % self.cpu_time]
        print ' & '.join([self.name] + gi1 + gi2 + gi3) + '\\\\'

    def print_result_table(self):
        gi1 = ['%d' % i for i in [self.gates, self.latches]] 
        gi2 = ['%d' % self.groups[g][0] for g in groups] 
        gi3 = ['%.0f\\%%' % self.coverage()] + ['%.0fs' % self.cpu_time] + ['%.1f' % self.maxrss]

        print ' & '.join([self.name] + gi1 + gi2 + gi3) + '\\\\'

    def coverage(self):
        return float(self.gates_covered) / float(self.gates) * 100

    def coverage(self):
        return float(self.gates_covered) / float(self.gates) * 100

    def get_fractional_coverage(self):
        assert sum(self.groups[g][1] for g in groups) == self.gates_covered
        fractions = [float(self.groups[g][1])/float(self.gates) for g in groups]
        #fractions.append(1-sum(fractions))
        labels = groups #+ ['unknown']
        return labels, fractions

# name, gates, latch, a/s, dec, dm, eq, gf, mux, lt, ram, sr, cnt, reg, cov, time, mem

files = ['results/router_flat_unsliceable.summary', 'results/12SOI_evoter_synthesis_flat_unsliceable.summary', 'results/Open8_CPU_flat_unsliceable.summary', 'results/cpu8080_flat_unsliceable.summary', 'results/ae18_core_flat_unsliceable.summary', 'results/mips_16_core_top_flat_unsliceable.summary', 'results/oc8051_top_flat_unsliceable.summary', 'results/risc_fpu_flat2_unsliceable.summary']
files = ['results/router_flat.summary', 'results/12SOI_evoter_synthesis_flat.summary', 'results/Open8_CPU_flat.summary', 'results/cpu8080_flat.summary', 'results/ae18_core_flat.summary', 'results/mips_16_core_top_flat.summary', 'results/oc8051_top_flat.summary', 'results/risc_fpu_flat2.summary']

def prettify(filename):
    fn = filename.replace('results/', '').replace('.summary', '').replace('_unsliceable','')
    if fn in name_dict: return name_dict[fn]
    else: return fn

labels = ['Design', 'gates', 'latches'] + comb_groups + ['cov', 'time']
print ' & '.join(labels) + '\\\\'
for arg in files:
    with open(arg, 'rt') as fileobj:
        sf = summary_file(prettify(arg), fileobj)
        sf.print_comb_table()


labels = ['Design', 'gates', 'latches'] + seq_groups + ['cov', 'time']
print ' & '.join(labels) + '\\\\'
for arg in files:
    with open(arg, 'rt') as fileobj:
        sf = summary_file(prettify(arg), fileobj)
        sf.print_seq_table()

#sys.exit(0)

bars = {}
names = []
gates = []
for arg in files:
    with open(arg, 'rt') as fileobj:
        sf = summary_file(prettify(arg), fileobj)
        gates.append(sf.gates)
        names.append(prettify(arg))
        labels,fractions = sf.get_fractional_coverage()
        assert labels == groups # + ['unknown']
        for l,f in itertools.izip(labels, fractions):
            if l in bars:
                bars[l].append(f)
            else:
                bars[l] = [f]


ys = np.arange(0, len(files))
left = np.array([0]*len(files))

print ys
print labels
print left

colors = ['OliveDrab', 'FireBrick', 'Gold', 'SpringGreen', 'SlateBlue', 'MediumVioletRed', 'Gray', 'GhostWhite', 'DeepPink', 'DarkTurquoise', 'DarkSeaGreen']


fig = plt.figure(figsize=(9,6))
ax = fig.add_subplot(111)
rects = []
for i, l in enumerate(labels):
    b = np.array(bars[l]) * 100
    r = ax.barh(ys, b, left=left, color=colors[i])
    rects.append(r[0])
    left = left + b
    prev_left = left


yticks = np.arange(0, len(files)) + 0.5
ax.set_yticks(yticks)
ax.set_yticklabels(names)

xticks = xrange(0,120,20)
xticklabels = ['%d%%' % i for i in xticks]
print xticks, xticklabels
ax.set_xticks(xticks)
ax.set_xticklabels(xticklabels)

ax.set_xlim((0,100))
ax.set_ylim((0, 3+len(files)))

ax.legend(rects,[group_names[l] for l in labels],loc='upper left', ncol=2)
ax.set_xlabel('Percentage of gates covered')
ax.grid(True, axis='x', linestyle=':')

for i, y in enumerate(ys):
    wid = prev_left[i]
    pos = wid+1 if wid < 85 else 50
    ax.text(pos, ys[i]+0.35, '%.0f%% of %d gates' % (wid, gates[i]), ha='left', va='center')

plt.savefig('coverage-bar.pdf', bbox_inches='tight')
#plt.show()
