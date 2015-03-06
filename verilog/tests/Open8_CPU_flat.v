
module Open8_CPU ( clock, reset, cpu_halt, interrupts, address, rd_data, 
        rd_enable, wr_data, wr_enable );
  input [7:0] interrupts;
  output [15:0] address;
  input [7:0] rd_data;
  output [7:0] wr_data;
  input clock, reset, cpu_halt;
  output rd_enable, wr_enable;
  wire   offset_sx_15, n1245, n1246, n1247, n1248, n1249, n1250, n1251, n1252,
         n1253, n1254, n1255, n1256, n1257, n1258, n1259, n1260, int_req,
         n14660, n14670, n14680, n14690, n14700, n14710, n14720, n14730,
         n14740, n14750, n14760, n14770, n14780, n14790, n14800, n14810,
         instr_prefetch, ack_q, ack_q1, int_ack, int_rti, wait_for_fsm, n15680,
         n15690, n15700, n15710, n15720, n15730, n15740, n15750, n15760,
         n15770, n15780, n15790, n15800, n15810, n15820, n15830, n15840,
         n15850, n15860, n15870, n15880, n15890, n15900, n15910, n15920,
         n15930, n15940, n15950, n15960, n15970, n15980, n15990, n16870,
         n16880, n16890, n16900, n16910, n16920, n16930, n16940, n16950,
         n16960, n16970, n16980, n16990, n17000, n17010, n17020, n21420,
         n21560, n21570, n21580, n21590, n21600, n21610, n21620, n21630,
         n21640, n21650, n21660, n21670, n21680, n21690, n21700, n21710,
         n24760, n25120, n25480, n2794, n2795, n2796, n2797, n2798, n2799,
         n2800, n2801, n2877, n2881, n2970, n2971, n2972, n2973, n2974, n2975,
         n2976, n2977, n2978, n2979, n72, n73, n74, n75, n76, n77, n78, n79,
         n80, n81, u4_u1_z_0, u4_u1_z_1, u4_u1_z_2, u4_u1_z_3, u4_u1_z_4,
         u4_u1_z_5, u4_u1_z_6, u4_u1_z_7, u4_u1_z_8, u4_u1_z_9, u4_u1_z_10,
         u4_u1_z_11, u4_u1_z_12, u4_u1_z_13, u4_u1_z_14, u4_u1_z_15, u4_u2_z_0,
         u4_u2_z_1, u4_u2_z_2, u4_u2_z_3, u4_u2_z_4, u4_u2_z_5, u4_u2_z_6,
         u4_u2_z_7, u4_u2_z_8, u4_u2_z_9, u4_u2_z_10, u4_u2_z_11, u4_u2_z_12,
         u4_u2_z_13, u4_u2_z_14, u4_u2_z_15, u4_u4_z_0, u4_u6_z_0, u4_u6_z_1,
         u4_u6_z_2, u4_u6_z_3, u4_u6_z_4, u4_u6_z_5, u4_u6_z_6, u4_u6_z_7,
         u4_u7_z_0, u4_u7_z_1, u4_u7_z_2, u4_u7_z_3, u4_u7_z_4, u4_u7_z_5,
         u4_u7_z_6, u4_u7_z_7, u4_u8_z_0, n266, n267, n268, n269, n270, n271,
         n272, n273, n274, n275, n276, n277, n278, n279, n280, n281, n282,
         n283, n284, n285, n286, n287, n288, n289, n290, n291, n292, n293,
         n294, n295, n296, n297, n298, n299, n300, n301, n302, n303, n304,
         n305, n306, n307, n308, n309, n310, n311, n312, n313, n314, n315,
         n316, n317, n318, n319, n320, n321, n322, n323, n324, n326, n327,
         n328, n329, n487, n488, n489, n490, n491, n492, n1292, n1293, n1294,
         n1295, n1296, n1297, n1298, n1299, n1300, n1301, n1302, n1303, n1304,
         n1305, n1306, n1307, n1308, n1309, n1310, n1311, n1312, n1313, n1314,
         n1315, n1316, n1317, n1318, n1319, n1320, n1321, n1322, n1323, n1324,
         n1325, n1326, n1327, n1328, n1329, n1330, n1331, n1332, n1333, n1334,
         n1335, n1336, n1337, n1338, n1339, n1340, n1341, n1342, n1343, n1344,
         n1345, n1346, n1347, n1348, n1349, n1350, n1351, n1352, n1353, n1354,
         n1355, n1356, n1357, n1358, n1359, n1360, n1361, n1362, n1363, n1364,
         n1365, n1366, n1367, n1368, n1369, n1370, n1371, n1372, n1373, n1374,
         n1375, n1376, n1377, n1378, n1379, n1380, n1381, n1382, n1383, n1384,
         n1385, n1386, n1387, n1388, n1389, n1390, n1391, n1392, n1393, n1394,
         n1395, n1396, n1397, n1398, n1399, n1400, n1401, n1402, n1403, n1404,
         n1405, n1406, n1407, n1408, n1409, n1410, n1411, n1412, n1413, n1414,
         n1415, n1416, n1417, n1418, n1419, n1420, n1421, n1422, n1423, n1424,
         n1425, n1426, n1427, n1428, n1429, n1430, n1431, n1432, n1433, n1434,
         n1435, n1436, n1437, n1438, n1439, n1440, n1441, n1442, n1443, n1444,
         n1445, n1446, n1447, n1448, n1449, n1450, n1451, n1452, n1453, n1454,
         n1455, n1456, n1457, n1458, n1459, n1460, n1461, n1462, n1463, n1464,
         n1465, n14661, n14671, n14681, n14691, n14701, n14711, n14721, n14731,
         n14741, n14751, n14761, n14771, n14781, n14791, n14801, n14811, n1482,
         n1483, n1484, n1485, n1486, n1487, n1488, n1489, n1490, n1491, n1514,
         n1515, n1516, n1517, n1518, n1519, n1520, n1521, n1522,
         r1183_carry_2_, n1523, n1524, n1525, n1526, n1527, n1528, n1529,
         n1530, n1531, n1532, n1533, n1534, n1535, n1536, n1537, n1538, n1539,
         n1540, n1541, n1542, n1543, n1544, n1545, n1546, n1547, n1548, n1549,
         n1550, n1551, n1552, n1553, n1554, n1555, n1556, n1557, n1558, n1559,
         n1560, n1561, n1562, n1563, n1564, n1565, n1566, n1567, n15681,
         n15691, n15701, n15711, n15721, n15731, n15741, n15751, n15761,
         n15771, n15781, n15791, n15801, n15811, n15821, n15831, n15841,
         n15851, n15861, n15871, n15881, n15891, n15901, n15911, n15921,
         n15931, n15941, n15951, n15961, n15971, n15981, n15991, n1600, n1601,
         n1602, n1603, n1604, n1605, n1606, n1607, n1608, n1609, n1610, n1611,
         n1612, n1613, n1614, n1615, n1616, n1617, n1618, n1619, n1620, n1621,
         n1622, n1623, n1624, n1625, n1626, n1627, n1628, n1629, n1630, n1631,
         n1632, n1633, n1634, n1635, n1636, n1637, n1638, n1639, n1640, n1641,
         n1642, n1643, n1644, n1645, n1646, n1647, n1648, n1649, n1650, n1651,
         n1652, n1653, n1654, n1655, n1656, n1657, n1658, n1659, n1660, n1661,
         n1662, n1663, n1664, n1665, n1666, n1667, n1668, n1669, n1670, n1671,
         n1672, n1673, n1674, n1675, n1676, n1677, n1678, n1679, n1680, n1681,
         n1682, n1683, n1684, n1685, n1686, n16871, n16881, n16891, n16901,
         n16911, n16921, n16931, n16941, n16951, n16961, n16971, n16981,
         n16991, n17001, n17011, n17021, n1703, n1704, n1705, n1706, n1707,
         n1708, n1709, n1710, n1711, n1712, n1713, n1714, n1715, n1716, n1717,
         n1718, n1719, n1720, n1721, n1722, n1723, n1724, n1725, n1726, n1727,
         n1728, n1729, n1730, n1731, n1732, n1733, n1734, n1735, n1736, n1737,
         n1738, n1739, n1740, n1741, n1742, n1743, n1744, n1745, n1746, n1747,
         n1748, n1749, n1750, n1751, n1752, n1753, n1754, n1755, n1756, n1757,
         n1758, n1759, n1760, n1761, n1762, n1763, n1764, n1765, n1766, n1767,
         n1768, n1769, n1770, n1771, n1772, n1773, n1774, n1775, n1776, n1777,
         n1778, n1779, n1780, n1781, n1782, n1783, n1784, n1785, n1786, n1787,
         n1788, n1789, n1790, n1791, n1792, n1793, n1794, n1795, n1796, n1797,
         n1798, n1799, n1800, n1801, n1802, n1803, n1804, n1805, n1806, n1807,
         n1808, n1809, n1810, n1811, n1812, n1813, n1814, n1815, n1816, n1817,
         n1818, n1819, n1820, n1821, n1822, n1823, n1824, n1825, n1826, n1827,
         n1828, n1829, n1830, n1831, n1832, n1833, n1834, n1835, n1836, n1837,
         n1838, n1839, n1840, n1841, n1842, n1843, n1844, n1845, n1846, n1847,
         n1848, n1849, n1850, n1851, n1852, n1853, n1854, n1855, n1856, n1857,
         n1858, n1859, n1860, n1861, n1862, n1863, n1864, n1865, n1866, n1867,
         n1868, n1869, n1870, n1871, n1872, n1873, n1874, n1875, n1876, n1877,
         n1878, n1879, n1880, n1881, n1882, n1883, n1884, n1885, n1886, n1887,
         n1888, n1889, n1890, n1891, n1892, n1893, n1894, n1895, n1896, n1897,
         n1898, n1899, n1900, n1901, n1902, n1903, n1904, n1905, n1906, n1907,
         n1908, n1909, n1910, n1911, n1912, n1913, n1914, n1915, n1916, n1917,
         n1918, n1919, n1920, n1921, n1922, n1923, n1924, n1925, n1926, n1927,
         n1928, n1929, n1930, n1931, n1932, n1933, n1934, n1935, n1936, n1937,
         n1938, n1939, n1940, n1941, n1942, n1943, n1944, n1945, n1946, n1947,
         n1948, n1949, n1950, n1951, n1952, n1953, n1954, n1955, n1956, n1957,
         n1958, n1959, n1960, n1961, n1962, n1963, n1964, n1965, n1966, n1967,
         n1968, n1969, n1970, n1971, n1972, n1973, n1974, n1975, n1976, n1977,
         n1978, n1979, n1980, n1981, n1982, n1983, n1984, n1985, n1986, n1987,
         n1988, n1989, n1990, n1991, n1992, n1993, n1994, n1995, n1996, n1997,
         n1998, n1999, n2000, n2001, n2002, n2003, n2004, n2005, n2006, n2007,
         n2008, n2009, n2010, n2011, n2012, n2013, n2014, n2015, n2016, n2017,
         n2018, n2019, n2020, n2021, n2022, n2023, n2024, n2025, n2026, n2027,
         n2028, n2029, n2030, n2031, n2032, n2033, n2034, n2035, n2036, n2037,
         n2038, n2039, n2040, n2041, n2042, n2043, n2044, n2045, n2046, n2047,
         n2048, n2049, n2050, n2051, n2052, n2053, n2054, n2055, n2056, n2057,
         n2058, n2059, n2060, n2061, n2062, n2063, n2064, n2065, n2066, n2067,
         n2068, n2069, n2070, n2071, n2072, n2073, n2074, n2075, n2076, n2077,
         n2078, n2079, n2080, n2081, n2082, n2083, n2084, n2085, n2086, n2087,
         n2088, n2089, n2090, n2091, n2092, n2093, n2094, n2095, n2096, n2097,
         n2098, n2099, n2100, n2101, n2102, n2103, n2104, n2105, n2106, n2107,
         n2108, n2109, n2110, n2111, n2112, n2113, n2114, n2115, n2116, n2117,
         n2118, n2119, n2120, n2121, n2122, n2123, n2124, n2125, n2126, n2127,
         n2128, n2129, n2130, n2131, n2132, n2133, n2134, n2135, n2136, n2137,
         n2138, n2139, n2140, n2141, n21421, n2143, n2144, n2145, n2146, n2147,
         n2148, n2149, n2150, n2151, n2152, n2153, n2154, n2155, n21561,
         n21571, n21581, n21591, n21601, n21611, n21621, n21631, n21641,
         n21651, n21661, n21671, n21681, n21691, n21701, n21711, n2172, n2173,
         n2174, n2175, n2176, n2177, n2178, n2179, n2180, n2181, n2182, n2183,
         n2184, n2185, n2186, n2187, n2188, n2189, n2190, n2191, n2192, n2193,
         n2194, n2195, n2196, n2197, n2198, n2199, n2200, n2201, n2202, n2203,
         n2204, n2205, n2206, n2207, n2208, n2209, n2210, n2211, n2212, n2213,
         n2214, n2215, n2216, n2217, n2218, n2219, n2220, n2221, n2222, n2223,
         n2224, n2225, n2226, n2227, n2228, n2229, n2230, n2231, n2232, n2233,
         n2234, n2235, n2236, n2237, n2238, n2239, n2240, n2241, n2242, n2243,
         n2244, n2245, n2246, n2247, n2248, n2249, n2250, n2251, n2252, n2253,
         n2254, n2255, n2256, n2257, n2258, n2259, n2260, n2261, n2262, n2263,
         n2264, n2265, n2266, n2267, n2268, n2269, n2270, n2271, n2272, n2273,
         n2274, n2275, n2276, n2277, n2278, n2279, n2280, n2281, n2282, n2283,
         n2284, n2285, n2286, n2287, n2288, n2289, n2290, n2291, n2292, n2293,
         n2294, n2295, n2296, n2297, n2298, n2299, n2300, n2301, n2302, n2303,
         n2304, n2305, n2306, n2307, n2308, n2309, n2310, n2311, n2312, n2313,
         n2314, n2315, n2316, n2317, n2318, n2319, n2320, n2321, n2322, n2323,
         n2324, n2325, n2326, n2327, n2328, n2329, n2330, n2331, n2332, n2333,
         n2334, n2335, n2336, n2337, n2338, n2339, n2340, n2341, n2342, n2343,
         n2344, n2345, n2346, n2347, n2348, n2349, n2350, n2351, n2352, n2353,
         n2354, n2355, n2356, n2357, n2358, n2359, n2360, n2361, n2362, n2363,
         n2364, n2365, n2366, n2367, n2368, n2369, n2370, n2371, n2372, n2373,
         n2374, n2375, n2376, n2377, n2378, n2379, n2380, n2381, n2382, n2383,
         n2384, n2385, n2386, n2387, n2388, n2389, n2390, n2391, n2392, n2393,
         n2394, n2395, n2396, n2397, n2398, n2399, n2400, n2401, n2402, n2403,
         n2404, n2405, n2406, n2407, n2408, n2409, n2410, n2411, n2412, n2413,
         n2414, n2415, n2416, n2417, n2418, n2419, n2420, n2421, n2422, n2423,
         n2424, n2425, n2426, n2427, n2428, n2429, n2430, n2431, n2432, n2433,
         n2434, n2435, n2436, n2437, n2438, n2439, n2440, n2441, n2442, n2443,
         n2444, n2445, n2446, n2447, n2448, n2449, n2450, n2451, n2452, n2453,
         n2454, n2455, n2456, n2457, n2458, n2459, n2460, n2461, n2462, n2463,
         n2464, n2465, n2466, n2467, n2468, n2469, n2470, n2471, n2472, n2473,
         n2474, n2475, n24761, n2477, n2478, n2479, n2480, n2481, n2482, n2483,
         n2484, n2485, n2486, n2487, n2488, n2489, n2490, n2491, n2492, n2493,
         n2494, n2495, n2496, n2497, n2498, n2499, n2500, n2501, n2502, n2503,
         n2504, n2505, n2506, n2507, n2508, n2509, n2510, n2511, n25121, n2513,
         n2514, n2515, n2516, n2517, n2518, n2519, n2520, n2521, n2522, n2523,
         n2524, n2525, n2526, n2527, n2528, n2529, n2530, n2531, n2532, n2533,
         n2534, n2535, n2536, n2537, n2538, n2539, n2540, n2541, n2542, n2543,
         n2544, n2545, n2546, n2547, n25481, n2549, n2550, n2551, n2552, n2553,
         n2554, n2555, n2556, n2557, n2558, n2559, n2560, n2561, n2562, n2563,
         n2564, n2565, n2566, n2567, n2568, n2569, n2570, n2571, n2572, n2573,
         n2574, n2575, n2576, n2577, n2578, n2579, n2580, n2581, n2582, n2583,
         n2584, n2585, n2586, n2587, n2588, n2589, n2590, n2591, n2592, n2593,
         n2594, n2595, n2596, n2597, n2598, n2599, n2600, n2601, n2602, n2603,
         n2604, n2605, n2606, n2607, n2608, n2609, n2610, n2611, n2612, n2613,
         n2614, n2615, n2616, n2617, n2618, n2619, n2620, n2621, n2622, n2623,
         n2624, n2625, n2626, n2627, n2628, n2629, n2630, n2631, n2632, n2633,
         n2634, n2635, n2636, n2637, n2638, n2639, n2640, n2641, n2642, n2643,
         n2644, n2645, n2646, n2647, n2648, n2649, n2650, n2651, n2652, n2653,
         n2654, n2655, n2656, n2657, n2658, n2659, n2660, n2661, n2662, n2663,
         n2664, n2665, n2666, r1189_n2, mult_890_n177, mult_890_n176,
         mult_890_n175, mult_890_n174, mult_890_n173, mult_890_n172,
         mult_890_n171, mult_890_n170, mult_890_n169, mult_890_n168,
         mult_890_n167, mult_890_n166, mult_890_n165, mult_890_n164,
         mult_890_n163, mult_890_n162, mult_890_n161, mult_890_n160,
         mult_890_n159, mult_890_n158, mult_890_n157, mult_890_n156,
         mult_890_n155, mult_890_n154, mult_890_n153, mult_890_n152,
         mult_890_n151, mult_890_n150, mult_890_n149, mult_890_n148,
         mult_890_n147, mult_890_n146, mult_890_n145, mult_890_n144,
         mult_890_n143, mult_890_n142, mult_890_n141, mult_890_n140,
         mult_890_n139, mult_890_n138, mult_890_n137, mult_890_n136,
         mult_890_n135, mult_890_n134, mult_890_n133, mult_890_n132,
         mult_890_n131, mult_890_n130, mult_890_n129, mult_890_n128,
         mult_890_n127, mult_890_n126, mult_890_n125, mult_890_n124,
         mult_890_n123, mult_890_n122, mult_890_n121, mult_890_n120,
         mult_890_n119, mult_890_n118, mult_890_n117, mult_890_n116,
         mult_890_n115, mult_890_n114, mult_890_n113, mult_890_n112,
         mult_890_n111, mult_890_n110, mult_890_n109, mult_890_n108,
         mult_890_n107, mult_890_n106, mult_890_n105, mult_890_n104,
         mult_890_n103, mult_890_n102, mult_890_n101, mult_890_n100,
         mult_890_n99, mult_890_n98, mult_890_n97, mult_890_n96, mult_890_n95,
         mult_890_n94, mult_890_n93, mult_890_n92, mult_890_n91, mult_890_n90,
         mult_890_n89, mult_890_n88, mult_890_n87, mult_890_n86, mult_890_n85,
         mult_890_n84, mult_890_n83, mult_890_n82, mult_890_n81, mult_890_n80,
         mult_890_n79, mult_890_n78, mult_890_n77, mult_890_n76, mult_890_n75,
         mult_890_n74, mult_890_n73, mult_890_n72, mult_890_n71, mult_890_n70,
         mult_890_n69, mult_890_n68, mult_890_n67, mult_890_n66, mult_890_n65,
         mult_890_n64, mult_890_n63, mult_890_n62, mult_890_n61, mult_890_n60,
         mult_890_n59, mult_890_n58, mult_890_n57, mult_890_n56, mult_890_n55,
         mult_890_n54, mult_890_n53, mult_890_n52, mult_890_n51, mult_890_n50,
         mult_890_n49, mult_890_n48, mult_890_n47, mult_890_n46, mult_890_n45,
         mult_890_n44, mult_890_n43, mult_890_n42, mult_890_n41, mult_890_n40,
         mult_890_n39, mult_890_n38, mult_890_n37, mult_890_n36, mult_890_n35,
         mult_890_n34, mult_890_n33, mult_890_n32, mult_890_n31, mult_890_n30,
         mult_890_n29, mult_890_n28, mult_890_n27, mult_890_n26, mult_890_n25,
         mult_890_n24, mult_890_n23, mult_890_n22, mult_890_n21, mult_890_n20,
         mult_890_n19, mult_890_n18, mult_890_n17, mult_890_n16, mult_890_n15,
         mult_890_n14, mult_890_n13, mult_890_n12, mult_890_n11, mult_890_n10,
         mult_890_n9, mult_890_n8, mult_890_n7, mult_890_n6, mult_890_n5,
         mult_890_n4, mult_890_n3, mult_890_n2;
  wire   [5:0] cpu_next_state;
  wire   [5:0] cpu_state;
  wire   [1:0] alu_ctrl;
  wire   [7:0] pc_ctrl;
  wire   [15:0] program_ctr;
  wire   [2:0] reg0;
  wire   [6:0] offset_sx;
  wire   [4:0] opcode;
  wire   [7:1] int_mask;
  wire   [15:0] regfile;
  wire   [2:0] flags;
  wire   [7:0] operand2;
  wire   [15:0] stack_ptr;
  wire   [15:0] isr_addr;
  wire   [15:0] mult;
  wire   [1:0] prefetch;
  wire   [6:0] pending;
  wire   [23:0] history;
  wire   [3:0] hst_ptr;
  wire   [15:2] r1194_carry;
  wire   [15:2] add_1_root_sub_995_carry;
  wire   [15:1] r1189_carry;
  wire   [15:2] add_1150_carry;
  wire   [7:1] r423_carry;

  ADDF_X1M_A12TR r1194_u1_1 ( .A(u4_u1_z_1), .B(offset_sx[1]), .CI(n1539), 
        .CO(r1194_carry[2]), .S(n1246) );
  ADDF_X1M_A12TR r1194_u1_2 ( .A(u4_u1_z_2), .B(offset_sx[2]), .CI(
        r1194_carry[2]), .CO(r1194_carry[3]), .S(n1247) );
  ADDF_X1M_A12TR r1194_u1_3 ( .A(u4_u1_z_3), .B(offset_sx[3]), .CI(
        r1194_carry[3]), .CO(r1194_carry[4]), .S(n1248) );
  ADDF_X1M_A12TR r1194_u1_4 ( .A(u4_u1_z_4), .B(offset_sx[4]), .CI(
        r1194_carry[4]), .CO(r1194_carry[5]), .S(n1249) );
  ADDF_X1M_A12TR r1194_u1_5 ( .A(u4_u1_z_5), .B(offset_sx[5]), .CI(
        r1194_carry[5]), .CO(r1194_carry[6]), .S(n1250) );
  ADDF_X1M_A12TR r1194_u1_6 ( .A(u4_u1_z_6), .B(offset_sx[6]), .CI(
        r1194_carry[6]), .CO(r1194_carry[7]), .S(n1251) );
  ADDF_X1M_A12TR r1194_u1_7 ( .A(u4_u1_z_7), .B(offset_sx_15), .CI(
        r1194_carry[7]), .CO(r1194_carry[8]), .S(n1252) );
  ADDF_X1M_A12TR r1194_u1_8 ( .A(u4_u1_z_8), .B(offset_sx_15), .CI(
        r1194_carry[8]), .CO(r1194_carry[9]), .S(n1253) );
  ADDF_X1M_A12TR r1194_u1_9 ( .A(u4_u1_z_9), .B(offset_sx_15), .CI(
        r1194_carry[9]), .CO(r1194_carry[10]), .S(n1254) );
  ADDF_X1M_A12TR r1194_u1_10 ( .A(u4_u1_z_10), .B(offset_sx_15), .CI(
        r1194_carry[10]), .CO(r1194_carry[11]), .S(n1255) );
  ADDF_X1M_A12TR r1194_u1_11 ( .A(u4_u1_z_11), .B(offset_sx_15), .CI(
        r1194_carry[11]), .CO(r1194_carry[12]), .S(n1256) );
  ADDF_X1M_A12TR r1194_u1_12 ( .A(u4_u1_z_12), .B(offset_sx_15), .CI(
        r1194_carry[12]), .CO(r1194_carry[13]), .S(n1257) );
  ADDF_X1M_A12TR r1194_u1_13 ( .A(u4_u1_z_13), .B(offset_sx_15), .CI(
        r1194_carry[13]), .CO(r1194_carry[14]), .S(n1258) );
  ADDF_X1M_A12TR r1194_u1_14 ( .A(u4_u1_z_14), .B(offset_sx_15), .CI(
        r1194_carry[14]), .CO(r1194_carry[15]), .S(n1259) );
  ADDF_X1M_A12TR r1194_u1_15 ( .A(u4_u1_z_15), .B(offset_sx_15), .CI(
        r1194_carry[15]), .CO(), .S(n1260) );
  ADDF_X1M_A12TR r1183_u2_1 ( .A(u4_u2_z_1), .B(n2656), .CI(n1538), .CO(
        r1183_carry_2_), .S(n15850) );
  ADDF_X1M_A12TR add_1_root_sub_995_u1_1 ( .A(program_ctr[1]), .B(pc_ctrl[1]), 
        .CI(n1540), .CO(add_1_root_sub_995_carry[2]), .S(n15690) );
  ADDF_X1M_A12TR add_1_root_sub_995_u1_2 ( .A(program_ctr[2]), .B(pc_ctrl[2]), 
        .CI(add_1_root_sub_995_carry[2]), .CO(add_1_root_sub_995_carry[3]), 
        .S(n15700) );
  ADDF_X1M_A12TR add_1_root_sub_995_u1_3 ( .A(program_ctr[3]), .B(pc_ctrl[3]), 
        .CI(add_1_root_sub_995_carry[3]), .CO(add_1_root_sub_995_carry[4]), 
        .S(n15710) );
  ADDF_X1M_A12TR add_1_root_sub_995_u1_4 ( .A(program_ctr[4]), .B(pc_ctrl[4]), 
        .CI(add_1_root_sub_995_carry[4]), .CO(add_1_root_sub_995_carry[5]), 
        .S(n15720) );
  ADDF_X1M_A12TR add_1_root_sub_995_u1_5 ( .A(program_ctr[5]), .B(pc_ctrl[5]), 
        .CI(add_1_root_sub_995_carry[5]), .CO(add_1_root_sub_995_carry[6]), 
        .S(n15730) );
  ADDF_X1M_A12TR add_1_root_sub_995_u1_6 ( .A(program_ctr[6]), .B(pc_ctrl[6]), 
        .CI(add_1_root_sub_995_carry[6]), .CO(add_1_root_sub_995_carry[7]), 
        .S(n15740) );
  ADDF_X1M_A12TR add_1_root_sub_995_u1_7 ( .A(program_ctr[7]), .B(pc_ctrl[7]), 
        .CI(add_1_root_sub_995_carry[7]), .CO(add_1_root_sub_995_carry[8]), 
        .S(n15750) );
  ADDF_X1M_A12TR add_1_root_sub_995_u1_8 ( .A(program_ctr[8]), .B(pc_ctrl[7]), 
        .CI(add_1_root_sub_995_carry[8]), .CO(add_1_root_sub_995_carry[9]), 
        .S(n15760) );
  ADDF_X1M_A12TR add_1_root_sub_995_u1_9 ( .A(program_ctr[9]), .B(pc_ctrl[7]), 
        .CI(add_1_root_sub_995_carry[9]), .CO(add_1_root_sub_995_carry[10]), 
        .S(n15770) );
  ADDF_X1M_A12TR add_1_root_sub_995_u1_10 ( .A(program_ctr[10]), .B(pc_ctrl[7]), .CI(add_1_root_sub_995_carry[10]), .CO(add_1_root_sub_995_carry[11]), .S(
        n15780) );
  ADDF_X1M_A12TR add_1_root_sub_995_u1_11 ( .A(program_ctr[11]), .B(pc_ctrl[7]), .CI(add_1_root_sub_995_carry[11]), .CO(add_1_root_sub_995_carry[12]), .S(
        n15790) );
  ADDF_X1M_A12TR add_1_root_sub_995_u1_12 ( .A(program_ctr[12]), .B(pc_ctrl[7]), .CI(add_1_root_sub_995_carry[12]), .CO(add_1_root_sub_995_carry[13]), .S(
        n15800) );
  ADDF_X1M_A12TR add_1_root_sub_995_u1_13 ( .A(program_ctr[13]), .B(pc_ctrl[7]), .CI(add_1_root_sub_995_carry[13]), .CO(add_1_root_sub_995_carry[14]), .S(
        n15810) );
  ADDF_X1M_A12TR add_1_root_sub_995_u1_14 ( .A(program_ctr[14]), .B(pc_ctrl[7]), .CI(add_1_root_sub_995_carry[14]), .CO(add_1_root_sub_995_carry[15]), .S(
        n15820) );
  ADDF_X1M_A12TR add_1_root_sub_995_u1_15 ( .A(program_ctr[15]), .B(pc_ctrl[7]), .CI(add_1_root_sub_995_carry[15]), .CO(), .S(n15830) );
  DFFQ_X1M_A12TR mult_reg_0_ ( .D(n14660), .CK(clock), .Q(mult[0]) );
  DFFQ_X1M_A12TR mult_reg_1_ ( .D(n14670), .CK(clock), .Q(mult[1]) );
  DFFQ_X1M_A12TR mult_reg_6_ ( .D(n14720), .CK(clock), .Q(mult[6]) );
  DFFQ_X1M_A12TR mult_reg_8_ ( .D(n14740), .CK(clock), .Q(mult[8]) );
  DFFQ_X1M_A12TR mult_reg_9_ ( .D(n14750), .CK(clock), .Q(mult[9]) );
  DFFQ_X1M_A12TR mult_reg_10_ ( .D(n14760), .CK(clock), .Q(mult[10]) );
  DFFQ_X1M_A12TR mult_reg_13_ ( .D(n14790), .CK(clock), .Q(mult[13]) );
  DFFQ_X1M_A12TR mult_reg_14_ ( .D(n14800), .CK(clock), .Q(mult[14]) );
  DFFQ_X1M_A12TR mult_reg_15_ ( .D(n14810), .CK(clock), .Q(mult[15]) );
  DFFQ_X1M_A12TR mult_reg_11_ ( .D(n14770), .CK(clock), .Q(mult[11]) );
  DFFQ_X1M_A12TR mult_reg_12_ ( .D(n14780), .CK(clock), .Q(mult[12]) );
  DFFQ_X1M_A12TR mult_reg_7_ ( .D(n14730), .CK(clock), .Q(mult[7]) );
  DFFSQ_X1M_A12TR int_mask_reg_6_ ( .D(n1446), .CK(clock), .SN(reset), .Q(
        int_mask[6]) );
  DFFSQ_X1M_A12TR int_mask_reg_7_ ( .D(n1431), .CK(clock), .SN(reset), .Q(
        int_mask[7]) );
  DFFSQ_X1M_A12TR int_mask_reg_2_ ( .D(n1435), .CK(clock), .SN(reset), .Q(
        int_mask[2]) );
  DFFSQ_X1M_A12TR int_mask_reg_4_ ( .D(n1433), .CK(clock), .SN(reset), .Q(
        int_mask[4]) );
  DFFSQ_X1M_A12TR int_mask_reg_5_ ( .D(n1432), .CK(clock), .SN(reset), .Q(
        int_mask[5]) );
  DFFSQ_X1M_A12TR int_mask_reg_1_ ( .D(n1436), .CK(clock), .SN(reset), .Q(
        int_mask[1]) );
  DFFSQ_X1M_A12TR int_mask_reg_3_ ( .D(n1434), .CK(clock), .SN(reset), .Q(
        int_mask[3]) );
  DFFRPQ_X1M_A12TR int_req_reg ( .D(n21420), .CK(clock), .R(n1542), .Q(int_req) );
  DFFRPQ_X1M_A12TR wait_for_fsm_reg ( .D(n1437), .CK(clock), .R(n1542), .Q(
        wait_for_fsm) );
  DFFRPQ_X1M_A12TR history_reg_1__2_ ( .D(n1371), .CK(clock), .R(n1550), .Q(
        history[23]) );
  DFFRPQ_X1M_A12TR history_reg_2__2_ ( .D(n1374), .CK(clock), .R(n1551), .Q(
        history[20]) );
  DFFRPQ_X1M_A12TR history_reg_3__2_ ( .D(n1377), .CK(clock), .R(n1550), .Q(
        history[17]) );
  DFFRPQ_X1M_A12TR history_reg_6__2_ ( .D(n1386), .CK(clock), .R(n1550), .Q(
        history[8]) );
  DFFRPQ_X1M_A12TR history_reg_7__2_ ( .D(n1389), .CK(clock), .R(n1551), .Q(
        history[5]) );
  DFFRPQ_X1M_A12TR history_reg_8__2_ ( .D(n1392), .CK(clock), .R(n1550), .Q(
        history[2]) );
  DFFRPQ_X1M_A12TR history_reg_1__1_ ( .D(n1372), .CK(clock), .R(n1550), .Q(
        history[22]) );
  DFFRPQ_X1M_A12TR history_reg_2__1_ ( .D(n1375), .CK(clock), .R(n1551), .Q(
        history[19]) );
  DFFRPQ_X1M_A12TR history_reg_3__1_ ( .D(n1378), .CK(clock), .R(n1551), .Q(
        history[16]) );
  DFFRPQ_X1M_A12TR history_reg_6__1_ ( .D(n1387), .CK(clock), .R(n1551), .Q(
        history[7]) );
  DFFRPQ_X1M_A12TR history_reg_7__1_ ( .D(n1390), .CK(clock), .R(n1550), .Q(
        history[4]) );
  DFFRPQ_X1M_A12TR history_reg_8__1_ ( .D(n1393), .CK(clock), .R(n1550), .Q(
        history[1]) );
  DFFRPQ_X1M_A12TR history_reg_1__0_ ( .D(n1373), .CK(clock), .R(n1544), .Q(
        history[21]) );
  DFFRPQ_X1M_A12TR history_reg_2__0_ ( .D(n1376), .CK(clock), .R(n1544), .Q(
        history[18]) );
  DFFRPQ_X1M_A12TR history_reg_3__0_ ( .D(n1379), .CK(clock), .R(n1544), .Q(
        history[15]) );
  DFFRPQ_X1M_A12TR history_reg_6__0_ ( .D(n1388), .CK(clock), .R(n1544), .Q(
        history[6]) );
  DFFRPQ_X1M_A12TR history_reg_7__0_ ( .D(n1391), .CK(clock), .R(n1544), .Q(
        history[3]) );
  DFFRPQ_X1M_A12TR history_reg_8__0_ ( .D(n1394), .CK(clock), .R(n1542), .Q(
        history[0]) );
  DFFRPQ_X1M_A12TR int_rti_reg ( .D(n2666), .CK(clock), .R(n1544), .Q(int_rti)
         );
  DFFRPQ_X1M_A12TR history_reg_5__2_ ( .D(n1383), .CK(clock), .R(n1551), .Q(
        history[11]) );
  DFFRPQ_X1M_A12TR history_reg_4__2_ ( .D(n1380), .CK(clock), .R(n1551), .Q(
        history[14]) );
  DFFRPQ_X1M_A12TR operand2_reg_7_ ( .D(n14751), .CK(clock), .R(n1547), .Q(
        operand2[7]) );
  DFFRPQ_X1M_A12TR operand2_reg_6_ ( .D(n14761), .CK(clock), .R(n1547), .Q(
        operand2[6]) );
  DFFRPQ_X1M_A12TR operand2_reg_5_ ( .D(n14771), .CK(clock), .R(n1547), .Q(
        operand2[5]) );
  DFFRPQ_X1M_A12TR operand2_reg_4_ ( .D(n14781), .CK(clock), .R(n1548), .Q(
        operand2[4]) );
  DFFRPQ_X1M_A12TR operand2_reg_3_ ( .D(n14791), .CK(clock), .R(n1548), .Q(
        operand2[3]) );
  DFFRPQ_X1M_A12TR operand2_reg_2_ ( .D(n14801), .CK(clock), .R(n1549), .Q(
        operand2[2]) );
  DFFRPQ_X1M_A12TR operand2_reg_1_ ( .D(n14811), .CK(clock), .R(n1549), .Q(
        operand2[1]) );
  DFFRPQ_X1M_A12TR operand2_reg_0_ ( .D(n1482), .CK(clock), .R(n1550), .Q(
        operand2[0]) );
  DFFRPQ_X1M_A12TR int_ack_reg ( .D(ack_q1), .CK(clock), .R(n1543), .Q(int_ack) );
  DFFRPQ_X1M_A12TR history_reg_5__1_ ( .D(n1384), .CK(clock), .R(n1551), .Q(
        history[10]) );
  DFFRPQ_X1M_A12TR history_reg_5__0_ ( .D(n1385), .CK(clock), .R(n1544), .Q(
        history[9]) );
  DFFRPQ_X1M_A12TR history_reg_4__1_ ( .D(n1381), .CK(clock), .R(n1550), .Q(
        history[13]) );
  DFFRPQ_X1M_A12TR history_reg_4__0_ ( .D(n1382), .CK(clock), .R(n1544), .Q(
        history[12]) );
  DFFRPQ_X1M_A12TR opcode_reg_3_ ( .D(n1462), .CK(clock), .R(n1543), .Q(
        opcode[3]) );
  DFFRPQ_X1M_A12TR opcode_reg_2_ ( .D(n1463), .CK(clock), .R(n1543), .Q(
        opcode[2]) );
  DFFRPQ_X1M_A12TR instr_prefetch_reg ( .D(n14661), .CK(clock), .R(n1542), .Q(
        instr_prefetch) );
  DFFRPQ_X1M_A12TR regfile_reg_1__7_ ( .D(n1308), .CK(clock), .R(n1545), .Q(
        regfile[7]) );
  DFFRPQ_X1M_A12TR pending_reg_0_ ( .D(n1444), .CK(clock), .R(n1550), .Q(
        pending[0]) );
  DFFRPQ_X1M_A12TR opcode_reg_4_ ( .D(n1461), .CK(clock), .R(n1543), .Q(
        opcode[4]) );
  DFFRPQ_X1M_A12TR isr_addr_reg_15_ ( .D(n1410), .CK(clock), .R(n1550), .Q(
        isr_addr[15]) );
  DFFRPQ_X1M_A12TR pending_reg_4_ ( .D(n1440), .CK(clock), .R(n1542), .Q(
        pending[4]) );
  DFFRPQ_X1M_A12TR pending_reg_1_ ( .D(n1443), .CK(clock), .R(n1542), .Q(
        pending[1]) );
  DFFSQ_X1M_A12TR isr_addr_reg_7_ ( .D(n1402), .CK(clock), .SN(reset), .Q(
        isr_addr[7]) );
  DFFRPQ_X1M_A12TR flags_reg_2_ ( .D(n1370), .CK(clock), .R(n1548), .Q(
        flags[2]) );
  DFFRPQ_X1M_A12TR pending_reg_6_ ( .D(n1438), .CK(clock), .R(n1542), .Q(
        pending[6]) );
  DFFRPQ_X1M_A12TR pending_reg_5_ ( .D(n1439), .CK(clock), .R(n1542), .Q(
        pending[5]) );
  DFFRPQ_X1M_A12TR regfile_reg_1__2_ ( .D(n1313), .CK(clock), .R(n1545), .Q(
        regfile[2]) );
  DFFRPQ_X1M_A12TR regfile_reg_1__3_ ( .D(n1312), .CK(clock), .R(n1545), .Q(
        regfile[3]) );
  DFFRPQ_X1M_A12TR regfile_reg_1__4_ ( .D(n1311), .CK(clock), .R(n1545), .Q(
        regfile[4]) );
  DFFRPQ_X1M_A12TR regfile_reg_1__5_ ( .D(n1310), .CK(clock), .R(n1545), .Q(
        regfile[5]) );
  DFFRPQ_X1M_A12TR regfile_reg_1__6_ ( .D(n1309), .CK(clock), .R(n1545), .Q(
        regfile[6]) );
  DFFRPQ_X1M_A12TR regfile_reg_1__1_ ( .D(n1314), .CK(clock), .R(n1545), .Q(
        regfile[1]) );
  DFFRPQ_X1M_A12TR regfile_reg_1__0_ ( .D(n1315), .CK(clock), .R(n1546), .Q(
        regfile[0]) );
  DFFRPQ_X1M_A12TR prefetch_reg_1_ ( .D(n14731), .CK(clock), .R(n1543), .Q(
        prefetch[1]) );
  DFFSQ_X1M_A12TR stack_ptr_reg_1_ ( .D(n1428), .CK(clock), .SN(reset), .Q(
        stack_ptr[1]) );
  DFFSQ_X1M_A12TR stack_ptr_reg_2_ ( .D(n1427), .CK(clock), .SN(reset), .Q(
        stack_ptr[2]) );
  DFFSQ_X1M_A12TR stack_ptr_reg_3_ ( .D(n1426), .CK(clock), .SN(reset), .Q(
        stack_ptr[3]) );
  DFFSQ_X1M_A12TR stack_ptr_reg_4_ ( .D(n1425), .CK(clock), .SN(reset), .Q(
        stack_ptr[4]) );
  DFFSQ_X1M_A12TR stack_ptr_reg_5_ ( .D(n1424), .CK(clock), .SN(reset), .Q(
        stack_ptr[5]) );
  DFFSQ_X1M_A12TR stack_ptr_reg_6_ ( .D(n1423), .CK(clock), .SN(reset), .Q(
        stack_ptr[6]) );
  DFFSQ_X1M_A12TR program_ctr_reg_4_ ( .D(n1302), .CK(clock), .SN(reset), .Q(
        program_ctr[4]) );
  DFFSQ_X1M_A12TR program_ctr_reg_7_ ( .D(n1299), .CK(clock), .SN(reset), .Q(
        program_ctr[7]) );
  DFFRPQ_X1M_A12TR prefetch_reg_0_ ( .D(n14741), .CK(clock), .R(n1543), .Q(
        prefetch[0]) );
  DFFRPQ_X1M_A12TR flags_reg_0_ ( .D(n1491), .CK(clock), .R(n1542), .Q(
        flags[0]) );
  DFFRPQ_X1M_A12TR cpu_state_reg_1_ ( .D(cpu_next_state[1]), .CK(clock), .R(
        n1543), .Q(cpu_state[1]) );
  DFFRPQ_X1M_A12TR cpu_state_reg_3_ ( .D(cpu_next_state[3]), .CK(clock), .R(
        n1542), .Q(cpu_state[3]) );
  DFFRPQ_X1M_A12TR cpu_state_reg_0_ ( .D(cpu_next_state[0]), .CK(clock), .R(
        n1543), .Q(cpu_state[0]) );
  DFFRPQ_X1M_A12TR subop_reg_2_ ( .D(n1458), .CK(clock), .R(n1543), .Q(reg0[2]) );
  DFFRPQ_X1M_A12TR subop_reg_1_ ( .D(n1459), .CK(clock), .R(n1543), .Q(reg0[1]) );
  DFFRPQ_X1M_A12TR isr_addr_reg_4_ ( .D(n1405), .CK(clock), .R(n1549), .Q(
        isr_addr[4]) );
  DFFRPQ_X1M_A12TR isr_addr_reg_5_ ( .D(n1404), .CK(clock), .R(n1549), .Q(
        isr_addr[5]) );
  DFFRPQ_X1M_A12TR isr_addr_reg_6_ ( .D(n1403), .CK(clock), .R(n1548), .Q(
        isr_addr[6]) );
  DFFRPQ_X1M_A12TR isr_addr_reg_8_ ( .D(n1401), .CK(clock), .R(n1549), .Q(
        isr_addr[8]) );
  DFFRPQ_X1M_A12TR isr_addr_reg_9_ ( .D(n1400), .CK(clock), .R(n1549), .Q(
        isr_addr[9]) );
  DFFRPQ_X1M_A12TR isr_addr_reg_10_ ( .D(n1399), .CK(clock), .R(n1549), .Q(
        isr_addr[10]) );
  DFFRPQ_X1M_A12TR isr_addr_reg_11_ ( .D(n1398), .CK(clock), .R(n1549), .Q(
        isr_addr[11]) );
  DFFRPQ_X1M_A12TR isr_addr_reg_12_ ( .D(n1397), .CK(clock), .R(n1549), .Q(
        isr_addr[12]) );
  DFFRPQ_X1M_A12TR isr_addr_reg_13_ ( .D(n1396), .CK(clock), .R(n1549), .Q(
        isr_addr[13]) );
  DFFRPQ_X1M_A12TR isr_addr_reg_14_ ( .D(n1395), .CK(clock), .R(n1550), .Q(
        isr_addr[14]) );
  DFFRPQ_X1M_A12TR isr_addr_reg_2_ ( .D(n1407), .CK(clock), .R(n1549), .Q(
        isr_addr[2]) );
  DFFRPQ_X1M_A12TR isr_addr_reg_1_ ( .D(n1408), .CK(clock), .R(n1550), .Q(
        isr_addr[1]) );
  DFFRPQ_X1M_A12TR isr_addr_reg_3_ ( .D(n1406), .CK(clock), .R(n1549), .Q(
        isr_addr[3]) );
  DFFRPQ_X1M_A12TR cpu_state_reg_2_ ( .D(cpu_next_state[2]), .CK(clock), .R(
        n1542), .Q(cpu_state[2]) );
  DFFRPQ_X1M_A12TR stack_ptr_reg_7_ ( .D(n1422), .CK(clock), .R(n1546), .Q(
        stack_ptr[7]) );
  DFFRPQ_X1M_A12TR stack_ptr_reg_8_ ( .D(n1421), .CK(clock), .R(n1547), .Q(
        stack_ptr[8]) );
  DFFRPQ_X1M_A12TR stack_ptr_reg_9_ ( .D(n1420), .CK(clock), .R(n1547), .Q(
        stack_ptr[9]) );
  DFFRPQ_X1M_A12TR stack_ptr_reg_10_ ( .D(n1419), .CK(clock), .R(n1547), .Q(
        stack_ptr[10]) );
  DFFRPQ_X1M_A12TR stack_ptr_reg_11_ ( .D(n1418), .CK(clock), .R(n1546), .Q(
        stack_ptr[11]) );
  DFFRPQ_X1M_A12TR stack_ptr_reg_12_ ( .D(n1417), .CK(clock), .R(n1546), .Q(
        stack_ptr[12]) );
  DFFRPQ_X1M_A12TR stack_ptr_reg_13_ ( .D(n1416), .CK(clock), .R(n1546), .Q(
        stack_ptr[13]) );
  DFFRPQ_X1M_A12TR stack_ptr_reg_14_ ( .D(n1415), .CK(clock), .R(n1546), .Q(
        stack_ptr[14]) );
  DFFRPQ_X1M_A12TR stack_ptr_reg_15_ ( .D(n1430), .CK(clock), .R(n1546), .Q(
        stack_ptr[15]) );
  DFFRPQ_X1M_A12TR hst_ptr_reg_2_ ( .D(n1411), .CK(clock), .R(n1544), .Q(
        hst_ptr[2]) );
  DFFRPQ_X1M_A12TR operand1_reg_6_ ( .D(n1484), .CK(clock), .R(n1541), .Q(
        offset_sx[6]) );
  DFFRPQ_X1M_A12TR operand1_reg_5_ ( .D(n1485), .CK(clock), .R(n1541), .Q(
        offset_sx[5]) );
  DFFRPQ_X1M_A12TR operand1_reg_4_ ( .D(n1486), .CK(clock), .R(n1541), .Q(
        offset_sx[4]) );
  DFFRPQ_X1M_A12TR operand1_reg_3_ ( .D(n1487), .CK(clock), .R(n1541), .Q(
        offset_sx[3]) );
  DFFRPQ_X1M_A12TR operand1_reg_2_ ( .D(n1488), .CK(clock), .R(n1541), .Q(
        offset_sx[2]) );
  DFFRPQ_X1M_A12TR operand1_reg_1_ ( .D(n1489), .CK(clock), .R(n1541), .Q(
        offset_sx[1]) );
  DFFRPQ_X1M_A12TR subop_reg_0_ ( .D(n1460), .CK(clock), .R(n1544), .Q(reg0[0]) );
  DFFRPQ_X1M_A12TR cpu_state_reg_4_ ( .D(cpu_next_state[4]), .CK(clock), .R(
        n1542), .Q(cpu_state[4]) );
  DFFRPQ_X1M_A12TR opcode_reg_0_ ( .D(n1465), .CK(clock), .R(n1546), .Q(
        opcode[0]) );
  DFFSQ_X1M_A12TR stack_ptr_reg_0_ ( .D(n1429), .CK(clock), .SN(reset), .Q(
        stack_ptr[0]) );
  DFFRPQ_X1M_A12TR opcode_reg_1_ ( .D(n1464), .CK(clock), .R(n1543), .Q(
        opcode[1]) );
  DFFRPQ_X1M_A12TR program_ctr_reg_1_ ( .D(n1305), .CK(clock), .R(n1546), .Q(
        program_ctr[1]) );
  DFFRPQ_X1M_A12TR program_ctr_reg_2_ ( .D(n1304), .CK(clock), .R(n1549), .Q(
        program_ctr[2]) );
  DFFRPQ_X1M_A12TR program_ctr_reg_3_ ( .D(n1303), .CK(clock), .R(n1548), .Q(
        program_ctr[3]) );
  DFFRPQ_X1M_A12TR program_ctr_reg_5_ ( .D(n1301), .CK(clock), .R(n1548), .Q(
        program_ctr[5]) );
  DFFRPQ_X1M_A12TR program_ctr_reg_6_ ( .D(n1300), .CK(clock), .R(n1548), .Q(
        program_ctr[6]) );
  DFFRPQ_X1M_A12TR hst_ptr_reg_3_ ( .D(n1414), .CK(clock), .R(n1545), .Q(
        hst_ptr[3]) );
  DFFRPQ_X1M_A12TR hst_ptr_reg_0_ ( .D(n1413), .CK(clock), .R(n1544), .Q(
        hst_ptr[0]) );
  DFFRPQ_X1M_A12TR cpu_state_reg_5_ ( .D(cpu_next_state[5]), .CK(clock), .R(
        n1542), .Q(cpu_state[5]) );
  DFFRPQ_X1M_A12TR program_ctr_reg_8_ ( .D(n1298), .CK(clock), .R(n1548), .Q(
        program_ctr[8]) );
  DFFRPQ_X1M_A12TR program_ctr_reg_9_ ( .D(n1297), .CK(clock), .R(n1548), .Q(
        program_ctr[9]) );
  DFFRPQ_X1M_A12TR program_ctr_reg_10_ ( .D(n1296), .CK(clock), .R(n1548), .Q(
        program_ctr[10]) );
  DFFRPQ_X1M_A12TR program_ctr_reg_11_ ( .D(n1295), .CK(clock), .R(n1547), .Q(
        program_ctr[11]) );
  DFFRPQ_X1M_A12TR program_ctr_reg_12_ ( .D(n1294), .CK(clock), .R(n1547), .Q(
        program_ctr[12]) );
  DFFRPQ_X1M_A12TR program_ctr_reg_13_ ( .D(n1293), .CK(clock), .R(n1547), .Q(
        program_ctr[13]) );
  DFFRPQ_X1M_A12TR program_ctr_reg_14_ ( .D(n1292), .CK(clock), .R(n1546), .Q(
        program_ctr[14]) );
  DFFRPQ_X1M_A12TR program_ctr_reg_15_ ( .D(n1307), .CK(clock), .R(n1548), .Q(
        program_ctr[15]) );
  DFFRPQ_X1M_A12TR hst_ptr_reg_1_ ( .D(n1412), .CK(clock), .R(n1544), .Q(
        hst_ptr[1]) );
  DFFRPQ_X1M_A12TR operand1_reg_0_ ( .D(n1490), .CK(clock), .R(n1541), .Q(
        offset_sx[0]) );
  DFFRPQ_X1M_A12TR isr_addr_reg_0_ ( .D(n1409), .CK(clock), .R(n1544), .Q(
        isr_addr[0]) );
  DFFRPQ_X1M_A12TR program_ctr_reg_0_ ( .D(n1306), .CK(clock), .R(n1546), .Q(
        program_ctr[0]) );
  DFFQN_X1M_A12TR mult_reg_4_ ( .D(n14700), .CK(clock), .QN(n327) );
  DFFQN_X1M_A12TR mult_reg_3_ ( .D(n14690), .CK(clock), .QN(n328) );
  DFFQN_X1M_A12TR mult_reg_5_ ( .D(n14710), .CK(clock), .QN(n326) );
  DFFQN_X1M_A12TR mult_reg_2_ ( .D(n14680), .CK(clock), .QN(n329) );
  DFFRPQ_X1M_A12TR flags_reg_1_ ( .D(n1364), .CK(clock), .R(n1541), .Q(
        flags[1]) );
  DFFRPQN_X1M_A12TR pending_reg_7_ ( .D(n1445), .CK(clock), .R(n1552), .QN(
        n324) );
  DFFRPQ_X1M_A12TR regfile_reg_0__2_ ( .D(n1451), .CK(clock), .R(n1545), .Q(
        regfile[10]) );
  DFFRPQ_X1M_A12TR regfile_reg_0__3_ ( .D(n1450), .CK(clock), .R(n1545), .Q(
        regfile[11]) );
  DFFRPQN_X1M_A12TR prefetch_reg_7_ ( .D(n14671), .CK(clock), .R(n1551), .QN(
        n487) );
  DFFRPQN_X1M_A12TR prefetch_reg_6_ ( .D(n14681), .CK(clock), .R(n1552), .QN(
        n488) );
  DFFRPQN_X1M_A12TR prefetch_reg_5_ ( .D(n14691), .CK(clock), .R(n1552), .QN(
        n489) );
  DFFRPQN_X1M_A12TR prefetch_reg_4_ ( .D(n14701), .CK(clock), .R(n1552), .QN(
        n490) );
  DFFRPQN_X1M_A12TR prefetch_reg_3_ ( .D(n14711), .CK(clock), .R(n1552), .QN(
        n491) );
  DFFRPQN_X1M_A12TR flags_reg_5_ ( .D(n1367), .CK(clock), .R(n1556), .QN(n282)
         );
  DFFRPQN_X1M_A12TR flags_reg_3_ ( .D(n1369), .CK(clock), .R(n1552), .QN(n281)
         );
  DFFRPQN_X1M_A12TR flags_reg_6_ ( .D(n1366), .CK(clock), .R(n1556), .QN(n280)
         );
  DFFRPQN_X1M_A12TR flags_reg_4_ ( .D(n1368), .CK(clock), .R(n1556), .QN(n279)
         );
  DFFRPQN_X1M_A12TR flags_reg_7_ ( .D(n1365), .CK(clock), .R(n1551), .QN(n278)
         );
  DFFRPQ_X1M_A12TR regfile_reg_0__6_ ( .D(n1447), .CK(clock), .R(n1545), .Q(
        regfile[14]) );
  DFFRPQ_X1M_A12TR regfile_reg_0__0_ ( .D(n1453), .CK(clock), .R(n1541), .Q(
        regfile[8]) );
  DFFRPQ_X1M_A12TR regfile_reg_0__1_ ( .D(n1452), .CK(clock), .R(n1541), .Q(
        regfile[9]) );
  DFFRPQ_X1M_A12TR regfile_reg_0__4_ ( .D(n1449), .CK(clock), .R(n1545), .Q(
        regfile[12]) );
  DFFRPQ_X1M_A12TR regfile_reg_0__5_ ( .D(n1448), .CK(clock), .R(n1545), .Q(
        regfile[13]) );
  DFFRPQN_X1M_A12TR pending_reg_2_ ( .D(n1442), .CK(clock), .R(n1551), .QN(
        n284) );
  DFFRPQN_X1M_A12TR pending_reg_3_ ( .D(n1441), .CK(clock), .R(n1552), .QN(
        n283) );
  DFFRPQN_X1M_A12TR regfile_reg_5__2_ ( .D(n1345), .CK(clock), .R(n1553), .QN(
        n317) );
  DFFRPQN_X1M_A12TR regfile_reg_5__3_ ( .D(n1344), .CK(clock), .R(n1554), .QN(
        n311) );
  DFFRPQN_X1M_A12TR regfile_reg_5__4_ ( .D(n1343), .CK(clock), .R(n1554), .QN(
        n305) );
  DFFRPQN_X1M_A12TR regfile_reg_5__5_ ( .D(n1342), .CK(clock), .R(n1552), .QN(
        n299) );
  DFFRPQN_X1M_A12TR regfile_reg_5__6_ ( .D(n1341), .CK(clock), .R(n1555), .QN(
        n293) );
  DFFRPQN_X1M_A12TR regfile_reg_5__1_ ( .D(n1346), .CK(clock), .R(n1553), .QN(
        n287) );
  DFFRPQN_X1M_A12TR regfile_reg_5__7_ ( .D(n1340), .CK(clock), .R(n1556), .QN(
        n274) );
  DFFRPQN_X1M_A12TR regfile_reg_5__0_ ( .D(n1347), .CK(clock), .R(n1553), .QN(
        n268) );
  DFFRPQN_X1M_A12TR regfile_reg_4__2_ ( .D(n1337), .CK(clock), .R(n1553), .QN(
        n318) );
  DFFRPQN_X1M_A12TR regfile_reg_4__3_ ( .D(n1336), .CK(clock), .R(n1554), .QN(
        n312) );
  DFFRPQN_X1M_A12TR regfile_reg_4__4_ ( .D(n1335), .CK(clock), .R(n1554), .QN(
        n306) );
  DFFRPQN_X1M_A12TR regfile_reg_4__5_ ( .D(n1334), .CK(clock), .R(n1555), .QN(
        n300) );
  DFFRPQN_X1M_A12TR regfile_reg_4__6_ ( .D(n1333), .CK(clock), .R(n1555), .QN(
        n294) );
  DFFRPQN_X1M_A12TR regfile_reg_4__1_ ( .D(n1338), .CK(clock), .R(n1553), .QN(
        n288) );
  DFFRPQN_X1M_A12TR regfile_reg_4__7_ ( .D(n1332), .CK(clock), .R(n1556), .QN(
        n275) );
  DFFRPQN_X1M_A12TR regfile_reg_4__0_ ( .D(n1339), .CK(clock), .R(n1553), .QN(
        n269) );
  DFFRPQN_X1M_A12TR regfile_reg_2__2_ ( .D(n1321), .CK(clock), .R(n1553), .QN(
        n320) );
  DFFRPQN_X1M_A12TR regfile_reg_3__2_ ( .D(n1329), .CK(clock), .R(n1553), .QN(
        n319) );
  DFFRPQN_X1M_A12TR regfile_reg_2__3_ ( .D(n1320), .CK(clock), .R(n1554), .QN(
        n314) );
  DFFRPQN_X1M_A12TR regfile_reg_3__3_ ( .D(n1328), .CK(clock), .R(n1554), .QN(
        n313) );
  DFFRPQN_X1M_A12TR regfile_reg_2__4_ ( .D(n1319), .CK(clock), .R(n1554), .QN(
        n308) );
  DFFRPQN_X1M_A12TR regfile_reg_3__4_ ( .D(n1327), .CK(clock), .R(n1554), .QN(
        n307) );
  DFFRPQN_X1M_A12TR regfile_reg_2__5_ ( .D(n1318), .CK(clock), .R(n1555), .QN(
        n302) );
  DFFRPQN_X1M_A12TR regfile_reg_3__5_ ( .D(n1326), .CK(clock), .R(n1555), .QN(
        n301) );
  DFFRPQN_X1M_A12TR regfile_reg_2__6_ ( .D(n1317), .CK(clock), .R(n1555), .QN(
        n296) );
  DFFRPQN_X1M_A12TR regfile_reg_3__6_ ( .D(n1325), .CK(clock), .R(n1555), .QN(
        n295) );
  DFFRPQN_X1M_A12TR regfile_reg_2__1_ ( .D(n1322), .CK(clock), .R(n1553), .QN(
        n290) );
  DFFRPQN_X1M_A12TR regfile_reg_3__1_ ( .D(n1330), .CK(clock), .R(n1552), .QN(
        n289) );
  DFFRPQN_X1M_A12TR regfile_reg_2__7_ ( .D(n1316), .CK(clock), .R(n1555), .QN(
        n277) );
  DFFRPQN_X1M_A12TR regfile_reg_3__7_ ( .D(n1324), .CK(clock), .R(n1556), .QN(
        n276) );
  DFFRPQN_X1M_A12TR regfile_reg_2__0_ ( .D(n1323), .CK(clock), .R(n1553), .QN(
        n271) );
  DFFRPQN_X1M_A12TR regfile_reg_3__0_ ( .D(n1331), .CK(clock), .R(n1553), .QN(
        n270) );
  DFFRPQN_X1M_A12TR prefetch_reg_2_ ( .D(n14721), .CK(clock), .R(n1552), .QN(
        n492) );
  DFFRPQ_X1M_A12TR regfile_reg_0__7_ ( .D(n1454), .CK(clock), .R(n1541), .Q(
        regfile[15]) );
  DFFRPQN_X1M_A12TR regfile_reg_7__2_ ( .D(n1361), .CK(clock), .R(n1554), .QN(
        n315) );
  DFFRPQN_X1M_A12TR regfile_reg_7__3_ ( .D(n1360), .CK(clock), .R(n1554), .QN(
        n309) );
  DFFRPQN_X1M_A12TR regfile_reg_7__4_ ( .D(n1359), .CK(clock), .R(n1555), .QN(
        n303) );
  DFFRPQN_X1M_A12TR regfile_reg_7__5_ ( .D(n1358), .CK(clock), .R(n1555), .QN(
        n297) );
  DFFRPQN_X1M_A12TR regfile_reg_7__6_ ( .D(n1357), .CK(clock), .R(n1555), .QN(
        n291) );
  DFFRPQN_X1M_A12TR regfile_reg_7__1_ ( .D(n1362), .CK(clock), .R(n1551), .QN(
        n285) );
  DFFRPQN_X1M_A12TR regfile_reg_7__7_ ( .D(n1356), .CK(clock), .R(n1556), .QN(
        n272) );
  DFFRPQN_X1M_A12TR regfile_reg_7__0_ ( .D(n1363), .CK(clock), .R(n1551), .QN(
        n266) );
  DFFRPQN_X1M_A12TR regfile_reg_6__2_ ( .D(n1353), .CK(clock), .R(n1554), .QN(
        n316) );
  DFFRPQN_X1M_A12TR regfile_reg_6__3_ ( .D(n1352), .CK(clock), .R(n1554), .QN(
        n310) );
  DFFRPQN_X1M_A12TR regfile_reg_6__4_ ( .D(n1351), .CK(clock), .R(n1554), .QN(
        n304) );
  DFFRPQN_X1M_A12TR regfile_reg_6__5_ ( .D(n1350), .CK(clock), .R(n1555), .QN(
        n298) );
  DFFRPQN_X1M_A12TR regfile_reg_6__6_ ( .D(n1349), .CK(clock), .R(n1555), .QN(
        n292) );
  DFFRPQN_X1M_A12TR regfile_reg_6__1_ ( .D(n1354), .CK(clock), .R(n1553), .QN(
        n286) );
  DFFRPQN_X1M_A12TR regfile_reg_6__7_ ( .D(n1348), .CK(clock), .R(n1556), .QN(
        n273) );
  DFFRPQN_X1M_A12TR regfile_reg_6__0_ ( .D(n1355), .CK(clock), .R(n1553), .QN(
        n267) );
  DFFRPQN_X1M_A12TR subop_p1_reg_1_ ( .D(n1456), .CK(clock), .R(n1552), .QN(
        n322) );
  DFFRPQN_X1M_A12TR subop_p1_reg_2_ ( .D(n1455), .CK(clock), .R(n1552), .QN(
        n323) );
  DFFRPQN_X1M_A12TR subop_p1_reg_0_ ( .D(n1457), .CK(clock), .R(n1552), .QN(
        n321) );
  DFFRPQ_X1M_A12TR operand1_reg_7_ ( .D(n1483), .CK(clock), .R(n1541), .Q(
        offset_sx_15) );
  DFFRPQ_X1M_A12TR wr_data_reg_0_ ( .D(n2970), .CK(clock), .R(n1548), .Q(
        wr_data[0]) );
  DFFRPQ_X1M_A12TR wr_data_reg_1_ ( .D(n2971), .CK(clock), .R(n1547), .Q(
        wr_data[1]) );
  DFFRPQ_X1M_A12TR wr_data_reg_2_ ( .D(n2972), .CK(clock), .R(n1548), .Q(
        wr_data[2]) );
  DFFRPQ_X1M_A12TR wr_data_reg_3_ ( .D(n2973), .CK(clock), .R(n1547), .Q(
        wr_data[3]) );
  DFFRPQ_X1M_A12TR wr_data_reg_4_ ( .D(n2974), .CK(clock), .R(n1547), .Q(
        wr_data[4]) );
  DFFRPQ_X1M_A12TR wr_data_reg_5_ ( .D(n2975), .CK(clock), .R(n1546), .Q(
        wr_data[5]) );
  DFFRPQ_X1M_A12TR wr_data_reg_6_ ( .D(n2976), .CK(clock), .R(n1546), .Q(
        wr_data[6]) );
  DFFRPQ_X1M_A12TR wr_data_reg_7_ ( .D(n2977), .CK(clock), .R(n1541), .Q(
        wr_data[7]) );
  DFFSQ_X1M_A12TR rd_enable_reg ( .D(n2978), .CK(clock), .SN(reset), .Q(
        rd_enable) );
  DFFRPQ_X1M_A12TR wr_enable_reg ( .D(n2979), .CK(clock), .R(n1547), .Q(
        wr_enable) );
  DFFRPQ_X1M_A12TR ack_q_reg ( .D(n2658), .CK(clock), .R(n1543), .Q(ack_q) );
  DFFRPQ_X1M_A12TR ack_q1_reg ( .D(ack_q), .CK(clock), .R(n1543), .Q(ack_q1)
         );
  MXT2_X1M_A12TR u1475 ( .A(n15681), .B(n15691), .S0(n2662), .Y(n1523) );
  OR3_X1M_A12TR u1476 ( .A(n1644), .B(n1645), .C(n1646), .Y(n1524) );
  MXT4_X1M_A12TR u1477 ( .A(n15791), .B(n15801), .C(n1524), .D(n15811), .S0(
        alu_ctrl[0]), .S1(alu_ctrl[1]), .Y(n1525) );
  OAI21_X1M_A12TR u1478 ( .A0(n2183), .A1(n1678), .B0(n1718), .Y(pc_ctrl[7])
         );
  AND2_X1M_A12TR u1479 ( .A(n1890), .B(n1871), .Y(u4_u4_z_0) );
  NOR2_X1M_A12TR u1480 ( .A(n1514), .B(n2660), .Y(n1566) );
  MXIT2_X0P7M_A12TR u1481 ( .A(n1514), .B(n1514), .S0(n2660), .Y(n15741) );
  MXIT2_X0P7M_A12TR u1482 ( .A(n1515), .B(n1515), .S0(n2660), .Y(n15761) );
  NAND2_X1M_A12TR u1483 ( .A(n1514), .B(n2660), .Y(n15781) );
  MXIT2_X0P7M_A12TR u1484 ( .A(n1515), .B(n2663), .S0(n2660), .Y(n15811) );
  OAI31_X1M_A12TR u1485 ( .A0(n2660), .A1(n73), .A2(alu_ctrl[1]), .B0(n15831), 
        .Y(n15721) );
  AO21B_X1M_A12TR u1486 ( .A0(n73), .A1(n2660), .B0N(n2662), .Y(n15831) );
  MXIT2_X0P7M_A12TR u1487 ( .A(n15731), .B(n15771), .S0(alu_ctrl[0]), .Y(
        n15821) );
  MXIT4_X1M_A12TR u1488 ( .A(n15741), .B(n15751), .C(n15851), .D(n15761), .S0(
        n2662), .S1(alu_ctrl[1]), .Y(n15731) );
  AOI21_X1M_A12TR u1489 ( .A0(n2662), .A1(n15781), .B0(n1566), .Y(n15771) );
  INV_X1M_A12TR u1490 ( .A(n1514), .Y(n15851) );
  MXIT4_X1M_A12TR u1491 ( .A(n73), .B(n25120), .C(n2794), .D(n2795), .S0(n2662), .S1(n2660), .Y(n15701) );
  BUFH_X1M_A12TR u1492 ( .A(n1562), .Y(n1545) );
  BUFH_X1M_A12TR u1493 ( .A(n1560), .Y(n1550) );
  BUFH_X1M_A12TR u1494 ( .A(n1560), .Y(n1549) );
  BUFH_X1M_A12TR u1495 ( .A(n1561), .Y(n1548) );
  BUFH_X1M_A12TR u1496 ( .A(n1561), .Y(n1547) );
  BUFH_X1M_A12TR u1497 ( .A(n1562), .Y(n1546) );
  BUFH_X1M_A12TR u1498 ( .A(n1563), .Y(n1544) );
  BUFH_X1M_A12TR u1499 ( .A(n1564), .Y(n1541) );
  BUFH_X1M_A12TR u1500 ( .A(n1563), .Y(n1543) );
  BUFH_X1M_A12TR u1501 ( .A(n1564), .Y(n1542) );
  BUFH_X1M_A12TR u1502 ( .A(n1559), .Y(n1551) );
  BUFH_X1M_A12TR u1503 ( .A(n1557), .Y(n1555) );
  BUFH_X1M_A12TR u1504 ( .A(n1558), .Y(n1554) );
  BUFH_X1M_A12TR u1505 ( .A(n1558), .Y(n1553) );
  BUFH_X1M_A12TR u1506 ( .A(n1559), .Y(n1552) );
  BUFH_X1M_A12TR u1507 ( .A(n1557), .Y(n1556) );
  MXIT2_X0P7M_A12TR u1508 ( .A(n2659), .B(n1522), .S0(n2660), .Y(n15751) );
  BUFH_X1M_A12TR u1509 ( .A(n2657), .Y(n1557) );
  BUFH_X1M_A12TR u1510 ( .A(n2657), .Y(n1558) );
  BUFH_X1M_A12TR u1511 ( .A(n2657), .Y(n1559) );
  BUFH_X1M_A12TR u1512 ( .A(n2657), .Y(n1560) );
  BUFH_X1M_A12TR u1513 ( .A(n2657), .Y(n1561) );
  BUFH_X1M_A12TR u1514 ( .A(n2657), .Y(n1562) );
  BUFH_X1M_A12TR u1515 ( .A(n2657), .Y(n1563) );
  BUFH_X1M_A12TR u1516 ( .A(n2657), .Y(n1564) );
  NOR2_X1M_A12TR u1517 ( .A(n1537), .B(u4_u2_z_14), .Y(n1565) );
  MXIT4_X1M_A12TR u1518 ( .A(n1514), .B(n1518), .C(n1516), .D(n1521), .S0(
        n2662), .S1(n2660), .Y(n15791) );
  MXIT4_X1M_A12TR u1519 ( .A(n1514), .B(n1519), .C(n1517), .D(n1520), .S0(
        n2662), .S1(n2660), .Y(n15801) );
  OR2_X1M_A12TR u1520 ( .A(r1183_carry_2_), .B(u4_u2_z_2), .Y(n1526) );
  OR2_X1M_A12TR u1521 ( .A(n1526), .B(u4_u2_z_3), .Y(n1527) );
  OR2_X1M_A12TR u1522 ( .A(n1527), .B(u4_u2_z_4), .Y(n1528) );
  OR2_X1M_A12TR u1523 ( .A(n1528), .B(u4_u2_z_5), .Y(n1529) );
  OR2_X1M_A12TR u1524 ( .A(n1529), .B(u4_u2_z_6), .Y(n1530) );
  OR2_X1M_A12TR u1525 ( .A(n1530), .B(u4_u2_z_7), .Y(n1531) );
  OR2_X1M_A12TR u1526 ( .A(n1531), .B(u4_u2_z_8), .Y(n1532) );
  OR2_X1M_A12TR u1527 ( .A(n1532), .B(u4_u2_z_9), .Y(n1533) );
  OR2_X1M_A12TR u1528 ( .A(n1533), .B(u4_u2_z_10), .Y(n1534) );
  OR2_X1M_A12TR u1529 ( .A(n1534), .B(u4_u2_z_11), .Y(n1535) );
  OR2_X1M_A12TR u1530 ( .A(n1535), .B(u4_u2_z_12), .Y(n1536) );
  OR2_X1M_A12TR u1531 ( .A(n1536), .B(u4_u2_z_13), .Y(n1537) );
  XOR2_X1M_A12TR u1532 ( .A(offset_sx[0]), .B(u4_u1_z_0), .Y(n1245) );
  XOR2_X1M_A12TR u1533 ( .A(pc_ctrl[0]), .B(program_ctr[0]), .Y(n15680) );
  MXIT4_X1M_A12TR u1534 ( .A(n73), .B(n24760), .C(n2664), .D(n2665), .S0(n2660), .S1(alu_ctrl[1]), .Y(n15681) );
  MXIT2_X0P7M_A12TR u1535 ( .A(n25480), .B(flags[1]), .S0(n2660), .Y(n15691)
         );
  XNOR2_X1M_A12TR u1536 ( .A(u4_u2_z_7), .B(n1530), .Y(n15910) );
  XNOR2_X1M_A12TR u1537 ( .A(u4_u2_z_4), .B(n1527), .Y(n15880) );
  XOR2_X1M_A12TR u1538 ( .A(u4_u2_z_15), .B(n1565), .Y(n15990) );
  XNOR2_X1M_A12TR u1539 ( .A(u4_u2_z_14), .B(n1537), .Y(n15980) );
  XNOR2_X1M_A12TR u1540 ( .A(u4_u2_z_13), .B(n1536), .Y(n15970) );
  XNOR2_X1M_A12TR u1541 ( .A(u4_u2_z_12), .B(n1535), .Y(n15960) );
  XNOR2_X1M_A12TR u1542 ( .A(u4_u2_z_11), .B(n1534), .Y(n15950) );
  XNOR2_X1M_A12TR u1543 ( .A(u4_u2_z_10), .B(n1533), .Y(n15940) );
  XNOR2_X1M_A12TR u1544 ( .A(u4_u2_z_9), .B(n1532), .Y(n15930) );
  XNOR2_X1M_A12TR u1545 ( .A(u4_u2_z_8), .B(n1531), .Y(n15920) );
  XNOR2_X1M_A12TR u1546 ( .A(u4_u2_z_6), .B(n1529), .Y(n15900) );
  XNOR2_X1M_A12TR u1547 ( .A(u4_u2_z_5), .B(n1528), .Y(n15890) );
  XNOR2_X1M_A12TR u1548 ( .A(u4_u2_z_3), .B(n1526), .Y(n15870) );
  XNOR2_X1M_A12TR u1549 ( .A(u4_u2_z_2), .B(r1183_carry_2_), .Y(n15860) );
  XNOR2_X1M_A12TR u1550 ( .A(n2655), .B(u4_u2_z_0), .Y(n15840) );
  MXIT2_X0P7M_A12TR u1551 ( .A(n73), .B(n1567), .S0(n2662), .Y(n15711) );
  MXIT2_X0P7M_A12TR u1552 ( .A(n1625), .B(n15841), .S0(alu_ctrl[1]), .Y(n1567)
         );
  INV_X1M_A12TR u1553 ( .A(n2664), .Y(n15841) );
  OR2_X1M_A12TR u1554 ( .A(u4_u2_z_0), .B(n2655), .Y(n1538) );
  AND2_X1M_A12TR u1555 ( .A(offset_sx[0]), .B(u4_u1_z_0), .Y(n1539) );
  AND2_X1M_A12TR u1556 ( .A(pc_ctrl[0]), .B(program_ctr[0]), .Y(n1540) );
  MXIT2_X0P7M_A12TR u1557 ( .A(n1525), .B(n15821), .S0(n2661), .Y(n2877) );
  MXIT4_X1M_A12TR u1558 ( .A(n15701), .B(n1523), .C(n15711), .D(n15721), .S0(
        alu_ctrl[0]), .S1(n2661), .Y(n2881) );
  TIELO_X1M_A12TR u1559 ( .Y(n81) );
  INV_X0P5B_A12TR u1560 ( .A(reset), .Y(n2657) );
  MXIT2_X0P5M_A12TR u1561 ( .A(n15861), .B(n15871), .S0(n15881), .Y(n2659) );
  AOI221_X0P5M_A12TR u1562 ( .A0(n15891), .A1(n15901), .B0(n15911), .B1(n15921), .C0(n15931), .Y(n15871) );
  OAI22_X0P5M_A12TR u1563 ( .A0(regfile[9]), .A1(n15941), .B0(regfile[15]), 
        .B1(n15951), .Y(n15931) );
  AOI221_X0P5M_A12TR u1564 ( .A0(n15891), .A1(n15961), .B0(n15911), .B1(n15971), .C0(n15981), .Y(n15861) );
  OAI22_X0P5M_A12TR u1565 ( .A0(regfile[8]), .A1(n15941), .B0(regfile[14]), 
        .B1(n15951), .Y(n15981) );
  NOR3_X0P5A_A12TR u1566 ( .A(n15991), .B(flags[1]), .C(n2801), .Y(n1521) );
  NOR3_X0P5A_A12TR u1567 ( .A(n15991), .B(flags[1]), .C(n2794), .Y(n1520) );
  NOR2_X0P5A_A12TR u1568 ( .A(n1600), .B(n1601), .Y(n1519) );
  NAND4_X0P5A_A12TR u1569 ( .A(n1602), .B(n1603), .C(n1604), .D(n1605), .Y(
        n1601) );
  XNOR2_X0P5M_A12TR u1570 ( .A(regfile[14]), .B(n2795), .Y(n1605) );
  XNOR2_X0P5M_A12TR u1571 ( .A(regfile[13]), .B(n2796), .Y(n1604) );
  XNOR2_X0P5M_A12TR u1572 ( .A(regfile[12]), .B(n2797), .Y(n1603) );
  XNOR2_X0P5M_A12TR u1573 ( .A(regfile[11]), .B(n2798), .Y(n1602) );
  NAND4_X0P5A_A12TR u1574 ( .A(n1606), .B(n1607), .C(n1608), .D(n1609), .Y(
        n1600) );
  XNOR2_X0P5M_A12TR u1575 ( .A(regfile[10]), .B(n2799), .Y(n1608) );
  XNOR2_X0P5M_A12TR u1576 ( .A(regfile[9]), .B(n2800), .Y(n1607) );
  XNOR2_X0P5M_A12TR u1577 ( .A(regfile[8]), .B(n2801), .Y(n1606) );
  NOR3_X0P5A_A12TR u1578 ( .A(n1610), .B(n1611), .C(n1612), .Y(n1518) );
  OAI22_X0P5M_A12TR u1579 ( .A0(n1613), .A1(n1614), .B0(n1615), .B1(n1616), 
        .Y(n1612) );
  OAI22_X0P5M_A12TR u1580 ( .A0(n1617), .A1(n15961), .B0(n1618), .B1(n15901), 
        .Y(n1611) );
  OAI211_X0P5M_A12TR u1581 ( .A0(n1619), .A1(n1620), .B0(n1621), .C0(n1622), 
        .Y(n1610) );
  AOI22_X0P5M_A12TR u1582 ( .A0(regfile[13]), .A1(n2796), .B0(regfile[12]), 
        .B1(n2797), .Y(n1622) );
  AND2_X0P5M_A12TR u1583 ( .A(n1516), .B(n1522), .Y(n1517) );
  NOR2_X0P5A_A12TR u1584 ( .A(n1623), .B(n1624), .Y(n1522) );
  NAND4_X0P5A_A12TR u1585 ( .A(n1614), .B(n1616), .C(n15961), .D(n15901), .Y(
        n1624) );
  NAND4_X0P5A_A12TR u1586 ( .A(n15971), .B(n15921), .C(n1620), .D(n1625), .Y(
        n1623) );
  NOR3_X0P5A_A12TR u1587 ( .A(n2794), .B(n2801), .C(n15991), .Y(n1516) );
  NAND4_X0P5A_A12TR u1588 ( .A(n1617), .B(n1618), .C(n1615), .D(n1626), .Y(
        n15991) );
  NOR3_X0P5A_A12TR u1589 ( .A(n2797), .B(n2795), .C(n2796), .Y(n1626) );
  NOR2_X0P5A_A12TR u1590 ( .A(n1627), .B(n1628), .Y(n1515) );
  OR4_X0P5M_A12TR u1591 ( .A(n1629), .B(n1630), .C(n1631), .D(n1632), .Y(n1628) );
  OR4_X0P5M_A12TR u1592 ( .A(n1633), .B(n2665), .C(n2664), .D(n2663), .Y(n1627) );
  NOR2_X0P5A_A12TR u1593 ( .A(n1634), .B(n1635), .Y(n1514) );
  NAND4_X0P5A_A12TR u1594 ( .A(n1636), .B(n1637), .C(n1638), .D(n1639), .Y(
        n1635) );
  NAND4_X0P5A_A12TR u1595 ( .A(n1640), .B(n1641), .C(n1642), .D(n1643), .Y(
        n1634) );
  NAND4_X0P5A_A12TR u1596 ( .A(n1647), .B(n1648), .C(n1649), .D(n1650), .Y(
        n1646) );
  NAND4B_X0P5M_A12TR u1597 ( .AN(mult[7]), .B(n1651), .C(n1652), .D(n1653), 
        .Y(n1645) );
  NAND4B_X0P5M_A12TR u1598 ( .AN(n1654), .B(n1655), .C(n1656), .D(n1657), .Y(
        n1644) );
  NOR2_X0P5A_A12TR u1599 ( .A(mult[12]), .B(mult[11]), .Y(n1656) );
  NAND4_X0P5A_A12TR u1600 ( .A(n329), .B(n328), .C(n327), .D(n326), .Y(n1654)
         );
  MXT2_X0P5M_A12TR u1601 ( .A(n2877), .B(flags[0]), .S0(n1658), .Y(n1491) );
  AOI211_X0P5M_A12TR u1602 ( .A0(n1659), .A1(n1660), .B0(n1661), .C0(n1662), 
        .Y(n1658) );
  MXIT2_X0P5M_A12TR u1603 ( .A(n1663), .B(n1664), .S0(n1665), .Y(n1490) );
  MXIT2_X0P5M_A12TR u1604 ( .A(n1666), .B(n1667), .S0(n1665), .Y(n1489) );
  MXIT2_X0P5M_A12TR u1605 ( .A(n1668), .B(n1669), .S0(n1665), .Y(n1488) );
  MXIT2_X0P5M_A12TR u1606 ( .A(n1670), .B(n1671), .S0(n1665), .Y(n1487) );
  MXIT2_X0P5M_A12TR u1607 ( .A(n1672), .B(n1673), .S0(n1665), .Y(n1486) );
  MXIT2_X0P5M_A12TR u1608 ( .A(n1674), .B(n1675), .S0(n1665), .Y(n1485) );
  MXIT2_X0P5M_A12TR u1609 ( .A(n1676), .B(n1677), .S0(n1665), .Y(n1484) );
  MXIT2_X0P5M_A12TR u1610 ( .A(n1678), .B(n1679), .S0(n1665), .Y(n1483) );
  NOR2_X0P5A_A12TR u1611 ( .A(n1680), .B(n1681), .Y(n1665) );
  MXIT2_X0P5M_A12TR u1612 ( .A(n1682), .B(n1664), .S0(n1683), .Y(n1482) );
  MXIT2_X0P5M_A12TR u1613 ( .A(n1684), .B(n1667), .S0(n1683), .Y(n14811) );
  MXIT2_X0P5M_A12TR u1614 ( .A(n1685), .B(n1669), .S0(n1683), .Y(n14801) );
  MXIT2_X0P5M_A12TR u1615 ( .A(n1686), .B(n1671), .S0(n1683), .Y(n14791) );
  MXIT2_X0P5M_A12TR u1616 ( .A(n16871), .B(n1673), .S0(n1683), .Y(n14781) );
  MXIT2_X0P5M_A12TR u1617 ( .A(n16881), .B(n1675), .S0(n1683), .Y(n14771) );
  MXIT2_X0P5M_A12TR u1618 ( .A(n16891), .B(n1677), .S0(n1683), .Y(n14761) );
  MXIT2_X0P5M_A12TR u1619 ( .A(n16901), .B(n1679), .S0(n1683), .Y(n14751) );
  NOR2B_X0P5M_A12TR u1620 ( .AN(n1681), .B(n1680), .Y(n1683) );
  MXIT2_X0P5M_A12TR u1621 ( .A(n16911), .B(n1664), .S0(n16921), .Y(n14741) );
  MXIT2_X0P5M_A12TR u1622 ( .A(n16931), .B(n1667), .S0(n16921), .Y(n14731) );
  MXIT2_X0P5M_A12TR u1623 ( .A(n492), .B(n1669), .S0(n16921), .Y(n14721) );
  MXIT2_X0P5M_A12TR u1624 ( .A(n491), .B(n1671), .S0(n16921), .Y(n14711) );
  MXIT2_X0P5M_A12TR u1625 ( .A(n490), .B(n1673), .S0(n16921), .Y(n14701) );
  MXIT2_X0P5M_A12TR u1626 ( .A(n489), .B(n1675), .S0(n16921), .Y(n14691) );
  MXIT2_X0P5M_A12TR u1627 ( .A(n488), .B(n1677), .S0(n16921), .Y(n14681) );
  MXIT2_X0P5M_A12TR u1628 ( .A(n487), .B(n1679), .S0(n16921), .Y(n14671) );
  OAI21B_X0P5M_A12TR u1629 ( .A0(n16941), .A1(n16951), .B0N(n16921), .Y(n14661) );
  AOI31_X0P5M_A12TR u1630 ( .A0(n16961), .A1(n16971), .A2(n16981), .B0(n16991), 
        .Y(n16921) );
  OAI222_X0P5M_A12TR u1631 ( .A0(n491), .A1(n17001), .B0(n1671), .B1(n17011), 
        .C0(n16941), .C1(n17021), .Y(n1465) );
  INV_X0P5B_A12TR u1632 ( .A(rd_data[3]), .Y(n1671) );
  OAI222_X0P5M_A12TR u1633 ( .A0(n490), .A1(n17001), .B0(n1673), .B1(n17011), 
        .C0(n16941), .C1(n1703), .Y(n1464) );
  INV_X0P5B_A12TR u1634 ( .A(rd_data[4]), .Y(n1673) );
  OAI222_X0P5M_A12TR u1635 ( .A0(n489), .A1(n17001), .B0(n1675), .B1(n17011), 
        .C0(n16941), .C1(n1704), .Y(n1463) );
  INV_X0P5B_A12TR u1636 ( .A(rd_data[5]), .Y(n1675) );
  OAI222_X0P5M_A12TR u1637 ( .A0(n488), .A1(n17001), .B0(n1677), .B1(n17011), 
        .C0(n16941), .C1(n1705), .Y(n1462) );
  INV_X0P5B_A12TR u1638 ( .A(rd_data[6]), .Y(n1677) );
  OAI222_X0P5M_A12TR u1639 ( .A0(n487), .A1(n17001), .B0(n1679), .B1(n17011), 
        .C0(n16941), .C1(n1706), .Y(n1461) );
  INV_X0P5B_A12TR u1640 ( .A(rd_data[7]), .Y(n1679) );
  OAI222_X0P5M_A12TR u1641 ( .A0(n16911), .A1(n17001), .B0(n1664), .B1(n17011), 
        .C0(n16941), .C1(n1707), .Y(n1460) );
  INV_X0P5B_A12TR u1642 ( .A(rd_data[0]), .Y(n1664) );
  INV_X0P5B_A12TR u1643 ( .A(prefetch[0]), .Y(n16911) );
  OAI222_X0P5M_A12TR u1644 ( .A0(n16931), .A1(n17001), .B0(n1667), .B1(n17011), 
        .C0(n16941), .C1(n1708), .Y(n1459) );
  INV_X0P5B_A12TR u1645 ( .A(rd_data[1]), .Y(n1667) );
  INV_X0P5B_A12TR u1646 ( .A(prefetch[1]), .Y(n16931) );
  OAI222_X0P5M_A12TR u1647 ( .A0(n492), .A1(n17001), .B0(n1669), .B1(n17011), 
        .C0(n16941), .C1(n1709), .Y(n1458) );
  OAI222_X0P5M_A12TR u1648 ( .A0(prefetch[0]), .A1(n17001), .B0(rd_data[0]), 
        .B1(n17011), .C0(n321), .C1(n16941), .Y(n1457) );
  OAI222_X0P5M_A12TR u1649 ( .A0(n1710), .A1(n17001), .B0(n1711), .B1(n17011), 
        .C0(n322), .C1(n16941), .Y(n1456) );
  XNOR2_X0P5M_A12TR u1650 ( .A(rd_data[0]), .B(rd_data[1]), .Y(n1711) );
  XNOR2_X0P5M_A12TR u1651 ( .A(prefetch[0]), .B(prefetch[1]), .Y(n1710) );
  OAI222_X0P5M_A12TR u1652 ( .A0(n17001), .A1(n1712), .B0(n1713), .B1(n17011), 
        .C0(n323), .C1(n16941), .Y(n1455) );
  NAND2_X0P5A_A12TR u1653 ( .A(n16941), .B(n16951), .Y(n17011) );
  INV_X0P5B_A12TR u1654 ( .A(instr_prefetch), .Y(n16951) );
  XNOR2_X0P5M_A12TR u1655 ( .A(n1714), .B(n1669), .Y(n1713) );
  INV_X0P5B_A12TR u1656 ( .A(rd_data[2]), .Y(n1669) );
  NAND2_X0P5A_A12TR u1657 ( .A(rd_data[1]), .B(rd_data[0]), .Y(n1714) );
  XNOR2_X0P5M_A12TR u1658 ( .A(n492), .B(n1715), .Y(n1712) );
  NAND2_X0P5A_A12TR u1659 ( .A(prefetch[1]), .B(prefetch[0]), .Y(n1715) );
  NAND2_X0P5A_A12TR u1660 ( .A(n16941), .B(instr_prefetch), .Y(n17001) );
  AND2_X0P5M_A12TR u1661 ( .A(n1681), .B(n1680), .Y(n16941) );
  OAI21_X0P5M_A12TR u1662 ( .A0(n1716), .A1(n1717), .B0(n1718), .Y(n1680) );
  NAND3B_X0P5M_A12TR u1663 ( .AN(n1719), .B(n1720), .C(n1721), .Y(n1717) );
  AOI211_X0P5M_A12TR u1664 ( .A0(n1722), .A1(n1723), .B0(n1724), .C0(n1725), 
        .Y(n1721) );
  NAND4B_X0P5M_A12TR u1665 ( .AN(n1726), .B(n1727), .C(n1728), .D(n1729), .Y(
        n1716) );
  NOR3_X0P5A_A12TR u1666 ( .A(n1730), .B(n1731), .C(n1732), .Y(n1729) );
  INV_X0P5B_A12TR u1667 ( .A(n1733), .Y(n1731) );
  AOI31_X0P5M_A12TR u1668 ( .A0(n1734), .A1(n1735), .A2(n1736), .B0(n16991), 
        .Y(n1681) );
  AND3_X0P5M_A12TR u1669 ( .A(n1737), .B(n1738), .C(n1739), .Y(n1734) );
  OAI222_X0P5M_A12TR u1670 ( .A0(n1740), .A1(n1625), .B0(n1741), .B1(n1742), 
        .C0(n1743), .C1(n1621), .Y(n1454) );
  INV_X0P5B_A12TR u1671 ( .A(n25120), .Y(n1621) );
  AOI211_X0P5M_A12TR u1672 ( .A0(mult[7]), .A1(n1662), .B0(n1744), .C0(n1745), 
        .Y(n1742) );
  OAI222_X0P5M_A12TR u1673 ( .A0(n1609), .A1(n1746), .B0(n1747), .B1(n1636), 
        .C0(n1748), .C1(n1749), .Y(n1744) );
  INV_X0P5B_A12TR u1674 ( .A(n73), .Y(n1636) );
  INV_X0P5B_A12TR u1675 ( .A(n1750), .Y(n1747) );
  INV_X0P5B_A12TR u1676 ( .A(n25480), .Y(n1609) );
  OAI211_X0P5M_A12TR u1677 ( .A0(n1748), .A1(n1613), .B0(n1751), .C0(n1752), 
        .Y(n1453) );
  NAND2_X0P5A_A12TR u1678 ( .A(n1753), .B(n1754), .Y(n1752) );
  OAI211_X0P5M_A12TR u1679 ( .A0(n1755), .A1(n1657), .B0(n1756), .C0(n1757), 
        .Y(n1753) );
  AOI32_X0P5M_A12TR u1680 ( .A0(n2801), .A1(n1614), .A2(n1758), .B0(n80), .B1(
        n1750), .Y(n1757) );
  INV_X0P5B_A12TR u1681 ( .A(mult[0]), .Y(n1657) );
  OAI21_X0P5M_A12TR u1682 ( .A0(n1759), .A1(n1760), .B0(regfile[8]), .Y(n1751)
         );
  MXIT2_X0P5M_A12TR u1683 ( .A(n1743), .B(n1746), .S0(n1613), .Y(n1760) );
  OAI211_X0P5M_A12TR u1684 ( .A0(n1748), .A1(n1615), .B0(n1761), .C0(n1762), 
        .Y(n1452) );
  NAND2_X0P5A_A12TR u1685 ( .A(n1763), .B(n1754), .Y(n1762) );
  OAI211_X0P5M_A12TR u1686 ( .A0(n1755), .A1(n1650), .B0(n1764), .C0(n1765), 
        .Y(n1763) );
  AOI32_X0P5M_A12TR u1687 ( .A0(n2800), .A1(n1616), .A2(n1758), .B0(n79), .B1(
        n1750), .Y(n1765) );
  INV_X0P5B_A12TR u1688 ( .A(mult[1]), .Y(n1650) );
  OAI21_X0P5M_A12TR u1689 ( .A0(n1759), .A1(n1766), .B0(regfile[9]), .Y(n1761)
         );
  MXIT2_X0P5M_A12TR u1690 ( .A(n1743), .B(n1746), .S0(n1615), .Y(n1766) );
  OAI211_X0P5M_A12TR u1691 ( .A0(n1748), .A1(n1617), .B0(n1767), .C0(n1768), 
        .Y(n1451) );
  NAND2_X0P5A_A12TR u1692 ( .A(n1769), .B(n1754), .Y(n1768) );
  OAI211_X0P5M_A12TR u1693 ( .A0(n329), .A1(n1755), .B0(n1770), .C0(n1771), 
        .Y(n1769) );
  AOI32_X0P5M_A12TR u1694 ( .A0(n2799), .A1(n15961), .A2(n1758), .B0(n78), 
        .B1(n1750), .Y(n1771) );
  OAI21_X0P5M_A12TR u1695 ( .A0(n1759), .A1(n1772), .B0(regfile[10]), .Y(n1767) );
  MXIT2_X0P5M_A12TR u1696 ( .A(n1743), .B(n1746), .S0(n1617), .Y(n1772) );
  OAI211_X0P5M_A12TR u1697 ( .A0(n1748), .A1(n1618), .B0(n1773), .C0(n1774), 
        .Y(n1450) );
  NAND2_X0P5A_A12TR u1698 ( .A(n1775), .B(n1754), .Y(n1774) );
  OAI211_X0P5M_A12TR u1699 ( .A0(n328), .A1(n1755), .B0(n1776), .C0(n1777), 
        .Y(n1775) );
  AOI32_X0P5M_A12TR u1700 ( .A0(n2798), .A1(n15901), .A2(n1758), .B0(n77), 
        .B1(n1750), .Y(n1777) );
  OAI21_X0P5M_A12TR u1701 ( .A0(n1759), .A1(n1778), .B0(regfile[11]), .Y(n1773) );
  MXIT2_X0P5M_A12TR u1702 ( .A(n1743), .B(n1746), .S0(n1618), .Y(n1778) );
  OAI211_X0P5M_A12TR u1703 ( .A0(n1748), .A1(n1779), .B0(n1780), .C0(n1781), 
        .Y(n1449) );
  NAND2_X0P5A_A12TR u1704 ( .A(n1782), .B(n1754), .Y(n1781) );
  OAI211_X0P5M_A12TR u1705 ( .A0(n327), .A1(n1755), .B0(n1783), .C0(n1784), 
        .Y(n1782) );
  AOI32_X0P5M_A12TR u1706 ( .A0(n2797), .A1(n15971), .A2(n1758), .B0(n76), 
        .B1(n1750), .Y(n1784) );
  OAI21_X0P5M_A12TR u1707 ( .A0(n1759), .A1(n1785), .B0(regfile[12]), .Y(n1780) );
  MXIT2_X0P5M_A12TR u1708 ( .A(n1743), .B(n1746), .S0(n1779), .Y(n1785) );
  OAI211_X0P5M_A12TR u1709 ( .A0(n1748), .A1(n1786), .B0(n1787), .C0(n1788), 
        .Y(n1448) );
  NAND2_X0P5A_A12TR u1710 ( .A(n1789), .B(n1754), .Y(n1788) );
  OAI211_X0P5M_A12TR u1711 ( .A0(n326), .A1(n1755), .B0(n1790), .C0(n1791), 
        .Y(n1789) );
  AOI32_X0P5M_A12TR u1712 ( .A0(n2796), .A1(n15921), .A2(n1758), .B0(n75), 
        .B1(n1750), .Y(n1791) );
  OAI21_X0P5M_A12TR u1713 ( .A0(n1759), .A1(n1792), .B0(regfile[13]), .Y(n1787) );
  MXIT2_X0P5M_A12TR u1714 ( .A(n1743), .B(n1746), .S0(n1786), .Y(n1792) );
  OAI211_X0P5M_A12TR u1715 ( .A0(n1748), .A1(n1619), .B0(n1793), .C0(n1794), 
        .Y(n1447) );
  NAND2_X0P5A_A12TR u1716 ( .A(n1795), .B(n1754), .Y(n1794) );
  OAI211_X0P5M_A12TR u1717 ( .A0(n1755), .A1(n1651), .B0(n1796), .C0(n1797), 
        .Y(n1795) );
  AOI32_X0P5M_A12TR u1718 ( .A0(n2795), .A1(n1620), .A2(n1758), .B0(n74), .B1(
        n1750), .Y(n1797) );
  NAND3_X0P5A_A12TR u1719 ( .A(n1798), .B(n1799), .C(n1800), .Y(n1750) );
  INV_X0P5B_A12TR u1720 ( .A(n1746), .Y(n1758) );
  INV_X0P5B_A12TR u1721 ( .A(mult[6]), .Y(n1651) );
  OAI21_X0P5M_A12TR u1722 ( .A0(n1759), .A1(n1801), .B0(regfile[14]), .Y(n1793) );
  MXIT2_X0P5M_A12TR u1723 ( .A(n1743), .B(n1746), .S0(n1619), .Y(n1801) );
  INV_X0P5B_A12TR u1724 ( .A(n1740), .Y(n1759) );
  NOR3_X0P5A_A12TR u1725 ( .A(n1802), .B(n1803), .C(n1741), .Y(n1740) );
  INV_X0P5B_A12TR u1726 ( .A(n1754), .Y(n1741) );
  NAND3_X0P5A_A12TR u1727 ( .A(n1800), .B(n1804), .C(n1805), .Y(n1754) );
  AOI211_X0P5M_A12TR u1728 ( .A0(n1806), .A1(n1660), .B0(n1662), .C0(n1807), 
        .Y(n1805) );
  INV_X0P5B_A12TR u1729 ( .A(n1799), .Y(n1807) );
  MXIT2_X0P5M_A12TR u1730 ( .A(n1808), .B(n1620), .S0(n1809), .Y(n1446) );
  OAI22_X0P5M_A12TR u1731 ( .A0(n1810), .A1(n1811), .B0(n1812), .B1(n1813), 
        .Y(n1445) );
  OR2_X0P5M_A12TR u1732 ( .A(n324), .B(n1812), .Y(n1811) );
  NOR3_X0P5A_A12TR u1733 ( .A(n1814), .B(n1815), .C(n1816), .Y(n1812) );
  AOI21_X0P5M_A12TR u1734 ( .A0(n1817), .A1(n1818), .B0(n1819), .Y(n1444) );
  MXIT2_X0P5M_A12TR u1735 ( .A(pending[0]), .B(n1820), .S0(n1810), .Y(n1819)
         );
  OAI22_X0P5M_A12TR u1736 ( .A0(n1821), .A1(n1822), .B0(n1810), .B1(n1823), 
        .Y(n1443) );
  NAND2B_X0P5M_A12TR u1737 ( .AN(n1821), .B(pending[1]), .Y(n1823) );
  NOR3_X0P5A_A12TR u1738 ( .A(n1815), .B(n1817), .C(n1824), .Y(n1821) );
  OAI31_X0P5M_A12TR u1739 ( .A0(n1810), .A1(n284), .A2(n1825), .B0(n1826), .Y(
        n1442) );
  OR2_X0P5M_A12TR u1740 ( .A(n1827), .B(n1825), .Y(n1826) );
  AND4_X0P5M_A12TR u1741 ( .A(n1828), .B(n1818), .C(n1824), .D(n1829), .Y(
        n1825) );
  OAI31_X0P5M_A12TR u1742 ( .A0(n1810), .A1(n283), .A2(n1830), .B0(n1831), .Y(
        n1441) );
  NAND3_X0P5A_A12TR u1743 ( .A(n1832), .B(n1833), .C(int_mask[3]), .Y(n1831)
         );
  INV_X0P5B_A12TR u1744 ( .A(n1833), .Y(n1830) );
  NAND3_X0P5A_A12TR u1745 ( .A(n1818), .B(n1834), .C(n1835), .Y(n1833) );
  OAI22_X0P5M_A12TR u1746 ( .A0(n1836), .A1(n1837), .B0(n1810), .B1(n1838), 
        .Y(n1440) );
  NAND2B_X0P5M_A12TR u1747 ( .AN(n1836), .B(pending[4]), .Y(n1838) );
  NOR3_X0P5A_A12TR u1748 ( .A(n1815), .B(n1839), .C(n1840), .Y(n1836) );
  OAI22_X0P5M_A12TR u1749 ( .A0(n1841), .A1(n1842), .B0(n1810), .B1(n1843), 
        .Y(n1439) );
  NAND2_X0P5A_A12TR u1750 ( .A(pending[5]), .B(n1844), .Y(n1843) );
  INV_X0P5B_A12TR u1751 ( .A(n1844), .Y(n1841) );
  NAND4_X0P5A_A12TR u1752 ( .A(n1845), .B(n1818), .C(n1846), .D(n1840), .Y(
        n1844) );
  OAI31_X0P5M_A12TR u1753 ( .A0(n1810), .A1(n1847), .A2(n1848), .B0(n1849), 
        .Y(n1438) );
  OR3_X0P5M_A12TR u1754 ( .A(n1850), .B(n1847), .C(n1808), .Y(n1849) );
  INV_X0P5B_A12TR u1755 ( .A(pending[6]), .Y(n1848) );
  AND3_X0P5M_A12TR u1756 ( .A(n1818), .B(n1851), .C(n1852), .Y(n1847) );
  NAND4_X0P5A_A12TR u1757 ( .A(n1842), .B(n1827), .C(n1853), .D(n1854), .Y(
        n1810) );
  AOI211_X0P5M_A12TR u1758 ( .A0(int_mask[3]), .A1(n1832), .B0(n1855), .C0(
        n1820), .Y(n1854) );
  AO21_X0P5M_A12TR u1759 ( .A0(n1707), .A1(n1856), .B0(interrupts[0]), .Y(
        n1820) );
  OAI21_X0P5M_A12TR u1760 ( .A0(n1850), .A1(n1808), .B0(n1837), .Y(n1855) );
  AO21A1AI2_X0P5M_A12TR u1761 ( .A0(n1857), .A1(n1722), .B0(interrupts[4]), 
        .C0(int_mask[4]), .Y(n1837) );
  AOI21_X0P5M_A12TR u1762 ( .A0(n1857), .A1(n1858), .B0(interrupts[6]), .Y(
        n1850) );
  AO21_X0P5M_A12TR u1763 ( .A0(n1857), .A1(n1859), .B0(interrupts[3]), .Y(
        n1832) );
  AND2_X0P5M_A12TR u1764 ( .A(n1813), .B(n1822), .Y(n1853) );
  AO21A1AI2_X0P5M_A12TR u1765 ( .A0(n1856), .A1(reg0[0]), .B0(interrupts[1]), 
        .C0(int_mask[1]), .Y(n1822) );
  AND2_X0P5M_A12TR u1766 ( .A(n1857), .B(n1860), .Y(n1856) );
  AO21A1AI2_X0P5M_A12TR u1767 ( .A0(n1857), .A1(n1861), .B0(interrupts[7]), 
        .C0(int_mask[7]), .Y(n1813) );
  AO21A1AI2_X0P5M_A12TR u1768 ( .A0(n1857), .A1(n1862), .B0(interrupts[2]), 
        .C0(int_mask[2]), .Y(n1827) );
  AO21A1AI2_X0P5M_A12TR u1769 ( .A0(n1857), .A1(n1863), .B0(interrupts[5]), 
        .C0(int_mask[5]), .Y(n1842) );
  OAI21B_X0P5M_A12TR u1770 ( .A0(int_ack), .A1(n1864), .B0N(n21420), .Y(n1437)
         );
  MXIT2_X0P5M_A12TR u1771 ( .A(n1865), .B(n1616), .S0(n1809), .Y(n1436) );
  MXIT2_X0P5M_A12TR u1772 ( .A(n1866), .B(n15961), .S0(n1809), .Y(n1435) );
  MXIT2_X0P5M_A12TR u1773 ( .A(n1867), .B(n15901), .S0(n1809), .Y(n1434) );
  MXIT2_X0P5M_A12TR u1774 ( .A(n1868), .B(n15971), .S0(n1809), .Y(n1433) );
  MXIT2_X0P5M_A12TR u1775 ( .A(n1869), .B(n15921), .S0(n1809), .Y(n1432) );
  MXIT2_X0P5M_A12TR u1776 ( .A(n1870), .B(n1625), .S0(n1809), .Y(n1431) );
  AND2_X0P5M_A12TR u1777 ( .A(n1863), .B(n1723), .Y(n1809) );
  AO22_X0P5M_A12TR u1778 ( .A0(n17020), .A1(n1871), .B0(stack_ptr[15]), .B1(
        n1872), .Y(n1430) );
  OAI211_X0P5M_A12TR u1779 ( .A0(n1873), .A1(n1874), .B0(n1875), .C0(n1876), 
        .Y(n1429) );
  NAND2_X0P5A_A12TR u1780 ( .A(n16870), .B(n1871), .Y(n1876) );
  INV_X0P5B_A12TR u1781 ( .A(stack_ptr[0]), .Y(n1874) );
  OAI211_X0P5M_A12TR u1782 ( .A0(n1873), .A1(n1877), .B0(n1875), .C0(n1878), 
        .Y(n1428) );
  NAND2_X0P5A_A12TR u1783 ( .A(n16880), .B(n1871), .Y(n1878) );
  INV_X0P5B_A12TR u1784 ( .A(stack_ptr[1]), .Y(n1877) );
  OAI211_X0P5M_A12TR u1785 ( .A0(n1873), .A1(n1879), .B0(n1875), .C0(n1880), 
        .Y(n1427) );
  NAND2_X0P5A_A12TR u1786 ( .A(n16890), .B(n1871), .Y(n1880) );
  INV_X0P5B_A12TR u1787 ( .A(stack_ptr[2]), .Y(n1879) );
  OAI211_X0P5M_A12TR u1788 ( .A0(n1873), .A1(n1881), .B0(n1875), .C0(n1882), 
        .Y(n1426) );
  NAND2_X0P5A_A12TR u1789 ( .A(n16900), .B(n1871), .Y(n1882) );
  INV_X0P5B_A12TR u1790 ( .A(stack_ptr[3]), .Y(n1881) );
  OAI211_X0P5M_A12TR u1791 ( .A0(n1873), .A1(n1883), .B0(n1875), .C0(n1884), 
        .Y(n1425) );
  NAND2_X0P5A_A12TR u1792 ( .A(n16910), .B(n1871), .Y(n1884) );
  INV_X0P5B_A12TR u1793 ( .A(stack_ptr[4]), .Y(n1883) );
  OAI211_X0P5M_A12TR u1794 ( .A0(n1873), .A1(n1885), .B0(n1875), .C0(n1886), 
        .Y(n1424) );
  NAND2_X0P5A_A12TR u1795 ( .A(n16920), .B(n1871), .Y(n1886) );
  INV_X0P5B_A12TR u1796 ( .A(stack_ptr[5]), .Y(n1885) );
  OAI211_X0P5M_A12TR u1797 ( .A0(n1873), .A1(n1887), .B0(n1875), .C0(n1888), 
        .Y(n1423) );
  NAND2_X0P5A_A12TR u1798 ( .A(n16930), .B(n1871), .Y(n1888) );
  INV_X0P5B_A12TR u1799 ( .A(stack_ptr[6]), .Y(n1887) );
  AO22_X0P5M_A12TR u1800 ( .A0(n16940), .A1(n1871), .B0(stack_ptr[7]), .B1(
        n1872), .Y(n1422) );
  AO22_X0P5M_A12TR u1801 ( .A0(n16950), .A1(n1871), .B0(stack_ptr[8]), .B1(
        n1872), .Y(n1421) );
  AO22_X0P5M_A12TR u1802 ( .A0(n16960), .A1(n1871), .B0(stack_ptr[9]), .B1(
        n1872), .Y(n1420) );
  AO22_X0P5M_A12TR u1803 ( .A0(n16970), .A1(n1871), .B0(stack_ptr[10]), .B1(
        n1872), .Y(n1419) );
  AO22_X0P5M_A12TR u1804 ( .A0(n16980), .A1(n1871), .B0(stack_ptr[11]), .B1(
        n1872), .Y(n1418) );
  AO22_X0P5M_A12TR u1805 ( .A0(n16990), .A1(n1871), .B0(stack_ptr[12]), .B1(
        n1872), .Y(n1417) );
  AO22_X0P5M_A12TR u1806 ( .A0(n17000), .A1(n1871), .B0(stack_ptr[13]), .B1(
        n1872), .Y(n1416) );
  AO22_X0P5M_A12TR u1807 ( .A0(n17010), .A1(n1871), .B0(stack_ptr[14]), .B1(
        n1872), .Y(n1415) );
  INV_X0P5B_A12TR u1808 ( .A(n1873), .Y(n1872) );
  NAND2_X0P5A_A12TR u1809 ( .A(n1889), .B(n1875), .Y(n1873) );
  NAND2_X0P5A_A12TR u1810 ( .A(n1890), .B(n1889), .Y(n1875) );
  MXIT2_X0P5M_A12TR u1811 ( .A(n1891), .B(n1892), .S0(hst_ptr[3]), .Y(n1414)
         );
  NOR3_X0P5A_A12TR u1812 ( .A(n1893), .B(n1894), .C(n1895), .Y(n1892) );
  OAI21_X0P5M_A12TR u1813 ( .A0(hst_ptr[0]), .A1(n1896), .B0(n1897), .Y(n1893)
         );
  MXIT2_X0P5M_A12TR u1814 ( .A(hst_ptr[1]), .B(n1896), .S0(n1898), .Y(n1897)
         );
  NAND4_X0P5A_A12TR u1815 ( .A(hst_ptr[2]), .B(n1899), .C(n1900), .D(n1898), 
        .Y(n1891) );
  XNOR2_X0P5M_A12TR u1816 ( .A(n1895), .B(hst_ptr[0]), .Y(n1413) );
  OAI21_X0P5M_A12TR u1817 ( .A0(n1901), .A1(n1902), .B0(n1903), .Y(n1412) );
  MXIT2_X0P5M_A12TR u1818 ( .A(n1904), .B(n1905), .S0(n1906), .Y(n1903) );
  NOR2_X0P5A_A12TR u1819 ( .A(n1895), .B(n1907), .Y(n1905) );
  XNOR2_X0P5M_A12TR u1820 ( .A(n1901), .B(hst_ptr[0]), .Y(n1907) );
  INV_X0P5B_A12TR u1821 ( .A(n1900), .Y(n1895) );
  MXIT2_X0P5M_A12TR u1822 ( .A(n1908), .B(n1909), .S0(n1896), .Y(n1411) );
  NAND2_X0P5A_A12TR u1823 ( .A(n1910), .B(n1900), .Y(n1909) );
  MXT2_X0P5M_A12TR u1824 ( .A(n1899), .B(n1911), .S0(n1901), .Y(n1910) );
  INV_X0P5B_A12TR u1825 ( .A(n1898), .Y(n1901) );
  AOI211_X0P5M_A12TR u1826 ( .A0(n1898), .A1(n1906), .B0(n1904), .C0(n1912), 
        .Y(n1908) );
  OAI21_X0P5M_A12TR u1827 ( .A0(n1913), .A1(n1898), .B0(n1900), .Y(n1904) );
  NAND2_X0P5A_A12TR u1828 ( .A(n1898), .B(n1864), .Y(n1900) );
  NAND2_X0P5A_A12TR u1829 ( .A(int_rti), .B(n1816), .Y(n1898) );
  AO22_X0P5M_A12TR u1830 ( .A0(n21710), .A1(n1914), .B0(isr_addr[15]), .B1(
        n1915), .Y(n1410) );
  AO22_X0P5M_A12TR u1831 ( .A0(n21560), .A1(n1914), .B0(isr_addr[0]), .B1(
        n1915), .Y(n1409) );
  OAI221_X0P5M_A12TR u1832 ( .A0(n1916), .A1(n1917), .B0(n1918), .B1(n1919), 
        .C0(n1920), .Y(n1408) );
  NAND2_X0P5A_A12TR u1833 ( .A(n21570), .B(n1914), .Y(n1920) );
  INV_X0P5B_A12TR u1834 ( .A(isr_addr[1]), .Y(n1919) );
  AO21A1AI2_X0P5M_A12TR u1835 ( .A0(n1921), .A1(n1922), .B0(n1923), .C0(n1829), 
        .Y(n1917) );
  AO1B2_X0P5M_A12TR u1836 ( .B0(n21580), .B1(n1914), .A0N(n1924), .Y(n1407) );
  AOI32_X0P5M_A12TR u1837 ( .A0(n1824), .A1(n1829), .A2(n1925), .B0(
        isr_addr[2]), .B1(n1915), .Y(n1924) );
  AOI21_X0P5M_A12TR u1838 ( .A0(n1926), .A1(n1922), .B0(n1916), .Y(n1925) );
  OAI221_X0P5M_A12TR u1839 ( .A0(n1839), .A1(n1916), .B0(n1918), .B1(n1927), 
        .C0(n1928), .Y(n1406) );
  NAND2_X0P5A_A12TR u1840 ( .A(n21590), .B(n1914), .Y(n1928) );
  INV_X0P5B_A12TR u1841 ( .A(isr_addr[3]), .Y(n1927) );
  AO22_X0P5M_A12TR u1842 ( .A0(n21600), .A1(n1914), .B0(isr_addr[4]), .B1(
        n1915), .Y(n1405) );
  AO22_X0P5M_A12TR u1843 ( .A0(n21610), .A1(n1914), .B0(isr_addr[5]), .B1(
        n1915), .Y(n1404) );
  AO22_X0P5M_A12TR u1844 ( .A0(n21620), .A1(n1914), .B0(isr_addr[6]), .B1(
        n1915), .Y(n1403) );
  NAND2_X0P5A_A12TR u1845 ( .A(n1929), .B(n1916), .Y(n1402) );
  NAND2_X0P5A_A12TR u1846 ( .A(n1918), .B(n1930), .Y(n1916) );
  MXIT2_X0P5M_A12TR u1847 ( .A(isr_addr[7]), .B(n21630), .S0(n1918), .Y(n1929)
         );
  AO22_X0P5M_A12TR u1848 ( .A0(n21640), .A1(n1914), .B0(isr_addr[8]), .B1(
        n1915), .Y(n1401) );
  AO22_X0P5M_A12TR u1849 ( .A0(n21650), .A1(n1914), .B0(isr_addr[9]), .B1(
        n1915), .Y(n1400) );
  AO22_X0P5M_A12TR u1850 ( .A0(n21660), .A1(n1914), .B0(isr_addr[10]), .B1(
        n1915), .Y(n1399) );
  AO22_X0P5M_A12TR u1851 ( .A0(n21670), .A1(n1914), .B0(isr_addr[11]), .B1(
        n1915), .Y(n1398) );
  AO22_X0P5M_A12TR u1852 ( .A0(n21680), .A1(n1914), .B0(isr_addr[12]), .B1(
        n1915), .Y(n1397) );
  AO22_X0P5M_A12TR u1853 ( .A0(n21690), .A1(n1914), .B0(isr_addr[13]), .B1(
        n1915), .Y(n1396) );
  AO22_X0P5M_A12TR u1854 ( .A0(n21700), .A1(n1914), .B0(isr_addr[14]), .B1(
        n1915), .Y(n1395) );
  INV_X0P5B_A12TR u1855 ( .A(n1918), .Y(n1915) );
  NAND2_X0P5A_A12TR u1856 ( .A(n1930), .B(n1864), .Y(n1918) );
  INV_X0P5B_A12TR u1857 ( .A(n1930), .Y(n1914) );
  MXIT2_X0P5M_A12TR u1858 ( .A(n1931), .B(n1932), .S0(n1933), .Y(n1394) );
  MXIT2_X0P5M_A12TR u1859 ( .A(n1934), .B(n1935), .S0(n1933), .Y(n1393) );
  MXIT2_X0P5M_A12TR u1860 ( .A(n1936), .B(n1839), .S0(n1933), .Y(n1392) );
  AND3_X0P5M_A12TR u1861 ( .A(hst_ptr[2]), .B(hst_ptr[1]), .C(n1937), .Y(n1933) );
  MXIT2_X0P5M_A12TR u1862 ( .A(n1938), .B(n1932), .S0(n1939), .Y(n1391) );
  MXIT2_X0P5M_A12TR u1863 ( .A(n1940), .B(n1935), .S0(n1939), .Y(n1390) );
  MXIT2_X0P5M_A12TR u1864 ( .A(n1941), .B(n1839), .S0(n1939), .Y(n1389) );
  AND3_X0P5M_A12TR u1865 ( .A(hst_ptr[2]), .B(hst_ptr[1]), .C(n1942), .Y(n1939) );
  MXIT2_X0P5M_A12TR u1866 ( .A(n1943), .B(n1932), .S0(n1944), .Y(n1388) );
  MXIT2_X0P5M_A12TR u1867 ( .A(n1945), .B(n1935), .S0(n1944), .Y(n1387) );
  MXIT2_X0P5M_A12TR u1868 ( .A(n1946), .B(n1839), .S0(n1944), .Y(n1386) );
  AND3_X0P5M_A12TR u1869 ( .A(hst_ptr[2]), .B(n1906), .C(n1937), .Y(n1944) );
  MXIT2_X0P5M_A12TR u1870 ( .A(n1947), .B(n1932), .S0(n1948), .Y(n1385) );
  INV_X0P5B_A12TR u1871 ( .A(history[9]), .Y(n1947) );
  MXIT2_X0P5M_A12TR u1872 ( .A(n1949), .B(n1935), .S0(n1948), .Y(n1384) );
  INV_X0P5B_A12TR u1873 ( .A(history[10]), .Y(n1949) );
  MXT2_X0P5M_A12TR u1874 ( .A(history[11]), .B(n1846), .S0(n1948), .Y(n1383)
         );
  AND3_X0P5M_A12TR u1875 ( .A(hst_ptr[2]), .B(n1906), .C(n1942), .Y(n1948) );
  MXIT2_X0P5M_A12TR u1876 ( .A(n1950), .B(n1932), .S0(n1951), .Y(n1382) );
  INV_X0P5B_A12TR u1877 ( .A(history[12]), .Y(n1950) );
  MXIT2_X0P5M_A12TR u1878 ( .A(n1952), .B(n1935), .S0(n1951), .Y(n1381) );
  INV_X0P5B_A12TR u1879 ( .A(history[13]), .Y(n1952) );
  MXT2_X0P5M_A12TR u1880 ( .A(history[14]), .B(n1846), .S0(n1951), .Y(n1380)
         );
  AND3_X0P5M_A12TR u1881 ( .A(hst_ptr[1]), .B(n1896), .C(n1937), .Y(n1951) );
  INV_X0P5B_A12TR u1882 ( .A(n1839), .Y(n1846) );
  MXIT2_X0P5M_A12TR u1883 ( .A(n1953), .B(n1932), .S0(n1954), .Y(n1379) );
  MXIT2_X0P5M_A12TR u1884 ( .A(n1955), .B(n1935), .S0(n1954), .Y(n1378) );
  MXIT2_X0P5M_A12TR u1885 ( .A(n1956), .B(n1839), .S0(n1954), .Y(n1377) );
  AND3_X0P5M_A12TR u1886 ( .A(hst_ptr[1]), .B(n1896), .C(n1942), .Y(n1954) );
  MXIT2_X0P5M_A12TR u1887 ( .A(n1957), .B(n1932), .S0(n1958), .Y(n1376) );
  MXIT2_X0P5M_A12TR u1888 ( .A(n1959), .B(n1935), .S0(n1958), .Y(n1375) );
  MXIT2_X0P5M_A12TR u1889 ( .A(n1960), .B(n1839), .S0(n1958), .Y(n1374) );
  AND2_X0P5M_A12TR u1890 ( .A(n1937), .B(n1961), .Y(n1958) );
  NOR2_X0P5A_A12TR u1891 ( .A(n1864), .B(n1913), .Y(n1937) );
  MXIT2_X0P5M_A12TR u1892 ( .A(n1962), .B(n1932), .S0(n1963), .Y(n1373) );
  OAI31_X0P5M_A12TR u1893 ( .A0(n1921), .A1(n1828), .A2(n1923), .B0(n1829), 
        .Y(n1932) );
  AO21A1AI2_X0P5M_A12TR u1894 ( .A0(n1852), .A1(n1964), .B0(n1965), .C0(n1966), 
        .Y(n1921) );
  INV_X0P5B_A12TR u1895 ( .A(n1967), .Y(n1852) );
  MXIT2_X0P5M_A12TR u1896 ( .A(n1968), .B(n1935), .S0(n1963), .Y(n1372) );
  NAND2B_X0P5M_A12TR u1897 ( .AN(n1926), .B(n1834), .Y(n1935) );
  AOI21_X0P5M_A12TR u1898 ( .A0(n1840), .A1(n1964), .B0(n1835), .Y(n1926) );
  MXIT2_X0P5M_A12TR u1899 ( .A(n1969), .B(n1839), .S0(n1963), .Y(n1371) );
  AND2_X0P5M_A12TR u1900 ( .A(n1942), .B(n1961), .Y(n1963) );
  NOR2_X0P5A_A12TR u1901 ( .A(n1864), .B(hst_ptr[0]), .Y(n1942) );
  OAI21_X0P5M_A12TR u1902 ( .A0(n1970), .A1(n1814), .B0(n1818), .Y(n1864) );
  INV_X0P5B_A12TR u1903 ( .A(n1815), .Y(n1818) );
  OAI21_X0P5M_A12TR u1904 ( .A0(n1971), .A1(n1972), .B0(n1973), .Y(n1815) );
  NAND4B_X0P5M_A12TR u1905 ( .AN(pending[0]), .B(n324), .C(n284), .D(n283), 
        .Y(n1972) );
  OR4_X0P5M_A12TR u1906 ( .A(pending[1]), .B(pending[4]), .C(pending[5]), .D(
        pending[6]), .Y(n1971) );
  NAND2_X0P5A_A12TR u1907 ( .A(n1851), .B(n1967), .Y(n1814) );
  OAI211_X0P5M_A12TR u1908 ( .A0(n1970), .A1(n1974), .B0(n1975), .C0(
        pending[6]), .Y(n1967) );
  NOR3_X0P5A_A12TR u1909 ( .A(n1845), .B(n1965), .C(n1839), .Y(n1851) );
  INV_X0P5B_A12TR u1910 ( .A(n1840), .Y(n1965) );
  AO21A1AI2_X0P5M_A12TR u1911 ( .A0(n1976), .A1(n1974), .B0(n1975), .C0(
        pending[4]), .Y(n1840) );
  INV_X0P5B_A12TR u1912 ( .A(n1964), .Y(n1845) );
  NAND2_X0P5A_A12TR u1913 ( .A(pending[5]), .B(n1975), .Y(n1964) );
  AO1B2_X0P5M_A12TR u1914 ( .B0(n1977), .B1(n1976), .A0N(n1816), .Y(n1975) );
  INV_X0P5B_A12TR u1915 ( .A(n1816), .Y(n1970) );
  NAND2_X0P5A_A12TR u1916 ( .A(n1834), .B(n1966), .Y(n1839) );
  INV_X0P5B_A12TR u1917 ( .A(n1835), .Y(n1966) );
  NOR2B_X0P5M_A12TR u1918 ( .AN(n1978), .B(n283), .Y(n1835) );
  NOR3_X0P5A_A12TR u1919 ( .A(n1817), .B(n1828), .C(n1923), .Y(n1834) );
  INV_X0P5B_A12TR u1920 ( .A(n1824), .Y(n1923) );
  OAI21_X0P5M_A12TR u1921 ( .A0(n1977), .A1(n1978), .B0(pending[1]), .Y(n1824)
         );
  INV_X0P5B_A12TR u1922 ( .A(n1922), .Y(n1828) );
  AO21A1AI2_X0P5M_A12TR u1923 ( .A0(n1977), .A1(n1974), .B0(n1978), .C0(n1979), 
        .Y(n1922) );
  INV_X0P5B_A12TR u1924 ( .A(n284), .Y(n1979) );
  INV_X0P5B_A12TR u1925 ( .A(n1829), .Y(n1817) );
  OAI31_X0P5M_A12TR u1926 ( .A0(n1978), .A1(n1977), .A2(n1974), .B0(pending[0]), .Y(n1829) );
  MXIT2_X0P5M_A12TR u1927 ( .A(n1980), .B(n1931), .S0(hst_ptr[3]), .Y(n1974)
         );
  INV_X0P5B_A12TR u1928 ( .A(history[0]), .Y(n1931) );
  MXIT2_X0P5M_A12TR u1929 ( .A(n1981), .B(n1982), .S0(n1896), .Y(n1980) );
  OAI222_X0P5M_A12TR u1930 ( .A0(n1902), .A1(n1957), .B0(n1983), .B1(n1962), 
        .C0(n1984), .C1(n1953), .Y(n1982) );
  INV_X0P5B_A12TR u1931 ( .A(history[15]), .Y(n1953) );
  INV_X0P5B_A12TR u1932 ( .A(history[21]), .Y(n1962) );
  INV_X0P5B_A12TR u1933 ( .A(history[18]), .Y(n1957) );
  OAI221_X0P5M_A12TR u1934 ( .A0(n1902), .A1(n1943), .B0(n1984), .B1(n1938), 
        .C0(n1985), .Y(n1981) );
  AOI22_X0P5M_A12TR u1935 ( .A0(history[12]), .A1(n1911), .B0(history[9]), 
        .B1(n1894), .Y(n1985) );
  INV_X0P5B_A12TR u1936 ( .A(history[3]), .Y(n1938) );
  INV_X0P5B_A12TR u1937 ( .A(history[6]), .Y(n1943) );
  MXIT2_X0P5M_A12TR u1938 ( .A(n1986), .B(n1934), .S0(hst_ptr[3]), .Y(n1977)
         );
  INV_X0P5B_A12TR u1939 ( .A(history[1]), .Y(n1934) );
  MXIT2_X0P5M_A12TR u1940 ( .A(n1987), .B(n1988), .S0(n1896), .Y(n1986) );
  OAI222_X0P5M_A12TR u1941 ( .A0(n1902), .A1(n1959), .B0(n1983), .B1(n1968), 
        .C0(n1984), .C1(n1955), .Y(n1988) );
  INV_X0P5B_A12TR u1942 ( .A(history[16]), .Y(n1955) );
  INV_X0P5B_A12TR u1943 ( .A(history[22]), .Y(n1968) );
  INV_X0P5B_A12TR u1944 ( .A(history[19]), .Y(n1959) );
  OAI221_X0P5M_A12TR u1945 ( .A0(n1902), .A1(n1945), .B0(n1984), .B1(n1940), 
        .C0(n1989), .Y(n1987) );
  AOI22_X0P5M_A12TR u1946 ( .A0(history[13]), .A1(n1911), .B0(history[10]), 
        .B1(n1894), .Y(n1989) );
  INV_X0P5B_A12TR u1947 ( .A(history[4]), .Y(n1940) );
  INV_X0P5B_A12TR u1948 ( .A(history[7]), .Y(n1945) );
  NAND2B_X0P5M_A12TR u1949 ( .AN(n1976), .B(n1816), .Y(n1978) );
  NAND3B_X0P5M_A12TR u1950 ( .AN(hst_ptr[3]), .B(n1913), .C(n1961), .Y(n1816)
         );
  NOR2_X0P5A_A12TR u1951 ( .A(hst_ptr[2]), .B(hst_ptr[1]), .Y(n1961) );
  MXIT2_X0P5M_A12TR u1952 ( .A(n1990), .B(n1936), .S0(hst_ptr[3]), .Y(n1976)
         );
  INV_X0P5B_A12TR u1953 ( .A(history[2]), .Y(n1936) );
  MXIT2_X0P5M_A12TR u1954 ( .A(n1991), .B(n1992), .S0(n1896), .Y(n1990) );
  INV_X0P5B_A12TR u1955 ( .A(hst_ptr[2]), .Y(n1896) );
  OAI222_X0P5M_A12TR u1956 ( .A0(n1902), .A1(n1960), .B0(n1983), .B1(n1969), 
        .C0(n1984), .C1(n1956), .Y(n1992) );
  INV_X0P5B_A12TR u1957 ( .A(history[17]), .Y(n1956) );
  INV_X0P5B_A12TR u1958 ( .A(n1894), .Y(n1983) );
  INV_X0P5B_A12TR u1959 ( .A(history[20]), .Y(n1960) );
  OAI221_X0P5M_A12TR u1960 ( .A0(n1902), .A1(n1946), .B0(n1984), .B1(n1941), 
        .C0(n1993), .Y(n1991) );
  AOI22_X0P5M_A12TR u1961 ( .A0(history[14]), .A1(n1911), .B0(history[11]), 
        .B1(n1894), .Y(n1993) );
  NOR2_X0P5A_A12TR u1962 ( .A(n1913), .B(hst_ptr[1]), .Y(n1894) );
  NOR2_X0P5A_A12TR u1963 ( .A(hst_ptr[0]), .B(hst_ptr[1]), .Y(n1911) );
  INV_X0P5B_A12TR u1964 ( .A(history[5]), .Y(n1941) );
  INV_X0P5B_A12TR u1965 ( .A(n1899), .Y(n1984) );
  NOR2_X0P5A_A12TR u1966 ( .A(n1906), .B(n1913), .Y(n1899) );
  INV_X0P5B_A12TR u1967 ( .A(hst_ptr[0]), .Y(n1913) );
  INV_X0P5B_A12TR u1968 ( .A(history[8]), .Y(n1946) );
  INV_X0P5B_A12TR u1969 ( .A(n1912), .Y(n1902) );
  NOR2_X0P5A_A12TR u1970 ( .A(n1906), .B(hst_ptr[0]), .Y(n1912) );
  INV_X0P5B_A12TR u1971 ( .A(hst_ptr[1]), .Y(n1906) );
  INV_X0P5B_A12TR u1972 ( .A(history[23]), .Y(n1969) );
  MXT2_X0P5M_A12TR u1973 ( .A(n2881), .B(flags[2]), .S0(n1994), .Y(n1370) );
  AOI21_X0P5M_A12TR u1974 ( .A0(n1659), .A1(n15891), .B0(n1661), .Y(n1994) );
  NAND3B_X0P5M_A12TR u1975 ( .AN(n1995), .B(n1996), .C(n1804), .Y(n1661) );
  AND4_X0P5M_A12TR u1976 ( .A(n1997), .B(n1746), .C(n1743), .D(n1748), .Y(
        n1804) );
  AOI21_X0P5M_A12TR u1977 ( .A0(n1998), .A1(n1999), .B0(n1803), .Y(n1748) );
  AND2_X0P5M_A12TR u1978 ( .A(n1998), .B(n2000), .Y(n1803) );
  NAND2_X0P5A_A12TR u1979 ( .A(n2001), .B(n2002), .Y(n1743) );
  NAND2_X0P5A_A12TR u1980 ( .A(n2003), .B(n2002), .Y(n1746) );
  NAND2_X0P5A_A12TR u1981 ( .A(n2004), .B(n2005), .Y(n1997) );
  INV_X0P5B_A12TR u1982 ( .A(n2006), .Y(n1996) );
  OAI221_X0P5M_A12TR u1983 ( .A0(n2007), .A1(alu_ctrl[0]), .B0(n2008), .B1(
        alu_ctrl[1]), .C0(n2009), .Y(n1995) );
  MXIT2_X0P5M_A12TR u1984 ( .A(n2010), .B(n281), .S0(n2011), .Y(n1369) );
  AOI21_X0P5M_A12TR u1985 ( .A0(n2012), .A1(n15891), .B0(n2013), .Y(n2011) );
  NOR2_X0P5A_A12TR u1986 ( .A(n2014), .B(n1632), .Y(n2010) );
  MXIT2_X0P5M_A12TR u1987 ( .A(n2015), .B(n279), .S0(n2016), .Y(n1368) );
  AOI21_X0P5M_A12TR u1988 ( .A0(n1659), .A1(n15911), .B0(n2013), .Y(n2016) );
  NOR2_X0P5A_A12TR u1989 ( .A(n2014), .B(n1631), .Y(n2015) );
  MXIT2_X0P5M_A12TR u1990 ( .A(n2017), .B(n282), .S0(n2018), .Y(n1367) );
  AOI21_X0P5M_A12TR u1991 ( .A0(n2012), .A1(n15911), .B0(n2013), .Y(n2018) );
  NOR2_X0P5A_A12TR u1992 ( .A(n2014), .B(n1630), .Y(n2017) );
  MXIT2_X0P5M_A12TR u1993 ( .A(n2019), .B(n280), .S0(n2020), .Y(n1366) );
  AOI21_X0P5M_A12TR u1994 ( .A0(n1659), .A1(n2021), .B0(n2013), .Y(n2020) );
  AOI21_X0P5M_A12TR u1995 ( .A0(n2022), .A1(n2023), .B0(n15881), .Y(n1659) );
  NOR2_X0P5A_A12TR u1996 ( .A(n2014), .B(n1629), .Y(n2019) );
  MXIT2_X0P5M_A12TR u1997 ( .A(n2024), .B(n278), .S0(n2025), .Y(n1365) );
  AOI21_X0P5M_A12TR u1998 ( .A0(n2012), .A1(n2021), .B0(n2013), .Y(n2025) );
  NOR2_X0P5A_A12TR u1999 ( .A(n2664), .B(n2014), .Y(n2024) );
  INV_X0P5B_A12TR u2000 ( .A(n2022), .Y(n2014) );
  MXIT2_X0P5M_A12TR u2001 ( .A(n2026), .B(n2027), .S0(n2028), .Y(n1364) );
  AOI211_X0P5M_A12TR u2002 ( .A0(n2012), .A1(n1660), .B0(n2029), .C0(n2006), 
        .Y(n2028) );
  NAND4B_X0P5M_A12TR u2003 ( .AN(n2013), .B(n2030), .C(n2031), .D(n2032), .Y(
        n2006) );
  NOR3_X0P5A_A12TR u2004 ( .A(n2033), .B(n2034), .C(n2035), .Y(n2013) );
  AOI21B_X0P5M_A12TR u2005 ( .A0(n2022), .A1(n2023), .B0N(n15881), .Y(n2012)
         );
  NAND2_X0P5A_A12TR u2006 ( .A(n2004), .B(n2003), .Y(n2023) );
  AOI211_X0P5M_A12TR u2007 ( .A0(n2036), .A1(n2801), .B0(n2037), .C0(n1633), 
        .Y(n2026) );
  OAI211_X0P5M_A12TR u2008 ( .A0(n1749), .A1(n2031), .B0(n2038), .C0(n2022), 
        .Y(n2037) );
  NAND2_X0P5A_A12TR u2009 ( .A(n2000), .B(n2039), .Y(n2022) );
  OAI21_X0P5M_A12TR u2010 ( .A0(n2040), .A1(n2029), .B0(n72), .Y(n2038) );
  MXIT2_X0P5M_A12TR u2011 ( .A(n266), .B(n2041), .S0(n2042), .Y(n1363) );
  MXIT2_X0P5M_A12TR u2012 ( .A(n285), .B(n2043), .S0(n2042), .Y(n1362) );
  MXIT2_X0P5M_A12TR u2013 ( .A(n315), .B(n2044), .S0(n2042), .Y(n1361) );
  MXIT2_X0P5M_A12TR u2014 ( .A(n309), .B(n2045), .S0(n2042), .Y(n1360) );
  MXIT2_X0P5M_A12TR u2015 ( .A(n303), .B(n2046), .S0(n2042), .Y(n1359) );
  MXIT2_X0P5M_A12TR u2016 ( .A(n297), .B(n2047), .S0(n2042), .Y(n1358) );
  MXIT2_X0P5M_A12TR u2017 ( .A(n291), .B(n2048), .S0(n2042), .Y(n1357) );
  MXIT2_X0P5M_A12TR u2018 ( .A(n272), .B(n2049), .S0(n2042), .Y(n1356) );
  NOR2_X0P5A_A12TR u2019 ( .A(n2050), .B(n15951), .Y(n2042) );
  MXIT2_X0P5M_A12TR u2020 ( .A(n267), .B(n2041), .S0(n2051), .Y(n1355) );
  MXIT2_X0P5M_A12TR u2021 ( .A(n286), .B(n2043), .S0(n2051), .Y(n1354) );
  MXIT2_X0P5M_A12TR u2022 ( .A(n316), .B(n2044), .S0(n2051), .Y(n1353) );
  MXIT2_X0P5M_A12TR u2023 ( .A(n310), .B(n2045), .S0(n2051), .Y(n1352) );
  MXIT2_X0P5M_A12TR u2024 ( .A(n304), .B(n2046), .S0(n2051), .Y(n1351) );
  MXIT2_X0P5M_A12TR u2025 ( .A(n298), .B(n2047), .S0(n2051), .Y(n1350) );
  MXIT2_X0P5M_A12TR u2026 ( .A(n292), .B(n2048), .S0(n2051), .Y(n1349) );
  MXIT2_X0P5M_A12TR u2027 ( .A(n273), .B(n2049), .S0(n2051), .Y(n1348) );
  AND2_X0P5M_A12TR u2028 ( .A(n1806), .B(n2021), .Y(n2051) );
  MXIT2_X0P5M_A12TR u2029 ( .A(n268), .B(n2041), .S0(n2052), .Y(n1347) );
  MXIT2_X0P5M_A12TR u2030 ( .A(n287), .B(n2043), .S0(n2052), .Y(n1346) );
  MXIT2_X0P5M_A12TR u2031 ( .A(n317), .B(n2044), .S0(n2052), .Y(n1345) );
  MXIT2_X0P5M_A12TR u2032 ( .A(n311), .B(n2045), .S0(n2052), .Y(n1344) );
  MXIT2_X0P5M_A12TR u2033 ( .A(n305), .B(n2046), .S0(n2052), .Y(n1343) );
  MXIT2_X0P5M_A12TR u2034 ( .A(n299), .B(n2047), .S0(n2052), .Y(n1342) );
  MXIT2_X0P5M_A12TR u2035 ( .A(n293), .B(n2048), .S0(n2052), .Y(n1341) );
  MXIT2_X0P5M_A12TR u2036 ( .A(n274), .B(n2049), .S0(n2052), .Y(n1340) );
  NOR2B_X0P5M_A12TR u2037 ( .AN(n15911), .B(n2050), .Y(n2052) );
  MXIT2_X0P5M_A12TR u2038 ( .A(n269), .B(n2041), .S0(n2053), .Y(n1339) );
  MXIT2_X0P5M_A12TR u2039 ( .A(n288), .B(n2043), .S0(n2053), .Y(n1338) );
  MXIT2_X0P5M_A12TR u2040 ( .A(n318), .B(n2044), .S0(n2053), .Y(n1337) );
  MXIT2_X0P5M_A12TR u2041 ( .A(n312), .B(n2045), .S0(n2053), .Y(n1336) );
  MXIT2_X0P5M_A12TR u2042 ( .A(n306), .B(n2046), .S0(n2053), .Y(n1335) );
  MXIT2_X0P5M_A12TR u2043 ( .A(n300), .B(n2047), .S0(n2053), .Y(n1334) );
  MXIT2_X0P5M_A12TR u2044 ( .A(n294), .B(n2048), .S0(n2053), .Y(n1333) );
  MXIT2_X0P5M_A12TR u2045 ( .A(n275), .B(n2049), .S0(n2053), .Y(n1332) );
  AND2_X0P5M_A12TR u2046 ( .A(n1806), .B(n15911), .Y(n2053) );
  MXIT2_X0P5M_A12TR u2047 ( .A(n270), .B(n2041), .S0(n2054), .Y(n1331) );
  MXIT2_X0P5M_A12TR u2048 ( .A(n289), .B(n2043), .S0(n2054), .Y(n1330) );
  MXIT2_X0P5M_A12TR u2049 ( .A(n319), .B(n2044), .S0(n2054), .Y(n1329) );
  MXIT2_X0P5M_A12TR u2050 ( .A(n313), .B(n2045), .S0(n2054), .Y(n1328) );
  MXIT2_X0P5M_A12TR u2051 ( .A(n307), .B(n2046), .S0(n2054), .Y(n1327) );
  MXIT2_X0P5M_A12TR u2052 ( .A(n301), .B(n2047), .S0(n2054), .Y(n1326) );
  MXIT2_X0P5M_A12TR u2053 ( .A(n295), .B(n2048), .S0(n2054), .Y(n1325) );
  MXIT2_X0P5M_A12TR u2054 ( .A(n276), .B(n2049), .S0(n2054), .Y(n1324) );
  NOR2B_X0P5M_A12TR u2055 ( .AN(n15891), .B(n2050), .Y(n2054) );
  MXIT2_X0P5M_A12TR u2056 ( .A(n271), .B(n2041), .S0(n2055), .Y(n1323) );
  INV_X0P5B_A12TR u2057 ( .A(n2056), .Y(n2041) );
  MXIT2_X0P5M_A12TR u2058 ( .A(n290), .B(n2043), .S0(n2055), .Y(n1322) );
  INV_X0P5B_A12TR u2059 ( .A(n2057), .Y(n2043) );
  MXIT2_X0P5M_A12TR u2060 ( .A(n320), .B(n2044), .S0(n2055), .Y(n1321) );
  INV_X0P5B_A12TR u2061 ( .A(n2058), .Y(n2044) );
  MXIT2_X0P5M_A12TR u2062 ( .A(n314), .B(n2045), .S0(n2055), .Y(n1320) );
  INV_X0P5B_A12TR u2063 ( .A(n2059), .Y(n2045) );
  MXIT2_X0P5M_A12TR u2064 ( .A(n308), .B(n2046), .S0(n2055), .Y(n1319) );
  INV_X0P5B_A12TR u2065 ( .A(n2060), .Y(n2046) );
  MXIT2_X0P5M_A12TR u2066 ( .A(n302), .B(n2047), .S0(n2055), .Y(n1318) );
  INV_X0P5B_A12TR u2067 ( .A(n2061), .Y(n2047) );
  MXIT2_X0P5M_A12TR u2068 ( .A(n296), .B(n2048), .S0(n2055), .Y(n1317) );
  INV_X0P5B_A12TR u2069 ( .A(n2062), .Y(n2048) );
  MXIT2_X0P5M_A12TR u2070 ( .A(n277), .B(n2049), .S0(n2055), .Y(n1316) );
  AND2_X0P5M_A12TR u2071 ( .A(n1806), .B(n15891), .Y(n2055) );
  NOR2B_X0P5M_A12TR u2072 ( .AN(n2063), .B(n15881), .Y(n1806) );
  OAI21_X0P5M_A12TR u2073 ( .A0(n1755), .A1(n1652), .B0(n2064), .Y(n1315) );
  MXIT2_X0P5M_A12TR u2074 ( .A(regfile[0]), .B(n2056), .S0(n2065), .Y(n2064)
         );
  OAI221_X0P5M_A12TR u2075 ( .A0(n1614), .A1(n2066), .B0(n1798), .B1(n1643), 
        .C0(n1756), .Y(n2056) );
  AOI221_X0P5M_A12TR u2076 ( .A0(flags[1]), .A1(n2067), .B0(n2800), .B1(n2036), 
        .C0(n2663), .Y(n1756) );
  OAI21_X0P5M_A12TR u2077 ( .A0(n2068), .A1(n1663), .B0(n2069), .Y(n2663) );
  INV_X0P5B_A12TR u2078 ( .A(n80), .Y(n1643) );
  INV_X0P5B_A12TR u2079 ( .A(mult[8]), .Y(n1652) );
  OAI21_X0P5M_A12TR u2080 ( .A0(n1755), .A1(n1653), .B0(n2070), .Y(n1314) );
  MXIT2_X0P5M_A12TR u2081 ( .A(regfile[1]), .B(n2057), .S0(n2065), .Y(n2070)
         );
  OAI221_X0P5M_A12TR u2082 ( .A0(n1616), .A1(n2066), .B0(n1798), .B1(n1642), 
        .C0(n1764), .Y(n2057) );
  AOI221_X0P5M_A12TR u2083 ( .A0(n2801), .A1(n2067), .B0(n2799), .B1(n2036), 
        .C0(n1633), .Y(n1764) );
  OAI22_X0P5M_A12TR u2084 ( .A0(n2068), .A1(n1666), .B0(n2069), .B1(n1865), 
        .Y(n1633) );
  INV_X0P5B_A12TR u2085 ( .A(n79), .Y(n1642) );
  INV_X0P5B_A12TR u2086 ( .A(mult[9]), .Y(n1653) );
  OAI21_X0P5M_A12TR u2087 ( .A0(n1755), .A1(n1655), .B0(n2071), .Y(n1313) );
  MXIT2_X0P5M_A12TR u2088 ( .A(regfile[2]), .B(n2058), .S0(n2065), .Y(n2071)
         );
  OAI221_X0P5M_A12TR u2089 ( .A0(n15961), .A1(n2066), .B0(n1798), .B1(n1641), 
        .C0(n1770), .Y(n2058) );
  AOI221_X0P5M_A12TR u2090 ( .A0(n2800), .A1(n2067), .B0(n2798), .B1(n2036), 
        .C0(n2665), .Y(n1770) );
  OAI22_X0P5M_A12TR u2091 ( .A0(n2068), .A1(n1668), .B0(n2069), .B1(n1866), 
        .Y(n2665) );
  INV_X0P5B_A12TR u2092 ( .A(n78), .Y(n1641) );
  INV_X0P5B_A12TR u2093 ( .A(mult[10]), .Y(n1655) );
  AO1B2_X0P5M_A12TR u2094 ( .B0(n1662), .B1(mult[11]), .A0N(n2072), .Y(n1312)
         );
  MXIT2_X0P5M_A12TR u2095 ( .A(regfile[3]), .B(n2059), .S0(n2065), .Y(n2072)
         );
  OAI221_X0P5M_A12TR u2096 ( .A0(n15901), .A1(n2066), .B0(n1798), .B1(n1640), 
        .C0(n1776), .Y(n2059) );
  AOI221_X0P5M_A12TR u2097 ( .A0(n2799), .A1(n2067), .B0(n2797), .B1(n2036), 
        .C0(n1632), .Y(n1776) );
  OAI22_X0P5M_A12TR u2098 ( .A0(n2068), .A1(n1670), .B0(n2069), .B1(n1867), 
        .Y(n1632) );
  INV_X0P5B_A12TR u2099 ( .A(n77), .Y(n1640) );
  AO1B2_X0P5M_A12TR u2100 ( .B0(n1662), .B1(mult[12]), .A0N(n2073), .Y(n1311)
         );
  MXIT2_X0P5M_A12TR u2101 ( .A(regfile[4]), .B(n2060), .S0(n2065), .Y(n2073)
         );
  OAI221_X0P5M_A12TR u2102 ( .A0(n15971), .A1(n2066), .B0(n1798), .B1(n1639), 
        .C0(n1783), .Y(n2060) );
  AOI221_X0P5M_A12TR u2103 ( .A0(n2798), .A1(n2067), .B0(n2796), .B1(n2036), 
        .C0(n1631), .Y(n1783) );
  OAI22_X0P5M_A12TR u2104 ( .A0(n2068), .A1(n1672), .B0(n2069), .B1(n1868), 
        .Y(n1631) );
  INV_X0P5B_A12TR u2105 ( .A(n76), .Y(n1639) );
  INV_X0P5B_A12TR u2106 ( .A(n1755), .Y(n1662) );
  OAI21_X0P5M_A12TR u2107 ( .A0(n1755), .A1(n1647), .B0(n2074), .Y(n1310) );
  MXIT2_X0P5M_A12TR u2108 ( .A(regfile[5]), .B(n2061), .S0(n2065), .Y(n2074)
         );
  OAI221_X0P5M_A12TR u2109 ( .A0(n15921), .A1(n2066), .B0(n1798), .B1(n1638), 
        .C0(n1790), .Y(n2061) );
  AOI221_X0P5M_A12TR u2110 ( .A0(n2797), .A1(n2067), .B0(n2795), .B1(n2036), 
        .C0(n1630), .Y(n1790) );
  OAI22_X0P5M_A12TR u2111 ( .A0(n2068), .A1(n1674), .B0(n2069), .B1(n1869), 
        .Y(n1630) );
  INV_X0P5B_A12TR u2112 ( .A(n75), .Y(n1638) );
  INV_X0P5B_A12TR u2113 ( .A(mult[13]), .Y(n1647) );
  OAI21_X0P5M_A12TR u2114 ( .A0(n1755), .A1(n1648), .B0(n2075), .Y(n1309) );
  MXIT2_X0P5M_A12TR u2115 ( .A(regfile[6]), .B(n2062), .S0(n2065), .Y(n2075)
         );
  OAI221_X0P5M_A12TR u2116 ( .A0(n1620), .A1(n2066), .B0(n1798), .B1(n1637), 
        .C0(n1796), .Y(n2062) );
  AOI221_X0P5M_A12TR u2117 ( .A0(n2796), .A1(n2067), .B0(n2794), .B1(n2036), 
        .C0(n1629), .Y(n1796) );
  OAI22_X0P5M_A12TR u2118 ( .A0(n2068), .A1(n1676), .B0(n2069), .B1(n1808), 
        .Y(n1629) );
  INV_X0P5B_A12TR u2119 ( .A(n74), .Y(n1637) );
  INV_X0P5B_A12TR u2120 ( .A(mult[14]), .Y(n1648) );
  OAI21_X0P5M_A12TR u2121 ( .A0(n1755), .A1(n1649), .B0(n2076), .Y(n1308) );
  MXT2_X0P5M_A12TR u2122 ( .A(n2077), .B(n2049), .S0(n2065), .Y(n2076) );
  OAI21_X0P5M_A12TR u2123 ( .A0(n15941), .A1(n2050), .B0(n1755), .Y(n2065) );
  NAND2_X0P5A_A12TR u2124 ( .A(n2063), .B(n15881), .Y(n2050) );
  NAND3_X0P5A_A12TR u2125 ( .A(n1798), .B(n2078), .C(n2079), .Y(n2063) );
  NOR3_X0P5A_A12TR u2126 ( .A(n1802), .B(n2036), .C(n2067), .Y(n2079) );
  INV_X0P5B_A12TR u2127 ( .A(n2031), .Y(n2067) );
  INV_X0P5B_A12TR u2128 ( .A(n2032), .Y(n2036) );
  OAI211_X0P5M_A12TR u2129 ( .A0(n2004), .A1(alu_ctrl[0]), .B0(alu_ctrl[1]), 
        .C0(n2033), .Y(n2078) );
  INV_X0P5B_A12TR u2130 ( .A(n2029), .Y(n1798) );
  AOI221_X0P5M_A12TR u2131 ( .A0(regfile[15]), .A1(n1802), .B0(n2029), .B1(n73), .C0(n1745), .Y(n2049) );
  OAI221_X0P5M_A12TR u2132 ( .A0(n1619), .A1(n2031), .B0(n2027), .B1(n2032), 
        .C0(n15841), .Y(n1745) );
  OAI22_X0P5M_A12TR u2133 ( .A0(n2068), .A1(n1678), .B0(n2069), .B1(n1870), 
        .Y(n2664) );
  INV_X0P5B_A12TR u2134 ( .A(n2080), .Y(n2068) );
  NAND2_X0P5A_A12TR u2135 ( .A(n2000), .B(n2002), .Y(n2032) );
  NAND2_X0P5A_A12TR u2136 ( .A(n1999), .B(n2002), .Y(n2031) );
  NAND2_X0P5A_A12TR u2137 ( .A(n2081), .B(n2008), .Y(n2029) );
  INV_X0P5B_A12TR u2138 ( .A(n2066), .Y(n1802) );
  NAND2_X0P5A_A12TR u2139 ( .A(n2004), .B(n1999), .Y(n2066) );
  INV_X0P5B_A12TR u2140 ( .A(mult[15]), .Y(n1649) );
  NAND2_X0P5A_A12TR u2141 ( .A(n2005), .B(n2002), .Y(n1755) );
  NOR2_X0P5A_A12TR u2142 ( .A(n2082), .B(n2661), .Y(n2002) );
  OAI21_X0P5M_A12TR u2143 ( .A0(n16901), .A1(n2083), .B0(n2084), .Y(n1307) );
  AOI22_X0P5M_A12TR u2144 ( .A0(program_ctr[15]), .A1(n2085), .B0(n15990), 
        .B1(n2086), .Y(n2084) );
  INV_X0P5B_A12TR u2145 ( .A(operand2[7]), .Y(n16901) );
  OAI21_X0P5M_A12TR u2146 ( .A0(n1663), .A1(n2083), .B0(n2087), .Y(n1306) );
  AOI22_X0P5M_A12TR u2147 ( .A0(program_ctr[0]), .A1(n2085), .B0(n15840), .B1(
        n2086), .Y(n2087) );
  OAI21_X0P5M_A12TR u2148 ( .A0(n1666), .A1(n2083), .B0(n2088), .Y(n1305) );
  AOI22_X0P5M_A12TR u2149 ( .A0(program_ctr[1]), .A1(n2085), .B0(n15850), .B1(
        n2086), .Y(n2088) );
  OAI21_X0P5M_A12TR u2150 ( .A0(n1668), .A1(n2083), .B0(n2089), .Y(n1304) );
  AOI22_X0P5M_A12TR u2151 ( .A0(program_ctr[2]), .A1(n2085), .B0(n15860), .B1(
        n2086), .Y(n2089) );
  OAI21_X0P5M_A12TR u2152 ( .A0(n1670), .A1(n2083), .B0(n2090), .Y(n1303) );
  AOI22_X0P5M_A12TR u2153 ( .A0(program_ctr[3]), .A1(n2085), .B0(n15870), .B1(
        n2086), .Y(n2090) );
  OAI21_X0P5M_A12TR u2154 ( .A0(n1672), .A1(n2083), .B0(n2091), .Y(n1302) );
  AOI22_X0P5M_A12TR u2155 ( .A0(program_ctr[4]), .A1(n2085), .B0(n15880), .B1(
        n2086), .Y(n2091) );
  OAI21_X0P5M_A12TR u2156 ( .A0(n1674), .A1(n2083), .B0(n2092), .Y(n1301) );
  AOI22_X0P5M_A12TR u2157 ( .A0(program_ctr[5]), .A1(n2085), .B0(n15890), .B1(
        n2086), .Y(n2092) );
  OAI21_X0P5M_A12TR u2158 ( .A0(n1676), .A1(n2083), .B0(n2093), .Y(n1300) );
  AOI22_X0P5M_A12TR u2159 ( .A0(program_ctr[6]), .A1(n2085), .B0(n15900), .B1(
        n2086), .Y(n2093) );
  OAI21_X0P5M_A12TR u2160 ( .A0(n1678), .A1(n2083), .B0(n2094), .Y(n1299) );
  AOI22_X0P5M_A12TR u2161 ( .A0(program_ctr[7]), .A1(n2085), .B0(n15910), .B1(
        n2086), .Y(n2094) );
  OAI21_X0P5M_A12TR u2162 ( .A0(n1682), .A1(n2083), .B0(n2095), .Y(n1298) );
  AOI22_X0P5M_A12TR u2163 ( .A0(program_ctr[8]), .A1(n2085), .B0(n15920), .B1(
        n2086), .Y(n2095) );
  INV_X0P5B_A12TR u2164 ( .A(operand2[0]), .Y(n1682) );
  OAI21_X0P5M_A12TR u2165 ( .A0(n1684), .A1(n2083), .B0(n2096), .Y(n1297) );
  AOI22_X0P5M_A12TR u2166 ( .A0(program_ctr[9]), .A1(n2085), .B0(n15930), .B1(
        n2086), .Y(n2096) );
  INV_X0P5B_A12TR u2167 ( .A(operand2[1]), .Y(n1684) );
  OAI21_X0P5M_A12TR u2168 ( .A0(n1685), .A1(n2083), .B0(n2097), .Y(n1296) );
  AOI22_X0P5M_A12TR u2169 ( .A0(program_ctr[10]), .A1(n2085), .B0(n15940), 
        .B1(n2086), .Y(n2097) );
  INV_X0P5B_A12TR u2170 ( .A(operand2[2]), .Y(n1685) );
  OAI21_X0P5M_A12TR u2171 ( .A0(n1686), .A1(n2083), .B0(n2098), .Y(n1295) );
  AOI22_X0P5M_A12TR u2172 ( .A0(program_ctr[11]), .A1(n2085), .B0(n15950), 
        .B1(n2086), .Y(n2098) );
  INV_X0P5B_A12TR u2173 ( .A(operand2[3]), .Y(n1686) );
  OAI21_X0P5M_A12TR u2174 ( .A0(n16871), .A1(n2083), .B0(n2099), .Y(n1294) );
  AOI22_X0P5M_A12TR u2175 ( .A0(program_ctr[12]), .A1(n2085), .B0(n15960), 
        .B1(n2086), .Y(n2099) );
  INV_X0P5B_A12TR u2176 ( .A(operand2[4]), .Y(n16871) );
  OAI21_X0P5M_A12TR u2177 ( .A0(n16881), .A1(n2083), .B0(n2100), .Y(n1293) );
  AOI22_X0P5M_A12TR u2178 ( .A0(program_ctr[13]), .A1(n2085), .B0(n15970), 
        .B1(n2086), .Y(n2100) );
  INV_X0P5B_A12TR u2179 ( .A(operand2[5]), .Y(n16881) );
  OAI21_X0P5M_A12TR u2180 ( .A0(n16891), .A1(n2083), .B0(n2101), .Y(n1292) );
  AOI22_X0P5M_A12TR u2181 ( .A0(program_ctr[14]), .A1(n2085), .B0(n15980), 
        .B1(n2086), .Y(n2101) );
  NOR2B_X0P5M_A12TR u2182 ( .AN(n2083), .B(n2086), .Y(n2085) );
  NAND2_X0P5A_A12TR u2183 ( .A(n2656), .B(n2655), .Y(n2086) );
  NOR2B_X0P5M_A12TR u2184 ( .AN(n2102), .B(n2103), .Y(n2656) );
  NAND2_X0P5A_A12TR u2185 ( .A(n2104), .B(n2105), .Y(n2083) );
  INV_X0P5B_A12TR u2186 ( .A(operand2[6]), .Y(n16891) );
  OAI22_X0P5M_A12TR u2187 ( .A0(n2106), .A1(n2007), .B0(n1800), .B1(n2027), 
        .Y(u4_u8_z_0) );
  INV_X0P5B_A12TR u2188 ( .A(flags[1]), .Y(n2027) );
  AND2_X0P5M_A12TR u2189 ( .A(n2107), .B(n2108), .Y(n1800) );
  NAND2_X0P5A_A12TR u2190 ( .A(n2109), .B(n2009), .Y(u4_u7_z_7) );
  MXIT2_X0P5M_A12TR u2191 ( .A(n2110), .B(n2111), .S0(n1749), .Y(n2109) );
  NAND2_X0P5A_A12TR u2192 ( .A(n2112), .B(n2009), .Y(u4_u7_z_6) );
  MXIT2_X0P5M_A12TR u2193 ( .A(n2110), .B(n2111), .S0(n1619), .Y(n2112) );
  NAND2_X0P5A_A12TR u2194 ( .A(n2113), .B(n2009), .Y(u4_u7_z_5) );
  MXIT2_X0P5M_A12TR u2195 ( .A(n2110), .B(n2111), .S0(n1786), .Y(n2113) );
  NAND2_X0P5A_A12TR u2196 ( .A(n2114), .B(n2009), .Y(u4_u7_z_4) );
  MXIT2_X0P5M_A12TR u2197 ( .A(n2110), .B(n2111), .S0(n1779), .Y(n2114) );
  NAND2_X0P5A_A12TR u2198 ( .A(n2115), .B(n2009), .Y(u4_u7_z_3) );
  MXIT2_X0P5M_A12TR u2199 ( .A(n2110), .B(n2111), .S0(n1618), .Y(n2115) );
  NAND2_X0P5A_A12TR u2200 ( .A(n2116), .B(n2009), .Y(u4_u7_z_2) );
  MXIT2_X0P5M_A12TR u2201 ( .A(n2110), .B(n2111), .S0(n1617), .Y(n2116) );
  NAND2_X0P5A_A12TR u2202 ( .A(n2117), .B(n2009), .Y(u4_u7_z_1) );
  MXIT2_X0P5M_A12TR u2203 ( .A(n2110), .B(n2111), .S0(n1615), .Y(n2117) );
  NAND2_X0P5A_A12TR u2204 ( .A(n2118), .B(n2119), .Y(u4_u7_z_0) );
  MXIT2_X0P5M_A12TR u2205 ( .A(n2110), .B(n2111), .S0(n1613), .Y(n2119) );
  NAND3_X0P5A_A12TR u2206 ( .A(n1799), .B(n2108), .C(n2008), .Y(n2110) );
  AOI31_X0P5M_A12TR u2207 ( .A0(n1998), .A1(n2005), .A2(flags[1]), .B0(n2120), 
        .Y(n2118) );
  OAI22_X0P5M_A12TR u2208 ( .A0(n2081), .A1(n1749), .B0(n2030), .B1(n1625), 
        .Y(u4_u6_z_7) );
  OAI22_X0P5M_A12TR u2209 ( .A0(n2081), .A1(n1619), .B0(n2030), .B1(n1620), 
        .Y(u4_u6_z_6) );
  OAI22_X0P5M_A12TR u2210 ( .A0(n2081), .A1(n1786), .B0(n2030), .B1(n15921), 
        .Y(u4_u6_z_5) );
  OAI22_X0P5M_A12TR u2211 ( .A0(n2081), .A1(n1779), .B0(n2030), .B1(n15971), 
        .Y(u4_u6_z_4) );
  OAI22_X0P5M_A12TR u2212 ( .A0(n2081), .A1(n1618), .B0(n2030), .B1(n15901), 
        .Y(u4_u6_z_3) );
  OAI22_X0P5M_A12TR u2213 ( .A0(n2081), .A1(n1617), .B0(n2030), .B1(n15961), 
        .Y(u4_u6_z_2) );
  OAI22_X0P5M_A12TR u2214 ( .A0(n2081), .A1(n1615), .B0(n2030), .B1(n1616), 
        .Y(u4_u6_z_1) );
  OAI221_X0P5M_A12TR u2215 ( .A0(n2030), .A1(n1614), .B0(n2081), .B1(n1613), 
        .C0(n2008), .Y(u4_u6_z_0) );
  NAND4_X0P5A_A12TR u2216 ( .A(n2121), .B(n2082), .C(n2033), .D(n2035), .Y(
        n2008) );
  XNOR2_X0P5M_A12TR u2217 ( .A(n2122), .B(n2034), .Y(n2121) );
  AOI21_X0P5M_A12TR u2218 ( .A0(n2005), .A1(n1998), .B0(n2120), .Y(n2081) );
  INV_X0P5B_A12TR u2219 ( .A(n2009), .Y(n2120) );
  NAND2_X0P5A_A12TR u2220 ( .A(n2039), .B(n2001), .Y(n2009) );
  NOR3_X0P5A_A12TR u2221 ( .A(alu_ctrl[0]), .B(alu_ctrl[1]), .C(n2660), .Y(
        n2001) );
  NOR3_X0P5A_A12TR u2222 ( .A(n2034), .B(alu_ctrl[0]), .C(n2033), .Y(n2005) );
  INV_X0P5B_A12TR u2223 ( .A(alu_ctrl[1]), .Y(n2034) );
  INV_X0P5B_A12TR u2224 ( .A(n2040), .Y(n2030) );
  NAND3B_X0P5M_A12TR u2225 ( .AN(n2111), .B(n1799), .C(n2108), .Y(n2040) );
  NAND2_X0P5A_A12TR u2226 ( .A(n2003), .B(n1998), .Y(n2108) );
  NOR2_X0P5A_A12TR u2227 ( .A(n2661), .B(n2662), .Y(n1998) );
  INV_X0P5B_A12TR u2228 ( .A(n2122), .Y(n2661) );
  NAND2_X0P5A_A12TR u2229 ( .A(n1999), .B(n2039), .Y(n1799) );
  NOR3_X0P5A_A12TR u2230 ( .A(alu_ctrl[0]), .B(alu_ctrl[1]), .C(n2033), .Y(
        n1999) );
  OAI21_X0P5M_A12TR u2231 ( .A0(n2106), .A1(n2007), .B0(n2107), .Y(n2111) );
  NAND2_X0P5A_A12TR u2232 ( .A(n2003), .B(n2039), .Y(n2107) );
  NOR2_X0P5A_A12TR u2233 ( .A(n2122), .B(n2662), .Y(n2039) );
  INV_X0P5B_A12TR u2234 ( .A(n2082), .Y(n2662) );
  NOR3_X0P5A_A12TR u2235 ( .A(n2660), .B(alu_ctrl[1]), .C(n2035), .Y(n2003) );
  INV_X0P5B_A12TR u2236 ( .A(n2033), .Y(n2660) );
  INV_X0P5B_A12TR u2237 ( .A(n2004), .Y(n2007) );
  NOR2_X0P5A_A12TR u2238 ( .A(n2122), .B(n2082), .Y(n2004) );
  OAI21_X0P5M_A12TR u2239 ( .A0(n2123), .A1(n2124), .B0(n1718), .Y(n2082) );
  OAI22_X0P5M_A12TR u2240 ( .A0(n1704), .A1(n2125), .B0(n2126), .B1(n2127), 
        .Y(n2124) );
  OAI31_X0P5M_A12TR u2241 ( .A0(n2128), .A1(n1726), .A2(n2658), .B0(n1718), 
        .Y(n2122) );
  NOR2_X0P5A_A12TR u2242 ( .A(n2129), .B(n2130), .Y(n1726) );
  OAI21B_X0P5M_A12TR u2243 ( .A0(n1705), .A1(n2125), .B0N(n2123), .Y(n2128) );
  OAI211_X0P5M_A12TR u2244 ( .A0(n2131), .A1(n2132), .B0(n2069), .C0(n1738), 
        .Y(n2123) );
  NAND2_X0P5A_A12TR u2245 ( .A(n1723), .B(n1858), .Y(n2069) );
  INV_X0P5B_A12TR u2246 ( .A(n2000), .Y(n2106) );
  NOR3_X0P5A_A12TR u2247 ( .A(n2033), .B(alu_ctrl[1]), .C(n2035), .Y(n2000) );
  NAND2_X0P5A_A12TR u2248 ( .A(n1718), .B(n2133), .Y(n2033) );
  OAI211_X0P5M_A12TR u2249 ( .A0(n1703), .A1(n2125), .B0(n2134), .C0(n2135), 
        .Y(n2133) );
  AND3_X0P5M_A12TR u2250 ( .A(n2136), .B(n2137), .C(n2138), .Y(n2135) );
  INV_X0P5B_A12TR u2251 ( .A(n1889), .Y(n1871) );
  NAND2_X0P5A_A12TR u2252 ( .A(n1718), .B(n2139), .Y(n1889) );
  OAI211_X0P5M_A12TR u2253 ( .A0(n2140), .A1(n2129), .B0(n2141), .C0(n21421), 
        .Y(n2139) );
  AOI21_X0P5M_A12TR u2254 ( .A0(n2143), .A1(n1862), .B0(n2144), .Y(n21421) );
  OA21A1OI2_X0P5M_A12TR u2255 ( .A0(n2145), .A1(n2146), .B0(n2147), .C0(n16991), .Y(n1890) );
  AO22_X0P5M_A12TR u2256 ( .A0(n15770), .A1(n2103), .B0(program_ctr[9]), .B1(
        n2148), .Y(u4_u2_z_9) );
  AO22_X0P5M_A12TR u2257 ( .A0(n15760), .A1(n2103), .B0(program_ctr[8]), .B1(
        n2148), .Y(u4_u2_z_8) );
  AO22_X0P5M_A12TR u2258 ( .A0(n15750), .A1(n2103), .B0(program_ctr[7]), .B1(
        n2148), .Y(u4_u2_z_7) );
  AO22_X0P5M_A12TR u2259 ( .A0(n15740), .A1(n2103), .B0(program_ctr[6]), .B1(
        n2148), .Y(u4_u2_z_6) );
  AO22_X0P5M_A12TR u2260 ( .A0(n15730), .A1(n2103), .B0(program_ctr[5]), .B1(
        n2148), .Y(u4_u2_z_5) );
  AO22_X0P5M_A12TR u2261 ( .A0(n15720), .A1(n2103), .B0(program_ctr[4]), .B1(
        n2148), .Y(u4_u2_z_4) );
  AO22_X0P5M_A12TR u2262 ( .A0(n15710), .A1(n2103), .B0(program_ctr[3]), .B1(
        n2148), .Y(u4_u2_z_3) );
  AO22_X0P5M_A12TR u2263 ( .A0(n15700), .A1(n2103), .B0(program_ctr[2]), .B1(
        n2148), .Y(u4_u2_z_2) );
  AO22_X0P5M_A12TR u2264 ( .A0(n15830), .A1(n2103), .B0(program_ctr[15]), .B1(
        n2148), .Y(u4_u2_z_15) );
  AO22_X0P5M_A12TR u2265 ( .A0(n15820), .A1(n2103), .B0(program_ctr[14]), .B1(
        n2148), .Y(u4_u2_z_14) );
  AO22_X0P5M_A12TR u2266 ( .A0(n15810), .A1(n2103), .B0(program_ctr[13]), .B1(
        n2148), .Y(u4_u2_z_13) );
  AO22_X0P5M_A12TR u2267 ( .A0(n15800), .A1(n2103), .B0(program_ctr[12]), .B1(
        n2148), .Y(u4_u2_z_12) );
  AO22_X0P5M_A12TR u2268 ( .A0(n15790), .A1(n2103), .B0(program_ctr[11]), .B1(
        n2148), .Y(u4_u2_z_11) );
  AO22_X0P5M_A12TR u2269 ( .A0(n15780), .A1(n2103), .B0(program_ctr[10]), .B1(
        n2148), .Y(u4_u2_z_10) );
  AO22_X0P5M_A12TR u2270 ( .A0(n15690), .A1(n2103), .B0(program_ctr[1]), .B1(
        n2148), .Y(u4_u2_z_1) );
  AO22_X0P5M_A12TR u2271 ( .A0(n15680), .A1(n2103), .B0(program_ctr[0]), .B1(
        n2148), .Y(u4_u2_z_0) );
  NAND2_X0P5A_A12TR u2272 ( .A(n2655), .B(n2102), .Y(n2148) );
  NAND2_X0P5A_A12TR u2273 ( .A(n2104), .B(n2149), .Y(n2102) );
  NAND2_X0P5A_A12TR u2274 ( .A(n2150), .B(n2151), .Y(n2655) );
  NOR2_X0P5A_A12TR u2275 ( .A(n2104), .B(n2150), .Y(n2103) );
  INV_X0P5B_A12TR u2276 ( .A(n2149), .Y(n2150) );
  NAND3_X0P5A_A12TR u2277 ( .A(n2152), .B(n1718), .C(n2153), .Y(n2149) );
  OAI31_X0P5M_A12TR u2278 ( .A0(n2154), .A1(n2155), .A2(n21561), .B0(n21571), 
        .Y(n2152) );
  INV_X0P5B_A12TR u2279 ( .A(n2151), .Y(n2104) );
  NAND3_X0P5A_A12TR u2280 ( .A(n21581), .B(n1718), .C(n2153), .Y(n2151) );
  OAI31_X0P5M_A12TR u2281 ( .A0(n21591), .A1(n21601), .A2(n21611), .B0(n21571), 
        .Y(n21581) );
  NAND3_X0P5A_A12TR u2282 ( .A(n21621), .B(n21631), .C(n21641), .Y(n21591) );
  NOR2_X0P5A_A12TR u2283 ( .A(n21651), .B(n21661), .Y(u4_u1_z_9) );
  NOR2_X0P5A_A12TR u2284 ( .A(n21651), .B(n21671), .Y(u4_u1_z_8) );
  AND2_X0P5M_A12TR u2285 ( .A(n21681), .B(n21691), .Y(u4_u1_z_7) );
  AND2_X0P5M_A12TR u2286 ( .A(n21701), .B(n21691), .Y(u4_u1_z_6) );
  AND2_X0P5M_A12TR u2287 ( .A(n21711), .B(n21691), .Y(u4_u1_z_5) );
  AND2_X0P5M_A12TR u2288 ( .A(n2172), .B(n21691), .Y(u4_u1_z_4) );
  AND2_X0P5M_A12TR u2289 ( .A(n2173), .B(n21691), .Y(u4_u1_z_3) );
  AND2_X0P5M_A12TR u2290 ( .A(n2174), .B(n21691), .Y(u4_u1_z_2) );
  NOR2_X0P5A_A12TR u2291 ( .A(n21651), .B(n2175), .Y(u4_u1_z_15) );
  NOR2_X0P5A_A12TR u2292 ( .A(n21651), .B(n2176), .Y(u4_u1_z_14) );
  NOR2_X0P5A_A12TR u2293 ( .A(n21651), .B(n2177), .Y(u4_u1_z_13) );
  NOR2_X0P5A_A12TR u2294 ( .A(n21651), .B(n2178), .Y(u4_u1_z_12) );
  NOR2_X0P5A_A12TR u2295 ( .A(n21651), .B(n2179), .Y(u4_u1_z_11) );
  NOR2_X0P5A_A12TR u2296 ( .A(n21651), .B(n2180), .Y(u4_u1_z_10) );
  AND2_X0P5M_A12TR u2297 ( .A(n2181), .B(n21691), .Y(u4_u1_z_1) );
  AND2_X0P5M_A12TR u2298 ( .A(n2182), .B(n21691), .Y(u4_u1_z_0) );
  INV_X0P5B_A12TR u2299 ( .A(offset_sx_15), .Y(n1678) );
  OAI21_X0P5M_A12TR u2300 ( .A0(n2183), .A1(n1676), .B0(n1718), .Y(pc_ctrl[6])
         );
  INV_X0P5B_A12TR u2301 ( .A(offset_sx[6]), .Y(n1676) );
  OAI21_X0P5M_A12TR u2302 ( .A0(n2183), .A1(n1674), .B0(n1718), .Y(pc_ctrl[5])
         );
  INV_X0P5B_A12TR u2303 ( .A(offset_sx[5]), .Y(n1674) );
  OAI21_X0P5M_A12TR u2304 ( .A0(n2183), .A1(n1672), .B0(n1718), .Y(pc_ctrl[4])
         );
  INV_X0P5B_A12TR u2305 ( .A(offset_sx[4]), .Y(n1672) );
  OAI21_X0P5M_A12TR u2306 ( .A0(n2183), .A1(n1670), .B0(n1718), .Y(pc_ctrl[3])
         );
  INV_X0P5B_A12TR u2307 ( .A(offset_sx[3]), .Y(n1670) );
  OAI21_X0P5M_A12TR u2308 ( .A0(n2183), .A1(n1668), .B0(n1718), .Y(pc_ctrl[2])
         );
  INV_X0P5B_A12TR u2309 ( .A(offset_sx[2]), .Y(n1668) );
  OA22_X0P5M_A12TR u2310 ( .A0(flags[0]), .A1(n2184), .B0(n2185), .B1(n2186), 
        .Y(n2183) );
  NAND2_X0P5A_A12TR u2311 ( .A(n2187), .B(n1666), .Y(pc_ctrl[1]) );
  INV_X0P5B_A12TR u2312 ( .A(offset_sx[1]), .Y(n1666) );
  NAND2_X0P5A_A12TR u2313 ( .A(n2187), .B(n1663), .Y(pc_ctrl[0]) );
  INV_X0P5B_A12TR u2314 ( .A(offset_sx[0]), .Y(n1663) );
  INV_X0P5B_A12TR u2315 ( .A(n2188), .Y(n2187) );
  NAND4B_X0P5M_A12TR u2316 ( .AN(n2189), .B(n2190), .C(n1736), .D(n2191), .Y(
        n2188) );
  NOR2_X0P5A_A12TR u2317 ( .A(n2192), .B(n2193), .Y(n2191) );
  AND4_X0P5M_A12TR u2318 ( .A(n21651), .B(n2126), .C(n2194), .D(n2195), .Y(
        n2190) );
  OR3_X0P5M_A12TR u2319 ( .A(n2196), .B(n2197), .C(n2198), .Y(n2979) );
  AND2_X0P5M_A12TR u2320 ( .A(n2199), .B(n2200), .Y(n2978) );
  OAI211_X0P5M_A12TR u2321 ( .A0(n2201), .A1(n2202), .B0(n2203), .C0(n2204), 
        .Y(n2977) );
  AOI22_X0P5M_A12TR u2322 ( .A0(n2205), .A1(program_ctr[15]), .B0(n2196), .B1(
        n2206), .Y(n2204) );
  OAI31_X0P5M_A12TR u2323 ( .A0(n2207), .A1(n2208), .A2(n2209), .B0(n2198), 
        .Y(n2203) );
  OAI22_X0P5M_A12TR u2324 ( .A0(n273), .A1(n2210), .B0(n272), .B1(n2211), .Y(
        n2209) );
  OAI22_X0P5M_A12TR u2325 ( .A0(n275), .A1(n2212), .B0(n274), .B1(n2213), .Y(
        n2208) );
  OAI221_X0P5M_A12TR u2326 ( .A0(n1625), .A1(n2214), .B0(n2077), .B1(n2215), 
        .C0(n2216), .Y(n2207) );
  AOI22_X0P5M_A12TR u2327 ( .A0(n2217), .A1(n2218), .B0(n2219), .B1(n2220), 
        .Y(n2216) );
  OAI211_X0P5M_A12TR u2328 ( .A0(n2221), .A1(n2202), .B0(n2222), .C0(n2223), 
        .Y(n2976) );
  AOI22_X0P5M_A12TR u2329 ( .A0(n2205), .A1(program_ctr[14]), .B0(n2196), .B1(
        n2224), .Y(n2223) );
  OAI31_X0P5M_A12TR u2330 ( .A0(n2225), .A1(n2226), .A2(n2227), .B0(n2198), 
        .Y(n2222) );
  OAI22_X0P5M_A12TR u2331 ( .A0(n292), .A1(n2210), .B0(n291), .B1(n2211), .Y(
        n2227) );
  OAI22_X0P5M_A12TR u2332 ( .A0(n294), .A1(n2212), .B0(n293), .B1(n2213), .Y(
        n2226) );
  OAI221_X0P5M_A12TR u2333 ( .A0(n1620), .A1(n2214), .B0(n2228), .B1(n2215), 
        .C0(n2229), .Y(n2225) );
  AOI22_X0P5M_A12TR u2334 ( .A0(n2217), .A1(n2230), .B0(n2219), .B1(n2231), 
        .Y(n2229) );
  OAI211_X0P5M_A12TR u2335 ( .A0(n2232), .A1(n2202), .B0(n2233), .C0(n2234), 
        .Y(n2975) );
  AOI22_X0P5M_A12TR u2336 ( .A0(n2205), .A1(program_ctr[13]), .B0(n2196), .B1(
        n2235), .Y(n2234) );
  OAI31_X0P5M_A12TR u2337 ( .A0(n2236), .A1(n2237), .A2(n2238), .B0(n2198), 
        .Y(n2233) );
  OAI22_X0P5M_A12TR u2338 ( .A0(n298), .A1(n2210), .B0(n297), .B1(n2211), .Y(
        n2238) );
  OAI22_X0P5M_A12TR u2339 ( .A0(n300), .A1(n2212), .B0(n299), .B1(n2213), .Y(
        n2237) );
  OAI221_X0P5M_A12TR u2340 ( .A0(n15921), .A1(n2214), .B0(n2239), .B1(n2215), 
        .C0(n2240), .Y(n2236) );
  AOI22_X0P5M_A12TR u2341 ( .A0(n2217), .A1(n2241), .B0(n2219), .B1(n2242), 
        .Y(n2240) );
  OAI211_X0P5M_A12TR u2342 ( .A0(n2243), .A1(n2202), .B0(n2244), .C0(n2245), 
        .Y(n2974) );
  AOI22_X0P5M_A12TR u2343 ( .A0(n2205), .A1(program_ctr[12]), .B0(n2196), .B1(
        n2246), .Y(n2245) );
  OAI31_X0P5M_A12TR u2344 ( .A0(n2247), .A1(n2248), .A2(n2249), .B0(n2198), 
        .Y(n2244) );
  OAI22_X0P5M_A12TR u2345 ( .A0(n304), .A1(n2210), .B0(n303), .B1(n2211), .Y(
        n2249) );
  OAI22_X0P5M_A12TR u2346 ( .A0(n306), .A1(n2212), .B0(n305), .B1(n2213), .Y(
        n2248) );
  OAI221_X0P5M_A12TR u2347 ( .A0(n15971), .A1(n2214), .B0(n2250), .B1(n2215), 
        .C0(n2251), .Y(n2247) );
  AOI22_X0P5M_A12TR u2348 ( .A0(n2217), .A1(n2252), .B0(n2219), .B1(n2253), 
        .Y(n2251) );
  OAI211_X0P5M_A12TR u2349 ( .A0(n2254), .A1(n2202), .B0(n2255), .C0(n2256), 
        .Y(n2973) );
  AOI22_X0P5M_A12TR u2350 ( .A0(n2205), .A1(program_ctr[11]), .B0(n2196), .B1(
        n2257), .Y(n2256) );
  OAI31_X0P5M_A12TR u2351 ( .A0(n2258), .A1(n2259), .A2(n2260), .B0(n2198), 
        .Y(n2255) );
  OAI22_X0P5M_A12TR u2352 ( .A0(n310), .A1(n2210), .B0(n309), .B1(n2211), .Y(
        n2260) );
  OAI22_X0P5M_A12TR u2353 ( .A0(n312), .A1(n2212), .B0(n311), .B1(n2213), .Y(
        n2259) );
  OAI221_X0P5M_A12TR u2354 ( .A0(n15901), .A1(n2214), .B0(n2261), .B1(n2215), 
        .C0(n2262), .Y(n2258) );
  AOI22_X0P5M_A12TR u2355 ( .A0(n2217), .A1(n2263), .B0(n2219), .B1(n2264), 
        .Y(n2262) );
  OAI211_X0P5M_A12TR u2356 ( .A0(n2265), .A1(n2202), .B0(n2266), .C0(n2267), 
        .Y(n2972) );
  AOI22_X0P5M_A12TR u2357 ( .A0(n2205), .A1(program_ctr[10]), .B0(n2196), .B1(
        flags[2]), .Y(n2267) );
  OAI31_X0P5M_A12TR u2358 ( .A0(n2268), .A1(n2269), .A2(n2270), .B0(n2198), 
        .Y(n2266) );
  OAI22_X0P5M_A12TR u2359 ( .A0(n316), .A1(n2210), .B0(n315), .B1(n2211), .Y(
        n2270) );
  OAI22_X0P5M_A12TR u2360 ( .A0(n318), .A1(n2212), .B0(n317), .B1(n2213), .Y(
        n2269) );
  OAI221_X0P5M_A12TR u2361 ( .A0(n15961), .A1(n2214), .B0(n2271), .B1(n2215), 
        .C0(n2272), .Y(n2268) );
  AOI22_X0P5M_A12TR u2362 ( .A0(n2217), .A1(n2273), .B0(n2219), .B1(n2274), 
        .Y(n2272) );
  OAI211_X0P5M_A12TR u2363 ( .A0(n2275), .A1(n2202), .B0(n2276), .C0(n2277), 
        .Y(n2971) );
  AOI22_X0P5M_A12TR u2364 ( .A0(n2205), .A1(program_ctr[9]), .B0(n2196), .B1(
        flags[1]), .Y(n2277) );
  OAI31_X0P5M_A12TR u2365 ( .A0(n2278), .A1(n2279), .A2(n2280), .B0(n2198), 
        .Y(n2276) );
  OAI22_X0P5M_A12TR u2366 ( .A0(n286), .A1(n2210), .B0(n285), .B1(n2211), .Y(
        n2280) );
  OAI22_X0P5M_A12TR u2367 ( .A0(n288), .A1(n2212), .B0(n287), .B1(n2213), .Y(
        n2279) );
  OAI221_X0P5M_A12TR u2368 ( .A0(n1616), .A1(n2214), .B0(n2281), .B1(n2215), 
        .C0(n2282), .Y(n2278) );
  AOI22_X0P5M_A12TR u2369 ( .A0(n2217), .A1(n2283), .B0(n2219), .B1(n2284), 
        .Y(n2282) );
  OAI211_X0P5M_A12TR u2370 ( .A0(n2285), .A1(n2202), .B0(n2286), .C0(n2287), 
        .Y(n2970) );
  AOI22_X0P5M_A12TR u2371 ( .A0(n2205), .A1(program_ctr[8]), .B0(n2196), .B1(
        flags[0]), .Y(n2287) );
  NOR2B_X0P5M_A12TR u2372 ( .AN(n2200), .B(n2199), .Y(n2196) );
  AND2_X0P5M_A12TR u2373 ( .A(n2197), .B(n2214), .Y(n2205) );
  OAI31_X0P5M_A12TR u2374 ( .A0(n2288), .A1(n2289), .A2(n2290), .B0(n2198), 
        .Y(n2286) );
  NOR2_X0P5A_A12TR u2375 ( .A(n2200), .B(n2199), .Y(n2198) );
  OAI31_X0P5M_A12TR u2376 ( .A0(n2291), .A1(n2292), .A2(n2293), .B0(n1718), 
        .Y(n2199) );
  NOR2_X0P5A_A12TR u2377 ( .A(n2126), .B(n2294), .Y(n2292) );
  OAI21_X0P5M_A12TR u2378 ( .A0(n2129), .A1(n2295), .B0(n2296), .Y(n2291) );
  NAND4B_X0P5M_A12TR u2379 ( .AN(n2297), .B(n2153), .C(n2298), .D(n1739), .Y(
        n2200) );
  NOR2_X0P5A_A12TR u2380 ( .A(n2299), .B(n2105), .Y(n2298) );
  NAND2_X0P5A_A12TR u2381 ( .A(n2300), .B(n2301), .Y(n2105) );
  AND4_X0P5M_A12TR u2382 ( .A(n2302), .B(n2303), .C(n2304), .D(n21651), .Y(
        n2153) );
  AND2_X0P5M_A12TR u2383 ( .A(n2194), .B(n2305), .Y(n2304) );
  NAND4_X0P5A_A12TR u2384 ( .A(n2306), .B(n2307), .C(n2308), .D(n2309), .Y(
        n2297) );
  OAI31_X0P5M_A12TR u2385 ( .A0(n2310), .A1(n2311), .A2(n2312), .B0(n21571), 
        .Y(n2306) );
  NAND3B_X0P5M_A12TR u2386 ( .AN(n2154), .B(n2313), .C(n2314), .Y(n2310) );
  OAI22_X0P5M_A12TR u2387 ( .A0(n267), .A1(n2210), .B0(n266), .B1(n2211), .Y(
        n2290) );
  NAND3_X0P5A_A12TR u2388 ( .A(n2315), .B(n2316), .C(n2317), .Y(n2211) );
  NAND3_X0P5A_A12TR u2389 ( .A(n2318), .B(n2315), .C(n2317), .Y(n2210) );
  INV_X0P5B_A12TR u2390 ( .A(n2319), .Y(n2317) );
  OAI22_X0P5M_A12TR u2391 ( .A0(n269), .A1(n2212), .B0(n268), .B1(n2213), .Y(
        n2289) );
  NAND3_X0P5A_A12TR u2392 ( .A(n2316), .B(n2319), .C(n2315), .Y(n2213) );
  NAND3_X0P5A_A12TR u2393 ( .A(n2315), .B(n2319), .C(n2318), .Y(n2212) );
  OAI221_X0P5M_A12TR u2394 ( .A0(n1614), .A1(n2214), .B0(n2320), .B1(n2215), 
        .C0(n2321), .Y(n2288) );
  AOI22_X0P5M_A12TR u2395 ( .A0(n2217), .A1(n2322), .B0(n2219), .B1(n2323), 
        .Y(n2321) );
  NOR3_X0P5A_A12TR u2396 ( .A(n2318), .B(n2315), .C(n2319), .Y(n2219) );
  NOR3_X0P5A_A12TR u2397 ( .A(n2316), .B(n2315), .C(n2319), .Y(n2217) );
  INV_X0P5B_A12TR u2398 ( .A(n2324), .Y(n2315) );
  NAND3_X0P5A_A12TR u2399 ( .A(n2319), .B(n2324), .C(n2316), .Y(n2215) );
  NAND2B_X0P5M_A12TR u2400 ( .AN(n2214), .B(n2197), .Y(n2202) );
  NOR2_X0P5A_A12TR u2401 ( .A(n1720), .B(n16991), .Y(n2197) );
  NAND3_X0P5A_A12TR u2402 ( .A(n2319), .B(n2324), .C(n2318), .Y(n2214) );
  INV_X0P5B_A12TR u2403 ( .A(n2316), .Y(n2318) );
  AO1B2_X0P5M_A12TR u2404 ( .B0(n2293), .B1(reg0[0]), .A0N(n2325), .Y(n2316)
         );
  NAND2_X0P5A_A12TR u2405 ( .A(reg0[2]), .B(n2293), .Y(n2324) );
  NAND2_X0P5A_A12TR u2406 ( .A(reg0[1]), .B(n2293), .Y(n2319) );
  OAI21_X0P5M_A12TR u2407 ( .A0(n2129), .A1(n21641), .B0(n2326), .Y(n2293) );
  INV_X0P5B_A12TR u2408 ( .A(n1613), .Y(n2801) );
  MXIT2_X0P5M_A12TR u2409 ( .A(n2327), .B(n2328), .S0(n15881), .Y(n1613) );
  OAI221_X0P5M_A12TR u2410 ( .A0(n266), .A1(n15951), .B0(n15941), .B1(n2320), 
        .C0(n2329), .Y(n2328) );
  AOI22_X0P5M_A12TR u2411 ( .A0(n15891), .A1(n2323), .B0(n15911), .B1(n2330), 
        .Y(n2329) );
  OAI221_X0P5M_A12TR u2412 ( .A0(n267), .A1(n15951), .B0(n15941), .B1(n1614), 
        .C0(n2331), .Y(n2327) );
  AOI22_X0P5M_A12TR u2413 ( .A0(n15891), .A1(n2322), .B0(n15911), .B1(n2332), 
        .Y(n2331) );
  INV_X0P5B_A12TR u2414 ( .A(n1615), .Y(n2800) );
  MXIT2_X0P5M_A12TR u2415 ( .A(n2333), .B(n2334), .S0(n15881), .Y(n1615) );
  OAI221_X0P5M_A12TR u2416 ( .A0(n285), .A1(n15951), .B0(n15941), .B1(n2281), 
        .C0(n2335), .Y(n2334) );
  AOI22_X0P5M_A12TR u2417 ( .A0(n15891), .A1(n2284), .B0(n15911), .B1(n2336), 
        .Y(n2335) );
  OAI221_X0P5M_A12TR u2418 ( .A0(n286), .A1(n15951), .B0(n15941), .B1(n1616), 
        .C0(n2337), .Y(n2333) );
  AOI22_X0P5M_A12TR u2419 ( .A0(n15891), .A1(n2283), .B0(n15911), .B1(n2338), 
        .Y(n2337) );
  INV_X0P5B_A12TR u2420 ( .A(n1617), .Y(n2799) );
  MXIT2_X0P5M_A12TR u2421 ( .A(n2339), .B(n2340), .S0(n15881), .Y(n1617) );
  OAI221_X0P5M_A12TR u2422 ( .A0(n315), .A1(n15951), .B0(n15941), .B1(n2271), 
        .C0(n2341), .Y(n2340) );
  AOI22_X0P5M_A12TR u2423 ( .A0(n15891), .A1(n2274), .B0(n15911), .B1(n2342), 
        .Y(n2341) );
  OAI221_X0P5M_A12TR u2424 ( .A0(n316), .A1(n15951), .B0(n15941), .B1(n15961), 
        .C0(n2343), .Y(n2339) );
  AOI22_X0P5M_A12TR u2425 ( .A0(n15891), .A1(n2273), .B0(n15911), .B1(n2344), 
        .Y(n2343) );
  INV_X0P5B_A12TR u2426 ( .A(n1618), .Y(n2798) );
  MXIT2_X0P5M_A12TR u2427 ( .A(n2345), .B(n2346), .S0(n15881), .Y(n1618) );
  OAI221_X0P5M_A12TR u2428 ( .A0(n309), .A1(n15951), .B0(n15941), .B1(n2261), 
        .C0(n2347), .Y(n2346) );
  AOI22_X0P5M_A12TR u2429 ( .A0(n15891), .A1(n2264), .B0(n15911), .B1(n2348), 
        .Y(n2347) );
  OAI221_X0P5M_A12TR u2430 ( .A0(n310), .A1(n15951), .B0(n15941), .B1(n15901), 
        .C0(n2349), .Y(n2345) );
  AOI22_X0P5M_A12TR u2431 ( .A0(n15891), .A1(n2263), .B0(n15911), .B1(n2350), 
        .Y(n2349) );
  INV_X0P5B_A12TR u2432 ( .A(n1779), .Y(n2797) );
  MXIT2_X0P5M_A12TR u2433 ( .A(n2351), .B(n2352), .S0(n15881), .Y(n1779) );
  OAI221_X0P5M_A12TR u2434 ( .A0(n303), .A1(n15951), .B0(n15941), .B1(n2250), 
        .C0(n2353), .Y(n2352) );
  AOI22_X0P5M_A12TR u2435 ( .A0(n15891), .A1(n2253), .B0(n15911), .B1(n2354), 
        .Y(n2353) );
  OAI221_X0P5M_A12TR u2436 ( .A0(n304), .A1(n15951), .B0(n15941), .B1(n15971), 
        .C0(n2355), .Y(n2351) );
  AOI22_X0P5M_A12TR u2437 ( .A0(n15891), .A1(n2252), .B0(n15911), .B1(n2356), 
        .Y(n2355) );
  INV_X0P5B_A12TR u2438 ( .A(n1786), .Y(n2796) );
  MXIT2_X0P5M_A12TR u2439 ( .A(n2357), .B(n2358), .S0(n15881), .Y(n1786) );
  OAI221_X0P5M_A12TR u2440 ( .A0(n297), .A1(n15951), .B0(n15941), .B1(n2239), 
        .C0(n2359), .Y(n2358) );
  AOI22_X0P5M_A12TR u2441 ( .A0(n15891), .A1(n2242), .B0(n15911), .B1(n2360), 
        .Y(n2359) );
  OAI221_X0P5M_A12TR u2442 ( .A0(n298), .A1(n15951), .B0(n15941), .B1(n15921), 
        .C0(n2361), .Y(n2357) );
  AOI22_X0P5M_A12TR u2443 ( .A0(n15891), .A1(n2241), .B0(n15911), .B1(n2362), 
        .Y(n2361) );
  INV_X0P5B_A12TR u2444 ( .A(n1619), .Y(n2795) );
  MXIT2_X0P5M_A12TR u2445 ( .A(n2363), .B(n2364), .S0(n15881), .Y(n1619) );
  OAI221_X0P5M_A12TR u2446 ( .A0(n291), .A1(n15951), .B0(n15941), .B1(n2228), 
        .C0(n2365), .Y(n2364) );
  AOI22_X0P5M_A12TR u2447 ( .A0(n15891), .A1(n2231), .B0(n15911), .B1(n2366), 
        .Y(n2365) );
  OAI221_X0P5M_A12TR u2448 ( .A0(n292), .A1(n15951), .B0(n15941), .B1(n1620), 
        .C0(n2367), .Y(n2363) );
  AOI22_X0P5M_A12TR u2449 ( .A0(n15891), .A1(n2230), .B0(n15911), .B1(n2368), 
        .Y(n2367) );
  INV_X0P5B_A12TR u2450 ( .A(n1749), .Y(n2794) );
  XNOR2_X0P5M_A12TR u2451 ( .A(regfile[15]), .B(n1749), .Y(n25480) );
  NOR2_X0P5A_A12TR u2452 ( .A(n1625), .B(n1749), .Y(n25120) );
  NAND2_X0P5A_A12TR u2453 ( .A(n1749), .B(n1625), .Y(n24760) );
  MXIT2_X0P5M_A12TR u2454 ( .A(n2369), .B(n2370), .S0(n15881), .Y(n1749) );
  OAI211_X0P5M_A12TR u2455 ( .A0(n321), .A1(n2371), .B0(n2134), .C0(n2372), 
        .Y(n15881) );
  OA21A1OI2_X0P5M_A12TR u2456 ( .A0(n2373), .A1(n2374), .B0(reg0[0]), .C0(
        n2658), .Y(n2372) );
  INV_X0P5B_A12TR u2457 ( .A(n2375), .Y(n2134) );
  OAI221_X0P5M_A12TR u2458 ( .A0(n272), .A1(n15951), .B0(n15941), .B1(n2077), 
        .C0(n2376), .Y(n2370) );
  AOI22_X0P5M_A12TR u2459 ( .A0(n15891), .A1(n2220), .B0(n15911), .B1(n2377), 
        .Y(n2376) );
  OAI221_X0P5M_A12TR u2460 ( .A0(n273), .A1(n15951), .B0(n15941), .B1(n1625), 
        .C0(n2378), .Y(n2369) );
  AOI22_X0P5M_A12TR u2461 ( .A0(n15891), .A1(n2218), .B0(n15911), .B1(n2379), 
        .Y(n2378) );
  NOR2_X0P5A_A12TR u2462 ( .A(n2380), .B(n2381), .Y(n15911) );
  NOR2_X0P5A_A12TR u2463 ( .A(n2382), .B(n2383), .Y(n15891) );
  INV_X0P5B_A12TR u2464 ( .A(n1660), .Y(n15941) );
  NOR2_X0P5A_A12TR u2465 ( .A(n2380), .B(n2382), .Y(n1660) );
  INV_X0P5B_A12TR u2466 ( .A(n2021), .Y(n15951) );
  NOR2_X0P5A_A12TR u2467 ( .A(n2383), .B(n2381), .Y(n2021) );
  INV_X0P5B_A12TR u2468 ( .A(n2382), .Y(n2381) );
  OAI22_X0P5M_A12TR u2469 ( .A0(n2384), .A1(n1709), .B0(n323), .B1(n2371), .Y(
        n2382) );
  INV_X0P5B_A12TR u2470 ( .A(n2380), .Y(n2383) );
  OAI221_X0P5M_A12TR u2471 ( .A0(n322), .A1(n2371), .B0(n2384), .B1(n1708), 
        .C0(n2136), .Y(n2380) );
  NOR3_X0P5A_A12TR u2472 ( .A(n2375), .B(n2373), .C(n2374), .Y(n2384) );
  NOR2B_X0P5M_A12TR u2473 ( .AN(n21611), .B(n2195), .Y(n2373) );
  NAND2_X0P5A_A12TR u2474 ( .A(n2385), .B(n2386), .Y(n2371) );
  NOR2_X0P5A_A12TR u2475 ( .A(int_ack), .B(n1973), .Y(n21420) );
  INV_X0P5B_A12TR u2476 ( .A(wait_for_fsm), .Y(n1973) );
  NAND4_X0P5A_A12TR u2477 ( .A(n2387), .B(n2388), .C(n1720), .D(n2389), .Y(
        cpu_next_state[5]) );
  AND3_X0P5M_A12TR u2478 ( .A(n2307), .B(n1727), .C(n2390), .Y(n2389) );
  AND2_X0P5M_A12TR u2479 ( .A(n2325), .B(n1737), .Y(n1720) );
  AOI21_X0P5M_A12TR u2480 ( .A0(n1723), .A1(n1861), .B0(n2658), .Y(n2325) );
  OAI21_X0P5M_A12TR u2481 ( .A0(n1859), .A1(n2391), .B0(n1723), .Y(n2388) );
  INV_X0P5B_A12TR u2482 ( .A(n2392), .Y(cpu_next_state[4]) );
  OA21A1OI2_X0P5M_A12TR u2483 ( .A0(n2393), .A1(n2394), .B0(n1718), .C0(n1857), 
        .Y(n2392) );
  NOR3_X0P5A_A12TR u2484 ( .A(n2129), .B(n16991), .C(n2395), .Y(n1857) );
  NAND4_X0P5A_A12TR u2485 ( .A(n2305), .B(n1733), .C(n2296), .D(n16971), .Y(
        n2394) );
  OAI211_X0P5M_A12TR u2486 ( .A0(n2396), .A1(n2129), .B0(n2397), .C0(n2326), 
        .Y(n2393) );
  AOI21_X0P5M_A12TR u2487 ( .A0(cpu_state[4]), .A1(n2398), .B0(n2399), .Y(
        n2397) );
  AOI31_X0P5M_A12TR u2488 ( .A0(n2400), .A1(n2401), .A2(n2402), .B0(n16991), 
        .Y(cpu_next_state[3]) );
  AND4_X0P5M_A12TR u2489 ( .A(n2390), .B(n1727), .C(n2309), .D(n2403), .Y(
        n2402) );
  NOR3_X0P5A_A12TR u2490 ( .A(n1730), .B(n2404), .C(n1732), .Y(n2403) );
  INV_X0P5B_A12TR u2491 ( .A(n2405), .Y(n1732) );
  INV_X0P5B_A12TR u2492 ( .A(n2406), .Y(n2404) );
  AOI222_X0P5M_A12TR u2493 ( .A0(n2407), .A1(n2408), .B0(n2409), .B1(n2410), 
        .C0(n2398), .C1(cpu_state[3]), .Y(n2401) );
  NAND4B_X0P5M_A12TR u2494 ( .AN(n2411), .B(n2412), .C(n2413), .D(n2414), .Y(
        n2410) );
  AOI21B_X0P5M_A12TR u2495 ( .A0(n1859), .A1(n2415), .B0N(n2395), .Y(n2413) );
  NAND4_X0P5A_A12TR u2496 ( .A(n2416), .B(n2417), .C(n21641), .D(n2418), .Y(
        n2411) );
  NOR2_X0P5A_A12TR u2497 ( .A(n2419), .B(n2399), .Y(n2400) );
  AOI31_X0P5M_A12TR u2498 ( .A0(n2420), .A1(n2421), .A2(n2422), .B0(n16991), 
        .Y(cpu_next_state[2]) );
  AND4_X0P5M_A12TR u2499 ( .A(n1737), .B(n2423), .C(n2405), .D(n2424), .Y(
        n2422) );
  AOI22_X0P5M_A12TR u2500 ( .A0(n2409), .A1(n2425), .B0(n2398), .B1(
        cpu_state[2]), .Y(n2421) );
  NAND4_X0P5A_A12TR u2501 ( .A(n2416), .B(n2314), .C(n2395), .D(n2426), .Y(
        n2425) );
  AOI21_X0P5M_A12TR u2502 ( .A0(n2415), .A1(n2391), .B0(n21561), .Y(n2426) );
  NAND4_X0P5A_A12TR u2503 ( .A(n2295), .B(n2130), .C(n2427), .D(n2428), .Y(
        n21561) );
  AND3_X0P5M_A12TR u2504 ( .A(n2418), .B(n21631), .C(n21621), .Y(n2428) );
  NAND4_X0P5A_A12TR u2505 ( .A(n2429), .B(n2430), .C(n2431), .D(n2432), .Y(
        n2395) );
  AOI22_X0P5M_A12TR u2506 ( .A0(n2433), .A1(n1865), .B0(n1863), .B1(n1869), 
        .Y(n2432) );
  INV_X0P5B_A12TR u2507 ( .A(int_mask[5]), .Y(n1869) );
  INV_X0P5B_A12TR u2508 ( .A(int_mask[1]), .Y(n1865) );
  AOI22_X0P5M_A12TR u2509 ( .A0(n1858), .A1(n1808), .B0(n1722), .B1(n1868), 
        .Y(n2431) );
  INV_X0P5B_A12TR u2510 ( .A(int_mask[4]), .Y(n1868) );
  INV_X0P5B_A12TR u2511 ( .A(int_mask[6]), .Y(n1808) );
  AOI22_X0P5M_A12TR u2512 ( .A0(n1861), .A1(n1870), .B0(n1859), .B1(n1867), 
        .Y(n2430) );
  INV_X0P5B_A12TR u2513 ( .A(int_mask[3]), .Y(n1867) );
  INV_X0P5B_A12TR u2514 ( .A(int_mask[7]), .Y(n1870) );
  AOI21_X0P5M_A12TR u2515 ( .A0(n1862), .A1(n1866), .B0(n2434), .Y(n2429) );
  INV_X0P5B_A12TR u2516 ( .A(int_mask[2]), .Y(n1866) );
  NOR2_X0P5A_A12TR u2517 ( .A(n2399), .B(n2435), .Y(n2420) );
  NAND4_X0P5A_A12TR u2518 ( .A(n2436), .B(n1728), .C(n2437), .D(n2438), .Y(
        n2399) );
  AOI31_X0P5M_A12TR u2519 ( .A0(n2439), .A1(n2440), .A2(n2441), .B0(n16991), 
        .Y(cpu_next_state[1]) );
  NOR3_X0P5A_A12TR u2520 ( .A(n2442), .B(n2435), .C(n2443), .Y(n2441) );
  OAI211_X0P5M_A12TR u2521 ( .A0(n2444), .A1(n2445), .B0(n2406), .C0(n2141), 
        .Y(n2435) );
  NAND3B_X0P5M_A12TR u2522 ( .AN(n2193), .B(n2446), .C(n1735), .Y(n2442) );
  AOI211_X0P5M_A12TR u2523 ( .A0(n2154), .A1(n21571), .B0(n1725), .C0(n2374), 
        .Y(n1735) );
  INV_X0P5B_A12TR u2524 ( .A(n2326), .Y(n1725) );
  NAND2_X0P5A_A12TR u2525 ( .A(n2447), .B(n2416), .Y(n2154) );
  OA21A1OI2_X0P5M_A12TR u2526 ( .A0(n2448), .A1(n2449), .B0(n2385), .C0(n2450), 
        .Y(n2440) );
  NAND2_X0P5A_A12TR u2527 ( .A(n2451), .B(n1727), .Y(n2450) );
  NAND2_X0P5A_A12TR u2528 ( .A(n2452), .B(n1862), .Y(n1727) );
  AOI22_X0P5M_A12TR u2529 ( .A0(n21571), .A1(n2453), .B0(n2398), .B1(
        cpu_state[1]), .Y(n2439) );
  OAI211_X0P5M_A12TR u2530 ( .A0(n2454), .A1(n2455), .B0(n2314), .C0(n2456), 
        .Y(n2453) );
  NAND2_X0P5A_A12TR u2531 ( .A(n1722), .B(n2415), .Y(n2314) );
  INV_X0P5B_A12TR u2532 ( .A(n1861), .Y(n2455) );
  AOI31_X0P5M_A12TR u2533 ( .A0(n2457), .A1(n2458), .A2(n2459), .B0(n16991), 
        .Y(cpu_next_state[0]) );
  AND4_X0P5M_A12TR u2534 ( .A(n2460), .B(n2305), .C(n1930), .D(n2461), .Y(
        n2459) );
  AND4_X0P5M_A12TR u2535 ( .A(n2194), .B(n2462), .C(n2436), .D(n16981), .Y(
        n2461) );
  AOI222_X0P5M_A12TR u2536 ( .A0(n2463), .A1(n2385), .B0(n21571), .B1(n2464), 
        .C0(n2398), .C1(cpu_state[0]), .Y(n2458) );
  INV_X0P5B_A12TR u2537 ( .A(n2307), .Y(n2398) );
  NAND4B_X0P5M_A12TR u2538 ( .AN(n2311), .B(n2465), .C(n2447), .D(n2466), .Y(
        n2464) );
  NAND2_X0P5A_A12TR u2539 ( .A(n1861), .B(n2415), .Y(n2466) );
  AOI211_X0P5M_A12TR u2540 ( .A0(n2415), .A1(n1859), .B0(n21601), .C0(n1706), 
        .Y(n2447) );
  AO21A1AI2_X0P5M_A12TR u2541 ( .A0(n2467), .A1(n2145), .B0(n2454), .C0(n2434), 
        .Y(n21601) );
  NOR2_X0P5A_A12TR u2542 ( .A(n1858), .B(n1863), .Y(n2467) );
  NAND3_X0P5A_A12TR u2543 ( .A(n2468), .B(n2130), .C(n2140), .Y(n2311) );
  AOI21_X0P5M_A12TR u2544 ( .A0(n2391), .A1(n2415), .B0(n2155), .Y(n2140) );
  INV_X0P5B_A12TR u2545 ( .A(n2417), .Y(n2155) );
  INV_X0P5B_A12TR u2546 ( .A(n2454), .Y(n2415) );
  OR2_X0P5M_A12TR u2547 ( .A(n2433), .B(n1862), .Y(n2391) );
  NOR3_X0P5A_A12TR u2548 ( .A(n2080), .B(n1724), .C(n2443), .Y(n2457) );
  NAND3_X0P5A_A12TR u2549 ( .A(n1736), .B(n2469), .C(n2470), .Y(n2443) );
  NOR3_X0P5A_A12TR u2550 ( .A(n2143), .B(n2471), .C(n2658), .Y(n2470) );
  INV_X0P5B_A12TR u2551 ( .A(n2136), .Y(n2658) );
  INV_X0P5B_A12TR u2552 ( .A(n2423), .Y(n2143) );
  AOI222_X0P5M_A12TR u2553 ( .A0(flags[0]), .A1(n2472), .B0(n2185), .B1(n2473), 
        .C0(n2474), .C1(n2463), .Y(n1736) );
  INV_X0P5B_A12TR u2554 ( .A(n2186), .Y(n2473) );
  XOR2_X0P5M_A12TR u2555 ( .A(n2475), .B(opcode[0]), .Y(n2185) );
  NAND4_X0P5A_A12TR u2556 ( .A(n24761), .B(n2477), .C(n2478), .D(n2479), .Y(
        n2475) );
  AOI22_X0P5M_A12TR u2557 ( .A0(n2480), .A1(flags[0]), .B0(flags[1]), .B1(
        n2433), .Y(n2479) );
  AOI22_X0P5M_A12TR u2558 ( .A0(n1863), .A1(n2235), .B0(n1858), .B1(n2224), 
        .Y(n2478) );
  INV_X0P5B_A12TR u2559 ( .A(n280), .Y(n2224) );
  INV_X0P5B_A12TR u2560 ( .A(n282), .Y(n2235) );
  AOI22_X0P5M_A12TR u2561 ( .A0(n1722), .A1(n2246), .B0(n1861), .B1(n2206), 
        .Y(n2477) );
  INV_X0P5B_A12TR u2562 ( .A(n278), .Y(n2206) );
  INV_X0P5B_A12TR u2563 ( .A(n279), .Y(n2246) );
  AOI22_X0P5M_A12TR u2564 ( .A0(n1859), .A1(n2257), .B0(flags[2]), .B1(n1862), 
        .Y(n24761) );
  INV_X0P5B_A12TR u2565 ( .A(n281), .Y(n2257) );
  INV_X0P5B_A12TR u2566 ( .A(n2184), .Y(n2472) );
  NAND3_X0P5A_A12TR u2567 ( .A(n2137), .B(n1738), .C(n2481), .Y(n2080) );
  NAND2_X0P5A_A12TR u2568 ( .A(n2482), .B(n2385), .Y(n1738) );
  OAI211_X0P5M_A12TR u2569 ( .A0(n2483), .A1(n2484), .B0(n2485), .C0(n2486), 
        .Y(address[9]) );
  AOI222_X0P5M_A12TR u2570 ( .A0(isr_addr[9]), .A1(n2487), .B0(operand2[1]), 
        .B1(n2193), .C0(n1254), .C1(n21691), .Y(n2486) );
  AOI22_X0P5M_A12TR u2571 ( .A0(n2488), .A1(n2489), .B0(stack_ptr[9]), .B1(
        n2189), .Y(n2485) );
  INV_X0P5B_A12TR u2572 ( .A(n21661), .Y(n2488) );
  MXIT2_X0P5M_A12TR u2573 ( .A(n2490), .B(n2491), .S0(n321), .Y(n21661) );
  OAI221_X0P5M_A12TR u2574 ( .A0(n290), .A1(n2492), .B0(n1616), .B1(n2493), 
        .C0(n2494), .Y(n2491) );
  AOI22_X0P5M_A12TR u2575 ( .A0(n2495), .A1(n2496), .B0(n2497), .B1(n2338), 
        .Y(n2494) );
  INV_X0P5B_A12TR u2576 ( .A(regfile[9]), .Y(n1616) );
  OAI221_X0P5M_A12TR u2577 ( .A0(n289), .A1(n2492), .B0(n2281), .B1(n2493), 
        .C0(n2498), .Y(n2490) );
  AOI22_X0P5M_A12TR u2578 ( .A0(n2495), .A1(n2499), .B0(n2497), .B1(n2336), 
        .Y(n2498) );
  INV_X0P5B_A12TR u2579 ( .A(regfile[1]), .Y(n2281) );
  INV_X0P5B_A12TR u2580 ( .A(program_ctr[9]), .Y(n2484) );
  OAI211_X0P5M_A12TR u2581 ( .A0(n2483), .A1(n2500), .B0(n2501), .C0(n2502), 
        .Y(address[8]) );
  AOI222_X0P5M_A12TR u2582 ( .A0(isr_addr[8]), .A1(n2487), .B0(operand2[0]), 
        .B1(n2193), .C0(n1253), .C1(n21691), .Y(n2502) );
  AOI22_X0P5M_A12TR u2583 ( .A0(n2503), .A1(n2489), .B0(stack_ptr[8]), .B1(
        n2189), .Y(n2501) );
  INV_X0P5B_A12TR u2584 ( .A(n21671), .Y(n2503) );
  MXIT2_X0P5M_A12TR u2585 ( .A(n2504), .B(n2505), .S0(n321), .Y(n21671) );
  OAI221_X0P5M_A12TR u2586 ( .A0(n271), .A1(n2492), .B0(n1614), .B1(n2493), 
        .C0(n2506), .Y(n2505) );
  AOI22_X0P5M_A12TR u2587 ( .A0(n2495), .A1(n2507), .B0(n2497), .B1(n2332), 
        .Y(n2506) );
  INV_X0P5B_A12TR u2588 ( .A(regfile[8]), .Y(n1614) );
  OAI221_X0P5M_A12TR u2589 ( .A0(n270), .A1(n2492), .B0(n2320), .B1(n2493), 
        .C0(n2508), .Y(n2504) );
  AOI22_X0P5M_A12TR u2590 ( .A0(n2495), .A1(n2509), .B0(n2497), .B1(n2330), 
        .Y(n2508) );
  INV_X0P5B_A12TR u2591 ( .A(regfile[0]), .Y(n2320) );
  INV_X0P5B_A12TR u2592 ( .A(program_ctr[8]), .Y(n2500) );
  OAI211_X0P5M_A12TR u2593 ( .A0(n2483), .A1(n2201), .B0(n2510), .C0(n2511), 
        .Y(address[7]) );
  AOI222_X0P5M_A12TR u2594 ( .A0(isr_addr[7]), .A1(n2487), .B0(offset_sx_15), 
        .B1(n2193), .C0(n1252), .C1(n21691), .Y(n2511) );
  AOI22_X0P5M_A12TR u2595 ( .A0(n21681), .A1(n2489), .B0(stack_ptr[7]), .B1(
        n2189), .Y(n2510) );
  NAND4_X0P5A_A12TR u2596 ( .A(n25121), .B(n2513), .C(n2514), .D(n2515), .Y(
        n21681) );
  AOI22_X0P5M_A12TR u2597 ( .A0(n2480), .A1(regfile[15]), .B0(n2433), .B1(
        regfile[7]), .Y(n2515) );
  AOI22_X0P5M_A12TR u2598 ( .A0(n1863), .A1(n2377), .B0(n1858), .B1(n2516), 
        .Y(n2514) );
  AOI22_X0P5M_A12TR u2599 ( .A0(n1722), .A1(n2379), .B0(n1861), .B1(n2517), 
        .Y(n2513) );
  AOI22_X0P5M_A12TR u2600 ( .A0(n1859), .A1(n2220), .B0(n1862), .B1(n2218), 
        .Y(n25121) );
  INV_X0P5B_A12TR u2601 ( .A(n277), .Y(n2218) );
  INV_X0P5B_A12TR u2602 ( .A(n276), .Y(n2220) );
  INV_X0P5B_A12TR u2603 ( .A(program_ctr[7]), .Y(n2201) );
  OAI211_X0P5M_A12TR u2604 ( .A0(n2483), .A1(n2221), .B0(n2518), .C0(n2519), 
        .Y(address[6]) );
  AOI222_X0P5M_A12TR u2605 ( .A0(isr_addr[6]), .A1(n2487), .B0(offset_sx[6]), 
        .B1(n2193), .C0(n1251), .C1(n21691), .Y(n2519) );
  AOI22_X0P5M_A12TR u2606 ( .A0(n21701), .A1(n2489), .B0(stack_ptr[6]), .B1(
        n2189), .Y(n2518) );
  NAND4_X0P5A_A12TR u2607 ( .A(n2520), .B(n2521), .C(n2522), .D(n2523), .Y(
        n21701) );
  AOI22_X0P5M_A12TR u2608 ( .A0(n2480), .A1(regfile[14]), .B0(n2433), .B1(
        regfile[6]), .Y(n2523) );
  AOI22_X0P5M_A12TR u2609 ( .A0(n1863), .A1(n2366), .B0(n1858), .B1(n2524), 
        .Y(n2522) );
  AOI22_X0P5M_A12TR u2610 ( .A0(n1722), .A1(n2368), .B0(n1861), .B1(n2525), 
        .Y(n2521) );
  AOI22_X0P5M_A12TR u2611 ( .A0(n1859), .A1(n2231), .B0(n1862), .B1(n2230), 
        .Y(n2520) );
  INV_X0P5B_A12TR u2612 ( .A(n296), .Y(n2230) );
  INV_X0P5B_A12TR u2613 ( .A(n295), .Y(n2231) );
  INV_X0P5B_A12TR u2614 ( .A(program_ctr[6]), .Y(n2221) );
  OAI211_X0P5M_A12TR u2615 ( .A0(n2483), .A1(n2232), .B0(n2526), .C0(n2527), 
        .Y(address[5]) );
  AOI222_X0P5M_A12TR u2616 ( .A0(isr_addr[5]), .A1(n2487), .B0(offset_sx[5]), 
        .B1(n2193), .C0(n1250), .C1(n21691), .Y(n2527) );
  AOI22_X0P5M_A12TR u2617 ( .A0(n21711), .A1(n2489), .B0(stack_ptr[5]), .B1(
        n2189), .Y(n2526) );
  NAND4_X0P5A_A12TR u2618 ( .A(n2528), .B(n2529), .C(n2530), .D(n2531), .Y(
        n21711) );
  AOI22_X0P5M_A12TR u2619 ( .A0(n2480), .A1(regfile[13]), .B0(n2433), .B1(
        regfile[5]), .Y(n2531) );
  AOI22_X0P5M_A12TR u2620 ( .A0(n1863), .A1(n2360), .B0(n1858), .B1(n2532), 
        .Y(n2530) );
  AOI22_X0P5M_A12TR u2621 ( .A0(n1722), .A1(n2362), .B0(n1861), .B1(n2533), 
        .Y(n2529) );
  AOI22_X0P5M_A12TR u2622 ( .A0(n1859), .A1(n2242), .B0(n1862), .B1(n2241), 
        .Y(n2528) );
  INV_X0P5B_A12TR u2623 ( .A(n302), .Y(n2241) );
  INV_X0P5B_A12TR u2624 ( .A(n301), .Y(n2242) );
  INV_X0P5B_A12TR u2625 ( .A(program_ctr[5]), .Y(n2232) );
  OAI211_X0P5M_A12TR u2626 ( .A0(n2483), .A1(n2243), .B0(n2534), .C0(n2535), 
        .Y(address[4]) );
  AOI222_X0P5M_A12TR u2627 ( .A0(isr_addr[4]), .A1(n2487), .B0(offset_sx[4]), 
        .B1(n2193), .C0(n1249), .C1(n21691), .Y(n2535) );
  AOI22_X0P5M_A12TR u2628 ( .A0(n2172), .A1(n2489), .B0(stack_ptr[4]), .B1(
        n2189), .Y(n2534) );
  NAND4_X0P5A_A12TR u2629 ( .A(n2536), .B(n2537), .C(n2538), .D(n2539), .Y(
        n2172) );
  AOI22_X0P5M_A12TR u2630 ( .A0(n2480), .A1(regfile[12]), .B0(n2433), .B1(
        regfile[4]), .Y(n2539) );
  AOI22_X0P5M_A12TR u2631 ( .A0(n1863), .A1(n2354), .B0(n1858), .B1(n2540), 
        .Y(n2538) );
  AOI22_X0P5M_A12TR u2632 ( .A0(n1722), .A1(n2356), .B0(n1861), .B1(n2541), 
        .Y(n2537) );
  AOI22_X0P5M_A12TR u2633 ( .A0(n1859), .A1(n2253), .B0(n1862), .B1(n2252), 
        .Y(n2536) );
  INV_X0P5B_A12TR u2634 ( .A(n308), .Y(n2252) );
  INV_X0P5B_A12TR u2635 ( .A(n307), .Y(n2253) );
  INV_X0P5B_A12TR u2636 ( .A(program_ctr[4]), .Y(n2243) );
  OAI211_X0P5M_A12TR u2637 ( .A0(n2483), .A1(n2254), .B0(n2542), .C0(n2543), 
        .Y(address[3]) );
  AOI222_X0P5M_A12TR u2638 ( .A0(isr_addr[3]), .A1(n2487), .B0(offset_sx[3]), 
        .B1(n2193), .C0(n1248), .C1(n21691), .Y(n2543) );
  AOI22_X0P5M_A12TR u2639 ( .A0(n2173), .A1(n2489), .B0(stack_ptr[3]), .B1(
        n2189), .Y(n2542) );
  NAND4_X0P5A_A12TR u2640 ( .A(n2544), .B(n2545), .C(n2546), .D(n2547), .Y(
        n2173) );
  AOI22_X0P5M_A12TR u2641 ( .A0(n2480), .A1(regfile[11]), .B0(n2433), .B1(
        regfile[3]), .Y(n2547) );
  AOI22_X0P5M_A12TR u2642 ( .A0(n1863), .A1(n2348), .B0(n1858), .B1(n25481), 
        .Y(n2546) );
  AOI22_X0P5M_A12TR u2643 ( .A0(n1722), .A1(n2350), .B0(n1861), .B1(n2549), 
        .Y(n2545) );
  AOI22_X0P5M_A12TR u2644 ( .A0(n1859), .A1(n2264), .B0(n1862), .B1(n2263), 
        .Y(n2544) );
  INV_X0P5B_A12TR u2645 ( .A(n314), .Y(n2263) );
  INV_X0P5B_A12TR u2646 ( .A(n313), .Y(n2264) );
  INV_X0P5B_A12TR u2647 ( .A(program_ctr[3]), .Y(n2254) );
  OAI211_X0P5M_A12TR u2648 ( .A0(n2483), .A1(n2265), .B0(n2550), .C0(n2551), 
        .Y(address[2]) );
  AOI222_X0P5M_A12TR u2649 ( .A0(isr_addr[2]), .A1(n2487), .B0(offset_sx[2]), 
        .B1(n2193), .C0(n1247), .C1(n21691), .Y(n2551) );
  AOI22_X0P5M_A12TR u2650 ( .A0(n2174), .A1(n2489), .B0(stack_ptr[2]), .B1(
        n2189), .Y(n2550) );
  NAND4_X0P5A_A12TR u2651 ( .A(n2552), .B(n2553), .C(n2554), .D(n2555), .Y(
        n2174) );
  AOI22_X0P5M_A12TR u2652 ( .A0(n2480), .A1(regfile[10]), .B0(n2433), .B1(
        regfile[2]), .Y(n2555) );
  AOI22_X0P5M_A12TR u2653 ( .A0(n1863), .A1(n2342), .B0(n1858), .B1(n2556), 
        .Y(n2554) );
  AOI22_X0P5M_A12TR u2654 ( .A0(n1722), .A1(n2344), .B0(n1861), .B1(n2557), 
        .Y(n2553) );
  AOI22_X0P5M_A12TR u2655 ( .A0(n1859), .A1(n2274), .B0(n1862), .B1(n2273), 
        .Y(n2552) );
  INV_X0P5B_A12TR u2656 ( .A(n320), .Y(n2273) );
  INV_X0P5B_A12TR u2657 ( .A(n319), .Y(n2274) );
  INV_X0P5B_A12TR u2658 ( .A(program_ctr[2]), .Y(n2265) );
  OAI211_X0P5M_A12TR u2659 ( .A0(n2483), .A1(n2275), .B0(n2558), .C0(n2559), 
        .Y(address[1]) );
  AOI222_X0P5M_A12TR u2660 ( .A0(isr_addr[1]), .A1(n2487), .B0(offset_sx[1]), 
        .B1(n2193), .C0(n1246), .C1(n21691), .Y(n2559) );
  AOI22_X0P5M_A12TR u2661 ( .A0(n2181), .A1(n2489), .B0(stack_ptr[1]), .B1(
        n2189), .Y(n2558) );
  NAND4_X0P5A_A12TR u2662 ( .A(n2560), .B(n2561), .C(n2562), .D(n2563), .Y(
        n2181) );
  AOI22_X0P5M_A12TR u2663 ( .A0(n2480), .A1(regfile[9]), .B0(n2433), .B1(
        regfile[1]), .Y(n2563) );
  AOI22_X0P5M_A12TR u2664 ( .A0(n1863), .A1(n2336), .B0(n1858), .B1(n2496), 
        .Y(n2562) );
  INV_X0P5B_A12TR u2665 ( .A(n286), .Y(n2496) );
  INV_X0P5B_A12TR u2666 ( .A(n287), .Y(n2336) );
  AOI22_X0P5M_A12TR u2667 ( .A0(n1722), .A1(n2338), .B0(n1861), .B1(n2499), 
        .Y(n2561) );
  INV_X0P5B_A12TR u2668 ( .A(n285), .Y(n2499) );
  INV_X0P5B_A12TR u2669 ( .A(n288), .Y(n2338) );
  AOI22_X0P5M_A12TR u2670 ( .A0(n1859), .A1(n2284), .B0(n1862), .B1(n2283), 
        .Y(n2560) );
  INV_X0P5B_A12TR u2671 ( .A(n290), .Y(n2283) );
  INV_X0P5B_A12TR u2672 ( .A(n289), .Y(n2284) );
  INV_X0P5B_A12TR u2673 ( .A(program_ctr[1]), .Y(n2275) );
  OAI211_X0P5M_A12TR u2674 ( .A0(n2483), .A1(n2564), .B0(n2565), .C0(n2566), 
        .Y(address[15]) );
  AOI222_X0P5M_A12TR u2675 ( .A0(isr_addr[15]), .A1(n2487), .B0(operand2[7]), 
        .B1(n2193), .C0(n1260), .C1(n21691), .Y(n2566) );
  AOI22_X0P5M_A12TR u2676 ( .A0(n2567), .A1(n2489), .B0(stack_ptr[15]), .B1(
        n2189), .Y(n2565) );
  INV_X0P5B_A12TR u2677 ( .A(n2175), .Y(n2567) );
  MXIT2_X0P5M_A12TR u2678 ( .A(n2568), .B(n2569), .S0(n321), .Y(n2175) );
  OAI221_X0P5M_A12TR u2679 ( .A0(n277), .A1(n2492), .B0(n1625), .B1(n2493), 
        .C0(n2570), .Y(n2569) );
  AOI22_X0P5M_A12TR u2680 ( .A0(n2495), .A1(n2516), .B0(n2497), .B1(n2379), 
        .Y(n2570) );
  INV_X0P5B_A12TR u2681 ( .A(n275), .Y(n2379) );
  INV_X0P5B_A12TR u2682 ( .A(n273), .Y(n2516) );
  INV_X0P5B_A12TR u2683 ( .A(regfile[15]), .Y(n1625) );
  OAI221_X0P5M_A12TR u2684 ( .A0(n276), .A1(n2492), .B0(n2077), .B1(n2493), 
        .C0(n2571), .Y(n2568) );
  AOI22_X0P5M_A12TR u2685 ( .A0(n2495), .A1(n2517), .B0(n2497), .B1(n2377), 
        .Y(n2571) );
  INV_X0P5B_A12TR u2686 ( .A(n274), .Y(n2377) );
  INV_X0P5B_A12TR u2687 ( .A(n272), .Y(n2517) );
  INV_X0P5B_A12TR u2688 ( .A(regfile[7]), .Y(n2077) );
  INV_X0P5B_A12TR u2689 ( .A(program_ctr[15]), .Y(n2564) );
  OAI211_X0P5M_A12TR u2690 ( .A0(n2483), .A1(n2572), .B0(n2573), .C0(n2574), 
        .Y(address[14]) );
  AOI222_X0P5M_A12TR u2691 ( .A0(isr_addr[14]), .A1(n2487), .B0(operand2[6]), 
        .B1(n2193), .C0(n1259), .C1(n21691), .Y(n2574) );
  AOI22_X0P5M_A12TR u2692 ( .A0(n2575), .A1(n2489), .B0(stack_ptr[14]), .B1(
        n2189), .Y(n2573) );
  INV_X0P5B_A12TR u2693 ( .A(n2176), .Y(n2575) );
  MXIT2_X0P5M_A12TR u2694 ( .A(n2576), .B(n2577), .S0(n321), .Y(n2176) );
  OAI221_X0P5M_A12TR u2695 ( .A0(n296), .A1(n2492), .B0(n1620), .B1(n2493), 
        .C0(n2578), .Y(n2577) );
  AOI22_X0P5M_A12TR u2696 ( .A0(n2495), .A1(n2524), .B0(n2497), .B1(n2368), 
        .Y(n2578) );
  INV_X0P5B_A12TR u2697 ( .A(n294), .Y(n2368) );
  INV_X0P5B_A12TR u2698 ( .A(n292), .Y(n2524) );
  INV_X0P5B_A12TR u2699 ( .A(regfile[14]), .Y(n1620) );
  OAI221_X0P5M_A12TR u2700 ( .A0(n295), .A1(n2492), .B0(n2228), .B1(n2493), 
        .C0(n2579), .Y(n2576) );
  AOI22_X0P5M_A12TR u2701 ( .A0(n2495), .A1(n2525), .B0(n2497), .B1(n2366), 
        .Y(n2579) );
  INV_X0P5B_A12TR u2702 ( .A(n293), .Y(n2366) );
  INV_X0P5B_A12TR u2703 ( .A(n291), .Y(n2525) );
  INV_X0P5B_A12TR u2704 ( .A(regfile[6]), .Y(n2228) );
  INV_X0P5B_A12TR u2705 ( .A(program_ctr[14]), .Y(n2572) );
  OAI211_X0P5M_A12TR u2706 ( .A0(n2483), .A1(n2580), .B0(n2581), .C0(n2582), 
        .Y(address[13]) );
  AOI222_X0P5M_A12TR u2707 ( .A0(isr_addr[13]), .A1(n2487), .B0(operand2[5]), 
        .B1(n2193), .C0(n1258), .C1(n21691), .Y(n2582) );
  AOI22_X0P5M_A12TR u2708 ( .A0(n2583), .A1(n2489), .B0(stack_ptr[13]), .B1(
        n2189), .Y(n2581) );
  INV_X0P5B_A12TR u2709 ( .A(n2177), .Y(n2583) );
  MXIT2_X0P5M_A12TR u2710 ( .A(n2584), .B(n2585), .S0(n321), .Y(n2177) );
  OAI221_X0P5M_A12TR u2711 ( .A0(n302), .A1(n2492), .B0(n15921), .B1(n2493), 
        .C0(n2586), .Y(n2585) );
  AOI22_X0P5M_A12TR u2712 ( .A0(n2495), .A1(n2532), .B0(n2497), .B1(n2362), 
        .Y(n2586) );
  INV_X0P5B_A12TR u2713 ( .A(n300), .Y(n2362) );
  INV_X0P5B_A12TR u2714 ( .A(n298), .Y(n2532) );
  INV_X0P5B_A12TR u2715 ( .A(regfile[13]), .Y(n15921) );
  OAI221_X0P5M_A12TR u2716 ( .A0(n301), .A1(n2492), .B0(n2239), .B1(n2493), 
        .C0(n2587), .Y(n2584) );
  AOI22_X0P5M_A12TR u2717 ( .A0(n2495), .A1(n2533), .B0(n2497), .B1(n2360), 
        .Y(n2587) );
  INV_X0P5B_A12TR u2718 ( .A(n299), .Y(n2360) );
  INV_X0P5B_A12TR u2719 ( .A(n297), .Y(n2533) );
  INV_X0P5B_A12TR u2720 ( .A(regfile[5]), .Y(n2239) );
  INV_X0P5B_A12TR u2721 ( .A(program_ctr[13]), .Y(n2580) );
  OAI211_X0P5M_A12TR u2722 ( .A0(n2483), .A1(n2588), .B0(n2589), .C0(n2590), 
        .Y(address[12]) );
  AOI222_X0P5M_A12TR u2723 ( .A0(isr_addr[12]), .A1(n2487), .B0(operand2[4]), 
        .B1(n2193), .C0(n1257), .C1(n21691), .Y(n2590) );
  AOI22_X0P5M_A12TR u2724 ( .A0(n2591), .A1(n2489), .B0(stack_ptr[12]), .B1(
        n2189), .Y(n2589) );
  INV_X0P5B_A12TR u2725 ( .A(n2178), .Y(n2591) );
  MXIT2_X0P5M_A12TR u2726 ( .A(n2592), .B(n2593), .S0(n321), .Y(n2178) );
  OAI221_X0P5M_A12TR u2727 ( .A0(n308), .A1(n2492), .B0(n15971), .B1(n2493), 
        .C0(n2594), .Y(n2593) );
  AOI22_X0P5M_A12TR u2728 ( .A0(n2495), .A1(n2540), .B0(n2497), .B1(n2356), 
        .Y(n2594) );
  INV_X0P5B_A12TR u2729 ( .A(n306), .Y(n2356) );
  INV_X0P5B_A12TR u2730 ( .A(n304), .Y(n2540) );
  INV_X0P5B_A12TR u2731 ( .A(regfile[12]), .Y(n15971) );
  OAI221_X0P5M_A12TR u2732 ( .A0(n307), .A1(n2492), .B0(n2250), .B1(n2493), 
        .C0(n2595), .Y(n2592) );
  AOI22_X0P5M_A12TR u2733 ( .A0(n2495), .A1(n2541), .B0(n2497), .B1(n2354), 
        .Y(n2595) );
  INV_X0P5B_A12TR u2734 ( .A(n305), .Y(n2354) );
  INV_X0P5B_A12TR u2735 ( .A(n303), .Y(n2541) );
  INV_X0P5B_A12TR u2736 ( .A(regfile[4]), .Y(n2250) );
  INV_X0P5B_A12TR u2737 ( .A(program_ctr[12]), .Y(n2588) );
  OAI211_X0P5M_A12TR u2738 ( .A0(n2483), .A1(n2596), .B0(n2597), .C0(n2598), 
        .Y(address[11]) );
  AOI222_X0P5M_A12TR u2739 ( .A0(isr_addr[11]), .A1(n2487), .B0(operand2[3]), 
        .B1(n2193), .C0(n1256), .C1(n21691), .Y(n2598) );
  AOI22_X0P5M_A12TR u2740 ( .A0(n2599), .A1(n2489), .B0(stack_ptr[11]), .B1(
        n2189), .Y(n2597) );
  INV_X0P5B_A12TR u2741 ( .A(n2179), .Y(n2599) );
  MXIT2_X0P5M_A12TR u2742 ( .A(n2600), .B(n2601), .S0(n321), .Y(n2179) );
  OAI221_X0P5M_A12TR u2743 ( .A0(n314), .A1(n2492), .B0(n15901), .B1(n2493), 
        .C0(n2602), .Y(n2601) );
  AOI22_X0P5M_A12TR u2744 ( .A0(n2495), .A1(n25481), .B0(n2497), .B1(n2350), 
        .Y(n2602) );
  INV_X0P5B_A12TR u2745 ( .A(n312), .Y(n2350) );
  INV_X0P5B_A12TR u2746 ( .A(n310), .Y(n25481) );
  INV_X0P5B_A12TR u2747 ( .A(regfile[11]), .Y(n15901) );
  OAI221_X0P5M_A12TR u2748 ( .A0(n313), .A1(n2492), .B0(n2261), .B1(n2493), 
        .C0(n2603), .Y(n2600) );
  AOI22_X0P5M_A12TR u2749 ( .A0(n2495), .A1(n2549), .B0(n2497), .B1(n2348), 
        .Y(n2603) );
  INV_X0P5B_A12TR u2750 ( .A(n311), .Y(n2348) );
  INV_X0P5B_A12TR u2751 ( .A(n309), .Y(n2549) );
  INV_X0P5B_A12TR u2752 ( .A(regfile[3]), .Y(n2261) );
  INV_X0P5B_A12TR u2753 ( .A(program_ctr[11]), .Y(n2596) );
  OAI211_X0P5M_A12TR u2754 ( .A0(n2483), .A1(n2604), .B0(n2605), .C0(n2606), 
        .Y(address[10]) );
  AOI222_X0P5M_A12TR u2755 ( .A0(isr_addr[10]), .A1(n2487), .B0(operand2[2]), 
        .B1(n2193), .C0(n1255), .C1(n21691), .Y(n2606) );
  AOI22_X0P5M_A12TR u2756 ( .A0(n2607), .A1(n2489), .B0(stack_ptr[10]), .B1(
        n2189), .Y(n2605) );
  INV_X0P5B_A12TR u2757 ( .A(n2180), .Y(n2607) );
  MXIT2_X0P5M_A12TR u2758 ( .A(n2608), .B(n2609), .S0(n321), .Y(n2180) );
  OAI221_X0P5M_A12TR u2759 ( .A0(n320), .A1(n2492), .B0(n15961), .B1(n2493), 
        .C0(n2610), .Y(n2609) );
  AOI22_X0P5M_A12TR u2760 ( .A0(n2495), .A1(n2556), .B0(n2497), .B1(n2344), 
        .Y(n2610) );
  INV_X0P5B_A12TR u2761 ( .A(n318), .Y(n2344) );
  INV_X0P5B_A12TR u2762 ( .A(n316), .Y(n2556) );
  INV_X0P5B_A12TR u2763 ( .A(regfile[10]), .Y(n15961) );
  OAI221_X0P5M_A12TR u2764 ( .A0(n319), .A1(n2492), .B0(n2271), .B1(n2493), 
        .C0(n2611), .Y(n2608) );
  AOI22_X0P5M_A12TR u2765 ( .A0(n2495), .A1(n2557), .B0(n2497), .B1(n2342), 
        .Y(n2611) );
  INV_X0P5B_A12TR u2766 ( .A(n317), .Y(n2342) );
  NOR2_X0P5A_A12TR u2767 ( .A(n2612), .B(n323), .Y(n2497) );
  INV_X0P5B_A12TR u2768 ( .A(n315), .Y(n2557) );
  NOR2_X0P5A_A12TR u2769 ( .A(n322), .B(n323), .Y(n2495) );
  NAND2_X0P5A_A12TR u2770 ( .A(n323), .B(n322), .Y(n2493) );
  INV_X0P5B_A12TR u2771 ( .A(regfile[2]), .Y(n2271) );
  NAND2_X0P5A_A12TR u2772 ( .A(n323), .B(n2612), .Y(n2492) );
  INV_X0P5B_A12TR u2773 ( .A(n322), .Y(n2612) );
  INV_X0P5B_A12TR u2774 ( .A(program_ctr[10]), .Y(n2604) );
  OAI211_X0P5M_A12TR u2775 ( .A0(n2483), .A1(n2285), .B0(n2613), .C0(n2614), 
        .Y(address[0]) );
  AOI222_X0P5M_A12TR u2776 ( .A0(isr_addr[0]), .A1(n2487), .B0(offset_sx[0]), 
        .B1(n2193), .C0(n1245), .C1(n21691), .Y(n2614) );
  AOI22_X0P5M_A12TR u2777 ( .A0(stack_ptr[0]), .A1(n2189), .B0(n2182), .B1(
        n2489), .Y(n2613) );
  NAND4_X0P5A_A12TR u2778 ( .A(n2615), .B(n2616), .C(n2617), .D(n2618), .Y(
        n2182) );
  AOI22_X0P5M_A12TR u2779 ( .A0(n2480), .A1(regfile[8]), .B0(n2433), .B1(
        regfile[0]), .Y(n2618) );
  NOR2B_X0P5M_A12TR u2780 ( .AN(n1860), .B(n1707), .Y(n2433) );
  INV_X0P5B_A12TR u2781 ( .A(n2145), .Y(n2480) );
  NAND2_X0P5A_A12TR u2782 ( .A(n1860), .B(n1707), .Y(n2145) );
  NOR2_X0P5A_A12TR u2783 ( .A(reg0[1]), .B(reg0[2]), .Y(n1860) );
  AOI22_X0P5M_A12TR u2784 ( .A0(n1863), .A1(n2330), .B0(n1858), .B1(n2507), 
        .Y(n2617) );
  INV_X0P5B_A12TR u2785 ( .A(n267), .Y(n2507) );
  NOR3_X0P5A_A12TR u2786 ( .A(n1708), .B(reg0[0]), .C(n1709), .Y(n1858) );
  INV_X0P5B_A12TR u2787 ( .A(n268), .Y(n2330) );
  NOR3_X0P5A_A12TR u2788 ( .A(n1709), .B(reg0[1]), .C(n1707), .Y(n1863) );
  AOI22_X0P5M_A12TR u2789 ( .A0(n1722), .A1(n2332), .B0(n1861), .B1(n2509), 
        .Y(n2616) );
  INV_X0P5B_A12TR u2790 ( .A(n266), .Y(n2509) );
  NOR3_X0P5A_A12TR u2791 ( .A(n1709), .B(n1708), .C(n1707), .Y(n1861) );
  INV_X0P5B_A12TR u2792 ( .A(n269), .Y(n2332) );
  NOR3_X0P5A_A12TR u2793 ( .A(reg0[0]), .B(reg0[1]), .C(n1709), .Y(n1722) );
  INV_X0P5B_A12TR u2794 ( .A(reg0[2]), .Y(n1709) );
  AOI22_X0P5M_A12TR u2795 ( .A0(n1859), .A1(n2323), .B0(n1862), .B1(n2322), 
        .Y(n2615) );
  INV_X0P5B_A12TR u2796 ( .A(n271), .Y(n2322) );
  NOR3_X0P5A_A12TR u2797 ( .A(reg0[0]), .B(reg0[2]), .C(n1708), .Y(n1862) );
  INV_X0P5B_A12TR u2798 ( .A(n270), .Y(n2323) );
  NOR3_X0P5A_A12TR u2799 ( .A(n1708), .B(reg0[2]), .C(n1707), .Y(n1859) );
  INV_X0P5B_A12TR u2800 ( .A(reg0[0]), .Y(n1707) );
  INV_X0P5B_A12TR u2801 ( .A(reg0[1]), .Y(n1708) );
  NAND3B_X0P5M_A12TR u2802 ( .AN(n2619), .B(n2438), .C(n2147), .Y(n2189) );
  INV_X0P5B_A12TR u2803 ( .A(n2144), .Y(n2147) );
  NAND4_X0P5A_A12TR u2804 ( .A(n1737), .B(n2301), .C(n2460), .D(n2136), .Y(
        n2144) );
  INV_X0P5B_A12TR u2805 ( .A(program_ctr[0]), .Y(n2285) );
  OA21A1OI2_X0P5M_A12TR u2806 ( .A0(n2620), .A1(n2621), .B0(n21571), .C0(n2622), .Y(n2483) );
  NAND3_X0P5A_A12TR u2807 ( .A(n2427), .B(n2454), .C(n2434), .Y(n2621) );
  OR3_X0P5M_A12TR u2808 ( .A(n2312), .B(n21611), .C(n2623), .Y(n2620) );
  NAND3_X0P5A_A12TR u2809 ( .A(opcode[4]), .B(n2130), .C(n2412), .Y(n21611) );
  NAND2_X0P5A_A12TR u2810 ( .A(n2624), .B(n2625), .Y(n2130) );
  OR6_X0P5M_A12TR u2811 ( .A(n1719), .B(n2489), .C(n2193), .D(n2622), .E(n2299), .F(n2626), .Y(alu_ctrl[1]) );
  NAND4B_X0P5M_A12TR u2812 ( .AN(n2627), .B(n16971), .C(n2146), .D(n2301), .Y(
        n2626) );
  NAND2_X0P5A_A12TR u2813 ( .A(n2628), .B(n2629), .Y(n2301) );
  INV_X0P5B_A12TR u2814 ( .A(n1723), .Y(n2146) );
  NOR2_X0P5A_A12TR u2815 ( .A(n2129), .B(n2454), .Y(n1723) );
  NAND3_X0P5A_A12TR u2816 ( .A(opcode[1]), .B(opcode[0]), .C(n2625), .Y(n2454)
         );
  AO1B2_X0P5M_A12TR u2817 ( .B0(n2412), .B1(n2456), .A0N(n2409), .Y(n16971) );
  INV_X0P5B_A12TR u2818 ( .A(n2623), .Y(n2456) );
  NAND3_X0P5A_A12TR u2819 ( .A(n2417), .B(n21641), .C(n2295), .Y(n2623) );
  NAND2_X0P5A_A12TR u2820 ( .A(n2630), .B(n2631), .Y(n2295) );
  NAND3_X0P5A_A12TR u2821 ( .A(n1704), .B(n1705), .C(n2624), .Y(n21641) );
  NAND3_X0P5A_A12TR u2822 ( .A(n1704), .B(n1705), .C(n2632), .Y(n2417) );
  AND2_X0P5M_A12TR u2823 ( .A(n2468), .B(n2313), .Y(n2412) );
  NAND2_X0P5A_A12TR u2824 ( .A(n2625), .B(n2631), .Y(n2313) );
  OAI211_X0P5M_A12TR u2825 ( .A0(n2434), .A1(n2129), .B0(n21651), .C0(n1737), 
        .Y(n2627) );
  NAND2_X0P5A_A12TR u2826 ( .A(n2448), .B(n2474), .Y(n1737) );
  INV_X0P5B_A12TR u2827 ( .A(n21691), .Y(n21651) );
  NAND2_X0P5A_A12TR u2828 ( .A(n2406), .B(n16981), .Y(n21691) );
  NAND2_X0P5A_A12TR u2829 ( .A(n2482), .B(n2629), .Y(n16981) );
  NAND3_X0P5A_A12TR u2830 ( .A(n2629), .B(n2633), .C(n2634), .Y(n2406) );
  NAND2_X0P5A_A12TR u2831 ( .A(n2625), .B(n2632), .Y(n2434) );
  NOR2_X0P5A_A12TR u2832 ( .A(n1704), .B(opcode[3]), .Y(n2625) );
  NAND3_X0P5A_A12TR u2833 ( .A(n2460), .B(n2438), .C(n2387), .Y(n2299) );
  NOR3_X0P5A_A12TR u2834 ( .A(n2619), .B(n16991), .C(n2487), .Y(n2387) );
  OAI21_X0P5M_A12TR u2835 ( .A0(n2126), .A1(n2294), .B0(n1930), .Y(n2487) );
  NAND2_X0P5A_A12TR u2836 ( .A(n2628), .B(n2385), .Y(n1930) );
  INV_X0P5B_A12TR u2837 ( .A(n2385), .Y(n2126) );
  INV_X0P5B_A12TR u2838 ( .A(n1718), .Y(n16991) );
  NAND3B_X0P5M_A12TR u2839 ( .AN(n1730), .B(n2423), .C(n2141), .Y(n2619) );
  NAND2_X0P5A_A12TR u2840 ( .A(n2448), .B(n2629), .Y(n2141) );
  INV_X0P5B_A12TR u2841 ( .A(n2294), .Y(n2448) );
  NAND2_X0P5A_A12TR u2842 ( .A(n2628), .B(n2635), .Y(n2423) );
  NOR2_X0P5A_A12TR u2843 ( .A(n2294), .B(n2444), .Y(n1730) );
  NAND3_X0P5A_A12TR u2844 ( .A(cpu_state[5]), .B(cpu_state[0]), .C(n2636), .Y(
        n2294) );
  NAND2_X0P5A_A12TR u2845 ( .A(n2474), .B(n2386), .Y(n2438) );
  NAND2_X0P5A_A12TR u2846 ( .A(n2637), .B(n2474), .Y(n2460) );
  NAND2B_X0P5M_A12TR u2847 ( .AN(n2192), .B(n2303), .Y(n2622) );
  AND4_X0P5M_A12TR u2848 ( .A(n2638), .B(n2639), .C(n2186), .D(n2184), .Y(
        n2303) );
  NAND2_X0P5A_A12TR u2849 ( .A(n2449), .B(n2629), .Y(n2184) );
  NAND2_X0P5A_A12TR u2850 ( .A(n2463), .B(n2629), .Y(n2186) );
  OAI21_X0P5M_A12TR u2851 ( .A0(n2463), .A1(n2449), .B0(n2385), .Y(n2639) );
  NAND2_X0P5A_A12TR u2852 ( .A(n2463), .B(n2474), .Y(n2638) );
  NAND4B_X0P5M_A12TR u2853 ( .AN(n2640), .B(n2302), .C(n1739), .D(n2300), .Y(
        n2192) );
  AOI21_X0P5M_A12TR u2854 ( .A0(n2635), .A1(n2449), .B0(n2452), .Y(n2300) );
  AND3_X0P5M_A12TR u2855 ( .A(cpu_state[5]), .B(cpu_state[0]), .C(n2407), .Y(
        n2452) );
  INV_X0P5B_A12TR u2856 ( .A(n1724), .Y(n1739) );
  OAI221_X0P5M_A12TR u2857 ( .A0(n2641), .A1(n2642), .B0(n2444), .B1(n2445), 
        .C0(n2390), .Y(n1724) );
  NAND3_X0P5A_A12TR u2858 ( .A(cpu_state[5]), .B(n2643), .C(n2407), .Y(n2390)
         );
  INV_X0P5B_A12TR u2859 ( .A(n2463), .Y(n2445) );
  NOR2_X0P5A_A12TR u2860 ( .A(n2644), .B(n2641), .Y(n2463) );
  AND4_X0P5M_A12TR u2861 ( .A(n2645), .B(n2469), .C(n2646), .D(n2446), .Y(
        n2302) );
  AND3_X0P5M_A12TR u2862 ( .A(n16961), .B(n1728), .C(n2138), .Y(n2446) );
  OAI31_X0P5M_A12TR u2863 ( .A0(n2482), .A1(n2637), .A2(n2386), .B0(n2385), 
        .Y(n2138) );
  NAND2_X0P5A_A12TR u2864 ( .A(n2386), .B(n2629), .Y(n1728) );
  NAND2_X0P5A_A12TR u2865 ( .A(n2474), .B(n2647), .Y(n16961) );
  NOR2_X0P5A_A12TR u2866 ( .A(n2375), .B(n2374), .Y(n2646) );
  INV_X0P5B_A12TR u2867 ( .A(n2481), .Y(n2374) );
  AOI21_X0P5M_A12TR u2868 ( .A0(n2629), .A1(n2648), .B0(n2649), .Y(n2481) );
  NAND2_X0P5A_A12TR u2869 ( .A(n2462), .B(n2451), .Y(n2375) );
  NAND2_X0P5A_A12TR u2870 ( .A(n2635), .B(n2647), .Y(n2451) );
  NAND2_X0P5A_A12TR u2871 ( .A(n2647), .B(n2629), .Y(n2462) );
  INV_X0P5B_A12TR u2872 ( .A(n2419), .Y(n2469) );
  OAI21_X0P5M_A12TR u2873 ( .A0(n2641), .A1(n2308), .B0(n2424), .Y(n2419) );
  NAND2_X0P5A_A12TR u2874 ( .A(n2648), .B(n2635), .Y(n2424) );
  INV_X0P5B_A12TR u2875 ( .A(n2132), .Y(n2648) );
  NAND2_X0P5A_A12TR u2876 ( .A(n2634), .B(n2408), .Y(n2132) );
  AND4_X0P5M_A12TR u2877 ( .A(n2405), .B(n1733), .C(n2436), .D(n2137), .Y(
        n2645) );
  NAND2_X0P5A_A12TR u2878 ( .A(n2637), .B(n2629), .Y(n2436) );
  INV_X0P5B_A12TR u2879 ( .A(n2131), .Y(n2629) );
  NAND2B_X0P5M_A12TR u2880 ( .AN(cpu_state[1]), .B(cpu_state[2]), .Y(n2131) );
  NAND3_X0P5A_A12TR u2881 ( .A(n2634), .B(n2633), .C(n2635), .Y(n1733) );
  NAND2_X0P5A_A12TR u2882 ( .A(n2650), .B(n2633), .Y(n2405) );
  NAND4_X0P5A_A12TR u2883 ( .A(n2307), .B(n2326), .C(n2308), .D(n2437), .Y(
        n2640) );
  NAND2_X0P5A_A12TR u2884 ( .A(n2385), .B(n2647), .Y(n2326) );
  AND3_X0P5M_A12TR u2885 ( .A(n2633), .B(n2651), .C(cpu_state[4]), .Y(n2647)
         );
  OAI211_X0P5M_A12TR u2886 ( .A0(cpu_state[4]), .A1(cpu_state[2]), .B0(n2644), 
        .C0(cpu_state[5]), .Y(n2307) );
  NAND2_X0P5A_A12TR u2887 ( .A(n2305), .B(n2309), .Y(n2193) );
  NAND2_X0P5A_A12TR u2888 ( .A(n2407), .B(n2633), .Y(n2309) );
  INV_X0P5B_A12TR u2889 ( .A(n2642), .Y(n2407) );
  NAND2_X0P5A_A12TR u2890 ( .A(n2634), .B(n2385), .Y(n2642) );
  NOR2_X0P5A_A12TR u2891 ( .A(cpu_state[1]), .B(cpu_state[2]), .Y(n2385) );
  NAND2_X0P5A_A12TR u2892 ( .A(n2482), .B(n2474), .Y(n2305) );
  OAI21_X0P5M_A12TR u2893 ( .A0(n2129), .A1(n2416), .B0(n2194), .Y(n2489) );
  NAND2_X0P5A_A12TR u2894 ( .A(n2482), .B(n2635), .Y(n2194) );
  NOR3_X0P5A_A12TR u2895 ( .A(n2652), .B(cpu_state[3]), .C(n2641), .Y(n2482)
         );
  NAND2_X0P5A_A12TR u2896 ( .A(n2653), .B(n2631), .Y(n2416) );
  NOR2_X0P5A_A12TR u2897 ( .A(n1703), .B(opcode[0]), .Y(n2631) );
  INV_X0P5B_A12TR u2898 ( .A(opcode[1]), .Y(n1703) );
  INV_X0P5B_A12TR u2899 ( .A(n2409), .Y(n2129) );
  AO1B2_X0P5M_A12TR u2900 ( .B0(n2312), .B1(n2409), .A0N(n2296), .Y(n1719) );
  NAND2B_X0P5M_A12TR u2901 ( .AN(n2427), .B(n2409), .Y(n2296) );
  NAND3_X0P5A_A12TR u2902 ( .A(opcode[1]), .B(opcode[0]), .C(n2630), .Y(n2427)
         );
  NOR2_X0P5A_A12TR u2903 ( .A(n2195), .B(n1706), .Y(n2409) );
  INV_X0P5B_A12TR u2904 ( .A(opcode[4]), .Y(n1706) );
  NAND3_X0P5A_A12TR u2905 ( .A(n2414), .B(n21631), .C(n2465), .Y(n2312) );
  AND2_X0P5M_A12TR u2906 ( .A(n2418), .B(n2396), .Y(n2465) );
  NAND2_X0P5A_A12TR u2907 ( .A(n2630), .B(n2632), .Y(n2396) );
  NAND3_X0P5A_A12TR u2908 ( .A(opcode[1]), .B(opcode[0]), .C(n2653), .Y(n2418)
         );
  NAND3_X0P5A_A12TR u2909 ( .A(n1704), .B(n1705), .C(opcode[1]), .Y(n21631) );
  AOI21B_X0P5M_A12TR u2910 ( .A0(n2632), .A1(n2653), .B0N(n21621), .Y(n2414)
         );
  NAND2_X0P5A_A12TR u2911 ( .A(n2624), .B(n2653), .Y(n21621) );
  NOR2_X0P5A_A12TR u2912 ( .A(n1705), .B(n1704), .Y(n2653) );
  INV_X0P5B_A12TR u2913 ( .A(opcode[2]), .Y(n1704) );
  NOR2_X0P5A_A12TR u2914 ( .A(n17021), .B(opcode[1]), .Y(n2632) );
  INV_X0P5B_A12TR u2915 ( .A(n2035), .Y(alu_ctrl[0]) );
  OAI31_X0P5M_A12TR u2916 ( .A0(n2654), .A1(n2666), .A2(n2649), .B0(n1718), 
        .Y(n2035) );
  OAI21_X0P5M_A12TR u2917 ( .A0(n21571), .A1(n2471), .B0(int_req), .Y(n1718)
         );
  INV_X0P5B_A12TR u2918 ( .A(n2437), .Y(n2471) );
  NAND2_X0P5A_A12TR u2919 ( .A(n2635), .B(n2386), .Y(n2437) );
  AND3_X0P5M_A12TR u2920 ( .A(cpu_state[3]), .B(n2633), .C(cpu_state[4]), .Y(
        n2386) );
  INV_X0P5B_A12TR u2921 ( .A(n2444), .Y(n2635) );
  NOR2_X0P5A_A12TR u2922 ( .A(n2127), .B(n2444), .Y(n2649) );
  NAND2_X0P5A_A12TR u2923 ( .A(cpu_state[1]), .B(cpu_state[2]), .Y(n2444) );
  INV_X0P5B_A12TR u2924 ( .A(n2637), .Y(n2127) );
  NOR3_X0P5A_A12TR u2925 ( .A(n2652), .B(n2651), .C(n2641), .Y(n2637) );
  INV_X0P5B_A12TR u2926 ( .A(n2408), .Y(n2641) );
  NOR2_X0P5A_A12TR u2927 ( .A(cpu_state[0]), .B(cpu_state[5]), .Y(n2408) );
  INV_X0P5B_A12TR u2928 ( .A(cpu_state[4]), .Y(n2652) );
  INV_X0P5B_A12TR u2929 ( .A(n2137), .Y(n2666) );
  NAND3_X0P5A_A12TR u2930 ( .A(n2650), .B(n2643), .C(cpu_state[5]), .Y(n2137)
         );
  INV_X0P5B_A12TR u2931 ( .A(n2308), .Y(n2650) );
  NAND2_X0P5A_A12TR u2932 ( .A(n2634), .B(n2474), .Y(n2308) );
  NOR2_X0P5A_A12TR u2933 ( .A(n2651), .B(cpu_state[4]), .Y(n2634) );
  INV_X0P5B_A12TR u2934 ( .A(cpu_state[3]), .Y(n2651) );
  OAI21_X0P5M_A12TR u2935 ( .A0(n17021), .A1(n2125), .B0(n2136), .Y(n2654) );
  NAND2_X0P5A_A12TR u2936 ( .A(n2628), .B(n2474), .Y(n2136) );
  AND3_X0P5M_A12TR u2937 ( .A(cpu_state[5]), .B(n2643), .C(n2636), .Y(n2628)
         );
  AO1B2_X0P5M_A12TR u2938 ( .B0(n2468), .B1(opcode[4]), .A0N(n21571), .Y(n2125) );
  INV_X0P5B_A12TR u2939 ( .A(n2195), .Y(n21571) );
  NAND2_X0P5A_A12TR u2940 ( .A(n2449), .B(n2474), .Y(n2195) );
  NOR2B_X0P5M_A12TR u2941 ( .AN(cpu_state[1]), .B(cpu_state[2]), .Y(n2474) );
  NOR2B_X0P5M_A12TR u2942 ( .AN(n2633), .B(n2644), .Y(n2449) );
  INV_X0P5B_A12TR u2943 ( .A(n2636), .Y(n2644) );
  NOR2_X0P5A_A12TR u2944 ( .A(cpu_state[3]), .B(cpu_state[4]), .Y(n2636) );
  NOR2_X0P5A_A12TR u2945 ( .A(n2643), .B(cpu_state[5]), .Y(n2633) );
  INV_X0P5B_A12TR u2946 ( .A(cpu_state[0]), .Y(n2643) );
  NAND2_X0P5A_A12TR u2947 ( .A(n2630), .B(n2624), .Y(n2468) );
  NOR2_X0P5A_A12TR u2948 ( .A(opcode[1]), .B(opcode[0]), .Y(n2624) );
  NOR2_X0P5A_A12TR u2949 ( .A(n1705), .B(opcode[2]), .Y(n2630) );
  INV_X0P5B_A12TR u2950 ( .A(opcode[3]), .Y(n1705) );
  INV_X0P5B_A12TR u2951 ( .A(opcode[0]), .Y(n17021) );
  INV_X0P5M_A12TR r1189_u2 ( .A(u4_u4_z_0), .Y(r1189_n2) );
  INV_X1M_A12TR r1189_u1 ( .A(stack_ptr[0]), .Y(n16870) );
  CGEN_X1M_A12TR r1189_u1_0 ( .A(stack_ptr[0]), .B(u4_u4_z_0), .CI(r1189_n2), 
        .CO(r1189_carry[1]) );
  ADDF_X1M_A12TR r1189_u1_1 ( .A(stack_ptr[1]), .B(u4_u4_z_0), .CI(
        r1189_carry[1]), .CO(r1189_carry[2]), .S(n16880) );
  ADDF_X1M_A12TR r1189_u1_2 ( .A(stack_ptr[2]), .B(u4_u4_z_0), .CI(
        r1189_carry[2]), .CO(r1189_carry[3]), .S(n16890) );
  ADDF_X1M_A12TR r1189_u1_3 ( .A(stack_ptr[3]), .B(u4_u4_z_0), .CI(
        r1189_carry[3]), .CO(r1189_carry[4]), .S(n16900) );
  ADDF_X1M_A12TR r1189_u1_4 ( .A(stack_ptr[4]), .B(u4_u4_z_0), .CI(
        r1189_carry[4]), .CO(r1189_carry[5]), .S(n16910) );
  ADDF_X1M_A12TR r1189_u1_5 ( .A(stack_ptr[5]), .B(u4_u4_z_0), .CI(
        r1189_carry[5]), .CO(r1189_carry[6]), .S(n16920) );
  ADDF_X1M_A12TR r1189_u1_6 ( .A(stack_ptr[6]), .B(u4_u4_z_0), .CI(
        r1189_carry[6]), .CO(r1189_carry[7]), .S(n16930) );
  ADDF_X1M_A12TR r1189_u1_7 ( .A(stack_ptr[7]), .B(u4_u4_z_0), .CI(
        r1189_carry[7]), .CO(r1189_carry[8]), .S(n16940) );
  ADDF_X1M_A12TR r1189_u1_8 ( .A(stack_ptr[8]), .B(u4_u4_z_0), .CI(
        r1189_carry[8]), .CO(r1189_carry[9]), .S(n16950) );
  ADDF_X1M_A12TR r1189_u1_9 ( .A(stack_ptr[9]), .B(u4_u4_z_0), .CI(
        r1189_carry[9]), .CO(r1189_carry[10]), .S(n16960) );
  ADDF_X1M_A12TR r1189_u1_10 ( .A(stack_ptr[10]), .B(u4_u4_z_0), .CI(
        r1189_carry[10]), .CO(r1189_carry[11]), .S(n16970) );
  ADDF_X1M_A12TR r1189_u1_11 ( .A(stack_ptr[11]), .B(u4_u4_z_0), .CI(
        r1189_carry[11]), .CO(r1189_carry[12]), .S(n16980) );
  ADDF_X1M_A12TR r1189_u1_12 ( .A(stack_ptr[12]), .B(u4_u4_z_0), .CI(
        r1189_carry[12]), .CO(r1189_carry[13]), .S(n16990) );
  ADDF_X1M_A12TR r1189_u1_13 ( .A(stack_ptr[13]), .B(u4_u4_z_0), .CI(
        r1189_carry[13]), .CO(r1189_carry[14]), .S(n17000) );
  ADDF_X1M_A12TR r1189_u1_14 ( .A(stack_ptr[14]), .B(u4_u4_z_0), .CI(
        r1189_carry[14]), .CO(r1189_carry[15]), .S(n17010) );
  ADDF_X1M_A12TR r1189_u1_15 ( .A(stack_ptr[15]), .B(u4_u4_z_0), .CI(
        r1189_carry[15]), .CO(), .S(n17020) );
  XOR2_X0P5M_A12TR add_1150_u2 ( .A(add_1150_carry[15]), .B(isr_addr[15]), .Y(
        n21710) );
  INV_X1M_A12TR add_1150_u1 ( .A(isr_addr[0]), .Y(n21560) );
  ADDH_X1M_A12TR add_1150_u1_1_1 ( .A(isr_addr[1]), .B(isr_addr[0]), .CO(
        add_1150_carry[2]), .S(n21570) );
  ADDH_X1M_A12TR add_1150_u1_1_2 ( .A(isr_addr[2]), .B(add_1150_carry[2]), 
        .CO(add_1150_carry[3]), .S(n21580) );
  ADDH_X1M_A12TR add_1150_u1_1_3 ( .A(isr_addr[3]), .B(add_1150_carry[3]), 
        .CO(add_1150_carry[4]), .S(n21590) );
  ADDH_X1M_A12TR add_1150_u1_1_4 ( .A(isr_addr[4]), .B(add_1150_carry[4]), 
        .CO(add_1150_carry[5]), .S(n21600) );
  ADDH_X1M_A12TR add_1150_u1_1_5 ( .A(isr_addr[5]), .B(add_1150_carry[5]), 
        .CO(add_1150_carry[6]), .S(n21610) );
  ADDH_X1M_A12TR add_1150_u1_1_6 ( .A(isr_addr[6]), .B(add_1150_carry[6]), 
        .CO(add_1150_carry[7]), .S(n21620) );
  ADDH_X1M_A12TR add_1150_u1_1_7 ( .A(isr_addr[7]), .B(add_1150_carry[7]), 
        .CO(add_1150_carry[8]), .S(n21630) );
  ADDH_X1M_A12TR add_1150_u1_1_8 ( .A(isr_addr[8]), .B(add_1150_carry[8]), 
        .CO(add_1150_carry[9]), .S(n21640) );
  ADDH_X1M_A12TR add_1150_u1_1_9 ( .A(isr_addr[9]), .B(add_1150_carry[9]), 
        .CO(add_1150_carry[10]), .S(n21650) );
  ADDH_X1M_A12TR add_1150_u1_1_10 ( .A(isr_addr[10]), .B(add_1150_carry[10]), 
        .CO(add_1150_carry[11]), .S(n21660) );
  ADDH_X1M_A12TR add_1150_u1_1_11 ( .A(isr_addr[11]), .B(add_1150_carry[11]), 
        .CO(add_1150_carry[12]), .S(n21670) );
  ADDH_X1M_A12TR add_1150_u1_1_12 ( .A(isr_addr[12]), .B(add_1150_carry[12]), 
        .CO(add_1150_carry[13]), .S(n21680) );
  ADDH_X1M_A12TR add_1150_u1_1_13 ( .A(isr_addr[13]), .B(add_1150_carry[13]), 
        .CO(add_1150_carry[14]), .S(n21690) );
  ADDH_X1M_A12TR add_1150_u1_1_14 ( .A(isr_addr[14]), .B(add_1150_carry[14]), 
        .CO(add_1150_carry[15]), .S(n21700) );
  ADDF_X1M_A12TR r423_u1_0 ( .A(u4_u6_z_0), .B(u4_u7_z_0), .CI(u4_u8_z_0), 
        .CO(r423_carry[1]), .S(n80) );
  ADDF_X1M_A12TR r423_u1_1 ( .A(u4_u6_z_1), .B(u4_u7_z_1), .CI(r423_carry[1]), 
        .CO(r423_carry[2]), .S(n79) );
  ADDF_X1M_A12TR r423_u1_2 ( .A(u4_u6_z_2), .B(u4_u7_z_2), .CI(r423_carry[2]), 
        .CO(r423_carry[3]), .S(n78) );
  ADDF_X1M_A12TR r423_u1_3 ( .A(u4_u6_z_3), .B(u4_u7_z_3), .CI(r423_carry[3]), 
        .CO(r423_carry[4]), .S(n77) );
  ADDF_X1M_A12TR r423_u1_4 ( .A(u4_u6_z_4), .B(u4_u7_z_4), .CI(r423_carry[4]), 
        .CO(r423_carry[5]), .S(n76) );
  ADDF_X1M_A12TR r423_u1_5 ( .A(u4_u6_z_5), .B(u4_u7_z_5), .CI(r423_carry[5]), 
        .CO(r423_carry[6]), .S(n75) );
  ADDF_X1M_A12TR r423_u1_6 ( .A(u4_u6_z_6), .B(u4_u7_z_6), .CI(r423_carry[6]), 
        .CO(r423_carry[7]), .S(n74) );
  ADDF_X1M_A12TR r423_u1_7 ( .A(u4_u6_z_7), .B(u4_u7_z_7), .CI(r423_carry[7]), 
        .CO(n72), .S(n73) );
  INV_X1M_A12TR mult_890_u155 ( .A(regfile[10]), .Y(mult_890_n175) );
  INV_X1M_A12TR mult_890_u154 ( .A(regfile[11]), .Y(mult_890_n174) );
  INV_X1M_A12TR mult_890_u153 ( .A(regfile[14]), .Y(mult_890_n171) );
  INV_X1M_A12TR mult_890_u152 ( .A(regfile[12]), .Y(mult_890_n173) );
  INV_X1M_A12TR mult_890_u151 ( .A(regfile[9]), .Y(mult_890_n176) );
  INV_X1M_A12TR mult_890_u150 ( .A(regfile[8]), .Y(mult_890_n177) );
  INV_X1M_A12TR mult_890_u149 ( .A(regfile[13]), .Y(mult_890_n172) );
  INV_X1M_A12TR mult_890_u148 ( .A(regfile[15]), .Y(mult_890_n170) );
  INV_X1M_A12TR mult_890_u147 ( .A(n2798), .Y(mult_890_n166) );
  INV_X1M_A12TR mult_890_u146 ( .A(n2800), .Y(mult_890_n168) );
  INV_X1M_A12TR mult_890_u145 ( .A(n2799), .Y(mult_890_n167) );
  INV_X1M_A12TR mult_890_u144 ( .A(n2794), .Y(mult_890_n162) );
  INV_X1M_A12TR mult_890_u143 ( .A(n2795), .Y(mult_890_n163) );
  INV_X1M_A12TR mult_890_u142 ( .A(n2796), .Y(mult_890_n164) );
  INV_X1M_A12TR mult_890_u141 ( .A(n2797), .Y(mult_890_n165) );
  INV_X1M_A12TR mult_890_u140 ( .A(n2801), .Y(mult_890_n169) );
  NOR2_X1M_A12TR mult_890_u121 ( .A(mult_890_n169), .B(mult_890_n177), .Y(
        n14660) );
  NOR2_X1M_A12TR mult_890_u120 ( .A(mult_890_n168), .B(mult_890_n177), .Y(
        mult_890_n161) );
  NOR2_X1M_A12TR mult_890_u119 ( .A(mult_890_n167), .B(mult_890_n177), .Y(
        mult_890_n160) );
  NOR2_X1M_A12TR mult_890_u118 ( .A(mult_890_n166), .B(mult_890_n177), .Y(
        mult_890_n159) );
  NOR2_X1M_A12TR mult_890_u117 ( .A(mult_890_n165), .B(mult_890_n177), .Y(
        mult_890_n158) );
  NOR2_X1M_A12TR mult_890_u116 ( .A(mult_890_n164), .B(mult_890_n177), .Y(
        mult_890_n157) );
  NOR2_X1M_A12TR mult_890_u115 ( .A(mult_890_n163), .B(mult_890_n177), .Y(
        mult_890_n156) );
  NOR2_X1M_A12TR mult_890_u114 ( .A(mult_890_n162), .B(mult_890_n177), .Y(
        mult_890_n155) );
  NOR2_X1M_A12TR mult_890_u113 ( .A(mult_890_n169), .B(mult_890_n176), .Y(
        mult_890_n154) );
  NOR2_X1M_A12TR mult_890_u112 ( .A(mult_890_n168), .B(mult_890_n176), .Y(
        mult_890_n153) );
  NOR2_X1M_A12TR mult_890_u111 ( .A(mult_890_n167), .B(mult_890_n176), .Y(
        mult_890_n152) );
  NOR2_X1M_A12TR mult_890_u110 ( .A(mult_890_n166), .B(mult_890_n176), .Y(
        mult_890_n151) );
  NOR2_X1M_A12TR mult_890_u109 ( .A(mult_890_n165), .B(mult_890_n176), .Y(
        mult_890_n150) );
  NOR2_X1M_A12TR mult_890_u108 ( .A(mult_890_n164), .B(mult_890_n176), .Y(
        mult_890_n149) );
  NOR2_X1M_A12TR mult_890_u107 ( .A(mult_890_n163), .B(mult_890_n176), .Y(
        mult_890_n148) );
  NOR2_X1M_A12TR mult_890_u106 ( .A(mult_890_n162), .B(mult_890_n176), .Y(
        mult_890_n147) );
  NOR2_X1M_A12TR mult_890_u105 ( .A(mult_890_n169), .B(mult_890_n175), .Y(
        mult_890_n146) );
  NOR2_X1M_A12TR mult_890_u104 ( .A(mult_890_n168), .B(mult_890_n175), .Y(
        mult_890_n145) );
  NOR2_X1M_A12TR mult_890_u103 ( .A(mult_890_n167), .B(mult_890_n175), .Y(
        mult_890_n144) );
  NOR2_X1M_A12TR mult_890_u102 ( .A(mult_890_n166), .B(mult_890_n175), .Y(
        mult_890_n143) );
  NOR2_X1M_A12TR mult_890_u101 ( .A(mult_890_n165), .B(mult_890_n175), .Y(
        mult_890_n142) );
  NOR2_X1M_A12TR mult_890_u100 ( .A(mult_890_n164), .B(mult_890_n175), .Y(
        mult_890_n141) );
  NOR2_X1M_A12TR mult_890_u99 ( .A(mult_890_n163), .B(mult_890_n175), .Y(
        mult_890_n140) );
  NOR2_X1M_A12TR mult_890_u98 ( .A(mult_890_n162), .B(mult_890_n175), .Y(
        mult_890_n139) );
  NOR2_X1M_A12TR mult_890_u97 ( .A(mult_890_n169), .B(mult_890_n174), .Y(
        mult_890_n138) );
  NOR2_X1M_A12TR mult_890_u96 ( .A(mult_890_n168), .B(mult_890_n174), .Y(
        mult_890_n137) );
  NOR2_X1M_A12TR mult_890_u95 ( .A(mult_890_n167), .B(mult_890_n174), .Y(
        mult_890_n136) );
  NOR2_X1M_A12TR mult_890_u94 ( .A(mult_890_n166), .B(mult_890_n174), .Y(
        mult_890_n135) );
  NOR2_X1M_A12TR mult_890_u93 ( .A(mult_890_n165), .B(mult_890_n174), .Y(
        mult_890_n134) );
  NOR2_X1M_A12TR mult_890_u92 ( .A(mult_890_n164), .B(mult_890_n174), .Y(
        mult_890_n133) );
  NOR2_X1M_A12TR mult_890_u91 ( .A(mult_890_n163), .B(mult_890_n174), .Y(
        mult_890_n132) );
  NOR2_X1M_A12TR mult_890_u90 ( .A(mult_890_n162), .B(mult_890_n174), .Y(
        mult_890_n131) );
  NOR2_X1M_A12TR mult_890_u89 ( .A(mult_890_n169), .B(mult_890_n173), .Y(
        mult_890_n130) );
  NOR2_X1M_A12TR mult_890_u88 ( .A(mult_890_n168), .B(mult_890_n173), .Y(
        mult_890_n129) );
  NOR2_X1M_A12TR mult_890_u87 ( .A(mult_890_n167), .B(mult_890_n173), .Y(
        mult_890_n128) );
  NOR2_X1M_A12TR mult_890_u86 ( .A(mult_890_n166), .B(mult_890_n173), .Y(
        mult_890_n127) );
  NOR2_X1M_A12TR mult_890_u85 ( .A(mult_890_n165), .B(mult_890_n173), .Y(
        mult_890_n126) );
  NOR2_X1M_A12TR mult_890_u84 ( .A(mult_890_n164), .B(mult_890_n173), .Y(
        mult_890_n125) );
  NOR2_X1M_A12TR mult_890_u83 ( .A(mult_890_n163), .B(mult_890_n173), .Y(
        mult_890_n124) );
  NOR2_X1M_A12TR mult_890_u82 ( .A(mult_890_n162), .B(mult_890_n173), .Y(
        mult_890_n123) );
  NOR2_X1M_A12TR mult_890_u81 ( .A(mult_890_n169), .B(mult_890_n172), .Y(
        mult_890_n122) );
  NOR2_X1M_A12TR mult_890_u80 ( .A(mult_890_n168), .B(mult_890_n172), .Y(
        mult_890_n121) );
  NOR2_X1M_A12TR mult_890_u79 ( .A(mult_890_n167), .B(mult_890_n172), .Y(
        mult_890_n120) );
  NOR2_X1M_A12TR mult_890_u78 ( .A(mult_890_n166), .B(mult_890_n172), .Y(
        mult_890_n119) );
  NOR2_X1M_A12TR mult_890_u77 ( .A(mult_890_n165), .B(mult_890_n172), .Y(
        mult_890_n118) );
  NOR2_X1M_A12TR mult_890_u76 ( .A(mult_890_n164), .B(mult_890_n172), .Y(
        mult_890_n117) );
  NOR2_X1M_A12TR mult_890_u75 ( .A(mult_890_n163), .B(mult_890_n172), .Y(
        mult_890_n116) );
  NOR2_X1M_A12TR mult_890_u74 ( .A(mult_890_n162), .B(mult_890_n172), .Y(
        mult_890_n115) );
  NOR2_X1M_A12TR mult_890_u73 ( .A(mult_890_n169), .B(mult_890_n171), .Y(
        mult_890_n114) );
  NOR2_X1M_A12TR mult_890_u72 ( .A(mult_890_n168), .B(mult_890_n171), .Y(
        mult_890_n113) );
  NOR2_X1M_A12TR mult_890_u71 ( .A(mult_890_n167), .B(mult_890_n171), .Y(
        mult_890_n112) );
  NOR2_X1M_A12TR mult_890_u70 ( .A(mult_890_n166), .B(mult_890_n171), .Y(
        mult_890_n111) );
  NOR2_X1M_A12TR mult_890_u69 ( .A(mult_890_n165), .B(mult_890_n171), .Y(
        mult_890_n110) );
  NOR2_X1M_A12TR mult_890_u68 ( .A(mult_890_n164), .B(mult_890_n171), .Y(
        mult_890_n109) );
  NOR2_X1M_A12TR mult_890_u67 ( .A(mult_890_n163), .B(mult_890_n171), .Y(
        mult_890_n108) );
  NOR2_X1M_A12TR mult_890_u66 ( .A(mult_890_n162), .B(mult_890_n171), .Y(
        mult_890_n107) );
  NOR2_X1M_A12TR mult_890_u65 ( .A(mult_890_n169), .B(mult_890_n170), .Y(
        mult_890_n106) );
  NOR2_X1M_A12TR mult_890_u64 ( .A(mult_890_n168), .B(mult_890_n170), .Y(
        mult_890_n105) );
  NOR2_X1M_A12TR mult_890_u63 ( .A(mult_890_n167), .B(mult_890_n170), .Y(
        mult_890_n104) );
  NOR2_X1M_A12TR mult_890_u62 ( .A(mult_890_n166), .B(mult_890_n170), .Y(
        mult_890_n103) );
  NOR2_X1M_A12TR mult_890_u61 ( .A(mult_890_n165), .B(mult_890_n170), .Y(
        mult_890_n102) );
  NOR2_X1M_A12TR mult_890_u60 ( .A(mult_890_n164), .B(mult_890_n170), .Y(
        mult_890_n101) );
  NOR2_X1M_A12TR mult_890_u59 ( .A(mult_890_n163), .B(mult_890_n170), .Y(
        mult_890_n100) );
  NOR2_X1M_A12TR mult_890_u58 ( .A(mult_890_n162), .B(mult_890_n170), .Y(
        mult_890_n99) );
  ADDH_X1M_A12TR mult_890_u57 ( .A(mult_890_n160), .B(mult_890_n153), .CO(
        mult_890_n97), .S(mult_890_n98) );
  ADDH_X1M_A12TR mult_890_u56 ( .A(mult_890_n145), .B(mult_890_n138), .CO(
        mult_890_n95), .S(mult_890_n96) );
  ADDF_X1M_A12TR mult_890_u55 ( .A(mult_890_n159), .B(mult_890_n152), .CI(
        mult_890_n97), .CO(mult_890_n93), .S(mult_890_n94) );
  ADDH_X1M_A12TR mult_890_u54 ( .A(mult_890_n137), .B(mult_890_n130), .CO(
        mult_890_n91), .S(mult_890_n92) );
  ADDF_X1M_A12TR mult_890_u53 ( .A(mult_890_n158), .B(mult_890_n144), .CI(
        mult_890_n151), .CO(mult_890_n89), .S(mult_890_n90) );
  ADDF_X1M_A12TR mult_890_u52 ( .A(mult_890_n95), .B(mult_890_n92), .CI(
        mult_890_n90), .CO(mult_890_n87), .S(mult_890_n88) );
  ADDH_X1M_A12TR mult_890_u51 ( .A(mult_890_n129), .B(mult_890_n122), .CO(
        mult_890_n85), .S(mult_890_n86) );
  ADDF_X1M_A12TR mult_890_u50 ( .A(mult_890_n157), .B(mult_890_n136), .CI(
        mult_890_n143), .CO(mult_890_n83), .S(mult_890_n84) );
  ADDF_X1M_A12TR mult_890_u49 ( .A(mult_890_n150), .B(mult_890_n91), .CI(
        mult_890_n89), .CO(mult_890_n81), .S(mult_890_n82) );
  ADDF_X1M_A12TR mult_890_u48 ( .A(mult_890_n86), .B(mult_890_n84), .CI(
        mult_890_n87), .CO(mult_890_n79), .S(mult_890_n80) );
  ADDH_X1M_A12TR mult_890_u47 ( .A(mult_890_n121), .B(mult_890_n114), .CO(
        mult_890_n77), .S(mult_890_n78) );
  ADDF_X1M_A12TR mult_890_u46 ( .A(mult_890_n156), .B(mult_890_n128), .CI(
        mult_890_n135), .CO(mult_890_n75), .S(mult_890_n76) );
  ADDF_X1M_A12TR mult_890_u45 ( .A(mult_890_n149), .B(mult_890_n142), .CI(
        mult_890_n85), .CO(mult_890_n73), .S(mult_890_n74) );
  ADDF_X1M_A12TR mult_890_u44 ( .A(mult_890_n83), .B(mult_890_n78), .CI(
        mult_890_n76), .CO(mult_890_n71), .S(mult_890_n72) );
  ADDF_X1M_A12TR mult_890_u43 ( .A(mult_890_n81), .B(mult_890_n74), .CI(
        mult_890_n72), .CO(mult_890_n69), .S(mult_890_n70) );
  ADDH_X1M_A12TR mult_890_u42 ( .A(mult_890_n113), .B(mult_890_n106), .CO(
        mult_890_n67), .S(mult_890_n68) );
  ADDF_X1M_A12TR mult_890_u41 ( .A(mult_890_n155), .B(mult_890_n120), .CI(
        mult_890_n127), .CO(mult_890_n65), .S(mult_890_n66) );
  ADDF_X1M_A12TR mult_890_u40 ( .A(mult_890_n148), .B(mult_890_n134), .CI(
        mult_890_n141), .CO(mult_890_n63), .S(mult_890_n64) );
  ADDF_X1M_A12TR mult_890_u39 ( .A(mult_890_n77), .B(mult_890_n75), .CI(
        mult_890_n68), .CO(mult_890_n61), .S(mult_890_n62) );
  ADDF_X1M_A12TR mult_890_u38 ( .A(mult_890_n64), .B(mult_890_n66), .CI(
        mult_890_n73), .CO(mult_890_n59), .S(mult_890_n60) );
  ADDF_X1M_A12TR mult_890_u37 ( .A(mult_890_n71), .B(mult_890_n62), .CI(
        mult_890_n60), .CO(mult_890_n57), .S(mult_890_n58) );
  ADDH_X1M_A12TR mult_890_u36 ( .A(mult_890_n112), .B(mult_890_n105), .CO(
        mult_890_n55), .S(mult_890_n56) );
  ADDF_X1M_A12TR mult_890_u35 ( .A(mult_890_n147), .B(mult_890_n119), .CI(
        mult_890_n126), .CO(mult_890_n53), .S(mult_890_n54) );
  ADDF_X1M_A12TR mult_890_u34 ( .A(mult_890_n140), .B(mult_890_n133), .CI(
        mult_890_n67), .CO(mult_890_n51), .S(mult_890_n52) );
  ADDF_X1M_A12TR mult_890_u33 ( .A(mult_890_n65), .B(mult_890_n63), .CI(
        mult_890_n56), .CO(mult_890_n49), .S(mult_890_n50) );
  ADDF_X1M_A12TR mult_890_u32 ( .A(mult_890_n54), .B(mult_890_n61), .CI(
        mult_890_n52), .CO(mult_890_n47), .S(mult_890_n48) );
  ADDF_X1M_A12TR mult_890_u31 ( .A(mult_890_n59), .B(mult_890_n50), .CI(
        mult_890_n48), .CO(mult_890_n45), .S(mult_890_n46) );
  ADDF_X1M_A12TR mult_890_u30 ( .A(mult_890_n139), .B(mult_890_n104), .CI(
        mult_890_n111), .CO(mult_890_n43), .S(mult_890_n44) );
  ADDF_X1M_A12TR mult_890_u29 ( .A(mult_890_n118), .B(mult_890_n125), .CI(
        mult_890_n132), .CO(mult_890_n41), .S(mult_890_n42) );
  ADDF_X1M_A12TR mult_890_u28 ( .A(mult_890_n55), .B(mult_890_n53), .CI(
        mult_890_n42), .CO(mult_890_n39), .S(mult_890_n40) );
  ADDF_X1M_A12TR mult_890_u27 ( .A(mult_890_n44), .B(mult_890_n51), .CI(
        mult_890_n49), .CO(mult_890_n37), .S(mult_890_n38) );
  ADDF_X1M_A12TR mult_890_u26 ( .A(mult_890_n40), .B(mult_890_n47), .CI(
        mult_890_n38), .CO(mult_890_n35), .S(mult_890_n36) );
  ADDF_X1M_A12TR mult_890_u25 ( .A(mult_890_n131), .B(mult_890_n103), .CI(
        mult_890_n110), .CO(mult_890_n33), .S(mult_890_n34) );
  ADDF_X1M_A12TR mult_890_u24 ( .A(mult_890_n124), .B(mult_890_n117), .CI(
        mult_890_n43), .CO(mult_890_n31), .S(mult_890_n32) );
  ADDF_X1M_A12TR mult_890_u23 ( .A(mult_890_n41), .B(mult_890_n34), .CI(
        mult_890_n39), .CO(mult_890_n29), .S(mult_890_n30) );
  ADDF_X1M_A12TR mult_890_u22 ( .A(mult_890_n32), .B(mult_890_n37), .CI(
        mult_890_n30), .CO(mult_890_n27), .S(mult_890_n28) );
  ADDF_X1M_A12TR mult_890_u21 ( .A(mult_890_n123), .B(mult_890_n102), .CI(
        mult_890_n109), .CO(mult_890_n25), .S(mult_890_n26) );
  ADDF_X1M_A12TR mult_890_u20 ( .A(mult_890_n116), .B(mult_890_n33), .CI(
        mult_890_n26), .CO(mult_890_n23), .S(mult_890_n24) );
  ADDF_X1M_A12TR mult_890_u19 ( .A(mult_890_n31), .B(mult_890_n24), .CI(
        mult_890_n29), .CO(mult_890_n21), .S(mult_890_n22) );
  ADDF_X1M_A12TR mult_890_u18 ( .A(mult_890_n115), .B(mult_890_n101), .CI(
        mult_890_n108), .CO(mult_890_n19), .S(mult_890_n20) );
  ADDF_X1M_A12TR mult_890_u17 ( .A(mult_890_n25), .B(mult_890_n20), .CI(
        mult_890_n23), .CO(mult_890_n17), .S(mult_890_n18) );
  ADDF_X1M_A12TR mult_890_u16 ( .A(mult_890_n107), .B(mult_890_n100), .CI(
        mult_890_n19), .CO(mult_890_n15), .S(mult_890_n16) );
  ADDH_X1M_A12TR mult_890_u15 ( .A(mult_890_n154), .B(mult_890_n161), .CO(
        mult_890_n14), .S(n14670) );
  ADDF_X1M_A12TR mult_890_u14 ( .A(mult_890_n146), .B(mult_890_n14), .CI(
        mult_890_n98), .CO(mult_890_n13), .S(n14680) );
  ADDF_X1M_A12TR mult_890_u13 ( .A(mult_890_n96), .B(mult_890_n13), .CI(
        mult_890_n94), .CO(mult_890_n12), .S(n14690) );
  ADDF_X1M_A12TR mult_890_u12 ( .A(mult_890_n93), .B(mult_890_n88), .CI(
        mult_890_n12), .CO(mult_890_n11), .S(n14700) );
  ADDF_X1M_A12TR mult_890_u11 ( .A(mult_890_n82), .B(mult_890_n80), .CI(
        mult_890_n11), .CO(mult_890_n10), .S(n14710) );
  ADDF_X1M_A12TR mult_890_u10 ( .A(mult_890_n79), .B(mult_890_n70), .CI(
        mult_890_n10), .CO(mult_890_n9), .S(n14720) );
  ADDF_X1M_A12TR mult_890_u9 ( .A(mult_890_n69), .B(mult_890_n58), .CI(
        mult_890_n9), .CO(mult_890_n8), .S(n14730) );
  ADDF_X1M_A12TR mult_890_u8 ( .A(mult_890_n57), .B(mult_890_n46), .CI(
        mult_890_n8), .CO(mult_890_n7), .S(n14740) );
  ADDF_X1M_A12TR mult_890_u7 ( .A(mult_890_n45), .B(mult_890_n36), .CI(
        mult_890_n7), .CO(mult_890_n6), .S(n14750) );
  ADDF_X1M_A12TR mult_890_u6 ( .A(mult_890_n35), .B(mult_890_n28), .CI(
        mult_890_n6), .CO(mult_890_n5), .S(n14760) );
  ADDF_X1M_A12TR mult_890_u5 ( .A(mult_890_n27), .B(mult_890_n22), .CI(
        mult_890_n5), .CO(mult_890_n4), .S(n14770) );
  ADDF_X1M_A12TR mult_890_u4 ( .A(mult_890_n18), .B(mult_890_n21), .CI(
        mult_890_n4), .CO(mult_890_n3), .S(n14780) );
  ADDF_X1M_A12TR mult_890_u3 ( .A(mult_890_n16), .B(mult_890_n17), .CI(
        mult_890_n3), .CO(mult_890_n2), .S(n14790) );
  ADDF_X1M_A12TR mult_890_u2 ( .A(mult_890_n99), .B(mult_890_n15), .CI(
        mult_890_n2), .CO(n14810), .S(n14800) );
endmodule

