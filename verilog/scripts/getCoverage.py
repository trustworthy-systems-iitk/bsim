#! /usr/bin/python

import sys

def main(argv):
    for arg in argv[1:]:
        process(arg)

def process(filename):
    counts = {}
    for line in open(filename):
        if line.strip(): 
            pos = line.rfind(':')
            p1 = line[:pos].strip()
            p2 = line[pos+1:].strip()
            if p1 == '# of gates':
                gates = int(p2)
            elif p1 == '# total gates covered':
                coverage1 = int(p2)
                coverage1_pct = float(coverage1) / gates * 100.00
            elif p1 == '# total gates covered without candidate modules':
                coverage2 = int(p2)
                coverage2_pct = float(coverage2) / gates * 100.00
    print '%30s = [%5.0f, %5.0f, 100.0] : %5.1f' % (cleanup(filename), coverage2_pct, coverage1_pct, coverage2_pct - coverage1_pct)

def cleanup(filename):
    p1 = filename.find('/')
    if p1 != -1: filename = filename[p1+1:]

    p2 = filename.rfind('.')
    if p2 != -1: filename = filename[:p2]

    return filename

module_types = [
    'a/s',
    'dec',
    'dmx',
    'eq',
    'gf',
    'mux',
    'pt',
    'ram',
    'sr',
    'uc',
    'dc',
]

def get_module_type(mn):
    words = mn.split()
    if words[1].startswith('mux'): return 'mux'
    elif words[1].startswith('decoder'): return 'dec'
    elif words[1].startswith('demux'): return 'dmx'
    elif words[1].endswith('gate'): return 'gf'
    elif words[1] == 'ram': return 'ram'
    elif words[1] == 'upcounter': return 'uc'
    elif words[1] == 'downcounter': return 'dc'
    elif words[1] == 'shiftregister': return 'sr'
    elif words[1].startswith('eqCmp') or words[1].startswith('neqCmp'): return 'eq'
    elif words[1] == 'ripple_addsub': return 'a/s'
    elif words[1] == 'xortree': return 'pt'
    else: 
        print words[1]
        assert False

if __name__ == '__main__':
    main(sys.argv)
