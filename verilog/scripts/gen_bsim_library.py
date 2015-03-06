import sys
import os

class verilog_module:
    def __init__(self, name, inputs, outputs):
        self.name = name
        self.inputs = inputs
        self.outputs = outputs

    def dump(self, fileobj):
        portlist = ', '.join(self.inputs + self.outputs)
        print >> fileobj, 'module %s ( %s );' % (self.name, portlist)
        for i in self.inputs:
            print >> fileobj, '  input %s;' % i
        for o in self.outputs:
            print >> fileobj, '  output %s;' % o
        print >> fileobj, 'endmodule'

def expand(name, min, max):
    return [('%s%d' % (name,d)) for d in xrange(min, max)]

decoders = []
demuxes = []
for inps in xrange(2,7):
    outs = 1 << inps
    outNames = expand('Y', 0, outs) + expand('YN', 0, outs)
    decoders.append(verilog_module('decoder%d%d' % (inps,outs), expand('X', 0, inps), outNames))
    demuxes.append(verilog_module('demux%d%d' % (inps,outs), ['A'] + expand('B', 0, inps), outNames))

for d in decoders: d.dump(sys.stdout)
for d in demuxes: d.dump(sys.stdout)
