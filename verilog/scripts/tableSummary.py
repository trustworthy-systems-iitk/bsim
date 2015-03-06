#! /usr/bin/python

import sys

def main(argv):
    for arg in argv[1:]:
        process(arg)


def process(filename):
    mod_counts = {}
    mod_gates = {}
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
            elif p1.startswith('module:'):
                key = p1.split()[1]
                if not found:
                    mod_counts[key] = int(p2)
                else:
                    mod_gates[key] = int(p2)

            elif p1 == '# total gates covered':
                coverage = int(p2)
                coverage_pct = float(coverage) / gates * 100
    assert found
    
    groups = {}
    for k in mod_counts:
        count = mod_counts[k]
        gates = mod_gates[k]

        typ, grp = translate(k)
        if grp in groups: info = groups[grp]
        else: info = {}

        if typ in info:
            cs, gs = info[typ]
        else:
            cs, gs = 0, 0

        cs += count
        gs += gates
        info[typ] = cs, gs
        groups[grp] = info


    for g in sorted(groups.keys()):
        dump_group(g, groups[g])


def dump_group(g, info):
    print '%s' % g
    tc, tg = 0, 0
    for t in sorted(info.keys()):
        cs, gs = info[t]
        print '%s,%d,%d' % (t, cs, gs)
        tc, tg = (tc+cs, tg+gs)
    print '%s,%d,%d' % ('Total', tc, tg) 

def translate(s):

    if s.endswith('tree'):
        return s[:s.find('tree')], 'Logic Trees'
    if s.endswith('gate'):
        return s[:s.find('gate')-1], 'Gating Functions'
    if s.startswith('clktree'):
        return s[8:], 'Clock Trees'
    if s.startswith('decoder'):
        s =  s[7:]
        s = s[:1] + ':' + s[1:] + ' decoder'
        return s, 'Decoders'
    if s.startswith('mux'):
        s = s[3:5]
        s = s[0] + ":" + s[1] + " MUX"
        return s, 'Multiplexers'
    if s.startswith('demux'):
        s =  s[5:]
        s = s[:1] + ':' + s[1:] + ' demux'
        return s, 'Demultiplexers'
    if s == 'ram':
        return 'ram', 'RAM Arrays'
    if s.startswith('eqCmp'):
        size = int(s[-1])
        return ('%d-bit equality comparators' % size, 'Equality/Inequality Comparators')
    if s.startswith('neqCmp'):
        size = int(s[-1])
        return ('%d-bit inequality comparators' % size, 'Equality/Inequality Comparators')
    if s == 'ripple_addsub':
        return 'Ripple Carry Adders/Subtractors', 'Adders/Subtractors'
    
    if s == 'shiftregister':
        return 'shiftregister', 'Shift Registers'
    assert False
    


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

if __name__ == '__main__':
    main(sys.argv)
