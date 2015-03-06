module router ( clk, reset, currx, curry, flitin_0, flitin_1, flitout_0, 
                flitout_1, creditin_0, creditin_1, creditout_0, creditout_1, seu
                 ) ;

    input clk ;
    input reset ;
    input [2:0]currx ;
    input [2:0]curry ;
    input [11:0]flitin_0 ;
    input [11:0]flitin_1 ;
    output [11:0]flitout_0 ;
    output [11:0]flitout_1 ;
    input creditin_0 ;
    input creditin_1 ;
    output creditout_0 ;
    output creditout_1 ;
    input seu ;

    wire colsel1_1, colsel1_0, colsel0_1, colsel0_0, flitout_switch_1_11, 
         flitout_switch_1_10, flitout_switch_1_9, flitout_switch_1_8, 
         flitout_switch_1_7, flitout_switch_1_6, flitout_switch_1_5, 
         flitout_switch_1_4, flitout_switch_1_3, flitout_switch_1_2, 
         flitout_switch_1_1, flitout_switch_1_0, tailout_1, debitout_1, 
         swalloc_resp_1_1, swalloc_resp_1_0, swalloc_req_1_1, swalloc_req_1_0, 
         flitout_switch_0_11, flitout_switch_0_10, flitout_switch_0_9, 
         flitout_switch_0_8, flitout_switch_0_7, flitout_switch_0_6, 
         flitout_switch_0_5, flitout_switch_0_4, flitout_switch_0_3, 
         flitout_switch_0_2, flitout_switch_0_1, flitout_switch_0_0, tailout_0, 
         debitout_0, swalloc_resp_0_1, swalloc_resp_0_0, swalloc_req_0_1, 
         swalloc_req_0_0, nx225, headflit_7, headflit_6, headflit_5, headflit_4, 
         headflit_3, headflit_2, headflit_1, headflit_0, routewire_1, 
         routewire_0, renable, validflitin, nx199, headflit_7__dup_261, 
         headflit_6__dup_262, headflit_5__dup_263, headflit_4__dup_264, 
         headflit_3__dup_265, headflit_2__dup_266, headflit_1__dup_267, 
         headflit_0__dup_268, routewire_1__dup_269, routewire_0__dup_270, 
         renable_dup_418, validflitin_dup_488, nx273, nomorebufs0, nomorebufs1, 
         nxt_resp1_1, nxt_resp1_0, nxt_resp0_1, nxt_resp0_0, state_invc1_1, 
         state_invc1_0, state_invc0_1, state_invc0_0, no_bufs1, no_bufs0, 
         tail_outvc1, tail_outvc0, debit_outvc1, debit_outvc0, elig_1, elig_0, 
         request1_1, request1_0, request0_1, request0_0, GND, nx44, nx283, nx48, 
         nx68, nx80, nx284, nx84, nx104, nx294, nx300, nx308, nx313, nx315, 
         nx318, nx320, nx322, nx325, nx328, nx330, nx333, nx370, NOT_nx284, 
         NOT_nx283, nx433, colsel1reg_1, colsel1reg_0, colsel0reg_1, 
         colsel0reg_0, GND_dup_287, PWR, nx0, nx8, nx16, nx24, headflit_11, 
         headflit_10, headflit_9, headflit_8, rdata_11, rdata_10, rdata_9, 
         rdata_8, rdata_7, rdata_6, rdata_5, rdata_4, rdata_3, rdata_2, rdata_1, 
         rdata_0, wdata_11, wdata_10, wdata_9, wdata_8, wdata_7, wdata_6, 
         wdata_5, wdata_4, wdata_3, wdata_2, wenable, dqenablereg, nx305, nx336, 
         nx12, nx28, nx321, nx58, nx66, nx78, nx180, nx184, nx186, nx189, nx191, 
         nx193, nx196, nx198, nx210, state_status_0, nx958, state_status_1, nx26, 
         nx40, nx334, nx70, nx74, nx90, nx94, nx118, state_queuelen_2, 
         state_queuelen_1, nx960, state_queuelen_0, nx124, nx150, full, nx188, 
         nx230, nx246, nx969, nx973, nx975, nx977, nx979, nx983, nx985, nx989, 
         nx992, nx994, nx998, nx1001, nx1004, nx1007, nx1009, nx1011, nx1013, 
         nx1018, nx1021, nx1025, nx1027, nx1029, nx1031, nx1033, nx1035, nx1041, 
         nx1045, nx1047, nx1051, headflit_11__dup_363, headflit_10__dup_364, 
         headflit_9__dup_365, headflit_8__dup_366, rdata_11__dup_375, 
         rdata_10__dup_376, rdata_9__dup_377, rdata_8__dup_378, rdata_7__dup_379, 
         rdata_6__dup_380, rdata_5__dup_381, rdata_4__dup_382, rdata_3__dup_383, 
         rdata_2__dup_384, rdata_1__dup_385, rdata_0__dup_386, wdata_11__dup_387, 
         wdata_10__dup_388, wdata_9__dup_389, wdata_8__dup_390, wdata_7__dup_391, 
         wdata_6__dup_392, wdata_5__dup_393, wdata_4__dup_394, wdata_3__dup_395, 
         wdata_2__dup_396, wenable_dup_397, dqenablereg_dup_398, nx399, nx400, 
         nx425, nx426, nx427, nx428, nx429, nx430, nx431, nx432, nx434, nx435, 
         nx436, nx437, nx438, nx439, nx440, state_status_0__dup_489, nx490, 
         state_status_1__dup_491, nx492, nx493, nx494, nx495, nx496, nx497, 
         nx498, nx499, state_queuelen_2__dup_500, state_queuelen_1__dup_501, 
         nx502, state_queuelen_0__dup_503, nx504, nx505, full_dup_506, nx507, 
         nx508, nx509, nx510, nx511, nx512, nx513, nx514, nx515, nx516, nx517, 
         nx518, nx519, nx520, nx521, nx522, nx523, nx524, nx525, nx526, nx527, 
         nx528, nx529, nx530, nx531, nx532, nx533, nx534, nx535, nx536, nx537, 
         nx538, state1, nx2, nx98, nx101, state1_dup_550, nx551, nx552, nx553, 
         nx546, nx4, state_assigned, nx559, nx560, nx568, state_credits_2, nx561, 
         nx34, nx42, nx46, state_credits_1, nx562, state_credits_0, nx569, nx570, 
         nx571, nx132, nx156, nx170, nx572, nx573, nx574, nx575, nx576, nx585, 
         nx589, nx591, nx593, nx595, nx598, nx602, nx604, nx607, nx609, nx611, 
         nx614, nx617, nx622, nx624, nx629, nx631, nx635, nx639, NOT_nx46, nx735, 
         nx638, nx640, state_assigned_dup_641, nx642, nx643, nx644, 
         state_credits_2__dup_645, nx646, nx647, nx648, nx649, 
         state_credits_1__dup_650, nx651, state_credits_0__dup_652, nx653, nx654, 
         nx655, nx656, nx657, nx658, nx659, nx660, nx661, nx662, nx663, nx664, 
         nx665, nx666, nx667, nx668, nx669, nx670, nx671, nx672, nx673, nx674, 
         nx675, nx676, nx677, nx678, nx679, nx680, nx681, nx682, nx683, nx684, 
         nx2683, buffers3_0, head_0, head_2, nx2714, head_1, nx18, nx745, nx60, 
         nx86, tail_0, nx88, tail_2, nx2716, tail_1, nx106, nx128, nx148, nx746, 
         nx747, nx748, buffers2_0, nx190, nx194, nx200, buffers0_0, nx206, nx749, 
         nx2718, nx2719, nx220, nx232, buffers1_0, nx238, nx242, nx252, nx264, 
         nx274, nx290, nx310, nx316, nx750, nx346, buffers3_1, buffers2_1, 
         buffers0_1, nx2720, nx2721, nx384, buffers1_1, nx402, nx751, nx464, 
         buffers3_2, buffers2_2, buffers0_2, nx752, buffers1_2, nx753, nx544, 
         nx558, nx754, nx596, buffers3_3, buffers2_3, buffers0_3, nx632, 
         buffers1_3, nx755, nx756, nx757, nx688, nx704, buffers3_4, buffers2_4, 
         buffers0_4, nx740, buffers1_4, nx758, nx766, nx780, nx796, nx812, 
         buffers3_5, buffers2_5, buffers0_5, nx848, buffers1_5, nx864, nx874, 
         nx888, nx904, nx920, buffers3_6, buffers2_6, buffers0_6, nx956, 
         buffers1_6, nx972, nx982, nx996, nx1012, nx1028, buffers3_7, buffers2_7, 
         buffers0_7, nx1064, buffers1_7, nx1080, nx1090, nx1104, nx1120, nx1136, 
         buffers3_8, buffers2_8, buffers0_8, nx1172, buffers1_8, nx1188, nx1198, 
         nx1212, nx1228, nx1244, buffers3_9, buffers2_9, buffers0_9, nx1280, 
         buffers1_9, nx1296, nx1306, nx1320, nx1336, nx1352, buffers3_10, 
         buffers2_10, buffers0_10, nx1388, buffers1_10, nx1404, nx1414, nx1428, 
         nx1444, nx1460, buffers3_11, buffers2_11, buffers0_11, nx1496, 
         buffers1_11, nx1512, nx1522, nx1536, nx1552, nx1568, nx1604, nx1620, 
         nx1632, nx1638, nx1642, nx1654, nx1666, nx1678, nx1690, nx1702, nx1714, 
         nx1726, nx1738, nx1750, nx1762, nx2748, nx2751, nx2755, nx2757, nx2761, 
         nx2763, nx2766, nx2772, nx2774, nx2776, nx2778, nx2782, nx2785, nx2787, 
         nx2789, nx2791, nx2793, nx2795, nx2801, nx2803, nx2807, nx2809, nx2815, 
         nx2817, nx2819, nx2821, nx2824, nx2827, nx2829, nx2831, nx2833, nx2839, 
         nx2841, nx2848, nx2852, nx2854, nx2858, nx2860, nx2863, nx2866, nx2868, 
         nx2870, nx2873, nx2875, nx2881, nx2884, nx2886, nx2888, nx2891, nx2893, 
         nx2899, nx2902, nx2904, nx2906, nx2909, nx2911, nx2917, nx2920, nx2922, 
         nx2924, nx2927, nx2929, nx2935, nx2938, nx2940, nx2942, nx2945, nx2947, 
         nx2953, nx2956, nx2958, nx2960, nx2963, nx2965, nx2971, nx2974, nx2976, 
         nx2978, nx2981, nx2983, nx2989, nx2992, nx2994, nx2996, nx2999, nx3001, 
         nx3007, nx3010, nx3012, nx3014, nx3017, nx3019, nx3025, nx3028, nx3030, 
         nx3032, nx3035, nx3037, nx3044, nx3053, nx3055, nx3058, nx3061, nx3064, 
         nx3067, nx3070, nx3072, nx3074, nx3077, nx3079, nx3082, nx3089, nx3092, 
         nx3095, nx3098, nx3101, nx3104, nx3107, nx3114, nx3116, nx3120, nx3122, 
         nx3125, nx3127, nx3130, nx3132, nx3135, nx3137, nx3140, nx3142, nx3145, 
         nx3147, nx3150, nx3152, nx3155, nx3157, nx3160, nx3162, nx3165, nx3167, 
         nx3170, nx3172, nx3176, nx3178, nx3183, nx3185, nx3189, nx3191, nx3195, 
         nx3201, nx3205, nx3209, nx3213, nx3217, nx3221, nx3225, nx3229, nx3233, 
         NOT_nx186, NOT_nx194, NOT_nx242, NOT_nx210, nx3871, nx1137, 
         buffers3_0__dup_1138, head_0__dup_1139, head_2__dup_1140, nx1141, 
         head_1__dup_1142, nx1143, nx1144, nx1145, nx1146, tail_0__dup_1147, 
         nx1148, tail_2__dup_1149, nx1150, tail_1__dup_1151, nx1152, nx1153, 
         nx1154, nx1155, nx1156, nx1157, buffers2_0__dup_1158, nx1159, nx1160, 
         nx1161, buffers0_0__dup_1162, nx1163, nx1164, nx1165, nx1166, nx1167, 
         nx1168, buffers1_0__dup_1169, nx1170, nx1171, nx1173, nx1174, nx1175, 
         nx1176, nx1177, nx1178, nx1179, nx1180, buffers3_1__dup_1181, 
         buffers2_1__dup_1182, buffers0_1__dup_1183, nx1184, nx1185, nx1186, 
         buffers1_1__dup_1187, nx1189, nx1190, nx1191, buffers3_2__dup_1192, 
         buffers2_2__dup_1193, buffers0_2__dup_1194, nx1195, 
         buffers1_2__dup_1196, nx1197, nx1199, nx1200, nx1201, nx1202, 
         buffers3_3__dup_1203, buffers2_3__dup_1204, buffers0_3__dup_1205, 
         nx1206, buffers1_3__dup_1207, nx1208, nx1209, nx1210, nx1211, nx1213, 
         buffers3_4__dup_1214, buffers2_4__dup_1215, buffers0_4__dup_1216, 
         nx1217, buffers1_4__dup_1218, nx1219, nx1220, nx1221, nx1222, nx1223, 
         buffers3_5__dup_1224, buffers2_5__dup_1225, buffers0_5__dup_1226, 
         nx1227, buffers1_5__dup_1228, nx1229, nx1230, nx1231, nx1232, nx1233, 
         buffers3_6__dup_1234, buffers2_6__dup_1235, buffers0_6__dup_1236, 
         nx1237, buffers1_6__dup_1238, nx1239, nx1240, nx1241, nx1242, nx1243, 
         buffers3_7__dup_1244, buffers2_7__dup_1245, buffers0_7__dup_1246, 
         nx1247, buffers1_7__dup_1248, nx1249, nx1250, nx1251, nx1252, nx1253, 
         buffers3_8__dup_1254, buffers2_8__dup_1255, buffers0_8__dup_1256, 
         nx1257, buffers1_8__dup_1258, nx1259, nx1260, nx1261, nx1262, nx1263, 
         buffers3_9__dup_1264, buffers2_9__dup_1265, buffers0_9__dup_1266, 
         nx1267, buffers1_9__dup_1268, nx1269, nx1270, nx1271, nx1272, nx1273, 
         buffers3_10__dup_1274, buffers2_10__dup_1275, buffers0_10__dup_1276, 
         nx1277, buffers1_10__dup_1278, nx1279, nx1281, nx1282, nx1283, nx1284, 
         buffers3_11__dup_1285, buffers2_11__dup_1286, buffers0_11__dup_1287, 
         nx1288, buffers1_11__dup_1289, nx1290, nx1291, nx1292, nx1293, nx1294, 
         nx1295, nx1297, nx1298, nx1299, nx1300, nx1301, nx1302, nx1303, nx1304, 
         nx1305, nx1307, nx1308, nx1309, nx1310, nx1311, nx1312, nx1313, nx1314, 
         nx1315, nx1316, nx1317, nx1318, nx1319, nx1321, nx1322, nx1323, nx1324, 
         nx1325, nx1326, nx1327, nx1328, nx1329, nx1330, nx1331, nx1332, nx1333, 
         nx1334, nx1335, nx1337, nx1338, nx1339, nx1340, nx1341, nx1342, nx1343, 
         nx1344, nx1345, nx1346, nx1347, nx1348, nx1349, nx1350, nx1351, nx1353, 
         nx1354, nx1355, nx1356, nx1357, nx1358, nx1359, nx1360, nx1361, nx1362, 
         nx1363, nx1364, nx1365, nx1366, nx1367, nx1368, nx1369, nx1370, nx1371, 
         nx1372, nx1373, nx1374, nx1375, nx1376, nx1377, nx1378, nx1379, nx1380, 
         nx1381, nx1382, nx1383, nx1384, nx1385, nx1386, nx1387, nx1389, nx1390, 
         nx1391, nx1392, nx1393, nx1394, nx1395, nx1396, nx1397, nx1398, nx1399, 
         nx1400, nx1401, nx1402, nx1403, nx1405, nx1406, nx1407, nx1408, nx1409, 
         nx1410, nx1411, nx1412, nx1413, nx1415, nx1416, nx1417, nx1418, nx1419, 
         nx1420, nx1421, nx1422, nx1423, nx1424, nx1425, nx1426, nx1427, nx1429, 
         nx1430, nx1431, nx1432, nx1433, nx1434, nx1435, nx1436, nx1437, nx1438, 
         nx1439, nx1440, nx1441, nx1442, nx1443, nx1445, nx1446, nx1447, nx1448, 
         nx1449, nx1450, nx1451, nx1452, nx1453, nx1454, nx1455, nx1456, nx1457, 
         nx1458, nx1459, nx1461, nx1462, nx1463, nx1464, nx1465, nx1466, nx1467, 
         nx1468, nx1469, nx1470, nx1471, nx1472, nx1473, nx1474, nx1475, nx1476, 
         nx1477, nx1478, nx1479, nx1480, nx1481, nx1482, nx1483, int01, 
         int01_dup_1484, int01_dup_1485, int01_dup_1486, int01_dup_1487, 
         int01_dup_1488, int01_dup_1489, int01_dup_1490, int01_dup_1491, 
         int01_dup_1492, int01_dup_1493, int01_dup_1494, int01_dup_1495, 
         int01_dup_1496, int01_dup_1497, int01_dup_1498, int01_dup_1499, 
         int01_dup_1500, int01_dup_1501, int01_dup_1502, int01_dup_1503, 
         int01_dup_1504, int01_dup_1505, int01_dup_1506;



    TIELO_X1M_A12TS ix226 (.Y (nx225)) ;
    NAND2_X0P5A_A12TS ix5 (.Y (validflitin), .A (flitin_0[1]), .B (flitin_0[0])
                      ) ;
    NOR2B_X0P7M_A12TS ix9 (.Y (renable), .AN (headflit_1), .B (headflit_0)) ;
    NOR2B_X0P7M_A12TS ix13 (.Y (creditout_0), .AN (debitout_0), .B (reset)) ;
    TIELO_X1M_A12TS ix200 (.Y (nx199)) ;
    NAND2_X0P5A_A12TS ix249 (.Y (validflitin_dup_488), .A (flitin_1[1]), .B (
                      flitin_1[0])) ;
    NOR2B_X0P7M_A12TS ix250 (.Y (renable_dup_418), .AN (headflit_1__dup_267), .B (
                      headflit_0__dup_268)) ;
    NOR2B_X0P7M_A12TS ix251 (.Y (creditout_1), .AN (debitout_1), .B (reset)) ;
    TIELO_X1M_A12TS ix252 (.Y (nx273)) ;
    AND2_X0P5M_A12TS ix105 (.Y (nx104), .A (nxt_resp1_0), .B (nx80)) ;
    NOR2_X0P5A_A12TS ix81 (.Y (nx80), .A (tailout_1), .B (reset)) ;
    AND2_X0P5M_A12TS ix85 (.Y (nx84), .A (nxt_resp1_1), .B (nx80)) ;
    OAI31_X0P5M_A12TS ix103 (.Y (nx284), .A0 (nx294), .A1 (swalloc_resp_1_1), .A2 (
                      swalloc_resp_1_0), .B0 (nx80)) ;
    NOR2_X0P5A_A12TS ix295 (.Y (nx294), .A (swalloc_req_1_1), .B (
                     swalloc_req_1_0)) ;
    AND2_X0P5M_A12TS ix69 (.Y (nx68), .A (nxt_resp0_0), .B (nx44)) ;
    NOR2_X0P5A_A12TS ix45 (.Y (nx44), .A (tailout_0), .B (reset)) ;
    AND2_X0P5M_A12TS ix49 (.Y (nx48), .A (nxt_resp0_1), .B (nx44)) ;
    OAI31_X0P5M_A12TS ix67 (.Y (nx283), .A0 (nx300), .A1 (swalloc_resp_0_1), .A2 (
                      swalloc_resp_0_0), .B0 (nx44)) ;
    NOR2_X0P5A_A12TS ix301 (.Y (nx300), .A (swalloc_req_0_1), .B (
                     swalloc_req_0_0)) ;
    TIELO_X1M_A12TS ix256 (.Y (GND)) ;
    AND2_X0P5M_A12TS ix133 (.Y (request0_0), .A (elig_0), .B (swalloc_req_0_0)
                     ) ;
    AND2_X0P5M_A12TS ix135 (.Y (request0_1), .A (elig_1), .B (swalloc_req_0_1)
                     ) ;
    AND2_X0P5M_A12TS ix129 (.Y (request1_0), .A (swalloc_req_1_0), .B (elig_0)
                     ) ;
    AND2_X0P5M_A12TS ix131 (.Y (request1_1), .A (swalloc_req_1_1), .B (elig_1)
                     ) ;
    AO1B2_X0P5M_A12TS ix39 (.Y (debit_outvc0), .A0N (nx308), .B0 (debitout_0), .B1 (
                      state_invc0_0)) ;
    AO1B2_X0P5M_A12TS ix19 (.Y (debit_outvc1), .A0N (nx313), .B0 (state_invc1_0)
                      , .B1 (debitout_0)) ;
    NAND3_X0P5A_A12TS ix314 (.Y (nx313), .A (nx315), .B (state_invc1_1), .C (
                      debitout_1)) ;
    INV_X0P5B_A12TS ix316 (.Y (nx315), .A (state_invc1_0)) ;
    OAI31_X0P5M_A12TS ix29 (.Y (tail_outvc0), .A0 (state_invc0_0), .A1 (nx318), 
                      .A2 (nx320), .B0 (nx322)) ;
    INV_X0P5B_A12TS ix319 (.Y (nx318), .A (tailout_1)) ;
    INV_X0P5B_A12TS ix321 (.Y (nx320), .A (state_invc0_1)) ;
    NAND2_X0P5A_A12TS ix323 (.Y (nx322), .A (tailout_0), .B (state_invc0_0)) ;
    AO1B2_X0P5M_A12TS ix274 (.Y (tail_outvc1), .A0N (nx325), .B0 (state_invc1_0)
                      , .B1 (tailout_0)) ;
    NAND3_X0P5A_A12TS ix326 (.Y (nx325), .A (tailout_1), .B (state_invc1_1), .C (
                      nx315)) ;
    OAI21_X0P5M_A12TS ix119 (.Y (nomorebufs1), .A0 (state_invc0_1), .A1 (nx328)
                      , .B0 (nx330)) ;
    INV_X0P5B_A12TS ix329 (.Y (nx328), .A (no_bufs1)) ;
    NAND2_X0P5A_A12TS ix331 (.Y (nx330), .A (state_invc0_1), .B (no_bufs0)) ;
    OAI21_X0P5M_A12TS ix127 (.Y (nomorebufs0), .A0 (state_invc0_0), .A1 (nx328)
                      , .B0 (nx333)) ;
    NAND2_X0P5A_A12TS ix334 (.Y (nx333), .A (state_invc0_0), .B (no_bufs0)) ;
    NAND3B_X0P5M_A12TS ix309 (.Y (nx308), .AN (state_invc0_0), .B (debitout_1), 
                       .C (state_invc0_1)) ;
    TIELO_X1M_A12TS ix371 (.Y (nx370)) ;
    SDFFSRPQ_X0P5M_A12TS reg_resp1_0 (.Q (swalloc_resp_1_0), .CK (clk), .D (
                         nx104), .R (GND), .SE (NOT_nx284), .SI (
                         swalloc_resp_1_0), .SN (nx433)) ;
    INV_X0P5B_A12TS ix432 (.Y (NOT_nx284), .A (nx284)) ;
    INV_X0P5B_A12TS ix434 (.Y (nx433), .A (GND)) ;
    SDFFSRPQ_X0P5M_A12TS reg_resp1_1 (.Q (swalloc_resp_1_1), .CK (clk), .D (nx84
                         ), .R (GND), .SE (NOT_nx284), .SI (swalloc_resp_1_1), .SN (
                         nx433)) ;
    SDFFSRPQ_X0P5M_A12TS reg_resp0_0 (.Q (swalloc_resp_0_0), .CK (clk), .D (nx68
                         ), .R (GND), .SE (NOT_nx283), .SI (swalloc_resp_0_0), .SN (
                         nx433)) ;
    INV_X0P5B_A12TS ix438 (.Y (NOT_nx283), .A (nx283)) ;
    SDFFSRPQ_X0P5M_A12TS reg_resp0_1 (.Q (swalloc_resp_0_1), .CK (clk), .D (nx48
                         ), .R (GND), .SE (NOT_nx283), .SI (swalloc_resp_0_1), .SN (
                         nx433)) ;
    NOR2_X0P5A_A12TS ix277 (.Y (colsel0_1), .A (swalloc_resp_0_0), .B (
                     swalloc_resp_1_0)) ;
    NAND2B_X0P7M_A12TS ix3 (.Y (colsel0_0), .AN (swalloc_resp_1_0), .B (
                       swalloc_resp_0_0)) ;
    NOR2_X0P5A_A12TS ix278 (.Y (colsel1_1), .A (swalloc_resp_0_1), .B (
                     swalloc_resp_1_1)) ;
    NAND2B_X0P7M_A12TS ix279 (.Y (colsel1_0), .AN (swalloc_resp_1_1), .B (
                       swalloc_resp_0_1)) ;
    TIEHI_X1M_A12TS ix144 (.Y (PWR)) ;
    TIELO_X1M_A12TS ix142 (.Y (GND_dup_287)) ;
    DFFQ_X0P5M_A12TS reg_colsel0reg_0 (.Q (colsel0reg_0), .CK (clk), .D (nx16)
                     ) ;
    OR2_X0P5M_A12TS ix17 (.Y (nx16), .A (reset), .B (colsel0_0)) ;
    DFFQ_X0P5M_A12TS reg_colsel0reg_1 (.Q (colsel0reg_1), .CK (clk), .D (nx24)
                     ) ;
    OR2_X0P5M_A12TS ix25 (.Y (nx24), .A (reset), .B (colsel0_1)) ;
    DFFQ_X0P5M_A12TS reg_colsel1reg_0 (.Q (colsel1reg_0), .CK (clk), .D (nx0)) ;
    OR2_X0P5M_A12TS ix1 (.Y (nx0), .A (reset), .B (colsel1_0)) ;
    DFFQ_X0P5M_A12TS reg_colsel1reg_1 (.Q (colsel1reg_1), .CK (clk), .D (nx8)) ;
    OR2_X0P5M_A12TS ix280 (.Y (nx8), .A (colsel1_1), .B (reset)) ;
    NAND2_X0P5A_A12TS ix288 (.Y (wenable), .A (flitin_0[1]), .B (flitin_0[0])) ;
    AND2_X0P5M_A12TS ix33 (.Y (wdata_2), .A (flitin_0[2]), .B (wenable)) ;
    AND2_X0P5M_A12TS ix35 (.Y (wdata_3), .A (flitin_0[3]), .B (wenable)) ;
    AND2_X0P5M_A12TS ix37 (.Y (wdata_4), .A (flitin_0[4]), .B (wenable)) ;
    AND2_X0P5M_A12TS ix289 (.Y (wdata_5), .A (flitin_0[5]), .B (wenable)) ;
    AND2_X0P5M_A12TS ix41 (.Y (wdata_6), .A (flitin_0[6]), .B (wenable)) ;
    AND2_X0P5M_A12TS ix43 (.Y (wdata_7), .A (flitin_0[7]), .B (wenable)) ;
    AND2_X0P5M_A12TS ix290 (.Y (wdata_8), .A (flitin_0[8]), .B (wenable)) ;
    AND2_X0P5M_A12TS ix47 (.Y (wdata_9), .A (flitin_0[9]), .B (wenable)) ;
    AND2_X0P5M_A12TS ix291 (.Y (wdata_10), .A (flitin_0[10]), .B (wenable)) ;
    AND2_X0P5M_A12TS ix51 (.Y (wdata_11), .A (flitin_0[11]), .B (wenable)) ;
    NAND2B_X0P7M_A12TS ix27 (.Y (flitout_switch_0_0), .AN (rdata_0), .B (
                       dqenablereg)) ;
    DFFQ_X0P5M_A12TS reg_dqenablereg (.Q (dqenablereg), .CK (clk), .D (nx305)) ;
    NOR2B_X0P7M_A12TS ix292 (.Y (nx305), .AN (debitout_0), .B (reset)) ;
    NAND2B_X0P7M_A12TS ix31 (.Y (flitout_switch_0_1), .AN (rdata_1), .B (
                       dqenablereg)) ;
    AND2_X0P5M_A12TS ix53 (.Y (flitout_switch_0_2), .A (rdata_2), .B (
                     dqenablereg)) ;
    AND2_X0P5M_A12TS ix55 (.Y (flitout_switch_0_3), .A (rdata_3), .B (
                     dqenablereg)) ;
    AND2_X0P5M_A12TS ix57 (.Y (flitout_switch_0_4), .A (rdata_4), .B (
                     dqenablereg)) ;
    AND2_X0P5M_A12TS ix59 (.Y (flitout_switch_0_5), .A (rdata_5), .B (
                     dqenablereg)) ;
    AND2_X0P5M_A12TS ix61 (.Y (flitout_switch_0_6), .A (rdata_6), .B (
                     dqenablereg)) ;
    AND2_X0P5M_A12TS ix63 (.Y (flitout_switch_0_7), .A (rdata_7), .B (
                     dqenablereg)) ;
    AND2_X0P5M_A12TS ix65 (.Y (flitout_switch_0_8), .A (rdata_8), .B (
                     dqenablereg)) ;
    AND2_X0P5M_A12TS ix293 (.Y (flitout_switch_0_9), .A (rdata_9), .B (
                     dqenablereg)) ;
    AND2_X0P5M_A12TS ix294 (.Y (flitout_switch_0_10), .A (rdata_10), .B (
                     dqenablereg)) ;
    AND2_X0P5M_A12TS ix71 (.Y (flitout_switch_0_11), .A (rdata_11), .B (
                     dqenablereg)) ;
    TIELO_X1M_A12TS ix337 (.Y (nx336)) ;
    DFFQ_X0P5M_A12TS reg_vc_req_0 (.Q (routewire_0), .CK (clk), .D (nx66)) ;
    NOR3_X0P5A_A12TS ix306 (.Y (nx66), .A (nx58), .B (reset), .C (nx180)) ;
    INV_X0P5B_A12TS ix181 (.Y (nx180), .A (renable)) ;
    DFFQ_X0P5M_A12TS reg_vc_req_1 (.Q (routewire_1), .CK (clk), .D (nx78)) ;
    NOR3_X0P5A_A12TS ix79 (.Y (nx78), .A (nx184), .B (reset), .C (nx180)) ;
    CGENI_X1M_A12TS ix185 (.CON (nx184), .A (nx186), .B (headflit_7), .CI (nx321
                    )) ;
    INV_X0P5B_A12TS ix187 (.Y (nx186), .A (currx[2])) ;
    CGENI_X1M_A12TS ix307 (.CON (nx321), .A (currx[1]), .B (nx189), .CI (nx191)
                    ) ;
    INV_X0P5B_A12TS ix190 (.Y (nx189), .A (headflit_6)) ;
    CGENI_X1M_A12TS ix192 (.CON (nx191), .A (nx193), .B (headflit_5), .CI (nx28)
                    ) ;
    INV_X0P5B_A12TS ix194 (.Y (nx193), .A (currx[0])) ;
    CGENI_X1M_A12TS ix308 (.CON (nx28), .A (nx196), .B (headflit_4), .CI (nx198)
                    ) ;
    INV_X0P5B_A12TS ix197 (.Y (nx196), .A (curry[2])) ;
    INV_X0P5B_A12TS ix310 (.Y (nx58), .A (nx184)) ;
    CGENI_X1M_A12TS ix21 (.CON (nx198), .A (curry[1]), .B (nx210), .CI (nx12)) ;
    INV_X0P5B_A12TS ix209 (.Y (nx210), .A (headflit_3)) ;
    NOR2B_X0P7M_A12TS ix311 (.Y (nx12), .AN (curry[0]), .B (headflit_2)) ;
    NOR2_X0P5A_A12TS ix970 (.Y (nx969), .A (state_queuelen_0), .B (
                     state_queuelen_1)) ;
    DFFQ_X0P5M_A12TS reg_state_queuelen_0 (.Q (state_queuelen_0), .CK (clk), .D (
                     nx124)) ;
    NOR2_X0P5A_A12TS ix125 (.Y (nx124), .A (reset), .B (nx973)) ;
    XOR2_X0P5M_A12TS ix974 (.Y (nx973), .A (state_queuelen_0), .B (nx975)) ;
    AOI31_X0P5M_A12TS ix976 (.Y (nx975), .A0 (debitout_0), .A1 (nx977), .A2 (
                      nx979), .B0 (nx960)) ;
    INV_X0P5B_A12TS ix978 (.Y (nx977), .A (validflitin)) ;
    INV_X0P5B_A12TS ix980 (.Y (nx979), .A (full)) ;
    DFFQ_X0P5M_A12TS reg_full (.Q (full), .CK (clk), .D (nx188)) ;
    OA21A1OI2_X0P5M_A12TS ix189 (.Y (nx188), .A0 (nx979), .A1 (debitout_0), .B0 (
                          nx983), .C0 (reset)) ;
    AO21A1AI2_X0P5M_A12TS ix984 (.Y (nx983), .A0 (nx985), .A1 (nx992), .B0 (full
                          ), .C0 (validflitin)) ;
    OAI31_X0P5M_A12TS ix986 (.Y (nx985), .A0 (state_queuelen_0), .A1 (
                      state_queuelen_1), .A2 (state_queuelen_2), .B0 (nx118)) ;
    DFFQ_X0P5M_A12TS reg_state_queuelen_1 (.Q (state_queuelen_1), .CK (clk), .D (
                     nx150)) ;
    OA21A1OI2_X0P5M_A12TS ix151 (.Y (nx150), .A0 (nx975), .A1 (nx989), .B0 (
                          nx1013), .C0 (reset)) ;
    XOR2_X0P5M_A12TS ix990 (.Y (nx989), .A (nx960), .B (nx1011)) ;
    NOR3_X0P5A_A12TS ix169 (.Y (nx960), .A (nx992), .B (nx977), .C (debitout_0)
                     ) ;
    NOR2_X0P5A_A12TS ix993 (.Y (nx992), .A (nx994), .B (state_queuelen_2)) ;
    NAND2_X0P5A_A12TS ix995 (.Y (nx994), .A (state_queuelen_1), .B (
                      state_queuelen_0)) ;
    DFFQ_X0P5M_A12TS reg_state_queuelen_2 (.Q (state_queuelen_2), .CK (clk), .D (
                     nx246)) ;
    OA21A1OI2_X0P5M_A12TS ix247 (.Y (nx246), .A0 (nx975), .A1 (nx998), .B0 (
                          nx1009), .C0 (reset)) ;
    AOI31_X0P5M_A12TS ix1000 (.Y (nx998), .A0 (nx1001), .A1 (nx1004), .A2 (nx969
                      ), .B0 (nx230)) ;
    INV_X0P5B_A12TS ix1005 (.Y (nx1004), .A (state_queuelen_2)) ;
    OA21A1OI2_X0P5M_A12TS ix231 (.Y (nx230), .A0 (nx969), .A1 (nx960), .B0 (
                          nx1007), .C0 (nx1004)) ;
    NAND2_X0P5A_A12TS ix1008 (.Y (nx1007), .A (nx994), .B (nx960)) ;
    NAND2_X0P5A_A12TS ix1010 (.Y (nx1009), .A (state_queuelen_2), .B (nx975)) ;
    AOI21_X0P5M_A12TS ix1012 (.Y (nx1011), .A0 (state_queuelen_1), .A1 (
                      state_queuelen_0), .B0 (nx969)) ;
    NAND2_X0P5A_A12TS ix1014 (.Y (nx1013), .A (state_queuelen_1), .B (nx975)) ;
    OAI31_X0P5M_A12TS ix322 (.Y (nx118), .A0 (state_status_0), .A1 (tailout_0), 
                      .A2 (nx1021), .B0 (nx1035)) ;
    DFFQ_X0P5M_A12TS reg_state_status_0 (.Q (state_status_0), .CK (clk), .D (
                     nx958)) ;
    NOR2_X0P5A_A12TS ix1019 (.Y (nx1018), .A (tailout_0), .B (reset)) ;
    OAI21_X0P5M_A12TS ix324 (.Y (nx40), .A0 (swalloc_resp_0_1), .A1 (
                      swalloc_resp_0_0), .B0 (nx1021)) ;
    INV_X0P5B_A12TS ix1022 (.Y (nx1021), .A (state_status_1)) ;
    DFFQ_X0P5M_A12TS reg_state_status_1 (.Q (state_status_1), .CK (clk), .D (
                     nx26)) ;
    OAI211_X0P5M_A12TS ix325 (.Y (nx26), .A0 (nx1025), .A1 (nx958), .B0 (nx1027)
                       , .C0 (nx1018)) ;
    INV_X0P5B_A12TS ix1026 (.Y (nx1025), .A (state_status_0)) ;
    AO21A1AI2_X0P5M_A12TS ix1028 (.Y (nx1027), .A0 (nx1029), .A1 (nx1031), .B0 (
                          nx1033), .C0 (state_status_1)) ;
    INV_X0P5B_A12TS ix1030 (.Y (nx1029), .A (routewire_1)) ;
    INV_X0P5B_A12TS ix1032 (.Y (nx1031), .A (routewire_0)) ;
    AOI211_X0P5M_A12TS ix1034 (.Y (nx1033), .A0 (state_status_0), .A1 (nx40), .B0 (
                       tailout_0), .C0 (reset)) ;
    OAI211_X0P5M_A12TS ix1036 (.Y (nx1035), .A0 (swalloc_resp_0_0), .A1 (
                       swalloc_resp_0_1), .B0 (nx1021), .C0 (state_status_0)) ;
    DFFQ_X0P5M_A12TS reg_state_swreq_0 (.Q (swalloc_req_0_0), .CK (clk), .D (
                     nx74)) ;
    INV_X0P5B_A12TS ix75 (.Y (nx74), .A (nx1041)) ;
    AOI32_X0P5M_A12TS ix1042 (.Y (nx1041), .A0 (nx334), .A1 (routewire_0), .A2 (
                      state_status_1), .B0 (swalloc_req_0_0), .B1 (nx70)) ;
    NOR3_X0P5A_A12TS ix327 (.Y (nx334), .A (nx1033), .B (tailout_0), .C (reset)
                     ) ;
    OAI21_X0P5M_A12TS ix328 (.Y (nx70), .A0 (routewire_1), .A1 (nx1045), .B0 (
                      nx1047)) ;
    AOI22_X0P5M_A12TS ix1048 (.Y (nx1047), .A0 (nx1025), .A1 (nx1018), .B0 (
                      nx1021), .B1 (nx334)) ;
    DFFQ_X0P5M_A12TS reg_state_swreq_1 (.Q (swalloc_req_0_1), .CK (clk), .D (
                     nx94)) ;
    INV_X0P5B_A12TS ix95 (.Y (nx94), .A (nx1051)) ;
    AOI32_X0P5M_A12TS ix1052 (.Y (nx1051), .A0 (nx334), .A1 (routewire_1), .A2 (
                      state_status_1), .B0 (swalloc_req_0_1), .B1 (nx90)) ;
    OAI21_X0P5M_A12TS ix91 (.Y (nx90), .A0 (routewire_0), .A1 (nx1045), .B0 (
                      nx1047)) ;
    INV_X0P5B_A12TS ix1002 (.Y (nx1001), .A (nx960)) ;
    INV_X0P5B_A12TS ix1046 (.Y (nx1045), .A (nx334)) ;
    INV_X0P5B_A12TS ix330 (.Y (nx958), .A (nx1033)) ;
    INV_X0P5B_A12TS ix257 (.Y (debitout_0), .A (nx985)) ;
    NAND2_X0P5A_A12TS ix335 (.Y (wenable_dup_397), .A (flitin_1[1]), .B (
                      flitin_1[0])) ;
    AND2_X0P5M_A12TS ix336 (.Y (wdata_2__dup_396), .A (flitin_1[2]), .B (
                     wenable_dup_397)) ;
    AND2_X0P5M_A12TS ix338 (.Y (wdata_3__dup_395), .A (flitin_1[3]), .B (
                     wenable_dup_397)) ;
    AND2_X0P5M_A12TS ix339 (.Y (wdata_4__dup_394), .A (flitin_1[4]), .B (
                     wenable_dup_397)) ;
    AND2_X0P5M_A12TS ix340 (.Y (wdata_5__dup_393), .A (flitin_1[5]), .B (
                     wenable_dup_397)) ;
    AND2_X0P5M_A12TS ix341 (.Y (wdata_6__dup_392), .A (flitin_1[6]), .B (
                     wenable_dup_397)) ;
    AND2_X0P5M_A12TS ix342 (.Y (wdata_7__dup_391), .A (flitin_1[7]), .B (
                     wenable_dup_397)) ;
    AND2_X0P5M_A12TS ix343 (.Y (wdata_8__dup_390), .A (flitin_1[8]), .B (
                     wenable_dup_397)) ;
    AND2_X0P5M_A12TS ix344 (.Y (wdata_9__dup_389), .A (flitin_1[9]), .B (
                     wenable_dup_397)) ;
    AND2_X0P5M_A12TS ix345 (.Y (wdata_10__dup_388), .A (flitin_1[10]), .B (
                     wenable_dup_397)) ;
    AND2_X0P5M_A12TS ix346 (.Y (wdata_11__dup_387), .A (flitin_1[11]), .B (
                     wenable_dup_397)) ;
    NAND2B_X0P7M_A12TS ix347 (.Y (flitout_switch_1_0), .AN (rdata_0__dup_386), .B (
                       dqenablereg_dup_398)) ;
    DFFQ_X0P5M_A12TS reg_dqenablereg_dup_4686 (.Q (dqenablereg_dup_398), .CK (
                     clk), .D (nx399)) ;
    NOR2B_X0P7M_A12TS ix348 (.Y (nx399), .AN (debitout_1), .B (reset)) ;
    NAND2B_X0P7M_A12TS ix349 (.Y (flitout_switch_1_1), .AN (rdata_1__dup_385), .B (
                       dqenablereg_dup_398)) ;
    AND2_X0P5M_A12TS ix350 (.Y (flitout_switch_1_2), .A (rdata_2__dup_384), .B (
                     dqenablereg_dup_398)) ;
    AND2_X0P5M_A12TS ix351 (.Y (flitout_switch_1_3), .A (rdata_3__dup_383), .B (
                     dqenablereg_dup_398)) ;
    AND2_X0P5M_A12TS ix352 (.Y (flitout_switch_1_4), .A (rdata_4__dup_382), .B (
                     dqenablereg_dup_398)) ;
    AND2_X0P5M_A12TS ix353 (.Y (flitout_switch_1_5), .A (rdata_5__dup_381), .B (
                     dqenablereg_dup_398)) ;
    AND2_X0P5M_A12TS ix354 (.Y (flitout_switch_1_6), .A (rdata_6__dup_380), .B (
                     dqenablereg_dup_398)) ;
    AND2_X0P5M_A12TS ix355 (.Y (flitout_switch_1_7), .A (rdata_7__dup_379), .B (
                     dqenablereg_dup_398)) ;
    AND2_X0P5M_A12TS ix356 (.Y (flitout_switch_1_8), .A (rdata_8__dup_378), .B (
                     dqenablereg_dup_398)) ;
    AND2_X0P5M_A12TS ix357 (.Y (flitout_switch_1_9), .A (rdata_9__dup_377), .B (
                     dqenablereg_dup_398)) ;
    AND2_X0P5M_A12TS ix358 (.Y (flitout_switch_1_10), .A (rdata_10__dup_376), .B (
                     dqenablereg_dup_398)) ;
    AND2_X0P5M_A12TS ix359 (.Y (flitout_switch_1_11), .A (rdata_11__dup_375), .B (
                     dqenablereg_dup_398)) ;
    TIELO_X1M_A12TS ix360 (.Y (nx400)) ;
    DFFQ_X0P5M_A12TS reg_vc_req_0__dup_4700 (.Q (routewire_0__dup_270), .CK (clk
                     ), .D (nx429)) ;
    NOR3_X0P5A_A12TS ix401 (.Y (nx429), .A (nx428), .B (reset), .C (nx431)) ;
    INV_X0P5B_A12TS ix402 (.Y (nx431), .A (renable_dup_418)) ;
    DFFQ_X0P5M_A12TS reg_vc_req_1__dup_4703 (.Q (routewire_1__dup_269), .CK (clk
                     ), .D (nx430)) ;
    NOR3_X0P5A_A12TS ix403 (.Y (nx430), .A (nx432), .B (reset), .C (nx431)) ;
    CGENI_X1M_A12TS ix404 (.CON (nx432), .A (nx434), .B (headflit_7__dup_261), .CI (
                    nx427)) ;
    INV_X0P5B_A12TS ix405 (.Y (nx434), .A (currx[2])) ;
    CGENI_X1M_A12TS ix406 (.CON (nx427), .A (currx[1]), .B (nx435), .CI (nx436)
                    ) ;
    INV_X0P5B_A12TS ix407 (.Y (nx435), .A (headflit_6__dup_262)) ;
    CGENI_X1M_A12TS ix408 (.CON (nx436), .A (nx437), .B (headflit_5__dup_263), .CI (
                    nx426)) ;
    INV_X0P5B_A12TS ix409 (.Y (nx437), .A (currx[0])) ;
    CGENI_X1M_A12TS ix410 (.CON (nx426), .A (nx438), .B (headflit_4__dup_264), .CI (
                    nx439)) ;
    INV_X0P5B_A12TS ix411 (.Y (nx438), .A (curry[2])) ;
    INV_X0P5B_A12TS ix412 (.Y (nx428), .A (nx432)) ;
    CGENI_X1M_A12TS ix413 (.CON (nx439), .A (curry[1]), .B (nx440), .CI (nx425)
                    ) ;
    INV_X0P5B_A12TS ix414 (.Y (nx440), .A (headflit_3__dup_265)) ;
    NOR2B_X0P7M_A12TS ix415 (.Y (nx425), .AN (curry[0]), .B (headflit_2__dup_266
                      )) ;
    NOR2_X0P5A_A12TS ix441 (.Y (nx510), .A (state_queuelen_0__dup_503), .B (
                     state_queuelen_1__dup_501)) ;
    DFFQ_X0P5M_A12TS reg_state_queuelen_0__dup_4718 (.Q (
                     state_queuelen_0__dup_503), .CK (clk), .D (nx504)) ;
    NOR2_X0P5A_A12TS ix442 (.Y (nx504), .A (reset), .B (nx511)) ;
    XOR2_X0P5M_A12TS ix443 (.Y (nx511), .A (state_queuelen_0__dup_503), .B (
                     nx512)) ;
    AOI31_X0P5M_A12TS ix444 (.Y (nx512), .A0 (debitout_1), .A1 (nx513), .A2 (
                      nx514), .B0 (nx502)) ;
    INV_X0P5B_A12TS ix445 (.Y (nx513), .A (validflitin_dup_488)) ;
    INV_X0P5B_A12TS ix446 (.Y (nx514), .A (full_dup_506)) ;
    DFFQ_X0P5M_A12TS reg_full_dup_4724 (.Q (full_dup_506), .CK (clk), .D (nx507)
                     ) ;
    OA21A1OI2_X0P5M_A12TS ix447 (.Y (nx507), .A0 (nx514), .A1 (debitout_1), .B0 (
                          nx515), .C0 (reset)) ;
    AO21A1AI2_X0P5M_A12TS ix448 (.Y (nx515), .A0 (nx516), .A1 (nx518), .B0 (
                          full_dup_506), .C0 (validflitin_dup_488)) ;
    OAI31_X0P5M_A12TS ix449 (.Y (nx516), .A0 (state_queuelen_0__dup_503), .A1 (
                      state_queuelen_1__dup_501), .A2 (state_queuelen_2__dup_500
                      ), .B0 (nx499)) ;
    DFFQ_X0P5M_A12TS reg_state_queuelen_1__dup_4728 (.Q (
                     state_queuelen_1__dup_501), .CK (clk), .D (nx505)) ;
    OA21A1OI2_X0P5M_A12TS ix450 (.Y (nx505), .A0 (nx512), .A1 (nx517), .B0 (
                          nx526), .C0 (reset)) ;
    XOR2_X0P5M_A12TS ix451 (.Y (nx517), .A (nx502), .B (nx525)) ;
    NOR3_X0P5A_A12TS ix452 (.Y (nx502), .A (nx518), .B (nx513), .C (debitout_1)
                     ) ;
    NOR2_X0P5A_A12TS ix453 (.Y (nx518), .A (nx519), .B (
                     state_queuelen_2__dup_500)) ;
    NAND2_X0P5A_A12TS ix454 (.Y (nx519), .A (state_queuelen_1__dup_501), .B (
                      state_queuelen_0__dup_503)) ;
    DFFQ_X0P5M_A12TS reg_state_queuelen_2__dup_4734 (.Q (
                     state_queuelen_2__dup_500), .CK (clk), .D (nx509)) ;
    OA21A1OI2_X0P5M_A12TS ix455 (.Y (nx509), .A0 (nx512), .A1 (nx520), .B0 (
                          nx524), .C0 (reset)) ;
    AOI31_X0P5M_A12TS ix456 (.Y (nx520), .A0 (nx521), .A1 (nx522), .A2 (nx510), 
                      .B0 (nx508)) ;
    INV_X0P5B_A12TS ix457 (.Y (nx522), .A (state_queuelen_2__dup_500)) ;
    OA21A1OI2_X0P5M_A12TS ix458 (.Y (nx508), .A0 (nx510), .A1 (nx502), .B0 (
                          nx523), .C0 (nx522)) ;
    NAND2_X0P5A_A12TS ix459 (.Y (nx523), .A (nx519), .B (nx502)) ;
    NAND2_X0P5A_A12TS ix460 (.Y (nx524), .A (state_queuelen_2__dup_500), .B (
                      nx512)) ;
    AOI21_X0P5M_A12TS ix461 (.Y (nx525), .A0 (state_queuelen_1__dup_501), .A1 (
                      state_queuelen_0__dup_503), .B0 (nx510)) ;
    NAND2_X0P5A_A12TS ix462 (.Y (nx526), .A (state_queuelen_1__dup_501), .B (
                      nx512)) ;
    OAI31_X0P5M_A12TS ix463 (.Y (nx499), .A0 (state_status_0__dup_489), .A1 (
                      tailout_1), .A2 (nx528), .B0 (nx534)) ;
    DFFQ_X0P5M_A12TS reg_state_status_0__dup_4744 (.Q (state_status_0__dup_489)
                     , .CK (clk), .D (nx490)) ;
    NOR2_X0P5A_A12TS ix464 (.Y (nx527), .A (tailout_1), .B (reset)) ;
    OAI21_X0P5M_A12TS ix465 (.Y (nx493), .A0 (swalloc_resp_1_1), .A1 (
                      swalloc_resp_1_0), .B0 (nx528)) ;
    INV_X0P5B_A12TS ix466 (.Y (nx528), .A (state_status_1__dup_491)) ;
    DFFQ_X0P5M_A12TS reg_state_status_1__dup_4748 (.Q (state_status_1__dup_491)
                     , .CK (clk), .D (nx492)) ;
    OAI211_X0P5M_A12TS ix467 (.Y (nx492), .A0 (nx529), .A1 (nx490), .B0 (nx530)
                       , .C0 (nx527)) ;
    INV_X0P5B_A12TS ix468 (.Y (nx529), .A (state_status_0__dup_489)) ;
    AO21A1AI2_X0P5M_A12TS ix469 (.Y (nx530), .A0 (nx531), .A1 (nx532), .B0 (
                          nx533), .C0 (state_status_1__dup_491)) ;
    INV_X0P5B_A12TS ix470 (.Y (nx531), .A (routewire_1__dup_269)) ;
    INV_X0P5B_A12TS ix471 (.Y (nx532), .A (routewire_0__dup_270)) ;
    AOI211_X0P5M_A12TS ix472 (.Y (nx533), .A0 (state_status_0__dup_489), .A1 (
                       nx493), .B0 (tailout_1), .C0 (reset)) ;
    OAI211_X0P5M_A12TS ix473 (.Y (nx534), .A0 (swalloc_resp_1_0), .A1 (
                       swalloc_resp_1_1), .B0 (nx528), .C0 (
                       state_status_0__dup_489)) ;
    DFFQ_X0P5M_A12TS reg_state_swreq_0__dup_4756 (.Q (swalloc_req_1_0), .CK (clk
                     ), .D (nx496)) ;
    INV_X0P5B_A12TS ix474 (.Y (nx496), .A (nx535)) ;
    AOI32_X0P5M_A12TS ix475 (.Y (nx535), .A0 (nx494), .A1 (routewire_0__dup_270)
                      , .A2 (state_status_1__dup_491), .B0 (swalloc_req_1_0), .B1 (
                      nx495)) ;
    NOR3_X0P5A_A12TS ix476 (.Y (nx494), .A (nx533), .B (tailout_1), .C (reset)
                     ) ;
    OAI21_X0P5M_A12TS ix477 (.Y (nx495), .A0 (routewire_1__dup_269), .A1 (nx536)
                      , .B0 (nx537)) ;
    AOI22_X0P5M_A12TS ix478 (.Y (nx537), .A0 (nx529), .A1 (nx527), .B0 (nx528), 
                      .B1 (nx494)) ;
    DFFQ_X0P5M_A12TS reg_state_swreq_1__dup_4762 (.Q (swalloc_req_1_1), .CK (clk
                     ), .D (nx498)) ;
    INV_X0P5B_A12TS ix479 (.Y (nx498), .A (nx538)) ;
    AOI32_X0P5M_A12TS ix480 (.Y (nx538), .A0 (nx494), .A1 (routewire_1__dup_269)
                      , .A2 (state_status_1__dup_491), .B0 (swalloc_req_1_1), .B1 (
                      nx497)) ;
    OAI21_X0P5M_A12TS ix481 (.Y (nx497), .A0 (routewire_0__dup_270), .A1 (nx536)
                      , .B0 (nx537)) ;
    INV_X0P5B_A12TS ix482 (.Y (nx521), .A (nx502)) ;
    INV_X0P5B_A12TS ix483 (.Y (nx536), .A (nx494)) ;
    INV_X0P5B_A12TS ix484 (.Y (nx490), .A (nx533)) ;
    INV_X0P5B_A12TS ix485 (.Y (debitout_1), .A (nx516)) ;
    INV_X0P5B_A12TS ix99 (.Y (nx98), .A (request0_0)) ;
    INV_X0P5B_A12TS ix15 (.Y (nxt_resp1_0), .A (nx101)) ;
    OAI21_X0P5M_A12TS ix102 (.Y (nx101), .A0 (state1), .A1 (nx98), .B0 (
                      request1_0)) ;
    DFFQ_X0P5M_A12TS reg_state1 (.Q (state1), .CK (clk), .D (nx2)) ;
    NOR2B_X0P7M_A12TS ix539 (.Y (nx2), .AN (GND), .B (reset)) ;
    AND2_X0P5M_A12TS ix540 (.Y (nxt_resp0_0), .A (request0_0), .B (nx101)) ;
    INV_X0P5B_A12TS ix543 (.Y (nx552), .A (request0_1)) ;
    INV_X0P5B_A12TS ix544 (.Y (nxt_resp1_1), .A (nx553)) ;
    OAI21_X0P5M_A12TS ix545 (.Y (nx553), .A0 (state1_dup_550), .A1 (nx552), .B0 (
                      request1_1)) ;
    DFFQ_X0P5M_A12TS reg_state1_dup_4779 (.Q (state1_dup_550), .CK (clk), .D (
                     nx551)) ;
    NOR2B_X0P7M_A12TS ix546 (.Y (nx551), .AN (GND), .B (reset)) ;
    AND2_X0P5M_A12TS ix547 (.Y (nxt_resp0_1), .A (request0_1), .B (nx553)) ;
    TIEHI_X1M_A12TS ix566 (.Y (nx46)) ;
    OAI31_X0P5M_A12TS ix554 (.Y (nx132), .A0 (nx574), .A1 (reset), .A2 (nx589), 
                      .B0 (nx593)) ;
    NAND3B_X0P5M_A12TS ix570 (.Y (nx574), .AN (no_bufs0), .B (nx576), .C (
                       creditin_0)) ;
    DFFQ_X0P5M_A12TS reg_no_bufs (.Q (no_bufs0), .CK (clk), .D (nx560)) ;
    INV_X0P5B_A12TS ix574 (.Y (nx575), .A (no_bufs0)) ;
    NOR3_X0P5A_A12TS ix576 (.Y (nx576), .A (nxt_resp0_0), .B (nxt_resp1_0), .C (
                     debit_outvc0)) ;
    INV_X0P5B_A12TS ix586 (.Y (nx585), .A (state_credits_1)) ;
    XOR2_X0P5M_A12TS ix590 (.Y (nx589), .A (state_credits_2), .B (nx591)) ;
    NAND2_X0P5A_A12TS ix592 (.Y (nx591), .A (state_credits_1), .B (
                      state_credits_0)) ;
    AO21A1AI2_X0P5M_A12TS ix594 (.Y (nx593), .A0 (nx562), .A1 (nx595), .B0 (
                          nx598), .C0 (state_credits_2)) ;
    NOR3_X0P5A_A12TS ix599 (.Y (nx598), .A (nx34), .B (reset), .C (nx561)) ;
    AOI211_X0P5M_A12TS ix147 (.Y (nx561), .A0 (nx602), .A1 (nx604), .B0 (
                       creditin_0), .C0 (nx576)) ;
    NOR2_X0P5A_A12TS ix603 (.Y (nx602), .A (state_credits_0), .B (
                     state_credits_1)) ;
    INV_X0P5B_A12TS ix605 (.Y (nx604), .A (state_credits_2)) ;
    OAI211_X0P5M_A12TS ix555 (.Y (nx571), .A0 (nx585), .A1 (nx607), .B0 (nx609)
                       , .C0 (nx611)) ;
    NOR2_X0P5A_A12TS ix608 (.Y (nx607), .A (nx598), .B (reset)) ;
    INV_X0P5B_A12TS ix610 (.Y (nx609), .A (reset)) ;
    MXIT2_X0P5M_A12TS ix612 (.Y (nx611), .A (nx595), .B (nx570), .S0 (nx614)) ;
    NOR2_X0P5A_A12TS ix556 (.Y (nx570), .A (nx574), .B (reset)) ;
    AOI21_X0P5M_A12TS ix615 (.Y (nx614), .A0 (state_credits_1), .A1 (
                      state_credits_0), .B0 (nx602)) ;
    OAI211_X0P5M_A12TS ix557 (.Y (nx569), .A0 (state_credits_0), .A1 (nx42), .B0 (
                       nx617), .C0 (nx609)) ;
    NAND2_X0P5A_A12TS ix618 (.Y (nx617), .A (state_credits_0), .B (nx42)) ;
    TIELO_X1M_A12TS ix558 (.Y (nx546)) ;
    DFFQ_X0P5M_A12TS reg_state_invc_0 (.Q (state_invc0_0), .CK (clk), .D (nx572)
                     ) ;
    INV_X0P5B_A12TS ix559 (.Y (nx572), .A (nx622)) ;
    AO21A1AI2_X0P5M_A12TS ix623 (.Y (nx622), .A0 (nx624), .A1 (state_invc0_0), .B0 (
                          nxt_resp0_0), .C0 (nx4)) ;
    INV_X0P5B_A12TS ix625 (.Y (nx624), .A (nxt_resp1_0)) ;
    NOR2_X0P5A_A12TS ix560 (.Y (nx4), .A (tail_outvc0), .B (reset)) ;
    DFFQ_X0P5M_A12TS reg_state_invc_1 (.Q (state_invc0_1), .CK (clk), .D (nx573)
                     ) ;
    INV_X0P5B_A12TS ix199 (.Y (nx573), .A (nx629)) ;
    AO21A1AI2_X0P5M_A12TS ix630 (.Y (nx629), .A0 (nx631), .A1 (state_invc0_1), .B0 (
                          nxt_resp1_0), .C0 (nx4)) ;
    INV_X0P5B_A12TS ix632 (.Y (nx631), .A (nxt_resp0_0)) ;
    DFFQ_X0P5M_A12TS reg_eligible (.Q (elig_0), .CK (clk), .D (nx170)) ;
    AND2_X0P5M_A12TS ix171 (.Y (nx170), .A (nx635), .B (nx639)) ;
    OAI31_X0P5M_A12TS ix636 (.Y (nx635), .A0 (nxt_resp0_0), .A1 (nxt_resp1_0), .A2 (
                      state_assigned), .B0 (nx4)) ;
    DFFQ_X0P5M_A12TS reg_state_assigned (.Q (state_assigned), .CK (clk), .D (
                     nx559)) ;
    INV_X0P5B_A12TS ix561 (.Y (nx559), .A (nx635)) ;
    AO21A1AI2_X0P5M_A12TS ix640 (.Y (nx639), .A0 (no_bufs0), .A1 (nx568), .B0 (
                          nx156), .C0 (nx609)) ;
    OA21A1OI2_X0P5M_A12TS ix157 (.Y (nx156), .A0 (nx576), .A1 (nx561), .B0 (
                          nx575), .C0 (creditin_0)) ;
    INV_X0P5B_A12TS ix562 (.Y (nx562), .A (nx602)) ;
    INV_X0P5B_A12TS ix563 (.Y (nx42), .A (nx607)) ;
    INV_X0P5B_A12TS ix564 (.Y (nx34), .A (nx574)) ;
    INV_X0P5B_A12TS ix565 (.Y (nx568), .A (nx576)) ;
    INV_X0P5B_A12TS ix165 (.Y (nx560), .A (nx639)) ;
    AND2_X0P5M_A12TS ix596 (.Y (nx595), .A (nx607), .B (nx574)) ;
    SDFFSRPQ_X0P5M_A12TS reg_state_credits_0 (.Q (state_credits_0), .CK (clk), .D (
                         nx569), .R (nx546), .SE (NOT_nx46), .SI (
                         state_credits_0), .SN (nx735)) ;
    INV_X0P5B_A12TS ix734 (.Y (NOT_nx46), .A (nx46)) ;
    INV_X0P5B_A12TS ix736 (.Y (nx735), .A (nx546)) ;
    SDFFSRPQ_X0P5M_A12TS reg_state_credits_1 (.Q (state_credits_1), .CK (clk), .D (
                         nx571), .R (nx546), .SE (NOT_nx46), .SI (
                         state_credits_1), .SN (nx735)) ;
    SDFFSRPQ_X0P5M_A12TS reg_state_credits_2 (.Q (state_credits_2), .CK (clk), .D (
                         nx132), .R (nx546), .SE (NOT_nx46), .SI (
                         state_credits_2), .SN (nx735)) ;
    TIEHI_X1M_A12TS ix577 (.Y (nx649)) ;
    OAI31_X0P5M_A12TS ix578 (.Y (nx656), .A0 (nx661), .A1 (reset), .A2 (nx665), 
                      .B0 (nx667)) ;
    NAND3B_X0P5M_A12TS ix579 (.Y (nx661), .AN (no_bufs1), .B (nx663), .C (
                       creditin_1)) ;
    DFFQ_X0P5M_A12TS reg_no_bufs_dup_4835 (.Q (no_bufs1), .CK (clk), .D (nx643)
                     ) ;
    INV_X0P5B_A12TS ix580 (.Y (nx662), .A (no_bufs1)) ;
    NOR3_X0P5A_A12TS ix581 (.Y (nx663), .A (nxt_resp0_1), .B (nxt_resp1_1), .C (
                     debit_outvc1)) ;
    INV_X0P5B_A12TS ix582 (.Y (nx664), .A (state_credits_1__dup_650)) ;
    XOR2_X0P5M_A12TS ix583 (.Y (nx665), .A (state_credits_2__dup_645), .B (nx666
                     )) ;
    NAND2_X0P5A_A12TS ix584 (.Y (nx666), .A (state_credits_1__dup_650), .B (
                      state_credits_0__dup_652)) ;
    AO21A1AI2_X0P5M_A12TS ix585 (.Y (nx667), .A0 (nx651), .A1 (nx668), .B0 (
                          nx669), .C0 (state_credits_2__dup_645)) ;
    NOR3_X0P5A_A12TS ix587 (.Y (nx669), .A (nx647), .B (reset), .C (nx646)) ;
    AOI211_X0P5M_A12TS ix588 (.Y (nx646), .A0 (nx670), .A1 (nx671), .B0 (
                       creditin_1), .C0 (nx663)) ;
    NOR2_X0P5A_A12TS ix589 (.Y (nx670), .A (state_credits_0__dup_652), .B (
                     state_credits_1__dup_650)) ;
    INV_X0P5B_A12TS ix591 (.Y (nx671), .A (state_credits_2__dup_645)) ;
    OAI211_X0P5M_A12TS ix593 (.Y (nx655), .A0 (nx664), .A1 (nx672), .B0 (nx673)
                       , .C0 (nx674)) ;
    NOR2_X0P5A_A12TS ix595 (.Y (nx672), .A (nx669), .B (reset)) ;
    INV_X0P5B_A12TS ix597 (.Y (nx673), .A (reset)) ;
    MXIT2_X0P5M_A12TS ix598 (.Y (nx674), .A (nx668), .B (nx654), .S0 (nx675)) ;
    NOR2_X0P5A_A12TS ix600 (.Y (nx654), .A (nx661), .B (reset)) ;
    AOI21_X0P5M_A12TS ix601 (.Y (nx675), .A0 (state_credits_1__dup_650), .A1 (
                      state_credits_0__dup_652), .B0 (nx670)) ;
    OAI211_X0P5M_A12TS ix602 (.Y (nx653), .A0 (state_credits_0__dup_652), .A1 (
                       nx648), .B0 (nx676), .C0 (nx673)) ;
    NAND2_X0P5A_A12TS ix604 (.Y (nx676), .A (state_credits_0__dup_652), .B (
                      nx648)) ;
    TIELO_X1M_A12TS ix606 (.Y (nx638)) ;
    DFFQ_X0P5M_A12TS reg_state_invc_0__dup_4855 (.Q (state_invc1_0), .CK (clk), 
                     .D (nx659)) ;
    INV_X0P5B_A12TS ix607 (.Y (nx659), .A (nx677)) ;
    AO21A1AI2_X0P5M_A12TS ix609 (.Y (nx677), .A0 (nx678), .A1 (state_invc1_0), .B0 (
                          nxt_resp0_1), .C0 (nx640)) ;
    INV_X0P5B_A12TS ix611 (.Y (nx678), .A (nxt_resp1_1)) ;
    NOR2_X0P5A_A12TS ix613 (.Y (nx640), .A (tail_outvc1), .B (reset)) ;
    DFFQ_X0P5M_A12TS reg_state_invc_1__dup_4860 (.Q (state_invc1_1), .CK (clk), 
                     .D (nx660)) ;
    INV_X0P5B_A12TS ix614 (.Y (nx660), .A (nx679)) ;
    AO21A1AI2_X0P5M_A12TS ix616 (.Y (nx679), .A0 (nx680), .A1 (state_invc1_1), .B0 (
                          nxt_resp1_1), .C0 (nx640)) ;
    INV_X0P5B_A12TS ix617 (.Y (nx680), .A (nxt_resp0_1)) ;
    DFFQ_X0P5M_A12TS reg_eligible_dup_4864 (.Q (elig_1), .CK (clk), .D (nx658)
                     ) ;
    AND2_X0P5M_A12TS ix619 (.Y (nx658), .A (nx681), .B (nx682)) ;
    OAI31_X0P5M_A12TS ix620 (.Y (nx681), .A0 (nxt_resp0_1), .A1 (nxt_resp1_1), .A2 (
                      state_assigned_dup_641), .B0 (nx640)) ;
    DFFQ_X0P5M_A12TS reg_state_assigned_dup_4867 (.Q (state_assigned_dup_641), .CK (
                     clk), .D (nx642)) ;
    INV_X0P5B_A12TS ix621 (.Y (nx642), .A (nx681)) ;
    AO21A1AI2_X0P5M_A12TS ix622 (.Y (nx682), .A0 (no_bufs1), .A1 (nx644), .B0 (
                          nx657), .C0 (nx673)) ;
    OA21A1OI2_X0P5M_A12TS ix624 (.Y (nx657), .A0 (nx663), .A1 (nx646), .B0 (
                          nx662), .C0 (creditin_1)) ;
    INV_X0P5B_A12TS ix626 (.Y (nx651), .A (nx670)) ;
    INV_X0P5B_A12TS ix627 (.Y (nx648), .A (nx672)) ;
    INV_X0P5B_A12TS ix628 (.Y (nx647), .A (nx661)) ;
    INV_X0P5B_A12TS ix629 (.Y (nx644), .A (nx663)) ;
    INV_X0P5B_A12TS ix631 (.Y (nx643), .A (nx682)) ;
    AND2_X0P5M_A12TS ix633 (.Y (nx668), .A (nx672), .B (nx661)) ;
    SDFFSRPQ_X0P5M_A12TS reg_state_credits_0__dup_4877 (.Q (
                         state_credits_0__dup_652), .CK (clk), .D (nx653), .R (
                         nx638), .SE (nx683), .SI (state_credits_0__dup_652), .SN (
                         nx684)) ;
    INV_X0P5B_A12TS ix634 (.Y (nx683), .A (nx649)) ;
    INV_X0P5B_A12TS ix635 (.Y (nx684), .A (nx638)) ;
    SDFFSRPQ_X0P5M_A12TS reg_state_credits_1__dup_4880 (.Q (
                         state_credits_1__dup_650), .CK (clk), .D (nx655), .R (
                         nx638), .SE (nx683), .SI (state_credits_1__dup_650), .SN (
                         nx684)) ;
    SDFFSRPQ_X0P5M_A12TS reg_state_credits_2__dup_4881 (.Q (
                         state_credits_2__dup_645), .CK (clk), .D (nx656), .R (
                         nx638), .SE (nx683), .SI (state_credits_2__dup_645), .SN (
                         nx684)) ;
    OAI22_X0P5M_A12TS ix1569 (.Y (nx1568), .A0 (nx2748), .A1 (nx747), .B0 (
                      nx2821), .B1 (nx2841)) ;
    AOI221_X0P5M_A12TS ix2749 (.Y (nx2748), .A0 (buffers3_11), .A1 (nx274), .B0 (
                       buffers2_11), .B1 (nx200), .C0 (nx1522)) ;
    NOR2_X0P5A_A12TS ix275 (.Y (nx274), .A (debitout_0), .B (nx2751)) ;
    AOI21_X0P5M_A12TS ix2752 (.Y (nx2751), .A0 (head_1), .A1 (head_0), .B0 (
                      head_2)) ;
    DFFQ_X0P5M_A12TS reg_head_1 (.Q (head_1), .CK (clk), .D (nx18)) ;
    OA21A1OI2_X0P5M_A12TS ix685 (.Y (nx18), .A0 (debitout_0), .A1 (nx2755), .B0 (
                          nx2757), .C0 (nx2714)) ;
    INV_X0P5B_A12TS ix2756 (.Y (nx2755), .A (head_1)) ;
    OAI211_X0P5M_A12TS ix2758 (.Y (nx2757), .A0 (head_0), .A1 (head_1), .B0 (
                       debitout_0), .C0 (nx2763)) ;
    DFFQ_X0P5M_A12TS reg_head_0 (.Q (head_0), .CK (clk), .D (nx60)) ;
    NOR2_X0P5A_A12TS ix686 (.Y (nx60), .A (nx2761), .B (reset)) ;
    XNOR2_X0P5M_A12TS ix2762 (.Y (nx2761), .A (head_0), .B (debitout_0)) ;
    NAND2_X0P5A_A12TS ix2764 (.Y (nx2763), .A (head_1), .B (head_0)) ;
    NAND2_X0P5A_A12TS ix687 (.Y (nx2714), .A (nx2766), .B (nx2778)) ;
    DFFQ_X0P5M_A12TS reg_head_2 (.Q (head_2), .CK (clk), .D (nx745)) ;
    OA21A1OI2_X0P5M_A12TS ix688 (.Y (nx745), .A0 (nx2772), .A1 (nx2774), .B0 (
                          nx2776), .C0 (nx2714)) ;
    INV_X0P5B_A12TS ix2773 (.Y (nx2772), .A (debitout_0)) ;
    XOR2_X0P5M_A12TS ix2775 (.Y (nx2774), .A (head_2), .B (nx2763)) ;
    NAND2_X0P5A_A12TS ix2777 (.Y (nx2776), .A (nx2772), .B (head_2)) ;
    INV_X0P5B_A12TS ix2779 (.Y (nx2778), .A (reset)) ;
    INV_X0P5B_A12TS ix2783 (.Y (nx2782), .A (head_0)) ;
    OAI22_X0P5M_A12TS ix1523 (.Y (nx1522), .A0 (nx2785), .A1 (nx2787), .B0 (
                      nx2791), .B1 (nx2793)) ;
    INV_X0P5B_A12TS ix2786 (.Y (nx2785), .A (buffers1_11)) ;
    NAND2_X0P5A_A12TS ix2788 (.Y (nx2787), .A (nx2772), .B (nx2789)) ;
    NOR3_X0P5A_A12TS ix2790 (.Y (nx2789), .A (head_1), .B (head_2), .C (nx2782)
                     ) ;
    INV_X0P5B_A12TS ix2792 (.Y (nx2791), .A (buffers0_11)) ;
    NAND2_X0P5A_A12TS ix2794 (.Y (nx2793), .A (nx2772), .B (nx2795)) ;
    NOR3_X0P5A_A12TS ix2796 (.Y (nx2795), .A (head_1), .B (head_2), .C (head_0)
                     ) ;
    NAND2_X0P5A_A12TS ix689 (.Y (nx747), .A (nx746), .B (nx2778)) ;
    AOI21_X0P5M_A12TS ix690 (.Y (nx746), .A0 (tail_1), .A1 (tail_0), .B0 (tail_2
                      )) ;
    DFFQ_X0P5M_A12TS reg_tail_1 (.Q (tail_1), .CK (clk), .D (nx106)) ;
    OA21A1OI2_X0P5M_A12TS ix107 (.Y (nx106), .A0 (wenable), .A1 (nx2801), .B0 (
                          nx2803), .C0 (nx2716)) ;
    INV_X0P5B_A12TS ix2802 (.Y (nx2801), .A (tail_1)) ;
    OAI211_X0P5M_A12TS ix2804 (.Y (nx2803), .A0 (tail_0), .A1 (tail_1), .B0 (
                       wenable), .C0 (nx2809)) ;
    DFFQ_X0P5M_A12TS reg_tail_0 (.Q (tail_0), .CK (clk), .D (nx148)) ;
    NOR2_X0P5A_A12TS ix149 (.Y (nx148), .A (nx2807), .B (reset)) ;
    XNOR2_X0P5M_A12TS ix2808 (.Y (nx2807), .A (tail_0), .B (wenable)) ;
    NAND2_X0P5A_A12TS ix2810 (.Y (nx2809), .A (tail_1), .B (tail_0)) ;
    OAI31_X0P5M_A12TS ix145 (.Y (nx2716), .A0 (nx88), .A1 (tail_2), .A2 (nx2809)
                      , .B0 (nx2778)) ;
    DFFQ_X0P5M_A12TS reg_tail_2 (.Q (tail_2), .CK (clk), .D (nx128)) ;
    OA21A1OI2_X0P5M_A12TS ix691 (.Y (nx128), .A0 (nx2815), .A1 (nx2817), .B0 (
                          nx2819), .C0 (nx2716)) ;
    INV_X0P5B_A12TS ix2816 (.Y (nx2815), .A (wenable)) ;
    XOR2_X0P5M_A12TS ix2818 (.Y (nx2817), .A (tail_2), .B (nx2809)) ;
    NAND2_X0P5A_A12TS ix2820 (.Y (nx2819), .A (nx2815), .B (tail_2)) ;
    AOI221_X0P5M_A12TS ix2822 (.Y (nx2821), .A0 (wdata_11), .A1 (wenable), .B0 (
                       buffers3_11), .B1 (nx750), .C0 (nx1552)) ;
    AOI21_X0P5M_A12TS ix692 (.Y (nx750), .A0 (nx2809), .A1 (nx2824), .B0 (
                      wenable)) ;
    INV_X0P5B_A12TS ix2825 (.Y (nx2824), .A (tail_2)) ;
    OAI21_X0P5M_A12TS ix1553 (.Y (nx1552), .A0 (nx2827), .A1 (nx2829), .B0 (
                      nx2833)) ;
    INV_X0P5B_A12TS ix2828 (.Y (nx2827), .A (buffers2_11)) ;
    NAND2_X0P5A_A12TS ix2830 (.Y (nx2829), .A (nx2831), .B (nx2807)) ;
    NOR3_X0P5A_A12TS ix2832 (.Y (nx2831), .A (nx2801), .B (tail_2), .C (tail_0)
                     ) ;
    AOI22_X0P5M_A12TS ix2834 (.Y (nx2833), .A0 (buffers1_11), .A1 (nx316), .B0 (
                      buffers0_11), .B1 (nx310)) ;
    INV_X0P5B_A12TS ix2840 (.Y (nx2839), .A (tail_0)) ;
    OAI22_X0P5M_A12TS ix1537 (.Y (nx1536), .A0 (nx2748), .A1 (nx190), .B0 (
                      nx2821), .B1 (nx2848)) ;
    NAND2_X0P5A_A12TS ix2849 (.Y (nx2848), .A (nx2778), .B (nx2831)) ;
    OAI22_X0P5M_A12TS ix1513 (.Y (nx1512), .A0 (nx2748), .A1 (nx238), .B0 (
                      nx2821), .B1 (nx2852)) ;
    NAND2_X0P5A_A12TS ix2853 (.Y (nx2852), .A (nx2778), .B (nx2854)) ;
    NOR3_X0P5A_A12TS ix2855 (.Y (nx2854), .A (tail_1), .B (tail_2), .C (nx2839)
                     ) ;
    OAI22_X0P5M_A12TS ix1497 (.Y (nx1496), .A0 (nx2748), .A1 (nx206), .B0 (
                      nx2821), .B1 (nx2858)) ;
    NAND2_X0P5A_A12TS ix2859 (.Y (nx2858), .A (nx2778), .B (nx2860)) ;
    NOR3_X0P5A_A12TS ix2861 (.Y (nx2860), .A (tail_1), .B (tail_2), .C (tail_0)
                     ) ;
    OAI22_X0P5M_A12TS ix1461 (.Y (nx1460), .A0 (nx2863), .A1 (nx747), .B0 (
                      nx2870), .B1 (nx2841)) ;
    AOI221_X0P5M_A12TS ix2864 (.Y (nx2863), .A0 (buffers3_10), .A1 (nx274), .B0 (
                       buffers2_10), .B1 (nx200), .C0 (nx1414)) ;
    OAI22_X0P5M_A12TS ix1415 (.Y (nx1414), .A0 (nx2866), .A1 (nx2787), .B0 (
                      nx2868), .B1 (nx2793)) ;
    INV_X0P5B_A12TS ix2867 (.Y (nx2866), .A (buffers1_10)) ;
    INV_X0P5B_A12TS ix2869 (.Y (nx2868), .A (buffers0_10)) ;
    AOI221_X0P5M_A12TS ix2871 (.Y (nx2870), .A0 (wdata_10), .A1 (wenable), .B0 (
                       buffers3_10), .B1 (nx750), .C0 (nx1444)) ;
    OAI21_X0P5M_A12TS ix1445 (.Y (nx1444), .A0 (nx2873), .A1 (nx2829), .B0 (
                      nx2875)) ;
    INV_X0P5B_A12TS ix2874 (.Y (nx2873), .A (buffers2_10)) ;
    AOI22_X0P5M_A12TS ix2876 (.Y (nx2875), .A0 (buffers1_10), .A1 (nx316), .B0 (
                      buffers0_10), .B1 (nx310)) ;
    OAI22_X0P5M_A12TS ix1429 (.Y (nx1428), .A0 (nx2863), .A1 (nx190), .B0 (
                      nx2870), .B1 (nx2848)) ;
    OAI22_X0P5M_A12TS ix1405 (.Y (nx1404), .A0 (nx2863), .A1 (nx238), .B0 (
                      nx2870), .B1 (nx2852)) ;
    OAI22_X0P5M_A12TS ix1389 (.Y (nx1388), .A0 (nx2863), .A1 (nx206), .B0 (
                      nx2870), .B1 (nx2858)) ;
    OAI22_X0P5M_A12TS ix1353 (.Y (nx1352), .A0 (nx2881), .A1 (nx747), .B0 (
                      nx2888), .B1 (nx2841)) ;
    AOI221_X0P5M_A12TS ix2882 (.Y (nx2881), .A0 (buffers3_9), .A1 (nx274), .B0 (
                       buffers2_9), .B1 (nx200), .C0 (nx1306)) ;
    OAI22_X0P5M_A12TS ix1307 (.Y (nx1306), .A0 (nx2884), .A1 (nx2787), .B0 (
                      nx2886), .B1 (nx2793)) ;
    INV_X0P5B_A12TS ix2885 (.Y (nx2884), .A (buffers1_9)) ;
    INV_X0P5B_A12TS ix2887 (.Y (nx2886), .A (buffers0_9)) ;
    AOI221_X0P5M_A12TS ix2889 (.Y (nx2888), .A0 (wdata_9), .A1 (wenable), .B0 (
                       buffers3_9), .B1 (nx750), .C0 (nx1336)) ;
    OAI21_X0P5M_A12TS ix1337 (.Y (nx1336), .A0 (nx2891), .A1 (nx2829), .B0 (
                      nx2893)) ;
    INV_X0P5B_A12TS ix2892 (.Y (nx2891), .A (buffers2_9)) ;
    AOI22_X0P5M_A12TS ix2894 (.Y (nx2893), .A0 (buffers1_9), .A1 (nx316), .B0 (
                      buffers0_9), .B1 (nx310)) ;
    OAI22_X0P5M_A12TS ix1321 (.Y (nx1320), .A0 (nx2881), .A1 (nx190), .B0 (
                      nx2888), .B1 (nx2848)) ;
    OAI22_X0P5M_A12TS ix1297 (.Y (nx1296), .A0 (nx2881), .A1 (nx238), .B0 (
                      nx2888), .B1 (nx2852)) ;
    OAI22_X0P5M_A12TS ix1281 (.Y (nx1280), .A0 (nx2881), .A1 (nx206), .B0 (
                      nx2888), .B1 (nx2858)) ;
    OAI22_X0P5M_A12TS ix1245 (.Y (nx1244), .A0 (nx2899), .A1 (nx747), .B0 (
                      nx2906), .B1 (nx2841)) ;
    AOI221_X0P5M_A12TS ix2900 (.Y (nx2899), .A0 (buffers3_8), .A1 (nx274), .B0 (
                       buffers2_8), .B1 (nx200), .C0 (nx1198)) ;
    OAI22_X0P5M_A12TS ix1199 (.Y (nx1198), .A0 (nx2902), .A1 (nx2787), .B0 (
                      nx2904), .B1 (nx2793)) ;
    INV_X0P5B_A12TS ix2903 (.Y (nx2902), .A (buffers1_8)) ;
    INV_X0P5B_A12TS ix2905 (.Y (nx2904), .A (buffers0_8)) ;
    AOI221_X0P5M_A12TS ix2907 (.Y (nx2906), .A0 (wdata_8), .A1 (wenable), .B0 (
                       buffers3_8), .B1 (nx750), .C0 (nx1228)) ;
    OAI21_X0P5M_A12TS ix1229 (.Y (nx1228), .A0 (nx2909), .A1 (nx2829), .B0 (
                      nx2911)) ;
    INV_X0P5B_A12TS ix2910 (.Y (nx2909), .A (buffers2_8)) ;
    AOI22_X0P5M_A12TS ix2912 (.Y (nx2911), .A0 (buffers1_8), .A1 (nx316), .B0 (
                      buffers0_8), .B1 (nx310)) ;
    OAI22_X0P5M_A12TS ix1213 (.Y (nx1212), .A0 (nx2899), .A1 (nx190), .B0 (
                      nx2906), .B1 (nx2848)) ;
    OAI22_X0P5M_A12TS ix1189 (.Y (nx1188), .A0 (nx2899), .A1 (nx238), .B0 (
                      nx2906), .B1 (nx2852)) ;
    OAI22_X0P5M_A12TS ix1173 (.Y (nx1172), .A0 (nx2899), .A1 (nx206), .B0 (
                      nx2906), .B1 (nx2858)) ;
    OAI22_X0P5M_A12TS ix1137 (.Y (nx1136), .A0 (nx2917), .A1 (nx747), .B0 (
                      nx2924), .B1 (nx2841)) ;
    AOI221_X0P5M_A12TS ix2918 (.Y (nx2917), .A0 (buffers3_7), .A1 (nx274), .B0 (
                       buffers2_7), .B1 (nx200), .C0 (nx1090)) ;
    OAI22_X0P5M_A12TS ix1091 (.Y (nx1090), .A0 (nx2920), .A1 (nx2787), .B0 (
                      nx2922), .B1 (nx2793)) ;
    INV_X0P5B_A12TS ix2921 (.Y (nx2920), .A (buffers1_7)) ;
    INV_X0P5B_A12TS ix2923 (.Y (nx2922), .A (buffers0_7)) ;
    AOI221_X0P5M_A12TS ix2925 (.Y (nx2924), .A0 (wdata_7), .A1 (wenable), .B0 (
                       buffers3_7), .B1 (nx750), .C0 (nx1120)) ;
    OAI21_X0P5M_A12TS ix1121 (.Y (nx1120), .A0 (nx2927), .A1 (nx2829), .B0 (
                      nx2929)) ;
    INV_X0P5B_A12TS ix2928 (.Y (nx2927), .A (buffers2_7)) ;
    AOI22_X0P5M_A12TS ix2930 (.Y (nx2929), .A0 (buffers1_7), .A1 (nx316), .B0 (
                      buffers0_7), .B1 (nx310)) ;
    OAI22_X0P5M_A12TS ix1105 (.Y (nx1104), .A0 (nx2917), .A1 (nx190), .B0 (
                      nx2924), .B1 (nx2848)) ;
    OAI22_X0P5M_A12TS ix1081 (.Y (nx1080), .A0 (nx2917), .A1 (nx238), .B0 (
                      nx2924), .B1 (nx2852)) ;
    OAI22_X0P5M_A12TS ix1065 (.Y (nx1064), .A0 (nx2917), .A1 (nx206), .B0 (
                      nx2924), .B1 (nx2858)) ;
    OAI22_X0P5M_A12TS ix1029 (.Y (nx1028), .A0 (nx2935), .A1 (nx747), .B0 (
                      nx2942), .B1 (nx2841)) ;
    AOI221_X0P5M_A12TS ix2936 (.Y (nx2935), .A0 (buffers3_6), .A1 (nx274), .B0 (
                       buffers2_6), .B1 (nx200), .C0 (nx982)) ;
    OAI22_X0P5M_A12TS ix983 (.Y (nx982), .A0 (nx2938), .A1 (nx2787), .B0 (nx2940
                      ), .B1 (nx2793)) ;
    INV_X0P5B_A12TS ix2939 (.Y (nx2938), .A (buffers1_6)) ;
    INV_X0P5B_A12TS ix2941 (.Y (nx2940), .A (buffers0_6)) ;
    AOI221_X0P5M_A12TS ix2943 (.Y (nx2942), .A0 (wdata_6), .A1 (wenable), .B0 (
                       buffers3_6), .B1 (nx750), .C0 (nx1012)) ;
    OAI21_X0P5M_A12TS ix1013 (.Y (nx1012), .A0 (nx2945), .A1 (nx2829), .B0 (
                      nx2947)) ;
    INV_X0P5B_A12TS ix2946 (.Y (nx2945), .A (buffers2_6)) ;
    AOI22_X0P5M_A12TS ix2948 (.Y (nx2947), .A0 (buffers1_6), .A1 (nx316), .B0 (
                      buffers0_6), .B1 (nx310)) ;
    OAI22_X0P5M_A12TS ix997 (.Y (nx996), .A0 (nx2935), .A1 (nx190), .B0 (nx2942)
                      , .B1 (nx2848)) ;
    OAI22_X0P5M_A12TS ix973 (.Y (nx972), .A0 (nx2935), .A1 (nx238), .B0 (nx2942)
                      , .B1 (nx2852)) ;
    OAI22_X0P5M_A12TS ix957 (.Y (nx956), .A0 (nx2935), .A1 (nx206), .B0 (nx2942)
                      , .B1 (nx2858)) ;
    OAI22_X0P5M_A12TS ix921 (.Y (nx920), .A0 (nx2953), .A1 (nx747), .B0 (nx2960)
                      , .B1 (nx2841)) ;
    AOI221_X0P5M_A12TS ix2954 (.Y (nx2953), .A0 (buffers3_5), .A1 (nx274), .B0 (
                       buffers2_5), .B1 (nx200), .C0 (nx874)) ;
    OAI22_X0P5M_A12TS ix875 (.Y (nx874), .A0 (nx2956), .A1 (nx2787), .B0 (nx2958
                      ), .B1 (nx2793)) ;
    INV_X0P5B_A12TS ix2957 (.Y (nx2956), .A (buffers1_5)) ;
    INV_X0P5B_A12TS ix2959 (.Y (nx2958), .A (buffers0_5)) ;
    AOI221_X0P5M_A12TS ix2961 (.Y (nx2960), .A0 (wdata_5), .A1 (wenable), .B0 (
                       buffers3_5), .B1 (nx750), .C0 (nx904)) ;
    OAI21_X0P5M_A12TS ix905 (.Y (nx904), .A0 (nx2963), .A1 (nx2829), .B0 (nx2965
                      )) ;
    INV_X0P5B_A12TS ix2964 (.Y (nx2963), .A (buffers2_5)) ;
    AOI22_X0P5M_A12TS ix2966 (.Y (nx2965), .A0 (buffers1_5), .A1 (nx316), .B0 (
                      buffers0_5), .B1 (nx310)) ;
    OAI22_X0P5M_A12TS ix889 (.Y (nx888), .A0 (nx2953), .A1 (nx190), .B0 (nx2960)
                      , .B1 (nx2848)) ;
    OAI22_X0P5M_A12TS ix865 (.Y (nx864), .A0 (nx2953), .A1 (nx238), .B0 (nx2960)
                      , .B1 (nx2852)) ;
    OAI22_X0P5M_A12TS ix849 (.Y (nx848), .A0 (nx2953), .A1 (nx206), .B0 (nx2960)
                      , .B1 (nx2858)) ;
    OAI22_X0P5M_A12TS ix813 (.Y (nx812), .A0 (nx2971), .A1 (nx747), .B0 (nx2978)
                      , .B1 (nx2841)) ;
    AOI221_X0P5M_A12TS ix2972 (.Y (nx2971), .A0 (buffers3_4), .A1 (nx274), .B0 (
                       buffers2_4), .B1 (nx200), .C0 (nx766)) ;
    OAI22_X0P5M_A12TS ix767 (.Y (nx766), .A0 (nx2974), .A1 (nx2787), .B0 (nx2976
                      ), .B1 (nx2793)) ;
    INV_X0P5B_A12TS ix2975 (.Y (nx2974), .A (buffers1_4)) ;
    INV_X0P5B_A12TS ix2977 (.Y (nx2976), .A (buffers0_4)) ;
    AOI221_X0P5M_A12TS ix2979 (.Y (nx2978), .A0 (wdata_4), .A1 (wenable), .B0 (
                       buffers3_4), .B1 (nx750), .C0 (nx796)) ;
    OAI21_X0P5M_A12TS ix797 (.Y (nx796), .A0 (nx2981), .A1 (nx2829), .B0 (nx2983
                      )) ;
    INV_X0P5B_A12TS ix2982 (.Y (nx2981), .A (buffers2_4)) ;
    AOI22_X0P5M_A12TS ix2984 (.Y (nx2983), .A0 (buffers1_4), .A1 (nx316), .B0 (
                      buffers0_4), .B1 (nx310)) ;
    OAI22_X0P5M_A12TS ix781 (.Y (nx780), .A0 (nx2971), .A1 (nx190), .B0 (nx2978)
                      , .B1 (nx2848)) ;
    OAI22_X0P5M_A12TS ix757 (.Y (nx758), .A0 (nx2971), .A1 (nx238), .B0 (nx2978)
                      , .B1 (nx2852)) ;
    OAI22_X0P5M_A12TS ix741 (.Y (nx740), .A0 (nx2971), .A1 (nx206), .B0 (nx2978)
                      , .B1 (nx2858)) ;
    OAI22_X0P5M_A12TS ix705 (.Y (nx704), .A0 (nx2989), .A1 (nx747), .B0 (nx2996)
                      , .B1 (nx2841)) ;
    AOI221_X0P5M_A12TS ix2990 (.Y (nx2989), .A0 (buffers3_3), .A1 (nx274), .B0 (
                       buffers2_3), .B1 (nx200), .C0 (nx756)) ;
    OAI22_X0P5M_A12TS ix659 (.Y (nx756), .A0 (nx2992), .A1 (nx2787), .B0 (nx2994
                      ), .B1 (nx2793)) ;
    INV_X0P5B_A12TS ix2993 (.Y (nx2992), .A (buffers1_3)) ;
    INV_X0P5B_A12TS ix2995 (.Y (nx2994), .A (buffers0_3)) ;
    AOI221_X0P5M_A12TS ix2997 (.Y (nx2996), .A0 (wdata_3), .A1 (wenable), .B0 (
                       buffers3_3), .B1 (nx750), .C0 (nx688)) ;
    OAI21_X0P5M_A12TS ix693 (.Y (nx688), .A0 (nx2999), .A1 (nx2829), .B0 (nx3001
                      )) ;
    INV_X0P5B_A12TS ix3000 (.Y (nx2999), .A (buffers2_3)) ;
    AOI22_X0P5M_A12TS ix3002 (.Y (nx3001), .A0 (buffers1_3), .A1 (nx316), .B0 (
                      buffers0_3), .B1 (nx310)) ;
    OAI22_X0P5M_A12TS ix673 (.Y (nx757), .A0 (nx2989), .A1 (nx190), .B0 (nx2996)
                      , .B1 (nx2848)) ;
    OAI22_X0P5M_A12TS ix649 (.Y (nx755), .A0 (nx2989), .A1 (nx238), .B0 (nx2996)
                      , .B1 (nx2852)) ;
    OAI22_X0P5M_A12TS ix694 (.Y (nx632), .A0 (nx2989), .A1 (nx206), .B0 (nx2996)
                      , .B1 (nx2858)) ;
    OAI22_X0P5M_A12TS ix695 (.Y (nx596), .A0 (nx3007), .A1 (nx747), .B0 (nx3014)
                      , .B1 (nx2841)) ;
    AOI221_X0P5M_A12TS ix3008 (.Y (nx3007), .A0 (buffers3_2), .A1 (nx274), .B0 (
                       buffers2_2), .B1 (nx200), .C0 (nx544)) ;
    OAI22_X0P5M_A12TS ix696 (.Y (nx544), .A0 (nx3010), .A1 (nx2787), .B0 (nx3012
                      ), .B1 (nx2793)) ;
    INV_X0P5B_A12TS ix3011 (.Y (nx3010), .A (buffers1_2)) ;
    INV_X0P5B_A12TS ix3013 (.Y (nx3012), .A (buffers0_2)) ;
    AOI221_X0P5M_A12TS ix3015 (.Y (nx3014), .A0 (wdata_2), .A1 (wenable), .B0 (
                       buffers3_2), .B1 (nx750), .C0 (nx754)) ;
    OAI21_X0P5M_A12TS ix575 (.Y (nx754), .A0 (nx3017), .A1 (nx2829), .B0 (nx3019
                      )) ;
    INV_X0P5B_A12TS ix3018 (.Y (nx3017), .A (buffers2_2)) ;
    AOI22_X0P5M_A12TS ix3020 (.Y (nx3019), .A0 (buffers1_2), .A1 (nx316), .B0 (
                      buffers0_2), .B1 (nx310)) ;
    OAI22_X0P5M_A12TS ix697 (.Y (nx558), .A0 (nx3007), .A1 (nx190), .B0 (nx3014)
                      , .B1 (nx2848)) ;
    OAI22_X0P5M_A12TS ix535 (.Y (nx753), .A0 (nx3007), .A1 (nx238), .B0 (nx3014)
                      , .B1 (nx2852)) ;
    OAI22_X0P5M_A12TS ix513 (.Y (nx752), .A0 (nx3007), .A1 (nx206), .B0 (nx3014)
                      , .B1 (nx2858)) ;
    OAI211_X0P5M_A12TS ix698 (.Y (nx464), .A0 (nx3025), .A1 (nx746), .B0 (nx3032
                       ), .C0 (nx2778)) ;
    INV_X0P5B_A12TS ix3029 (.Y (nx3028), .A (buffers2_1)) ;
    AOI22_X0P5M_A12TS ix3031 (.Y (nx3030), .A0 (buffers1_1), .A1 (nx316), .B0 (
                      buffers0_1), .B1 (nx310)) ;
    NAND2_X0P5A_A12TS ix3033 (.Y (nx3032), .A (nx2720), .B (nx746)) ;
    OAI211_X0P5M_A12TS ix421 (.Y (nx2720), .A0 (nx3035), .A1 (nx2751), .B0 (
                       nx2772), .C0 (nx3037)) ;
    INV_X0P5B_A12TS ix3036 (.Y (nx3035), .A (buffers3_1)) ;
    AOI222_X0P5M_A12TS ix3038 (.Y (nx3037), .A0 (buffers1_1), .A1 (nx264), .B0 (
                       buffers0_1), .B1 (nx232), .C0 (buffers2_1), .C1 (nx200)
                       ) ;
    OAI211_X0P5M_A12TS ix431 (.Y (nx751), .A0 (nx2831), .A1 (nx3044), .B0 (
                       nx3055), .C0 (nx2778)) ;
    NOR3_X0P5A_A12TS ix3054 (.Y (nx3053), .A (nx2755), .B (head_2), .C (head_0)
                     ) ;
    NAND2_X0P5A_A12TS ix3056 (.Y (nx3055), .A (nx2831), .B (nx2721)) ;
    OAI211_X0P5M_A12TS ix699 (.Y (nx2721), .A0 (nx3028), .A1 (nx2829), .B0 (
                       nx3058), .C0 (nx3030)) ;
    AOI22_X0P5M_A12TS ix3059 (.Y (nx3058), .A0 (flitin_0[1]), .A1 (wenable), .B0 (
                      buffers3_1), .B1 (nx750)) ;
    OAI211_X0P5M_A12TS ix700 (.Y (nx402), .A0 (nx2854), .A1 (nx3044), .B0 (
                       nx3061), .C0 (nx2778)) ;
    NAND2_X0P5A_A12TS ix3062 (.Y (nx3061), .A (nx2854), .B (nx2721)) ;
    OAI211_X0P5M_A12TS ix385 (.Y (nx384), .A0 (nx2860), .A1 (nx3044), .B0 (
                       nx3064), .C0 (nx2778)) ;
    NAND2_X0P5A_A12TS ix3065 (.Y (nx3064), .A (nx2860), .B (nx2721)) ;
    OAI211_X0P5M_A12TS ix701 (.Y (nx346), .A0 (nx3067), .A1 (nx746), .B0 (nx3074
                       ), .C0 (nx2778)) ;
    INV_X0P5B_A12TS ix3071 (.Y (nx3070), .A (buffers2_0)) ;
    AOI22_X0P5M_A12TS ix3073 (.Y (nx3072), .A0 (buffers1_0), .A1 (nx316), .B0 (
                      buffers0_0), .B1 (nx310)) ;
    NAND2_X0P5A_A12TS ix3075 (.Y (nx3074), .A (nx2718), .B (nx746)) ;
    OAI211_X0P5M_A12TS ix281 (.Y (nx2718), .A0 (nx3077), .A1 (nx2751), .B0 (
                       nx2772), .C0 (nx3079)) ;
    INV_X0P5B_A12TS ix3078 (.Y (nx3077), .A (buffers3_0)) ;
    AOI222_X0P5M_A12TS ix3080 (.Y (nx3079), .A0 (buffers1_0), .A1 (nx264), .B0 (
                       buffers0_0), .B1 (nx232), .C0 (buffers2_0), .C1 (nx200)
                       ) ;
    OAI211_X0P5M_A12TS ix702 (.Y (nx290), .A0 (nx2831), .A1 (nx3082), .B0 (
                       nx3089), .C0 (nx2778)) ;
    NAND2_X0P5A_A12TS ix3090 (.Y (nx3089), .A (nx2831), .B (nx2719)) ;
    OAI211_X0P5M_A12TS ix703 (.Y (nx2719), .A0 (nx3070), .A1 (nx2829), .B0 (
                       nx3092), .C0 (nx3072)) ;
    AOI22_X0P5M_A12TS ix3093 (.Y (nx3092), .A0 (flitin_0[0]), .A1 (wenable), .B0 (
                      buffers3_0), .B1 (nx750)) ;
    OAI211_X0P5M_A12TS ix253 (.Y (nx252), .A0 (nx2854), .A1 (nx3082), .B0 (
                       nx3095), .C0 (nx2778)) ;
    NAND2_X0P5A_A12TS ix3096 (.Y (nx3095), .A (nx2854), .B (nx2719)) ;
    NOR2_X0P5A_A12TS ix3099 (.Y (nx3098), .A (nx2854), .B (reset)) ;
    OAI211_X0P5M_A12TS ix221 (.Y (nx220), .A0 (nx2860), .A1 (nx3082), .B0 (
                       nx3101), .C0 (nx2778)) ;
    NAND2_X0P5A_A12TS ix3102 (.Y (nx3101), .A (nx2860), .B (nx2719)) ;
    NOR2_X0P5A_A12TS ix3105 (.Y (nx3104), .A (nx2860), .B (reset)) ;
    NOR2_X0P5A_A12TS ix3108 (.Y (nx3107), .A (nx2831), .B (reset)) ;
    TIELO_X1M_A12TS ix2684 (.Y (nx2683)) ;
    NAND2_X0P5A_A12TS ix373 (.Y (headflit_0), .A (nx3114), .B (nx3116)) ;
    AOI22_X0P5M_A12TS ix3115 (.Y (nx3114), .A0 (buffers0_0), .A1 (nx2795), .B0 (
                      buffers1_0), .B1 (nx2789)) ;
    AOI22_X0P5M_A12TS ix3117 (.Y (nx3116), .A0 (buffers2_0), .A1 (nx3053), .B0 (
                      buffers3_0), .B1 (nx86)) ;
    NAND2_X0P5A_A12TS ix491 (.Y (headflit_1), .A (nx3120), .B (nx3122)) ;
    AOI22_X0P5M_A12TS ix3121 (.Y (nx3120), .A0 (buffers0_1), .A1 (nx2795), .B0 (
                      buffers1_1), .B1 (nx2789)) ;
    AOI22_X0P5M_A12TS ix3123 (.Y (nx3122), .A0 (buffers2_1), .A1 (nx3053), .B0 (
                      buffers3_1), .B1 (nx86)) ;
    NAND2_X0P5A_A12TS ix704 (.Y (headflit_2), .A (nx3125), .B (nx3127)) ;
    AOI22_X0P5M_A12TS ix3126 (.Y (nx3125), .A0 (buffers0_2), .A1 (nx2795), .B0 (
                      buffers1_2), .B1 (nx2789)) ;
    AOI22_X0P5M_A12TS ix3128 (.Y (nx3127), .A0 (buffers2_2), .A1 (nx3053), .B0 (
                      buffers3_2), .B1 (nx86)) ;
    NAND2_X0P5A_A12TS ix731 (.Y (headflit_3), .A (nx3130), .B (nx3132)) ;
    AOI22_X0P5M_A12TS ix3131 (.Y (nx3130), .A0 (buffers0_3), .A1 (nx2795), .B0 (
                      buffers1_3), .B1 (nx2789)) ;
    AOI22_X0P5M_A12TS ix3133 (.Y (nx3132), .A0 (buffers2_3), .A1 (nx3053), .B0 (
                      buffers3_3), .B1 (nx86)) ;
    NAND2_X0P5A_A12TS ix839 (.Y (headflit_4), .A (nx3135), .B (nx3137)) ;
    AOI22_X0P5M_A12TS ix3136 (.Y (nx3135), .A0 (buffers0_4), .A1 (nx2795), .B0 (
                      buffers1_4), .B1 (nx2789)) ;
    AOI22_X0P5M_A12TS ix3138 (.Y (nx3137), .A0 (buffers2_4), .A1 (nx3053), .B0 (
                      buffers3_4), .B1 (nx86)) ;
    NAND2_X0P5A_A12TS ix947 (.Y (headflit_5), .A (nx3140), .B (nx3142)) ;
    AOI22_X0P5M_A12TS ix3141 (.Y (nx3140), .A0 (buffers0_5), .A1 (nx2795), .B0 (
                      buffers1_5), .B1 (nx2789)) ;
    AOI22_X0P5M_A12TS ix3143 (.Y (nx3142), .A0 (buffers2_5), .A1 (nx3053), .B0 (
                      buffers3_5), .B1 (nx86)) ;
    NAND2_X0P5A_A12TS ix1055 (.Y (headflit_6), .A (nx3145), .B (nx3147)) ;
    AOI22_X0P5M_A12TS ix3146 (.Y (nx3145), .A0 (buffers0_6), .A1 (nx2795), .B0 (
                      buffers1_6), .B1 (nx2789)) ;
    AOI22_X0P5M_A12TS ix3148 (.Y (nx3147), .A0 (buffers2_6), .A1 (nx3053), .B0 (
                      buffers3_6), .B1 (nx86)) ;
    NAND2_X0P5A_A12TS ix1163 (.Y (headflit_7), .A (nx3150), .B (nx3152)) ;
    AOI22_X0P5M_A12TS ix3151 (.Y (nx3150), .A0 (buffers0_7), .A1 (nx2795), .B0 (
                      buffers1_7), .B1 (nx2789)) ;
    AOI22_X0P5M_A12TS ix3153 (.Y (nx3152), .A0 (buffers2_7), .A1 (nx3053), .B0 (
                      buffers3_7), .B1 (nx86)) ;
    NAND2_X0P5A_A12TS ix1271 (.Y (headflit_8), .A (nx3155), .B (nx3157)) ;
    AOI22_X0P5M_A12TS ix3156 (.Y (nx3155), .A0 (buffers0_8), .A1 (nx2795), .B0 (
                      buffers1_8), .B1 (nx2789)) ;
    AOI22_X0P5M_A12TS ix3158 (.Y (nx3157), .A0 (buffers2_8), .A1 (nx3053), .B0 (
                      buffers3_8), .B1 (nx86)) ;
    NAND2_X0P5A_A12TS ix1379 (.Y (headflit_9), .A (nx3160), .B (nx3162)) ;
    AOI22_X0P5M_A12TS ix3161 (.Y (nx3160), .A0 (buffers0_9), .A1 (nx2795), .B0 (
                      buffers1_9), .B1 (nx2789)) ;
    AOI22_X0P5M_A12TS ix3163 (.Y (nx3162), .A0 (buffers2_9), .A1 (nx3053), .B0 (
                      buffers3_9), .B1 (nx86)) ;
    NAND2_X0P5A_A12TS ix1487 (.Y (headflit_10), .A (nx3165), .B (nx3167)) ;
    AOI22_X0P5M_A12TS ix3166 (.Y (nx3165), .A0 (buffers0_10), .A1 (nx2795), .B0 (
                      buffers1_10), .B1 (nx2789)) ;
    AOI22_X0P5M_A12TS ix3168 (.Y (nx3167), .A0 (buffers2_10), .A1 (nx3053), .B0 (
                      buffers3_10), .B1 (nx86)) ;
    NAND2_X0P5A_A12TS ix1595 (.Y (headflit_11), .A (nx3170), .B (nx3172)) ;
    AOI22_X0P5M_A12TS ix3171 (.Y (nx3170), .A0 (buffers0_11), .A1 (nx2795), .B0 (
                      buffers1_11), .B1 (nx2789)) ;
    AOI22_X0P5M_A12TS ix3173 (.Y (nx3172), .A0 (buffers2_11), .A1 (nx3053), .B0 (
                      buffers3_11), .B1 (nx86)) ;
    DFFQ_X0P5M_A12TS reg_tailout (.Q (tailout_0), .CK (clk), .D (nx1762)) ;
    NOR3_X0P5A_A12TS ix1763 (.Y (nx1762), .A (nx3176), .B (nx3178), .C (
                     headflit_1)) ;
    NAND2_X0P5A_A12TS ix3177 (.Y (nx3176), .A (debitout_0), .B (nx2778)) ;
    DFFQ_X0P5M_A12TS reg_rdata_0 (.Q (rdata_0), .CK (clk), .D (nx1604)) ;
    OAI211_X0P5M_A12TS ix1605 (.Y (nx1604), .A0 (debitout_0), .A1 (nx3183), .B0 (
                       nx3185), .C0 (nx2778)) ;
    INV_X0P5B_A12TS ix3184 (.Y (nx3183), .A (rdata_0)) ;
    NAND2_X0P5A_A12TS ix3186 (.Y (nx3185), .A (debitout_0), .B (headflit_0)) ;
    DFFQ_X0P5M_A12TS reg_rdata_1 (.Q (rdata_1), .CK (clk), .D (nx1620)) ;
    OAI211_X0P5M_A12TS ix1621 (.Y (nx1620), .A0 (debitout_0), .A1 (nx3189), .B0 (
                       nx3191), .C0 (nx2778)) ;
    INV_X0P5B_A12TS ix3190 (.Y (nx3189), .A (rdata_1)) ;
    NAND2_X0P5A_A12TS ix3192 (.Y (nx3191), .A (debitout_0), .B (headflit_1)) ;
    DFFQ_X0P5M_A12TS reg_rdata_2 (.Q (rdata_2), .CK (clk), .D (nx1642)) ;
    INV_X0P5B_A12TS ix1643 (.Y (nx1642), .A (nx3195)) ;
    AOI22_X0P5M_A12TS ix3196 (.Y (nx3195), .A0 (rdata_2), .A1 (nx1632), .B0 (
                      headflit_2), .B1 (nx1638)) ;
    NOR2_X0P5A_A12TS ix1633 (.Y (nx1632), .A (debitout_0), .B (reset)) ;
    DFFQ_X0P5M_A12TS reg_rdata_3 (.Q (rdata_3), .CK (clk), .D (nx1654)) ;
    INV_X0P5B_A12TS ix1655 (.Y (nx1654), .A (nx3201)) ;
    AOI22_X0P5M_A12TS ix3202 (.Y (nx3201), .A0 (rdata_3), .A1 (nx1632), .B0 (
                      headflit_3), .B1 (nx1638)) ;
    DFFQ_X0P5M_A12TS reg_rdata_4 (.Q (rdata_4), .CK (clk), .D (nx1666)) ;
    INV_X0P5B_A12TS ix1667 (.Y (nx1666), .A (nx3205)) ;
    AOI22_X0P5M_A12TS ix3206 (.Y (nx3205), .A0 (rdata_4), .A1 (nx1632), .B0 (
                      headflit_4), .B1 (nx1638)) ;
    DFFQ_X0P5M_A12TS reg_rdata_5 (.Q (rdata_5), .CK (clk), .D (nx1678)) ;
    INV_X0P5B_A12TS ix1679 (.Y (nx1678), .A (nx3209)) ;
    AOI22_X0P5M_A12TS ix3210 (.Y (nx3209), .A0 (rdata_5), .A1 (nx1632), .B0 (
                      headflit_5), .B1 (nx1638)) ;
    DFFQ_X0P5M_A12TS reg_rdata_6 (.Q (rdata_6), .CK (clk), .D (nx1690)) ;
    INV_X0P5B_A12TS ix1691 (.Y (nx1690), .A (nx3213)) ;
    AOI22_X0P5M_A12TS ix3214 (.Y (nx3213), .A0 (rdata_6), .A1 (nx1632), .B0 (
                      headflit_6), .B1 (nx1638)) ;
    DFFQ_X0P5M_A12TS reg_rdata_7 (.Q (rdata_7), .CK (clk), .D (nx1702)) ;
    INV_X0P5B_A12TS ix1703 (.Y (nx1702), .A (nx3217)) ;
    AOI22_X0P5M_A12TS ix3218 (.Y (nx3217), .A0 (rdata_7), .A1 (nx1632), .B0 (
                      headflit_7), .B1 (nx1638)) ;
    DFFQ_X0P5M_A12TS reg_rdata_8 (.Q (rdata_8), .CK (clk), .D (nx1714)) ;
    INV_X0P5B_A12TS ix1715 (.Y (nx1714), .A (nx3221)) ;
    AOI22_X0P5M_A12TS ix3222 (.Y (nx3221), .A0 (rdata_8), .A1 (nx1632), .B0 (
                      headflit_8), .B1 (nx1638)) ;
    DFFQ_X0P5M_A12TS reg_rdata_9 (.Q (rdata_9), .CK (clk), .D (nx1726)) ;
    INV_X0P5B_A12TS ix1727 (.Y (nx1726), .A (nx3225)) ;
    AOI22_X0P5M_A12TS ix3226 (.Y (nx3225), .A0 (rdata_9), .A1 (nx1632), .B0 (
                      headflit_9), .B1 (nx1638)) ;
    DFFQ_X0P5M_A12TS reg_rdata_10 (.Q (rdata_10), .CK (clk), .D (nx1738)) ;
    INV_X0P5B_A12TS ix1739 (.Y (nx1738), .A (nx3229)) ;
    AOI22_X0P5M_A12TS ix3230 (.Y (nx3229), .A0 (rdata_10), .A1 (nx1632), .B0 (
                      headflit_10), .B1 (nx1638)) ;
    DFFQ_X0P5M_A12TS reg_rdata_11 (.Q (rdata_11), .CK (clk), .D (nx1750)) ;
    INV_X0P5B_A12TS ix1751 (.Y (nx1750), .A (nx3233)) ;
    AOI22_X0P5M_A12TS ix3234 (.Y (nx3233), .A0 (rdata_11), .A1 (nx1632), .B0 (
                      headflit_11), .B1 (nx1638)) ;
    INV_X0P5B_A12TS ix1639 (.Y (nx1638), .A (nx3176)) ;
    INV_X0P5B_A12TS ix3026 (.Y (nx3025), .A (nx2721)) ;
    INV_X0P5B_A12TS ix3045 (.Y (nx3044), .A (nx2720)) ;
    INV_X0P5B_A12TS ix265 (.Y (nx264), .A (nx2787)) ;
    INV_X0P5B_A12TS ix239 (.Y (nx238), .A (nx3098)) ;
    INV_X0P5B_A12TS ix233 (.Y (nx232), .A (nx2793)) ;
    INV_X0P5B_A12TS ix3068 (.Y (nx3067), .A (nx2719)) ;
    INV_X0P5B_A12TS ix3083 (.Y (nx3082), .A (nx2718)) ;
    INV_X0P5B_A12TS ix207 (.Y (nx206), .A (nx3104)) ;
    INV_X0P5B_A12TS ix191 (.Y (nx190), .A (nx3107)) ;
    INV_X0P5B_A12TS ix89 (.Y (nx88), .A (nx2807)) ;
    INV_X0P5B_A12TS ix87 (.Y (nx86), .A (nx2751)) ;
    INV_X0P5B_A12TS ix3179 (.Y (nx3178), .A (headflit_0)) ;
    NAND4B_X0P5M_A12TS ix2767 (.Y (nx2766), .AN (head_2), .B (head_0), .C (
                       debitout_0), .D (head_1)) ;
    NOR2B_X0P7M_A12TS ix317 (.Y (nx316), .AN (nx2854), .B (nx2807)) ;
    AND2_X0P5M_A12TS ix706 (.Y (nx310), .A (nx2860), .B (nx2807)) ;
    OR2_X0P5M_A12TS ix2842 (.Y (nx2841), .A (reset), .B (nx746)) ;
    NOR2B_X0P7M_A12TS ix3052 (.Y (nx200), .AN (nx3053), .B (debitout_0)) ;
    NAND2B_X0P7M_A12TS ix243 (.Y (nx242), .AN (nx2789), .B (nx3098)) ;
    NAND2B_X0P7M_A12TS ix211 (.Y (nx749), .AN (nx2795), .B (nx3104)) ;
    NAND2B_X0P7M_A12TS ix195 (.Y (nx194), .AN (nx3053), .B (nx3107)) ;
    NAND2B_X0P7M_A12TS ix707 (.Y (nx748), .AN (nx747), .B (nx2751)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_11 (.Q (buffers1_11), .CK (clk), .D (
                         nx1512), .R (nx2683), .SE (NOT_nx242), .SI (buffers1_11
                         ), .SN (nx3871)) ;
    INV_X0P5B_A12TS ix3870 (.Y (NOT_nx242), .A (nx242)) ;
    INV_X2M_A12TS ix3872 (.Y (nx3871), .A (nx2683)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_11 (.Q (buffers0_11), .CK (clk), .D (
                         nx1496), .R (nx2683), .SE (NOT_nx210), .SI (buffers0_11
                         ), .SN (nx3871)) ;
    INV_X0P5B_A12TS ix3875 (.Y (NOT_nx210), .A (nx749)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_11 (.Q (buffers2_11), .CK (clk), .D (
                         nx1536), .R (nx2683), .SE (NOT_nx194), .SI (buffers2_11
                         ), .SN (nx3871)) ;
    INV_X0P5B_A12TS ix3878 (.Y (NOT_nx194), .A (nx194)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_11 (.Q (buffers3_11), .CK (clk), .D (
                         nx1568), .R (nx2683), .SE (NOT_nx186), .SI (buffers3_11
                         ), .SN (nx3871)) ;
    INV_X0P5B_A12TS ix3881 (.Y (NOT_nx186), .A (nx748)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_10 (.Q (buffers1_10), .CK (clk), .D (
                         nx1404), .R (nx2683), .SE (NOT_nx242), .SI (buffers1_10
                         ), .SN (nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_10 (.Q (buffers0_10), .CK (clk), .D (
                         nx1388), .R (nx2683), .SE (NOT_nx210), .SI (buffers0_10
                         ), .SN (nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_10 (.Q (buffers2_10), .CK (clk), .D (
                         nx1428), .R (nx2683), .SE (NOT_nx194), .SI (buffers2_10
                         ), .SN (nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_10 (.Q (buffers3_10), .CK (clk), .D (
                         nx1460), .R (nx2683), .SE (NOT_nx186), .SI (buffers3_10
                         ), .SN (nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_9 (.Q (buffers1_9), .CK (clk), .D (nx1296)
                         , .R (nx2683), .SE (NOT_nx242), .SI (buffers1_9), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_9 (.Q (buffers0_9), .CK (clk), .D (nx1280)
                         , .R (nx2683), .SE (NOT_nx210), .SI (buffers0_9), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_9 (.Q (buffers2_9), .CK (clk), .D (nx1320)
                         , .R (nx2683), .SE (NOT_nx194), .SI (buffers2_9), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_9 (.Q (buffers3_9), .CK (clk), .D (nx1352)
                         , .R (nx2683), .SE (NOT_nx186), .SI (buffers3_9), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_8 (.Q (buffers1_8), .CK (clk), .D (nx1188)
                         , .R (nx2683), .SE (NOT_nx242), .SI (buffers1_8), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_8 (.Q (buffers0_8), .CK (clk), .D (nx1172)
                         , .R (nx2683), .SE (NOT_nx210), .SI (buffers0_8), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_8 (.Q (buffers2_8), .CK (clk), .D (nx1212)
                         , .R (nx2683), .SE (NOT_nx194), .SI (buffers2_8), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_8 (.Q (buffers3_8), .CK (clk), .D (nx1244)
                         , .R (nx2683), .SE (NOT_nx186), .SI (buffers3_8), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_7 (.Q (buffers1_7), .CK (clk), .D (nx1080)
                         , .R (nx2683), .SE (NOT_nx242), .SI (buffers1_7), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_7 (.Q (buffers0_7), .CK (clk), .D (nx1064)
                         , .R (nx2683), .SE (NOT_nx210), .SI (buffers0_7), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_7 (.Q (buffers2_7), .CK (clk), .D (nx1104)
                         , .R (nx2683), .SE (NOT_nx194), .SI (buffers2_7), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_7 (.Q (buffers3_7), .CK (clk), .D (nx1136)
                         , .R (nx2683), .SE (NOT_nx186), .SI (buffers3_7), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_6 (.Q (buffers1_6), .CK (clk), .D (nx972)
                         , .R (nx2683), .SE (NOT_nx242), .SI (buffers1_6), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_6 (.Q (buffers0_6), .CK (clk), .D (nx956)
                         , .R (nx2683), .SE (NOT_nx210), .SI (buffers0_6), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_6 (.Q (buffers2_6), .CK (clk), .D (nx996)
                         , .R (nx2683), .SE (NOT_nx194), .SI (buffers2_6), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_6 (.Q (buffers3_6), .CK (clk), .D (nx1028)
                         , .R (nx2683), .SE (NOT_nx186), .SI (buffers3_6), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_5 (.Q (buffers1_5), .CK (clk), .D (nx864)
                         , .R (nx2683), .SE (NOT_nx242), .SI (buffers1_5), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_5 (.Q (buffers0_5), .CK (clk), .D (nx848)
                         , .R (nx2683), .SE (NOT_nx210), .SI (buffers0_5), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_5 (.Q (buffers2_5), .CK (clk), .D (nx888)
                         , .R (nx2683), .SE (NOT_nx194), .SI (buffers2_5), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_5 (.Q (buffers3_5), .CK (clk), .D (nx920)
                         , .R (nx2683), .SE (NOT_nx186), .SI (buffers3_5), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_4 (.Q (buffers1_4), .CK (clk), .D (nx758)
                         , .R (nx2683), .SE (NOT_nx242), .SI (buffers1_4), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_4 (.Q (buffers0_4), .CK (clk), .D (nx740)
                         , .R (nx2683), .SE (NOT_nx210), .SI (buffers0_4), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_4 (.Q (buffers2_4), .CK (clk), .D (nx780)
                         , .R (nx2683), .SE (NOT_nx194), .SI (buffers2_4), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_4 (.Q (buffers3_4), .CK (clk), .D (nx812)
                         , .R (nx2683), .SE (NOT_nx186), .SI (buffers3_4), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_3 (.Q (buffers1_3), .CK (clk), .D (nx755)
                         , .R (nx2683), .SE (NOT_nx242), .SI (buffers1_3), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_3 (.Q (buffers0_3), .CK (clk), .D (nx632)
                         , .R (nx2683), .SE (NOT_nx210), .SI (buffers0_3), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_3 (.Q (buffers2_3), .CK (clk), .D (nx757)
                         , .R (nx2683), .SE (NOT_nx194), .SI (buffers2_3), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_3 (.Q (buffers3_3), .CK (clk), .D (nx704)
                         , .R (nx2683), .SE (NOT_nx186), .SI (buffers3_3), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_2 (.Q (buffers1_2), .CK (clk), .D (nx753)
                         , .R (nx2683), .SE (NOT_nx242), .SI (buffers1_2), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_2 (.Q (buffers0_2), .CK (clk), .D (nx752)
                         , .R (nx2683), .SE (NOT_nx210), .SI (buffers0_2), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_2 (.Q (buffers2_2), .CK (clk), .D (nx558)
                         , .R (nx2683), .SE (NOT_nx194), .SI (buffers2_2), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_2 (.Q (buffers3_2), .CK (clk), .D (nx596)
                         , .R (nx2683), .SE (NOT_nx186), .SI (buffers3_2), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_1 (.Q (buffers1_1), .CK (clk), .D (nx402)
                         , .R (nx2683), .SE (NOT_nx242), .SI (buffers1_1), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_1 (.Q (buffers0_1), .CK (clk), .D (nx384)
                         , .R (nx2683), .SE (NOT_nx210), .SI (buffers0_1), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_1 (.Q (buffers2_1), .CK (clk), .D (nx751)
                         , .R (nx2683), .SE (NOT_nx194), .SI (buffers2_1), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_1 (.Q (buffers3_1), .CK (clk), .D (nx464)
                         , .R (nx2683), .SE (NOT_nx186), .SI (buffers3_1), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_0 (.Q (buffers1_0), .CK (clk), .D (nx252)
                         , .R (nx2683), .SE (NOT_nx242), .SI (buffers1_0), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_0 (.Q (buffers0_0), .CK (clk), .D (nx220)
                         , .R (nx2683), .SE (NOT_nx210), .SI (buffers0_0), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_0 (.Q (buffers2_0), .CK (clk), .D (nx290)
                         , .R (nx2683), .SE (NOT_nx194), .SI (buffers2_0), .SN (
                         nx3871)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_0 (.Q (buffers3_0), .CK (clk), .D (nx346)
                         , .R (nx2683), .SE (NOT_nx186), .SI (buffers3_0), .SN (
                         nx3871)) ;
    OAI22_X0P5M_A12TS ix759 (.Y (nx1294), .A0 (nx1312), .A1 (nx1156), .B0 (
                      nx1339), .B1 (nx1346)) ;
    AOI221_X0P5M_A12TS ix760 (.Y (nx1312), .A0 (buffers3_11__dup_1285), .A1 (
                       nx1175), .B0 (buffers2_11__dup_1286), .B1 (nx1161), .C0 (
                       nx1291)) ;
    NOR2_X0P5A_A12TS ix761 (.Y (nx1175), .A (debitout_1), .B (nx1313)) ;
    AOI21_X0P5M_A12TS ix762 (.Y (nx1313), .A0 (head_1__dup_1142), .A1 (
                      head_0__dup_1139), .B0 (head_2__dup_1140)) ;
    DFFQ_X0P5M_A12TS reg_head_1__dup_5266 (.Q (head_1__dup_1142), .CK (clk), .D (
                     nx1143)) ;
    OA21A1OI2_X0P5M_A12TS ix763 (.Y (nx1143), .A0 (debitout_1), .A1 (nx1314), .B0 (
                          nx1315), .C0 (nx1141)) ;
    INV_X0P5B_A12TS ix764 (.Y (nx1314), .A (head_1__dup_1142)) ;
    OAI211_X0P5M_A12TS ix765 (.Y (nx1315), .A0 (head_0__dup_1139), .A1 (
                       head_1__dup_1142), .B0 (debitout_1), .C0 (nx1317)) ;
    DFFQ_X0P5M_A12TS reg_head_0__dup_5270 (.Q (head_0__dup_1139), .CK (clk), .D (
                     nx1145)) ;
    NOR2_X0P5A_A12TS ix766 (.Y (nx1145), .A (nx1316), .B (reset)) ;
    XNOR2_X0P5M_A12TS ix768 (.Y (nx1316), .A (head_0__dup_1139), .B (debitout_1)
                      ) ;
    NAND2_X0P5A_A12TS ix769 (.Y (nx1317), .A (head_1__dup_1142), .B (
                      head_0__dup_1139)) ;
    NAND2_X0P5A_A12TS ix770 (.Y (nx1141), .A (nx1318), .B (nx1323)) ;
    DFFQ_X0P5M_A12TS reg_head_2__dup_5275 (.Q (head_2__dup_1140), .CK (clk), .D (
                     nx1144)) ;
    OA21A1OI2_X0P5M_A12TS ix771 (.Y (nx1144), .A0 (nx1319), .A1 (nx1321), .B0 (
                          nx1322), .C0 (nx1141)) ;
    INV_X0P5B_A12TS ix772 (.Y (nx1319), .A (debitout_1)) ;
    XOR2_X0P5M_A12TS ix773 (.Y (nx1321), .A (head_2__dup_1140), .B (nx1317)) ;
    NAND2_X0P5A_A12TS ix774 (.Y (nx1322), .A (nx1319), .B (head_2__dup_1140)) ;
    INV_X0P5B_A12TS ix775 (.Y (nx1323), .A (reset)) ;
    INV_X0P5B_A12TS ix776 (.Y (nx1324), .A (head_0__dup_1139)) ;
    OAI22_X0P5M_A12TS ix777 (.Y (nx1291), .A0 (nx1325), .A1 (nx1326), .B0 (
                      nx1328), .B1 (nx1329)) ;
    INV_X0P5B_A12TS ix778 (.Y (nx1325), .A (buffers1_11__dup_1289)) ;
    NAND2_X0P5A_A12TS ix779 (.Y (nx1326), .A (nx1319), .B (nx1327)) ;
    NOR3_X0P5A_A12TS ix780 (.Y (nx1327), .A (head_1__dup_1142), .B (
                     head_2__dup_1140), .C (nx1324)) ;
    INV_X0P5B_A12TS ix782 (.Y (nx1328), .A (buffers0_11__dup_1287)) ;
    NAND2_X0P5A_A12TS ix783 (.Y (nx1329), .A (nx1319), .B (nx1330)) ;
    NOR3_X0P5A_A12TS ix784 (.Y (nx1330), .A (head_1__dup_1142), .B (
                     head_2__dup_1140), .C (head_0__dup_1139)) ;
    NAND2_X0P5A_A12TS ix785 (.Y (nx1156), .A (nx1155), .B (nx1323)) ;
    AOI21_X0P5M_A12TS ix786 (.Y (nx1155), .A0 (tail_1__dup_1151), .A1 (
                      tail_0__dup_1147), .B0 (tail_2__dup_1149)) ;
    DFFQ_X0P5M_A12TS reg_tail_1__dup_5291 (.Q (tail_1__dup_1151), .CK (clk), .D (
                     nx1152)) ;
    OA21A1OI2_X0P5M_A12TS ix787 (.Y (nx1152), .A0 (wenable_dup_397), .A1 (nx1331
                          ), .B0 (nx1332), .C0 (nx1150)) ;
    INV_X0P5B_A12TS ix788 (.Y (nx1331), .A (tail_1__dup_1151)) ;
    OAI211_X0P5M_A12TS ix789 (.Y (nx1332), .A0 (tail_0__dup_1147), .A1 (
                       tail_1__dup_1151), .B0 (wenable_dup_397), .C0 (nx1334)) ;
    DFFQ_X0P5M_A12TS reg_tail_0__dup_5295 (.Q (tail_0__dup_1147), .CK (clk), .D (
                     nx1154)) ;
    NOR2_X0P5A_A12TS ix790 (.Y (nx1154), .A (nx1333), .B (reset)) ;
    XNOR2_X0P5M_A12TS ix791 (.Y (nx1333), .A (tail_0__dup_1147), .B (
                      wenable_dup_397)) ;
    NAND2_X0P5A_A12TS ix792 (.Y (nx1334), .A (tail_1__dup_1151), .B (
                      tail_0__dup_1147)) ;
    OAI31_X0P5M_A12TS ix793 (.Y (nx1150), .A0 (nx1148), .A1 (tail_2__dup_1149), 
                      .A2 (nx1334), .B0 (nx1323)) ;
    DFFQ_X0P5M_A12TS reg_tail_2__dup_5300 (.Q (tail_2__dup_1149), .CK (clk), .D (
                     nx1153)) ;
    OA21A1OI2_X0P5M_A12TS ix794 (.Y (nx1153), .A0 (nx1335), .A1 (nx1337), .B0 (
                          nx1338), .C0 (nx1150)) ;
    INV_X0P5B_A12TS ix795 (.Y (nx1335), .A (wenable_dup_397)) ;
    XOR2_X0P5M_A12TS ix796 (.Y (nx1337), .A (tail_2__dup_1149), .B (nx1334)) ;
    NAND2_X0P5A_A12TS ix798 (.Y (nx1338), .A (nx1335), .B (tail_2__dup_1149)) ;
    AOI221_X0P5M_A12TS ix799 (.Y (nx1339), .A0 (wdata_11__dup_387), .A1 (
                       wenable_dup_397), .B0 (buffers3_11__dup_1285), .B1 (
                       nx1179), .C0 (nx1293)) ;
    AOI21_X0P5M_A12TS ix800 (.Y (nx1179), .A0 (nx1334), .A1 (nx1340), .B0 (
                      wenable_dup_397)) ;
    INV_X0P5B_A12TS ix801 (.Y (nx1340), .A (tail_2__dup_1149)) ;
    OAI21_X0P5M_A12TS ix802 (.Y (nx1293), .A0 (nx1341), .A1 (nx1342), .B0 (
                      nx1344)) ;
    INV_X0P5B_A12TS ix803 (.Y (nx1341), .A (buffers2_11__dup_1286)) ;
    NAND2_X0P5A_A12TS ix804 (.Y (nx1342), .A (nx1343), .B (nx1333)) ;
    NOR3_X0P5A_A12TS ix805 (.Y (nx1343), .A (nx1331), .B (tail_2__dup_1149), .C (
                     tail_0__dup_1147)) ;
    AOI22_X0P5M_A12TS ix806 (.Y (nx1344), .A0 (buffers1_11__dup_1289), .A1 (
                      nx1178), .B0 (buffers0_11__dup_1287), .B1 (nx1177)) ;
    INV_X0P5B_A12TS ix807 (.Y (nx1345), .A (tail_0__dup_1147)) ;
    OAI22_X0P5M_A12TS ix808 (.Y (nx1292), .A0 (nx1312), .A1 (nx1159), .B0 (
                      nx1339), .B1 (nx1347)) ;
    NAND2_X0P5A_A12TS ix809 (.Y (nx1347), .A (nx1323), .B (nx1343)) ;
    OAI22_X0P5M_A12TS ix810 (.Y (nx1290), .A0 (nx1312), .A1 (nx1170), .B0 (
                      nx1339), .B1 (nx1348)) ;
    NAND2_X0P5A_A12TS ix811 (.Y (nx1348), .A (nx1323), .B (nx1349)) ;
    NOR3_X0P5A_A12TS ix812 (.Y (nx1349), .A (tail_1__dup_1151), .B (
                     tail_2__dup_1149), .C (nx1345)) ;
    OAI22_X0P5M_A12TS ix814 (.Y (nx1288), .A0 (nx1312), .A1 (nx1163), .B0 (
                      nx1339), .B1 (nx1350)) ;
    NAND2_X0P5A_A12TS ix815 (.Y (nx1350), .A (nx1323), .B (nx1351)) ;
    NOR3_X0P5A_A12TS ix816 (.Y (nx1351), .A (tail_1__dup_1151), .B (
                     tail_2__dup_1149), .C (tail_0__dup_1147)) ;
    OAI22_X0P5M_A12TS ix817 (.Y (nx1284), .A0 (nx1353), .A1 (nx1156), .B0 (
                      nx1356), .B1 (nx1346)) ;
    AOI221_X0P5M_A12TS ix818 (.Y (nx1353), .A0 (buffers3_10__dup_1274), .A1 (
                       nx1175), .B0 (buffers2_10__dup_1275), .B1 (nx1161), .C0 (
                       nx1281)) ;
    OAI22_X0P5M_A12TS ix819 (.Y (nx1281), .A0 (nx1354), .A1 (nx1326), .B0 (
                      nx1355), .B1 (nx1329)) ;
    INV_X0P5B_A12TS ix820 (.Y (nx1354), .A (buffers1_10__dup_1278)) ;
    INV_X0P5B_A12TS ix821 (.Y (nx1355), .A (buffers0_10__dup_1276)) ;
    AOI221_X0P5M_A12TS ix822 (.Y (nx1356), .A0 (wdata_10__dup_388), .A1 (
                       wenable_dup_397), .B0 (buffers3_10__dup_1274), .B1 (
                       nx1179), .C0 (nx1283)) ;
    OAI21_X0P5M_A12TS ix823 (.Y (nx1283), .A0 (nx1357), .A1 (nx1342), .B0 (
                      nx1358)) ;
    INV_X0P5B_A12TS ix824 (.Y (nx1357), .A (buffers2_10__dup_1275)) ;
    AOI22_X0P5M_A12TS ix825 (.Y (nx1358), .A0 (buffers1_10__dup_1278), .A1 (
                      nx1178), .B0 (buffers0_10__dup_1276), .B1 (nx1177)) ;
    OAI22_X0P5M_A12TS ix826 (.Y (nx1282), .A0 (nx1353), .A1 (nx1159), .B0 (
                      nx1356), .B1 (nx1347)) ;
    OAI22_X0P5M_A12TS ix827 (.Y (nx1279), .A0 (nx1353), .A1 (nx1170), .B0 (
                      nx1356), .B1 (nx1348)) ;
    OAI22_X0P5M_A12TS ix828 (.Y (nx1277), .A0 (nx1353), .A1 (nx1163), .B0 (
                      nx1356), .B1 (nx1350)) ;
    OAI22_X0P5M_A12TS ix829 (.Y (nx1273), .A0 (nx1359), .A1 (nx1156), .B0 (
                      nx1362), .B1 (nx1346)) ;
    AOI221_X0P5M_A12TS ix830 (.Y (nx1359), .A0 (buffers3_9__dup_1264), .A1 (
                       nx1175), .B0 (buffers2_9__dup_1265), .B1 (nx1161), .C0 (
                       nx1270)) ;
    OAI22_X0P5M_A12TS ix831 (.Y (nx1270), .A0 (nx1360), .A1 (nx1326), .B0 (
                      nx1361), .B1 (nx1329)) ;
    INV_X0P5B_A12TS ix832 (.Y (nx1360), .A (buffers1_9__dup_1268)) ;
    INV_X0P5B_A12TS ix833 (.Y (nx1361), .A (buffers0_9__dup_1266)) ;
    AOI221_X0P5M_A12TS ix834 (.Y (nx1362), .A0 (wdata_9__dup_389), .A1 (
                       wenable_dup_397), .B0 (buffers3_9__dup_1264), .B1 (nx1179
                       ), .C0 (nx1272)) ;
    OAI21_X0P5M_A12TS ix835 (.Y (nx1272), .A0 (nx1363), .A1 (nx1342), .B0 (
                      nx1364)) ;
    INV_X0P5B_A12TS ix836 (.Y (nx1363), .A (buffers2_9__dup_1265)) ;
    AOI22_X0P5M_A12TS ix837 (.Y (nx1364), .A0 (buffers1_9__dup_1268), .A1 (
                      nx1178), .B0 (buffers0_9__dup_1266), .B1 (nx1177)) ;
    OAI22_X0P5M_A12TS ix838 (.Y (nx1271), .A0 (nx1359), .A1 (nx1159), .B0 (
                      nx1362), .B1 (nx1347)) ;
    OAI22_X0P5M_A12TS ix840 (.Y (nx1269), .A0 (nx1359), .A1 (nx1170), .B0 (
                      nx1362), .B1 (nx1348)) ;
    OAI22_X0P5M_A12TS ix841 (.Y (nx1267), .A0 (nx1359), .A1 (nx1163), .B0 (
                      nx1362), .B1 (nx1350)) ;
    OAI22_X0P5M_A12TS ix842 (.Y (nx1263), .A0 (nx1365), .A1 (nx1156), .B0 (
                      nx1368), .B1 (nx1346)) ;
    AOI221_X0P5M_A12TS ix843 (.Y (nx1365), .A0 (buffers3_8__dup_1254), .A1 (
                       nx1175), .B0 (buffers2_8__dup_1255), .B1 (nx1161), .C0 (
                       nx1260)) ;
    OAI22_X0P5M_A12TS ix844 (.Y (nx1260), .A0 (nx1366), .A1 (nx1326), .B0 (
                      nx1367), .B1 (nx1329)) ;
    INV_X0P5B_A12TS ix845 (.Y (nx1366), .A (buffers1_8__dup_1258)) ;
    INV_X0P5B_A12TS ix846 (.Y (nx1367), .A (buffers0_8__dup_1256)) ;
    AOI221_X0P5M_A12TS ix847 (.Y (nx1368), .A0 (wdata_8__dup_390), .A1 (
                       wenable_dup_397), .B0 (buffers3_8__dup_1254), .B1 (nx1179
                       ), .C0 (nx1262)) ;
    OAI21_X0P5M_A12TS ix848 (.Y (nx1262), .A0 (nx1369), .A1 (nx1342), .B0 (
                      nx1370)) ;
    INV_X0P5B_A12TS ix850 (.Y (nx1369), .A (buffers2_8__dup_1255)) ;
    AOI22_X0P5M_A12TS ix851 (.Y (nx1370), .A0 (buffers1_8__dup_1258), .A1 (
                      nx1178), .B0 (buffers0_8__dup_1256), .B1 (nx1177)) ;
    OAI22_X0P5M_A12TS ix852 (.Y (nx1261), .A0 (nx1365), .A1 (nx1159), .B0 (
                      nx1368), .B1 (nx1347)) ;
    OAI22_X0P5M_A12TS ix853 (.Y (nx1259), .A0 (nx1365), .A1 (nx1170), .B0 (
                      nx1368), .B1 (nx1348)) ;
    OAI22_X0P5M_A12TS ix854 (.Y (nx1257), .A0 (nx1365), .A1 (nx1163), .B0 (
                      nx1368), .B1 (nx1350)) ;
    OAI22_X0P5M_A12TS ix855 (.Y (nx1253), .A0 (nx1371), .A1 (nx1156), .B0 (
                      nx1374), .B1 (nx1346)) ;
    AOI221_X0P5M_A12TS ix856 (.Y (nx1371), .A0 (buffers3_7__dup_1244), .A1 (
                       nx1175), .B0 (buffers2_7__dup_1245), .B1 (nx1161), .C0 (
                       nx1250)) ;
    OAI22_X0P5M_A12TS ix857 (.Y (nx1250), .A0 (nx1372), .A1 (nx1326), .B0 (
                      nx1373), .B1 (nx1329)) ;
    INV_X0P5B_A12TS ix858 (.Y (nx1372), .A (buffers1_7__dup_1248)) ;
    INV_X0P5B_A12TS ix859 (.Y (nx1373), .A (buffers0_7__dup_1246)) ;
    AOI221_X0P5M_A12TS ix860 (.Y (nx1374), .A0 (wdata_7__dup_391), .A1 (
                       wenable_dup_397), .B0 (buffers3_7__dup_1244), .B1 (nx1179
                       ), .C0 (nx1252)) ;
    OAI21_X0P5M_A12TS ix861 (.Y (nx1252), .A0 (nx1375), .A1 (nx1342), .B0 (
                      nx1376)) ;
    INV_X0P5B_A12TS ix862 (.Y (nx1375), .A (buffers2_7__dup_1245)) ;
    AOI22_X0P5M_A12TS ix863 (.Y (nx1376), .A0 (buffers1_7__dup_1248), .A1 (
                      nx1178), .B0 (buffers0_7__dup_1246), .B1 (nx1177)) ;
    OAI22_X0P5M_A12TS ix864 (.Y (nx1251), .A0 (nx1371), .A1 (nx1159), .B0 (
                      nx1374), .B1 (nx1347)) ;
    OAI22_X0P5M_A12TS ix866 (.Y (nx1249), .A0 (nx1371), .A1 (nx1170), .B0 (
                      nx1374), .B1 (nx1348)) ;
    OAI22_X0P5M_A12TS ix867 (.Y (nx1247), .A0 (nx1371), .A1 (nx1163), .B0 (
                      nx1374), .B1 (nx1350)) ;
    OAI22_X0P5M_A12TS ix868 (.Y (nx1243), .A0 (nx1377), .A1 (nx1156), .B0 (
                      nx1380), .B1 (nx1346)) ;
    AOI221_X0P5M_A12TS ix869 (.Y (nx1377), .A0 (buffers3_6__dup_1234), .A1 (
                       nx1175), .B0 (buffers2_6__dup_1235), .B1 (nx1161), .C0 (
                       nx1240)) ;
    OAI22_X0P5M_A12TS ix870 (.Y (nx1240), .A0 (nx1378), .A1 (nx1326), .B0 (
                      nx1379), .B1 (nx1329)) ;
    INV_X0P5B_A12TS ix871 (.Y (nx1378), .A (buffers1_6__dup_1238)) ;
    INV_X0P5B_A12TS ix872 (.Y (nx1379), .A (buffers0_6__dup_1236)) ;
    AOI221_X0P5M_A12TS ix873 (.Y (nx1380), .A0 (wdata_6__dup_392), .A1 (
                       wenable_dup_397), .B0 (buffers3_6__dup_1234), .B1 (nx1179
                       ), .C0 (nx1242)) ;
    OAI21_X0P5M_A12TS ix874 (.Y (nx1242), .A0 (nx1381), .A1 (nx1342), .B0 (
                      nx1382)) ;
    INV_X0P5B_A12TS ix876 (.Y (nx1381), .A (buffers2_6__dup_1235)) ;
    AOI22_X0P5M_A12TS ix877 (.Y (nx1382), .A0 (buffers1_6__dup_1238), .A1 (
                      nx1178), .B0 (buffers0_6__dup_1236), .B1 (nx1177)) ;
    OAI22_X0P5M_A12TS ix878 (.Y (nx1241), .A0 (nx1377), .A1 (nx1159), .B0 (
                      nx1380), .B1 (nx1347)) ;
    OAI22_X0P5M_A12TS ix879 (.Y (nx1239), .A0 (nx1377), .A1 (nx1170), .B0 (
                      nx1380), .B1 (nx1348)) ;
    OAI22_X0P5M_A12TS ix880 (.Y (nx1237), .A0 (nx1377), .A1 (nx1163), .B0 (
                      nx1380), .B1 (nx1350)) ;
    OAI22_X0P5M_A12TS ix881 (.Y (nx1233), .A0 (nx1383), .A1 (nx1156), .B0 (
                      nx1386), .B1 (nx1346)) ;
    AOI221_X0P5M_A12TS ix882 (.Y (nx1383), .A0 (buffers3_5__dup_1224), .A1 (
                       nx1175), .B0 (buffers2_5__dup_1225), .B1 (nx1161), .C0 (
                       nx1230)) ;
    OAI22_X0P5M_A12TS ix883 (.Y (nx1230), .A0 (nx1384), .A1 (nx1326), .B0 (
                      nx1385), .B1 (nx1329)) ;
    INV_X0P5B_A12TS ix884 (.Y (nx1384), .A (buffers1_5__dup_1228)) ;
    INV_X0P5B_A12TS ix885 (.Y (nx1385), .A (buffers0_5__dup_1226)) ;
    AOI221_X0P5M_A12TS ix886 (.Y (nx1386), .A0 (wdata_5__dup_393), .A1 (
                       wenable_dup_397), .B0 (buffers3_5__dup_1224), .B1 (nx1179
                       ), .C0 (nx1232)) ;
    OAI21_X0P5M_A12TS ix887 (.Y (nx1232), .A0 (nx1387), .A1 (nx1342), .B0 (
                      nx1389)) ;
    INV_X0P5B_A12TS ix888 (.Y (nx1387), .A (buffers2_5__dup_1225)) ;
    AOI22_X0P5M_A12TS ix890 (.Y (nx1389), .A0 (buffers1_5__dup_1228), .A1 (
                      nx1178), .B0 (buffers0_5__dup_1226), .B1 (nx1177)) ;
    OAI22_X0P5M_A12TS ix891 (.Y (nx1231), .A0 (nx1383), .A1 (nx1159), .B0 (
                      nx1386), .B1 (nx1347)) ;
    OAI22_X0P5M_A12TS ix892 (.Y (nx1229), .A0 (nx1383), .A1 (nx1170), .B0 (
                      nx1386), .B1 (nx1348)) ;
    OAI22_X0P5M_A12TS ix893 (.Y (nx1227), .A0 (nx1383), .A1 (nx1163), .B0 (
                      nx1386), .B1 (nx1350)) ;
    OAI22_X0P5M_A12TS ix894 (.Y (nx1223), .A0 (nx1390), .A1 (nx1156), .B0 (
                      nx1393), .B1 (nx1346)) ;
    AOI221_X0P5M_A12TS ix895 (.Y (nx1390), .A0 (buffers3_4__dup_1214), .A1 (
                       nx1175), .B0 (buffers2_4__dup_1215), .B1 (nx1161), .C0 (
                       nx1220)) ;
    OAI22_X0P5M_A12TS ix896 (.Y (nx1220), .A0 (nx1391), .A1 (nx1326), .B0 (
                      nx1392), .B1 (nx1329)) ;
    INV_X0P5B_A12TS ix897 (.Y (nx1391), .A (buffers1_4__dup_1218)) ;
    INV_X0P5B_A12TS ix898 (.Y (nx1392), .A (buffers0_4__dup_1216)) ;
    AOI221_X0P5M_A12TS ix899 (.Y (nx1393), .A0 (wdata_4__dup_394), .A1 (
                       wenable_dup_397), .B0 (buffers3_4__dup_1214), .B1 (nx1179
                       ), .C0 (nx1222)) ;
    OAI21_X0P5M_A12TS ix900 (.Y (nx1222), .A0 (nx1394), .A1 (nx1342), .B0 (
                      nx1395)) ;
    INV_X0P5B_A12TS ix901 (.Y (nx1394), .A (buffers2_4__dup_1215)) ;
    AOI22_X0P5M_A12TS ix902 (.Y (nx1395), .A0 (buffers1_4__dup_1218), .A1 (
                      nx1178), .B0 (buffers0_4__dup_1216), .B1 (nx1177)) ;
    OAI22_X0P5M_A12TS ix903 (.Y (nx1221), .A0 (nx1390), .A1 (nx1159), .B0 (
                      nx1393), .B1 (nx1347)) ;
    OAI22_X0P5M_A12TS ix904 (.Y (nx1219), .A0 (nx1390), .A1 (nx1170), .B0 (
                      nx1393), .B1 (nx1348)) ;
    OAI22_X0P5M_A12TS ix906 (.Y (nx1217), .A0 (nx1390), .A1 (nx1163), .B0 (
                      nx1393), .B1 (nx1350)) ;
    OAI22_X0P5M_A12TS ix907 (.Y (nx1213), .A0 (nx1396), .A1 (nx1156), .B0 (
                      nx1399), .B1 (nx1346)) ;
    AOI221_X0P5M_A12TS ix908 (.Y (nx1396), .A0 (buffers3_3__dup_1203), .A1 (
                       nx1175), .B0 (buffers2_3__dup_1204), .B1 (nx1161), .C0 (
                       nx1209)) ;
    OAI22_X0P5M_A12TS ix909 (.Y (nx1209), .A0 (nx1397), .A1 (nx1326), .B0 (
                      nx1398), .B1 (nx1329)) ;
    INV_X0P5B_A12TS ix910 (.Y (nx1397), .A (buffers1_3__dup_1207)) ;
    INV_X0P5B_A12TS ix911 (.Y (nx1398), .A (buffers0_3__dup_1205)) ;
    AOI221_X0P5M_A12TS ix912 (.Y (nx1399), .A0 (wdata_3__dup_395), .A1 (
                       wenable_dup_397), .B0 (buffers3_3__dup_1203), .B1 (nx1179
                       ), .C0 (nx1211)) ;
    OAI21_X0P5M_A12TS ix913 (.Y (nx1211), .A0 (nx1400), .A1 (nx1342), .B0 (
                      nx1401)) ;
    INV_X0P5B_A12TS ix914 (.Y (nx1400), .A (buffers2_3__dup_1204)) ;
    AOI22_X0P5M_A12TS ix915 (.Y (nx1401), .A0 (buffers1_3__dup_1207), .A1 (
                      nx1178), .B0 (buffers0_3__dup_1205), .B1 (nx1177)) ;
    OAI22_X0P5M_A12TS ix916 (.Y (nx1210), .A0 (nx1396), .A1 (nx1159), .B0 (
                      nx1399), .B1 (nx1347)) ;
    OAI22_X0P5M_A12TS ix917 (.Y (nx1208), .A0 (nx1396), .A1 (nx1170), .B0 (
                      nx1399), .B1 (nx1348)) ;
    OAI22_X0P5M_A12TS ix918 (.Y (nx1206), .A0 (nx1396), .A1 (nx1163), .B0 (
                      nx1399), .B1 (nx1350)) ;
    OAI22_X0P5M_A12TS ix919 (.Y (nx1202), .A0 (nx1402), .A1 (nx1156), .B0 (
                      nx1406), .B1 (nx1346)) ;
    AOI221_X0P5M_A12TS ix920 (.Y (nx1402), .A0 (buffers3_2__dup_1192), .A1 (
                       nx1175), .B0 (buffers2_2__dup_1193), .B1 (nx1161), .C0 (
                       nx1199)) ;
    OAI22_X0P5M_A12TS ix922 (.Y (nx1199), .A0 (nx1403), .A1 (nx1326), .B0 (
                      nx1405), .B1 (nx1329)) ;
    INV_X0P5B_A12TS ix923 (.Y (nx1403), .A (buffers1_2__dup_1196)) ;
    INV_X0P5B_A12TS ix924 (.Y (nx1405), .A (buffers0_2__dup_1194)) ;
    AOI221_X0P5M_A12TS ix925 (.Y (nx1406), .A0 (wdata_2__dup_396), .A1 (
                       wenable_dup_397), .B0 (buffers3_2__dup_1192), .B1 (nx1179
                       ), .C0 (nx1201)) ;
    OAI21_X0P5M_A12TS ix926 (.Y (nx1201), .A0 (nx1407), .A1 (nx1342), .B0 (
                      nx1408)) ;
    INV_X0P5B_A12TS ix927 (.Y (nx1407), .A (buffers2_2__dup_1193)) ;
    AOI22_X0P5M_A12TS ix928 (.Y (nx1408), .A0 (buffers1_2__dup_1196), .A1 (
                      nx1178), .B0 (buffers0_2__dup_1194), .B1 (nx1177)) ;
    OAI22_X0P5M_A12TS ix929 (.Y (nx1200), .A0 (nx1402), .A1 (nx1159), .B0 (
                      nx1406), .B1 (nx1347)) ;
    OAI22_X0P5M_A12TS ix930 (.Y (nx1197), .A0 (nx1402), .A1 (nx1170), .B0 (
                      nx1406), .B1 (nx1348)) ;
    OAI22_X0P5M_A12TS ix931 (.Y (nx1195), .A0 (nx1402), .A1 (nx1163), .B0 (
                      nx1406), .B1 (nx1350)) ;
    OAI211_X0P5M_A12TS ix932 (.Y (nx1191), .A0 (nx1409), .A1 (nx1155), .B0 (
                       nx1412), .C0 (nx1323)) ;
    INV_X0P5B_A12TS ix933 (.Y (nx1410), .A (buffers2_1__dup_1182)) ;
    AOI22_X0P5M_A12TS ix934 (.Y (nx1411), .A0 (buffers1_1__dup_1187), .A1 (
                      nx1178), .B0 (buffers0_1__dup_1183), .B1 (nx1177)) ;
    NAND2_X0P5A_A12TS ix935 (.Y (nx1412), .A (nx1184), .B (nx1155)) ;
    OAI211_X0P5M_A12TS ix936 (.Y (nx1184), .A0 (nx1413), .A1 (nx1313), .B0 (
                       nx1319), .C0 (nx1415)) ;
    INV_X0P5B_A12TS ix937 (.Y (nx1413), .A (buffers3_1__dup_1181)) ;
    AOI222_X0P5M_A12TS ix938 (.Y (nx1415), .A0 (buffers1_1__dup_1187), .A1 (
                       nx1174), .B0 (buffers0_1__dup_1183), .B1 (nx1168), .C0 (
                       buffers2_1__dup_1182), .C1 (nx1161)) ;
    OAI211_X0P5M_A12TS ix939 (.Y (nx1190), .A0 (nx1343), .A1 (nx1416), .B0 (
                       nx1418), .C0 (nx1323)) ;
    NOR3_X0P5A_A12TS ix940 (.Y (nx1417), .A (nx1314), .B (head_2__dup_1140), .C (
                     head_0__dup_1139)) ;
    NAND2_X0P5A_A12TS ix941 (.Y (nx1418), .A (nx1343), .B (nx1185)) ;
    OAI211_X0P5M_A12TS ix942 (.Y (nx1185), .A0 (nx1410), .A1 (nx1342), .B0 (
                       nx1419), .C0 (nx1411)) ;
    AOI22_X0P5M_A12TS ix943 (.Y (nx1419), .A0 (flitin_1[1]), .A1 (
                      wenable_dup_397), .B0 (buffers3_1__dup_1181), .B1 (nx1179)
                      ) ;
    OAI211_X0P5M_A12TS ix944 (.Y (nx1189), .A0 (nx1349), .A1 (nx1416), .B0 (
                       nx1420), .C0 (nx1323)) ;
    NAND2_X0P5A_A12TS ix945 (.Y (nx1420), .A (nx1349), .B (nx1185)) ;
    OAI211_X0P5M_A12TS ix946 (.Y (nx1186), .A0 (nx1351), .A1 (nx1416), .B0 (
                       nx1421), .C0 (nx1323)) ;
    NAND2_X0P5A_A12TS ix948 (.Y (nx1421), .A (nx1351), .B (nx1185)) ;
    OAI211_X0P5M_A12TS ix949 (.Y (nx1180), .A0 (nx1422), .A1 (nx1155), .B0 (
                       nx1425), .C0 (nx1323)) ;
    INV_X0P5B_A12TS ix950 (.Y (nx1423), .A (buffers2_0__dup_1158)) ;
    AOI22_X0P5M_A12TS ix951 (.Y (nx1424), .A0 (buffers1_0__dup_1169), .A1 (
                      nx1178), .B0 (buffers0_0__dup_1162), .B1 (nx1177)) ;
    NAND2_X0P5A_A12TS ix952 (.Y (nx1425), .A (nx1165), .B (nx1155)) ;
    OAI211_X0P5M_A12TS ix953 (.Y (nx1165), .A0 (nx1426), .A1 (nx1313), .B0 (
                       nx1319), .C0 (nx1427)) ;
    INV_X0P5B_A12TS ix954 (.Y (nx1426), .A (buffers3_0__dup_1138)) ;
    AOI222_X0P5M_A12TS ix955 (.Y (nx1427), .A0 (buffers1_0__dup_1169), .A1 (
                       nx1174), .B0 (buffers0_0__dup_1162), .B1 (nx1168), .C0 (
                       buffers2_0__dup_1158), .C1 (nx1161)) ;
    OAI211_X0P5M_A12TS ix956 (.Y (nx1176), .A0 (nx1343), .A1 (nx1429), .B0 (
                       nx1430), .C0 (nx1323)) ;
    NAND2_X0P5A_A12TS ix958 (.Y (nx1430), .A (nx1343), .B (nx1166)) ;
    OAI211_X0P5M_A12TS ix959 (.Y (nx1166), .A0 (nx1423), .A1 (nx1342), .B0 (
                       nx1431), .C0 (nx1424)) ;
    AOI22_X0P5M_A12TS ix960 (.Y (nx1431), .A0 (flitin_1[0]), .A1 (
                      wenable_dup_397), .B0 (buffers3_0__dup_1138), .B1 (nx1179)
                      ) ;
    OAI211_X0P5M_A12TS ix961 (.Y (nx1173), .A0 (nx1349), .A1 (nx1429), .B0 (
                       nx1432), .C0 (nx1323)) ;
    NAND2_X0P5A_A12TS ix962 (.Y (nx1432), .A (nx1349), .B (nx1166)) ;
    NOR2_X0P5A_A12TS ix963 (.Y (nx1433), .A (nx1349), .B (reset)) ;
    OAI211_X0P5M_A12TS ix964 (.Y (nx1167), .A0 (nx1351), .A1 (nx1429), .B0 (
                       nx1434), .C0 (nx1323)) ;
    NAND2_X0P5A_A12TS ix965 (.Y (nx1434), .A (nx1351), .B (nx1166)) ;
    NOR2_X0P5A_A12TS ix966 (.Y (nx1435), .A (nx1351), .B (reset)) ;
    NOR2_X0P5A_A12TS ix967 (.Y (nx1436), .A (nx1343), .B (reset)) ;
    TIELO_X1M_A12TS ix968 (.Y (nx1137)) ;
    NAND2_X0P5A_A12TS ix969 (.Y (headflit_0__dup_268), .A (nx1437), .B (nx1438)
                      ) ;
    AOI22_X0P5M_A12TS ix971 (.Y (nx1437), .A0 (buffers0_0__dup_1162), .A1 (
                      nx1330), .B0 (buffers1_0__dup_1169), .B1 (nx1327)) ;
    AOI22_X0P5M_A12TS ix972 (.Y (nx1438), .A0 (buffers2_0__dup_1158), .A1 (
                      nx1417), .B0 (buffers3_0__dup_1138), .B1 (nx1146)) ;
    NAND2_X0P5A_A12TS ix975 (.Y (headflit_1__dup_267), .A (nx1439), .B (nx1440)
                      ) ;
    AOI22_X0P5M_A12TS ix977 (.Y (nx1439), .A0 (buffers0_1__dup_1183), .A1 (
                      nx1330), .B0 (buffers1_1__dup_1187), .B1 (nx1327)) ;
    AOI22_X0P5M_A12TS ix979 (.Y (nx1440), .A0 (buffers2_1__dup_1182), .A1 (
                      nx1417), .B0 (buffers3_1__dup_1181), .B1 (nx1146)) ;
    NAND2_X0P5A_A12TS ix981 (.Y (headflit_2__dup_266), .A (nx1441), .B (nx1442)
                      ) ;
    AOI22_X0P5M_A12TS ix982 (.Y (nx1441), .A0 (buffers0_2__dup_1194), .A1 (
                      nx1330), .B0 (buffers1_2__dup_1196), .B1 (nx1327)) ;
    AOI22_X0P5M_A12TS ix985 (.Y (nx1442), .A0 (buffers2_2__dup_1193), .A1 (
                      nx1417), .B0 (buffers3_2__dup_1192), .B1 (nx1146)) ;
    NAND2_X0P5A_A12TS ix987 (.Y (headflit_3__dup_265), .A (nx1443), .B (nx1445)
                      ) ;
    AOI22_X0P5M_A12TS ix988 (.Y (nx1443), .A0 (buffers0_3__dup_1205), .A1 (
                      nx1330), .B0 (buffers1_3__dup_1207), .B1 (nx1327)) ;
    AOI22_X0P5M_A12TS ix989 (.Y (nx1445), .A0 (buffers2_3__dup_1204), .A1 (
                      nx1417), .B0 (buffers3_3__dup_1203), .B1 (nx1146)) ;
    NAND2_X0P5A_A12TS ix991 (.Y (headflit_4__dup_264), .A (nx1446), .B (nx1447)
                      ) ;
    AOI22_X0P5M_A12TS ix992 (.Y (nx1446), .A0 (buffers0_4__dup_1216), .A1 (
                      nx1330), .B0 (buffers1_4__dup_1218), .B1 (nx1327)) ;
    AOI22_X0P5M_A12TS ix994 (.Y (nx1447), .A0 (buffers2_4__dup_1215), .A1 (
                      nx1417), .B0 (buffers3_4__dup_1214), .B1 (nx1146)) ;
    NAND2_X0P5A_A12TS ix996 (.Y (headflit_5__dup_263), .A (nx1448), .B (nx1449)
                      ) ;
    AOI22_X0P5M_A12TS ix998 (.Y (nx1448), .A0 (buffers0_5__dup_1226), .A1 (
                      nx1330), .B0 (buffers1_5__dup_1228), .B1 (nx1327)) ;
    AOI22_X0P5M_A12TS ix999 (.Y (nx1449), .A0 (buffers2_5__dup_1225), .A1 (
                      nx1417), .B0 (buffers3_5__dup_1224), .B1 (nx1146)) ;
    NAND2_X0P5A_A12TS ix1001 (.Y (headflit_6__dup_262), .A (nx1450), .B (nx1451)
                      ) ;
    AOI22_X0P5M_A12TS ix1003 (.Y (nx1450), .A0 (buffers0_6__dup_1236), .A1 (
                      nx1330), .B0 (buffers1_6__dup_1238), .B1 (nx1327)) ;
    AOI22_X0P5M_A12TS ix1004 (.Y (nx1451), .A0 (buffers2_6__dup_1235), .A1 (
                      nx1417), .B0 (buffers3_6__dup_1234), .B1 (nx1146)) ;
    NAND2_X0P5A_A12TS ix1006 (.Y (headflit_7__dup_261), .A (nx1452), .B (nx1453)
                      ) ;
    AOI22_X0P5M_A12TS ix1007 (.Y (nx1452), .A0 (buffers0_7__dup_1246), .A1 (
                      nx1330), .B0 (buffers1_7__dup_1248), .B1 (nx1327)) ;
    AOI22_X0P5M_A12TS ix1009 (.Y (nx1453), .A0 (buffers2_7__dup_1245), .A1 (
                      nx1417), .B0 (buffers3_7__dup_1244), .B1 (nx1146)) ;
    NAND2_X0P5A_A12TS ix1011 (.Y (headflit_8__dup_366), .A (nx1454), .B (nx1455)
                      ) ;
    AOI22_X0P5M_A12TS ix1015 (.Y (nx1454), .A0 (buffers0_8__dup_1256), .A1 (
                      nx1330), .B0 (buffers1_8__dup_1258), .B1 (nx1327)) ;
    AOI22_X0P5M_A12TS ix1016 (.Y (nx1455), .A0 (buffers2_8__dup_1255), .A1 (
                      nx1417), .B0 (buffers3_8__dup_1254), .B1 (nx1146)) ;
    NAND2_X0P5A_A12TS ix1017 (.Y (headflit_9__dup_365), .A (nx1456), .B (nx1457)
                      ) ;
    AOI22_X0P5M_A12TS ix1018 (.Y (nx1456), .A0 (buffers0_9__dup_1266), .A1 (
                      nx1330), .B0 (buffers1_9__dup_1268), .B1 (nx1327)) ;
    AOI22_X0P5M_A12TS ix1020 (.Y (nx1457), .A0 (buffers2_9__dup_1265), .A1 (
                      nx1417), .B0 (buffers3_9__dup_1264), .B1 (nx1146)) ;
    NAND2_X0P5A_A12TS ix1021 (.Y (headflit_10__dup_364), .A (nx1458), .B (nx1459
                      )) ;
    AOI22_X0P5M_A12TS ix1023 (.Y (nx1458), .A0 (buffers0_10__dup_1276), .A1 (
                      nx1330), .B0 (buffers1_10__dup_1278), .B1 (nx1327)) ;
    AOI22_X0P5M_A12TS ix1024 (.Y (nx1459), .A0 (buffers2_10__dup_1275), .A1 (
                      nx1417), .B0 (buffers3_10__dup_1274), .B1 (nx1146)) ;
    NAND2_X0P5A_A12TS ix1025 (.Y (headflit_11__dup_363), .A (nx1461), .B (nx1462
                      )) ;
    AOI22_X0P5M_A12TS ix1027 (.Y (nx1461), .A0 (buffers0_11__dup_1287), .A1 (
                      nx1330), .B0 (buffers1_11__dup_1289), .B1 (nx1327)) ;
    AOI22_X0P5M_A12TS ix1031 (.Y (nx1462), .A0 (buffers2_11__dup_1286), .A1 (
                      nx1417), .B0 (buffers3_11__dup_1285), .B1 (nx1146)) ;
    DFFQ_X0P5M_A12TS reg_tailout_dup_5501 (.Q (tailout_1), .CK (clk), .D (nx1311
                     )) ;
    NOR3_X0P5A_A12TS ix1033 (.Y (nx1311), .A (nx1463), .B (nx1464), .C (
                     headflit_1__dup_267)) ;
    NAND2_X0P5A_A12TS ix1035 (.Y (nx1463), .A (debitout_1), .B (nx1323)) ;
    DFFQ_X0P5M_A12TS reg_rdata_0__dup_5504 (.Q (rdata_0__dup_386), .CK (clk), .D (
                     nx1295)) ;
    OAI211_X0P5M_A12TS ix1037 (.Y (nx1295), .A0 (debitout_1), .A1 (nx1465), .B0 (
                       nx1466), .C0 (nx1323)) ;
    INV_X0P5B_A12TS ix1038 (.Y (nx1465), .A (rdata_0__dup_386)) ;
    NAND2_X0P5A_A12TS ix1039 (.Y (nx1466), .A (debitout_1), .B (
                      headflit_0__dup_268)) ;
    DFFQ_X0P5M_A12TS reg_rdata_1__dup_5508 (.Q (rdata_1__dup_385), .CK (clk), .D (
                     nx1297)) ;
    OAI211_X0P5M_A12TS ix1040 (.Y (nx1297), .A0 (debitout_1), .A1 (nx1467), .B0 (
                       nx1468), .C0 (nx1323)) ;
    INV_X0P5B_A12TS ix1041 (.Y (nx1467), .A (rdata_1__dup_385)) ;
    NAND2_X0P5A_A12TS ix1043 (.Y (nx1468), .A (debitout_1), .B (
                      headflit_1__dup_267)) ;
    DFFQ_X0P5M_A12TS reg_rdata_2__dup_5512 (.Q (rdata_2__dup_384), .CK (clk), .D (
                     nx1300)) ;
    INV_X0P5B_A12TS ix1044 (.Y (nx1300), .A (nx1469)) ;
    AOI22_X0P5M_A12TS ix1045 (.Y (nx1469), .A0 (rdata_2__dup_384), .A1 (nx1298)
                      , .B0 (headflit_2__dup_266), .B1 (nx1299)) ;
    NOR2_X0P5A_A12TS ix1047 (.Y (nx1298), .A (debitout_1), .B (reset)) ;
    DFFQ_X0P5M_A12TS reg_rdata_3__dup_5516 (.Q (rdata_3__dup_383), .CK (clk), .D (
                     nx1301)) ;
    INV_X0P5B_A12TS ix1049 (.Y (nx1301), .A (nx1470)) ;
    AOI22_X0P5M_A12TS ix1050 (.Y (nx1470), .A0 (rdata_3__dup_383), .A1 (nx1298)
                      , .B0 (headflit_3__dup_265), .B1 (nx1299)) ;
    DFFQ_X0P5M_A12TS reg_rdata_4__dup_5519 (.Q (rdata_4__dup_382), .CK (clk), .D (
                     nx1302)) ;
    INV_X0P5B_A12TS ix1051 (.Y (nx1302), .A (nx1471)) ;
    AOI22_X0P5M_A12TS ix1053 (.Y (nx1471), .A0 (rdata_4__dup_382), .A1 (nx1298)
                      , .B0 (headflit_4__dup_264), .B1 (nx1299)) ;
    DFFQ_X0P5M_A12TS reg_rdata_5__dup_5522 (.Q (rdata_5__dup_381), .CK (clk), .D (
                     nx1303)) ;
    INV_X0P5B_A12TS ix1054 (.Y (nx1303), .A (nx1472)) ;
    AOI22_X0P5M_A12TS ix1056 (.Y (nx1472), .A0 (rdata_5__dup_381), .A1 (nx1298)
                      , .B0 (headflit_5__dup_263), .B1 (nx1299)) ;
    DFFQ_X0P5M_A12TS reg_rdata_6__dup_5525 (.Q (rdata_6__dup_380), .CK (clk), .D (
                     nx1304)) ;
    INV_X0P5B_A12TS ix1057 (.Y (nx1304), .A (nx1473)) ;
    AOI22_X0P5M_A12TS ix1058 (.Y (nx1473), .A0 (rdata_6__dup_380), .A1 (nx1298)
                      , .B0 (headflit_6__dup_262), .B1 (nx1299)) ;
    DFFQ_X0P5M_A12TS reg_rdata_7__dup_5528 (.Q (rdata_7__dup_379), .CK (clk), .D (
                     nx1305)) ;
    INV_X0P5B_A12TS ix1059 (.Y (nx1305), .A (nx1474)) ;
    AOI22_X0P5M_A12TS ix1060 (.Y (nx1474), .A0 (rdata_7__dup_379), .A1 (nx1298)
                      , .B0 (headflit_7__dup_261), .B1 (nx1299)) ;
    DFFQ_X0P5M_A12TS reg_rdata_8__dup_5531 (.Q (rdata_8__dup_378), .CK (clk), .D (
                     nx1307)) ;
    INV_X0P5B_A12TS ix1061 (.Y (nx1307), .A (nx1475)) ;
    AOI22_X0P5M_A12TS ix1062 (.Y (nx1475), .A0 (rdata_8__dup_378), .A1 (nx1298)
                      , .B0 (headflit_8__dup_366), .B1 (nx1299)) ;
    DFFQ_X0P5M_A12TS reg_rdata_9__dup_5534 (.Q (rdata_9__dup_377), .CK (clk), .D (
                     nx1308)) ;
    INV_X0P5B_A12TS ix1063 (.Y (nx1308), .A (nx1476)) ;
    AOI22_X0P5M_A12TS ix1064 (.Y (nx1476), .A0 (rdata_9__dup_377), .A1 (nx1298)
                      , .B0 (headflit_9__dup_365), .B1 (nx1299)) ;
    DFFQ_X0P5M_A12TS reg_rdata_10__dup_5537 (.Q (rdata_10__dup_376), .CK (clk), 
                     .D (nx1309)) ;
    INV_X0P5B_A12TS ix1066 (.Y (nx1309), .A (nx1477)) ;
    AOI22_X0P5M_A12TS ix1067 (.Y (nx1477), .A0 (rdata_10__dup_376), .A1 (nx1298)
                      , .B0 (headflit_10__dup_364), .B1 (nx1299)) ;
    DFFQ_X0P5M_A12TS reg_rdata_11__dup_5540 (.Q (rdata_11__dup_375), .CK (clk), 
                     .D (nx1310)) ;
    INV_X0P5B_A12TS ix1068 (.Y (nx1310), .A (nx1478)) ;
    AOI22_X0P5M_A12TS ix1069 (.Y (nx1478), .A0 (rdata_11__dup_375), .A1 (nx1298)
                      , .B0 (headflit_11__dup_363), .B1 (nx1299)) ;
    INV_X0P5B_A12TS ix1070 (.Y (nx1299), .A (nx1463)) ;
    INV_X0P5B_A12TS ix1071 (.Y (nx1409), .A (nx1185)) ;
    INV_X0P5B_A12TS ix1072 (.Y (nx1416), .A (nx1184)) ;
    INV_X0P5B_A12TS ix1073 (.Y (nx1174), .A (nx1326)) ;
    INV_X0P5B_A12TS ix1074 (.Y (nx1170), .A (nx1433)) ;
    INV_X0P5B_A12TS ix1075 (.Y (nx1168), .A (nx1329)) ;
    INV_X0P5B_A12TS ix1076 (.Y (nx1422), .A (nx1166)) ;
    INV_X0P5B_A12TS ix1077 (.Y (nx1429), .A (nx1165)) ;
    INV_X0P5B_A12TS ix1078 (.Y (nx1163), .A (nx1435)) ;
    INV_X0P5B_A12TS ix1079 (.Y (nx1159), .A (nx1436)) ;
    INV_X0P5B_A12TS ix1080 (.Y (nx1148), .A (nx1333)) ;
    INV_X0P5B_A12TS ix1082 (.Y (nx1146), .A (nx1313)) ;
    INV_X0P5B_A12TS ix1083 (.Y (nx1464), .A (headflit_0__dup_268)) ;
    NAND4B_X0P5M_A12TS ix1084 (.Y (nx1318), .AN (head_2__dup_1140), .B (
                       head_0__dup_1139), .C (debitout_1), .D (head_1__dup_1142)
                       ) ;
    NOR2B_X0P7M_A12TS ix1085 (.Y (nx1178), .AN (nx1349), .B (nx1333)) ;
    AND2_X0P5M_A12TS ix1086 (.Y (nx1177), .A (nx1351), .B (nx1333)) ;
    OR2_X0P5M_A12TS ix1087 (.Y (nx1346), .A (reset), .B (nx1155)) ;
    NOR2B_X0P7M_A12TS ix1088 (.Y (nx1161), .AN (nx1417), .B (debitout_1)) ;
    NAND2B_X0P7M_A12TS ix1089 (.Y (nx1171), .AN (nx1327), .B (nx1433)) ;
    NAND2B_X0P7M_A12TS ix1090 (.Y (nx1164), .AN (nx1330), .B (nx1435)) ;
    NAND2B_X0P7M_A12TS ix1092 (.Y (nx1160), .AN (nx1417), .B (nx1436)) ;
    NAND2B_X0P7M_A12TS ix1093 (.Y (nx1157), .AN (nx1156), .B (nx1313)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_11__dup_5565 (.Q (buffers1_11__dup_1289), 
                         .CK (clk), .D (nx1290), .R (nx1137), .SE (nx1481), .SI (
                         buffers1_11__dup_1289), .SN (nx1483)) ;
    INV_X0P5B_A12TS ix1094 (.Y (nx1481), .A (nx1171)) ;
    INV_X2M_A12TS ix1095 (.Y (nx1483), .A (nx1137)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_11__dup_5568 (.Q (buffers0_11__dup_1287), 
                         .CK (clk), .D (nx1288), .R (nx1137), .SE (nx1482), .SI (
                         buffers0_11__dup_1287), .SN (nx1483)) ;
    INV_X0P5B_A12TS ix1096 (.Y (nx1482), .A (nx1164)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_11__dup_5570 (.Q (buffers2_11__dup_1286), 
                         .CK (clk), .D (nx1292), .R (nx1137), .SE (nx1480), .SI (
                         buffers2_11__dup_1286), .SN (nx1483)) ;
    INV_X0P5B_A12TS ix1097 (.Y (nx1480), .A (nx1160)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_11__dup_5572 (.Q (buffers3_11__dup_1285), 
                         .CK (clk), .D (nx1294), .R (nx1137), .SE (nx1479), .SI (
                         buffers3_11__dup_1285), .SN (nx1483)) ;
    INV_X0P5B_A12TS ix1098 (.Y (nx1479), .A (nx1157)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_10__dup_5574 (.Q (buffers1_10__dup_1278), 
                         .CK (clk), .D (nx1279), .R (nx1137), .SE (nx1481), .SI (
                         buffers1_10__dup_1278), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_10__dup_5575 (.Q (buffers0_10__dup_1276), 
                         .CK (clk), .D (nx1277), .R (nx1137), .SE (nx1482), .SI (
                         buffers0_10__dup_1276), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_10__dup_5576 (.Q (buffers2_10__dup_1275), 
                         .CK (clk), .D (nx1282), .R (nx1137), .SE (nx1480), .SI (
                         buffers2_10__dup_1275), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_10__dup_5577 (.Q (buffers3_10__dup_1274), 
                         .CK (clk), .D (nx1284), .R (nx1137), .SE (nx1479), .SI (
                         buffers3_10__dup_1274), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_9__dup_5578 (.Q (buffers1_9__dup_1268), .CK (
                         clk), .D (nx1269), .R (nx1137), .SE (nx1481), .SI (
                         buffers1_9__dup_1268), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_9__dup_5579 (.Q (buffers0_9__dup_1266), .CK (
                         clk), .D (nx1267), .R (nx1137), .SE (nx1482), .SI (
                         buffers0_9__dup_1266), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_9__dup_5580 (.Q (buffers2_9__dup_1265), .CK (
                         clk), .D (nx1271), .R (nx1137), .SE (nx1480), .SI (
                         buffers2_9__dup_1265), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_9__dup_5581 (.Q (buffers3_9__dup_1264), .CK (
                         clk), .D (nx1273), .R (nx1137), .SE (nx1479), .SI (
                         buffers3_9__dup_1264), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_8__dup_5582 (.Q (buffers1_8__dup_1258), .CK (
                         clk), .D (nx1259), .R (nx1137), .SE (nx1481), .SI (
                         buffers1_8__dup_1258), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_8__dup_5583 (.Q (buffers0_8__dup_1256), .CK (
                         clk), .D (nx1257), .R (nx1137), .SE (nx1482), .SI (
                         buffers0_8__dup_1256), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_8__dup_5584 (.Q (buffers2_8__dup_1255), .CK (
                         clk), .D (nx1261), .R (nx1137), .SE (nx1480), .SI (
                         buffers2_8__dup_1255), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_8__dup_5585 (.Q (buffers3_8__dup_1254), .CK (
                         clk), .D (nx1263), .R (nx1137), .SE (nx1479), .SI (
                         buffers3_8__dup_1254), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_7__dup_5586 (.Q (buffers1_7__dup_1248), .CK (
                         clk), .D (nx1249), .R (nx1137), .SE (nx1481), .SI (
                         buffers1_7__dup_1248), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_7__dup_5587 (.Q (buffers0_7__dup_1246), .CK (
                         clk), .D (nx1247), .R (nx1137), .SE (nx1482), .SI (
                         buffers0_7__dup_1246), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_7__dup_5588 (.Q (buffers2_7__dup_1245), .CK (
                         clk), .D (nx1251), .R (nx1137), .SE (nx1480), .SI (
                         buffers2_7__dup_1245), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_7__dup_5589 (.Q (buffers3_7__dup_1244), .CK (
                         clk), .D (nx1253), .R (nx1137), .SE (nx1479), .SI (
                         buffers3_7__dup_1244), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_6__dup_5590 (.Q (buffers1_6__dup_1238), .CK (
                         clk), .D (nx1239), .R (nx1137), .SE (nx1481), .SI (
                         buffers1_6__dup_1238), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_6__dup_5591 (.Q (buffers0_6__dup_1236), .CK (
                         clk), .D (nx1237), .R (nx1137), .SE (nx1482), .SI (
                         buffers0_6__dup_1236), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_6__dup_5592 (.Q (buffers2_6__dup_1235), .CK (
                         clk), .D (nx1241), .R (nx1137), .SE (nx1480), .SI (
                         buffers2_6__dup_1235), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_6__dup_5593 (.Q (buffers3_6__dup_1234), .CK (
                         clk), .D (nx1243), .R (nx1137), .SE (nx1479), .SI (
                         buffers3_6__dup_1234), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_5__dup_5594 (.Q (buffers1_5__dup_1228), .CK (
                         clk), .D (nx1229), .R (nx1137), .SE (nx1481), .SI (
                         buffers1_5__dup_1228), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_5__dup_5595 (.Q (buffers0_5__dup_1226), .CK (
                         clk), .D (nx1227), .R (nx1137), .SE (nx1482), .SI (
                         buffers0_5__dup_1226), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_5__dup_5596 (.Q (buffers2_5__dup_1225), .CK (
                         clk), .D (nx1231), .R (nx1137), .SE (nx1480), .SI (
                         buffers2_5__dup_1225), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_5__dup_5597 (.Q (buffers3_5__dup_1224), .CK (
                         clk), .D (nx1233), .R (nx1137), .SE (nx1479), .SI (
                         buffers3_5__dup_1224), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_4__dup_5598 (.Q (buffers1_4__dup_1218), .CK (
                         clk), .D (nx1219), .R (nx1137), .SE (nx1481), .SI (
                         buffers1_4__dup_1218), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_4__dup_5599 (.Q (buffers0_4__dup_1216), .CK (
                         clk), .D (nx1217), .R (nx1137), .SE (nx1482), .SI (
                         buffers0_4__dup_1216), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_4__dup_5600 (.Q (buffers2_4__dup_1215), .CK (
                         clk), .D (nx1221), .R (nx1137), .SE (nx1480), .SI (
                         buffers2_4__dup_1215), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_4__dup_5601 (.Q (buffers3_4__dup_1214), .CK (
                         clk), .D (nx1223), .R (nx1137), .SE (nx1479), .SI (
                         buffers3_4__dup_1214), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_3__dup_5602 (.Q (buffers1_3__dup_1207), .CK (
                         clk), .D (nx1208), .R (nx1137), .SE (nx1481), .SI (
                         buffers1_3__dup_1207), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_3__dup_5603 (.Q (buffers0_3__dup_1205), .CK (
                         clk), .D (nx1206), .R (nx1137), .SE (nx1482), .SI (
                         buffers0_3__dup_1205), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_3__dup_5604 (.Q (buffers2_3__dup_1204), .CK (
                         clk), .D (nx1210), .R (nx1137), .SE (nx1480), .SI (
                         buffers2_3__dup_1204), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_3__dup_5605 (.Q (buffers3_3__dup_1203), .CK (
                         clk), .D (nx1213), .R (nx1137), .SE (nx1479), .SI (
                         buffers3_3__dup_1203), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_2__dup_5606 (.Q (buffers1_2__dup_1196), .CK (
                         clk), .D (nx1197), .R (nx1137), .SE (nx1481), .SI (
                         buffers1_2__dup_1196), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_2__dup_5607 (.Q (buffers0_2__dup_1194), .CK (
                         clk), .D (nx1195), .R (nx1137), .SE (nx1482), .SI (
                         buffers0_2__dup_1194), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_2__dup_5608 (.Q (buffers2_2__dup_1193), .CK (
                         clk), .D (nx1200), .R (nx1137), .SE (nx1480), .SI (
                         buffers2_2__dup_1193), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_2__dup_5609 (.Q (buffers3_2__dup_1192), .CK (
                         clk), .D (nx1202), .R (nx1137), .SE (nx1479), .SI (
                         buffers3_2__dup_1192), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_1__dup_5610 (.Q (buffers1_1__dup_1187), .CK (
                         clk), .D (nx1189), .R (nx1137), .SE (nx1481), .SI (
                         buffers1_1__dup_1187), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_1__dup_5611 (.Q (buffers0_1__dup_1183), .CK (
                         clk), .D (nx1186), .R (nx1137), .SE (nx1482), .SI (
                         buffers0_1__dup_1183), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_1__dup_5612 (.Q (buffers2_1__dup_1182), .CK (
                         clk), .D (nx1190), .R (nx1137), .SE (nx1480), .SI (
                         buffers2_1__dup_1182), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_1__dup_5613 (.Q (buffers3_1__dup_1181), .CK (
                         clk), .D (nx1191), .R (nx1137), .SE (nx1479), .SI (
                         buffers3_1__dup_1181), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers1_0__dup_5614 (.Q (buffers1_0__dup_1169), .CK (
                         clk), .D (nx1173), .R (nx1137), .SE (nx1481), .SI (
                         buffers1_0__dup_1169), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers0_0__dup_5615 (.Q (buffers0_0__dup_1162), .CK (
                         clk), .D (nx1167), .R (nx1137), .SE (nx1482), .SI (
                         buffers0_0__dup_1162), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers2_0__dup_5616 (.Q (buffers2_0__dup_1158), .CK (
                         clk), .D (nx1176), .R (nx1137), .SE (nx1480), .SI (
                         buffers2_0__dup_1158), .SN (nx1483)) ;
    SDFFSRPQ_X0P5M_A12TS reg_buffers3_0__dup_5617 (.Q (buffers3_0__dup_1138), .CK (
                         clk), .D (nx1180), .R (nx1137), .SE (nx1479), .SI (
                         buffers3_0__dup_1138), .SN (nx1483)) ;
    MXT2_X0P5M_A12TS ix7 (.Y (int01), .A (flitout_switch_0_0), .B (
                     flitout_switch_1_0), .S0 (colsel0reg_0)) ;
    MXT2_X0P5M_A12TS ix1507 (.Y (flitout_0[0]), .A (int01), .B (PWR), .S0 (
                     colsel0reg_1)) ;
    MXT2_X0P5M_A12TS ix1508 (.Y (int01_dup_1484), .A (flitout_switch_0_0), .B (
                     flitout_switch_1_0), .S0 (colsel1reg_0)) ;
    MXT2_X0P5M_A12TS ix1509 (.Y (flitout_1[0]), .A (int01_dup_1484), .B (PWR), .S0 (
                     colsel1reg_1)) ;
    MXT2_X0P5M_A12TS ix1510 (.Y (int01_dup_1485), .A (flitout_switch_0_1), .B (
                     flitout_switch_1_1), .S0 (colsel0reg_0)) ;
    MXT2_X0P5M_A12TS ix1511 (.Y (flitout_0[1]), .A (int01_dup_1485), .B (PWR), .S0 (
                     colsel0reg_1)) ;
    MXT2_X0P5M_A12TS ix1512 (.Y (int01_dup_1486), .A (flitout_switch_0_1), .B (
                     flitout_switch_1_1), .S0 (colsel1reg_0)) ;
    MXT2_X0P5M_A12TS ix1514 (.Y (flitout_1[1]), .A (int01_dup_1486), .B (PWR), .S0 (
                     colsel1reg_1)) ;
    MXT2_X0P5M_A12TS ix1515 (.Y (int01_dup_1487), .A (flitout_switch_0_2), .B (
                     flitout_switch_1_2), .S0 (colsel0reg_0)) ;
    MXT2_X0P5M_A12TS ix1516 (.Y (flitout_0[2]), .A (int01_dup_1487), .B (
                     GND_dup_287), .S0 (colsel0reg_1)) ;
    MXT2_X0P5M_A12TS ix1517 (.Y (int01_dup_1488), .A (flitout_switch_0_2), .B (
                     flitout_switch_1_2), .S0 (colsel1reg_0)) ;
    MXT2_X0P5M_A12TS ix1518 (.Y (flitout_1[2]), .A (int01_dup_1488), .B (
                     GND_dup_287), .S0 (colsel1reg_1)) ;
    MXT2_X0P5M_A12TS ix1519 (.Y (int01_dup_1489), .A (flitout_switch_0_3), .B (
                     flitout_switch_1_3), .S0 (colsel0reg_0)) ;
    MXT2_X0P5M_A12TS ix1520 (.Y (flitout_0[3]), .A (int01_dup_1489), .B (
                     GND_dup_287), .S0 (colsel0reg_1)) ;
    MXT2_X0P5M_A12TS ix1521 (.Y (int01_dup_1490), .A (flitout_switch_0_3), .B (
                     flitout_switch_1_3), .S0 (colsel1reg_0)) ;
    MXT2_X0P5M_A12TS ix1522 (.Y (flitout_1[3]), .A (int01_dup_1490), .B (
                     GND_dup_287), .S0 (colsel1reg_1)) ;
    MXT2_X0P5M_A12TS ix1524 (.Y (int01_dup_1491), .A (flitout_switch_0_4), .B (
                     flitout_switch_1_4), .S0 (colsel0reg_0)) ;
    MXT2_X0P5M_A12TS ix1525 (.Y (flitout_0[4]), .A (int01_dup_1491), .B (
                     GND_dup_287), .S0 (colsel0reg_1)) ;
    MXT2_X0P5M_A12TS ix1526 (.Y (int01_dup_1492), .A (flitout_switch_0_4), .B (
                     flitout_switch_1_4), .S0 (colsel1reg_0)) ;
    MXT2_X0P5M_A12TS ix1527 (.Y (flitout_1[4]), .A (int01_dup_1492), .B (
                     GND_dup_287), .S0 (colsel1reg_1)) ;
    MXT2_X0P5M_A12TS ix1528 (.Y (int01_dup_1493), .A (flitout_switch_0_5), .B (
                     flitout_switch_1_5), .S0 (colsel0reg_0)) ;
    MXT2_X0P5M_A12TS ix1529 (.Y (flitout_0[5]), .A (int01_dup_1493), .B (
                     GND_dup_287), .S0 (colsel0reg_1)) ;
    MXT2_X0P5M_A12TS ix1530 (.Y (int01_dup_1494), .A (flitout_switch_0_5), .B (
                     flitout_switch_1_5), .S0 (colsel1reg_0)) ;
    MXT2_X0P5M_A12TS ix1531 (.Y (flitout_1[5]), .A (int01_dup_1494), .B (
                     GND_dup_287), .S0 (colsel1reg_1)) ;
    MXT2_X0P5M_A12TS ix1532 (.Y (int01_dup_1495), .A (flitout_switch_0_6), .B (
                     flitout_switch_1_6), .S0 (colsel0reg_0)) ;
    MXT2_X0P5M_A12TS ix1533 (.Y (flitout_0[6]), .A (int01_dup_1495), .B (
                     GND_dup_287), .S0 (colsel0reg_1)) ;
    MXT2_X0P5M_A12TS ix1534 (.Y (int01_dup_1496), .A (flitout_switch_0_6), .B (
                     flitout_switch_1_6), .S0 (colsel1reg_0)) ;
    MXT2_X0P5M_A12TS ix1535 (.Y (flitout_1[6]), .A (int01_dup_1496), .B (
                     GND_dup_287), .S0 (colsel1reg_1)) ;
    MXT2_X0P5M_A12TS ix1536 (.Y (int01_dup_1497), .A (flitout_switch_0_7), .B (
                     flitout_switch_1_7), .S0 (colsel0reg_0)) ;
    MXT2_X0P5M_A12TS ix1538 (.Y (flitout_0[7]), .A (int01_dup_1497), .B (
                     GND_dup_287), .S0 (colsel0reg_1)) ;
    MXT2_X0P5M_A12TS ix1539 (.Y (int01_dup_1498), .A (flitout_switch_0_7), .B (
                     flitout_switch_1_7), .S0 (colsel1reg_0)) ;
    MXT2_X0P5M_A12TS ix1540 (.Y (flitout_1[7]), .A (int01_dup_1498), .B (
                     GND_dup_287), .S0 (colsel1reg_1)) ;
    MXT2_X0P5M_A12TS ix1541 (.Y (int01_dup_1499), .A (flitout_switch_0_8), .B (
                     flitout_switch_1_8), .S0 (colsel0reg_0)) ;
    MXT2_X0P5M_A12TS ix1542 (.Y (flitout_0[8]), .A (int01_dup_1499), .B (
                     GND_dup_287), .S0 (colsel0reg_1)) ;
    MXT2_X0P5M_A12TS ix1543 (.Y (int01_dup_1500), .A (flitout_switch_0_8), .B (
                     flitout_switch_1_8), .S0 (colsel1reg_0)) ;
    MXT2_X0P5M_A12TS ix1544 (.Y (flitout_1[8]), .A (int01_dup_1500), .B (
                     GND_dup_287), .S0 (colsel1reg_1)) ;
    MXT2_X0P5M_A12TS ix1545 (.Y (int01_dup_1501), .A (flitout_switch_0_9), .B (
                     flitout_switch_1_9), .S0 (colsel0reg_0)) ;
    MXT2_X0P5M_A12TS ix1546 (.Y (flitout_0[9]), .A (int01_dup_1501), .B (
                     GND_dup_287), .S0 (colsel0reg_1)) ;
    MXT2_X0P5M_A12TS ix1547 (.Y (int01_dup_1502), .A (flitout_switch_0_9), .B (
                     flitout_switch_1_9), .S0 (colsel1reg_0)) ;
    MXT2_X0P5M_A12TS ix1548 (.Y (flitout_1[9]), .A (int01_dup_1502), .B (
                     GND_dup_287), .S0 (colsel1reg_1)) ;
    MXT2_X0P5M_A12TS ix1549 (.Y (int01_dup_1503), .A (flitout_switch_0_10), .B (
                     flitout_switch_1_10), .S0 (colsel0reg_0)) ;
    MXT2_X0P5M_A12TS ix1550 (.Y (flitout_0[10]), .A (int01_dup_1503), .B (
                     GND_dup_287), .S0 (colsel0reg_1)) ;
    MXT2_X0P5M_A12TS ix1551 (.Y (int01_dup_1504), .A (flitout_switch_0_10), .B (
                     flitout_switch_1_10), .S0 (colsel1reg_0)) ;
    MXT2_X0P5M_A12TS ix1552 (.Y (flitout_1[10]), .A (int01_dup_1504), .B (
                     GND_dup_287), .S0 (colsel1reg_1)) ;
    MXT2_X0P5M_A12TS ix1554 (.Y (int01_dup_1505), .A (flitout_switch_0_11), .B (
                     flitout_switch_1_11), .S0 (colsel0reg_0)) ;
    MXT2_X0P5M_A12TS ix1555 (.Y (flitout_0[11]), .A (int01_dup_1505), .B (
                     GND_dup_287), .S0 (colsel0reg_1)) ;
    MXT2_X0P5M_A12TS ix1556 (.Y (int01_dup_1506), .A (flitout_switch_0_11), .B (
                     flitout_switch_1_11), .S0 (colsel1reg_0)) ;
    MXT2_X0P5M_A12TS ix1557 (.Y (flitout_1[11]), .A (int01_dup_1506), .B (
                     GND_dup_287), .S0 (colsel1reg_1)) ;
endmodule
