import sys
import xml.etree.ElementTree as ET


def remove(filename):
    tree = ET.parse(filename)
    root = tree.getroot()

    #elem = root.find('countPaths')
    #if elem is not None:
    #    root.remove(elem)

    #elem = root.find('noPads')
    #if elem is not None:
    #    root.remove(elem)
    while True:
        elem = root.find('minMultibitElementSize')
        if elem is None:
            break
        else: 
            root.remove(elem)

    elem = ET.Element('minMultibitElementSize')
    elem.text = '2'
    root.append(elem)

    tree.write(filename)
    
def main(argv):
    for filename in argv[1:]:
        remove(filename)

if __name__ == '__main__':
    main(sys.argv)

