

module voting_machine ( osc_clk, reset_n, x_coord, y_coord, xy_valid, 
                        max_selections, contest_num, current_contest_state ) ;

    input osc_clk ;
    input reset_n ;
    input [11:0]x_coord ;
    input [11:0]y_coord ;
    input xy_valid ;
    input [3:0]max_selections ;
    output [2:0]contest_num ;
    output [11:0]current_contest_state ;

    wire ss_state_7_11, ss_state_7_10, ss_state_7_9, ss_state_7_8, ss_state_7_7, 
         ss_state_7_6, ss_state_7_5, ss_state_7_4, ss_state_7_3, ss_state_7_2, 
         ss_state_7_1, ss_state_7_0, ss_state_6_11, ss_state_6_10, ss_state_6_9, 
         ss_state_6_8, ss_state_6_7, ss_state_6_6, ss_state_6_5, ss_state_6_4, 
         ss_state_6_3, ss_state_6_2, ss_state_6_1, ss_state_6_0, ss_state_5_11, 
         ss_state_5_10, ss_state_5_9, ss_state_5_8, ss_state_5_7, ss_state_5_6, 
         ss_state_5_5, ss_state_5_4, ss_state_5_3, ss_state_5_2, ss_state_5_1, 
         ss_state_5_0, ss_state_4_11, ss_state_4_10, ss_state_4_9, ss_state_4_8, 
         ss_state_4_7, ss_state_4_6, ss_state_4_5, ss_state_4_4, ss_state_4_3, 
         ss_state_4_2, ss_state_4_1, ss_state_4_0, ss_state_3_11, ss_state_3_10, 
         ss_state_3_9, ss_state_3_8, ss_state_3_7, ss_state_3_6, ss_state_3_5, 
         ss_state_3_4, ss_state_3_3, ss_state_3_2, ss_state_3_1, ss_state_3_0, 
         ss_state_2_11, ss_state_2_10, ss_state_2_9, ss_state_2_8, ss_state_2_7, 
         ss_state_2_6, ss_state_2_5, ss_state_2_4, ss_state_2_3, ss_state_2_2, 
         ss_state_2_1, ss_state_2_0, ss_state_1_11, ss_state_1_10, ss_state_1_9, 
         ss_state_1_8, ss_state_1_7, ss_state_1_6, ss_state_1_5, ss_state_1_4, 
         ss_state_1_3, ss_state_1_2, ss_state_1_1, ss_state_1_0, ss_state_0_11, 
         ss_state_0_10, ss_state_0_9, ss_state_0_8, ss_state_0_7, ss_state_0_6, 
         ss_state_0_5, ss_state_0_4, ss_state_0_3, ss_state_0_2, ss_state_0_1, 
         ss_state_0_0, ss_selector_7, ss_selector_6, ss_selector_5, 
         ss_selector_4, ss_selector_3, ss_selector_2, ss_selector_1, 
         ss_selector_0, ss_enable, touch_pulse, button_num_3, button_num_2, 
         button_num_1, button_num_0, reset, map_irq_pulse_n, map_irq_pulse, 
         nx118, state_0, nx124, state_1, nx125, nx14, nx26, nx132, nx134, nx138, 
         nx198, nx212, nx72325, nx4, nx434, nx24, nx28, nx32, nx36, nx40, nx46, 
         a_12__dup_60616, nx48, nx56, nx58, nx66, nx68, nx74, nx102, nx116, 
         nx130, nx136, nx140, nx152, nx160, nx166, nx170, nx184, nx192, nx204, 
         nx226, nx232, nx238, nx242, nx246, nx256, nx258, nx260, nx266, nx274, 
         nx278, nx284, nx288, nx294, nx310, nx312, nx314, nx322, nx328, nx336, 
         nx338, nx348, nx362, nx372, nx390, nx404, nx414, nx426, nx430, nx440, 
         nx454, nx462, nx472, nx488, nx538, nx548, nx558, nx562, nx578, nx592, 
         nx608, nx616, nx628, nx630, nx634, nx640, nx656, nx660, nx668, nx672, 
         nx682, nx692, nx708, nx736, nx748, nx778, nx792, nx800, nx810, nx824, 
         nx854, nx880, nx890, nx908, nx912, nx916, nx918, nx920, nx922, nx924, 
         nx938, nx942, nx958, nx962, nx972, nx974, nx984, nx992, nx1000, nx1012, 
         nx1020, nx1026, nx1030, nx1032, nx1034, nx1036, nx1042, nx1052, nx1066, 
         nx1104, nx1118, nx1142, nx1164, nx72361, nx72364, nx72374, nx72377, 
         nx72383, nx72385, nx72388, nx72391, nx72394, nx72396, nx72399, nx72401, 
         nx72404, nx72406, nx72409, nx72411, nx72413, nx72417, nx72422, nx72424, 
         nx72437, nx72441, nx72444, nx72446, nx72450, nx72452, nx72456, nx72458, 
         nx72472, nx72474, nx72476, nx72479, nx72485, nx72488, nx72490, nx72493, 
         nx72495, nx72497, nx72512, nx72516, nx72520, nx72522, nx72524, nx72526, 
         nx72530, nx72534, nx72536, nx72545, nx72550, nx72555, nx72559, nx72562, 
         nx72565, nx72567, nx72570, nx72572, nx72576, nx72586, nx72589, nx72591, 
         nx72593, nx72596, nx72599, nx72603, nx72607, nx72609, nx72611, nx72616, 
         nx72618, nx72621, nx72626, nx72629, nx72632, nx72636, nx72643, nx72645, 
         nx72647, nx72655, nx72660, nx72662, nx72667, nx72674, nx72676, nx72679, 
         nx72682, nx72686, nx72689, nx72692, nx72695, nx72697, nx72699, nx72701, 
         nx72704, nx72706, nx72713, nx72717, nx72719, nx72721, nx72724, nx72727, 
         nx72729, nx72733, nx72738, nx72744, nx72747, nx72749, nx72752, nx72754, 
         nx72757, nx72763, nx72766, nx72771, nx72774, nx72777, nx72783, nx72785, 
         nx72787, nx72793, nx72795, nx72797, nx72799, nx72802, nx72804, nx72807, 
         nx72809, nx72815, nx72818, nx72825, nx72830, nx72834, nx72837, nx72840, 
         nx72855, nx72858, nx72865, nx72869, nx72871, nx72874, nx72882, nx72884, 
         nx72891, NOT_nx4, nx72991, nx73005, cast, nx461, nx8, nx16, NOT_reset, 
         nx42, nx52, nx447, nx448, nx82, nx112, nx484, nx487, nx490, nx492, 
         nx497, nx499, nx506, nx515, nx517, nx521, NOT_nx474, nx614, nx629, 
         nx449, nx642, nx644, nx646, nx457, nx165, nx168, nx172, nx2321, nx493, 
         nx2346, nx494, nx495, nx30, nx44, nx496, nx62, nx88, nx94, nx106, nx498, 
         nx500, nx501, nx502, nx503, nx156, nx162, nx176, nx186, nx504, nx214, 
         nx220, nx250, nx505, nx507, nx508, nx290, nx306, nx509, nx326, nx510, 
         nx342, nx364, nx370, nx398, nx412, nx416, nx420, nx428, nx436, nx458, 
         nx474, nx511, nx512, nx2356, nx2359, nx2361, nx2364, nx2367, nx2369, 
         nx2372, nx2374, nx2376, nx2378, nx2380, nx2382, nx2384, nx2386, nx2388, 
         nx2390, nx2392, nx2394, nx2396, nx2398, nx2400, nx2402, nx2404, nx2408, 
         nx2413, nx2416, nx2418, nx2424, nx2432, nx2434, nx2436, nx2438, nx2440, 
         nx2442, nx2444, nx2446, nx2448, nx2451, nx2455, nx2459, nx2463, nx2469, 
         nx2471, nx2475, nx2477, nx2483, nx2487, nx2490, nx2495, nx2499, nx2503, 
         nx2507, nx2511, nx2515, nx2605, nx2615, nx2625, nx2635, nx2645, nx2655, 
         nx2665, nx2675, nx2685, nx2695, nx2705, nx2715, nx2729, nx655, nx657, 
         nx658, nx659, nx661, nx662, nx663, nx664, nx665, nx666, nx667, nx669, 
         nx670, nx671, nx673, nx674, nx675, nx676, nx677, nx678, nx679, nx680, 
         nx681, nx683, nx684, nx685, nx686, nx687, nx688, nx689, nx690, nx691, 
         nx693, nx694, nx695, nx696, nx697, nx698, nx699, nx700, nx701, nx702, 
         nx703, nx704, nx705, nx706, nx707, nx709, nx710, nx711, nx712, nx713, 
         nx714, nx715, nx716, nx717, nx718, nx719, nx720, nx721, nx722, nx723, 
         nx724, nx725, nx726, nx727, nx728, nx729, nx730, nx731, nx732, nx733, 
         nx734, nx735, nx737, nx738, nx739, nx740, nx741, nx742, nx743, nx744, 
         nx745, nx746, nx747, nx749, nx750, nx751, nx752, nx753, nx754, nx755, 
         nx756, nx757, nx758, nx759, nx760, nx761, nx762, nx763, nx764, nx765, 
         nx766, nx767, nx768, nx769, nx770, nx771, nx772, nx773, nx774, nx775, 
         nx776, nx909, nx910, nx911, nx913, nx914, nx915, nx917, nx919, nx921, 
         nx923, nx925, nx926, nx927, nx928, nx929, nx930, nx931, nx932, nx933, 
         nx934, nx935, nx936, nx937, nx939, nx940, nx941, nx943, nx944, nx945, 
         nx946, nx947, nx948, nx949, nx950, nx951, nx952, nx953, nx954, nx955, 
         nx956, nx957, nx959, nx960, nx961, nx963, nx964, nx965, nx966, nx967, 
         nx968, nx969, nx970, nx971, nx973, nx975, nx976, nx977, nx978, nx979, 
         nx980, nx981, nx982, nx983, nx985, nx986, nx987, nx988, nx989, nx990, 
         nx991, nx993, nx994, nx995, nx996, nx997, nx998, nx999, nx1001, nx1002, 
         nx1003, nx1004, nx1005, nx1006, nx1007, nx1008, nx1009, nx1010, nx1011, 
         nx1013, nx1014, nx1015, nx1016, nx1017, nx1018, nx1019, nx1021, nx1022, 
         nx1023, nx1024, nx1025, nx1027, nx1028, nx1029, nx1031, nx1033, nx1035, 
         nx1037, nx1038, nx1039, nx1040, nx1041, nx1043, nx1044, nx1173, nx1174, 
         nx1175, nx1176, nx1177, nx1178, nx1179, nx1180, nx1181, nx1182, nx1183, 
         nx1184, nx1185, nx1186, nx1187, nx1188, nx1189, nx1190, nx1191, nx1192, 
         nx1193, nx1194, nx1195, nx1196, nx1197, nx1198, nx1199, nx1200, nx1201, 
         nx1202, nx1203, nx1204, nx1205, nx1206, nx1207, nx1208, nx1209, nx1210, 
         nx1211, nx1212, nx1213, nx1214, nx1215, nx1216, nx1217, nx1218, nx1219, 
         nx1220, nx1221, nx1222, nx1223, nx1224, nx1225, nx1226, nx1227, nx1228, 
         nx1229, nx1230, nx1231, nx1232, nx1233, nx1234, nx1235, nx1236, nx1237, 
         nx1238, nx1239, nx1240, nx1241, nx1242, nx1243, nx1244, nx1245, nx1246, 
         nx1247, nx1248, nx1249, nx1250, nx1251, nx1252, nx1253, nx1254, nx1255, 
         nx1256, nx1257, nx1258, nx1259, nx1260, nx1261, nx1262, nx1263, nx1264, 
         nx1265, nx1266, nx1267, nx1268, nx1269, nx1270, nx1271, nx1272, nx1273, 
         nx1274, nx1275, nx1276, nx1277, nx1278, nx1279, nx1280, nx1281, nx1282, 
         nx1283, nx1284, nx1285, nx1409, nx1410, nx1411, nx1412, nx1413, nx1414, 
         nx1415, nx1416, nx1417, nx1418, nx1419, nx1420, nx1421, nx1422, nx1423, 
         nx1424, nx1425, nx1426, nx1427, nx1428, nx1429, nx1430, nx1431, nx1432, 
         nx1433, nx1434, nx1435, nx1436, nx1437, nx1438, nx1439, nx1440, nx1441, 
         nx1442, nx1443, nx1444, nx1445, nx1446, nx1447, nx1448, nx1449, nx1450, 
         nx1451, nx1452, nx1453, nx1454, nx1455, nx1456, nx1457, nx1458, nx1459, 
         nx1460, nx1461, nx1462, nx1463, nx1464, nx1465, nx1466, nx1467, nx1468, 
         nx1469, nx1470, nx1471, nx1472, nx1473, nx1474, nx1475, nx1476, nx1477, 
         nx1478, nx1479, nx1480, nx1481, nx1482, nx1483, nx1484, nx1485, nx1486, 
         nx1487, nx1488, nx1489, nx1490, nx1491, nx1492, nx1493, nx1494, nx1495, 
         nx1496, nx1497, nx1498, nx1499, nx1500, nx1501, nx1502, nx1503, nx1504, 
         nx1505, nx1506, nx1507, nx1508, nx1509, nx1510, nx1511, nx1512, nx1513, 
         nx1514, nx1515, nx1516, nx1517, nx1518, nx1519, nx1520, nx1521, nx1645, 
         nx1646, nx1647, nx1648, nx1649, nx1650, nx1651, nx1652, nx1653, nx1654, 
         nx1655, nx1656, nx1657, nx1658, nx1659, nx1660, nx1661, nx1662, nx1663, 
         nx1664, nx1665, nx1666, nx1667, nx1668, nx1669, nx1670, nx1671, nx1672, 
         nx1673, nx1674, nx1675, nx1676, nx1677, nx1678, nx1679, nx1680, nx1681, 
         nx1682, nx1683, nx1684, nx1685, nx1686, nx1687, nx1688, nx1689, nx1690, 
         nx1691, nx1692, nx1693, nx1694, nx1695, nx1696, nx1697, nx1698, nx1699, 
         nx1700, nx1701, nx1702, nx1703, nx1704, nx1705, nx1706, nx1707, nx1708, 
         nx1709, nx1710, nx1711, nx1712, nx1713, nx1714, nx1715, nx1716, nx1717, 
         nx1718, nx1719, nx1720, nx1721, nx1722, nx1723, nx1724, nx1725, nx1726, 
         nx1727, nx1728, nx1729, nx1730, nx1731, nx1732, nx1733, nx1734, nx1735, 
         nx1736, nx1737, nx1738, nx1739, nx1740, nx1741, nx1742, nx1743, nx1744, 
         nx1745, nx1746, nx1747, nx1748, nx1749, nx1750, nx1751, nx1752, nx1753, 
         nx1754, nx1755, nx1756, nx1757, nx1881, nx1882, nx1883, nx1884, nx1885, 
         nx1886, nx1887, nx1888, nx1889, nx1890, nx1891, nx1892, nx1893, nx1894, 
         nx1895, nx1896, nx1897, nx1898, nx1899, nx1900, nx1901, nx1902, nx1903, 
         nx1904, nx1905, nx1906, nx1907, nx1908, nx1909, nx1910, nx1911, nx1912, 
         nx1913, nx1914, nx1915, nx1916, nx1917, nx1918, nx1919, nx1920, nx1921, 
         nx1922, nx1923, nx1924, nx1925, nx1926, nx1927, nx1928, nx1929, nx1930, 
         nx1931, nx1932, nx1933, nx1934, nx1935, nx1936, nx1937, nx1938, nx1939, 
         nx1940, nx1941, nx1942, nx1943, nx1944, nx1945, nx1946, nx1947, nx1948, 
         nx1949, nx1950, nx1951, nx1952, nx1953, nx1954, nx1955, nx1956, nx1957, 
         nx1958, nx1959, nx1960, nx1961, nx1962, nx1963, nx1964, nx1965, nx1966, 
         nx1967, nx1968, nx1969, nx1970, nx1971, nx1972, nx1973, nx1974, nx1975, 
         nx1976, nx1977, nx1978, nx1979, nx1980, nx1981, nx1982, nx1983, nx1984, 
         nx1985, nx1986, nx1987, nx1988, nx1989, nx1990, nx1991, nx1992, nx1993, 
         nx2117, nx2118, nx2119, nx2120, nx2121, nx2122, nx2123, nx2124, nx2125, 
         nx2126, nx2127, nx2128, nx2129, nx2130, nx2131, nx2132, nx2133, nx2134, 
         nx2135, nx2136, nx2137, nx2138, nx2139, nx2140, nx2141, nx2142, nx2143, 
         nx2144, nx2145, nx2146, nx2147, nx2148, nx2149, nx2150, nx2151, nx2152, 
         nx2153, nx2154, nx2155, nx2156, nx2157, nx2158, nx2159, nx2160, nx2161, 
         nx2162, nx2163, nx2164, nx2165, nx2166, nx2167, nx2168, nx2169, nx2170, 
         nx2171, nx2172, nx2173, nx2174, nx2175, nx2176, nx2177, nx2178, nx2179, 
         nx2180, nx2181, nx2182, nx2183, nx2184, nx2185, nx2186, nx2187, nx2188, 
         nx2189, nx2190, nx2191, nx2192, nx2193, nx2194, nx2195, nx2196, nx2197, 
         nx2198, nx2199, nx2200, nx2201, nx2202, nx2203, nx2204, nx2205, nx2206, 
         nx2207, nx2208, nx2209, nx2210, nx2211, nx2212, nx2213, nx2214, nx2215, 
         nx2216, nx2217, nx2218, nx2219, nx2220, nx2221, nx2222, nx2223, nx2224, 
         nx2225, nx2226, nx2227, nx2228, nx2229, nx2261, nx2262, nx2263, nx34, 
         nx2264, nx54, nx2265, nx76, nx361, nx367, nx369, nx371, nx377, nx379, 
         nx2266, nx393, nx395, nx397, nx399, nx403, nx405, nx407, nx409, nx413, 
         nx415, nx417, nx419, nx422, nx424, nx2267, nx2268, nx431, nx433, nx435, 
         nx437, nx2269, nx442, nx444, nx446, nx2270, nx451, nx453, nx455, nx2271, 
         nx460, nx2272, nx464, nx467, nx469, nx471, nx473, nx476, nx478, nx480, 
         nx482, nx485, nx2273, nx489, nx491;



    INV_X0P5B_A12TS ix343 (.Y (map_irq_pulse_n), .A (map_irq_pulse)) ;
    INV_X2M_A12TS ix345 (.Y (reset), .A (reset_n)) ;
    OAI31_X0P5M_A12TS ix33 (.Y (nx125), .A0 (nx132), .A1 (state_1), .A2 (state_0
                      ), .B0 (nx124)) ;
    NAND2_X0P5A_A12TS ix133 (.Y (nx132), .A (xy_valid), .B (nx134)) ;
    INV_X0P5B_A12TS ix135 (.Y (nx134), .A (reset)) ;
    DFFQ_X0P5M_A12TS reg_state_1 (.Q (state_1), .CK (osc_clk), .D (nx14)) ;
    OAI31_X0P5M_A12TS ix15 (.Y (nx14), .A0 (nx138), .A1 (reset), .A2 (nx26), .B0 (
                      nx124)) ;
    INV_X0P5B_A12TS ix139 (.Y (nx138), .A (xy_valid)) ;
    NOR2_X0P5A_A12TS ix27 (.Y (nx26), .A (state_1), .B (state_0)) ;
    DFFQ_X0P5M_A12TS reg_state_0 (.Q (state_0), .CK (osc_clk), .D (nx125)) ;
    NAND3_X0P5A_A12TS ix47 (.Y (nx124), .A (state_1), .B (nx134), .C (state_0)
                      ) ;
    TIELO_X1M_A12TS ix119 (.Y (nx118)) ;
    DFFSRPQ_X0P5M_A12TS reg_output_pulse (.Q (map_irq_pulse), .CK (osc_clk), .D (
                        nx198), .R (nx118), .SN (nx212)) ;
    MXT2_X0P5M_A12TS ix199 (.Y (nx198), .A (map_irq_pulse), .B (nx125), .S0 (
                     nx124)) ;
    INV_X0P5B_A12TS ix213 (.Y (nx212), .A (nx118)) ;
    NAND2_X0P5A_A12TS ix1165 (.Y (nx1164), .A (nx1030), .B (nx72799)) ;
    NAND3_X0P5A_A12TS ix1031 (.Y (nx1030), .A (nx72361), .B (nx72616), .C (
                      nx72636)) ;
    AOI31_X0P5M_A12TS ix72362 (.Y (nx72361), .A0 (nx992), .A1 (nx912), .A2 (
                      nx1020), .B0 (nx962)) ;
    NOR3_X0P5A_A12TS ix993 (.Y (nx992), .A (nx72364), .B (y_coord[9]), .C (
                     nx72385)) ;
    AOI31_X0P5M_A12TS ix72365 (.Y (nx72364), .A0 (nx972), .A1 (nx430), .A2 (
                      nx72374), .B0 (nx984)) ;
    NOR3_X0P5A_A12TS ix973 (.Y (nx972), .A (y_coord[3]), .B (y_coord[5]), .C (
                     y_coord[4])) ;
    NOR2_X0P5A_A12TS ix72375 (.Y (nx72374), .A (nx920), .B (nx974)) ;
    XNOR2_X0P5M_A12TS ix72378 (.Y (nx72377), .A (a_12__dup_60616), .B (
                      y_coord[7])) ;
    XOR2_X0P5M_A12TS ix975 (.Y (nx974), .A (a_12__dup_60616), .B (y_coord[8])) ;
    AOI31_X0P5M_A12TS ix985 (.Y (nx984), .A0 (y_coord[7]), .A1 (y_coord[6]), .A2 (
                      y_coord[8]), .B0 (nx72383)) ;
    INV_X0P5B_A12TS ix72384 (.Y (nx72383), .A (a_12__dup_60616)) ;
    NOR3_X0P5A_A12TS ix72389 (.Y (nx72388), .A (nx136), .B (nx130), .C (nx152)
                     ) ;
    AOI21_X0P5M_A12TS ix137 (.Y (nx136), .A0 (nx72391), .A1 (nx72413), .B0 (nx40
                      )) ;
    NAND2_X0P5A_A12TS ix72392 (.Y (nx72391), .A (x_coord[10]), .B (nx36)) ;
    NOR2_X0P5A_A12TS ix37 (.Y (nx36), .A (nx72394), .B (nx72396)) ;
    INV_X0P5B_A12TS ix72395 (.Y (nx72394), .A (x_coord[9])) ;
    NAND2_X0P5A_A12TS ix72397 (.Y (nx72396), .A (x_coord[8]), .B (nx32)) ;
    NOR2_X0P5A_A12TS ix402 (.Y (nx32), .A (nx72399), .B (nx72401)) ;
    INV_X0P5B_A12TS ix72400 (.Y (nx72399), .A (x_coord[7])) ;
    NAND2_X0P5A_A12TS ix72402 (.Y (nx72401), .A (x_coord[6]), .B (nx28)) ;
    NOR2_X0P5A_A12TS ix29 (.Y (nx28), .A (nx72404), .B (nx72406)) ;
    INV_X0P5B_A12TS ix72405 (.Y (nx72404), .A (x_coord[5])) ;
    NAND2_X0P5A_A12TS ix72407 (.Y (nx72406), .A (x_coord[4]), .B (nx24)) ;
    NOR2_X0P5A_A12TS ix25 (.Y (nx24), .A (nx72409), .B (nx72411)) ;
    INV_X0P5B_A12TS ix72410 (.Y (nx72409), .A (x_coord[3])) ;
    NAND3_X0P5A_A12TS ix72412 (.Y (nx72411), .A (x_coord[2]), .B (x_coord[1]), .C (
                      x_coord[0])) ;
    INV_X0P5B_A12TS ix72414 (.Y (nx72413), .A (x_coord[11])) ;
    NOR2_X0P5A_A12TS ix41 (.Y (nx40), .A (nx72413), .B (nx72391)) ;
    INV_X0P5B_A12TS ix131 (.Y (nx130), .A (nx72417)) ;
    OAI21_X0P5M_A12TS ix72418 (.Y (nx72417), .A0 (nx36), .A1 (x_coord[10]), .B0 (
                      nx72391)) ;
    AOI21_X0P5M_A12TS ix153 (.Y (nx152), .A0 (nx72396), .A1 (nx72394), .B0 (nx36
                      )) ;
    XOR2_X0P5M_A12TS ix72423 (.Y (nx72422), .A (nx72424), .B (a_12__dup_60616)
                     ) ;
    OAI21_X0P5M_A12TS ix72425 (.Y (nx72424), .A0 (nx32), .A1 (x_coord[8]), .B0 (
                      nx72396)) ;
    OAI21_X0P5M_A12TS ix72438 (.Y (nx72437), .A0 (nx28), .A1 (x_coord[6]), .B0 (
                      nx72401)) ;
    XNOR2_X0P5M_A12TS ix72442 (.Y (nx72441), .A (nx46), .B (a_12__dup_60616)) ;
    AOI21_X0P5M_A12TS ix403 (.Y (nx46), .A0 (nx72406), .A1 (nx72404), .B0 (nx28)
                      ) ;
    OAI21_X0P5M_A12TS ix72445 (.Y (nx72444), .A0 (nx24), .A1 (x_coord[4]), .B0 (
                      nx72406)) ;
    CGENI_X1M_A12TS ix72447 (.CON (nx72446), .A (nx74), .B (nx72383), .CI (nx102
                    )) ;
    AOI21_X0P5M_A12TS ix75 (.Y (nx74), .A0 (nx72411), .A1 (nx72409), .B0 (nx24)
                      ) ;
    CGENI_X1M_A12TS ix103 (.CON (nx102), .A (nx72450), .B (a_12__dup_60616), .CI (
                    nx72452)) ;
    AO21A1AI2_X0P5M_A12TS ix72451 (.Y (nx72450), .A0 (x_coord[1]), .A1 (
                          x_coord[0]), .B0 (x_coord[2]), .C0 (nx72411)) ;
    INV_X0P5B_A12TS ix72457 (.Y (nx72456), .A (x_coord[0])) ;
    NAND2_X0P5A_A12TS ix72459 (.Y (nx72458), .A (nx46), .B (nx72383)) ;
    NOR2_X0P5A_A12TS ix641 (.Y (nx640), .A (nx72424), .B (a_12__dup_60616)) ;
    XNOR2_X0P5M_A12TS ix72473 (.Y (nx72472), .A (a_12__dup_60616), .B (
                      x_coord[8])) ;
    XNOR2_X0P5M_A12TS ix72475 (.Y (nx72474), .A (a_12__dup_60616), .B (
                      x_coord[5])) ;
    XNOR2_X0P5M_A12TS ix72477 (.Y (nx72476), .A (a_12__dup_60616), .B (
                      x_coord[7])) ;
    OA21A1OI2_X0P5M_A12TS ix185 (.Y (nx184), .A0 (x_coord[6]), .A1 (x_coord[5])
                          , .B0 (x_coord[7]), .C0 (nx72383)) ;
    AOI21_X0P5M_A12TS ix295 (.Y (nx294), .A0 (nx72485), .A1 (nx72497), .B0 (
                      nx246)) ;
    NAND2_X0P5A_A12TS ix72486 (.Y (nx72485), .A (y_coord[6]), .B (nx242)) ;
    NOR2_X0P5A_A12TS ix243 (.Y (nx242), .A (nx72488), .B (nx72490)) ;
    INV_X0P5B_A12TS ix72489 (.Y (nx72488), .A (y_coord[5])) ;
    NAND2_X0P5A_A12TS ix72491 (.Y (nx72490), .A (y_coord[4]), .B (nx238)) ;
    NOR2_X0P5A_A12TS ix239 (.Y (nx238), .A (nx72493), .B (nx72495)) ;
    INV_X0P5B_A12TS ix72494 (.Y (nx72493), .A (y_coord[3])) ;
    NAND3_X0P5A_A12TS ix72496 (.Y (nx72495), .A (y_coord[2]), .B (y_coord[0]), .C (
                      y_coord[1])) ;
    INV_X0P5B_A12TS ix72498 (.Y (nx72497), .A (y_coord[7])) ;
    NOR2_X0P5A_A12TS ix247 (.Y (nx246), .A (nx72497), .B (nx72485)) ;
    AOI21_X0P5M_A12TS ix363 (.Y (nx362), .A0 (nx72495), .A1 (nx72493), .B0 (
                      nx238)) ;
    AO21A1AI2_X0P5M_A12TS ix72513 (.Y (nx72512), .A0 (y_coord[0]), .A1 (
                          y_coord[1]), .B0 (y_coord[2]), .C0 (nx72495)) ;
    OAI21_X0P5M_A12TS ix72517 (.Y (nx72516), .A0 (nx238), .A1 (y_coord[4]), .B0 (
                      nx72490)) ;
    AOI21_X0P5M_A12TS ix311 (.Y (nx310), .A0 (nx72490), .A1 (nx72488), .B0 (
                      nx242)) ;
    NAND2_X0P5A_A12TS ix72521 (.Y (nx72520), .A (nx310), .B (nx72383)) ;
    XOR2_X0P5M_A12TS ix72523 (.Y (nx72522), .A (nx72524), .B (a_12__dup_60616)
                     ) ;
    OAI21_X0P5M_A12TS ix72525 (.Y (nx72524), .A0 (nx246), .A1 (y_coord[8]), .B0 (
                      nx72526)) ;
    NAND2_X0P5A_A12TS ix72527 (.Y (nx72526), .A (y_coord[8]), .B (nx246)) ;
    INV_X0P5B_A12TS ix72531 (.Y (nx72530), .A (y_coord[11])) ;
    INV_X0P5B_A12TS ix72535 (.Y (nx72534), .A (y_coord[9])) ;
    NOR3_X0P5A_A12TS ix963 (.Y (nx962), .A (nx72545), .B (nx72559), .C (nx72589)
                     ) ;
    NOR3_X0P5A_A12TS ix549 (.Y (nx548), .A (nx72385), .B (y_coord[9]), .C (
                     y_coord[8])) ;
    CGENI_X1M_A12TS ix441 (.CON (nx440), .A (nx72383), .B (y_coord[3]), .CI (
                    nx72550)) ;
    CGENI_X1M_A12TS ix427 (.CON (nx426), .A (nx72383), .B (y_coord[1]), .CI (
                    y_coord[0])) ;
    NOR3_X0P5A_A12TS ix72556 (.Y (nx72555), .A (nx924), .B (nx922), .C (nx920)
                     ) ;
    XOR2_X0P5M_A12TS ix925 (.Y (nx924), .A (a_12__dup_60616), .B (y_coord[5])) ;
    XOR2_X0P5M_A12TS ix923 (.Y (nx922), .A (a_12__dup_60616), .B (y_coord[4])) ;
    OAI211_X0P5M_A12TS ix72560 (.Y (nx72559), .A0 (nx880), .A1 (nx184), .B0 (
                       nx434), .C0 (nx908)) ;
    OAI22_X0P5M_A12TS ix881 (.Y (nx880), .A0 (nx72383), .A1 (x_coord[8]), .B0 (
                      nx72562), .B1 (nx72572)) ;
    NOR2_X0P5A_A12TS ix205 (.Y (nx204), .A (x_coord[3]), .B (nx72565)) ;
    CGENI_X1M_A12TS ix72566 (.CON (nx72565), .A (a_12__dup_60616), .B (nx72567)
                    , .CI (nx192)) ;
    INV_X0P5B_A12TS ix72568 (.Y (nx72567), .A (x_coord[2])) ;
    NOR2_X0P5A_A12TS ix193 (.Y (nx192), .A (x_coord[1]), .B (x_coord[0])) ;
    INV_X0P5B_A12TS ix72571 (.Y (nx72570), .A (x_coord[4])) ;
    NAND3_X0P5A_A12TS ix72573 (.Y (nx72572), .A (nx72472), .B (nx72474), .C (
                      nx72476)) ;
    NOR3_X0P5A_A12TS ix404 (.Y (nx434), .A (x_coord[9]), .B (x_coord[11]), .C (
                     x_coord[10])) ;
    OAI31_X0P5M_A12TS ix72577 (.Y (nx72576), .A0 (nx166), .A1 (nx160), .A2 (
                      nx890), .B0 (nx72422)) ;
    AOI21_X0P5M_A12TS ix167 (.Y (nx166), .A0 (nx72401), .A1 (nx72399), .B0 (nx32
                      )) ;
    AO21A1AI2_X0P5M_A12TS ix891 (.Y (nx890), .A0 (nx72444), .A1 (nx72446), .B0 (
                          nx56), .C0 (nx72458)) ;
    AOI31_X0P5M_A12TS ix72594 (.Y (nx72593), .A0 (nx942), .A1 (nx72607), .A2 (
                      nx72609), .B0 (nx288)) ;
    NAND3_X0P5A_A12TS ix943 (.Y (nx942), .A (nx72596), .B (nx72603), .C (nx72516
                      )) ;
    NAND2_X0P5A_A12TS ix72600 (.Y (nx72599), .A (y_coord[0]), .B (y_coord[1])) ;
    CGENI_X1M_A12TS ix72604 (.CON (nx72603), .A (nx336), .B (nx72383), .CI (
                    nx348)) ;
    NAND2_X0P5A_A12TS ix349 (.Y (nx348), .A (y_coord[0]), .B (y_coord[1])) ;
    XNOR2_X0P5M_A12TS ix72608 (.Y (nx72607), .A (nx310), .B (a_12__dup_60616)) ;
    XOR2_X0P5M_A12TS ix72610 (.Y (nx72609), .A (nx72611), .B (a_12__dup_60616)
                     ) ;
    OAI21_X0P5M_A12TS ix72612 (.Y (nx72611), .A0 (nx242), .A1 (y_coord[6]), .B0 (
                      nx72485)) ;
    AO21A1AI2_X0P5M_A12TS ix72619 (.Y (nx72618), .A0 (a_12__dup_60616), .A1 (
                          nx72534), .B0 (nx462), .C0 (nx404)) ;
    NOR3_X0P5A_A12TS ix463 (.Y (nx462), .A (nx72621), .B (y_coord[8]), .C (nx454
                     )) ;
    AOI32_X0P5M_A12TS ix72622 (.Y (nx72621), .A0 (nx414), .A1 (nx440), .A2 (
                      nx72377), .B0 (a_12__dup_60616), .B1 (nx72497)) ;
    NOR3_X0P5A_A12TS ix415 (.Y (nx414), .A (y_coord[4]), .B (y_coord[6]), .C (
                     y_coord[5])) ;
    XOR2_X0P5M_A12TS ix455 (.Y (nx454), .A (a_12__dup_60616), .B (y_coord[9])) ;
    NOR2_X0P5A_A12TS ix405 (.Y (nx404), .A (y_coord[11]), .B (y_coord[10])) ;
    AOI211_X0P5M_A12TS ix72627 (.Y (nx72626), .A0 (nx72522), .A1 (nx390), .B0 (
                       nx288), .C0 (nx258)) ;
    CGENI_X1M_A12TS ix391 (.CON (nx390), .A (nx72591), .B (a_12__dup_60616), .CI (
                    nx72629)) ;
    AOI31_X0P5M_A12TS ix72630 (.Y (nx72629), .A0 (nx372), .A1 (nx72607), .A2 (
                      nx72609), .B0 (nx322)) ;
    CGENI_X1M_A12TS ix373 (.CON (nx372), .A (nx72516), .B (a_12__dup_60616), .CI (
                    nx72632)) ;
    CGENI_X1M_A12TS ix323 (.CON (nx322), .A (nx72611), .B (a_12__dup_60616), .CI (
                    nx72520)) ;
    NOR2_X0P5A_A12TS ix259 (.Y (nx258), .A (nx72524), .B (a_12__dup_60616)) ;
    NOR2_X0P5A_A12TS ix261 (.Y (nx260), .A (nx72534), .B (nx72526)) ;
    OAI21_X0P5M_A12TS ix72646 (.Y (nx72645), .A0 (nx260), .A1 (y_coord[10]), .B0 (
                      nx72647)) ;
    NAND2_X0P5A_A12TS ix72648 (.Y (nx72647), .A (y_coord[10]), .B (nx260)) ;
    NOR2_X0P5A_A12TS ix279 (.Y (nx278), .A (nx72530), .B (nx72647)) ;
    AOI21_X0P5M_A12TS ix72656 (.Y (nx72655), .A0 (nx136), .A1 (nx72383), .B0 (
                      nx40)) ;
    OR6_X0P5M_A12TS ix72661 (.Y (nx72660), .A (x_coord[3]), .B (y_coord[2]), .C (
                    x_coord[6]), .D (x_coord[1]), .E (x_coord[4]), .F (nx72662)
                    ) ;
    NAND4_X0P5A_A12TS ix72668 (.Y (nx72667), .A (nx538), .B (nx434), .C (nx548)
                      , .D (nx414)) ;
    XNOR2_X0P5M_A12TS ix539 (.Y (nx538), .A (y_coord[0]), .B (a_12__dup_60616)
                      ) ;
    NAND2_X0P5A_A12TS ix559 (.Y (nx558), .A (nx72674), .B (nx72383)) ;
    OR6_X0P5M_A12TS ix72675 (.Y (nx72674), .A (nx72660), .B (x_coord[5]), .C (
                    x_coord[2]), .D (y_coord[3]), .E (nx72676), .F (nx72667)) ;
    XOR2_X0P5M_A12TS ix72677 (.Y (nx72676), .A (x_coord[0]), .B (a_12__dup_60616
                     )) ;
    NOR2_X0P5A_A12TS ix657 (.Y (nx656), .A (nx72417), .B (a_12__dup_60616)) ;
    XOR2_X0P5M_A12TS ix72680 (.Y (nx72679), .A (nx72417), .B (a_12__dup_60616)
                     ) ;
    NAND2_X0P5A_A12TS ix72683 (.Y (nx72682), .A (nx152), .B (nx72383)) ;
    XNOR2_X0P5M_A12TS ix72687 (.Y (nx72686), .A (nx152), .B (a_12__dup_60616)) ;
    OAI22_X0P5M_A12TS ix635 (.Y (nx634), .A0 (nx72689), .A1 (nx72674), .B0 (
                      nx72383), .B1 (nx72713)) ;
    CGENI_X1M_A12TS ix72690 (.CON (nx72689), .A (nx166), .B (nx72383), .CI (
                    nx608)) ;
    CGENI_X1M_A12TS ix609 (.CON (nx608), .A (nx72437), .B (a_12__dup_60616), .CI (
                    nx72692)) ;
    OA21A1OI2_X0P5M_A12TS ix72693 (.Y (nx72692), .A0 (nx592), .A1 (nx68), .B0 (
                          nx72441), .C0 (nx58)) ;
    NOR3_X0P5A_A12TS ix593 (.Y (nx592), .A (nx72695), .B (nx72446), .C (nx66)) ;
    AOI21_X0P5M_A12TS ix72696 (.Y (nx72695), .A0 (nx72456), .A1 (nx72697), .B0 (
                      nx578)) ;
    XOR2_X0P5M_A12TS ix72698 (.Y (nx72697), .A (nx72699), .B (a_12__dup_60616)
                     ) ;
    OAI21_X0P5M_A12TS ix72700 (.Y (nx72699), .A0 (x_coord[0]), .A1 (x_coord[1])
                      , .B0 (nx72701)) ;
    NAND2_X0P5A_A12TS ix72702 (.Y (nx72701), .A (x_coord[1]), .B (x_coord[0])) ;
    OAI211_X0P5M_A12TS ix579 (.Y (nx578), .A0 (nx72699), .A1 (a_12__dup_60616), 
                       .B0 (nx72704), .C0 (nx72706)) ;
    XNOR2_X0P5M_A12TS ix72705 (.Y (nx72704), .A (nx74), .B (a_12__dup_60616)) ;
    XOR2_X0P5M_A12TS ix72707 (.Y (nx72706), .A (nx72450), .B (a_12__dup_60616)
                     ) ;
    XNOR2_X0P5M_A12TS ix67 (.Y (nx66), .A (nx72444), .B (a_12__dup_60616)) ;
    NOR2_X0P5A_A12TS ix69 (.Y (nx68), .A (nx72444), .B (a_12__dup_60616)) ;
    NAND2_X0P5A_A12TS ix72714 (.Y (nx72713), .A (nx616), .B (nx628)) ;
    NAND4_X0P5A_A12TS ix629 (.Y (nx628), .A (nx72717), .B (nx72719), .C (nx72721
                      ), .D (nx72676)) ;
    NOR3_X0P5A_A12TS ix72718 (.Y (nx72717), .A (nx58), .B (nx68), .C (nx578)) ;
    XNOR2_X0P5M_A12TS ix72720 (.Y (nx72719), .A (nx166), .B (a_12__dup_60616)) ;
    XOR2_X0P5M_A12TS ix72722 (.Y (nx72721), .A (nx72437), .B (a_12__dup_60616)
                     ) ;
    XNOR2_X0P5M_A12TS ix72725 (.Y (nx72724), .A (nx266), .B (a_12__dup_60616)) ;
    AOI21_X0P5M_A12TS ix267 (.Y (nx266), .A0 (nx72526), .A1 (nx72534), .B0 (
                      nx260)) ;
    XOR2_X0P5M_A12TS ix72728 (.Y (nx72727), .A (nx72645), .B (a_12__dup_60616)
                     ) ;
    XNOR2_X0P5M_A12TS ix72730 (.Y (nx72729), .A (nx284), .B (a_12__dup_60616)) ;
    AOI21_X0P5M_A12TS ix285 (.Y (nx284), .A0 (nx72647), .A1 (nx72530), .B0 (
                      nx278)) ;
    OAI22_X0P5M_A12TS ix825 (.Y (nx824), .A0 (nx72536), .A1 (nx72733), .B0 (
                      nx256), .B1 (nx72752)) ;
    AOI22_X0P5M_A12TS ix72734 (.Y (nx72733), .A0 (nx668), .A1 (nx558), .B0 (
                      nx72738), .B1 (nx660)) ;
    XNOR2_X0P5M_A12TS ix72739 (.Y (nx72738), .A (nx136), .B (a_12__dup_60616)) ;
    AOI32_X0P5M_A12TS ix72745 (.Y (nx72744), .A0 (nx72686), .A1 (nx72422), .A2 (
                      nx634), .B0 (nx558), .B1 (nx562)) ;
    AOI32_X0P5M_A12TS ix72753 (.Y (nx72752), .A0 (nx672), .A1 (nx294), .A2 (
                      nx72383), .B0 (nx72754), .B1 (nx810)) ;
    XNOR2_X0P5M_A12TS ix72755 (.Y (nx72754), .A (nx294), .B (a_12__dup_60616)) ;
    OAI31_X0P5M_A12TS ix811 (.Y (nx810), .A0 (nx72733), .A1 (nx72611), .A2 (
                      a_12__dup_60616), .B0 (nx72757)) ;
    AO21A1AI2_X0P5M_A12TS ix72758 (.Y (nx72757), .A0 (nx314), .A1 (nx792), .B0 (
                          nx800), .C0 (nx72609)) ;
    OAI22_X0P5M_A12TS ix793 (.Y (nx792), .A0 (nx72383), .A1 (nx72763), .B0 (
                      nx72771), .B1 (nx72674)) ;
    OA21A1OI2_X0P5M_A12TS ix72764 (.Y (nx72763), .A0 (nx708), .A1 (nx656), .B0 (
                          nx72738), .C0 (nx668)) ;
    AOI21_X0P5M_A12TS ix709 (.Y (nx708), .A0 (nx72766), .A1 (nx72682), .B0 (
                      nx488)) ;
    AO21A1AI2_X0P5M_A12TS ix72767 (.Y (nx72766), .A0 (nx72422), .A1 (nx630), .B0 (
                          nx640), .C0 (nx72686)) ;
    OA21A1OI2_X0P5M_A12TS ix72772 (.Y (nx72771), .A0 (nx736), .A1 (nx656), .B0 (
                          nx72738), .C0 (nx668)) ;
    AOI21_X0P5M_A12TS ix737 (.Y (nx736), .A0 (nx72774), .A1 (nx72682), .B0 (
                      nx488)) ;
    AO21A1AI2_X0P5M_A12TS ix72775 (.Y (nx72774), .A0 (nx72422), .A1 (nx616), .B0 (
                          nx640), .C0 (nx72686)) ;
    OA21A1OI2_X0P5M_A12TS ix801 (.Y (nx800), .A0 (nx72777), .A1 (nx72785), .B0 (
                          nx72787), .C0 (nx312)) ;
    CGENI_X1M_A12TS ix72778 (.CON (nx72777), .A (nx328), .B (nx72383), .CI (
                    nx778)) ;
    AOI31_X0P5M_A12TS ix779 (.Y (nx778), .A0 (nx72596), .A1 (nx72512), .A2 (
                      nx72783), .B0 (a_12__dup_60616)) ;
    OAI21_X0P5M_A12TS ix72784 (.Y (nx72783), .A0 (y_coord[1]), .A1 (y_coord[0])
                      , .B0 (nx72599)) ;
    INV_X0P5B_A12TS ix72786 (.Y (nx72785), .A (nx792)) ;
    NAND4_X0P5A_A12TS ix72788 (.Y (nx72787), .A (nx692), .B (nx748), .C (nx72795
                      ), .D (nx72797)) ;
    NOR3_X0P5A_A12TS ix693 (.Y (nx692), .A (nx682), .B (y_coord[0]), .C (nx338)
                     ) ;
    XOR2_X0P5M_A12TS ix683 (.Y (nx682), .A (nx362), .B (a_12__dup_60616)) ;
    XNOR2_X0P5M_A12TS ix339 (.Y (nx338), .A (nx72512), .B (a_12__dup_60616)) ;
    OAI22_X0P5M_A12TS ix749 (.Y (nx748), .A0 (nx72763), .A1 (nx72793), .B0 (
                      nx72771), .B1 (nx72674)) ;
    NAND2_X0P5A_A12TS ix72794 (.Y (nx72793), .A (a_12__dup_60616), .B (
                      y_coord[0])) ;
    XOR2_X0P5M_A12TS ix72796 (.Y (nx72795), .A (nx72516), .B (a_12__dup_60616)
                     ) ;
    XOR2_X0P5M_A12TS ix72798 (.Y (nx72797), .A (nx72783), .B (a_12__dup_60616)
                     ) ;
    NAND2_X0P5A_A12TS ix72800 (.Y (nx72799), .A (nx472), .B (nx72818)) ;
    NOR3_X0P5A_A12TS ix473 (.Y (nx472), .A (nx72802), .B (nx72626), .C (nx72618)
                     ) ;
    OAI22_X0P5M_A12TS ix72805 (.Y (nx72804), .A0 (nx226), .A1 (nx184), .B0 (
                      nx170), .B1 (nx140)) ;
    OA21A1OI2_X0P5M_A12TS ix227 (.Y (nx226), .A0 (nx72383), .A1 (x_coord[4]), .B0 (
                          nx72807), .C0 (x_coord[6])) ;
    NAND4_X0P5A_A12TS ix72808 (.Y (nx72807), .A (nx204), .B (nx72474), .C (
                      nx72476), .D (nx72809)) ;
    XNOR2_X0P5M_A12TS ix72810 (.Y (nx72809), .A (a_12__dup_60616), .B (
                      x_coord[4])) ;
    AOI21_X0P5M_A12TS ix72816 (.Y (nx72815), .A0 (nx72441), .A1 (nx116), .B0 (
                      nx58)) ;
    CGENI_X1M_A12TS ix117 (.CON (nx116), .A (nx72446), .B (nx72444), .CI (
                    a_12__dup_60616)) ;
    AOI31_X0P5M_A12TS ix72819 (.Y (nx72818), .A0 (nx232), .A1 (nx958), .A2 (
                      nx938), .B0 (nx1042)) ;
    NOR3_X0P5A_A12TS ix233 (.Y (nx232), .A (x_coord[8]), .B (nx72479), .C (
                     nx72804)) ;
    MXIT2_X0P5M_A12TS ix72826 (.Y (nx72825), .A (a_12__dup_60616), .B (nx440), .S0 (
                      nx72555)) ;
    NOR2_X0P5A_A12TS ix72831 (.Y (nx72830), .A (nx1032), .B (nx854)) ;
    AO21A1AI2_X0P5M_A12TS ix855 (.Y (nx854), .A0 (nx72834), .A1 (nx72643), .B0 (
                          nx72733), .C0 (nx72837)) ;
    OAI31_X0P5M_A12TS ix72835 (.Y (nx72834), .A0 (nx284), .A1 (nx274), .A2 (
                      nx266), .B0 (nx72383)) ;
    INV_X0P5B_A12TS ix275 (.Y (nx274), .A (nx72645)) ;
    NAND4_X0P5A_A12TS ix72838 (.Y (nx72837), .A (nx72724), .B (nx72727), .C (
                      nx72729), .D (nx824)) ;
    OAI221_X0P5M_A12TS ix1143 (.Y (nx1142), .A0 (nx72840), .A1 (nx1026), .B0 (
                       nx1042), .B1 (nx72818), .C0 (nx72799)) ;
    NOR3_X0P5A_A12TS ix72841 (.Y (nx72840), .A (nx1026), .B (nx916), .C (nx854)
                     ) ;
    XOR2_X0P5M_A12TS ix919 (.Y (nx918), .A (a_12__dup_60616), .B (y_coord[6])) ;
    AOI211_X0P5M_A12TS ix72856 (.Y (nx72855), .A0 (nx72522), .A1 (nx1012), .B0 (
                       nx288), .C0 (nx258)) ;
    NAND4_X0P5A_A12TS ix1013 (.Y (nx1012), .A (nx72591), .B (nx72611), .C (
                      nx72858), .D (nx72520)) ;
    OAI21_X0P5M_A12TS ix72859 (.Y (nx72858), .A0 (nx1000), .A1 (nx328), .B0 (
                      nx72607)) ;
    CGENI_X1M_A12TS ix1001 (.CON (nx1000), .A (nx72596), .B (a_12__dup_60616), .CI (
                    nx72603)) ;
    NOR3_X0P5A_A12TS ix917 (.Y (nx916), .A (nx72618), .B (nx72559), .C (nx72626)
                     ) ;
    OAI211_X0P5M_A12TS ix1119 (.Y (nx1118), .A0 (nx72865), .A1 (nx72869), .B0 (
                       nx72799), .C0 (nx72871)) ;
    AOI211_X0P5M_A12TS ix72866 (.Y (nx72865), .A0 (nx232), .A1 (nx1036), .B0 (
                       nx1034), .C0 (nx854)) ;
    NAND2_X0P5A_A12TS ix72870 (.Y (nx72869), .A (nx72636), .B (nx72830)) ;
    AOI31_X0P5M_A12TS ix72872 (.Y (nx72871), .A0 (nx1032), .A1 (nx72616), .A2 (
                      nx72361), .B0 (nx1104)) ;
    AOI211_X0P5M_A12TS ix1105 (.Y (nx1104), .A0 (nx72874), .A1 (nx72840), .B0 (
                       nx962), .C0 (nx72361)) ;
    NAND2_X0P5A_A12TS ix72875 (.Y (nx72874), .A (nx72830), .B (nx72818)) ;
    NAND2_X0P5A_A12TS ix1067 (.Y (nx1066), .A (nx72799), .B (nx1032)) ;
    TIEHI_X1M_A12TS ix49 (.Y (nx48)) ;
    NOR2_X0P5A_A12TS ix5 (.Y (nx4), .A (reset), .B (map_irq_pulse_n)) ;
    TIELO_X1M_A12TS ix72326 (.Y (nx72325)) ;
    DFFQ_X0P5M_A12TS reg_found_match (.Q (touch_pulse), .CK (osc_clk), .D (
                     nx1052)) ;
    AOI21_X0P5M_A12TS ix1053 (.Y (nx1052), .A0 (nx72818), .A1 (nx72882), .B0 (
                      nx72884)) ;
    INV_X0P5B_A12TS ix1043 (.Y (nx1042), .A (nx72865)) ;
    INV_X0P5B_A12TS ix1035 (.Y (nx1034), .A (nx72830)) ;
    INV_X0P5B_A12TS ix1033 (.Y (nx1032), .A (nx72840)) ;
    INV_X0P5B_A12TS ix1027 (.Y (nx1026), .A (nx72361)) ;
    INV_X0P5B_A12TS ix1021 (.Y (nx1020), .A (nx72855)) ;
    INV_X0P5B_A12TS ix72590 (.Y (nx72589), .A (nx958)) ;
    INV_X0P5B_A12TS ix939 (.Y (nx938), .A (nx72545)) ;
    INV_X0P5B_A12TS ix72617 (.Y (nx72616), .A (nx916)) ;
    INV_X0P5B_A12TS ix913 (.Y (nx912), .A (nx72559)) ;
    INV_X0P5B_A12TS ix72637 (.Y (nx72636), .A (nx854)) ;
    INV_X0P5B_A12TS ix673 (.Y (nx672), .A (nx72733)) ;
    INV_X0P5B_A12TS ix669 (.Y (nx668), .A (nx72655)) ;
    INV_X0P5B_A12TS ix631 (.Y (nx630), .A (nx72713)) ;
    INV_X0P5B_A12TS ix617 (.Y (nx616), .A (nx72689)) ;
    INV_X0P5B_A12TS ix563 (.Y (nx562), .A (nx72682)) ;
    INV_X0P5B_A12TS ix489 (.Y (nx488), .A (nx72679)) ;
    INV_X0P5B_A12TS ix72883 (.Y (nx72882), .A (nx472)) ;
    INV_X0P5B_A12TS ix431 (.Y (nx430), .A (nx72550)) ;
    INV_X0P5B_A12TS ix72386 (.Y (nx72385), .A (nx404)) ;
    INV_X0P5B_A12TS ix72597 (.Y (nx72596), .A (nx362)) ;
    INV_X0P5B_A12TS ix337 (.Y (nx336), .A (nx72512)) ;
    INV_X0P5B_A12TS ix329 (.Y (nx328), .A (nx72516)) ;
    INV_X0P5B_A12TS ix315 (.Y (nx314), .A (nx72520)) ;
    INV_X0P5B_A12TS ix313 (.Y (nx312), .A (nx72607)) ;
    INV_X0P5B_A12TS ix72592 (.Y (nx72591), .A (nx294)) ;
    INV_X0P5B_A12TS ix72644 (.Y (nx72643), .A (nx278)) ;
    INV_X0P5B_A12TS ix72537 (.Y (nx72536), .A (nx258)) ;
    INV_X0P5B_A12TS ix257 (.Y (nx256), .A (nx72522)) ;
    INV_X0P5B_A12TS ix72803 (.Y (nx72802), .A (nx232)) ;
    INV_X0P5B_A12TS ix161 (.Y (nx160), .A (nx72437)) ;
    INV_X0P5B_A12TS ix72748 (.Y (nx72747), .A (nx152)) ;
    INV_X0P5B_A12TS ix59 (.Y (nx58), .A (nx72458)) ;
    INV_X0P5B_A12TS ix57 (.Y (nx56), .A (nx72441)) ;
    INV_X0P5B_A12TS ix72587 (.Y (nx72586), .A (nx40)) ;
    INV_X0P5B_A12TS ix72750 (.Y (nx72749), .A (nx36)) ;
    INV_X0P5B_A12TS ix72480 (.Y (nx72479), .A (nx434)) ;
    INV_X0P5B_A12TS ix72885 (.Y (nx72884), .A (nx4)) ;
    NAND2B_X0P7M_A12TS ix921 (.Y (nx920), .AN (nx918), .B (nx72377)) ;
    AND2_X0P5M_A12TS ix72453 (.Y (nx72452), .A (x_coord[1]), .B (x_coord[0])) ;
    NAND2B_X0P7M_A12TS ix72546 (.Y (nx72545), .AN (nx72825), .B (nx548)) ;
    NAND2B_X0P7M_A12TS ix72551 (.Y (nx72550), .AN (y_coord[2]), .B (nx426)) ;
    NAND3B_X0P5M_A12TS ix72563 (.Y (nx72562), .AN (x_coord[6]), .B (nx204), .C (
                       nx72570)) ;
    NAND4B_X0P5M_A12TS ix909 (.Y (nx908), .AN (nx640), .B (nx72388), .C (nx72576
                       ), .D (nx72586)) ;
    NAND4B_X0P5M_A12TS ix289 (.Y (nx288), .AN (y_coord[10]), .B (nx72530), .C (
                       nx72534), .D (nx72526)) ;
    NOR2B_X0P7M_A12TS ix72633 (.Y (nx72632), .AN (nx72603), .B (nx362)) ;
    OR4_X0P5M_A12TS ix72663 (.Y (nx72662), .A (y_coord[7]), .B (y_coord[1]), .C (
                    x_coord[8]), .D (x_coord[7])) ;
    AO22_X0P5M_A12TS ix661 (.Y (nx660), .A0 (nx558), .A1 (nx656), .B0 (nx72679)
                     , .B1 (nx72891)) ;
    INV_X0P5B_A12TS ix72890 (.Y (nx72891), .A (nx72744)) ;
    NAND4B_X0P5M_A12TS ix171 (.Y (nx170), .AN (nx166), .B (nx72437), .C (nx72747
                       ), .D (nx72424)) ;
    NAND4B_X0P5M_A12TS ix141 (.Y (nx140), .AN (x_coord[10]), .B (nx72413), .C (
                       nx72749), .D (nx72815)) ;
    NAND4B_X0P5M_A12TS ix959 (.Y (nx958), .AN (nx322), .B (nx72524), .C (nx72591
                       ), .D (nx72593)) ;
    NOR2B_X0P7M_A12TS ix1037 (.Y (nx1036), .AN (nx992), .B (nx72855)) ;
    DFFSRPQ_X1M_A12TS reg_memory_3__24 (.Q (a_12__dup_60616), .CK (osc_clk), .D (
                      nx72991), .R (nx72325), .SN (nx73005)) ;
    MXT2_X0P5M_A12TS ix72992 (.Y (nx72991), .A (a_12__dup_60616), .B (nx48), .S0 (
                     reset)) ;
    INV_X0P5B_A12TS ix73006 (.Y (nx73005), .A (nx72325)) ;
    SDFFSRPQ_X0P5M_A12TS reg_button_num_0 (.Q (button_num_0), .CK (osc_clk), .D (
                         nx1118), .R (nx72325), .SE (NOT_nx4), .SI (button_num_0
                         ), .SN (nx73005)) ;
    INV_X0P5B_A12TS ix73009 (.Y (NOT_nx4), .A (nx4)) ;
    SDFFSRPQ_X0P5M_A12TS reg_button_num_1 (.Q (button_num_1), .CK (osc_clk), .D (
                         nx1142), .R (nx72325), .SE (NOT_nx4), .SI (button_num_1
                         ), .SN (nx73005)) ;
    SDFFSRPQ_X0P5M_A12TS reg_button_num_2 (.Q (button_num_2), .CK (osc_clk), .D (
                         nx1164), .R (nx72325), .SE (NOT_nx4), .SI (button_num_2
                         ), .SN (nx73005)) ;
    SDFFSRPQ_X0P5M_A12TS reg_button_num_3 (.Q (button_num_3), .CK (osc_clk), .D (
                         nx1066), .R (nx72325), .SE (NOT_nx4), .SI (button_num_3
                         ), .SN (nx73005)) ;
    NOR2_X0P5A_A12TS ix113 (.Y (nx112), .A (reset), .B (contest_num[0])) ;
    NOR2_X0P5A_A12TS ix435 (.Y (nx448), .A (reset), .B (nx484)) ;
    MXIT2_X0P5M_A12TS ix485 (.Y (nx484), .A (nx447), .B (nx52), .S0 (nx492)) ;
    XNOR2_X0P5M_A12TS ix436 (.Y (nx447), .A (contest_num[2]), .B (nx487)) ;
    NAND2_X0P5A_A12TS ix488 (.Y (nx487), .A (contest_num[1]), .B (contest_num[0]
                      )) ;
    XOR2_X0P5M_A12TS ix53 (.Y (nx52), .A (contest_num[2]), .B (nx490)) ;
    NOR2_X0P5A_A12TS ix491 (.Y (nx490), .A (contest_num[0]), .B (contest_num[1])
                     ) ;
    NAND4B_X0P5M_A12TS ix493 (.Y (nx492), .AN (cast), .B (button_num_1), .C (
                       button_num_0), .D (nx82)) ;
    AOI31_X0P5M_A12TS ix83 (.Y (nx82), .A0 (contest_num[1]), .A1 (contest_num[0]
                      ), .A2 (contest_num[2]), .B0 (nx8)) ;
    NAND2_X0P5A_A12TS ix9 (.Y (nx8), .A (button_num_3), .B (button_num_2)) ;
    NOR2_X0P5A_A12TS ix43 (.Y (nx42), .A (reset), .B (nx497)) ;
    XNOR2_X0P5M_A12TS ix498 (.Y (nx497), .A (nx492), .B (nx499)) ;
    AOI21_X0P5M_A12TS ix500 (.Y (nx499), .A0 (contest_num[1]), .A1 (
                      contest_num[0]), .B0 (nx490)) ;
    INV_X0P5B_A12TS ix507 (.Y (nx506), .A (button_num_1)) ;
    INV_X0P5B_A12TS ix513 (.Y (NOT_reset), .A (reset)) ;
    OAI31_X0P5M_A12TS ix17 (.Y (nx16), .A0 (button_num_1), .A1 (nx515), .A2 (
                      nx517), .B0 (NOT_reset)) ;
    INV_X0P5B_A12TS ix516 (.Y (nx515), .A (button_num_0)) ;
    NAND3_X0P5A_A12TS ix518 (.Y (nx517), .A (touch_pulse), .B (button_num_3), .C (
                      button_num_2)) ;
    TIELO_X1M_A12TS ix462 (.Y (nx461)) ;
    AOI211_X0P5M_A12TS ix129 (.Y (ss_enable), .A0 (button_num_3), .A1 (
                       button_num_2), .B0 (nx521), .C0 (cast)) ;
    NAND2_X0P5A_A12TS ix522 (.Y (nx521), .A (touch_pulse), .B (NOT_reset)) ;
    SDFFSRPQ_X0P5M_A12TS reg_contest_num_0 (.Q (contest_num[0]), .CK (osc_clk), 
                         .D (nx112), .R (nx461), .SE (NOT_nx474), .SI (
                         contest_num[0]), .SN (nx629)) ;
    INV_X0P5B_A12TS ix630 (.Y (nx629), .A (nx461)) ;
    DFFSRPQ_X0P5M_A12TS reg_cast (.Q (cast), .CK (osc_clk), .D (nx614), .R (
                        nx461), .SN (nx629)) ;
    MXT2_X0P5M_A12TS ix615 (.Y (nx614), .A (cast), .B (NOT_reset), .S0 (nx16)) ;
    SDFFSRPQ_X0P5M_A12TS reg_contest_num_1 (.Q (contest_num[1]), .CK (osc_clk), 
                         .D (nx42), .R (nx461), .SE (NOT_nx474), .SI (
                         contest_num[1]), .SN (nx629)) ;
    SDFFSRPQ_X0P5M_A12TS reg_contest_num_2 (.Q (contest_num[2]), .CK (osc_clk), 
                         .D (nx448), .R (nx461), .SE (NOT_nx474), .SI (
                         contest_num[2]), .SN (nx629)) ;
    OA21A1OI2_X0P5M_A12TS ix107 (.Y (NOT_nx474), .A0 (nx449), .A1 (nx642), .B0 (
                          touch_pulse), .C0 (reset)) ;
    INV_X0P5B_A12TS ix437 (.Y (nx642), .A (nx492)) ;
    AOI211_X0P5M_A12TS ix503 (.Y (nx449), .A0 (nx490), .A1 (nx644), .B0 (nx646)
                       , .C0 (nx8)) ;
    INV_X0P5B_A12TS ix643 (.Y (nx644), .A (contest_num[2])) ;
    OR3_X0P5M_A12TS ix93 (.Y (nx646), .A (cast), .B (nx506), .C (button_num_0)
                    ) ;
    NOR3_X0P5A_A12TS ix450 (.Y (ss_selector_0), .A (nx457), .B (contest_num[1])
                     , .C (contest_num[0])) ;
    NOR3_X0P5A_A12TS ix45 (.Y (ss_selector_1), .A (nx457), .B (contest_num[1]), 
                     .C (nx165)) ;
    INV_X0P5B_A12TS ix166 (.Y (nx165), .A (contest_num[0])) ;
    NOR3_X0P5A_A12TS ix39 (.Y (ss_selector_2), .A (nx457), .B (nx168), .C (
                     contest_num[0])) ;
    INV_X0P5B_A12TS ix169 (.Y (nx168), .A (contest_num[1])) ;
    NOR3_X0P5A_A12TS ix451 (.Y (ss_selector_3), .A (nx457), .B (nx168), .C (
                     nx165)) ;
    NOR3_X0P5A_A12TS ix452 (.Y (ss_selector_4), .A (nx172), .B (contest_num[1])
                     , .C (contest_num[0])) ;
    NAND2_X0P5A_A12TS ix173 (.Y (nx172), .A (ss_enable), .B (contest_num[2])) ;
    NOR3_X0P5A_A12TS ix453 (.Y (ss_selector_5), .A (nx172), .B (contest_num[1])
                     , .C (nx165)) ;
    NOR3_X0P5A_A12TS ix11 (.Y (ss_selector_6), .A (nx172), .B (nx168), .C (
                     contest_num[0])) ;
    NOR3_X0P5A_A12TS ix454 (.Y (ss_selector_7), .A (nx172), .B (nx168), .C (
                     nx165)) ;
    NAND2B_X0P7M_A12TS ix456 (.Y (nx457), .AN (contest_num[2]), .B (ss_enable)
                       ) ;
    NOR2_X0P5A_A12TS ix499 (.Y (nx512), .A (reset), .B (ss_state_0_11)) ;
    OAI21_X0P5M_A12TS ix458 (.Y (nx511), .A0 (nx493), .A1 (nx2356), .B0 (nx2455)
                      ) ;
    NAND2_X0P5A_A12TS ix459 (.Y (nx493), .A (button_num_0), .B (button_num_1)) ;
    AOI21_X0P5M_A12TS ix2357 (.Y (nx2356), .A0 (ss_state_0_11), .A1 (nx494), .B0 (
                      nx2346)) ;
    NOR3_X0P5A_A12TS ix460 (.Y (nx494), .A (nx2359), .B (nx2361), .C (
                     button_num_2)) ;
    INV_X0P5B_A12TS ix2360 (.Y (nx2359), .A (ss_selector_0)) ;
    INV_X0P5B_A12TS ix2362 (.Y (nx2361), .A (button_num_3)) ;
    NOR3_X0P5A_A12TS ix483 (.Y (nx2346), .A (nx2364), .B (nx2361), .C (
                     button_num_2)) ;
    NAND2_X0P5A_A12TS ix2365 (.Y (nx2364), .A (ss_selector_0), .B (nx474)) ;
    MXIT2_X0P5M_A12TS ix475 (.Y (nx474), .A (nx2367), .B (nx2369), .S0 (nx2448)
                      ) ;
    INV_X0P5B_A12TS ix2368 (.Y (nx2367), .A (max_selections[3])) ;
    MXIT2_X0P5M_A12TS ix2370 (.Y (nx2369), .A (nx458), .B (max_selections[2]), .S0 (
                      nx436)) ;
    MXIT2_X0P5M_A12TS ix461 (.Y (nx458), .A (nx2372), .B (nx2374), .S0 (nx2404)
                      ) ;
    INV_X0P5B_A12TS ix2373 (.Y (nx2372), .A (max_selections[1])) ;
    NAND2_X0P5A_A12TS ix2375 (.Y (nx2374), .A (max_selections[0]), .B (nx2376)
                      ) ;
    XNOR2_X0P5M_A12TS ix2377 (.Y (nx2376), .A (nx2378), .B (nx2386)) ;
    XOR2_X0P5M_A12TS ix2379 (.Y (nx2378), .A (ss_state_0_1), .B (nx2380)) ;
    XOR2_X0P5M_A12TS ix2381 (.Y (nx2380), .A (ss_state_0_0), .B (nx2382)) ;
    OAI21_X0P5M_A12TS ix2383 (.Y (nx2382), .A0 (ss_state_0_2), .A1 (ss_state_0_3
                      ), .B0 (nx2384)) ;
    NAND2_X0P5A_A12TS ix2385 (.Y (nx2384), .A (ss_state_0_3), .B (ss_state_0_2)
                      ) ;
    XNOR2_X0P5M_A12TS ix2387 (.Y (nx2386), .A (nx2388), .B (nx2396)) ;
    XOR2_X0P5M_A12TS ix2389 (.Y (nx2388), .A (ss_state_0_5), .B (nx2390)) ;
    XOR2_X0P5M_A12TS ix2391 (.Y (nx2390), .A (ss_state_0_4), .B (nx2392)) ;
    OAI21_X0P5M_A12TS ix2393 (.Y (nx2392), .A0 (ss_state_0_6), .A1 (ss_state_0_7
                      ), .B0 (nx2394)) ;
    NAND2_X0P5A_A12TS ix2395 (.Y (nx2394), .A (ss_state_0_7), .B (ss_state_0_6)
                      ) ;
    XOR2_X0P5M_A12TS ix2397 (.Y (nx2396), .A (ss_state_0_9), .B (nx2398)) ;
    XOR2_X0P5M_A12TS ix2399 (.Y (nx2398), .A (ss_state_0_8), .B (nx2400)) ;
    OAI21_X0P5M_A12TS ix2401 (.Y (nx2400), .A0 (ss_state_0_10), .A1 (
                      ss_state_0_11), .B0 (nx2402)) ;
    NAND2_X0P5A_A12TS ix2403 (.Y (nx2402), .A (ss_state_0_11), .B (ss_state_0_10
                      )) ;
    XNOR3_X0P5M_A12TS ix2405 (.Y (nx2404), .A (max_selections[1]), .B (nx412), .C (
                      nx398)) ;
    NOR2_X0P5A_A12TS ix413 (.Y (nx412), .A (nx2378), .B (nx2386)) ;
    XNOR3_X0P5M_A12TS ix399 (.Y (nx398), .A (nx2408), .B (nx2384), .C (nx2413)
                      ) ;
    CGENI_X1M_A12TS ix2409 (.CON (nx2408), .A (ss_state_0_1), .B (ss_state_0_0)
                    , .CI (nx326)) ;
    XOR2_X0P5M_A12TS ix2414 (.Y (nx2413), .A (nx250), .B (nx2416)) ;
    NOR2_X0P5A_A12TS ix251 (.Y (nx250), .A (nx2388), .B (nx2396)) ;
    XNOR3_X0P5M_A12TS ix2417 (.Y (nx2416), .A (nx2418), .B (nx2394), .C (nx498)
                      ) ;
    CGENI_X1M_A12TS ix2419 (.CON (nx2418), .A (ss_state_0_5), .B (ss_state_0_4)
                    , .CI (nx176)) ;
    AOI21_X0P5M_A12TS ix464 (.Y (nx498), .A0 (nx2402), .A1 (nx2424), .B0 (nx106)
                      ) ;
    CGENI_X1M_A12TS ix2425 (.CON (nx2424), .A (ss_state_0_9), .B (ss_state_0_8)
                    , .CI (nx44)) ;
    XNOR3_X0P5M_A12TS ix465 (.Y (nx436), .A (max_selections[2]), .B (nx416), .C (
                      nx2434)) ;
    MXIT2_X0P5M_A12TS ix417 (.Y (nx416), .A (nx2413), .B (nx2432), .S0 (nx398)
                      ) ;
    XNOR2_X0P5M_A12TS ix2435 (.Y (nx2434), .A (nx2436), .B (nx2438)) ;
    NAND4_X0P5A_A12TS ix2437 (.Y (nx2436), .A (ss_state_0_1), .B (ss_state_0_0)
                      , .C (ss_state_0_3), .D (ss_state_0_2)) ;
    XNOR2_X0P5M_A12TS ix2439 (.Y (nx2438), .A (nx2440), .B (nx2442)) ;
    MXIT2_X0P5M_A12TS ix2441 (.Y (nx2440), .A (nx250), .B (nx498), .S0 (nx2416)
                      ) ;
    XNOR2_X0P5M_A12TS ix2443 (.Y (nx2442), .A (nx2444), .B (nx2446)) ;
    NAND4_X0P5A_A12TS ix2445 (.Y (nx2444), .A (ss_state_0_5), .B (ss_state_0_4)
                      , .C (ss_state_0_7), .D (ss_state_0_6)) ;
    NAND4_X0P5A_A12TS ix2447 (.Y (nx2446), .A (ss_state_0_9), .B (ss_state_0_8)
                      , .C (ss_state_0_11), .D (ss_state_0_10)) ;
    XNOR3_X0P5M_A12TS ix2449 (.Y (nx2448), .A (max_selections[3]), .B (nx420), .C (
                      nx428)) ;
    CGENI_X1M_A12TS ix421 (.CON (nx420), .A (nx2451), .B (nx2436), .CI (nx2438)
                    ) ;
    CGENI_X1M_A12TS ix429 (.CON (nx428), .A (nx2440), .B (nx2444), .CI (nx2446)
                    ) ;
    INV_X0P5B_A12TS ix2456 (.Y (nx2455), .A (reset)) ;
    NOR2_X0P5A_A12TS ix371 (.Y (nx370), .A (reset), .B (ss_state_0_1)) ;
    NAND2_X0P5A_A12TS ix365 (.Y (nx364), .A (nx2459), .B (nx2455)) ;
    AO21A1AI2_X0P5M_A12TS ix2460 (.Y (nx2459), .A0 (ss_state_0_1), .A1 (nx507), 
                          .B0 (nx505), .C0 (nx2463)) ;
    NOR3_X0P5A_A12TS ix466 (.Y (nx507), .A (nx2359), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR3_X0P5A_A12TS ix467 (.Y (nx505), .A (nx2364), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR2_X0P5A_A12TS ix468 (.Y (nx342), .A (reset), .B (ss_state_0_0)) ;
    NAND2_X0P5A_A12TS ix469 (.Y (nx510), .A (nx2469), .B (nx2455)) ;
    AO21A1AI2_X0P5M_A12TS ix2470 (.Y (nx2469), .A0 (ss_state_0_0), .A1 (nx507), 
                          .B0 (nx505), .C0 (nx2471)) ;
    NOR2_X0P5A_A12TS ix2472 (.Y (nx2471), .A (button_num_0), .B (button_num_1)
                     ) ;
    NOR2_X0P5A_A12TS ix470 (.Y (nx509), .A (reset), .B (ss_state_0_2)) ;
    NAND2_X0P5A_A12TS ix307 (.Y (nx306), .A (nx2475), .B (nx2455)) ;
    AO21A1AI2_X0P5M_A12TS ix2476 (.Y (nx2475), .A0 (ss_state_0_2), .A1 (nx507), 
                          .B0 (nx505), .C0 (nx2477)) ;
    NOR2_X0P5A_A12TS ix291 (.Y (nx290), .A (reset), .B (ss_state_0_3)) ;
    OAI21_X0P5M_A12TS ix471 (.Y (nx508), .A0 (nx493), .A1 (nx2483), .B0 (nx2455)
                      ) ;
    AOI21_X0P5M_A12TS ix2484 (.Y (nx2483), .A0 (ss_state_0_3), .A1 (nx507), .B0 (
                      nx505)) ;
    NOR2_X0P5A_A12TS ix221 (.Y (nx220), .A (reset), .B (ss_state_0_5)) ;
    NAND2_X0P5A_A12TS ix215 (.Y (nx214), .A (nx2487), .B (nx2455)) ;
    AO21A1AI2_X0P5M_A12TS ix2488 (.Y (nx2487), .A0 (ss_state_0_5), .A1 (nx501), 
                          .B0 (nx500), .C0 (nx2463)) ;
    NOR3_X0P5A_A12TS ix125 (.Y (nx501), .A (nx2359), .B (button_num_3), .C (
                     nx2490)) ;
    INV_X0P5B_A12TS ix2491 (.Y (nx2490), .A (button_num_2)) ;
    NOR3_X0P5A_A12TS ix472 (.Y (nx500), .A (nx2364), .B (button_num_3), .C (
                     nx2490)) ;
    NOR2_X0P5A_A12TS ix474 (.Y (nx504), .A (reset), .B (ss_state_0_4)) ;
    NAND2_X0P5A_A12TS ix187 (.Y (nx186), .A (nx2495), .B (nx2455)) ;
    AO21A1AI2_X0P5M_A12TS ix2496 (.Y (nx2495), .A0 (ss_state_0_4), .A1 (nx501), 
                          .B0 (nx500), .C0 (nx2471)) ;
    NOR2_X0P5A_A12TS ix163 (.Y (nx162), .A (reset), .B (ss_state_0_6)) ;
    NAND2_X0P5A_A12TS ix157 (.Y (nx156), .A (nx2499), .B (nx2455)) ;
    AO21A1AI2_X0P5M_A12TS ix2500 (.Y (nx2499), .A0 (ss_state_0_6), .A1 (nx501), 
                          .B0 (nx500), .C0 (nx2477)) ;
    NOR2_X0P5A_A12TS ix476 (.Y (nx503), .A (reset), .B (ss_state_0_7)) ;
    OAI21_X0P5M_A12TS ix477 (.Y (nx502), .A0 (nx493), .A1 (nx2503), .B0 (nx2455)
                      ) ;
    AOI21_X0P5M_A12TS ix2504 (.Y (nx2503), .A0 (ss_state_0_7), .A1 (nx501), .B0 (
                      nx500)) ;
    NOR2_X0P5A_A12TS ix95 (.Y (nx94), .A (reset), .B (ss_state_0_9)) ;
    NAND2_X0P5A_A12TS ix89 (.Y (nx88), .A (nx2507), .B (nx2455)) ;
    AO21A1AI2_X0P5M_A12TS ix2508 (.Y (nx2507), .A0 (ss_state_0_9), .A1 (nx494), 
                          .B0 (nx2346), .C0 (nx2463)) ;
    NOR2_X0P5A_A12TS ix63 (.Y (nx62), .A (reset), .B (ss_state_0_8)) ;
    NAND2_X0P5A_A12TS ix478 (.Y (nx496), .A (nx2511), .B (nx2455)) ;
    AO21A1AI2_X0P5M_A12TS ix2512 (.Y (nx2511), .A0 (ss_state_0_8), .A1 (nx494), 
                          .B0 (nx2346), .C0 (nx2471)) ;
    NOR2_X0P5A_A12TS ix31 (.Y (nx30), .A (reset), .B (ss_state_0_10)) ;
    NAND2_X0P5A_A12TS ix479 (.Y (nx495), .A (nx2515), .B (nx2455)) ;
    AO21A1AI2_X0P5M_A12TS ix2516 (.Y (nx2515), .A0 (ss_state_0_10), .A1 (nx494)
                          , .B0 (nx2346), .C0 (nx2477)) ;
    TIELO_X1M_A12TS ix2322 (.Y (nx2321)) ;
    INV_X0P5B_A12TS ix2452 (.Y (nx2451), .A (nx416)) ;
    INV_X0P5B_A12TS ix2433 (.Y (nx2432), .A (nx412)) ;
    INV_X0P5B_A12TS ix327 (.Y (nx326), .A (nx2382)) ;
    INV_X0P5B_A12TS ix177 (.Y (nx176), .A (nx2392)) ;
    INV_X0P5B_A12TS ix480 (.Y (nx106), .A (nx2446)) ;
    INV_X0P5B_A12TS ix481 (.Y (nx44), .A (nx2400)) ;
    NOR2B_X0P7M_A12TS ix2464 (.Y (nx2463), .AN (button_num_0), .B (button_num_1)
                      ) ;
    NOR2B_X0P7M_A12TS ix2478 (.Y (nx2477), .AN (button_num_1), .B (button_num_0)
                      ) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_0 (.Q (ss_state_0_0), .CK (osc_clk)
                        , .D (nx2625), .R (nx2321), .SN (nx2729)) ;
    MXT2_X0P5M_A12TS ix2626 (.Y (nx2625), .A (ss_state_0_0), .B (nx342), .S0 (
                     nx510)) ;
    INV_X0P5B_A12TS ix2730 (.Y (nx2729), .A (nx2321)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_1 (.Q (ss_state_0_1), .CK (osc_clk)
                        , .D (nx2615), .R (nx2321), .SN (nx2729)) ;
    MXT2_X0P5M_A12TS ix2616 (.Y (nx2615), .A (ss_state_0_1), .B (nx370), .S0 (
                     nx364)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_2 (.Q (ss_state_0_2), .CK (osc_clk)
                        , .D (nx2635), .R (nx2321), .SN (nx2729)) ;
    MXT2_X0P5M_A12TS ix2636 (.Y (nx2635), .A (ss_state_0_2), .B (nx509), .S0 (
                     nx306)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_3 (.Q (ss_state_0_3), .CK (osc_clk)
                        , .D (nx2645), .R (nx2321), .SN (nx2729)) ;
    MXT2_X0P5M_A12TS ix2646 (.Y (nx2645), .A (ss_state_0_3), .B (nx290), .S0 (
                     nx508)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_4 (.Q (ss_state_0_4), .CK (osc_clk)
                        , .D (nx2665), .R (nx2321), .SN (nx2729)) ;
    MXT2_X0P5M_A12TS ix2666 (.Y (nx2665), .A (ss_state_0_4), .B (nx504), .S0 (
                     nx186)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_5 (.Q (ss_state_0_5), .CK (osc_clk)
                        , .D (nx2655), .R (nx2321), .SN (nx2729)) ;
    MXT2_X0P5M_A12TS ix2656 (.Y (nx2655), .A (ss_state_0_5), .B (nx220), .S0 (
                     nx214)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_6 (.Q (ss_state_0_6), .CK (osc_clk)
                        , .D (nx2675), .R (nx2321), .SN (nx2729)) ;
    MXT2_X0P5M_A12TS ix2676 (.Y (nx2675), .A (ss_state_0_6), .B (nx162), .S0 (
                     nx156)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_7 (.Q (ss_state_0_7), .CK (osc_clk)
                        , .D (nx2685), .R (nx2321), .SN (nx2729)) ;
    MXT2_X0P5M_A12TS ix2686 (.Y (nx2685), .A (ss_state_0_7), .B (nx503), .S0 (
                     nx502)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_8 (.Q (ss_state_0_8), .CK (osc_clk)
                        , .D (nx2705), .R (nx2321), .SN (nx2729)) ;
    MXT2_X0P5M_A12TS ix2706 (.Y (nx2705), .A (ss_state_0_8), .B (nx62), .S0 (
                     nx496)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_9 (.Q (ss_state_0_9), .CK (osc_clk)
                        , .D (nx2695), .R (nx2321), .SN (nx2729)) ;
    MXT2_X0P5M_A12TS ix2696 (.Y (nx2695), .A (ss_state_0_9), .B (nx94), .S0 (
                     nx88)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_10 (.Q (ss_state_0_10), .CK (osc_clk
                        ), .D (nx2715), .R (nx2321), .SN (nx2729)) ;
    MXT2_X0P5M_A12TS ix2716 (.Y (nx2715), .A (ss_state_0_10), .B (nx30), .S0 (
                     nx495)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_11 (.Q (ss_state_0_11), .CK (osc_clk
                        ), .D (nx2605), .R (nx2321), .SN (nx2729)) ;
    MXT2_X0P5M_A12TS ix2606 (.Y (nx2605), .A (ss_state_0_11), .B (nx512), .S0 (
                     nx511)) ;
    NOR2_X0P5A_A12TS ix514 (.Y (nx706), .A (reset), .B (ss_state_1_11)) ;
    OAI21_X0P5M_A12TS ix515 (.Y (nx705), .A0 (nx657), .A1 (nx707), .B0 (nx747)
                      ) ;
    NAND2_X0P5A_A12TS ix517 (.Y (nx657), .A (button_num_0), .B (button_num_1)) ;
    AOI21_X0P5M_A12TS ix519 (.Y (nx707), .A0 (ss_state_1_11), .A1 (nx659), .B0 (
                      nx658)) ;
    NOR3_X0P5A_A12TS ix520 (.Y (nx659), .A (nx709), .B (nx710), .C (button_num_2
                     )) ;
    INV_X0P5B_A12TS ix521 (.Y (nx709), .A (ss_selector_1)) ;
    INV_X0P5B_A12TS ix523 (.Y (nx710), .A (button_num_3)) ;
    NOR3_X0P5A_A12TS ix524 (.Y (nx658), .A (nx711), .B (nx710), .C (button_num_2
                     )) ;
    NAND2_X0P5A_A12TS ix525 (.Y (nx711), .A (ss_selector_1), .B (nx704)) ;
    MXIT2_X0P5M_A12TS ix526 (.Y (nx704), .A (nx712), .B (nx713), .S0 (nx745)) ;
    INV_X0P5B_A12TS ix527 (.Y (nx712), .A (max_selections[3])) ;
    MXIT2_X0P5M_A12TS ix528 (.Y (nx713), .A (nx703), .B (max_selections[2]), .S0 (
                      nx702)) ;
    MXIT2_X0P5M_A12TS ix529 (.Y (nx703), .A (nx714), .B (nx715), .S0 (nx730)) ;
    INV_X0P5B_A12TS ix530 (.Y (nx714), .A (max_selections[1])) ;
    NAND2_X0P5A_A12TS ix531 (.Y (nx715), .A (max_selections[0]), .B (nx716)) ;
    XNOR2_X0P5M_A12TS ix532 (.Y (nx716), .A (nx717), .B (nx721)) ;
    XOR2_X0P5M_A12TS ix533 (.Y (nx717), .A (ss_state_1_1), .B (nx718)) ;
    XOR2_X0P5M_A12TS ix534 (.Y (nx718), .A (ss_state_1_0), .B (nx719)) ;
    OAI21_X0P5M_A12TS ix535 (.Y (nx719), .A0 (ss_state_1_2), .A1 (ss_state_1_3)
                      , .B0 (nx720)) ;
    NAND2_X0P5A_A12TS ix536 (.Y (nx720), .A (ss_state_1_3), .B (ss_state_1_2)) ;
    XNOR2_X0P5M_A12TS ix537 (.Y (nx721), .A (nx722), .B (nx726)) ;
    XOR2_X0P5M_A12TS ix538 (.Y (nx722), .A (ss_state_1_5), .B (nx723)) ;
    XOR2_X0P5M_A12TS ix540 (.Y (nx723), .A (ss_state_1_4), .B (nx724)) ;
    OAI21_X0P5M_A12TS ix541 (.Y (nx724), .A0 (ss_state_1_6), .A1 (ss_state_1_7)
                      , .B0 (nx725)) ;
    NAND2_X0P5A_A12TS ix542 (.Y (nx725), .A (ss_state_1_7), .B (ss_state_1_6)) ;
    XOR2_X0P5M_A12TS ix543 (.Y (nx726), .A (ss_state_1_9), .B (nx727)) ;
    XOR2_X0P5M_A12TS ix544 (.Y (nx727), .A (ss_state_1_8), .B (nx728)) ;
    OAI21_X0P5M_A12TS ix545 (.Y (nx728), .A0 (ss_state_1_10), .A1 (ss_state_1_11
                      ), .B0 (nx729)) ;
    NAND2_X0P5A_A12TS ix546 (.Y (nx729), .A (ss_state_1_11), .B (ss_state_1_10)
                      ) ;
    XNOR3_X0P5M_A12TS ix547 (.Y (nx730), .A (max_selections[1]), .B (nx698), .C (
                      nx697)) ;
    NOR2_X0P5A_A12TS ix548 (.Y (nx698), .A (nx717), .B (nx721)) ;
    XNOR3_X0P5M_A12TS ix550 (.Y (nx697), .A (nx731), .B (nx720), .C (nx732)) ;
    CGENI_X1M_A12TS ix551 (.CON (nx731), .A (ss_state_1_1), .B (ss_state_1_0), .CI (
                    nx691)) ;
    XOR2_X0P5M_A12TS ix552 (.Y (nx732), .A (nx684), .B (nx733)) ;
    NOR2_X0P5A_A12TS ix553 (.Y (nx684), .A (nx722), .B (nx726)) ;
    XNOR3_X0P5M_A12TS ix554 (.Y (nx733), .A (nx734), .B (nx725), .C (nx670)) ;
    CGENI_X1M_A12TS ix555 (.CON (nx734), .A (ss_state_1_5), .B (ss_state_1_4), .CI (
                    nx678)) ;
    AOI21_X0P5M_A12TS ix556 (.Y (nx670), .A0 (nx729), .A1 (nx735), .B0 (nx669)
                      ) ;
    CGENI_X1M_A12TS ix557 (.CON (nx735), .A (ss_state_1_9), .B (ss_state_1_8), .CI (
                    nx663)) ;
    XNOR3_X0P5M_A12TS ix558 (.Y (nx702), .A (max_selections[2]), .B (nx699), .C (
                      nx738)) ;
    MXIT2_X0P5M_A12TS ix560 (.Y (nx699), .A (nx732), .B (nx737), .S0 (nx697)) ;
    XNOR2_X0P5M_A12TS ix561 (.Y (nx738), .A (nx739), .B (nx740)) ;
    NAND4_X0P5A_A12TS ix562 (.Y (nx739), .A (ss_state_1_1), .B (ss_state_1_0), .C (
                      ss_state_1_3), .D (ss_state_1_2)) ;
    XNOR2_X0P5M_A12TS ix564 (.Y (nx740), .A (nx741), .B (nx742)) ;
    MXIT2_X0P5M_A12TS ix565 (.Y (nx741), .A (nx684), .B (nx670), .S0 (nx733)) ;
    XNOR2_X0P5M_A12TS ix566 (.Y (nx742), .A (nx743), .B (nx744)) ;
    NAND4_X0P5A_A12TS ix567 (.Y (nx743), .A (ss_state_1_5), .B (ss_state_1_4), .C (
                      ss_state_1_7), .D (ss_state_1_6)) ;
    NAND4_X0P5A_A12TS ix568 (.Y (nx744), .A (ss_state_1_9), .B (ss_state_1_8), .C (
                      ss_state_1_11), .D (ss_state_1_10)) ;
    XNOR3_X0P5M_A12TS ix569 (.Y (nx745), .A (max_selections[3]), .B (nx700), .C (
                      nx701)) ;
    CGENI_X1M_A12TS ix570 (.CON (nx700), .A (nx746), .B (nx739), .CI (nx740)) ;
    CGENI_X1M_A12TS ix571 (.CON (nx701), .A (nx741), .B (nx743), .CI (nx744)) ;
    INV_X0P5B_A12TS ix572 (.Y (nx747), .A (reset)) ;
    NOR2_X0P5A_A12TS ix573 (.Y (nx696), .A (reset), .B (ss_state_1_1)) ;
    NAND2_X0P5A_A12TS ix574 (.Y (nx695), .A (nx749), .B (nx747)) ;
    AO21A1AI2_X0P5M_A12TS ix575 (.Y (nx749), .A0 (ss_state_1_1), .A1 (nx686), .B0 (
                          nx685), .C0 (nx750)) ;
    NOR3_X0P5A_A12TS ix576 (.Y (nx686), .A (nx709), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR3_X0P5A_A12TS ix577 (.Y (nx685), .A (nx711), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR2_X0P5A_A12TS ix578 (.Y (nx694), .A (reset), .B (ss_state_1_0)) ;
    NAND2_X0P5A_A12TS ix580 (.Y (nx693), .A (nx751), .B (nx747)) ;
    AO21A1AI2_X0P5M_A12TS ix581 (.Y (nx751), .A0 (ss_state_1_0), .A1 (nx686), .B0 (
                          nx685), .C0 (nx752)) ;
    NOR2_X0P5A_A12TS ix582 (.Y (nx752), .A (button_num_0), .B (button_num_1)) ;
    NOR2_X0P5A_A12TS ix583 (.Y (nx690), .A (reset), .B (ss_state_1_2)) ;
    NAND2_X0P5A_A12TS ix584 (.Y (nx689), .A (nx753), .B (nx747)) ;
    AO21A1AI2_X0P5M_A12TS ix585 (.Y (nx753), .A0 (ss_state_1_2), .A1 (nx686), .B0 (
                          nx685), .C0 (nx754)) ;
    NOR2_X0P5A_A12TS ix586 (.Y (nx688), .A (reset), .B (ss_state_1_3)) ;
    OAI21_X0P5M_A12TS ix587 (.Y (nx687), .A0 (nx657), .A1 (nx755), .B0 (nx747)
                      ) ;
    AOI21_X0P5M_A12TS ix588 (.Y (nx755), .A0 (ss_state_1_3), .A1 (nx686), .B0 (
                      nx685)) ;
    NOR2_X0P5A_A12TS ix589 (.Y (nx683), .A (reset), .B (ss_state_1_5)) ;
    NAND2_X0P5A_A12TS ix590 (.Y (nx681), .A (nx756), .B (nx747)) ;
    AO21A1AI2_X0P5M_A12TS ix591 (.Y (nx756), .A0 (ss_state_1_5), .A1 (nx673), .B0 (
                          nx671), .C0 (nx750)) ;
    NOR3_X0P5A_A12TS ix592 (.Y (nx673), .A (nx709), .B (button_num_3), .C (nx757
                     )) ;
    INV_X0P5B_A12TS ix594 (.Y (nx757), .A (button_num_2)) ;
    NOR3_X0P5A_A12TS ix595 (.Y (nx671), .A (nx711), .B (button_num_3), .C (nx757
                     )) ;
    NOR2_X0P5A_A12TS ix596 (.Y (nx680), .A (reset), .B (ss_state_1_4)) ;
    NAND2_X0P5A_A12TS ix597 (.Y (nx679), .A (nx758), .B (nx747)) ;
    AO21A1AI2_X0P5M_A12TS ix598 (.Y (nx758), .A0 (ss_state_1_4), .A1 (nx673), .B0 (
                          nx671), .C0 (nx752)) ;
    NOR2_X0P5A_A12TS ix599 (.Y (nx677), .A (reset), .B (ss_state_1_6)) ;
    NAND2_X0P5A_A12TS ix600 (.Y (nx676), .A (nx759), .B (nx747)) ;
    AO21A1AI2_X0P5M_A12TS ix601 (.Y (nx759), .A0 (ss_state_1_6), .A1 (nx673), .B0 (
                          nx671), .C0 (nx754)) ;
    NOR2_X0P5A_A12TS ix602 (.Y (nx675), .A (reset), .B (ss_state_1_7)) ;
    OAI21_X0P5M_A12TS ix603 (.Y (nx674), .A0 (nx657), .A1 (nx760), .B0 (nx747)
                      ) ;
    AOI21_X0P5M_A12TS ix604 (.Y (nx760), .A0 (ss_state_1_7), .A1 (nx673), .B0 (
                      nx671)) ;
    NOR2_X0P5A_A12TS ix605 (.Y (nx667), .A (reset), .B (ss_state_1_9)) ;
    NAND2_X0P5A_A12TS ix606 (.Y (nx666), .A (nx761), .B (nx747)) ;
    AO21A1AI2_X0P5M_A12TS ix607 (.Y (nx761), .A0 (ss_state_1_9), .A1 (nx659), .B0 (
                          nx658), .C0 (nx750)) ;
    NOR2_X0P5A_A12TS ix608 (.Y (nx665), .A (reset), .B (ss_state_1_8)) ;
    NAND2_X0P5A_A12TS ix610 (.Y (nx664), .A (nx762), .B (nx747)) ;
    AO21A1AI2_X0P5M_A12TS ix611 (.Y (nx762), .A0 (ss_state_1_8), .A1 (nx659), .B0 (
                          nx658), .C0 (nx752)) ;
    NOR2_X0P5A_A12TS ix612 (.Y (nx662), .A (reset), .B (ss_state_1_10)) ;
    NAND2_X0P5A_A12TS ix613 (.Y (nx661), .A (nx763), .B (nx747)) ;
    AO21A1AI2_X0P5M_A12TS ix614 (.Y (nx763), .A0 (ss_state_1_10), .A1 (nx659), .B0 (
                          nx658), .C0 (nx754)) ;
    TIELO_X1M_A12TS ix616 (.Y (nx655)) ;
    INV_X0P5B_A12TS ix618 (.Y (nx746), .A (nx699)) ;
    INV_X0P5B_A12TS ix619 (.Y (nx737), .A (nx698)) ;
    INV_X0P5B_A12TS ix620 (.Y (nx691), .A (nx719)) ;
    INV_X0P5B_A12TS ix621 (.Y (nx678), .A (nx724)) ;
    INV_X0P5B_A12TS ix622 (.Y (nx669), .A (nx744)) ;
    INV_X0P5B_A12TS ix623 (.Y (nx663), .A (nx728)) ;
    NOR2B_X0P7M_A12TS ix624 (.Y (nx750), .AN (button_num_0), .B (button_num_1)
                      ) ;
    NOR2B_X0P7M_A12TS ix625 (.Y (nx754), .AN (button_num_1), .B (button_num_0)
                      ) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_0__dup_27820 (.Q (ss_state_1_0), .CK (
                        osc_clk), .D (nx766), .R (nx655), .SN (nx776)) ;
    MXT2_X0P5M_A12TS ix626 (.Y (nx766), .A (ss_state_1_0), .B (nx694), .S0 (
                     nx693)) ;
    INV_X0P5B_A12TS ix627 (.Y (nx776), .A (nx655)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_1__dup_27823 (.Q (ss_state_1_1), .CK (
                        osc_clk), .D (nx765), .R (nx655), .SN (nx776)) ;
    MXT2_X0P5M_A12TS ix628 (.Y (nx765), .A (ss_state_1_1), .B (nx696), .S0 (
                     nx695)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_2__dup_27825 (.Q (ss_state_1_2), .CK (
                        osc_clk), .D (nx767), .R (nx655), .SN (nx776)) ;
    MXT2_X0P5M_A12TS ix632 (.Y (nx767), .A (ss_state_1_2), .B (nx690), .S0 (
                     nx689)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_3__dup_27827 (.Q (ss_state_1_3), .CK (
                        osc_clk), .D (nx768), .R (nx655), .SN (nx776)) ;
    MXT2_X0P5M_A12TS ix633 (.Y (nx768), .A (ss_state_1_3), .B (nx688), .S0 (
                     nx687)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_4__dup_27829 (.Q (ss_state_1_4), .CK (
                        osc_clk), .D (nx770), .R (nx655), .SN (nx776)) ;
    MXT2_X0P5M_A12TS ix634 (.Y (nx770), .A (ss_state_1_4), .B (nx680), .S0 (
                     nx679)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_5__dup_27831 (.Q (ss_state_1_5), .CK (
                        osc_clk), .D (nx769), .R (nx655), .SN (nx776)) ;
    MXT2_X0P5M_A12TS ix636 (.Y (nx769), .A (ss_state_1_5), .B (nx683), .S0 (
                     nx681)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_6__dup_27833 (.Q (ss_state_1_6), .CK (
                        osc_clk), .D (nx771), .R (nx655), .SN (nx776)) ;
    MXT2_X0P5M_A12TS ix637 (.Y (nx771), .A (ss_state_1_6), .B (nx677), .S0 (
                     nx676)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_7__dup_27835 (.Q (ss_state_1_7), .CK (
                        osc_clk), .D (nx772), .R (nx655), .SN (nx776)) ;
    MXT2_X0P5M_A12TS ix638 (.Y (nx772), .A (ss_state_1_7), .B (nx675), .S0 (
                     nx674)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_8__dup_27837 (.Q (ss_state_1_8), .CK (
                        osc_clk), .D (nx774), .R (nx655), .SN (nx776)) ;
    MXT2_X0P5M_A12TS ix639 (.Y (nx774), .A (ss_state_1_8), .B (nx665), .S0 (
                     nx664)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_9__dup_27839 (.Q (ss_state_1_9), .CK (
                        osc_clk), .D (nx773), .R (nx655), .SN (nx776)) ;
    MXT2_X0P5M_A12TS ix640 (.Y (nx773), .A (ss_state_1_9), .B (nx667), .S0 (
                     nx666)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_10__dup_27841 (.Q (ss_state_1_10), .CK (
                        osc_clk), .D (nx775), .R (nx655), .SN (nx776)) ;
    MXT2_X0P5M_A12TS ix642 (.Y (nx775), .A (ss_state_1_10), .B (nx662), .S0 (
                     nx661)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_11__dup_27843 (.Q (ss_state_1_11), .CK (
                        osc_clk), .D (nx764), .R (nx655), .SN (nx776)) ;
    MXT2_X0P5M_A12TS ix644 (.Y (nx764), .A (ss_state_1_11), .B (nx706), .S0 (
                     nx705)) ;
    NOR2_X0P5A_A12TS ix777 (.Y (nx964), .A (reset), .B (ss_state_2_11)) ;
    OAI21_X0P5M_A12TS ix778 (.Y (nx963), .A0 (nx910), .A1 (nx965), .B0 (nx1008)
                      ) ;
    NAND2_X0P5A_A12TS ix780 (.Y (nx910), .A (button_num_0), .B (button_num_1)) ;
    AOI21_X0P5M_A12TS ix781 (.Y (nx965), .A0 (ss_state_2_11), .A1 (nx913), .B0 (
                      nx911)) ;
    NOR3_X0P5A_A12TS ix782 (.Y (nx913), .A (nx966), .B (nx967), .C (button_num_2
                     )) ;
    INV_X0P5B_A12TS ix783 (.Y (nx966), .A (ss_selector_2)) ;
    INV_X0P5B_A12TS ix784 (.Y (nx967), .A (button_num_3)) ;
    NOR3_X0P5A_A12TS ix785 (.Y (nx911), .A (nx968), .B (nx967), .C (button_num_2
                     )) ;
    NAND2_X0P5A_A12TS ix786 (.Y (nx968), .A (ss_selector_2), .B (nx961)) ;
    MXIT2_X0P5M_A12TS ix787 (.Y (nx961), .A (nx969), .B (nx970), .S0 (nx1006)) ;
    INV_X0P5B_A12TS ix788 (.Y (nx969), .A (max_selections[3])) ;
    MXIT2_X0P5M_A12TS ix789 (.Y (nx970), .A (nx960), .B (max_selections[2]), .S0 (
                      nx959)) ;
    MXIT2_X0P5M_A12TS ix790 (.Y (nx960), .A (nx971), .B (nx973), .S0 (nx990)) ;
    INV_X0P5B_A12TS ix791 (.Y (nx971), .A (max_selections[1])) ;
    NAND2_X0P5A_A12TS ix792 (.Y (nx973), .A (max_selections[0]), .B (nx975)) ;
    XNOR2_X0P5M_A12TS ix794 (.Y (nx975), .A (nx976), .B (nx980)) ;
    XOR2_X0P5M_A12TS ix795 (.Y (nx976), .A (ss_state_2_1), .B (nx977)) ;
    XOR2_X0P5M_A12TS ix796 (.Y (nx977), .A (ss_state_2_0), .B (nx978)) ;
    OAI21_X0P5M_A12TS ix797 (.Y (nx978), .A0 (ss_state_2_2), .A1 (ss_state_2_3)
                      , .B0 (nx979)) ;
    NAND2_X0P5A_A12TS ix798 (.Y (nx979), .A (ss_state_2_3), .B (ss_state_2_2)) ;
    XNOR2_X0P5M_A12TS ix799 (.Y (nx980), .A (nx981), .B (nx986)) ;
    XOR2_X0P5M_A12TS ix800 (.Y (nx981), .A (ss_state_2_5), .B (nx982)) ;
    XOR2_X0P5M_A12TS ix802 (.Y (nx982), .A (ss_state_2_4), .B (nx983)) ;
    OAI21_X0P5M_A12TS ix803 (.Y (nx983), .A0 (ss_state_2_6), .A1 (ss_state_2_7)
                      , .B0 (nx985)) ;
    NAND2_X0P5A_A12TS ix804 (.Y (nx985), .A (ss_state_2_7), .B (ss_state_2_6)) ;
    XOR2_X0P5M_A12TS ix805 (.Y (nx986), .A (ss_state_2_9), .B (nx987)) ;
    XOR2_X0P5M_A12TS ix806 (.Y (nx987), .A (ss_state_2_8), .B (nx988)) ;
    OAI21_X0P5M_A12TS ix807 (.Y (nx988), .A0 (ss_state_2_10), .A1 (ss_state_2_11
                      ), .B0 (nx989)) ;
    NAND2_X0P5A_A12TS ix808 (.Y (nx989), .A (ss_state_2_11), .B (ss_state_2_10)
                      ) ;
    XNOR3_X0P5M_A12TS ix809 (.Y (nx990), .A (max_selections[1]), .B (nx954), .C (
                      nx953)) ;
    NOR2_X0P5A_A12TS ix810 (.Y (nx954), .A (nx976), .B (nx980)) ;
    XNOR3_X0P5M_A12TS ix812 (.Y (nx953), .A (nx991), .B (nx979), .C (nx993)) ;
    CGENI_X1M_A12TS ix813 (.CON (nx991), .A (ss_state_2_1), .B (ss_state_2_0), .CI (
                    nx948)) ;
    XOR2_X0P5M_A12TS ix814 (.Y (nx993), .A (nx940), .B (nx994)) ;
    NOR2_X0P5A_A12TS ix815 (.Y (nx940), .A (nx981), .B (nx986)) ;
    XNOR3_X0P5M_A12TS ix816 (.Y (nx994), .A (nx995), .B (nx985), .C (nx927)) ;
    CGENI_X1M_A12TS ix817 (.CON (nx995), .A (ss_state_2_5), .B (ss_state_2_4), .CI (
                    nx934)) ;
    AOI21_X0P5M_A12TS ix818 (.Y (nx927), .A0 (nx989), .A1 (nx996), .B0 (nx926)
                      ) ;
    CGENI_X1M_A12TS ix819 (.CON (nx996), .A (ss_state_2_9), .B (ss_state_2_8), .CI (
                    nx917)) ;
    XNOR3_X0P5M_A12TS ix820 (.Y (nx959), .A (max_selections[2]), .B (nx955), .C (
                      nx998)) ;
    MXIT2_X0P5M_A12TS ix821 (.Y (nx955), .A (nx993), .B (nx997), .S0 (nx953)) ;
    XNOR2_X0P5M_A12TS ix822 (.Y (nx998), .A (nx999), .B (nx1001)) ;
    NAND4_X0P5A_A12TS ix823 (.Y (nx999), .A (ss_state_2_1), .B (ss_state_2_0), .C (
                      ss_state_2_3), .D (ss_state_2_2)) ;
    XNOR2_X0P5M_A12TS ix824 (.Y (nx1001), .A (nx1002), .B (nx1003)) ;
    MXIT2_X0P5M_A12TS ix826 (.Y (nx1002), .A (nx940), .B (nx927), .S0 (nx994)) ;
    XNOR2_X0P5M_A12TS ix827 (.Y (nx1003), .A (nx1004), .B (nx1005)) ;
    NAND4_X0P5A_A12TS ix828 (.Y (nx1004), .A (ss_state_2_5), .B (ss_state_2_4), 
                      .C (ss_state_2_7), .D (ss_state_2_6)) ;
    NAND4_X0P5A_A12TS ix829 (.Y (nx1005), .A (ss_state_2_9), .B (ss_state_2_8), 
                      .C (ss_state_2_11), .D (ss_state_2_10)) ;
    XNOR3_X0P5M_A12TS ix830 (.Y (nx1006), .A (max_selections[3]), .B (nx956), .C (
                      nx957)) ;
    CGENI_X1M_A12TS ix831 (.CON (nx956), .A (nx1007), .B (nx999), .CI (nx1001)
                    ) ;
    CGENI_X1M_A12TS ix832 (.CON (nx957), .A (nx1002), .B (nx1004), .CI (nx1005)
                    ) ;
    INV_X0P5B_A12TS ix833 (.Y (nx1008), .A (reset)) ;
    NOR2_X0P5A_A12TS ix834 (.Y (nx952), .A (reset), .B (ss_state_2_1)) ;
    NAND2_X0P5A_A12TS ix835 (.Y (nx951), .A (nx1009), .B (nx1008)) ;
    AO21A1AI2_X0P5M_A12TS ix836 (.Y (nx1009), .A0 (ss_state_2_1), .A1 (nx943), .B0 (
                          nx941), .C0 (nx1010)) ;
    NOR3_X0P5A_A12TS ix837 (.Y (nx943), .A (nx966), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR3_X0P5A_A12TS ix838 (.Y (nx941), .A (nx968), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR2_X0P5A_A12TS ix839 (.Y (nx950), .A (reset), .B (ss_state_2_0)) ;
    NAND2_X0P5A_A12TS ix840 (.Y (nx949), .A (nx1011), .B (nx1008)) ;
    AO21A1AI2_X0P5M_A12TS ix841 (.Y (nx1011), .A0 (ss_state_2_0), .A1 (nx943), .B0 (
                          nx941), .C0 (nx1013)) ;
    NOR2_X0P5A_A12TS ix842 (.Y (nx1013), .A (button_num_0), .B (button_num_1)) ;
    NOR2_X0P5A_A12TS ix843 (.Y (nx947), .A (reset), .B (ss_state_2_2)) ;
    NAND2_X0P5A_A12TS ix844 (.Y (nx946), .A (nx1014), .B (nx1008)) ;
    AO21A1AI2_X0P5M_A12TS ix845 (.Y (nx1014), .A0 (ss_state_2_2), .A1 (nx943), .B0 (
                          nx941), .C0 (nx1015)) ;
    NOR2_X0P5A_A12TS ix846 (.Y (nx945), .A (reset), .B (ss_state_2_3)) ;
    OAI21_X0P5M_A12TS ix847 (.Y (nx944), .A0 (nx910), .A1 (nx1016), .B0 (nx1008)
                      ) ;
    AOI21_X0P5M_A12TS ix848 (.Y (nx1016), .A0 (ss_state_2_3), .A1 (nx943), .B0 (
                      nx941)) ;
    NOR2_X0P5A_A12TS ix849 (.Y (nx939), .A (reset), .B (ss_state_2_5)) ;
    NAND2_X0P5A_A12TS ix850 (.Y (nx937), .A (nx1017), .B (nx1008)) ;
    AO21A1AI2_X0P5M_A12TS ix851 (.Y (nx1017), .A0 (ss_state_2_5), .A1 (nx929), .B0 (
                          nx928), .C0 (nx1010)) ;
    NOR3_X0P5A_A12TS ix852 (.Y (nx929), .A (nx966), .B (button_num_3), .C (
                     nx1018)) ;
    INV_X0P5B_A12TS ix853 (.Y (nx1018), .A (button_num_2)) ;
    NOR3_X0P5A_A12TS ix854 (.Y (nx928), .A (nx968), .B (button_num_3), .C (
                     nx1018)) ;
    NOR2_X0P5A_A12TS ix856 (.Y (nx936), .A (reset), .B (ss_state_2_4)) ;
    NAND2_X0P5A_A12TS ix857 (.Y (nx935), .A (nx1019), .B (nx1008)) ;
    AO21A1AI2_X0P5M_A12TS ix858 (.Y (nx1019), .A0 (ss_state_2_4), .A1 (nx929), .B0 (
                          nx928), .C0 (nx1013)) ;
    NOR2_X0P5A_A12TS ix859 (.Y (nx933), .A (reset), .B (ss_state_2_6)) ;
    NAND2_X0P5A_A12TS ix860 (.Y (nx932), .A (nx1021), .B (nx1008)) ;
    AO21A1AI2_X0P5M_A12TS ix861 (.Y (nx1021), .A0 (ss_state_2_6), .A1 (nx929), .B0 (
                          nx928), .C0 (nx1015)) ;
    NOR2_X0P5A_A12TS ix862 (.Y (nx931), .A (reset), .B (ss_state_2_7)) ;
    OAI21_X0P5M_A12TS ix863 (.Y (nx930), .A0 (nx910), .A1 (nx1022), .B0 (nx1008)
                      ) ;
    AOI21_X0P5M_A12TS ix864 (.Y (nx1022), .A0 (ss_state_2_7), .A1 (nx929), .B0 (
                      nx928)) ;
    NOR2_X0P5A_A12TS ix865 (.Y (nx925), .A (reset), .B (ss_state_2_9)) ;
    NAND2_X0P5A_A12TS ix866 (.Y (nx923), .A (nx1023), .B (nx1008)) ;
    AO21A1AI2_X0P5M_A12TS ix867 (.Y (nx1023), .A0 (ss_state_2_9), .A1 (nx913), .B0 (
                          nx911), .C0 (nx1010)) ;
    NOR2_X0P5A_A12TS ix868 (.Y (nx921), .A (reset), .B (ss_state_2_8)) ;
    NAND2_X0P5A_A12TS ix869 (.Y (nx919), .A (nx1024), .B (nx1008)) ;
    AO21A1AI2_X0P5M_A12TS ix870 (.Y (nx1024), .A0 (ss_state_2_8), .A1 (nx913), .B0 (
                          nx911), .C0 (nx1013)) ;
    NOR2_X0P5A_A12TS ix871 (.Y (nx915), .A (reset), .B (ss_state_2_10)) ;
    NAND2_X0P5A_A12TS ix872 (.Y (nx914), .A (nx1025), .B (nx1008)) ;
    AO21A1AI2_X0P5M_A12TS ix873 (.Y (nx1025), .A0 (ss_state_2_10), .A1 (nx913), 
                          .B0 (nx911), .C0 (nx1015)) ;
    TIELO_X1M_A12TS ix874 (.Y (nx909)) ;
    INV_X0P5B_A12TS ix875 (.Y (nx1007), .A (nx955)) ;
    INV_X0P5B_A12TS ix876 (.Y (nx997), .A (nx954)) ;
    INV_X0P5B_A12TS ix877 (.Y (nx948), .A (nx978)) ;
    INV_X0P5B_A12TS ix878 (.Y (nx934), .A (nx983)) ;
    INV_X0P5B_A12TS ix879 (.Y (nx926), .A (nx1005)) ;
    INV_X0P5B_A12TS ix880 (.Y (nx917), .A (nx988)) ;
    NOR2B_X0P7M_A12TS ix882 (.Y (nx1010), .AN (button_num_0), .B (button_num_1)
                      ) ;
    NOR2B_X0P7M_A12TS ix883 (.Y (nx1015), .AN (button_num_1), .B (button_num_0)
                      ) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_0__dup_27945 (.Q (ss_state_2_0), .CK (
                        osc_clk), .D (nx1029), .R (nx909), .SN (nx1044)) ;
    MXT2_X0P5M_A12TS ix884 (.Y (nx1029), .A (ss_state_2_0), .B (nx950), .S0 (
                     nx949)) ;
    INV_X0P5B_A12TS ix885 (.Y (nx1044), .A (nx909)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_1__dup_27948 (.Q (ss_state_2_1), .CK (
                        osc_clk), .D (nx1028), .R (nx909), .SN (nx1044)) ;
    MXT2_X0P5M_A12TS ix886 (.Y (nx1028), .A (ss_state_2_1), .B (nx952), .S0 (
                     nx951)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_2__dup_27950 (.Q (ss_state_2_2), .CK (
                        osc_clk), .D (nx1031), .R (nx909), .SN (nx1044)) ;
    MXT2_X0P5M_A12TS ix887 (.Y (nx1031), .A (ss_state_2_2), .B (nx947), .S0 (
                     nx946)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_3__dup_27952 (.Q (ss_state_2_3), .CK (
                        osc_clk), .D (nx1033), .R (nx909), .SN (nx1044)) ;
    MXT2_X0P5M_A12TS ix888 (.Y (nx1033), .A (ss_state_2_3), .B (nx945), .S0 (
                     nx944)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_4__dup_27954 (.Q (ss_state_2_4), .CK (
                        osc_clk), .D (nx1037), .R (nx909), .SN (nx1044)) ;
    MXT2_X0P5M_A12TS ix889 (.Y (nx1037), .A (ss_state_2_4), .B (nx936), .S0 (
                     nx935)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_5__dup_27956 (.Q (ss_state_2_5), .CK (
                        osc_clk), .D (nx1035), .R (nx909), .SN (nx1044)) ;
    MXT2_X0P5M_A12TS ix890 (.Y (nx1035), .A (ss_state_2_5), .B (nx939), .S0 (
                     nx937)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_6__dup_27958 (.Q (ss_state_2_6), .CK (
                        osc_clk), .D (nx1038), .R (nx909), .SN (nx1044)) ;
    MXT2_X0P5M_A12TS ix892 (.Y (nx1038), .A (ss_state_2_6), .B (nx933), .S0 (
                     nx932)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_7__dup_27960 (.Q (ss_state_2_7), .CK (
                        osc_clk), .D (nx1039), .R (nx909), .SN (nx1044)) ;
    MXT2_X0P5M_A12TS ix893 (.Y (nx1039), .A (ss_state_2_7), .B (nx931), .S0 (
                     nx930)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_8__dup_27962 (.Q (ss_state_2_8), .CK (
                        osc_clk), .D (nx1041), .R (nx909), .SN (nx1044)) ;
    MXT2_X0P5M_A12TS ix894 (.Y (nx1041), .A (ss_state_2_8), .B (nx921), .S0 (
                     nx919)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_9__dup_27964 (.Q (ss_state_2_9), .CK (
                        osc_clk), .D (nx1040), .R (nx909), .SN (nx1044)) ;
    MXT2_X0P5M_A12TS ix895 (.Y (nx1040), .A (ss_state_2_9), .B (nx925), .S0 (
                     nx923)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_10__dup_27966 (.Q (ss_state_2_10), .CK (
                        osc_clk), .D (nx1043), .R (nx909), .SN (nx1044)) ;
    MXT2_X0P5M_A12TS ix896 (.Y (nx1043), .A (ss_state_2_10), .B (nx915), .S0 (
                     nx914)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_11__dup_27968 (.Q (ss_state_2_11), .CK (
                        osc_clk), .D (nx1027), .R (nx909), .SN (nx1044)) ;
    MXT2_X0P5M_A12TS ix897 (.Y (nx1027), .A (ss_state_2_11), .B (nx964), .S0 (
                     nx963)) ;
    NOR2_X0P5A_A12TS ix1045 (.Y (nx1218), .A (reset), .B (ss_state_3_11)) ;
    OAI21_X0P5M_A12TS ix1046 (.Y (nx1217), .A0 (nx1174), .A1 (nx1219), .B0 (
                      nx1257)) ;
    NAND2_X0P5A_A12TS ix1047 (.Y (nx1174), .A (button_num_0), .B (button_num_1)
                      ) ;
    AOI21_X0P5M_A12TS ix1048 (.Y (nx1219), .A0 (ss_state_3_11), .A1 (nx1176), .B0 (
                      nx1175)) ;
    NOR3_X0P5A_A12TS ix1049 (.Y (nx1176), .A (nx1220), .B (nx1221), .C (
                     button_num_2)) ;
    INV_X0P5B_A12TS ix1050 (.Y (nx1220), .A (ss_selector_3)) ;
    INV_X0P5B_A12TS ix1051 (.Y (nx1221), .A (button_num_3)) ;
    NOR3_X0P5A_A12TS ix1052 (.Y (nx1175), .A (nx1222), .B (nx1221), .C (
                     button_num_2)) ;
    NAND2_X0P5A_A12TS ix1054 (.Y (nx1222), .A (ss_selector_3), .B (nx1216)) ;
    MXIT2_X0P5M_A12TS ix1055 (.Y (nx1216), .A (nx1223), .B (nx1224), .S0 (nx1255
                      )) ;
    INV_X0P5B_A12TS ix1056 (.Y (nx1223), .A (max_selections[3])) ;
    MXIT2_X0P5M_A12TS ix1057 (.Y (nx1224), .A (nx1215), .B (max_selections[2]), 
                      .S0 (nx1214)) ;
    MXIT2_X0P5M_A12TS ix1058 (.Y (nx1215), .A (nx1225), .B (nx1226), .S0 (nx1241
                      )) ;
    INV_X0P5B_A12TS ix1059 (.Y (nx1225), .A (max_selections[1])) ;
    NAND2_X0P5A_A12TS ix1060 (.Y (nx1226), .A (max_selections[0]), .B (nx1227)
                      ) ;
    XNOR2_X0P5M_A12TS ix1061 (.Y (nx1227), .A (nx1228), .B (nx1232)) ;
    XOR2_X0P5M_A12TS ix1062 (.Y (nx1228), .A (ss_state_3_1), .B (nx1229)) ;
    XOR2_X0P5M_A12TS ix1063 (.Y (nx1229), .A (ss_state_3_0), .B (nx1230)) ;
    OAI21_X0P5M_A12TS ix1064 (.Y (nx1230), .A0 (ss_state_3_2), .A1 (ss_state_3_3
                      ), .B0 (nx1231)) ;
    NAND2_X0P5A_A12TS ix1065 (.Y (nx1231), .A (ss_state_3_3), .B (ss_state_3_2)
                      ) ;
    XNOR2_X0P5M_A12TS ix1066 (.Y (nx1232), .A (nx1233), .B (nx1237)) ;
    XOR2_X0P5M_A12TS ix1068 (.Y (nx1233), .A (ss_state_3_5), .B (nx1234)) ;
    XOR2_X0P5M_A12TS ix1069 (.Y (nx1234), .A (ss_state_3_4), .B (nx1235)) ;
    OAI21_X0P5M_A12TS ix1070 (.Y (nx1235), .A0 (ss_state_3_6), .A1 (ss_state_3_7
                      ), .B0 (nx1236)) ;
    NAND2_X0P5A_A12TS ix1071 (.Y (nx1236), .A (ss_state_3_7), .B (ss_state_3_6)
                      ) ;
    XOR2_X0P5M_A12TS ix1072 (.Y (nx1237), .A (ss_state_3_9), .B (nx1238)) ;
    XOR2_X0P5M_A12TS ix1073 (.Y (nx1238), .A (ss_state_3_8), .B (nx1239)) ;
    OAI21_X0P5M_A12TS ix1074 (.Y (nx1239), .A0 (ss_state_3_10), .A1 (
                      ss_state_3_11), .B0 (nx1240)) ;
    NAND2_X0P5A_A12TS ix1075 (.Y (nx1240), .A (ss_state_3_11), .B (ss_state_3_10
                      )) ;
    XNOR3_X0P5M_A12TS ix1076 (.Y (nx1241), .A (max_selections[1]), .B (nx1210), 
                      .C (nx1209)) ;
    NOR2_X0P5A_A12TS ix1077 (.Y (nx1210), .A (nx1228), .B (nx1232)) ;
    XNOR3_X0P5M_A12TS ix1078 (.Y (nx1209), .A (nx1242), .B (nx1231), .C (nx1243)
                      ) ;
    CGENI_X1M_A12TS ix1079 (.CON (nx1242), .A (ss_state_3_1), .B (ss_state_3_0)
                    , .CI (nx1204)) ;
    XOR2_X0P5M_A12TS ix1080 (.Y (nx1243), .A (nx1197), .B (nx1244)) ;
    NOR2_X0P5A_A12TS ix1081 (.Y (nx1197), .A (nx1233), .B (nx1237)) ;
    XNOR3_X0P5M_A12TS ix1082 (.Y (nx1244), .A (nx1245), .B (nx1236), .C (nx1185)
                      ) ;
    CGENI_X1M_A12TS ix1083 (.CON (nx1245), .A (ss_state_3_5), .B (ss_state_3_4)
                    , .CI (nx1192)) ;
    AOI21_X0P5M_A12TS ix1084 (.Y (nx1185), .A0 (nx1240), .A1 (nx1246), .B0 (
                      nx1184)) ;
    CGENI_X1M_A12TS ix1085 (.CON (nx1246), .A (ss_state_3_9), .B (ss_state_3_8)
                    , .CI (nx1179)) ;
    XNOR3_X0P5M_A12TS ix1086 (.Y (nx1214), .A (max_selections[2]), .B (nx1211), 
                      .C (nx1248)) ;
    MXIT2_X0P5M_A12TS ix1087 (.Y (nx1211), .A (nx1243), .B (nx1247), .S0 (nx1209
                      )) ;
    XNOR2_X0P5M_A12TS ix1088 (.Y (nx1248), .A (nx1249), .B (nx1250)) ;
    NAND4_X0P5A_A12TS ix1089 (.Y (nx1249), .A (ss_state_3_1), .B (ss_state_3_0)
                      , .C (ss_state_3_3), .D (ss_state_3_2)) ;
    XNOR2_X0P5M_A12TS ix1090 (.Y (nx1250), .A (nx1251), .B (nx1252)) ;
    MXIT2_X0P5M_A12TS ix1091 (.Y (nx1251), .A (nx1197), .B (nx1185), .S0 (nx1244
                      )) ;
    XNOR2_X0P5M_A12TS ix1092 (.Y (nx1252), .A (nx1253), .B (nx1254)) ;
    NAND4_X0P5A_A12TS ix1093 (.Y (nx1253), .A (ss_state_3_5), .B (ss_state_3_4)
                      , .C (ss_state_3_7), .D (ss_state_3_6)) ;
    NAND4_X0P5A_A12TS ix1094 (.Y (nx1254), .A (ss_state_3_9), .B (ss_state_3_8)
                      , .C (ss_state_3_11), .D (ss_state_3_10)) ;
    XNOR3_X0P5M_A12TS ix1095 (.Y (nx1255), .A (max_selections[3]), .B (nx1212), 
                      .C (nx1213)) ;
    CGENI_X1M_A12TS ix1096 (.CON (nx1212), .A (nx1256), .B (nx1249), .CI (nx1250
                    )) ;
    CGENI_X1M_A12TS ix1097 (.CON (nx1213), .A (nx1251), .B (nx1253), .CI (nx1254
                    )) ;
    INV_X0P5B_A12TS ix1098 (.Y (nx1257), .A (reset)) ;
    NOR2_X0P5A_A12TS ix1099 (.Y (nx1208), .A (reset), .B (ss_state_3_1)) ;
    NAND2_X0P5A_A12TS ix1100 (.Y (nx1207), .A (nx1258), .B (nx1257)) ;
    AO21A1AI2_X0P5M_A12TS ix1101 (.Y (nx1258), .A0 (ss_state_3_1), .A1 (nx1199)
                          , .B0 (nx1198), .C0 (nx1259)) ;
    NOR3_X0P5A_A12TS ix1102 (.Y (nx1199), .A (nx1220), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR3_X0P5A_A12TS ix1103 (.Y (nx1198), .A (nx1222), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR2_X0P5A_A12TS ix1104 (.Y (nx1206), .A (reset), .B (ss_state_3_0)) ;
    NAND2_X0P5A_A12TS ix1106 (.Y (nx1205), .A (nx1260), .B (nx1257)) ;
    AO21A1AI2_X0P5M_A12TS ix1107 (.Y (nx1260), .A0 (ss_state_3_0), .A1 (nx1199)
                          , .B0 (nx1198), .C0 (nx1261)) ;
    NOR2_X0P5A_A12TS ix1108 (.Y (nx1261), .A (button_num_0), .B (button_num_1)
                     ) ;
    NOR2_X0P5A_A12TS ix1109 (.Y (nx1203), .A (reset), .B (ss_state_3_2)) ;
    NAND2_X0P5A_A12TS ix1110 (.Y (nx1202), .A (nx1262), .B (nx1257)) ;
    AO21A1AI2_X0P5M_A12TS ix1111 (.Y (nx1262), .A0 (ss_state_3_2), .A1 (nx1199)
                          , .B0 (nx1198), .C0 (nx1263)) ;
    NOR2_X0P5A_A12TS ix1112 (.Y (nx1201), .A (reset), .B (ss_state_3_3)) ;
    OAI21_X0P5M_A12TS ix1113 (.Y (nx1200), .A0 (nx1174), .A1 (nx1264), .B0 (
                      nx1257)) ;
    AOI21_X0P5M_A12TS ix1114 (.Y (nx1264), .A0 (ss_state_3_3), .A1 (nx1199), .B0 (
                      nx1198)) ;
    NOR2_X0P5A_A12TS ix1115 (.Y (nx1196), .A (reset), .B (ss_state_3_5)) ;
    NAND2_X0P5A_A12TS ix1116 (.Y (nx1195), .A (nx1265), .B (nx1257)) ;
    AO21A1AI2_X0P5M_A12TS ix1117 (.Y (nx1265), .A0 (ss_state_3_5), .A1 (nx1187)
                          , .B0 (nx1186), .C0 (nx1259)) ;
    NOR3_X0P5A_A12TS ix1118 (.Y (nx1187), .A (nx1220), .B (button_num_3), .C (
                     nx1266)) ;
    INV_X0P5B_A12TS ix1120 (.Y (nx1266), .A (button_num_2)) ;
    NOR3_X0P5A_A12TS ix1121 (.Y (nx1186), .A (nx1222), .B (button_num_3), .C (
                     nx1266)) ;
    NOR2_X0P5A_A12TS ix1122 (.Y (nx1194), .A (reset), .B (ss_state_3_4)) ;
    NAND2_X0P5A_A12TS ix1123 (.Y (nx1193), .A (nx1267), .B (nx1257)) ;
    AO21A1AI2_X0P5M_A12TS ix1124 (.Y (nx1267), .A0 (ss_state_3_4), .A1 (nx1187)
                          , .B0 (nx1186), .C0 (nx1261)) ;
    NOR2_X0P5A_A12TS ix1125 (.Y (nx1191), .A (reset), .B (ss_state_3_6)) ;
    NAND2_X0P5A_A12TS ix1126 (.Y (nx1190), .A (nx1268), .B (nx1257)) ;
    AO21A1AI2_X0P5M_A12TS ix1127 (.Y (nx1268), .A0 (ss_state_3_6), .A1 (nx1187)
                          , .B0 (nx1186), .C0 (nx1263)) ;
    NOR2_X0P5A_A12TS ix1128 (.Y (nx1189), .A (reset), .B (ss_state_3_7)) ;
    OAI21_X0P5M_A12TS ix1129 (.Y (nx1188), .A0 (nx1174), .A1 (nx1269), .B0 (
                      nx1257)) ;
    AOI21_X0P5M_A12TS ix1130 (.Y (nx1269), .A0 (ss_state_3_7), .A1 (nx1187), .B0 (
                      nx1186)) ;
    NOR2_X0P5A_A12TS ix1131 (.Y (nx1183), .A (reset), .B (ss_state_3_9)) ;
    NAND2_X0P5A_A12TS ix1132 (.Y (nx1182), .A (nx1270), .B (nx1257)) ;
    AO21A1AI2_X0P5M_A12TS ix1133 (.Y (nx1270), .A0 (ss_state_3_9), .A1 (nx1176)
                          , .B0 (nx1175), .C0 (nx1259)) ;
    NOR2_X0P5A_A12TS ix1134 (.Y (nx1181), .A (reset), .B (ss_state_3_8)) ;
    NAND2_X0P5A_A12TS ix1135 (.Y (nx1180), .A (nx1271), .B (nx1257)) ;
    AO21A1AI2_X0P5M_A12TS ix1136 (.Y (nx1271), .A0 (ss_state_3_8), .A1 (nx1176)
                          , .B0 (nx1175), .C0 (nx1261)) ;
    NOR2_X0P5A_A12TS ix1137 (.Y (nx1178), .A (reset), .B (ss_state_3_10)) ;
    NAND2_X0P5A_A12TS ix1138 (.Y (nx1177), .A (nx1272), .B (nx1257)) ;
    AO21A1AI2_X0P5M_A12TS ix1139 (.Y (nx1272), .A0 (ss_state_3_10), .A1 (nx1176)
                          , .B0 (nx1175), .C0 (nx1263)) ;
    TIELO_X1M_A12TS ix1140 (.Y (nx1173)) ;
    INV_X0P5B_A12TS ix1141 (.Y (nx1256), .A (nx1211)) ;
    INV_X0P5B_A12TS ix1142 (.Y (nx1247), .A (nx1210)) ;
    INV_X0P5B_A12TS ix1144 (.Y (nx1204), .A (nx1230)) ;
    INV_X0P5B_A12TS ix1145 (.Y (nx1192), .A (nx1235)) ;
    INV_X0P5B_A12TS ix1146 (.Y (nx1184), .A (nx1254)) ;
    INV_X0P5B_A12TS ix1147 (.Y (nx1179), .A (nx1239)) ;
    NOR2B_X0P7M_A12TS ix1148 (.Y (nx1259), .AN (button_num_0), .B (button_num_1)
                      ) ;
    NOR2B_X0P7M_A12TS ix1149 (.Y (nx1263), .AN (button_num_1), .B (button_num_0)
                      ) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_0__dup_28070 (.Q (ss_state_3_0), .CK (
                        osc_clk), .D (nx1275), .R (nx1173), .SN (nx1285)) ;
    MXT2_X0P5M_A12TS ix1150 (.Y (nx1275), .A (ss_state_3_0), .B (nx1206), .S0 (
                     nx1205)) ;
    INV_X0P5B_A12TS ix1151 (.Y (nx1285), .A (nx1173)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_1__dup_28073 (.Q (ss_state_3_1), .CK (
                        osc_clk), .D (nx1274), .R (nx1173), .SN (nx1285)) ;
    MXT2_X0P5M_A12TS ix1152 (.Y (nx1274), .A (ss_state_3_1), .B (nx1208), .S0 (
                     nx1207)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_2__dup_28075 (.Q (ss_state_3_2), .CK (
                        osc_clk), .D (nx1276), .R (nx1173), .SN (nx1285)) ;
    MXT2_X0P5M_A12TS ix1153 (.Y (nx1276), .A (ss_state_3_2), .B (nx1203), .S0 (
                     nx1202)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_3__dup_28077 (.Q (ss_state_3_3), .CK (
                        osc_clk), .D (nx1277), .R (nx1173), .SN (nx1285)) ;
    MXT2_X0P5M_A12TS ix1154 (.Y (nx1277), .A (ss_state_3_3), .B (nx1201), .S0 (
                     nx1200)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_4__dup_28079 (.Q (ss_state_3_4), .CK (
                        osc_clk), .D (nx1279), .R (nx1173), .SN (nx1285)) ;
    MXT2_X0P5M_A12TS ix1155 (.Y (nx1279), .A (ss_state_3_4), .B (nx1194), .S0 (
                     nx1193)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_5__dup_28081 (.Q (ss_state_3_5), .CK (
                        osc_clk), .D (nx1278), .R (nx1173), .SN (nx1285)) ;
    MXT2_X0P5M_A12TS ix1156 (.Y (nx1278), .A (ss_state_3_5), .B (nx1196), .S0 (
                     nx1195)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_6__dup_28083 (.Q (ss_state_3_6), .CK (
                        osc_clk), .D (nx1280), .R (nx1173), .SN (nx1285)) ;
    MXT2_X0P5M_A12TS ix1157 (.Y (nx1280), .A (ss_state_3_6), .B (nx1191), .S0 (
                     nx1190)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_7__dup_28085 (.Q (ss_state_3_7), .CK (
                        osc_clk), .D (nx1281), .R (nx1173), .SN (nx1285)) ;
    MXT2_X0P5M_A12TS ix1158 (.Y (nx1281), .A (ss_state_3_7), .B (nx1189), .S0 (
                     nx1188)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_8__dup_28087 (.Q (ss_state_3_8), .CK (
                        osc_clk), .D (nx1283), .R (nx1173), .SN (nx1285)) ;
    MXT2_X0P5M_A12TS ix1159 (.Y (nx1283), .A (ss_state_3_8), .B (nx1181), .S0 (
                     nx1180)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_9__dup_28089 (.Q (ss_state_3_9), .CK (
                        osc_clk), .D (nx1282), .R (nx1173), .SN (nx1285)) ;
    MXT2_X0P5M_A12TS ix1160 (.Y (nx1282), .A (ss_state_3_9), .B (nx1183), .S0 (
                     nx1182)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_10__dup_28091 (.Q (ss_state_3_10), .CK (
                        osc_clk), .D (nx1284), .R (nx1173), .SN (nx1285)) ;
    MXT2_X0P5M_A12TS ix1161 (.Y (nx1284), .A (ss_state_3_10), .B (nx1178), .S0 (
                     nx1177)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_11__dup_28093 (.Q (ss_state_3_11), .CK (
                        osc_clk), .D (nx1273), .R (nx1173), .SN (nx1285)) ;
    MXT2_X0P5M_A12TS ix1162 (.Y (nx1273), .A (ss_state_3_11), .B (nx1218), .S0 (
                     nx1217)) ;
    NOR2_X0P5A_A12TS ix1286 (.Y (nx1454), .A (reset), .B (ss_state_4_11)) ;
    OAI21_X0P5M_A12TS ix1287 (.Y (nx1453), .A0 (nx1410), .A1 (nx1455), .B0 (
                      nx1493)) ;
    NAND2_X0P5A_A12TS ix1288 (.Y (nx1410), .A (button_num_0), .B (button_num_1)
                      ) ;
    AOI21_X0P5M_A12TS ix1289 (.Y (nx1455), .A0 (ss_state_4_11), .A1 (nx1412), .B0 (
                      nx1411)) ;
    NOR3_X0P5A_A12TS ix1290 (.Y (nx1412), .A (nx1456), .B (nx1457), .C (
                     button_num_2)) ;
    INV_X0P5B_A12TS ix1291 (.Y (nx1456), .A (ss_selector_4)) ;
    INV_X0P5B_A12TS ix1292 (.Y (nx1457), .A (button_num_3)) ;
    NOR3_X0P5A_A12TS ix1293 (.Y (nx1411), .A (nx1458), .B (nx1457), .C (
                     button_num_2)) ;
    NAND2_X0P5A_A12TS ix1294 (.Y (nx1458), .A (ss_selector_4), .B (nx1452)) ;
    MXIT2_X0P5M_A12TS ix1295 (.Y (nx1452), .A (nx1459), .B (nx1460), .S0 (nx1491
                      )) ;
    INV_X0P5B_A12TS ix1296 (.Y (nx1459), .A (max_selections[3])) ;
    MXIT2_X0P5M_A12TS ix1297 (.Y (nx1460), .A (nx1451), .B (max_selections[2]), 
                      .S0 (nx1450)) ;
    MXIT2_X0P5M_A12TS ix1298 (.Y (nx1451), .A (nx1461), .B (nx1462), .S0 (nx1477
                      )) ;
    INV_X0P5B_A12TS ix1299 (.Y (nx1461), .A (max_selections[1])) ;
    NAND2_X0P5A_A12TS ix1300 (.Y (nx1462), .A (max_selections[0]), .B (nx1463)
                      ) ;
    XNOR2_X0P5M_A12TS ix1301 (.Y (nx1463), .A (nx1464), .B (nx1468)) ;
    XOR2_X0P5M_A12TS ix1302 (.Y (nx1464), .A (ss_state_4_1), .B (nx1465)) ;
    XOR2_X0P5M_A12TS ix1303 (.Y (nx1465), .A (ss_state_4_0), .B (nx1466)) ;
    OAI21_X0P5M_A12TS ix1304 (.Y (nx1466), .A0 (ss_state_4_2), .A1 (ss_state_4_3
                      ), .B0 (nx1467)) ;
    NAND2_X0P5A_A12TS ix1305 (.Y (nx1467), .A (ss_state_4_3), .B (ss_state_4_2)
                      ) ;
    XNOR2_X0P5M_A12TS ix1306 (.Y (nx1468), .A (nx1469), .B (nx1473)) ;
    XOR2_X0P5M_A12TS ix1307 (.Y (nx1469), .A (ss_state_4_5), .B (nx1470)) ;
    XOR2_X0P5M_A12TS ix1308 (.Y (nx1470), .A (ss_state_4_4), .B (nx1471)) ;
    OAI21_X0P5M_A12TS ix1309 (.Y (nx1471), .A0 (ss_state_4_6), .A1 (ss_state_4_7
                      ), .B0 (nx1472)) ;
    NAND2_X0P5A_A12TS ix1310 (.Y (nx1472), .A (ss_state_4_7), .B (ss_state_4_6)
                      ) ;
    XOR2_X0P5M_A12TS ix1311 (.Y (nx1473), .A (ss_state_4_9), .B (nx1474)) ;
    XOR2_X0P5M_A12TS ix1312 (.Y (nx1474), .A (ss_state_4_8), .B (nx1475)) ;
    OAI21_X0P5M_A12TS ix1313 (.Y (nx1475), .A0 (ss_state_4_10), .A1 (
                      ss_state_4_11), .B0 (nx1476)) ;
    NAND2_X0P5A_A12TS ix1314 (.Y (nx1476), .A (ss_state_4_11), .B (ss_state_4_10
                      )) ;
    XNOR3_X0P5M_A12TS ix1315 (.Y (nx1477), .A (max_selections[1]), .B (nx1446), 
                      .C (nx1445)) ;
    NOR2_X0P5A_A12TS ix1316 (.Y (nx1446), .A (nx1464), .B (nx1468)) ;
    XNOR3_X0P5M_A12TS ix1317 (.Y (nx1445), .A (nx1478), .B (nx1467), .C (nx1479)
                      ) ;
    CGENI_X1M_A12TS ix1318 (.CON (nx1478), .A (ss_state_4_1), .B (ss_state_4_0)
                    , .CI (nx1440)) ;
    XOR2_X0P5M_A12TS ix1319 (.Y (nx1479), .A (nx1433), .B (nx1480)) ;
    NOR2_X0P5A_A12TS ix1320 (.Y (nx1433), .A (nx1469), .B (nx1473)) ;
    XNOR3_X0P5M_A12TS ix1321 (.Y (nx1480), .A (nx1481), .B (nx1472), .C (nx1421)
                      ) ;
    CGENI_X1M_A12TS ix1322 (.CON (nx1481), .A (ss_state_4_5), .B (ss_state_4_4)
                    , .CI (nx1428)) ;
    AOI21_X0P5M_A12TS ix1323 (.Y (nx1421), .A0 (nx1476), .A1 (nx1482), .B0 (
                      nx1420)) ;
    CGENI_X1M_A12TS ix1324 (.CON (nx1482), .A (ss_state_4_9), .B (ss_state_4_8)
                    , .CI (nx1415)) ;
    XNOR3_X0P5M_A12TS ix1325 (.Y (nx1450), .A (max_selections[2]), .B (nx1447), 
                      .C (nx1484)) ;
    MXIT2_X0P5M_A12TS ix1326 (.Y (nx1447), .A (nx1479), .B (nx1483), .S0 (nx1445
                      )) ;
    XNOR2_X0P5M_A12TS ix1327 (.Y (nx1484), .A (nx1485), .B (nx1486)) ;
    NAND4_X0P5A_A12TS ix1328 (.Y (nx1485), .A (ss_state_4_1), .B (ss_state_4_0)
                      , .C (ss_state_4_3), .D (ss_state_4_2)) ;
    XNOR2_X0P5M_A12TS ix1329 (.Y (nx1486), .A (nx1487), .B (nx1488)) ;
    MXIT2_X0P5M_A12TS ix1330 (.Y (nx1487), .A (nx1433), .B (nx1421), .S0 (nx1480
                      )) ;
    XNOR2_X0P5M_A12TS ix1331 (.Y (nx1488), .A (nx1489), .B (nx1490)) ;
    NAND4_X0P5A_A12TS ix1332 (.Y (nx1489), .A (ss_state_4_5), .B (ss_state_4_4)
                      , .C (ss_state_4_7), .D (ss_state_4_6)) ;
    NAND4_X0P5A_A12TS ix1333 (.Y (nx1490), .A (ss_state_4_9), .B (ss_state_4_8)
                      , .C (ss_state_4_11), .D (ss_state_4_10)) ;
    XNOR3_X0P5M_A12TS ix1334 (.Y (nx1491), .A (max_selections[3]), .B (nx1448), 
                      .C (nx1449)) ;
    CGENI_X1M_A12TS ix1335 (.CON (nx1448), .A (nx1492), .B (nx1485), .CI (nx1486
                    )) ;
    CGENI_X1M_A12TS ix1336 (.CON (nx1449), .A (nx1487), .B (nx1489), .CI (nx1490
                    )) ;
    INV_X0P5B_A12TS ix1337 (.Y (nx1493), .A (reset)) ;
    NOR2_X0P5A_A12TS ix1338 (.Y (nx1444), .A (reset), .B (ss_state_4_1)) ;
    NAND2_X0P5A_A12TS ix1339 (.Y (nx1443), .A (nx1494), .B (nx1493)) ;
    AO21A1AI2_X0P5M_A12TS ix1340 (.Y (nx1494), .A0 (ss_state_4_1), .A1 (nx1435)
                          , .B0 (nx1434), .C0 (nx1495)) ;
    NOR3_X0P5A_A12TS ix1341 (.Y (nx1435), .A (nx1456), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR3_X0P5A_A12TS ix1342 (.Y (nx1434), .A (nx1458), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR2_X0P5A_A12TS ix1343 (.Y (nx1442), .A (reset), .B (ss_state_4_0)) ;
    NAND2_X0P5A_A12TS ix1344 (.Y (nx1441), .A (nx1496), .B (nx1493)) ;
    AO21A1AI2_X0P5M_A12TS ix1345 (.Y (nx1496), .A0 (ss_state_4_0), .A1 (nx1435)
                          , .B0 (nx1434), .C0 (nx1497)) ;
    NOR2_X0P5A_A12TS ix1346 (.Y (nx1497), .A (button_num_0), .B (button_num_1)
                     ) ;
    NOR2_X0P5A_A12TS ix1347 (.Y (nx1439), .A (reset), .B (ss_state_4_2)) ;
    NAND2_X0P5A_A12TS ix1348 (.Y (nx1438), .A (nx1498), .B (nx1493)) ;
    AO21A1AI2_X0P5M_A12TS ix1349 (.Y (nx1498), .A0 (ss_state_4_2), .A1 (nx1435)
                          , .B0 (nx1434), .C0 (nx1499)) ;
    NOR2_X0P5A_A12TS ix1350 (.Y (nx1437), .A (reset), .B (ss_state_4_3)) ;
    OAI21_X0P5M_A12TS ix1351 (.Y (nx1436), .A0 (nx1410), .A1 (nx1500), .B0 (
                      nx1493)) ;
    AOI21_X0P5M_A12TS ix1352 (.Y (nx1500), .A0 (ss_state_4_3), .A1 (nx1435), .B0 (
                      nx1434)) ;
    NOR2_X0P5A_A12TS ix1353 (.Y (nx1432), .A (reset), .B (ss_state_4_5)) ;
    NAND2_X0P5A_A12TS ix1354 (.Y (nx1431), .A (nx1501), .B (nx1493)) ;
    AO21A1AI2_X0P5M_A12TS ix1355 (.Y (nx1501), .A0 (ss_state_4_5), .A1 (nx1423)
                          , .B0 (nx1422), .C0 (nx1495)) ;
    NOR3_X0P5A_A12TS ix1356 (.Y (nx1423), .A (nx1456), .B (button_num_3), .C (
                     nx1502)) ;
    INV_X0P5B_A12TS ix1357 (.Y (nx1502), .A (button_num_2)) ;
    NOR3_X0P5A_A12TS ix1358 (.Y (nx1422), .A (nx1458), .B (button_num_3), .C (
                     nx1502)) ;
    NOR2_X0P5A_A12TS ix1359 (.Y (nx1430), .A (reset), .B (ss_state_4_4)) ;
    NAND2_X0P5A_A12TS ix1360 (.Y (nx1429), .A (nx1503), .B (nx1493)) ;
    AO21A1AI2_X0P5M_A12TS ix1361 (.Y (nx1503), .A0 (ss_state_4_4), .A1 (nx1423)
                          , .B0 (nx1422), .C0 (nx1497)) ;
    NOR2_X0P5A_A12TS ix1362 (.Y (nx1427), .A (reset), .B (ss_state_4_6)) ;
    NAND2_X0P5A_A12TS ix1363 (.Y (nx1426), .A (nx1504), .B (nx1493)) ;
    AO21A1AI2_X0P5M_A12TS ix1364 (.Y (nx1504), .A0 (ss_state_4_6), .A1 (nx1423)
                          , .B0 (nx1422), .C0 (nx1499)) ;
    NOR2_X0P5A_A12TS ix1365 (.Y (nx1425), .A (reset), .B (ss_state_4_7)) ;
    OAI21_X0P5M_A12TS ix1366 (.Y (nx1424), .A0 (nx1410), .A1 (nx1505), .B0 (
                      nx1493)) ;
    AOI21_X0P5M_A12TS ix1367 (.Y (nx1505), .A0 (ss_state_4_7), .A1 (nx1423), .B0 (
                      nx1422)) ;
    NOR2_X0P5A_A12TS ix1368 (.Y (nx1419), .A (reset), .B (ss_state_4_9)) ;
    NAND2_X0P5A_A12TS ix1369 (.Y (nx1418), .A (nx1506), .B (nx1493)) ;
    AO21A1AI2_X0P5M_A12TS ix1370 (.Y (nx1506), .A0 (ss_state_4_9), .A1 (nx1412)
                          , .B0 (nx1411), .C0 (nx1495)) ;
    NOR2_X0P5A_A12TS ix1371 (.Y (nx1417), .A (reset), .B (ss_state_4_8)) ;
    NAND2_X0P5A_A12TS ix1372 (.Y (nx1416), .A (nx1507), .B (nx1493)) ;
    AO21A1AI2_X0P5M_A12TS ix1373 (.Y (nx1507), .A0 (ss_state_4_8), .A1 (nx1412)
                          , .B0 (nx1411), .C0 (nx1497)) ;
    NOR2_X0P5A_A12TS ix1374 (.Y (nx1414), .A (reset), .B (ss_state_4_10)) ;
    NAND2_X0P5A_A12TS ix1375 (.Y (nx1413), .A (nx1508), .B (nx1493)) ;
    AO21A1AI2_X0P5M_A12TS ix1376 (.Y (nx1508), .A0 (ss_state_4_10), .A1 (nx1412)
                          , .B0 (nx1411), .C0 (nx1499)) ;
    TIELO_X1M_A12TS ix1377 (.Y (nx1409)) ;
    INV_X0P5B_A12TS ix1378 (.Y (nx1492), .A (nx1447)) ;
    INV_X0P5B_A12TS ix1379 (.Y (nx1483), .A (nx1446)) ;
    INV_X0P5B_A12TS ix1380 (.Y (nx1440), .A (nx1466)) ;
    INV_X0P5B_A12TS ix1381 (.Y (nx1428), .A (nx1471)) ;
    INV_X0P5B_A12TS ix1382 (.Y (nx1420), .A (nx1490)) ;
    INV_X0P5B_A12TS ix1383 (.Y (nx1415), .A (nx1475)) ;
    NOR2B_X0P7M_A12TS ix1384 (.Y (nx1495), .AN (button_num_0), .B (button_num_1)
                      ) ;
    NOR2B_X0P7M_A12TS ix1385 (.Y (nx1499), .AN (button_num_1), .B (button_num_0)
                      ) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_0__dup_28195 (.Q (ss_state_4_0), .CK (
                        osc_clk), .D (nx1511), .R (nx1409), .SN (nx1521)) ;
    MXT2_X0P5M_A12TS ix1386 (.Y (nx1511), .A (ss_state_4_0), .B (nx1442), .S0 (
                     nx1441)) ;
    INV_X0P5B_A12TS ix1387 (.Y (nx1521), .A (nx1409)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_1__dup_28198 (.Q (ss_state_4_1), .CK (
                        osc_clk), .D (nx1510), .R (nx1409), .SN (nx1521)) ;
    MXT2_X0P5M_A12TS ix1388 (.Y (nx1510), .A (ss_state_4_1), .B (nx1444), .S0 (
                     nx1443)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_2__dup_28200 (.Q (ss_state_4_2), .CK (
                        osc_clk), .D (nx1512), .R (nx1409), .SN (nx1521)) ;
    MXT2_X0P5M_A12TS ix1389 (.Y (nx1512), .A (ss_state_4_2), .B (nx1439), .S0 (
                     nx1438)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_3__dup_28202 (.Q (ss_state_4_3), .CK (
                        osc_clk), .D (nx1513), .R (nx1409), .SN (nx1521)) ;
    MXT2_X0P5M_A12TS ix1390 (.Y (nx1513), .A (ss_state_4_3), .B (nx1437), .S0 (
                     nx1436)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_4__dup_28204 (.Q (ss_state_4_4), .CK (
                        osc_clk), .D (nx1515), .R (nx1409), .SN (nx1521)) ;
    MXT2_X0P5M_A12TS ix1391 (.Y (nx1515), .A (ss_state_4_4), .B (nx1430), .S0 (
                     nx1429)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_5__dup_28206 (.Q (ss_state_4_5), .CK (
                        osc_clk), .D (nx1514), .R (nx1409), .SN (nx1521)) ;
    MXT2_X0P5M_A12TS ix1392 (.Y (nx1514), .A (ss_state_4_5), .B (nx1432), .S0 (
                     nx1431)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_6__dup_28208 (.Q (ss_state_4_6), .CK (
                        osc_clk), .D (nx1516), .R (nx1409), .SN (nx1521)) ;
    MXT2_X0P5M_A12TS ix1393 (.Y (nx1516), .A (ss_state_4_6), .B (nx1427), .S0 (
                     nx1426)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_7__dup_28210 (.Q (ss_state_4_7), .CK (
                        osc_clk), .D (nx1517), .R (nx1409), .SN (nx1521)) ;
    MXT2_X0P5M_A12TS ix1394 (.Y (nx1517), .A (ss_state_4_7), .B (nx1425), .S0 (
                     nx1424)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_8__dup_28212 (.Q (ss_state_4_8), .CK (
                        osc_clk), .D (nx1519), .R (nx1409), .SN (nx1521)) ;
    MXT2_X0P5M_A12TS ix1395 (.Y (nx1519), .A (ss_state_4_8), .B (nx1417), .S0 (
                     nx1416)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_9__dup_28214 (.Q (ss_state_4_9), .CK (
                        osc_clk), .D (nx1518), .R (nx1409), .SN (nx1521)) ;
    MXT2_X0P5M_A12TS ix1396 (.Y (nx1518), .A (ss_state_4_9), .B (nx1419), .S0 (
                     nx1418)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_10__dup_28216 (.Q (ss_state_4_10), .CK (
                        osc_clk), .D (nx1520), .R (nx1409), .SN (nx1521)) ;
    MXT2_X0P5M_A12TS ix1397 (.Y (nx1520), .A (ss_state_4_10), .B (nx1414), .S0 (
                     nx1413)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_11__dup_28218 (.Q (ss_state_4_11), .CK (
                        osc_clk), .D (nx1509), .R (nx1409), .SN (nx1521)) ;
    MXT2_X0P5M_A12TS ix1398 (.Y (nx1509), .A (ss_state_4_11), .B (nx1454), .S0 (
                     nx1453)) ;
    NOR2_X0P5A_A12TS ix1522 (.Y (nx1690), .A (reset), .B (ss_state_5_11)) ;
    OAI21_X0P5M_A12TS ix1523 (.Y (nx1689), .A0 (nx1646), .A1 (nx1691), .B0 (
                      nx1729)) ;
    NAND2_X0P5A_A12TS ix1524 (.Y (nx1646), .A (button_num_0), .B (button_num_1)
                      ) ;
    AOI21_X0P5M_A12TS ix1525 (.Y (nx1691), .A0 (ss_state_5_11), .A1 (nx1648), .B0 (
                      nx1647)) ;
    NOR3_X0P5A_A12TS ix1526 (.Y (nx1648), .A (nx1692), .B (nx1693), .C (
                     button_num_2)) ;
    INV_X0P5B_A12TS ix1527 (.Y (nx1692), .A (ss_selector_5)) ;
    INV_X0P5B_A12TS ix1528 (.Y (nx1693), .A (button_num_3)) ;
    NOR3_X0P5A_A12TS ix1529 (.Y (nx1647), .A (nx1694), .B (nx1693), .C (
                     button_num_2)) ;
    NAND2_X0P5A_A12TS ix1530 (.Y (nx1694), .A (ss_selector_5), .B (nx1688)) ;
    MXIT2_X0P5M_A12TS ix1531 (.Y (nx1688), .A (nx1695), .B (nx1696), .S0 (nx1727
                      )) ;
    INV_X0P5B_A12TS ix1532 (.Y (nx1695), .A (max_selections[3])) ;
    MXIT2_X0P5M_A12TS ix1533 (.Y (nx1696), .A (nx1687), .B (max_selections[2]), 
                      .S0 (nx1686)) ;
    MXIT2_X0P5M_A12TS ix1534 (.Y (nx1687), .A (nx1697), .B (nx1698), .S0 (nx1713
                      )) ;
    INV_X0P5B_A12TS ix1535 (.Y (nx1697), .A (max_selections[1])) ;
    NAND2_X0P5A_A12TS ix1536 (.Y (nx1698), .A (max_selections[0]), .B (nx1699)
                      ) ;
    XNOR2_X0P5M_A12TS ix1537 (.Y (nx1699), .A (nx1700), .B (nx1704)) ;
    XOR2_X0P5M_A12TS ix1538 (.Y (nx1700), .A (ss_state_5_1), .B (nx1701)) ;
    XOR2_X0P5M_A12TS ix1539 (.Y (nx1701), .A (ss_state_5_0), .B (nx1702)) ;
    OAI21_X0P5M_A12TS ix1540 (.Y (nx1702), .A0 (ss_state_5_2), .A1 (ss_state_5_3
                      ), .B0 (nx1703)) ;
    NAND2_X0P5A_A12TS ix1541 (.Y (nx1703), .A (ss_state_5_3), .B (ss_state_5_2)
                      ) ;
    XNOR2_X0P5M_A12TS ix1542 (.Y (nx1704), .A (nx1705), .B (nx1709)) ;
    XOR2_X0P5M_A12TS ix1543 (.Y (nx1705), .A (ss_state_5_5), .B (nx1706)) ;
    XOR2_X0P5M_A12TS ix1544 (.Y (nx1706), .A (ss_state_5_4), .B (nx1707)) ;
    OAI21_X0P5M_A12TS ix1545 (.Y (nx1707), .A0 (ss_state_5_6), .A1 (ss_state_5_7
                      ), .B0 (nx1708)) ;
    NAND2_X0P5A_A12TS ix1546 (.Y (nx1708), .A (ss_state_5_7), .B (ss_state_5_6)
                      ) ;
    XOR2_X0P5M_A12TS ix1547 (.Y (nx1709), .A (ss_state_5_9), .B (nx1710)) ;
    XOR2_X0P5M_A12TS ix1548 (.Y (nx1710), .A (ss_state_5_8), .B (nx1711)) ;
    OAI21_X0P5M_A12TS ix1549 (.Y (nx1711), .A0 (ss_state_5_10), .A1 (
                      ss_state_5_11), .B0 (nx1712)) ;
    NAND2_X0P5A_A12TS ix1550 (.Y (nx1712), .A (ss_state_5_11), .B (ss_state_5_10
                      )) ;
    XNOR3_X0P5M_A12TS ix1551 (.Y (nx1713), .A (max_selections[1]), .B (nx1682), 
                      .C (nx1681)) ;
    NOR2_X0P5A_A12TS ix1552 (.Y (nx1682), .A (nx1700), .B (nx1704)) ;
    XNOR3_X0P5M_A12TS ix1553 (.Y (nx1681), .A (nx1714), .B (nx1703), .C (nx1715)
                      ) ;
    CGENI_X1M_A12TS ix1554 (.CON (nx1714), .A (ss_state_5_1), .B (ss_state_5_0)
                    , .CI (nx1676)) ;
    XOR2_X0P5M_A12TS ix1555 (.Y (nx1715), .A (nx1669), .B (nx1716)) ;
    NOR2_X0P5A_A12TS ix1556 (.Y (nx1669), .A (nx1705), .B (nx1709)) ;
    XNOR3_X0P5M_A12TS ix1557 (.Y (nx1716), .A (nx1717), .B (nx1708), .C (nx1657)
                      ) ;
    CGENI_X1M_A12TS ix1558 (.CON (nx1717), .A (ss_state_5_5), .B (ss_state_5_4)
                    , .CI (nx1664)) ;
    AOI21_X0P5M_A12TS ix1559 (.Y (nx1657), .A0 (nx1712), .A1 (nx1718), .B0 (
                      nx1656)) ;
    CGENI_X1M_A12TS ix1560 (.CON (nx1718), .A (ss_state_5_9), .B (ss_state_5_8)
                    , .CI (nx1651)) ;
    XNOR3_X0P5M_A12TS ix1561 (.Y (nx1686), .A (max_selections[2]), .B (nx1683), 
                      .C (nx1720)) ;
    MXIT2_X0P5M_A12TS ix1562 (.Y (nx1683), .A (nx1715), .B (nx1719), .S0 (nx1681
                      )) ;
    XNOR2_X0P5M_A12TS ix1563 (.Y (nx1720), .A (nx1721), .B (nx1722)) ;
    NAND4_X0P5A_A12TS ix1564 (.Y (nx1721), .A (ss_state_5_1), .B (ss_state_5_0)
                      , .C (ss_state_5_3), .D (ss_state_5_2)) ;
    XNOR2_X0P5M_A12TS ix1565 (.Y (nx1722), .A (nx1723), .B (nx1724)) ;
    MXIT2_X0P5M_A12TS ix1566 (.Y (nx1723), .A (nx1669), .B (nx1657), .S0 (nx1716
                      )) ;
    XNOR2_X0P5M_A12TS ix1567 (.Y (nx1724), .A (nx1725), .B (nx1726)) ;
    NAND4_X0P5A_A12TS ix1568 (.Y (nx1725), .A (ss_state_5_5), .B (ss_state_5_4)
                      , .C (ss_state_5_7), .D (ss_state_5_6)) ;
    NAND4_X0P5A_A12TS ix1569 (.Y (nx1726), .A (ss_state_5_9), .B (ss_state_5_8)
                      , .C (ss_state_5_11), .D (ss_state_5_10)) ;
    XNOR3_X0P5M_A12TS ix1570 (.Y (nx1727), .A (max_selections[3]), .B (nx1684), 
                      .C (nx1685)) ;
    CGENI_X1M_A12TS ix1571 (.CON (nx1684), .A (nx1728), .B (nx1721), .CI (nx1722
                    )) ;
    CGENI_X1M_A12TS ix1572 (.CON (nx1685), .A (nx1723), .B (nx1725), .CI (nx1726
                    )) ;
    INV_X0P5B_A12TS ix1573 (.Y (nx1729), .A (reset)) ;
    NOR2_X0P5A_A12TS ix1574 (.Y (nx1680), .A (reset), .B (ss_state_5_1)) ;
    NAND2_X0P5A_A12TS ix1575 (.Y (nx1679), .A (nx1730), .B (nx1729)) ;
    AO21A1AI2_X0P5M_A12TS ix1576 (.Y (nx1730), .A0 (ss_state_5_1), .A1 (nx1671)
                          , .B0 (nx1670), .C0 (nx1731)) ;
    NOR3_X0P5A_A12TS ix1577 (.Y (nx1671), .A (nx1692), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR3_X0P5A_A12TS ix1578 (.Y (nx1670), .A (nx1694), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR2_X0P5A_A12TS ix1579 (.Y (nx1678), .A (reset), .B (ss_state_5_0)) ;
    NAND2_X0P5A_A12TS ix1580 (.Y (nx1677), .A (nx1732), .B (nx1729)) ;
    AO21A1AI2_X0P5M_A12TS ix1581 (.Y (nx1732), .A0 (ss_state_5_0), .A1 (nx1671)
                          , .B0 (nx1670), .C0 (nx1733)) ;
    NOR2_X0P5A_A12TS ix1582 (.Y (nx1733), .A (button_num_0), .B (button_num_1)
                     ) ;
    NOR2_X0P5A_A12TS ix1583 (.Y (nx1675), .A (reset), .B (ss_state_5_2)) ;
    NAND2_X0P5A_A12TS ix1584 (.Y (nx1674), .A (nx1734), .B (nx1729)) ;
    AO21A1AI2_X0P5M_A12TS ix1585 (.Y (nx1734), .A0 (ss_state_5_2), .A1 (nx1671)
                          , .B0 (nx1670), .C0 (nx1735)) ;
    NOR2_X0P5A_A12TS ix1586 (.Y (nx1673), .A (reset), .B (ss_state_5_3)) ;
    OAI21_X0P5M_A12TS ix1587 (.Y (nx1672), .A0 (nx1646), .A1 (nx1736), .B0 (
                      nx1729)) ;
    AOI21_X0P5M_A12TS ix1588 (.Y (nx1736), .A0 (ss_state_5_3), .A1 (nx1671), .B0 (
                      nx1670)) ;
    NOR2_X0P5A_A12TS ix1589 (.Y (nx1668), .A (reset), .B (ss_state_5_5)) ;
    NAND2_X0P5A_A12TS ix1590 (.Y (nx1667), .A (nx1737), .B (nx1729)) ;
    AO21A1AI2_X0P5M_A12TS ix1591 (.Y (nx1737), .A0 (ss_state_5_5), .A1 (nx1659)
                          , .B0 (nx1658), .C0 (nx1731)) ;
    NOR3_X0P5A_A12TS ix1592 (.Y (nx1659), .A (nx1692), .B (button_num_3), .C (
                     nx1738)) ;
    INV_X0P5B_A12TS ix1593 (.Y (nx1738), .A (button_num_2)) ;
    NOR3_X0P5A_A12TS ix1594 (.Y (nx1658), .A (nx1694), .B (button_num_3), .C (
                     nx1738)) ;
    NOR2_X0P5A_A12TS ix1595 (.Y (nx1666), .A (reset), .B (ss_state_5_4)) ;
    NAND2_X0P5A_A12TS ix1596 (.Y (nx1665), .A (nx1739), .B (nx1729)) ;
    AO21A1AI2_X0P5M_A12TS ix1597 (.Y (nx1739), .A0 (ss_state_5_4), .A1 (nx1659)
                          , .B0 (nx1658), .C0 (nx1733)) ;
    NOR2_X0P5A_A12TS ix1598 (.Y (nx1663), .A (reset), .B (ss_state_5_6)) ;
    NAND2_X0P5A_A12TS ix1599 (.Y (nx1662), .A (nx1740), .B (nx1729)) ;
    AO21A1AI2_X0P5M_A12TS ix1600 (.Y (nx1740), .A0 (ss_state_5_6), .A1 (nx1659)
                          , .B0 (nx1658), .C0 (nx1735)) ;
    NOR2_X0P5A_A12TS ix1601 (.Y (nx1661), .A (reset), .B (ss_state_5_7)) ;
    OAI21_X0P5M_A12TS ix1602 (.Y (nx1660), .A0 (nx1646), .A1 (nx1741), .B0 (
                      nx1729)) ;
    AOI21_X0P5M_A12TS ix1603 (.Y (nx1741), .A0 (ss_state_5_7), .A1 (nx1659), .B0 (
                      nx1658)) ;
    NOR2_X0P5A_A12TS ix1604 (.Y (nx1655), .A (reset), .B (ss_state_5_9)) ;
    NAND2_X0P5A_A12TS ix1605 (.Y (nx1654), .A (nx1742), .B (nx1729)) ;
    AO21A1AI2_X0P5M_A12TS ix1606 (.Y (nx1742), .A0 (ss_state_5_9), .A1 (nx1648)
                          , .B0 (nx1647), .C0 (nx1731)) ;
    NOR2_X0P5A_A12TS ix1607 (.Y (nx1653), .A (reset), .B (ss_state_5_8)) ;
    NAND2_X0P5A_A12TS ix1608 (.Y (nx1652), .A (nx1743), .B (nx1729)) ;
    AO21A1AI2_X0P5M_A12TS ix1609 (.Y (nx1743), .A0 (ss_state_5_8), .A1 (nx1648)
                          , .B0 (nx1647), .C0 (nx1733)) ;
    NOR2_X0P5A_A12TS ix1610 (.Y (nx1650), .A (reset), .B (ss_state_5_10)) ;
    NAND2_X0P5A_A12TS ix1611 (.Y (nx1649), .A (nx1744), .B (nx1729)) ;
    AO21A1AI2_X0P5M_A12TS ix1612 (.Y (nx1744), .A0 (ss_state_5_10), .A1 (nx1648)
                          , .B0 (nx1647), .C0 (nx1735)) ;
    TIELO_X1M_A12TS ix1613 (.Y (nx1645)) ;
    INV_X0P5B_A12TS ix1614 (.Y (nx1728), .A (nx1683)) ;
    INV_X0P5B_A12TS ix1615 (.Y (nx1719), .A (nx1682)) ;
    INV_X0P5B_A12TS ix1616 (.Y (nx1676), .A (nx1702)) ;
    INV_X0P5B_A12TS ix1617 (.Y (nx1664), .A (nx1707)) ;
    INV_X0P5B_A12TS ix1618 (.Y (nx1656), .A (nx1726)) ;
    INV_X0P5B_A12TS ix1619 (.Y (nx1651), .A (nx1711)) ;
    NOR2B_X0P7M_A12TS ix1620 (.Y (nx1731), .AN (button_num_0), .B (button_num_1)
                      ) ;
    NOR2B_X0P7M_A12TS ix1621 (.Y (nx1735), .AN (button_num_1), .B (button_num_0)
                      ) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_0__dup_28320 (.Q (ss_state_5_0), .CK (
                        osc_clk), .D (nx1747), .R (nx1645), .SN (nx1757)) ;
    MXT2_X0P5M_A12TS ix1622 (.Y (nx1747), .A (ss_state_5_0), .B (nx1678), .S0 (
                     nx1677)) ;
    INV_X0P5B_A12TS ix1623 (.Y (nx1757), .A (nx1645)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_1__dup_28323 (.Q (ss_state_5_1), .CK (
                        osc_clk), .D (nx1746), .R (nx1645), .SN (nx1757)) ;
    MXT2_X0P5M_A12TS ix1624 (.Y (nx1746), .A (ss_state_5_1), .B (nx1680), .S0 (
                     nx1679)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_2__dup_28325 (.Q (ss_state_5_2), .CK (
                        osc_clk), .D (nx1748), .R (nx1645), .SN (nx1757)) ;
    MXT2_X0P5M_A12TS ix1625 (.Y (nx1748), .A (ss_state_5_2), .B (nx1675), .S0 (
                     nx1674)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_3__dup_28327 (.Q (ss_state_5_3), .CK (
                        osc_clk), .D (nx1749), .R (nx1645), .SN (nx1757)) ;
    MXT2_X0P5M_A12TS ix1626 (.Y (nx1749), .A (ss_state_5_3), .B (nx1673), .S0 (
                     nx1672)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_4__dup_28329 (.Q (ss_state_5_4), .CK (
                        osc_clk), .D (nx1751), .R (nx1645), .SN (nx1757)) ;
    MXT2_X0P5M_A12TS ix1627 (.Y (nx1751), .A (ss_state_5_4), .B (nx1666), .S0 (
                     nx1665)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_5__dup_28331 (.Q (ss_state_5_5), .CK (
                        osc_clk), .D (nx1750), .R (nx1645), .SN (nx1757)) ;
    MXT2_X0P5M_A12TS ix1628 (.Y (nx1750), .A (ss_state_5_5), .B (nx1668), .S0 (
                     nx1667)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_6__dup_28333 (.Q (ss_state_5_6), .CK (
                        osc_clk), .D (nx1752), .R (nx1645), .SN (nx1757)) ;
    MXT2_X0P5M_A12TS ix1629 (.Y (nx1752), .A (ss_state_5_6), .B (nx1663), .S0 (
                     nx1662)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_7__dup_28335 (.Q (ss_state_5_7), .CK (
                        osc_clk), .D (nx1753), .R (nx1645), .SN (nx1757)) ;
    MXT2_X0P5M_A12TS ix1630 (.Y (nx1753), .A (ss_state_5_7), .B (nx1661), .S0 (
                     nx1660)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_8__dup_28337 (.Q (ss_state_5_8), .CK (
                        osc_clk), .D (nx1755), .R (nx1645), .SN (nx1757)) ;
    MXT2_X0P5M_A12TS ix1631 (.Y (nx1755), .A (ss_state_5_8), .B (nx1653), .S0 (
                     nx1652)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_9__dup_28339 (.Q (ss_state_5_9), .CK (
                        osc_clk), .D (nx1754), .R (nx1645), .SN (nx1757)) ;
    MXT2_X0P5M_A12TS ix1632 (.Y (nx1754), .A (ss_state_5_9), .B (nx1655), .S0 (
                     nx1654)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_10__dup_28341 (.Q (ss_state_5_10), .CK (
                        osc_clk), .D (nx1756), .R (nx1645), .SN (nx1757)) ;
    MXT2_X0P5M_A12TS ix1633 (.Y (nx1756), .A (ss_state_5_10), .B (nx1650), .S0 (
                     nx1649)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_11__dup_28343 (.Q (ss_state_5_11), .CK (
                        osc_clk), .D (nx1745), .R (nx1645), .SN (nx1757)) ;
    MXT2_X0P5M_A12TS ix1634 (.Y (nx1745), .A (ss_state_5_11), .B (nx1690), .S0 (
                     nx1689)) ;
    NOR2_X0P5A_A12TS ix1758 (.Y (nx1926), .A (reset), .B (ss_state_6_11)) ;
    OAI21_X0P5M_A12TS ix1759 (.Y (nx1925), .A0 (nx1882), .A1 (nx1927), .B0 (
                      nx1965)) ;
    NAND2_X0P5A_A12TS ix1760 (.Y (nx1882), .A (button_num_0), .B (button_num_1)
                      ) ;
    AOI21_X0P5M_A12TS ix1761 (.Y (nx1927), .A0 (ss_state_6_11), .A1 (nx1884), .B0 (
                      nx1883)) ;
    NOR3_X0P5A_A12TS ix1762 (.Y (nx1884), .A (nx1928), .B (nx1929), .C (
                     button_num_2)) ;
    INV_X0P5B_A12TS ix1763 (.Y (nx1928), .A (ss_selector_6)) ;
    INV_X0P5B_A12TS ix1764 (.Y (nx1929), .A (button_num_3)) ;
    NOR3_X0P5A_A12TS ix1765 (.Y (nx1883), .A (nx1930), .B (nx1929), .C (
                     button_num_2)) ;
    NAND2_X0P5A_A12TS ix1766 (.Y (nx1930), .A (ss_selector_6), .B (nx1924)) ;
    MXIT2_X0P5M_A12TS ix1767 (.Y (nx1924), .A (nx1931), .B (nx1932), .S0 (nx1963
                      )) ;
    INV_X0P5B_A12TS ix1768 (.Y (nx1931), .A (max_selections[3])) ;
    MXIT2_X0P5M_A12TS ix1769 (.Y (nx1932), .A (nx1923), .B (max_selections[2]), 
                      .S0 (nx1922)) ;
    MXIT2_X0P5M_A12TS ix1770 (.Y (nx1923), .A (nx1933), .B (nx1934), .S0 (nx1949
                      )) ;
    INV_X0P5B_A12TS ix1771 (.Y (nx1933), .A (max_selections[1])) ;
    NAND2_X0P5A_A12TS ix1772 (.Y (nx1934), .A (max_selections[0]), .B (nx1935)
                      ) ;
    XNOR2_X0P5M_A12TS ix1773 (.Y (nx1935), .A (nx1936), .B (nx1940)) ;
    XOR2_X0P5M_A12TS ix1774 (.Y (nx1936), .A (ss_state_6_1), .B (nx1937)) ;
    XOR2_X0P5M_A12TS ix1775 (.Y (nx1937), .A (ss_state_6_0), .B (nx1938)) ;
    OAI21_X0P5M_A12TS ix1776 (.Y (nx1938), .A0 (ss_state_6_2), .A1 (ss_state_6_3
                      ), .B0 (nx1939)) ;
    NAND2_X0P5A_A12TS ix1777 (.Y (nx1939), .A (ss_state_6_3), .B (ss_state_6_2)
                      ) ;
    XNOR2_X0P5M_A12TS ix1778 (.Y (nx1940), .A (nx1941), .B (nx1945)) ;
    XOR2_X0P5M_A12TS ix1779 (.Y (nx1941), .A (ss_state_6_5), .B (nx1942)) ;
    XOR2_X0P5M_A12TS ix1780 (.Y (nx1942), .A (ss_state_6_4), .B (nx1943)) ;
    OAI21_X0P5M_A12TS ix1781 (.Y (nx1943), .A0 (ss_state_6_6), .A1 (ss_state_6_7
                      ), .B0 (nx1944)) ;
    NAND2_X0P5A_A12TS ix1782 (.Y (nx1944), .A (ss_state_6_7), .B (ss_state_6_6)
                      ) ;
    XOR2_X0P5M_A12TS ix1783 (.Y (nx1945), .A (ss_state_6_9), .B (nx1946)) ;
    XOR2_X0P5M_A12TS ix1784 (.Y (nx1946), .A (ss_state_6_8), .B (nx1947)) ;
    OAI21_X0P5M_A12TS ix1785 (.Y (nx1947), .A0 (ss_state_6_10), .A1 (
                      ss_state_6_11), .B0 (nx1948)) ;
    NAND2_X0P5A_A12TS ix1786 (.Y (nx1948), .A (ss_state_6_11), .B (ss_state_6_10
                      )) ;
    XNOR3_X0P5M_A12TS ix1787 (.Y (nx1949), .A (max_selections[1]), .B (nx1918), 
                      .C (nx1917)) ;
    NOR2_X0P5A_A12TS ix1788 (.Y (nx1918), .A (nx1936), .B (nx1940)) ;
    XNOR3_X0P5M_A12TS ix1789 (.Y (nx1917), .A (nx1950), .B (nx1939), .C (nx1951)
                      ) ;
    CGENI_X1M_A12TS ix1790 (.CON (nx1950), .A (ss_state_6_1), .B (ss_state_6_0)
                    , .CI (nx1912)) ;
    XOR2_X0P5M_A12TS ix1791 (.Y (nx1951), .A (nx1905), .B (nx1952)) ;
    NOR2_X0P5A_A12TS ix1792 (.Y (nx1905), .A (nx1941), .B (nx1945)) ;
    XNOR3_X0P5M_A12TS ix1793 (.Y (nx1952), .A (nx1953), .B (nx1944), .C (nx1893)
                      ) ;
    CGENI_X1M_A12TS ix1794 (.CON (nx1953), .A (ss_state_6_5), .B (ss_state_6_4)
                    , .CI (nx1900)) ;
    AOI21_X0P5M_A12TS ix1795 (.Y (nx1893), .A0 (nx1948), .A1 (nx1954), .B0 (
                      nx1892)) ;
    CGENI_X1M_A12TS ix1796 (.CON (nx1954), .A (ss_state_6_9), .B (ss_state_6_8)
                    , .CI (nx1887)) ;
    XNOR3_X0P5M_A12TS ix1797 (.Y (nx1922), .A (max_selections[2]), .B (nx1919), 
                      .C (nx1956)) ;
    MXIT2_X0P5M_A12TS ix1798 (.Y (nx1919), .A (nx1951), .B (nx1955), .S0 (nx1917
                      )) ;
    XNOR2_X0P5M_A12TS ix1799 (.Y (nx1956), .A (nx1957), .B (nx1958)) ;
    NAND4_X0P5A_A12TS ix1800 (.Y (nx1957), .A (ss_state_6_1), .B (ss_state_6_0)
                      , .C (ss_state_6_3), .D (ss_state_6_2)) ;
    XNOR2_X0P5M_A12TS ix1801 (.Y (nx1958), .A (nx1959), .B (nx1960)) ;
    MXIT2_X0P5M_A12TS ix1802 (.Y (nx1959), .A (nx1905), .B (nx1893), .S0 (nx1952
                      )) ;
    XNOR2_X0P5M_A12TS ix1803 (.Y (nx1960), .A (nx1961), .B (nx1962)) ;
    NAND4_X0P5A_A12TS ix1804 (.Y (nx1961), .A (ss_state_6_5), .B (ss_state_6_4)
                      , .C (ss_state_6_7), .D (ss_state_6_6)) ;
    NAND4_X0P5A_A12TS ix1805 (.Y (nx1962), .A (ss_state_6_9), .B (ss_state_6_8)
                      , .C (ss_state_6_11), .D (ss_state_6_10)) ;
    XNOR3_X0P5M_A12TS ix1806 (.Y (nx1963), .A (max_selections[3]), .B (nx1920), 
                      .C (nx1921)) ;
    CGENI_X1M_A12TS ix1807 (.CON (nx1920), .A (nx1964), .B (nx1957), .CI (nx1958
                    )) ;
    CGENI_X1M_A12TS ix1808 (.CON (nx1921), .A (nx1959), .B (nx1961), .CI (nx1962
                    )) ;
    INV_X0P5B_A12TS ix1809 (.Y (nx1965), .A (reset)) ;
    NOR2_X0P5A_A12TS ix1810 (.Y (nx1916), .A (reset), .B (ss_state_6_1)) ;
    NAND2_X0P5A_A12TS ix1811 (.Y (nx1915), .A (nx1966), .B (nx1965)) ;
    AO21A1AI2_X0P5M_A12TS ix1812 (.Y (nx1966), .A0 (ss_state_6_1), .A1 (nx1907)
                          , .B0 (nx1906), .C0 (nx1967)) ;
    NOR3_X0P5A_A12TS ix1813 (.Y (nx1907), .A (nx1928), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR3_X0P5A_A12TS ix1814 (.Y (nx1906), .A (nx1930), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR2_X0P5A_A12TS ix1815 (.Y (nx1914), .A (reset), .B (ss_state_6_0)) ;
    NAND2_X0P5A_A12TS ix1816 (.Y (nx1913), .A (nx1968), .B (nx1965)) ;
    AO21A1AI2_X0P5M_A12TS ix1817 (.Y (nx1968), .A0 (ss_state_6_0), .A1 (nx1907)
                          , .B0 (nx1906), .C0 (nx1969)) ;
    NOR2_X0P5A_A12TS ix1818 (.Y (nx1969), .A (button_num_0), .B (button_num_1)
                     ) ;
    NOR2_X0P5A_A12TS ix1819 (.Y (nx1911), .A (reset), .B (ss_state_6_2)) ;
    NAND2_X0P5A_A12TS ix1820 (.Y (nx1910), .A (nx1970), .B (nx1965)) ;
    AO21A1AI2_X0P5M_A12TS ix1821 (.Y (nx1970), .A0 (ss_state_6_2), .A1 (nx1907)
                          , .B0 (nx1906), .C0 (nx1971)) ;
    NOR2_X0P5A_A12TS ix1822 (.Y (nx1909), .A (reset), .B (ss_state_6_3)) ;
    OAI21_X0P5M_A12TS ix1823 (.Y (nx1908), .A0 (nx1882), .A1 (nx1972), .B0 (
                      nx1965)) ;
    AOI21_X0P5M_A12TS ix1824 (.Y (nx1972), .A0 (ss_state_6_3), .A1 (nx1907), .B0 (
                      nx1906)) ;
    NOR2_X0P5A_A12TS ix1825 (.Y (nx1904), .A (reset), .B (ss_state_6_5)) ;
    NAND2_X0P5A_A12TS ix1826 (.Y (nx1903), .A (nx1973), .B (nx1965)) ;
    AO21A1AI2_X0P5M_A12TS ix1827 (.Y (nx1973), .A0 (ss_state_6_5), .A1 (nx1895)
                          , .B0 (nx1894), .C0 (nx1967)) ;
    NOR3_X0P5A_A12TS ix1828 (.Y (nx1895), .A (nx1928), .B (button_num_3), .C (
                     nx1974)) ;
    INV_X0P5B_A12TS ix1829 (.Y (nx1974), .A (button_num_2)) ;
    NOR3_X0P5A_A12TS ix1830 (.Y (nx1894), .A (nx1930), .B (button_num_3), .C (
                     nx1974)) ;
    NOR2_X0P5A_A12TS ix1831 (.Y (nx1902), .A (reset), .B (ss_state_6_4)) ;
    NAND2_X0P5A_A12TS ix1832 (.Y (nx1901), .A (nx1975), .B (nx1965)) ;
    AO21A1AI2_X0P5M_A12TS ix1833 (.Y (nx1975), .A0 (ss_state_6_4), .A1 (nx1895)
                          , .B0 (nx1894), .C0 (nx1969)) ;
    NOR2_X0P5A_A12TS ix1834 (.Y (nx1899), .A (reset), .B (ss_state_6_6)) ;
    NAND2_X0P5A_A12TS ix1835 (.Y (nx1898), .A (nx1976), .B (nx1965)) ;
    AO21A1AI2_X0P5M_A12TS ix1836 (.Y (nx1976), .A0 (ss_state_6_6), .A1 (nx1895)
                          , .B0 (nx1894), .C0 (nx1971)) ;
    NOR2_X0P5A_A12TS ix1837 (.Y (nx1897), .A (reset), .B (ss_state_6_7)) ;
    OAI21_X0P5M_A12TS ix1838 (.Y (nx1896), .A0 (nx1882), .A1 (nx1977), .B0 (
                      nx1965)) ;
    AOI21_X0P5M_A12TS ix1839 (.Y (nx1977), .A0 (ss_state_6_7), .A1 (nx1895), .B0 (
                      nx1894)) ;
    NOR2_X0P5A_A12TS ix1840 (.Y (nx1891), .A (reset), .B (ss_state_6_9)) ;
    NAND2_X0P5A_A12TS ix1841 (.Y (nx1890), .A (nx1978), .B (nx1965)) ;
    AO21A1AI2_X0P5M_A12TS ix1842 (.Y (nx1978), .A0 (ss_state_6_9), .A1 (nx1884)
                          , .B0 (nx1883), .C0 (nx1967)) ;
    NOR2_X0P5A_A12TS ix1843 (.Y (nx1889), .A (reset), .B (ss_state_6_8)) ;
    NAND2_X0P5A_A12TS ix1844 (.Y (nx1888), .A (nx1979), .B (nx1965)) ;
    AO21A1AI2_X0P5M_A12TS ix1845 (.Y (nx1979), .A0 (ss_state_6_8), .A1 (nx1884)
                          , .B0 (nx1883), .C0 (nx1969)) ;
    NOR2_X0P5A_A12TS ix1846 (.Y (nx1886), .A (reset), .B (ss_state_6_10)) ;
    NAND2_X0P5A_A12TS ix1847 (.Y (nx1885), .A (nx1980), .B (nx1965)) ;
    AO21A1AI2_X0P5M_A12TS ix1848 (.Y (nx1980), .A0 (ss_state_6_10), .A1 (nx1884)
                          , .B0 (nx1883), .C0 (nx1971)) ;
    TIELO_X1M_A12TS ix1849 (.Y (nx1881)) ;
    INV_X0P5B_A12TS ix1850 (.Y (nx1964), .A (nx1919)) ;
    INV_X0P5B_A12TS ix1851 (.Y (nx1955), .A (nx1918)) ;
    INV_X0P5B_A12TS ix1852 (.Y (nx1912), .A (nx1938)) ;
    INV_X0P5B_A12TS ix1853 (.Y (nx1900), .A (nx1943)) ;
    INV_X0P5B_A12TS ix1854 (.Y (nx1892), .A (nx1962)) ;
    INV_X0P5B_A12TS ix1855 (.Y (nx1887), .A (nx1947)) ;
    NOR2B_X0P7M_A12TS ix1856 (.Y (nx1967), .AN (button_num_0), .B (button_num_1)
                      ) ;
    NOR2B_X0P7M_A12TS ix1857 (.Y (nx1971), .AN (button_num_1), .B (button_num_0)
                      ) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_0__dup_28445 (.Q (ss_state_6_0), .CK (
                        osc_clk), .D (nx1983), .R (nx1881), .SN (nx1993)) ;
    MXT2_X0P5M_A12TS ix1858 (.Y (nx1983), .A (ss_state_6_0), .B (nx1914), .S0 (
                     nx1913)) ;
    INV_X0P5B_A12TS ix1859 (.Y (nx1993), .A (nx1881)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_1__dup_28448 (.Q (ss_state_6_1), .CK (
                        osc_clk), .D (nx1982), .R (nx1881), .SN (nx1993)) ;
    MXT2_X0P5M_A12TS ix1860 (.Y (nx1982), .A (ss_state_6_1), .B (nx1916), .S0 (
                     nx1915)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_2__dup_28450 (.Q (ss_state_6_2), .CK (
                        osc_clk), .D (nx1984), .R (nx1881), .SN (nx1993)) ;
    MXT2_X0P5M_A12TS ix1861 (.Y (nx1984), .A (ss_state_6_2), .B (nx1911), .S0 (
                     nx1910)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_3__dup_28452 (.Q (ss_state_6_3), .CK (
                        osc_clk), .D (nx1985), .R (nx1881), .SN (nx1993)) ;
    MXT2_X0P5M_A12TS ix1862 (.Y (nx1985), .A (ss_state_6_3), .B (nx1909), .S0 (
                     nx1908)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_4__dup_28454 (.Q (ss_state_6_4), .CK (
                        osc_clk), .D (nx1987), .R (nx1881), .SN (nx1993)) ;
    MXT2_X0P5M_A12TS ix1863 (.Y (nx1987), .A (ss_state_6_4), .B (nx1902), .S0 (
                     nx1901)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_5__dup_28456 (.Q (ss_state_6_5), .CK (
                        osc_clk), .D (nx1986), .R (nx1881), .SN (nx1993)) ;
    MXT2_X0P5M_A12TS ix1864 (.Y (nx1986), .A (ss_state_6_5), .B (nx1904), .S0 (
                     nx1903)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_6__dup_28458 (.Q (ss_state_6_6), .CK (
                        osc_clk), .D (nx1988), .R (nx1881), .SN (nx1993)) ;
    MXT2_X0P5M_A12TS ix1865 (.Y (nx1988), .A (ss_state_6_6), .B (nx1899), .S0 (
                     nx1898)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_7__dup_28460 (.Q (ss_state_6_7), .CK (
                        osc_clk), .D (nx1989), .R (nx1881), .SN (nx1993)) ;
    MXT2_X0P5M_A12TS ix1866 (.Y (nx1989), .A (ss_state_6_7), .B (nx1897), .S0 (
                     nx1896)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_8__dup_28462 (.Q (ss_state_6_8), .CK (
                        osc_clk), .D (nx1991), .R (nx1881), .SN (nx1993)) ;
    MXT2_X0P5M_A12TS ix1867 (.Y (nx1991), .A (ss_state_6_8), .B (nx1889), .S0 (
                     nx1888)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_9__dup_28464 (.Q (ss_state_6_9), .CK (
                        osc_clk), .D (nx1990), .R (nx1881), .SN (nx1993)) ;
    MXT2_X0P5M_A12TS ix1868 (.Y (nx1990), .A (ss_state_6_9), .B (nx1891), .S0 (
                     nx1890)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_10__dup_28466 (.Q (ss_state_6_10), .CK (
                        osc_clk), .D (nx1992), .R (nx1881), .SN (nx1993)) ;
    MXT2_X0P5M_A12TS ix1869 (.Y (nx1992), .A (ss_state_6_10), .B (nx1886), .S0 (
                     nx1885)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_11__dup_28468 (.Q (ss_state_6_11), .CK (
                        osc_clk), .D (nx1981), .R (nx1881), .SN (nx1993)) ;
    MXT2_X0P5M_A12TS ix1870 (.Y (nx1981), .A (ss_state_6_11), .B (nx1926), .S0 (
                     nx1925)) ;
    NOR2_X0P5A_A12TS ix1994 (.Y (nx2162), .A (reset), .B (ss_state_7_11)) ;
    OAI21_X0P5M_A12TS ix1995 (.Y (nx2161), .A0 (nx2118), .A1 (nx2163), .B0 (
                      nx2201)) ;
    NAND2_X0P5A_A12TS ix1996 (.Y (nx2118), .A (button_num_0), .B (button_num_1)
                      ) ;
    AOI21_X0P5M_A12TS ix1997 (.Y (nx2163), .A0 (ss_state_7_11), .A1 (nx2120), .B0 (
                      nx2119)) ;
    NOR3_X0P5A_A12TS ix1998 (.Y (nx2120), .A (nx2164), .B (nx2165), .C (
                     button_num_2)) ;
    INV_X0P5B_A12TS ix1999 (.Y (nx2164), .A (ss_selector_7)) ;
    INV_X0P5B_A12TS ix2000 (.Y (nx2165), .A (button_num_3)) ;
    NOR3_X0P5A_A12TS ix2001 (.Y (nx2119), .A (nx2166), .B (nx2165), .C (
                     button_num_2)) ;
    NAND2_X0P5A_A12TS ix2002 (.Y (nx2166), .A (ss_selector_7), .B (nx2160)) ;
    MXIT2_X0P5M_A12TS ix2003 (.Y (nx2160), .A (nx2167), .B (nx2168), .S0 (nx2199
                      )) ;
    INV_X0P5B_A12TS ix2004 (.Y (nx2167), .A (max_selections[3])) ;
    MXIT2_X0P5M_A12TS ix2005 (.Y (nx2168), .A (nx2159), .B (max_selections[2]), 
                      .S0 (nx2158)) ;
    MXIT2_X0P5M_A12TS ix2006 (.Y (nx2159), .A (nx2169), .B (nx2170), .S0 (nx2185
                      )) ;
    INV_X0P5B_A12TS ix2007 (.Y (nx2169), .A (max_selections[1])) ;
    NAND2_X0P5A_A12TS ix2008 (.Y (nx2170), .A (max_selections[0]), .B (nx2171)
                      ) ;
    XNOR2_X0P5M_A12TS ix2009 (.Y (nx2171), .A (nx2172), .B (nx2176)) ;
    XOR2_X0P5M_A12TS ix2010 (.Y (nx2172), .A (ss_state_7_1), .B (nx2173)) ;
    XOR2_X0P5M_A12TS ix2011 (.Y (nx2173), .A (ss_state_7_0), .B (nx2174)) ;
    OAI21_X0P5M_A12TS ix2012 (.Y (nx2174), .A0 (ss_state_7_2), .A1 (ss_state_7_3
                      ), .B0 (nx2175)) ;
    NAND2_X0P5A_A12TS ix2013 (.Y (nx2175), .A (ss_state_7_3), .B (ss_state_7_2)
                      ) ;
    XNOR2_X0P5M_A12TS ix2014 (.Y (nx2176), .A (nx2177), .B (nx2181)) ;
    XOR2_X0P5M_A12TS ix2015 (.Y (nx2177), .A (ss_state_7_5), .B (nx2178)) ;
    XOR2_X0P5M_A12TS ix2016 (.Y (nx2178), .A (ss_state_7_4), .B (nx2179)) ;
    OAI21_X0P5M_A12TS ix2017 (.Y (nx2179), .A0 (ss_state_7_6), .A1 (ss_state_7_7
                      ), .B0 (nx2180)) ;
    NAND2_X0P5A_A12TS ix2018 (.Y (nx2180), .A (ss_state_7_7), .B (ss_state_7_6)
                      ) ;
    XOR2_X0P5M_A12TS ix2019 (.Y (nx2181), .A (ss_state_7_9), .B (nx2182)) ;
    XOR2_X0P5M_A12TS ix2020 (.Y (nx2182), .A (ss_state_7_8), .B (nx2183)) ;
    OAI21_X0P5M_A12TS ix2021 (.Y (nx2183), .A0 (ss_state_7_10), .A1 (
                      ss_state_7_11), .B0 (nx2184)) ;
    NAND2_X0P5A_A12TS ix2022 (.Y (nx2184), .A (ss_state_7_11), .B (ss_state_7_10
                      )) ;
    XNOR3_X0P5M_A12TS ix2023 (.Y (nx2185), .A (max_selections[1]), .B (nx2154), 
                      .C (nx2153)) ;
    NOR2_X0P5A_A12TS ix2024 (.Y (nx2154), .A (nx2172), .B (nx2176)) ;
    XNOR3_X0P5M_A12TS ix2025 (.Y (nx2153), .A (nx2186), .B (nx2175), .C (nx2187)
                      ) ;
    CGENI_X1M_A12TS ix2026 (.CON (nx2186), .A (ss_state_7_1), .B (ss_state_7_0)
                    , .CI (nx2148)) ;
    XOR2_X0P5M_A12TS ix2027 (.Y (nx2187), .A (nx2141), .B (nx2188)) ;
    NOR2_X0P5A_A12TS ix2028 (.Y (nx2141), .A (nx2177), .B (nx2181)) ;
    XNOR3_X0P5M_A12TS ix2029 (.Y (nx2188), .A (nx2189), .B (nx2180), .C (nx2129)
                      ) ;
    CGENI_X1M_A12TS ix2030 (.CON (nx2189), .A (ss_state_7_5), .B (ss_state_7_4)
                    , .CI (nx2136)) ;
    AOI21_X0P5M_A12TS ix2031 (.Y (nx2129), .A0 (nx2184), .A1 (nx2190), .B0 (
                      nx2128)) ;
    CGENI_X1M_A12TS ix2032 (.CON (nx2190), .A (ss_state_7_9), .B (ss_state_7_8)
                    , .CI (nx2123)) ;
    XNOR3_X0P5M_A12TS ix2033 (.Y (nx2158), .A (max_selections[2]), .B (nx2155), 
                      .C (nx2192)) ;
    MXIT2_X0P5M_A12TS ix2034 (.Y (nx2155), .A (nx2187), .B (nx2191), .S0 (nx2153
                      )) ;
    XNOR2_X0P5M_A12TS ix2035 (.Y (nx2192), .A (nx2193), .B (nx2194)) ;
    NAND4_X0P5A_A12TS ix2036 (.Y (nx2193), .A (ss_state_7_1), .B (ss_state_7_0)
                      , .C (ss_state_7_3), .D (ss_state_7_2)) ;
    XNOR2_X0P5M_A12TS ix2037 (.Y (nx2194), .A (nx2195), .B (nx2196)) ;
    MXIT2_X0P5M_A12TS ix2038 (.Y (nx2195), .A (nx2141), .B (nx2129), .S0 (nx2188
                      )) ;
    XNOR2_X0P5M_A12TS ix2039 (.Y (nx2196), .A (nx2197), .B (nx2198)) ;
    NAND4_X0P5A_A12TS ix2040 (.Y (nx2197), .A (ss_state_7_5), .B (ss_state_7_4)
                      , .C (ss_state_7_7), .D (ss_state_7_6)) ;
    NAND4_X0P5A_A12TS ix2041 (.Y (nx2198), .A (ss_state_7_9), .B (ss_state_7_8)
                      , .C (ss_state_7_11), .D (ss_state_7_10)) ;
    XNOR3_X0P5M_A12TS ix2042 (.Y (nx2199), .A (max_selections[3]), .B (nx2156), 
                      .C (nx2157)) ;
    CGENI_X1M_A12TS ix2043 (.CON (nx2156), .A (nx2200), .B (nx2193), .CI (nx2194
                    )) ;
    CGENI_X1M_A12TS ix2044 (.CON (nx2157), .A (nx2195), .B (nx2197), .CI (nx2198
                    )) ;
    INV_X0P5B_A12TS ix2045 (.Y (nx2201), .A (reset)) ;
    NOR2_X0P5A_A12TS ix2046 (.Y (nx2152), .A (reset), .B (ss_state_7_1)) ;
    NAND2_X0P5A_A12TS ix2047 (.Y (nx2151), .A (nx2202), .B (nx2201)) ;
    AO21A1AI2_X0P5M_A12TS ix2048 (.Y (nx2202), .A0 (ss_state_7_1), .A1 (nx2143)
                          , .B0 (nx2142), .C0 (nx2203)) ;
    NOR3_X0P5A_A12TS ix2049 (.Y (nx2143), .A (nx2164), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR3_X0P5A_A12TS ix2050 (.Y (nx2142), .A (nx2166), .B (button_num_3), .C (
                     button_num_2)) ;
    NOR2_X0P5A_A12TS ix2051 (.Y (nx2150), .A (reset), .B (ss_state_7_0)) ;
    NAND2_X0P5A_A12TS ix2052 (.Y (nx2149), .A (nx2204), .B (nx2201)) ;
    AO21A1AI2_X0P5M_A12TS ix2053 (.Y (nx2204), .A0 (ss_state_7_0), .A1 (nx2143)
                          , .B0 (nx2142), .C0 (nx2205)) ;
    NOR2_X0P5A_A12TS ix2054 (.Y (nx2205), .A (button_num_0), .B (button_num_1)
                     ) ;
    NOR2_X0P5A_A12TS ix2055 (.Y (nx2147), .A (reset), .B (ss_state_7_2)) ;
    NAND2_X0P5A_A12TS ix2056 (.Y (nx2146), .A (nx2206), .B (nx2201)) ;
    AO21A1AI2_X0P5M_A12TS ix2057 (.Y (nx2206), .A0 (ss_state_7_2), .A1 (nx2143)
                          , .B0 (nx2142), .C0 (nx2207)) ;
    NOR2_X0P5A_A12TS ix2058 (.Y (nx2145), .A (reset), .B (ss_state_7_3)) ;
    OAI21_X0P5M_A12TS ix2059 (.Y (nx2144), .A0 (nx2118), .A1 (nx2208), .B0 (
                      nx2201)) ;
    AOI21_X0P5M_A12TS ix2060 (.Y (nx2208), .A0 (ss_state_7_3), .A1 (nx2143), .B0 (
                      nx2142)) ;
    NOR2_X0P5A_A12TS ix2061 (.Y (nx2140), .A (reset), .B (ss_state_7_5)) ;
    NAND2_X0P5A_A12TS ix2062 (.Y (nx2139), .A (nx2209), .B (nx2201)) ;
    AO21A1AI2_X0P5M_A12TS ix2063 (.Y (nx2209), .A0 (ss_state_7_5), .A1 (nx2131)
                          , .B0 (nx2130), .C0 (nx2203)) ;
    NOR3_X0P5A_A12TS ix2064 (.Y (nx2131), .A (nx2164), .B (button_num_3), .C (
                     nx2210)) ;
    INV_X0P5B_A12TS ix2065 (.Y (nx2210), .A (button_num_2)) ;
    NOR3_X0P5A_A12TS ix2066 (.Y (nx2130), .A (nx2166), .B (button_num_3), .C (
                     nx2210)) ;
    NOR2_X0P5A_A12TS ix2067 (.Y (nx2138), .A (reset), .B (ss_state_7_4)) ;
    NAND2_X0P5A_A12TS ix2068 (.Y (nx2137), .A (nx2211), .B (nx2201)) ;
    AO21A1AI2_X0P5M_A12TS ix2069 (.Y (nx2211), .A0 (ss_state_7_4), .A1 (nx2131)
                          , .B0 (nx2130), .C0 (nx2205)) ;
    NOR2_X0P5A_A12TS ix2070 (.Y (nx2135), .A (reset), .B (ss_state_7_6)) ;
    NAND2_X0P5A_A12TS ix2071 (.Y (nx2134), .A (nx2212), .B (nx2201)) ;
    AO21A1AI2_X0P5M_A12TS ix2072 (.Y (nx2212), .A0 (ss_state_7_6), .A1 (nx2131)
                          , .B0 (nx2130), .C0 (nx2207)) ;
    NOR2_X0P5A_A12TS ix2073 (.Y (nx2133), .A (reset), .B (ss_state_7_7)) ;
    OAI21_X0P5M_A12TS ix2074 (.Y (nx2132), .A0 (nx2118), .A1 (nx2213), .B0 (
                      nx2201)) ;
    AOI21_X0P5M_A12TS ix2075 (.Y (nx2213), .A0 (ss_state_7_7), .A1 (nx2131), .B0 (
                      nx2130)) ;
    NOR2_X0P5A_A12TS ix2076 (.Y (nx2127), .A (reset), .B (ss_state_7_9)) ;
    NAND2_X0P5A_A12TS ix2077 (.Y (nx2126), .A (nx2214), .B (nx2201)) ;
    AO21A1AI2_X0P5M_A12TS ix2078 (.Y (nx2214), .A0 (ss_state_7_9), .A1 (nx2120)
                          , .B0 (nx2119), .C0 (nx2203)) ;
    NOR2_X0P5A_A12TS ix2079 (.Y (nx2125), .A (reset), .B (ss_state_7_8)) ;
    NAND2_X0P5A_A12TS ix2080 (.Y (nx2124), .A (nx2215), .B (nx2201)) ;
    AO21A1AI2_X0P5M_A12TS ix2081 (.Y (nx2215), .A0 (ss_state_7_8), .A1 (nx2120)
                          , .B0 (nx2119), .C0 (nx2205)) ;
    NOR2_X0P5A_A12TS ix2082 (.Y (nx2122), .A (reset), .B (ss_state_7_10)) ;
    NAND2_X0P5A_A12TS ix2083 (.Y (nx2121), .A (nx2216), .B (nx2201)) ;
    AO21A1AI2_X0P5M_A12TS ix2084 (.Y (nx2216), .A0 (ss_state_7_10), .A1 (nx2120)
                          , .B0 (nx2119), .C0 (nx2207)) ;
    TIELO_X1M_A12TS ix2085 (.Y (nx2117)) ;
    INV_X0P5B_A12TS ix2086 (.Y (nx2200), .A (nx2155)) ;
    INV_X0P5B_A12TS ix2087 (.Y (nx2191), .A (nx2154)) ;
    INV_X0P5B_A12TS ix2088 (.Y (nx2148), .A (nx2174)) ;
    INV_X0P5B_A12TS ix2089 (.Y (nx2136), .A (nx2179)) ;
    INV_X0P5B_A12TS ix2090 (.Y (nx2128), .A (nx2198)) ;
    INV_X0P5B_A12TS ix2091 (.Y (nx2123), .A (nx2183)) ;
    NOR2B_X0P7M_A12TS ix2092 (.Y (nx2203), .AN (button_num_0), .B (button_num_1)
                      ) ;
    NOR2B_X0P7M_A12TS ix2093 (.Y (nx2207), .AN (button_num_1), .B (button_num_0)
                      ) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_0__dup_28570 (.Q (ss_state_7_0), .CK (
                        osc_clk), .D (nx2219), .R (nx2117), .SN (nx2229)) ;
    MXT2_X0P5M_A12TS ix2094 (.Y (nx2219), .A (ss_state_7_0), .B (nx2150), .S0 (
                     nx2149)) ;
    INV_X0P5B_A12TS ix2095 (.Y (nx2229), .A (nx2117)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_1__dup_28573 (.Q (ss_state_7_1), .CK (
                        osc_clk), .D (nx2218), .R (nx2117), .SN (nx2229)) ;
    MXT2_X0P5M_A12TS ix2096 (.Y (nx2218), .A (ss_state_7_1), .B (nx2152), .S0 (
                     nx2151)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_2__dup_28575 (.Q (ss_state_7_2), .CK (
                        osc_clk), .D (nx2220), .R (nx2117), .SN (nx2229)) ;
    MXT2_X0P5M_A12TS ix2097 (.Y (nx2220), .A (ss_state_7_2), .B (nx2147), .S0 (
                     nx2146)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_3__dup_28577 (.Q (ss_state_7_3), .CK (
                        osc_clk), .D (nx2221), .R (nx2117), .SN (nx2229)) ;
    MXT2_X0P5M_A12TS ix2098 (.Y (nx2221), .A (ss_state_7_3), .B (nx2145), .S0 (
                     nx2144)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_4__dup_28579 (.Q (ss_state_7_4), .CK (
                        osc_clk), .D (nx2223), .R (nx2117), .SN (nx2229)) ;
    MXT2_X0P5M_A12TS ix2099 (.Y (nx2223), .A (ss_state_7_4), .B (nx2138), .S0 (
                     nx2137)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_5__dup_28581 (.Q (ss_state_7_5), .CK (
                        osc_clk), .D (nx2222), .R (nx2117), .SN (nx2229)) ;
    MXT2_X0P5M_A12TS ix2100 (.Y (nx2222), .A (ss_state_7_5), .B (nx2140), .S0 (
                     nx2139)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_6__dup_28583 (.Q (ss_state_7_6), .CK (
                        osc_clk), .D (nx2224), .R (nx2117), .SN (nx2229)) ;
    MXT2_X0P5M_A12TS ix2101 (.Y (nx2224), .A (ss_state_7_6), .B (nx2135), .S0 (
                     nx2134)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_7__dup_28585 (.Q (ss_state_7_7), .CK (
                        osc_clk), .D (nx2225), .R (nx2117), .SN (nx2229)) ;
    MXT2_X0P5M_A12TS ix2102 (.Y (nx2225), .A (ss_state_7_7), .B (nx2133), .S0 (
                     nx2132)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_8__dup_28587 (.Q (ss_state_7_8), .CK (
                        osc_clk), .D (nx2227), .R (nx2117), .SN (nx2229)) ;
    MXT2_X0P5M_A12TS ix2103 (.Y (nx2227), .A (ss_state_7_8), .B (nx2125), .S0 (
                     nx2124)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_9__dup_28589 (.Q (ss_state_7_9), .CK (
                        osc_clk), .D (nx2226), .R (nx2117), .SN (nx2229)) ;
    MXT2_X0P5M_A12TS ix2104 (.Y (nx2226), .A (ss_state_7_9), .B (nx2127), .S0 (
                     nx2126)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_10__dup_28591 (.Q (ss_state_7_10), .CK (
                        osc_clk), .D (nx2228), .R (nx2117), .SN (nx2229)) ;
    MXT2_X0P5M_A12TS ix2105 (.Y (nx2228), .A (ss_state_7_10), .B (nx2122), .S0 (
                     nx2121)) ;
    DFFSRPQ_X0P5M_A12TS reg_selection_state_11__dup_28593 (.Q (ss_state_7_11), .CK (
                        osc_clk), .D (nx2217), .R (nx2117), .SN (nx2229)) ;
    MXT2_X0P5M_A12TS ix2106 (.Y (nx2217), .A (ss_state_7_11), .B (nx2162), .S0 (
                     nx2161)) ;
    NAND4_X0P5A_A12TS ix85 (.Y (current_contest_state[0]), .A (nx361), .B (nx371
                      ), .C (nx379), .D (nx2266)) ;
    AOI22_X0P5M_A12TS ix362 (.Y (nx361), .A0 (ss_state_0_0), .A1 (nx2265), .B0 (
                      ss_state_6_0), .B1 (nx76)) ;
    NOR3_X0P5A_A12TS ix2230 (.Y (nx2265), .A (contest_num[0]), .B (
                     contest_num[2]), .C (contest_num[1])) ;
    NOR3_X0P5A_A12TS ix77 (.Y (nx76), .A (contest_num[0]), .B (nx367), .C (nx369
                     )) ;
    INV_X0P5B_A12TS ix368 (.Y (nx367), .A (contest_num[2])) ;
    INV_X0P5B_A12TS ix370 (.Y (nx369), .A (contest_num[1])) ;
    AOI22_X0P5M_A12TS ix372 (.Y (nx371), .A0 (ss_state_4_0), .A1 (nx54), .B0 (
                      ss_state_7_0), .B1 (nx2264)) ;
    NOR3_X0P5A_A12TS ix55 (.Y (nx54), .A (contest_num[0]), .B (nx367), .C (
                     contest_num[1])) ;
    INV_X0P5B_A12TS ix378 (.Y (nx377), .A (contest_num[0])) ;
    AOI22_X0P5M_A12TS ix380 (.Y (nx379), .A0 (ss_state_1_0), .A1 (nx2263), .B0 (
                      ss_state_3_0), .B1 (nx34)) ;
    NOR3_X0P5A_A12TS ix2231 (.Y (nx2263), .A (nx377), .B (contest_num[2]), .C (
                     contest_num[1])) ;
    NOR3_X0P5A_A12TS ix35 (.Y (nx34), .A (nx377), .B (contest_num[2]), .C (nx369
                     )) ;
    AOI22_X0P5M_A12TS ix2232 (.Y (nx2266), .A0 (ss_state_2_0), .A1 (nx2262), .B0 (
                      ss_state_5_0), .B1 (nx2261)) ;
    NOR3_X0P5A_A12TS ix2233 (.Y (nx2262), .A (contest_num[0]), .B (
                     contest_num[2]), .C (nx369)) ;
    NOR3_X0P5A_A12TS ix2234 (.Y (nx2261), .A (nx377), .B (nx367), .C (
                     contest_num[1])) ;
    NAND4_X0P5A_A12TS ix115 (.Y (current_contest_state[1]), .A (nx393), .B (
                      nx395), .C (nx397), .D (nx399)) ;
    AOI22_X0P5M_A12TS ix394 (.Y (nx393), .A0 (ss_state_0_1), .A1 (nx2265), .B0 (
                      ss_state_6_1), .B1 (nx76)) ;
    AOI22_X0P5M_A12TS ix396 (.Y (nx395), .A0 (ss_state_4_1), .A1 (nx54), .B0 (
                      ss_state_7_1), .B1 (nx2264)) ;
    AOI22_X0P5M_A12TS ix398 (.Y (nx397), .A0 (ss_state_1_1), .A1 (nx2263), .B0 (
                      ss_state_3_1), .B1 (nx34)) ;
    AOI22_X0P5M_A12TS ix400 (.Y (nx399), .A0 (ss_state_2_1), .A1 (nx2262), .B0 (
                      ss_state_5_1), .B1 (nx2261)) ;
    NAND4_X0P5A_A12TS ix145 (.Y (current_contest_state[2]), .A (nx403), .B (
                      nx405), .C (nx407), .D (nx409)) ;
    AOI22_X0P5M_A12TS ix2235 (.Y (nx403), .A0 (ss_state_0_2), .A1 (nx2265), .B0 (
                      ss_state_6_2), .B1 (nx76)) ;
    AOI22_X0P5M_A12TS ix406 (.Y (nx405), .A0 (ss_state_4_2), .A1 (nx54), .B0 (
                      ss_state_7_2), .B1 (nx2264)) ;
    AOI22_X0P5M_A12TS ix408 (.Y (nx407), .A0 (ss_state_1_2), .A1 (nx2263), .B0 (
                      ss_state_3_2), .B1 (nx34)) ;
    AOI22_X0P5M_A12TS ix410 (.Y (nx409), .A0 (ss_state_2_2), .A1 (nx2262), .B0 (
                      ss_state_5_2), .B1 (nx2261)) ;
    NAND4_X0P5A_A12TS ix175 (.Y (current_contest_state[3]), .A (nx413), .B (
                      nx415), .C (nx417), .D (nx419)) ;
    AOI22_X0P5M_A12TS ix414 (.Y (nx413), .A0 (ss_state_0_3), .A1 (nx2265), .B0 (
                      ss_state_6_3), .B1 (nx76)) ;
    AOI22_X0P5M_A12TS ix416 (.Y (nx415), .A0 (ss_state_4_3), .A1 (nx54), .B0 (
                      ss_state_7_3), .B1 (nx2264)) ;
    AOI22_X0P5M_A12TS ix418 (.Y (nx417), .A0 (ss_state_1_3), .A1 (nx2263), .B0 (
                      ss_state_3_3), .B1 (nx34)) ;
    AOI22_X0P5M_A12TS ix420 (.Y (nx419), .A0 (ss_state_2_3), .A1 (nx2262), .B0 (
                      ss_state_5_3), .B1 (nx2261)) ;
    NAND4_X0P5A_A12TS ix2236 (.Y (current_contest_state[4]), .A (nx422), .B (
                      nx424), .C (nx2267), .D (nx2268)) ;
    AOI22_X0P5M_A12TS ix423 (.Y (nx422), .A0 (ss_state_0_4), .A1 (nx2265), .B0 (
                      ss_state_6_4), .B1 (nx76)) ;
    AOI22_X0P5M_A12TS ix425 (.Y (nx424), .A0 (ss_state_4_4), .A1 (nx54), .B0 (
                      ss_state_7_4), .B1 (nx2264)) ;
    AOI22_X0P5M_A12TS ix2237 (.Y (nx2267), .A0 (ss_state_1_4), .A1 (nx2263), .B0 (
                      ss_state_3_4), .B1 (nx34)) ;
    AOI22_X0P5M_A12TS ix2238 (.Y (nx2268), .A0 (ss_state_2_4), .A1 (nx2262), .B0 (
                      ss_state_5_4), .B1 (nx2261)) ;
    NAND4_X0P5A_A12TS ix235 (.Y (current_contest_state[5]), .A (nx431), .B (
                      nx433), .C (nx435), .D (nx437)) ;
    AOI22_X0P5M_A12TS ix432 (.Y (nx431), .A0 (ss_state_0_5), .A1 (nx2265), .B0 (
                      ss_state_6_5), .B1 (nx76)) ;
    AOI22_X0P5M_A12TS ix434 (.Y (nx433), .A0 (ss_state_4_5), .A1 (nx54), .B0 (
                      ss_state_7_5), .B1 (nx2264)) ;
    AOI22_X0P5M_A12TS ix2239 (.Y (nx435), .A0 (ss_state_1_5), .A1 (nx2263), .B0 (
                      ss_state_3_5), .B1 (nx34)) ;
    AOI22_X0P5M_A12TS ix438 (.Y (nx437), .A0 (ss_state_2_5), .A1 (nx2262), .B0 (
                      ss_state_5_5), .B1 (nx2261)) ;
    NAND4_X0P5A_A12TS ix265 (.Y (current_contest_state[6]), .A (nx2269), .B (
                      nx442), .C (nx444), .D (nx446)) ;
    AOI22_X0P5M_A12TS ix2240 (.Y (nx2269), .A0 (ss_state_0_6), .A1 (nx2265), .B0 (
                      ss_state_6_6), .B1 (nx76)) ;
    AOI22_X0P5M_A12TS ix443 (.Y (nx442), .A0 (ss_state_4_6), .A1 (nx54), .B0 (
                      ss_state_7_6), .B1 (nx2264)) ;
    AOI22_X0P5M_A12TS ix445 (.Y (nx444), .A0 (ss_state_1_6), .A1 (nx2263), .B0 (
                      ss_state_3_6), .B1 (nx34)) ;
    AOI22_X0P5M_A12TS ix447 (.Y (nx446), .A0 (ss_state_2_6), .A1 (nx2262), .B0 (
                      ss_state_5_6), .B1 (nx2261)) ;
    NAND4_X0P5A_A12TS ix2241 (.Y (current_contest_state[7]), .A (nx2270), .B (
                      nx451), .C (nx453), .D (nx455)) ;
    AOI22_X0P5M_A12TS ix2242 (.Y (nx2270), .A0 (ss_state_0_7), .A1 (nx2265), .B0 (
                      ss_state_6_7), .B1 (nx76)) ;
    AOI22_X0P5M_A12TS ix2243 (.Y (nx451), .A0 (ss_state_4_7), .A1 (nx54), .B0 (
                      ss_state_7_7), .B1 (nx2264)) ;
    AOI22_X0P5M_A12TS ix2244 (.Y (nx453), .A0 (ss_state_1_7), .A1 (nx2263), .B0 (
                      ss_state_3_7), .B1 (nx34)) ;
    AOI22_X0P5M_A12TS ix2245 (.Y (nx455), .A0 (ss_state_2_7), .A1 (nx2262), .B0 (
                      ss_state_5_7), .B1 (nx2261)) ;
    NAND4_X0P5A_A12TS ix325 (.Y (current_contest_state[8]), .A (nx2271), .B (
                      nx460), .C (nx2272), .D (nx464)) ;
    AOI22_X0P5M_A12TS ix2246 (.Y (nx2271), .A0 (ss_state_0_8), .A1 (nx2265), .B0 (
                      ss_state_6_8), .B1 (nx76)) ;
    AOI22_X0P5M_A12TS ix2247 (.Y (nx460), .A0 (ss_state_4_8), .A1 (nx54), .B0 (
                      ss_state_7_8), .B1 (nx2264)) ;
    AOI22_X0P5M_A12TS ix2248 (.Y (nx2272), .A0 (ss_state_1_8), .A1 (nx2263), .B0 (
                      ss_state_3_8), .B1 (nx34)) ;
    AOI22_X0P5M_A12TS ix2249 (.Y (nx464), .A0 (ss_state_2_8), .A1 (nx2262), .B0 (
                      ss_state_5_8), .B1 (nx2261)) ;
    NAND4_X0P5A_A12TS ix355 (.Y (current_contest_state[9]), .A (nx467), .B (
                      nx469), .C (nx471), .D (nx473)) ;
    AOI22_X0P5M_A12TS ix2250 (.Y (nx467), .A0 (ss_state_0_9), .A1 (nx2265), .B0 (
                      ss_state_6_9), .B1 (nx76)) ;
    AOI22_X0P5M_A12TS ix2251 (.Y (nx469), .A0 (ss_state_4_9), .A1 (nx54), .B0 (
                      ss_state_7_9), .B1 (nx2264)) ;
    AOI22_X0P5M_A12TS ix2252 (.Y (nx471), .A0 (ss_state_1_9), .A1 (nx2263), .B0 (
                      ss_state_3_9), .B1 (nx34)) ;
    AOI22_X0P5M_A12TS ix2253 (.Y (nx473), .A0 (ss_state_2_9), .A1 (nx2262), .B0 (
                      ss_state_5_9), .B1 (nx2261)) ;
    NAND4_X0P5A_A12TS ix385 (.Y (current_contest_state[10]), .A (nx476), .B (
                      nx478), .C (nx480), .D (nx482)) ;
    AOI22_X0P5M_A12TS ix2254 (.Y (nx476), .A0 (ss_state_0_10), .A1 (nx2265), .B0 (
                      ss_state_6_10), .B1 (nx76)) ;
    AOI22_X0P5M_A12TS ix2255 (.Y (nx478), .A0 (ss_state_4_10), .A1 (nx54), .B0 (
                      ss_state_7_10), .B1 (nx2264)) ;
    AOI22_X0P5M_A12TS ix2256 (.Y (nx480), .A0 (ss_state_1_10), .A1 (nx2263), .B0 (
                      ss_state_3_10), .B1 (nx34)) ;
    AOI22_X0P5M_A12TS ix2257 (.Y (nx482), .A0 (ss_state_2_10), .A1 (nx2262), .B0 (
                      ss_state_5_10), .B1 (nx2261)) ;
    NAND4_X0P5A_A12TS ix2258 (.Y (current_contest_state[11]), .A (nx485), .B (
                      nx2273), .C (nx489), .D (nx491)) ;
    AOI22_X0P5M_A12TS ix486 (.Y (nx485), .A0 (ss_state_0_11), .A1 (nx2265), .B0 (
                      ss_state_6_11), .B1 (nx76)) ;
    AOI22_X0P5M_A12TS ix2259 (.Y (nx2273), .A0 (ss_state_4_11), .A1 (nx54), .B0 (
                      ss_state_7_11), .B1 (nx2264)) ;
    AOI22_X0P5M_A12TS ix490 (.Y (nx489), .A0 (ss_state_1_11), .A1 (nx2263), .B0 (
                      ss_state_3_11), .B1 (nx34)) ;
    AOI22_X0P5M_A12TS ix492 (.Y (nx491), .A0 (ss_state_2_11), .A1 (nx2262), .B0 (
                      ss_state_5_11), .B1 (nx2261)) ;
    AND3_X0P5M_A12TS ix2260 (.Y (nx2264), .A (contest_num[0]), .B (
                     contest_num[2]), .C (contest_num[1])) ;
endmodule
