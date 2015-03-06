import sys
import os
from collections import namedtuple

# python scripts/parse-summary-v2.py 

files = [
    'router_flat',
    'voting_machine_flat',
    'Open8_CPU_flat',
    'cpu8080_flat',
    'ae18_core_flat',
    'mips_16_core_top_flat',
    'oc8051_top_flat2',
    'risc_fpu_flat2'
]

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

    def print_result_table(self):
        gi1 = ['%d' % i for i in [self.gates, self.latches]] 
        gi2 = ['%d' % self.groups[g][0] for g in groups] 
        gi3 = ['%.0f\\%%' % self.coverage()] + ['%.0fs' % self.cpu_time] + ['%.1f' % self.maxrss]

        print ' & '.join([self.name] + gi1 + gi2 + gi3) + '\\\\'

    def coverage(self):
        return float(self.gates_covered) / float(self.gates) * 100

# name, gates, latch, a/s, dec, dm, eq, gf, mux, lt, ram, sr, cnt, reg, cov, time, mem

def prettify(filename):
    return filename.replace('results/', '').replace('.summary', '')


for f in files:
    f1 = 'results/' + f + '.summary'
    f2 = 'results/' + f + '_unsliceable.summary'

    with open(f1, 'rt') as f1o:
        sumfile = summary_file(name_dict[f], f1o)
        mc1 = sumfile.inferred_modules
        cov1 = sumfile.coverage()
        tim1 = sumfile.cpu_time

    with open(f2, 'rt') as f2o:
        sumfile = summary_file(name_dict[f], f2o)
        mc2 = sumfile.inferred_modules
        cov2 = sumfile.coverage()
        tim2 = sumfile.cpu_time

    mc_del = ((float(mc1) - float(mc2)) / float(mc2)) * 100.0
    cov_del = (float(cov1) - float(cov2))
    print '%-20s & %6d & %5.1f\\%% & %.0fs & %6d & %5.1f\\%% &  %.0fs & %5.1f\\%% \\\\' % (name_dict[f], mc2, cov2, tim2, mc1, cov1, tim1, cov_del)
