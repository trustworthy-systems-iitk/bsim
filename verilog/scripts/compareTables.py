import sys
import re

from readAnnotations import read_verilog
from collections import defaultdict

def readSrcTable(filename):
    with open(filename, 'r') as fileobj:
        modules = defaultdict(set)
        for line in fileobj:
            line = line.strip()
            words = [w.strip() for w in line.split()]
            if len(words) > 1:
                assert len(words) == 2
                mod = words[1]
                inst = words[0]
                modules[mod].add(inst)
        return modules

instances = (read_verilog(sys.argv[1]))
print '# of modules: %d' % len(instances)

modules1 = defaultdict(set)
for i in instances:
    name = i['module']
    modules1[name] = modules1[name].union(i['compmap'])

modules2 = readSrcTable(sys.argv[2])

print len(modules1)
print len(modules2)

for m in modules1:
    i1 = modules1[m]
    i2 = modules2[m]

    d1 = i1.difference(i2)
    d2 = i2.difference(i1)

    if len(d1):
        print 'd1:', m, d1
    if len(d2):
        print 'd2:', m, d2

print 'yes'
