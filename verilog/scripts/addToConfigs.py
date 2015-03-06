import sys
import xml.etree.ElementTree as ET

def add(filename):
    try:
        tree = ET.parse(filename)
        root = tree.getroot()
    except ET.ParseError, e:
	print filename
        raise e

    # elements to add.
    elem = ET.Element('minMultibitElementSize')
    elem.text = '2'
    root.append(elem)

    # elem = ET.Element('addFakeConnections')
    # elem.text = 'false'
    # root.append(elem)

    # elem = ET.Element('refFileName')
    # elem.text = ''
    # root.append(elem)

    # elements to add.
    # elem = ET.Element('mergeModules2')
    # elem.text = 'false'
    # root.append(elem)

    # elements to add.
    # elem = ET.Element('sliceableILP')
    # elem.text = 'true'
    # root.append(elem)

    # elements to add.
    # elem = ET.Element('freqBasedNC')
    # elem.text = 'false'
    # root.append(elem)

    # elements to add.
    # elem = ET.Element('freqWordDetect')
    # elem.text = 'false'
    # root.append(elem)

    tree.write(filename)
    
def main(argv):
    for filename in argv[1:]:
        add(filename)

if __name__ == '__main__':
    main(sys.argv)
