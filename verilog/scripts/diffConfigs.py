import sys
import xml.etree.ElementTree as ET

def check(ref_filename, check_filename):
    ref_tree = ET.parse(ref_filename)
    ref_tags = set([child.tag for child in ref_tree.getroot()])
    check_tree = ET.parse(check_filename)
    check_tags = set([child.tag for child in check_tree.getroot()])
    if ref_tags != check_tags:
        l1 = list(ref_tags - check_tags)
        if l1:
            print ('only in %s: ' % ref_filename), ' '.join(l1)
        l2 = list(check_tags - ref_tags)
        if l2:
            print ('only in %s: ' % check_filename), ' '.join(l2)

    else:
        print 'identical tags.'

def main(argv):
    if len(argv) != 3:
        print 'Syntax error.'
        print 'Usage: %s <ref-xmlfile> <xmlfile-to-check>'
    else:
        check(argv[1], argv[2])

if __name__ == '__main__':
    main(sys.argv)
