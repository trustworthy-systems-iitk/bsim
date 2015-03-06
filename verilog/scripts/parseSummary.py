#! /usr/bin/python

import sys

def main(argv):
    # labels = ['des', 'i', 'o', 'g', 'l'] + module_types
    # print ' & '.join(labels) + '& cov \\\\ \hline'

    gates = 0
    coverage = 0
    time = 0
    for arg in argv[1:]:
        c, g, t = process(arg)
        coverage += c
        gates += g
        arg = arg.replace('results/', '')
        arg = arg.replace('.summary', '')
        arg = arg.replace('_overlap', '')
        # print '%-40s %7d %7d %7.0f' % (arg, c, g, float(c)/float(g)*100)
        time += t
    print 'total cover: %d %d, %0.f %d' % (gates, coverage, float(coverage)/ float(gates) * 100, time)


def process(filename):
    counts = {}
    found = False
    for line in open(filename):
        if line.strip(): 
            pos = line.rfind(':')
            p1 = line[:pos].strip()
            p2 = line[pos+1:].strip()
            if line.find('number of gates covered') != -1:
                found = True
            elif p1 == '# of inputs':
                inputs = int(p2)
            elif p1 == '# of outputs':
                outputs = int(p2)
            elif p1 == '# of latches':
                latches = int(p2)
            elif p1 == '# of gates':
                gates = int(p2)
            elif p1 == 'total time taken':
                time = float(p2)
            elif p1.startswith('module:'):
                if found: continue

                mtype = get_module_type(p1)
                if mtype in counts: counts[mtype] += int(p2)
                else: counts[mtype] = int(p2)
            elif p1 == '# total gates covered':
                coverage = int(p2)
                coverage_pct = float(coverage) / gates * 100
    assert found

    filename = filename.replace('results/', '')
    filename = filename.replace('.summary', '')
    filename = filename.replace('_overlap', '')

    def g(n):
        if n in counts: return counts[n]
        else: return 0

    intlist = [inputs, outputs, gates, latches]+[g(n) for n in module_types]
    # strlist = [str(x) for x in intlist] + ['%.0f\\%%' % coverage_pct, '%.0fs' % time]
    strlist = [str(x) for x in intlist] + ['%.0f%%' % coverage_pct, '%.0fs' % time]
    # print filename + '\n' + ' & '.join(strlist) + ' &'
    print ','.join([filename] + strlist)
    return (coverage, gates, time)

module_types = [
    'a/s',
    'dec',
    'dmx',
    'eq',
    'gf',
    'mux',
    'lt',
    'ram',
    'sr',
    'cnt',
]

def get_module_type(mn):
    words = mn.split()
    if words[1].startswith('mux'): return 'mux'
    elif words[1].startswith('decoder'): return 'dec'
    elif words[1].startswith('demux'): return 'dmx'
    elif words[1].endswith('gate'): return 'gf'
    elif words[1] == 'ram': return 'ram'
    elif words[1] == 'upcounter': return 'cnt'
    elif words[1] == 'downcounter': return 'cnt'
    elif words[1] == 'shiftreg': return 'sr'
    elif words[1].startswith('eqCmp') or words[1].startswith('neqCmp'): return 'eq'
    elif words[1] == 'ripple_addsub': return 'a/s'
    elif words[1].endswith('tree'): return 'lt'
    elif words[1].startswith('clktree'): return 'ct'
    elif words[1].startswith('framebuffer_read'): return 'fb'
    else: 
        print words[1]
        assert False

if __name__ == '__main__':
    main(sys.argv)
