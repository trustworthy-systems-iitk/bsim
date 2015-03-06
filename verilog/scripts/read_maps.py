with open('maps.txt', 'r') as mapfile:
  gateset = set()
  for line in mapfile:
      line = line.strip()
      if not line: continue

      start = line.find('<compmap>')
      assert start == 0
      start += len('<compmap>')
      end = line.find('</compmap>')
      assert end != -1
      line = line[start:end].strip()
      words_stripped = [w.strip() for w in line.split(',')]
      words = [w for w in words_stripped if len(w) > 0]
      for w in words:
          components = w.strip().split(':')
          assert len(components) == 2, components
          tag,name = components[0], components[1]
          assert tag.startswith('F'), tag
          assert name not in gateset, name
          gateset.add(name)

  print len(gateset)

