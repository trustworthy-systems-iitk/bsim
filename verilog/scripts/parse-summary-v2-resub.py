import sys
import os
from collections import namedtuple

# this file was written while resubmitting the major revision
# of the tetc13 paper.
# it assumes that for each of the files listed below there is
# <filename>.summary and <filename>_overlap.summary file.

# python scripts/parse-summary-v2-resub.py results/router_flat results/voting_machine_flat results/Open8_CPU_flat results/cpu8080_flat results/ae18_core_flat results/mips_16_core_top_flat results/oc8051_top_flat2 results/risc_fpu_flat2

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
    'voting_machine_flat':'eVoter',
    'Open8_CPU_flat':'Open8',
    'cpu8080_flat':'cpu8080',
    'ae18_core_flat':'ae18',
    'mips_16_core_top_flat':'MIPS16',
    'oc8051_top_flat':'oc8051',
    'oc8051_top_flat2':'oc8051',
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

    def print_result_table(self, header):
        gi1 = ['\\multirow{2}{*}{%d}' % i for i in [self.gates, self.latches]] if header else ['', '']
        gi2 = ['%d' % self.groups[g][0] for g in groups] 
        gi3 = ['%.0f\\%%' % self.coverage()] + ['%.0fs' % self.cpu_time] + ['%.1f' % self.maxrss]

        if not header:
            gi2 = add_bold(gi2)
            gi3 = add_bold(gi3)

        print ' & '.join((['\\multirow{2}{*}{%s}' % self.name] if header else ['']) + gi1 + gi2 + gi3) + '\\\\'
        if not header: print '\\hline'

    def coverage(self):
        return float(self.gates_covered) / float(self.gates) * 100

# name, gates, latch, a/s, dec, dm, eq, gf, mux, lt, ram, sr, cnt, reg, cov, time, mem

def add_bold(l):
    l2 = []
    for w in l:
        w = '\\cellcolor[gray]{0.75}{%s}' % w
        l2.append(w)
    return l2

def prettify(filename):
    return filename.replace('results/', '').replace('.summary', '')

for arg in sys.argv[1:]:
    f1 = arg + '.summary'
    with open(f1, 'rt') as fileobj:
        sumfile = summary_file(prettify(arg), fileobj)
        sumfile.print_info_table()

for arg in sys.argv[1:]:
    f1 = arg + '.summary'
    f2 = arg + '_overlap.summary'
    with open(f2, 'rt') as fileobj:
        sumfile = summary_file(prettify(arg), fileobj)
        sumfile.print_result_table(True)
    with open(f1, 'rt') as fileobj:
        sumfile = summary_file(prettify(arg), fileobj)
        sumfile.print_result_table(False)
