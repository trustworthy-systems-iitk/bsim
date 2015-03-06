
module voting_machine ( osc_clk, reset_n, x_coord, y_coord, xy_valid, 
        max_selections, contest_num, current_contest_state );
  input [11:0] x_coord;
  input [11:0] y_coord;
  input [3:0] max_selections;
  output [2:0] contest_num;
  output [11:0] current_contest_state;
  input osc_clk, reset_n, xy_valid;
  wire   map_irq_pulse, touch_pulse, ss_enable, n3, n4, pulse_n9, pulse_n8,
         pulse_n7, pulse_n5, pulse_n4, pulse_n3, pulse_n2, pulse_n1, pulse_n6,
         pulse_n18, pulse_n17, pulse_state_0_, pulse_state_1_, map_n127,
         map_n126, map_n125, map_n124, map_n123, map_n122, map_n121, map_n120,
         map_n119, map_n118, map_n117, map_n116, map_n115, map_n114, map_n113,
         map_n112, map_n111, map_n110, map_n109, map_n108, map_n107, map_n106,
         map_n105, map_n104, map_n103, map_n102, map_n101, map_n100, map_n99,
         map_n98, map_n97, map_n96, map_n95, map_n94, map_n93, map_n92,
         map_n91, map_n90, map_n89, map_n88, map_n87, map_n86, map_n85,
         map_n84, map_n83, map_n82, map_n81, map_n80, map_n79, map_n78,
         map_n77, map_n76, map_n75, map_n74, map_n73, map_n72, map_n71,
         map_n70, map_n69, map_n68, map_n67, map_n66, map_n65, map_n64,
         map_n63, map_n62, map_n61, map_n60, map_n59, map_n58, map_n57,
         map_n56, map_n55, map_n54, map_n53, map_n52, map_n51, map_n50,
         map_n49, map_n48, map_n47, map_n46, map_n45, map_n44, map_n43,
         map_n42, map_n41, map_n40, map_n39, map_n38, map_n37, map_n36,
         map_n35, map_n34, map_n33, map_n32, map_n31, map_n30, map_n29,
         map_n28, map_n27, map_n26, map_n25, map_n24, map_n23, map_n22,
         map_n21, map_n20, map_n19, map_n18, map_n17, map_n16, map_n15,
         map_n14, map_n13, map_n12, map_n11, map_n10, map_n9, map_n8, map_n7,
         map_n6, map_n5, map_n3, map_n2, map_n1, map_n379, map_n378, map_n377,
         map_n376, map_n4, map_n1921, map_n1893, map_n1892, map_n1891,
         map_n1890, map_n1889, map_n1888, map_n1887, map_n1886, map_n1885,
         map_n1884, map_n1883, map_n1882, map_n1881, map_n1865, map_n1864,
         map_n1863, map_n1862, map_n1861, map_n1860, map_n1859, map_n1858,
         map_n1857, map_n1856, map_n1855, map_n1854, map_n1853, map_n1776,
         map_n1775, map_n1774, map_n1773, map_n1772, map_n1771, map_n1770,
         map_n1769, map_n1768, map_n1767, map_n1766, map_n1765, map_n1764,
         map_n1748, map_n1747, map_n1746, map_n1745, map_n1744, map_n1743,
         map_n1742, map_n1741, map_n1740, map_n1739, map_n1738, map_n1737,
         map_n1736, map_n1659, map_n1658, map_n1657, map_n1656, map_n1655,
         map_n1654, map_n1653, map_n1652, map_n1651, map_n1650, map_n1649,
         map_n1648, map_n1647, map_n1631, map_n1630, map_n1629, map_n1628,
         map_n1627, map_n1626, map_n1625, map_n1624, map_n1623, map_n1622,
         map_n1621, map_n1620, map_n1619, map_n3770, map_n3760, map_n375,
         map_n374, map_n373, map_n372, map_n371, map_n370, map_n369, map_n368,
         map_n367, map_n366, map_n365, map_n349, map_n348, map_n347, map_n346,
         map_n345, map_n344, map_n343, map_n342, map_n341, map_n340, map_n339,
         map_n338, map_n337, map_n265, map_n264, map_n263, map_n262, map_n261,
         map_n260, map_n259, map_n258, map_n257, map_n256, map_n255, map_n254,
         map_n253, map_n237, map_n236, map_n235, map_n234, map_n233, map_n232,
         map_n231, map_n230, map_n229, map_n228, map_n227, map_n226, map_n225,
         map_n158, map_n157, map_n156, map_n155, map_n154, map_n153, map_n152,
         map_n151, map_n150, map_n149, map_n148, map_n147, map_n146, map_n130,
         map_n129, map_n128, map_n1270, map_n1260, map_n1250, map_n1240,
         map_n1230, map_n1220, map_n1210, map_n1200, map_n1190, map_n1180,
         controller_n28, controller_n27, controller_n26, controller_n25,
         controller_n24, controller_n23, controller_n22, controller_n21,
         controller_n20, controller_n19, controller_n18, controller_n17,
         controller_n16, controller_n15, controller_n14, controller_n12,
         controller_n11, controller_n10, controller_n9, controller_n8,
         controller_n7, controller_n6, controller_n5, controller_n4,
         controller_n3, controller_n2, controller_n1, controller_n47,
         controller_n46, controller_n45, controller_n44, controller_n13,
         controller_cast, demux_1to8_n4, demux_1to8_n3, demux_1to8_n2,
         demux_1to8_n1, ss0_n91, ss0_n90, ss0_n89, ss0_n88, ss0_n87, ss0_n86,
         ss0_n85, ss0_n84, ss0_n83, ss0_n82, ss0_n81, ss0_n80, ss0_n79,
         ss0_n78, ss0_n77, ss0_n76, ss0_n75, ss0_n74, ss0_n73, ss0_n72,
         ss0_n71, ss0_n70, ss0_n69, ss0_n68, ss0_n67, ss0_n66, ss0_n65,
         ss0_n64, ss0_n63, ss0_n62, ss0_n61, ss0_n60, ss0_n59, ss0_n58,
         ss0_n57, ss0_n56, ss0_n55, ss0_n54, ss0_n53, ss0_n52, ss0_n51,
         ss0_n50, ss0_n49, ss0_n48, ss0_n47, ss0_n46, ss0_n45, ss0_n44,
         ss0_n43, ss0_n42, ss0_n41, ss0_n40, ss0_n39, ss0_n38, ss0_n37,
         ss0_n36, ss0_n35, ss0_n34, ss0_n33, ss0_n32, ss0_n31, ss0_n30,
         ss0_n29, ss0_n28, ss0_n27, ss0_n26, ss0_n25, ss0_n24, ss0_n23,
         ss0_n22, ss0_n21, ss0_n20, ss0_n19, ss0_n18, ss0_n17, ss0_n16,
         ss0_n15, ss0_n14, ss0_n13, ss0_n12, ss0_n11, ss0_n10, ss0_n9, ss0_n8,
         ss0_n7, ss0_n6, ss0_n5, ss0_n4, ss0_n3, ss0_n2, ss0_n1, ss0_n433,
         ss0_n432, ss0_n431, ss0_n430, ss0_n429, ss0_n428, ss0_n427, ss0_n426,
         ss0_n425, ss0_n424, ss0_n423, ss0_n422, ss1_n103, ss1_n102, ss1_n101,
         ss1_n100, ss1_n99, ss1_n98, ss1_n97, ss1_n96, ss1_n95, ss1_n94,
         ss1_n93, ss1_n92, ss1_n91, ss1_n90, ss1_n89, ss1_n88, ss1_n87,
         ss1_n86, ss1_n85, ss1_n84, ss1_n83, ss1_n82, ss1_n81, ss1_n80,
         ss1_n79, ss1_n78, ss1_n77, ss1_n76, ss1_n75, ss1_n74, ss1_n73,
         ss1_n72, ss1_n71, ss1_n70, ss1_n69, ss1_n68, ss1_n67, ss1_n66,
         ss1_n65, ss1_n64, ss1_n63, ss1_n62, ss1_n61, ss1_n60, ss1_n59,
         ss1_n58, ss1_n57, ss1_n56, ss1_n55, ss1_n54, ss1_n53, ss1_n52,
         ss1_n51, ss1_n50, ss1_n49, ss1_n48, ss1_n47, ss1_n46, ss1_n45,
         ss1_n44, ss1_n43, ss1_n42, ss1_n41, ss1_n40, ss1_n39, ss1_n38,
         ss1_n37, ss1_n36, ss1_n35, ss1_n34, ss1_n33, ss1_n32, ss1_n31,
         ss1_n30, ss1_n29, ss1_n28, ss1_n27, ss1_n26, ss1_n25, ss1_n24,
         ss1_n23, ss1_n22, ss1_n21, ss1_n20, ss1_n19, ss1_n18, ss1_n17,
         ss1_n16, ss1_n15, ss1_n14, ss1_n13, ss1_n12, ss1_n11, ss1_n10, ss1_n9,
         ss1_n8, ss1_n7, ss1_n6, ss1_n5, ss1_n4, ss1_n3, ss1_n2, ss1_n1,
         ss2_n103, ss2_n102, ss2_n101, ss2_n100, ss2_n99, ss2_n98, ss2_n97,
         ss2_n96, ss2_n95, ss2_n94, ss2_n93, ss2_n92, ss2_n91, ss2_n90,
         ss2_n89, ss2_n88, ss2_n87, ss2_n86, ss2_n85, ss2_n84, ss2_n83,
         ss2_n82, ss2_n81, ss2_n80, ss2_n79, ss2_n78, ss2_n77, ss2_n76,
         ss2_n75, ss2_n74, ss2_n73, ss2_n72, ss2_n71, ss2_n70, ss2_n69,
         ss2_n68, ss2_n67, ss2_n66, ss2_n65, ss2_n64, ss2_n63, ss2_n62,
         ss2_n61, ss2_n60, ss2_n59, ss2_n58, ss2_n57, ss2_n56, ss2_n55,
         ss2_n54, ss2_n53, ss2_n52, ss2_n51, ss2_n50, ss2_n49, ss2_n48,
         ss2_n47, ss2_n46, ss2_n45, ss2_n44, ss2_n43, ss2_n42, ss2_n41,
         ss2_n40, ss2_n39, ss2_n38, ss2_n37, ss2_n36, ss2_n35, ss2_n34,
         ss2_n33, ss2_n32, ss2_n31, ss2_n30, ss2_n29, ss2_n28, ss2_n27,
         ss2_n26, ss2_n25, ss2_n24, ss2_n23, ss2_n22, ss2_n21, ss2_n20,
         ss2_n19, ss2_n18, ss2_n17, ss2_n16, ss2_n15, ss2_n14, ss2_n13,
         ss2_n12, ss2_n11, ss2_n10, ss2_n9, ss2_n8, ss2_n7, ss2_n6, ss2_n5,
         ss2_n4, ss2_n3, ss2_n2, ss2_n1, ss3_n103, ss3_n102, ss3_n101,
         ss3_n100, ss3_n99, ss3_n98, ss3_n97, ss3_n96, ss3_n95, ss3_n94,
         ss3_n93, ss3_n92, ss3_n91, ss3_n90, ss3_n89, ss3_n88, ss3_n87,
         ss3_n86, ss3_n85, ss3_n84, ss3_n83, ss3_n82, ss3_n81, ss3_n80,
         ss3_n79, ss3_n78, ss3_n77, ss3_n76, ss3_n75, ss3_n74, ss3_n73,
         ss3_n72, ss3_n71, ss3_n70, ss3_n69, ss3_n68, ss3_n67, ss3_n66,
         ss3_n65, ss3_n64, ss3_n63, ss3_n62, ss3_n61, ss3_n60, ss3_n59,
         ss3_n58, ss3_n57, ss3_n56, ss3_n55, ss3_n54, ss3_n53, ss3_n52,
         ss3_n51, ss3_n50, ss3_n49, ss3_n48, ss3_n47, ss3_n46, ss3_n45,
         ss3_n44, ss3_n43, ss3_n42, ss3_n41, ss3_n40, ss3_n39, ss3_n38,
         ss3_n37, ss3_n36, ss3_n35, ss3_n34, ss3_n33, ss3_n32, ss3_n31,
         ss3_n30, ss3_n29, ss3_n28, ss3_n27, ss3_n26, ss3_n25, ss3_n24,
         ss3_n23, ss3_n22, ss3_n21, ss3_n20, ss3_n19, ss3_n18, ss3_n17,
         ss3_n16, ss3_n15, ss3_n14, ss3_n13, ss3_n12, ss3_n11, ss3_n10, ss3_n9,
         ss3_n8, ss3_n7, ss3_n6, ss3_n5, ss3_n4, ss3_n3, ss3_n2, ss3_n1,
         ss4_n103, ss4_n102, ss4_n101, ss4_n100, ss4_n99, ss4_n98, ss4_n97,
         ss4_n96, ss4_n95, ss4_n94, ss4_n93, ss4_n92, ss4_n91, ss4_n90,
         ss4_n89, ss4_n88, ss4_n87, ss4_n86, ss4_n85, ss4_n84, ss4_n83,
         ss4_n82, ss4_n81, ss4_n80, ss4_n79, ss4_n78, ss4_n77, ss4_n76,
         ss4_n75, ss4_n74, ss4_n73, ss4_n72, ss4_n71, ss4_n70, ss4_n69,
         ss4_n68, ss4_n67, ss4_n66, ss4_n65, ss4_n64, ss4_n63, ss4_n62,
         ss4_n61, ss4_n60, ss4_n59, ss4_n58, ss4_n57, ss4_n56, ss4_n55,
         ss4_n54, ss4_n53, ss4_n52, ss4_n51, ss4_n50, ss4_n49, ss4_n48,
         ss4_n47, ss4_n46, ss4_n45, ss4_n44, ss4_n43, ss4_n42, ss4_n41,
         ss4_n40, ss4_n39, ss4_n38, ss4_n37, ss4_n36, ss4_n35, ss4_n34,
         ss4_n33, ss4_n32, ss4_n31, ss4_n30, ss4_n29, ss4_n28, ss4_n27,
         ss4_n26, ss4_n25, ss4_n24, ss4_n23, ss4_n22, ss4_n21, ss4_n20,
         ss4_n19, ss4_n18, ss4_n17, ss4_n16, ss4_n15, ss4_n14, ss4_n13,
         ss4_n12, ss4_n11, ss4_n10, ss4_n9, ss4_n8, ss4_n7, ss4_n6, ss4_n5,
         ss4_n4, ss4_n3, ss4_n2, ss4_n1, ss5_n103, ss5_n102, ss5_n101,
         ss5_n100, ss5_n99, ss5_n98, ss5_n97, ss5_n96, ss5_n95, ss5_n94,
         ss5_n93, ss5_n92, ss5_n91, ss5_n90, ss5_n89, ss5_n88, ss5_n87,
         ss5_n86, ss5_n85, ss5_n84, ss5_n83, ss5_n82, ss5_n81, ss5_n80,
         ss5_n79, ss5_n78, ss5_n77, ss5_n76, ss5_n75, ss5_n74, ss5_n73,
         ss5_n72, ss5_n71, ss5_n70, ss5_n69, ss5_n68, ss5_n67, ss5_n66,
         ss5_n65, ss5_n64, ss5_n63, ss5_n62, ss5_n61, ss5_n60, ss5_n59,
         ss5_n58, ss5_n57, ss5_n56, ss5_n55, ss5_n54, ss5_n53, ss5_n52,
         ss5_n51, ss5_n50, ss5_n49, ss5_n48, ss5_n47, ss5_n46, ss5_n45,
         ss5_n44, ss5_n43, ss5_n42, ss5_n41, ss5_n40, ss5_n39, ss5_n38,
         ss5_n37, ss5_n36, ss5_n35, ss5_n34, ss5_n33, ss5_n32, ss5_n31,
         ss5_n30, ss5_n29, ss5_n28, ss5_n27, ss5_n26, ss5_n25, ss5_n24,
         ss5_n23, ss5_n22, ss5_n21, ss5_n20, ss5_n19, ss5_n18, ss5_n17,
         ss5_n16, ss5_n15, ss5_n14, ss5_n13, ss5_n12, ss5_n11, ss5_n10, ss5_n9,
         ss5_n8, ss5_n7, ss5_n6, ss5_n5, ss5_n4, ss5_n3, ss5_n2, ss5_n1,
         ss6_n103, ss6_n102, ss6_n101, ss6_n100, ss6_n99, ss6_n98, ss6_n97,
         ss6_n96, ss6_n95, ss6_n94, ss6_n93, ss6_n92, ss6_n91, ss6_n90,
         ss6_n89, ss6_n88, ss6_n87, ss6_n86, ss6_n85, ss6_n84, ss6_n83,
         ss6_n82, ss6_n81, ss6_n80, ss6_n79, ss6_n78, ss6_n77, ss6_n76,
         ss6_n75, ss6_n74, ss6_n73, ss6_n72, ss6_n71, ss6_n70, ss6_n69,
         ss6_n68, ss6_n67, ss6_n66, ss6_n65, ss6_n64, ss6_n63, ss6_n62,
         ss6_n61, ss6_n60, ss6_n59, ss6_n58, ss6_n57, ss6_n56, ss6_n55,
         ss6_n54, ss6_n53, ss6_n52, ss6_n51, ss6_n50, ss6_n49, ss6_n48,
         ss6_n47, ss6_n46, ss6_n45, ss6_n44, ss6_n43, ss6_n42, ss6_n41,
         ss6_n40, ss6_n39, ss6_n38, ss6_n37, ss6_n36, ss6_n35, ss6_n34,
         ss6_n33, ss6_n32, ss6_n31, ss6_n30, ss6_n29, ss6_n28, ss6_n27,
         ss6_n26, ss6_n25, ss6_n24, ss6_n23, ss6_n22, ss6_n21, ss6_n20,
         ss6_n19, ss6_n18, ss6_n17, ss6_n16, ss6_n15, ss6_n14, ss6_n13,
         ss6_n12, ss6_n11, ss6_n10, ss6_n9, ss6_n8, ss6_n7, ss6_n6, ss6_n5,
         ss6_n4, ss6_n3, ss6_n2, ss6_n1, ss7_n103, ss7_n102, ss7_n101,
         ss7_n100, ss7_n99, ss7_n98, ss7_n97, ss7_n96, ss7_n95, ss7_n94,
         ss7_n93, ss7_n92, ss7_n91, ss7_n90, ss7_n89, ss7_n88, ss7_n87,
         ss7_n86, ss7_n85, ss7_n84, ss7_n83, ss7_n82, ss7_n81, ss7_n80,
         ss7_n79, ss7_n78, ss7_n77, ss7_n76, ss7_n75, ss7_n74, ss7_n73,
         ss7_n72, ss7_n71, ss7_n70, ss7_n69, ss7_n68, ss7_n67, ss7_n66,
         ss7_n65, ss7_n64, ss7_n63, ss7_n62, ss7_n61, ss7_n60, ss7_n59,
         ss7_n58, ss7_n57, ss7_n56, ss7_n55, ss7_n54, ss7_n53, ss7_n52,
         ss7_n51, ss7_n50, ss7_n49, ss7_n48, ss7_n47, ss7_n46, ss7_n45,
         ss7_n44, ss7_n43, ss7_n42, ss7_n41, ss7_n40, ss7_n39, ss7_n38,
         ss7_n37, ss7_n36, ss7_n35, ss7_n34, ss7_n33, ss7_n32, ss7_n31,
         ss7_n30, ss7_n29, ss7_n28, ss7_n27, ss7_n26, ss7_n25, ss7_n24,
         ss7_n23, ss7_n22, ss7_n21, ss7_n20, ss7_n19, ss7_n18, ss7_n17,
         ss7_n16, ss7_n15, ss7_n14, ss7_n13, ss7_n12, ss7_n11, ss7_n10, ss7_n9,
         ss7_n8, ss7_n7, ss7_n6, ss7_n5, ss7_n4, ss7_n3, ss7_n2, ss7_n1,
         mux_8to1_n58, mux_8to1_n57, mux_8to1_n56, mux_8to1_n55, mux_8to1_n54,
         mux_8to1_n53, mux_8to1_n52, mux_8to1_n51, mux_8to1_n50, mux_8to1_n49,
         mux_8to1_n48, mux_8to1_n47, mux_8to1_n46, mux_8to1_n45, mux_8to1_n44,
         mux_8to1_n43, mux_8to1_n42, mux_8to1_n41, mux_8to1_n40, mux_8to1_n39,
         mux_8to1_n38, mux_8to1_n37, mux_8to1_n36, mux_8to1_n35, mux_8to1_n34,
         mux_8to1_n33, mux_8to1_n32, mux_8to1_n31, mux_8to1_n30, mux_8to1_n29,
         mux_8to1_n28, mux_8to1_n27, mux_8to1_n26, mux_8to1_n25, mux_8to1_n24,
         mux_8to1_n23, mux_8to1_n22, mux_8to1_n21, mux_8to1_n20, mux_8to1_n19,
         mux_8to1_n18, mux_8to1_n17, mux_8to1_n16, mux_8to1_n15, mux_8to1_n14,
         mux_8to1_n13, mux_8to1_n12, mux_8to1_n11, mux_8to1_n10, mux_8to1_n9,
         mux_8to1_n8, mux_8to1_n7, mux_8to1_n6, mux_8to1_n5, mux_8to1_n4,
         mux_8to1_n3, mux_8to1_n2, mux_8to1_n1;
  wire   [3:0] button_num;
  wire   [7:0] ss_selector;
  wire   [11:0] ss_state_0;
  wire   [11:0] ss_state_1;
  wire   [11:0] ss_state_2;
  wire   [11:0] ss_state_3;
  wire   [11:0] ss_state_4;
  wire   [11:0] ss_state_5;
  wire   [11:0] ss_state_6;
  wire   [11:0] ss_state_7;
  wire   [11:2] map_add_131_3_i16_carry;
  wire   [11:2] map_add_131_i16_carry;
  wire   [11:2] map_add_131_3_i15_carry;
  wire   [11:2] map_add_131_i15_carry;
  wire   [11:2] map_add_131_3_i14_carry;
  wire   [11:2] map_add_131_i14_carry;
  wire   [11:2] map_add_131_3_i3_carry;
  wire   [11:2] map_add_131_i3_carry;
  wire   [11:2] map_add_131_3_i2_carry;
  wire   [11:2] map_add_131_i2_carry;
  wire   [11:2] map_add_131_3_carry;
  wire   [11:2] map_add_131_carry;

  INV_X1M_A12TR u3 ( .A(reset_n), .Y(n4) );
  INV_X0P5B_A12TR u4 ( .A(map_irq_pulse), .Y(n3) );
  XNOR2_X0P5M_A12TR pulse_u13 ( .A(pulse_state_0_), .B(pulse_state_1_), .Y(
        pulse_n7) );
  NOR2B_X0P5M_A12TR pulse_u12 ( .AN(pulse_n7), .B(n4), .Y(pulse_n2) );
  INV_X0P5B_A12TR pulse_u11 ( .A(pulse_n2), .Y(pulse_n9) );
  INV_X0P5B_A12TR pulse_u10 ( .A(xy_valid), .Y(pulse_n5) );
  INV_X0P5B_A12TR pulse_u9 ( .A(pulse_state_0_), .Y(pulse_n3) );
  NOR2_X0P5A_A12TR pulse_u8 ( .A(pulse_n9), .B(pulse_n3), .Y(pulse_n4) );
  INV_X0P5B_A12TR pulse_u7 ( .A(pulse_n4), .Y(pulse_n8) );
  OAI21_X0P5M_A12TR pulse_u6 ( .A0(pulse_n9), .A1(pulse_n5), .B0(pulse_n8), 
        .Y(pulse_n17) );
  OAI31_X0P5M_A12TR pulse_u5 ( .A0(pulse_n5), .A1(n4), .A2(pulse_n7), .B0(
        pulse_n8), .Y(pulse_n18) );
  AOI32_X0P5M_A12TR pulse_u4 ( .A0(pulse_n2), .A1(pulse_n3), .A2(xy_valid), 
        .B0(map_irq_pulse), .B1(pulse_n4), .Y(pulse_n1) );
  INV_X0P5B_A12TR pulse_u3 ( .A(pulse_n1), .Y(pulse_n6) );
  DFFQ_X1M_A12TR pulse_state_reg_0_ ( .D(pulse_n17), .CK(osc_clk), .Q(
        pulse_state_0_) );
  DFFQ_X1M_A12TR pulse_output_pulse_reg ( .D(pulse_n6), .CK(osc_clk), .Q(
        map_irq_pulse) );
  DFFQ_X1M_A12TR pulse_state_reg_1_ ( .D(pulse_n18), .CK(osc_clk), .Q(
        pulse_state_1_) );
  INV_X0P5B_A12TR map_u134 ( .A(x_coord[10]), .Y(map_n124) );
  INV_X0P5B_A12TR map_u133 ( .A(x_coord[11]), .Y(map_n64) );
  INV_X0P5B_A12TR map_u132 ( .A(x_coord[9]), .Y(map_n65) );
  OA21A1OI2_X0P5M_A12TR map_u131 ( .A0(x_coord[0]), .A1(x_coord[1]), .B0(
        x_coord[2]), .C0(x_coord[3]), .Y(map_n127) );
  INV_X0P5B_A12TR map_u130 ( .A(map_n127), .Y(map_n68) );
  OA21_X0P5M_A12TR map_u129 ( .A0(map_n68), .A1(x_coord[4]), .B0(x_coord[5]), 
        .Y(map_n126) );
  OAI211_X0P5M_A12TR map_u128 ( .A0(map_n126), .A1(x_coord[6]), .B0(x_coord[7]), .C0(x_coord[8]), .Y(map_n125) );
  AND4_X0P5M_A12TR map_u127 ( .A(map_n124), .B(map_n64), .C(map_n65), .D(
        map_n125), .Y(map_n77) );
  NOR3_X0P5A_A12TR map_u126 ( .A(y_coord[11]), .B(y_coord[9]), .C(y_coord[10]), 
        .Y(map_n48) );
  OAI211_X0P5M_A12TR map_u125 ( .A0(map_n253), .A1(map_n254), .B0(map_n255), 
        .C0(map_n256), .Y(map_n123) );
  NAND2B_X0P5M_A12TR map_u124 ( .AN(map_n257), .B(map_n123), .Y(map_n122) );
  AOI211_X0P5M_A12TR map_u123 ( .A0(map_n258), .A1(map_n122), .B0(map_n260), 
        .C0(map_n259), .Y(map_n121) );
  AOI2XB1_X0P5M_A12TR map_u122 ( .A1N(map_n121), .A0(map_n261), .B0(map_n262), 
        .Y(map_n114) );
  NOR3_X0P5A_A12TR map_u121 ( .A(map_n263), .B(map_n265), .C(map_n264), .Y(
        map_n115) );
  OAI211_X0P5M_A12TR map_u120 ( .A0(map_n225), .A1(map_n226), .B0(map_n227), 
        .C0(map_n228), .Y(map_n120) );
  NAND2B_X0P5M_A12TR map_u119 ( .AN(map_n229), .B(map_n120), .Y(map_n119) );
  AOI211_X0P5M_A12TR map_u118 ( .A0(map_n230), .A1(map_n119), .B0(map_n232), 
        .C0(map_n231), .Y(map_n118) );
  AOI2XB1_X0P5M_A12TR map_u117 ( .A1N(map_n118), .A0(map_n233), .B0(map_n234), 
        .Y(map_n116) );
  NOR3_X0P5A_A12TR map_u116 ( .A(map_n235), .B(map_n237), .C(map_n236), .Y(
        map_n117) );
  AOI22_X0P5M_A12TR map_u115 ( .A0(map_n114), .A1(map_n115), .B0(map_n116), 
        .B1(map_n117), .Y(map_n111) );
  NAND2_X0P5A_A12TR map_u114 ( .A(y_coord[1]), .B(y_coord[0]), .Y(map_n75) );
  INV_X0P5B_A12TR map_u113 ( .A(y_coord[2]), .Y(map_n92) );
  NOR3_X0P5A_A12TR map_u112 ( .A(y_coord[3]), .B(y_coord[5]), .C(y_coord[4]), 
        .Y(map_n113) );
  NAND3_X0P5A_A12TR map_u111 ( .A(map_n75), .B(map_n92), .C(map_n113), .Y(
        map_n112) );
  NAND4_X0P5A_A12TR map_u110 ( .A(y_coord[8]), .B(y_coord[7]), .C(y_coord[6]), 
        .D(map_n112), .Y(map_n58) );
  NAND4_X0P5A_A12TR map_u109 ( .A(map_n77), .B(map_n48), .C(map_n111), .D(
        map_n58), .Y(map_n10) );
  INV_X0P5B_A12TR map_u108 ( .A(y_coord[3]), .Y(map_n94) );
  INV_X0P5B_A12TR map_u107 ( .A(y_coord[4]), .Y(map_n95) );
  AO21A1AI2_X0P5M_A12TR map_u106 ( .A0(map_n92), .A1(map_n75), .B0(map_n94), 
        .C0(map_n95), .Y(map_n110) );
  OAI31_X0P5M_A12TR map_u105 ( .A0(map_n110), .A1(y_coord[6]), .A2(y_coord[5]), 
        .B0(y_coord[7]), .Y(map_n109) );
  INV_X0P5B_A12TR map_u104 ( .A(map_n109), .Y(map_n107) );
  OR2_X0P5M_A12TR map_u103 ( .A(y_coord[11]), .B(y_coord[10]), .Y(map_n108) );
  OA21A1OI2_X0P5M_A12TR map_u102 ( .A0(y_coord[8]), .A1(map_n107), .B0(
        y_coord[9]), .C0(map_n108), .Y(map_n23) );
  OAI211_X0P5M_A12TR map_u101 ( .A0(map_n337), .A1(map_n338), .B0(map_n339), 
        .C0(map_n340), .Y(map_n106) );
  NAND2B_X0P5M_A12TR map_u100 ( .AN(map_n341), .B(map_n106), .Y(map_n105) );
  AOI211_X0P5M_A12TR map_u99 ( .A0(map_n342), .A1(map_n105), .B0(map_n344), 
        .C0(map_n343), .Y(map_n104) );
  AOI2XB1_X0P5M_A12TR map_u98 ( .A1N(map_n104), .A0(map_n345), .B0(map_n346), 
        .Y(map_n97) );
  NOR3_X0P5A_A12TR map_u97 ( .A(map_n347), .B(map_n349), .C(map_n348), .Y(
        map_n98) );
  OA21A1OI2_X0P5M_A12TR map_u96 ( .A0(map_n366), .A1(map_n365), .B0(map_n367), 
        .C0(map_n368), .Y(map_n103) );
  NOR2B_X0P5M_A12TR map_u95 ( .AN(map_n369), .B(map_n103), .Y(map_n101) );
  AND3_X0P5M_A12TR map_u94 ( .A(map_n372), .B(map_n371), .C(map_n373), .Y(
        map_n102) );
  AOI31_X0P5M_A12TR map_u93 ( .A0(map_n101), .A1(map_n370), .A2(map_n102), 
        .B0(map_n374), .Y(map_n99) );
  NOR3_X0P5A_A12TR map_u92 ( .A(map_n375), .B(map_n3770), .C(map_n3760), .Y(
        map_n100) );
  AOI22_X0P5M_A12TR map_u91 ( .A0(map_n97), .A1(map_n98), .B0(map_n99), .B1(
        map_n100), .Y(map_n96) );
  NAND3_X0P5A_A12TR map_u90 ( .A(map_n23), .B(map_n77), .C(map_n96), .Y(map_n9) );
  NOR2_X0P5A_A12TR map_u89 ( .A(map_n94), .B(map_n95), .Y(map_n93) );
  NAND4_X0P5A_A12TR map_u88 ( .A(y_coord[7]), .B(y_coord[6]), .C(map_n93), .D(
        y_coord[5]), .Y(map_n69) );
  AOI21_X0P5M_A12TR map_u87 ( .A0(map_n92), .A1(map_n75), .B0(map_n69), .Y(
        map_n90) );
  INV_X0P5B_A12TR map_u86 ( .A(map_n48), .Y(map_n91) );
  NOR3_X0P5A_A12TR map_u85 ( .A(map_n90), .B(y_coord[8]), .C(map_n91), .Y(
        map_n35) );
  NOR2_X0P5A_A12TR map_u84 ( .A(map_n156), .B(map_n155), .Y(map_n79) );
  NOR2_X0P5A_A12TR map_u83 ( .A(map_n158), .B(map_n157), .Y(map_n80) );
  NOR2_X0P5A_A12TR map_u82 ( .A(map_n150), .B(map_n149), .Y(map_n88) );
  OAI21_X0P5M_A12TR map_u81 ( .A0(map_n146), .A1(map_n147), .B0(map_n148), .Y(
        map_n89) );
  AOI21B_X0P5M_A12TR map_u80 ( .A0(map_n88), .A1(map_n89), .B0N(map_n151), .Y(
        map_n87) );
  AOI211_X0P5M_A12TR map_u79 ( .A0(map_n87), .A1(map_n152), .B0(map_n154), 
        .C0(map_n153), .Y(map_n81) );
  OAI211_X0P5M_A12TR map_u78 ( .A0(map_n1180), .A1(map_n1190), .B0(map_n1200), 
        .C0(map_n1210), .Y(map_n86) );
  NAND2B_X0P5M_A12TR map_u77 ( .AN(map_n1220), .B(map_n86), .Y(map_n85) );
  AOI211_X0P5M_A12TR map_u76 ( .A0(map_n1230), .A1(map_n85), .B0(map_n1250), 
        .C0(map_n1240), .Y(map_n84) );
  AOI2XB1_X0P5M_A12TR map_u75 ( .A1N(map_n84), .A0(map_n1260), .B0(map_n1270), 
        .Y(map_n82) );
  NOR3_X0P5A_A12TR map_u74 ( .A(map_n128), .B(map_n130), .C(map_n129), .Y(
        map_n83) );
  AOI32_X0P5M_A12TR map_u73 ( .A0(map_n79), .A1(map_n80), .A2(map_n81), .B0(
        map_n82), .B1(map_n83), .Y(map_n78) );
  NAND3_X0P5A_A12TR map_u72 ( .A(map_n77), .B(map_n35), .C(map_n78), .Y(
        map_n11) );
  INV_X0P5B_A12TR map_u71 ( .A(x_coord[8]), .Y(map_n66) );
  NAND3_X0P5A_A12TR map_u70 ( .A(x_coord[5]), .B(x_coord[2]), .C(x_coord[7]), 
        .Y(map_n70) );
  AND2_X0P5M_A12TR map_u69 ( .A(x_coord[0]), .B(x_coord[10]), .Y(map_n76) );
  NAND4B_X0P5M_A12TR map_u68 ( .AN(map_n75), .B(y_coord[8]), .C(y_coord[9]), 
        .D(map_n76), .Y(map_n71) );
  AND2_X0P5M_A12TR map_u67 ( .A(x_coord[1]), .B(x_coord[11]), .Y(map_n74) );
  NAND4_X0P5A_A12TR map_u66 ( .A(x_coord[6]), .B(x_coord[4]), .C(map_n74), .D(
        x_coord[3]), .Y(map_n72) );
  NAND4_X0P5A_A12TR map_u65 ( .A(y_coord[2]), .B(y_coord[11]), .C(y_coord[10]), 
        .D(x_coord[9]), .Y(map_n73) );
  OR6_X0P5M_A12TR map_u64 ( .A(map_n66), .B(map_n69), .C(map_n70), .D(map_n71), 
        .E(map_n72), .F(map_n73), .Y(map_n8) );
  NAND4_X0P5A_A12TR map_u63 ( .A(map_n10), .B(map_n9), .C(map_n11), .D(map_n8), 
        .Y(map_n5) );
  INV_X0P5B_A12TR map_u62 ( .A(map_n5), .Y(map_n1) );
  AOI31_X0P5M_A12TR map_u61 ( .A0(x_coord[5]), .A1(map_n68), .A2(x_coord[4]), 
        .B0(x_coord[6]), .Y(map_n67) );
  INV_X0P5B_A12TR map_u60 ( .A(map_n67), .Y(map_n62) );
  NAND3_X0P5A_A12TR map_u59 ( .A(map_n64), .B(map_n65), .C(map_n66), .Y(
        map_n63) );
  AOI211_X0P5M_A12TR map_u58 ( .A0(map_n62), .A1(x_coord[7]), .B0(x_coord[10]), 
        .C0(map_n63), .Y(map_n22) );
  OAI211_X0P5M_A12TR map_u57 ( .A0(map_n1647), .A1(map_n1648), .B0(map_n1649), 
        .C0(map_n1650), .Y(map_n61) );
  NAND2B_X0P5M_A12TR map_u56 ( .AN(map_n1651), .B(map_n61), .Y(map_n60) );
  AOI211_X0P5M_A12TR map_u55 ( .A0(map_n1652), .A1(map_n60), .B0(map_n1654), 
        .C0(map_n1653), .Y(map_n59) );
  AOI2XB1_X0P5M_A12TR map_u54 ( .A1N(map_n59), .A0(map_n1655), .B0(map_n1656), 
        .Y(map_n56) );
  NOR3_X0P5A_A12TR map_u53 ( .A(map_n1657), .B(map_n1659), .C(map_n1658), .Y(
        map_n57) );
  AOI21B_X0P5M_A12TR map_u52 ( .A0(map_n56), .A1(map_n57), .B0N(map_n58), .Y(
        map_n49) );
  OA211_X0P5M_A12TR map_u51 ( .A0(map_n1619), .A1(map_n1620), .B0(map_n1621), 
        .C0(map_n1622), .Y(map_n55) );
  AOI31_X0P5M_A12TR map_u50 ( .A0(map_n1624), .A1(map_n1623), .A2(map_n55), 
        .B0(map_n1625), .Y(map_n51) );
  NOR2_X0P5A_A12TR map_u49 ( .A(map_n1627), .B(map_n1626), .Y(map_n52) );
  NOR2_X0P5A_A12TR map_u48 ( .A(map_n1629), .B(map_n1628), .Y(map_n53) );
  NOR2_X0P5A_A12TR map_u47 ( .A(map_n1631), .B(map_n1630), .Y(map_n54) );
  NAND4_X0P5A_A12TR map_u46 ( .A(map_n51), .B(map_n52), .C(map_n53), .D(
        map_n54), .Y(map_n50) );
  NAND4_X0P5A_A12TR map_u45 ( .A(map_n22), .B(map_n48), .C(map_n49), .D(
        map_n50), .Y(map_n13) );
  OA211_X0P5M_A12TR map_u44 ( .A0(map_n1736), .A1(map_n1737), .B0(map_n1738), 
        .C0(map_n1739), .Y(map_n47) );
  AOI31_X0P5M_A12TR map_u43 ( .A0(map_n1741), .A1(map_n1740), .A2(map_n47), 
        .B0(map_n1742), .Y(map_n37) );
  NOR2_X0P5A_A12TR map_u42 ( .A(map_n1744), .B(map_n1743), .Y(map_n38) );
  OR2_X0P5M_A12TR map_u41 ( .A(map_n1748), .B(map_n1747), .Y(map_n46) );
  NOR3_X0P5A_A12TR map_u40 ( .A(map_n46), .B(map_n1746), .C(map_n1745), .Y(
        map_n39) );
  NOR2_X0P5A_A12TR map_u39 ( .A(map_n1768), .B(map_n1767), .Y(map_n44) );
  OAI21_X0P5M_A12TR map_u38 ( .A0(map_n1764), .A1(map_n1765), .B0(map_n1766), 
        .Y(map_n45) );
  AOI21B_X0P5M_A12TR map_u37 ( .A0(map_n44), .A1(map_n45), .B0N(map_n1769), 
        .Y(map_n43) );
  AOI211_X0P5M_A12TR map_u36 ( .A0(map_n43), .A1(map_n1770), .B0(map_n1772), 
        .C0(map_n1771), .Y(map_n40) );
  OR2_X0P5M_A12TR map_u35 ( .A(map_n1776), .B(map_n1775), .Y(map_n42) );
  NOR3_X0P5A_A12TR map_u34 ( .A(map_n42), .B(map_n1774), .C(map_n1773), .Y(
        map_n41) );
  AOI32_X0P5M_A12TR map_u33 ( .A0(map_n37), .A1(map_n38), .A2(map_n39), .B0(
        map_n40), .B1(map_n41), .Y(map_n36) );
  NAND3_X0P5A_A12TR map_u32 ( .A(map_n22), .B(map_n35), .C(map_n36), .Y(
        map_n18) );
  OA211_X0P5M_A12TR map_u31 ( .A0(map_n1853), .A1(map_n1854), .B0(map_n1855), 
        .C0(map_n1856), .Y(map_n34) );
  AOI31_X0P5M_A12TR map_u30 ( .A0(map_n1858), .A1(map_n1857), .A2(map_n34), 
        .B0(map_n1859), .Y(map_n25) );
  NOR2_X0P5A_A12TR map_u29 ( .A(map_n1861), .B(map_n1860), .Y(map_n26) );
  OR2_X0P5M_A12TR map_u28 ( .A(map_n1865), .B(map_n1864), .Y(map_n33) );
  NOR3_X0P5A_A12TR map_u27 ( .A(map_n33), .B(map_n1863), .C(map_n1862), .Y(
        map_n27) );
  OA21A1OI2_X0P5M_A12TR map_u26 ( .A0(map_n1882), .A1(map_n1881), .B0(
        map_n1883), .C0(map_n1884), .Y(map_n32) );
  NOR2B_X0P5M_A12TR map_u25 ( .AN(map_n1885), .B(map_n32), .Y(map_n30) );
  AND3_X0P5M_A12TR map_u24 ( .A(map_n1888), .B(map_n1887), .C(map_n1889), .Y(
        map_n31) );
  AOI31_X0P5M_A12TR map_u23 ( .A0(map_n30), .A1(map_n1886), .A2(map_n31), .B0(
        map_n1890), .Y(map_n28) );
  NOR3_X0P5A_A12TR map_u22 ( .A(map_n1891), .B(map_n1893), .C(map_n1892), .Y(
        map_n29) );
  AOI32_X0P5M_A12TR map_u21 ( .A0(map_n25), .A1(map_n26), .A2(map_n27), .B0(
        map_n28), .B1(map_n29), .Y(map_n24) );
  NAND3_X0P5A_A12TR map_u20 ( .A(map_n22), .B(map_n23), .C(map_n24), .Y(
        map_n20) );
  NAND2_X0P5A_A12TR map_u19 ( .A(map_n18), .B(map_n20), .Y(map_n12) );
  INV_X0P5B_A12TR map_u18 ( .A(map_n12), .Y(map_n21) );
  OR2_X0P5M_A12TR map_u17 ( .A(n4), .B(n3), .Y(map_n2) );
  AOI31_X0P5M_A12TR map_u16 ( .A0(map_n1), .A1(map_n13), .A2(map_n21), .B0(
        map_n2), .Y(map_n1921) );
  INV_X0P5B_A12TR map_u15 ( .A(map_n11), .Y(map_n15) );
  INV_X0P5B_A12TR map_u14 ( .A(map_n20), .Y(map_n17) );
  INV_X0P5B_A12TR map_u13 ( .A(map_n13), .Y(map_n19) );
  AO21A1AI2_X0P5M_A12TR map_u12 ( .A0(map_n17), .A1(map_n18), .B0(map_n19), 
        .C0(map_n1), .Y(map_n16) );
  OAI211_X0P5M_A12TR map_u11 ( .A0(map_n15), .A1(map_n10), .B0(map_n16), .C0(
        map_n8), .Y(map_n14) );
  MXT2_X0P5M_A12TR map_u10 ( .A(map_n14), .B(button_num[0]), .S0(map_n2), .Y(
        map_n376) );
  NAND2_X0P5A_A12TR map_u9 ( .A(map_n12), .B(map_n13), .Y(map_n6) );
  NAND3B_X0P5M_A12TR map_u8 ( .AN(map_n9), .B(map_n10), .C(map_n11), .Y(map_n7) );
  OAI211_X0P5M_A12TR map_u7 ( .A0(map_n5), .A1(map_n6), .B0(map_n7), .C0(
        map_n8), .Y(map_n3) );
  MXT2_X0P5M_A12TR map_u6 ( .A(map_n3), .B(button_num[1]), .S0(map_n2), .Y(
        map_n377) );
  MXT2_X0P5M_A12TR map_u5 ( .A(map_n1), .B(button_num[2]), .S0(map_n2), .Y(
        map_n378) );
  MXT2_X0P5M_A12TR map_u4 ( .A(map_n1), .B(button_num[3]), .S0(map_n2), .Y(
        map_n379) );
  TIELO_X1M_A12TR map_u3 ( .Y(map_n4) );
  DFFQ_X1M_A12TR map_button_num_reg_0_ ( .D(map_n376), .CK(osc_clk), .Q(
        button_num[0]) );
  DFFQ_X1M_A12TR map_button_num_reg_1_ ( .D(map_n377), .CK(osc_clk), .Q(
        button_num[1]) );
  DFFQ_X1M_A12TR map_button_num_reg_2_ ( .D(map_n378), .CK(osc_clk), .Q(
        button_num[2]) );
  DFFQ_X1M_A12TR map_button_num_reg_3_ ( .D(map_n379), .CK(osc_clk), .Q(
        button_num[3]) );
  DFFQ_X1M_A12TR map_found_match_reg ( .D(map_n1921), .CK(osc_clk), .Q(
        touch_pulse) );
  INV_X1M_A12TR map_add_131_3_i16_u1 ( .A(y_coord[0]), .Y(map_n1881) );
  ADDH_X1M_A12TR map_add_131_3_i16_u1_1_4 ( .A(y_coord[4]), .B(
        map_add_131_3_i16_carry[4]), .CO(map_add_131_3_i16_carry[5]), .S(
        map_n1885) );
  ADDH_X1M_A12TR map_add_131_3_i16_u1_1_5 ( .A(y_coord[5]), .B(
        map_add_131_3_i16_carry[5]), .CO(map_add_131_3_i16_carry[6]), .S(
        map_n1886) );
  ADDH_X1M_A12TR map_add_131_3_i16_u1_1_6 ( .A(y_coord[6]), .B(
        map_add_131_3_i16_carry[6]), .CO(map_add_131_3_i16_carry[7]), .S(
        map_n1887) );
  ADDH_X1M_A12TR map_add_131_3_i16_u1_1_7 ( .A(y_coord[7]), .B(
        map_add_131_3_i16_carry[7]), .CO(map_add_131_3_i16_carry[8]), .S(
        map_n1888) );
  ADDH_X1M_A12TR map_add_131_3_i16_u1_1_3 ( .A(y_coord[3]), .B(
        map_add_131_3_i16_carry[3]), .CO(map_add_131_3_i16_carry[4]), .S(
        map_n1884) );
  ADDH_X1M_A12TR map_add_131_3_i16_u1_1_8 ( .A(y_coord[8]), .B(
        map_add_131_3_i16_carry[8]), .CO(map_add_131_3_i16_carry[9]), .S(
        map_n1889) );
  ADDH_X1M_A12TR map_add_131_3_i16_u1_1_9 ( .A(y_coord[9]), .B(
        map_add_131_3_i16_carry[9]), .CO(map_add_131_3_i16_carry[10]), .S(
        map_n1890) );
  ADDH_X1M_A12TR map_add_131_3_i16_u1_1_10 ( .A(y_coord[10]), .B(
        map_add_131_3_i16_carry[10]), .CO(map_add_131_3_i16_carry[11]), .S(
        map_n1891) );
  ADDH_X1M_A12TR map_add_131_3_i16_u1_1_2 ( .A(y_coord[2]), .B(
        map_add_131_3_i16_carry[2]), .CO(map_add_131_3_i16_carry[3]), .S(
        map_n1883) );
  ADDH_X1M_A12TR map_add_131_3_i16_u1_1_1 ( .A(y_coord[1]), .B(y_coord[0]), 
        .CO(map_add_131_3_i16_carry[2]), .S(map_n1882) );
  ADDH_X1M_A12TR map_add_131_3_i16_u1_1_11 ( .A(y_coord[11]), .B(
        map_add_131_3_i16_carry[11]), .CO(map_n1893), .S(map_n1892) );
  INV_X1M_A12TR map_add_131_i16_u1 ( .A(x_coord[0]), .Y(map_n1853) );
  ADDH_X1M_A12TR map_add_131_i16_u1_1_11 ( .A(x_coord[11]), .B(
        map_add_131_i16_carry[11]), .CO(map_n1865), .S(map_n1864) );
  ADDH_X1M_A12TR map_add_131_i16_u1_1_10 ( .A(x_coord[10]), .B(
        map_add_131_i16_carry[10]), .CO(map_add_131_i16_carry[11]), .S(
        map_n1863) );
  ADDH_X1M_A12TR map_add_131_i16_u1_1_3 ( .A(x_coord[3]), .B(
        map_add_131_i16_carry[3]), .CO(map_add_131_i16_carry[4]), .S(map_n1856) );
  ADDH_X1M_A12TR map_add_131_i16_u1_1_9 ( .A(x_coord[9]), .B(
        map_add_131_i16_carry[9]), .CO(map_add_131_i16_carry[10]), .S(
        map_n1862) );
  ADDH_X1M_A12TR map_add_131_i16_u1_1_4 ( .A(x_coord[4]), .B(
        map_add_131_i16_carry[4]), .CO(map_add_131_i16_carry[5]), .S(map_n1857) );
  ADDH_X1M_A12TR map_add_131_i16_u1_1_5 ( .A(x_coord[5]), .B(
        map_add_131_i16_carry[5]), .CO(map_add_131_i16_carry[6]), .S(map_n1858) );
  ADDH_X1M_A12TR map_add_131_i16_u1_1_6 ( .A(x_coord[6]), .B(
        map_add_131_i16_carry[6]), .CO(map_add_131_i16_carry[7]), .S(map_n1859) );
  ADDH_X1M_A12TR map_add_131_i16_u1_1_8 ( .A(x_coord[8]), .B(
        map_add_131_i16_carry[8]), .CO(map_add_131_i16_carry[9]), .S(map_n1861) );
  ADDH_X1M_A12TR map_add_131_i16_u1_1_7 ( .A(x_coord[7]), .B(
        map_add_131_i16_carry[7]), .CO(map_add_131_i16_carry[8]), .S(map_n1860) );
  ADDH_X1M_A12TR map_add_131_i16_u1_1_2 ( .A(x_coord[2]), .B(
        map_add_131_i16_carry[2]), .CO(map_add_131_i16_carry[3]), .S(map_n1855) );
  ADDH_X1M_A12TR map_add_131_i16_u1_1_1 ( .A(x_coord[1]), .B(x_coord[0]), .CO(
        map_add_131_i16_carry[2]), .S(map_n1854) );
  INV_X1M_A12TR map_add_131_3_i15_u1 ( .A(y_coord[0]), .Y(map_n1764) );
  ADDH_X1M_A12TR map_add_131_3_i15_u1_1_11 ( .A(y_coord[11]), .B(
        map_add_131_3_i15_carry[11]), .CO(map_n1776), .S(map_n1775) );
  ADDH_X1M_A12TR map_add_131_3_i15_u1_1_5 ( .A(y_coord[5]), .B(
        map_add_131_3_i15_carry[5]), .CO(map_add_131_3_i15_carry[6]), .S(
        map_n1769) );
  ADDH_X1M_A12TR map_add_131_3_i15_u1_1_6 ( .A(y_coord[6]), .B(
        map_add_131_3_i15_carry[6]), .CO(map_add_131_3_i15_carry[7]), .S(
        map_n1770) );
  ADDH_X1M_A12TR map_add_131_3_i15_u1_1_4 ( .A(y_coord[4]), .B(
        map_add_131_3_i15_carry[4]), .CO(map_add_131_3_i15_carry[5]), .S(
        map_n1768) );
  ADDH_X1M_A12TR map_add_131_3_i15_u1_1_3 ( .A(y_coord[3]), .B(
        map_add_131_3_i15_carry[3]), .CO(map_add_131_3_i15_carry[4]), .S(
        map_n1767) );
  ADDH_X1M_A12TR map_add_131_3_i15_u1_1_7 ( .A(y_coord[7]), .B(
        map_add_131_3_i15_carry[7]), .CO(map_add_131_3_i15_carry[8]), .S(
        map_n1771) );
  ADDH_X1M_A12TR map_add_131_3_i15_u1_1_8 ( .A(y_coord[8]), .B(
        map_add_131_3_i15_carry[8]), .CO(map_add_131_3_i15_carry[9]), .S(
        map_n1772) );
  ADDH_X1M_A12TR map_add_131_3_i15_u1_1_9 ( .A(y_coord[9]), .B(
        map_add_131_3_i15_carry[9]), .CO(map_add_131_3_i15_carry[10]), .S(
        map_n1773) );
  ADDH_X1M_A12TR map_add_131_3_i15_u1_1_10 ( .A(y_coord[10]), .B(
        map_add_131_3_i15_carry[10]), .CO(map_add_131_3_i15_carry[11]), .S(
        map_n1774) );
  ADDH_X1M_A12TR map_add_131_3_i15_u1_1_2 ( .A(y_coord[2]), .B(
        map_add_131_3_i15_carry[2]), .CO(map_add_131_3_i15_carry[3]), .S(
        map_n1766) );
  ADDH_X1M_A12TR map_add_131_3_i15_u1_1_1 ( .A(y_coord[1]), .B(y_coord[0]), 
        .CO(map_add_131_3_i15_carry[2]), .S(map_n1765) );
  INV_X1M_A12TR map_add_131_i15_u1 ( .A(x_coord[0]), .Y(map_n1736) );
  ADDH_X1M_A12TR map_add_131_i15_u1_1_11 ( .A(x_coord[11]), .B(
        map_add_131_i15_carry[11]), .CO(map_n1748), .S(map_n1747) );
  ADDH_X1M_A12TR map_add_131_i15_u1_1_10 ( .A(x_coord[10]), .B(
        map_add_131_i15_carry[10]), .CO(map_add_131_i15_carry[11]), .S(
        map_n1746) );
  ADDH_X1M_A12TR map_add_131_i15_u1_1_3 ( .A(x_coord[3]), .B(
        map_add_131_i15_carry[3]), .CO(map_add_131_i15_carry[4]), .S(map_n1739) );
  ADDH_X1M_A12TR map_add_131_i15_u1_1_9 ( .A(x_coord[9]), .B(
        map_add_131_i15_carry[9]), .CO(map_add_131_i15_carry[10]), .S(
        map_n1745) );
  ADDH_X1M_A12TR map_add_131_i15_u1_1_4 ( .A(x_coord[4]), .B(
        map_add_131_i15_carry[4]), .CO(map_add_131_i15_carry[5]), .S(map_n1740) );
  ADDH_X1M_A12TR map_add_131_i15_u1_1_5 ( .A(x_coord[5]), .B(
        map_add_131_i15_carry[5]), .CO(map_add_131_i15_carry[6]), .S(map_n1741) );
  ADDH_X1M_A12TR map_add_131_i15_u1_1_6 ( .A(x_coord[6]), .B(
        map_add_131_i15_carry[6]), .CO(map_add_131_i15_carry[7]), .S(map_n1742) );
  ADDH_X1M_A12TR map_add_131_i15_u1_1_8 ( .A(x_coord[8]), .B(
        map_add_131_i15_carry[8]), .CO(map_add_131_i15_carry[9]), .S(map_n1744) );
  ADDH_X1M_A12TR map_add_131_i15_u1_1_7 ( .A(x_coord[7]), .B(
        map_add_131_i15_carry[7]), .CO(map_add_131_i15_carry[8]), .S(map_n1743) );
  ADDH_X1M_A12TR map_add_131_i15_u1_1_2 ( .A(x_coord[2]), .B(
        map_add_131_i15_carry[2]), .CO(map_add_131_i15_carry[3]), .S(map_n1738) );
  ADDH_X1M_A12TR map_add_131_i15_u1_1_1 ( .A(x_coord[1]), .B(x_coord[0]), .CO(
        map_add_131_i15_carry[2]), .S(map_n1737) );
  INV_X1M_A12TR map_add_131_3_i14_u1 ( .A(y_coord[0]), .Y(map_n1647) );
  ADDH_X1M_A12TR map_add_131_3_i14_u1_1_4 ( .A(y_coord[4]), .B(
        map_add_131_3_i14_carry[4]), .CO(map_add_131_3_i14_carry[5]), .S(
        map_n1651) );
  ADDH_X1M_A12TR map_add_131_3_i14_u1_1_5 ( .A(y_coord[5]), .B(
        map_add_131_3_i14_carry[5]), .CO(map_add_131_3_i14_carry[6]), .S(
        map_n1652) );
  ADDH_X1M_A12TR map_add_131_3_i14_u1_1_6 ( .A(y_coord[6]), .B(
        map_add_131_3_i14_carry[6]), .CO(map_add_131_3_i14_carry[7]), .S(
        map_n1653) );
  ADDH_X1M_A12TR map_add_131_3_i14_u1_1_3 ( .A(y_coord[3]), .B(
        map_add_131_3_i14_carry[3]), .CO(map_add_131_3_i14_carry[4]), .S(
        map_n1650) );
  ADDH_X1M_A12TR map_add_131_3_i14_u1_1_7 ( .A(y_coord[7]), .B(
        map_add_131_3_i14_carry[7]), .CO(map_add_131_3_i14_carry[8]), .S(
        map_n1654) );
  ADDH_X1M_A12TR map_add_131_3_i14_u1_1_8 ( .A(y_coord[8]), .B(
        map_add_131_3_i14_carry[8]), .CO(map_add_131_3_i14_carry[9]), .S(
        map_n1655) );
  ADDH_X1M_A12TR map_add_131_3_i14_u1_1_9 ( .A(y_coord[9]), .B(
        map_add_131_3_i14_carry[9]), .CO(map_add_131_3_i14_carry[10]), .S(
        map_n1656) );
  ADDH_X1M_A12TR map_add_131_3_i14_u1_1_10 ( .A(y_coord[10]), .B(
        map_add_131_3_i14_carry[10]), .CO(map_add_131_3_i14_carry[11]), .S(
        map_n1657) );
  ADDH_X1M_A12TR map_add_131_3_i14_u1_1_2 ( .A(y_coord[2]), .B(
        map_add_131_3_i14_carry[2]), .CO(map_add_131_3_i14_carry[3]), .S(
        map_n1649) );
  ADDH_X1M_A12TR map_add_131_3_i14_u1_1_1 ( .A(y_coord[1]), .B(y_coord[0]), 
        .CO(map_add_131_3_i14_carry[2]), .S(map_n1648) );
  ADDH_X1M_A12TR map_add_131_3_i14_u1_1_11 ( .A(y_coord[11]), .B(
        map_add_131_3_i14_carry[11]), .CO(map_n1659), .S(map_n1658) );
  INV_X1M_A12TR map_add_131_i14_u1 ( .A(x_coord[0]), .Y(map_n1619) );
  ADDH_X1M_A12TR map_add_131_i14_u1_1_11 ( .A(x_coord[11]), .B(
        map_add_131_i14_carry[11]), .CO(map_n1631), .S(map_n1630) );
  ADDH_X1M_A12TR map_add_131_i14_u1_1_3 ( .A(x_coord[3]), .B(
        map_add_131_i14_carry[3]), .CO(map_add_131_i14_carry[4]), .S(map_n1622) );
  ADDH_X1M_A12TR map_add_131_i14_u1_1_4 ( .A(x_coord[4]), .B(
        map_add_131_i14_carry[4]), .CO(map_add_131_i14_carry[5]), .S(map_n1623) );
  ADDH_X1M_A12TR map_add_131_i14_u1_1_5 ( .A(x_coord[5]), .B(
        map_add_131_i14_carry[5]), .CO(map_add_131_i14_carry[6]), .S(map_n1624) );
  ADDH_X1M_A12TR map_add_131_i14_u1_1_6 ( .A(x_coord[6]), .B(
        map_add_131_i14_carry[6]), .CO(map_add_131_i14_carry[7]), .S(map_n1625) );
  ADDH_X1M_A12TR map_add_131_i14_u1_1_7 ( .A(x_coord[7]), .B(
        map_add_131_i14_carry[7]), .CO(map_add_131_i14_carry[8]), .S(map_n1626) );
  ADDH_X1M_A12TR map_add_131_i14_u1_1_8 ( .A(x_coord[8]), .B(
        map_add_131_i14_carry[8]), .CO(map_add_131_i14_carry[9]), .S(map_n1627) );
  ADDH_X1M_A12TR map_add_131_i14_u1_1_9 ( .A(x_coord[9]), .B(
        map_add_131_i14_carry[9]), .CO(map_add_131_i14_carry[10]), .S(
        map_n1628) );
  ADDH_X1M_A12TR map_add_131_i14_u1_1_10 ( .A(x_coord[10]), .B(
        map_add_131_i14_carry[10]), .CO(map_add_131_i14_carry[11]), .S(
        map_n1629) );
  ADDH_X1M_A12TR map_add_131_i14_u1_1_2 ( .A(x_coord[2]), .B(
        map_add_131_i14_carry[2]), .CO(map_add_131_i14_carry[3]), .S(map_n1621) );
  ADDH_X1M_A12TR map_add_131_i14_u1_1_1 ( .A(x_coord[1]), .B(x_coord[0]), .CO(
        map_add_131_i14_carry[2]), .S(map_n1620) );
  INV_X1M_A12TR map_add_131_3_i3_u1 ( .A(y_coord[0]), .Y(map_n365) );
  ADDH_X1M_A12TR map_add_131_3_i3_u1_1_4 ( .A(y_coord[4]), .B(
        map_add_131_3_i3_carry[4]), .CO(map_add_131_3_i3_carry[5]), .S(
        map_n369) );
  ADDH_X1M_A12TR map_add_131_3_i3_u1_1_5 ( .A(y_coord[5]), .B(
        map_add_131_3_i3_carry[5]), .CO(map_add_131_3_i3_carry[6]), .S(
        map_n370) );
  ADDH_X1M_A12TR map_add_131_3_i3_u1_1_6 ( .A(y_coord[6]), .B(
        map_add_131_3_i3_carry[6]), .CO(map_add_131_3_i3_carry[7]), .S(
        map_n371) );
  ADDH_X1M_A12TR map_add_131_3_i3_u1_1_7 ( .A(y_coord[7]), .B(
        map_add_131_3_i3_carry[7]), .CO(map_add_131_3_i3_carry[8]), .S(
        map_n372) );
  ADDH_X1M_A12TR map_add_131_3_i3_u1_1_3 ( .A(y_coord[3]), .B(
        map_add_131_3_i3_carry[3]), .CO(map_add_131_3_i3_carry[4]), .S(
        map_n368) );
  ADDH_X1M_A12TR map_add_131_3_i3_u1_1_8 ( .A(y_coord[8]), .B(
        map_add_131_3_i3_carry[8]), .CO(map_add_131_3_i3_carry[9]), .S(
        map_n373) );
  ADDH_X1M_A12TR map_add_131_3_i3_u1_1_9 ( .A(y_coord[9]), .B(
        map_add_131_3_i3_carry[9]), .CO(map_add_131_3_i3_carry[10]), .S(
        map_n374) );
  ADDH_X1M_A12TR map_add_131_3_i3_u1_1_10 ( .A(y_coord[10]), .B(
        map_add_131_3_i3_carry[10]), .CO(map_add_131_3_i3_carry[11]), .S(
        map_n375) );
  ADDH_X1M_A12TR map_add_131_3_i3_u1_1_2 ( .A(y_coord[2]), .B(
        map_add_131_3_i3_carry[2]), .CO(map_add_131_3_i3_carry[3]), .S(
        map_n367) );
  ADDH_X1M_A12TR map_add_131_3_i3_u1_1_1 ( .A(y_coord[1]), .B(y_coord[0]), 
        .CO(map_add_131_3_i3_carry[2]), .S(map_n366) );
  ADDH_X1M_A12TR map_add_131_3_i3_u1_1_11 ( .A(y_coord[11]), .B(
        map_add_131_3_i3_carry[11]), .CO(map_n3770), .S(map_n3760) );
  INV_X1M_A12TR map_add_131_i3_u1 ( .A(x_coord[0]), .Y(map_n337) );
  ADDH_X1M_A12TR map_add_131_i3_u1_1_4 ( .A(x_coord[4]), .B(
        map_add_131_i3_carry[4]), .CO(map_add_131_i3_carry[5]), .S(map_n341)
         );
  ADDH_X1M_A12TR map_add_131_i3_u1_1_5 ( .A(x_coord[5]), .B(
        map_add_131_i3_carry[5]), .CO(map_add_131_i3_carry[6]), .S(map_n342)
         );
  ADDH_X1M_A12TR map_add_131_i3_u1_1_6 ( .A(x_coord[6]), .B(
        map_add_131_i3_carry[6]), .CO(map_add_131_i3_carry[7]), .S(map_n343)
         );
  ADDH_X1M_A12TR map_add_131_i3_u1_1_3 ( .A(x_coord[3]), .B(
        map_add_131_i3_carry[3]), .CO(map_add_131_i3_carry[4]), .S(map_n340)
         );
  ADDH_X1M_A12TR map_add_131_i3_u1_1_7 ( .A(x_coord[7]), .B(
        map_add_131_i3_carry[7]), .CO(map_add_131_i3_carry[8]), .S(map_n344)
         );
  ADDH_X1M_A12TR map_add_131_i3_u1_1_8 ( .A(x_coord[8]), .B(
        map_add_131_i3_carry[8]), .CO(map_add_131_i3_carry[9]), .S(map_n345)
         );
  ADDH_X1M_A12TR map_add_131_i3_u1_1_9 ( .A(x_coord[9]), .B(
        map_add_131_i3_carry[9]), .CO(map_add_131_i3_carry[10]), .S(map_n346)
         );
  ADDH_X1M_A12TR map_add_131_i3_u1_1_10 ( .A(x_coord[10]), .B(
        map_add_131_i3_carry[10]), .CO(map_add_131_i3_carry[11]), .S(map_n347)
         );
  ADDH_X1M_A12TR map_add_131_i3_u1_1_2 ( .A(x_coord[2]), .B(
        map_add_131_i3_carry[2]), .CO(map_add_131_i3_carry[3]), .S(map_n339)
         );
  ADDH_X1M_A12TR map_add_131_i3_u1_1_1 ( .A(x_coord[1]), .B(x_coord[0]), .CO(
        map_add_131_i3_carry[2]), .S(map_n338) );
  ADDH_X1M_A12TR map_add_131_i3_u1_1_11 ( .A(x_coord[11]), .B(
        map_add_131_i3_carry[11]), .CO(map_n349), .S(map_n348) );
  INV_X1M_A12TR map_add_131_3_i2_u1 ( .A(y_coord[0]), .Y(map_n253) );
  ADDH_X1M_A12TR map_add_131_3_i2_u1_1_4 ( .A(y_coord[4]), .B(
        map_add_131_3_i2_carry[4]), .CO(map_add_131_3_i2_carry[5]), .S(
        map_n257) );
  ADDH_X1M_A12TR map_add_131_3_i2_u1_1_5 ( .A(y_coord[5]), .B(
        map_add_131_3_i2_carry[5]), .CO(map_add_131_3_i2_carry[6]), .S(
        map_n258) );
  ADDH_X1M_A12TR map_add_131_3_i2_u1_1_6 ( .A(y_coord[6]), .B(
        map_add_131_3_i2_carry[6]), .CO(map_add_131_3_i2_carry[7]), .S(
        map_n259) );
  ADDH_X1M_A12TR map_add_131_3_i2_u1_1_3 ( .A(y_coord[3]), .B(
        map_add_131_3_i2_carry[3]), .CO(map_add_131_3_i2_carry[4]), .S(
        map_n256) );
  ADDH_X1M_A12TR map_add_131_3_i2_u1_1_7 ( .A(y_coord[7]), .B(
        map_add_131_3_i2_carry[7]), .CO(map_add_131_3_i2_carry[8]), .S(
        map_n260) );
  ADDH_X1M_A12TR map_add_131_3_i2_u1_1_8 ( .A(y_coord[8]), .B(
        map_add_131_3_i2_carry[8]), .CO(map_add_131_3_i2_carry[9]), .S(
        map_n261) );
  ADDH_X1M_A12TR map_add_131_3_i2_u1_1_9 ( .A(y_coord[9]), .B(
        map_add_131_3_i2_carry[9]), .CO(map_add_131_3_i2_carry[10]), .S(
        map_n262) );
  ADDH_X1M_A12TR map_add_131_3_i2_u1_1_10 ( .A(y_coord[10]), .B(
        map_add_131_3_i2_carry[10]), .CO(map_add_131_3_i2_carry[11]), .S(
        map_n263) );
  ADDH_X1M_A12TR map_add_131_3_i2_u1_1_2 ( .A(y_coord[2]), .B(
        map_add_131_3_i2_carry[2]), .CO(map_add_131_3_i2_carry[3]), .S(
        map_n255) );
  ADDH_X1M_A12TR map_add_131_3_i2_u1_1_1 ( .A(y_coord[1]), .B(y_coord[0]), 
        .CO(map_add_131_3_i2_carry[2]), .S(map_n254) );
  ADDH_X1M_A12TR map_add_131_3_i2_u1_1_11 ( .A(y_coord[11]), .B(
        map_add_131_3_i2_carry[11]), .CO(map_n265), .S(map_n264) );
  INV_X1M_A12TR map_add_131_i2_u1 ( .A(x_coord[0]), .Y(map_n225) );
  ADDH_X1M_A12TR map_add_131_i2_u1_1_4 ( .A(x_coord[4]), .B(
        map_add_131_i2_carry[4]), .CO(map_add_131_i2_carry[5]), .S(map_n229)
         );
  ADDH_X1M_A12TR map_add_131_i2_u1_1_5 ( .A(x_coord[5]), .B(
        map_add_131_i2_carry[5]), .CO(map_add_131_i2_carry[6]), .S(map_n230)
         );
  ADDH_X1M_A12TR map_add_131_i2_u1_1_6 ( .A(x_coord[6]), .B(
        map_add_131_i2_carry[6]), .CO(map_add_131_i2_carry[7]), .S(map_n231)
         );
  ADDH_X1M_A12TR map_add_131_i2_u1_1_3 ( .A(x_coord[3]), .B(
        map_add_131_i2_carry[3]), .CO(map_add_131_i2_carry[4]), .S(map_n228)
         );
  ADDH_X1M_A12TR map_add_131_i2_u1_1_7 ( .A(x_coord[7]), .B(
        map_add_131_i2_carry[7]), .CO(map_add_131_i2_carry[8]), .S(map_n232)
         );
  ADDH_X1M_A12TR map_add_131_i2_u1_1_8 ( .A(x_coord[8]), .B(
        map_add_131_i2_carry[8]), .CO(map_add_131_i2_carry[9]), .S(map_n233)
         );
  ADDH_X1M_A12TR map_add_131_i2_u1_1_9 ( .A(x_coord[9]), .B(
        map_add_131_i2_carry[9]), .CO(map_add_131_i2_carry[10]), .S(map_n234)
         );
  ADDH_X1M_A12TR map_add_131_i2_u1_1_10 ( .A(x_coord[10]), .B(
        map_add_131_i2_carry[10]), .CO(map_add_131_i2_carry[11]), .S(map_n235)
         );
  ADDH_X1M_A12TR map_add_131_i2_u1_1_2 ( .A(x_coord[2]), .B(
        map_add_131_i2_carry[2]), .CO(map_add_131_i2_carry[3]), .S(map_n227)
         );
  ADDH_X1M_A12TR map_add_131_i2_u1_1_1 ( .A(x_coord[1]), .B(x_coord[0]), .CO(
        map_add_131_i2_carry[2]), .S(map_n226) );
  ADDH_X1M_A12TR map_add_131_i2_u1_1_11 ( .A(x_coord[11]), .B(
        map_add_131_i2_carry[11]), .CO(map_n237), .S(map_n236) );
  INV_X1M_A12TR map_add_131_3_u1 ( .A(y_coord[0]), .Y(map_n146) );
  ADDH_X1M_A12TR map_add_131_3_u1_1_5 ( .A(y_coord[5]), .B(
        map_add_131_3_carry[5]), .CO(map_add_131_3_carry[6]), .S(map_n151) );
  ADDH_X1M_A12TR map_add_131_3_u1_1_6 ( .A(y_coord[6]), .B(
        map_add_131_3_carry[6]), .CO(map_add_131_3_carry[7]), .S(map_n152) );
  ADDH_X1M_A12TR map_add_131_3_u1_1_4 ( .A(y_coord[4]), .B(
        map_add_131_3_carry[4]), .CO(map_add_131_3_carry[5]), .S(map_n150) );
  ADDH_X1M_A12TR map_add_131_3_u1_1_3 ( .A(y_coord[3]), .B(
        map_add_131_3_carry[3]), .CO(map_add_131_3_carry[4]), .S(map_n149) );
  ADDH_X1M_A12TR map_add_131_3_u1_1_7 ( .A(y_coord[7]), .B(
        map_add_131_3_carry[7]), .CO(map_add_131_3_carry[8]), .S(map_n153) );
  ADDH_X1M_A12TR map_add_131_3_u1_1_8 ( .A(y_coord[8]), .B(
        map_add_131_3_carry[8]), .CO(map_add_131_3_carry[9]), .S(map_n154) );
  ADDH_X1M_A12TR map_add_131_3_u1_1_10 ( .A(y_coord[10]), .B(
        map_add_131_3_carry[10]), .CO(map_add_131_3_carry[11]), .S(map_n156)
         );
  ADDH_X1M_A12TR map_add_131_3_u1_1_9 ( .A(y_coord[9]), .B(
        map_add_131_3_carry[9]), .CO(map_add_131_3_carry[10]), .S(map_n155) );
  ADDH_X1M_A12TR map_add_131_3_u1_1_2 ( .A(y_coord[2]), .B(
        map_add_131_3_carry[2]), .CO(map_add_131_3_carry[3]), .S(map_n148) );
  ADDH_X1M_A12TR map_add_131_3_u1_1_1 ( .A(y_coord[1]), .B(y_coord[0]), .CO(
        map_add_131_3_carry[2]), .S(map_n147) );
  ADDH_X1M_A12TR map_add_131_3_u1_1_11 ( .A(y_coord[11]), .B(
        map_add_131_3_carry[11]), .CO(map_n158), .S(map_n157) );
  INV_X1M_A12TR map_add_131_u1 ( .A(x_coord[0]), .Y(map_n1180) );
  ADDH_X1M_A12TR map_add_131_u1_1_11 ( .A(x_coord[11]), .B(
        map_add_131_carry[11]), .CO(map_n130), .S(map_n129) );
  ADDH_X1M_A12TR map_add_131_u1_1_10 ( .A(x_coord[10]), .B(
        map_add_131_carry[10]), .CO(map_add_131_carry[11]), .S(map_n128) );
  ADDH_X1M_A12TR map_add_131_u1_1_4 ( .A(x_coord[4]), .B(map_add_131_carry[4]), 
        .CO(map_add_131_carry[5]), .S(map_n1220) );
  ADDH_X1M_A12TR map_add_131_u1_1_5 ( .A(x_coord[5]), .B(map_add_131_carry[5]), 
        .CO(map_add_131_carry[6]), .S(map_n1230) );
  ADDH_X1M_A12TR map_add_131_u1_1_3 ( .A(x_coord[3]), .B(map_add_131_carry[3]), 
        .CO(map_add_131_carry[4]), .S(map_n1210) );
  ADDH_X1M_A12TR map_add_131_u1_1_6 ( .A(x_coord[6]), .B(map_add_131_carry[6]), 
        .CO(map_add_131_carry[7]), .S(map_n1240) );
  ADDH_X1M_A12TR map_add_131_u1_1_7 ( .A(x_coord[7]), .B(map_add_131_carry[7]), 
        .CO(map_add_131_carry[8]), .S(map_n1250) );
  ADDH_X1M_A12TR map_add_131_u1_1_8 ( .A(x_coord[8]), .B(map_add_131_carry[8]), 
        .CO(map_add_131_carry[9]), .S(map_n1260) );
  ADDH_X1M_A12TR map_add_131_u1_1_9 ( .A(x_coord[9]), .B(map_add_131_carry[9]), 
        .CO(map_add_131_carry[10]), .S(map_n1270) );
  ADDH_X1M_A12TR map_add_131_u1_1_2 ( .A(x_coord[2]), .B(map_add_131_carry[2]), 
        .CO(map_add_131_carry[3]), .S(map_n1200) );
  ADDH_X1M_A12TR map_add_131_u1_1_1 ( .A(x_coord[1]), .B(x_coord[0]), .CO(
        map_add_131_carry[2]), .S(map_n1190) );
  INV_X0P5B_A12TR controller_u35 ( .A(contest_num[0]), .Y(controller_n20) );
  INV_X0P5B_A12TR controller_u34 ( .A(contest_num[1]), .Y(controller_n12) );
  AOI31_X0P5M_A12TR controller_u33 ( .A0(controller_n20), .A1(controller_n12), 
        .A2(controller_n13), .B0(button_num[0]), .Y(controller_n28) );
  NAND2_X0P5A_A12TR controller_u32 ( .A(button_num[3]), .B(button_num[2]), .Y(
        controller_n1) );
  INV_X0P5B_A12TR controller_u31 ( .A(controller_n1), .Y(controller_n5) );
  INV_X0P5B_A12TR controller_u30 ( .A(controller_cast), .Y(controller_n2) );
  AND3_X0P5M_A12TR controller_u29 ( .A(controller_n5), .B(controller_n2), .C(
        button_num[1]), .Y(controller_n27) );
  NAND2_X0P5A_A12TR controller_u28 ( .A(controller_n28), .B(controller_n27), 
        .Y(controller_n17) );
  INV_X0P5B_A12TR controller_u27 ( .A(controller_n13), .Y(contest_num[2]) );
  NAND3_X0P5A_A12TR controller_u26 ( .A(contest_num[0]), .B(contest_num[2]), 
        .C(contest_num[1]), .Y(controller_n25) );
  NAND3_X0P5A_A12TR controller_u25 ( .A(button_num[0]), .B(controller_n25), 
        .C(controller_n27), .Y(controller_n21) );
  NAND2_X0P5A_A12TR controller_u24 ( .A(controller_n17), .B(controller_n21), 
        .Y(controller_n11) );
  INV_X0P5B_A12TR controller_u23 ( .A(touch_pulse), .Y(controller_n7) );
  INV_X0P5B_A12TR controller_u22 ( .A(n4), .Y(controller_n3) );
  NAND2_X0P5A_A12TR controller_u21 ( .A(controller_n7), .B(controller_n3), .Y(
        controller_n14) );
  OA21_X0P5M_A12TR controller_u20 ( .A0(n4), .A1(controller_n11), .B0(
        controller_n14), .Y(controller_n9) );
  NAND2_X0P5A_A12TR controller_u19 ( .A(controller_n14), .B(controller_n3), 
        .Y(controller_n10) );
  XOR2_X0P5M_A12TR controller_u18 ( .A(controller_n20), .B(contest_num[1]), 
        .Y(controller_n26) );
  INV_X0P5B_A12TR controller_u17 ( .A(controller_n17), .Y(controller_n24) );
  NAND2_X0P5A_A12TR controller_u16 ( .A(controller_n26), .B(controller_n24), 
        .Y(controller_n18) );
  OAI31_X0P5M_A12TR controller_u15 ( .A0(controller_n12), .A1(controller_n24), 
        .A2(controller_n20), .B0(controller_n25), .Y(controller_n23) );
  AO21A1AI2_X0P5M_A12TR controller_u14 ( .A0(controller_n18), .A1(
        contest_num[2]), .B0(controller_n23), .C0(controller_n11), .Y(
        controller_n22) );
  OAI22_X0P5M_A12TR controller_u13 ( .A0(controller_n13), .A1(controller_n9), 
        .B0(controller_n10), .B1(controller_n22), .Y(controller_n44) );
  NOR2_X0P5A_A12TR controller_u12 ( .A(controller_n20), .B(controller_n21), 
        .Y(controller_n19) );
  XOR2_X0P5M_A12TR controller_u11 ( .A(contest_num[1]), .B(controller_n19), 
        .Y(controller_n16) );
  AOI21B_X0P5M_A12TR controller_u10 ( .A0(controller_n16), .A1(controller_n17), 
        .B0N(controller_n18), .Y(controller_n15) );
  OAI22_X0P5M_A12TR controller_u9 ( .A0(controller_n12), .A1(controller_n14), 
        .B0(controller_n15), .B1(controller_n10), .Y(controller_n45) );
  NAND2B_X0P5M_A12TR controller_u8 ( .AN(controller_n10), .B(controller_n11), 
        .Y(controller_n8) );
  MXIT2_X0P5M_A12TR controller_u7 ( .A(controller_n8), .B(controller_n9), .S0(
        contest_num[0]), .Y(controller_n46) );
  NOR2_X0P5A_A12TR controller_u6 ( .A(button_num[1]), .B(controller_n7), .Y(
        controller_n6) );
  AOI31_X0P5M_A12TR controller_u5 ( .A0(button_num[0]), .A1(controller_n5), 
        .A2(controller_n6), .B0(n4), .Y(controller_n4) );
  MXIT2_X0P5M_A12TR controller_u4 ( .A(n4), .B(controller_n2), .S0(
        controller_n4), .Y(controller_n47) );
  AND4_X0P5M_A12TR controller_u3 ( .A(touch_pulse), .B(controller_n1), .C(
        controller_n2), .D(controller_n3), .Y(ss_enable) );
  DFFQN_X1M_A12TR controller_contest_num_reg_2_ ( .D(controller_n44), .CK(
        osc_clk), .QN(controller_n13) );
  DFFQ_X1M_A12TR controller_contest_num_reg_1_ ( .D(controller_n45), .CK(
        osc_clk), .Q(contest_num[1]) );
  DFFQ_X1M_A12TR controller_contest_num_reg_0_ ( .D(controller_n46), .CK(
        osc_clk), .Q(contest_num[0]) );
  DFFQ_X1M_A12TR controller_cast_reg ( .D(controller_n47), .CK(osc_clk), .Q(
        controller_cast) );
  NAND2B_X0P5M_A12TR demux_1to8_u12 ( .AN(contest_num[2]), .B(ss_enable), .Y(
        demux_1to8_n4) );
  NOR3_X0P5A_A12TR demux_1to8_u11 ( .A(demux_1to8_n4), .B(contest_num[1]), .C(
        contest_num[0]), .Y(ss_selector[0]) );
  INV_X0P5B_A12TR demux_1to8_u10 ( .A(contest_num[0]), .Y(demux_1to8_n3) );
  NOR3_X0P5A_A12TR demux_1to8_u9 ( .A(demux_1to8_n4), .B(contest_num[1]), .C(
        demux_1to8_n3), .Y(ss_selector[1]) );
  INV_X0P5B_A12TR demux_1to8_u8 ( .A(contest_num[1]), .Y(demux_1to8_n1) );
  NOR3_X0P5A_A12TR demux_1to8_u7 ( .A(demux_1to8_n4), .B(contest_num[0]), .C(
        demux_1to8_n1), .Y(ss_selector[2]) );
  NOR3_X0P5A_A12TR demux_1to8_u6 ( .A(demux_1to8_n4), .B(demux_1to8_n3), .C(
        demux_1to8_n1), .Y(ss_selector[3]) );
  NAND2_X0P5A_A12TR demux_1to8_u5 ( .A(contest_num[2]), .B(ss_enable), .Y(
        demux_1to8_n2) );
  NOR3_X0P5A_A12TR demux_1to8_u4 ( .A(demux_1to8_n2), .B(contest_num[1]), .C(
        contest_num[0]), .Y(ss_selector[4]) );
  NOR3_X0P5A_A12TR demux_1to8_u3 ( .A(demux_1to8_n3), .B(contest_num[1]), .C(
        demux_1to8_n2), .Y(ss_selector[5]) );
  NOR3_X0P5A_A12TR demux_1to8_u2 ( .A(demux_1to8_n1), .B(contest_num[0]), .C(
        demux_1to8_n2), .Y(ss_selector[6]) );
  NOR3_X0P5A_A12TR demux_1to8_u1 ( .A(demux_1to8_n1), .B(demux_1to8_n2), .C(
        demux_1to8_n3), .Y(ss_selector[7]) );
  INV_X0P5B_A12TR ss0_u105 ( .A(ss_state_0[11]), .Y(ss0_n47) );
  INV_X0P5B_A12TR ss0_u104 ( .A(ss_state_0[7]), .Y(ss0_n32) );
  INV_X0P5B_A12TR ss0_u103 ( .A(ss_state_0[8]), .Y(ss0_n35) );
  XNOR2_X0P5M_A12TR ss0_u102 ( .A(ss0_n32), .B(ss0_n35), .Y(ss0_n81) );
  INV_X0P5B_A12TR ss0_u101 ( .A(ss0_n81), .Y(ss0_n91) );
  AOI22_X0P5M_A12TR ss0_u100 ( .A0(ss_state_0[6]), .A1(ss0_n91), .B0(
        ss_state_0[7]), .B1(ss_state_0[8]), .Y(ss0_n62) );
  XOR2_X0P5M_A12TR ss0_u99 ( .A(ss_state_0[6]), .B(ss0_n91), .Y(ss0_n84) );
  INV_X0P5B_A12TR ss0_u98 ( .A(ss_state_0[10]), .Y(ss0_n44) );
  XNOR2_X0P5M_A12TR ss0_u97 ( .A(ss0_n44), .B(ss0_n47), .Y(ss0_n80) );
  INV_X0P5B_A12TR ss0_u96 ( .A(ss0_n80), .Y(ss0_n90) );
  XOR2_X0P5M_A12TR ss0_u95 ( .A(ss_state_0[9]), .B(ss0_n90), .Y(ss0_n85) );
  NAND2_X0P5A_A12TR ss0_u94 ( .A(ss0_n84), .B(ss0_n85), .Y(ss0_n64) );
  AOI22_X0P5M_A12TR ss0_u93 ( .A0(ss_state_0[10]), .A1(ss_state_0[11]), .B0(
        ss_state_0[9]), .B1(ss0_n90), .Y(ss0_n63) );
  CGEN_X1M_A12TR ss0_u92 ( .A(ss0_n62), .B(ss0_n64), .CI(ss0_n63), .CO(ss0_n75) );
  INV_X0P5B_A12TR ss0_u91 ( .A(ss_state_0[4]), .Y(ss0_n22) );
  INV_X0P5B_A12TR ss0_u90 ( .A(ss_state_0[5]), .Y(ss0_n26) );
  XOR2_X0P5M_A12TR ss0_u89 ( .A(ss0_n22), .B(ss0_n26), .Y(ss0_n88) );
  AOI22_X0P5M_A12TR ss0_u88 ( .A0(ss_state_0[5]), .A1(ss_state_0[4]), .B0(
        ss0_n88), .B1(ss_state_0[3]), .Y(ss0_n86) );
  INV_X0P5B_A12TR ss0_u87 ( .A(ss_state_0[1]), .Y(ss0_n7) );
  INV_X0P5B_A12TR ss0_u86 ( .A(ss_state_0[2]), .Y(ss0_n11) );
  XOR2_X0P5M_A12TR ss0_u85 ( .A(ss0_n7), .B(ss0_n11), .Y(ss0_n89) );
  AOI22_X0P5M_A12TR ss0_u84 ( .A0(ss_state_0[2]), .A1(ss_state_0[1]), .B0(
        ss0_n89), .B1(ss_state_0[0]), .Y(ss0_n87) );
  XNOR2_X0P5M_A12TR ss0_u83 ( .A(ss0_n87), .B(ss0_n86), .Y(ss0_n73) );
  XOR2_X0P5M_A12TR ss0_u82 ( .A(ss_state_0[0]), .B(ss0_n89), .Y(ss0_n82) );
  XOR2_X0P5M_A12TR ss0_u81 ( .A(ss_state_0[3]), .B(ss0_n88), .Y(ss0_n83) );
  NAND2_X0P5A_A12TR ss0_u80 ( .A(ss0_n82), .B(ss0_n83), .Y(ss0_n74) );
  OAI22_X0P5M_A12TR ss0_u79 ( .A0(ss0_n86), .A1(ss0_n87), .B0(ss0_n73), .B1(
        ss0_n74), .Y(ss0_n60) );
  INV_X0P5B_A12TR ss0_u78 ( .A(ss0_n60), .Y(ss0_n76) );
  XOR2_X0P5M_A12TR ss0_u77 ( .A(ss0_n74), .B(ss0_n73), .Y(ss0_n77) );
  OAI21_X0P5M_A12TR ss0_u76 ( .A0(ss0_n84), .A1(ss0_n85), .B0(ss0_n64), .Y(
        ss0_n68) );
  OAI21_X0P5M_A12TR ss0_u75 ( .A0(ss0_n82), .A1(ss0_n83), .B0(ss0_n74), .Y(
        ss0_n69) );
  NOR2_X0P5A_A12TR ss0_u74 ( .A(ss0_n68), .B(ss0_n69), .Y(ss0_n70) );
  INV_X0P5B_A12TR ss0_u73 ( .A(ss_state_0[6]), .Y(ss0_n29) );
  OAI22_X0P5M_A12TR ss0_u72 ( .A0(ss0_n35), .A1(ss0_n32), .B0(ss0_n81), .B1(
        ss0_n29), .Y(ss0_n78) );
  INV_X0P5B_A12TR ss0_u71 ( .A(ss_state_0[9]), .Y(ss0_n41) );
  OAI22_X0P5M_A12TR ss0_u70 ( .A0(ss0_n80), .A1(ss0_n41), .B0(ss0_n47), .B1(
        ss0_n44), .Y(ss0_n79) );
  XNOR3_X0P5M_A12TR ss0_u69 ( .A(ss0_n64), .B(ss0_n78), .C(ss0_n79), .Y(
        ss0_n71) );
  CGENI_X1M_A12TR ss0_u68 ( .A(ss0_n77), .B(ss0_n70), .CI(ss0_n71), .CON(
        ss0_n59) );
  CGEN_X1M_A12TR ss0_u67 ( .A(ss0_n75), .B(ss0_n76), .CI(ss0_n59), .CO(ss0_n54) );
  XNOR2_X0P5M_A12TR ss0_u66 ( .A(ss0_n73), .B(ss0_n74), .Y(ss0_n72) );
  XOR3_X0P5M_A12TR ss0_u65 ( .A(ss0_n71), .B(ss0_n70), .C(ss0_n72), .Y(ss0_n65) );
  AO21_X0P5M_A12TR ss0_u64 ( .A0(ss0_n68), .A1(ss0_n69), .B0(ss0_n70), .Y(
        ss0_n67) );
  OA211_X0P5M_A12TR ss0_u63 ( .A0(max_selections[1]), .A1(ss0_n65), .B0(
        ss0_n67), .C0(max_selections[0]), .Y(ss0_n66) );
  AOI21_X0P5M_A12TR ss0_u62 ( .A0(ss0_n65), .A1(max_selections[1]), .B0(
        ss0_n66), .Y(ss0_n56) );
  CGENI_X1M_A12TR ss0_u61 ( .A(ss0_n62), .B(ss0_n63), .CI(ss0_n64), .CON(
        ss0_n61) );
  XNOR3_X0P5M_A12TR ss0_u60 ( .A(ss0_n59), .B(ss0_n60), .C(ss0_n61), .Y(
        ss0_n57) );
  AO1B2_X0P5M_A12TR ss0_u59 ( .B0(ss0_n57), .B1(ss0_n56), .A0N(
        max_selections[2]), .Y(ss0_n58) );
  OAI21_X0P5M_A12TR ss0_u58 ( .A0(ss0_n56), .A1(ss0_n57), .B0(ss0_n58), .Y(
        ss0_n55) );
  AND2_X0P5M_A12TR ss0_u57 ( .A(ss0_n54), .B(ss0_n55), .Y(ss0_n53) );
  INV_X0P5B_A12TR ss0_u56 ( .A(n4), .Y(ss0_n51) );
  OAI221_X0P5M_A12TR ss0_u55 ( .A0(max_selections[3]), .A1(ss0_n53), .B0(
        ss0_n54), .B1(ss0_n55), .C0(ss0_n51), .Y(ss0_n52) );
  INV_X0P5B_A12TR ss0_u54 ( .A(ss0_n52), .Y(ss0_n6) );
  NAND2_X0P5A_A12TR ss0_u53 ( .A(ss0_n6), .B(ss0_n47), .Y(ss0_n48) );
  AOI21_X0P5M_A12TR ss0_u52 ( .A0(button_num[0]), .A1(button_num[1]), .B0(n4), 
        .Y(ss0_n18) );
  INV_X0P5B_A12TR ss0_u51 ( .A(ss_selector[0]), .Y(ss0_n50) );
  AO21A1AI2_X0P5M_A12TR ss0_u50 ( .A0(button_num[3]), .A1(button_num[2]), .B0(
        ss0_n50), .C0(ss0_n51), .Y(ss0_n21) );
  OAI21_X0P5M_A12TR ss0_u49 ( .A0(n4), .A1(button_num[3]), .B0(ss0_n21), .Y(
        ss0_n38) );
  NOR2_X0P5A_A12TR ss0_u48 ( .A(ss0_n18), .B(ss0_n38), .Y(ss0_n49) );
  MXIT2_X0P5M_A12TR ss0_u47 ( .A(ss0_n47), .B(ss0_n48), .S0(ss0_n49), .Y(
        ss0_n422) );
  NAND2_X0P5A_A12TR ss0_u46 ( .A(ss0_n6), .B(ss0_n44), .Y(ss0_n45) );
  INV_X0P5B_A12TR ss0_u45 ( .A(button_num[0]), .Y(ss0_n39) );
  AOI21_X0P5M_A12TR ss0_u44 ( .A0(ss0_n39), .A1(button_num[1]), .B0(n4), .Y(
        ss0_n14) );
  NOR2_X0P5A_A12TR ss0_u43 ( .A(ss0_n14), .B(ss0_n38), .Y(ss0_n46) );
  MXIT2_X0P5M_A12TR ss0_u42 ( .A(ss0_n44), .B(ss0_n45), .S0(ss0_n46), .Y(
        ss0_n423) );
  NAND2_X0P5A_A12TR ss0_u41 ( .A(ss0_n6), .B(ss0_n41), .Y(ss0_n42) );
  INV_X0P5B_A12TR ss0_u40 ( .A(button_num[1]), .Y(ss0_n40) );
  AOI21_X0P5M_A12TR ss0_u39 ( .A0(ss0_n40), .A1(button_num[0]), .B0(n4), .Y(
        ss0_n10) );
  NOR2_X0P5A_A12TR ss0_u38 ( .A(ss0_n10), .B(ss0_n38), .Y(ss0_n43) );
  MXIT2_X0P5M_A12TR ss0_u37 ( .A(ss0_n41), .B(ss0_n42), .S0(ss0_n43), .Y(
        ss0_n424) );
  NAND2_X0P5A_A12TR ss0_u36 ( .A(ss0_n6), .B(ss0_n35), .Y(ss0_n36) );
  AOI21_X0P5M_A12TR ss0_u35 ( .A0(ss0_n39), .A1(ss0_n40), .B0(n4), .Y(ss0_n4)
         );
  NOR2_X0P5A_A12TR ss0_u34 ( .A(ss0_n4), .B(ss0_n38), .Y(ss0_n37) );
  MXIT2_X0P5M_A12TR ss0_u33 ( .A(ss0_n35), .B(ss0_n36), .S0(ss0_n37), .Y(
        ss0_n425) );
  NAND2_X0P5A_A12TR ss0_u32 ( .A(ss0_n6), .B(ss0_n32), .Y(ss0_n33) );
  OAI21_X0P5M_A12TR ss0_u31 ( .A0(n4), .A1(button_num[2]), .B0(ss0_n21), .Y(
        ss0_n25) );
  NOR2_X0P5A_A12TR ss0_u30 ( .A(ss0_n18), .B(ss0_n25), .Y(ss0_n34) );
  MXIT2_X0P5M_A12TR ss0_u29 ( .A(ss0_n32), .B(ss0_n33), .S0(ss0_n34), .Y(
        ss0_n426) );
  NAND2_X0P5A_A12TR ss0_u28 ( .A(ss0_n6), .B(ss0_n29), .Y(ss0_n30) );
  NOR2_X0P5A_A12TR ss0_u27 ( .A(ss0_n14), .B(ss0_n25), .Y(ss0_n31) );
  MXIT2_X0P5M_A12TR ss0_u26 ( .A(ss0_n29), .B(ss0_n30), .S0(ss0_n31), .Y(
        ss0_n427) );
  NAND2_X0P5A_A12TR ss0_u25 ( .A(ss0_n6), .B(ss0_n26), .Y(ss0_n27) );
  NOR2_X0P5A_A12TR ss0_u24 ( .A(ss0_n10), .B(ss0_n25), .Y(ss0_n28) );
  MXIT2_X0P5M_A12TR ss0_u23 ( .A(ss0_n26), .B(ss0_n27), .S0(ss0_n28), .Y(
        ss0_n428) );
  NAND2_X0P5A_A12TR ss0_u22 ( .A(ss0_n6), .B(ss0_n22), .Y(ss0_n23) );
  NOR2_X0P5A_A12TR ss0_u21 ( .A(ss0_n4), .B(ss0_n25), .Y(ss0_n24) );
  MXIT2_X0P5M_A12TR ss0_u20 ( .A(ss0_n22), .B(ss0_n23), .S0(ss0_n24), .Y(
        ss0_n429) );
  INV_X0P5B_A12TR ss0_u19 ( .A(ss_state_0[3]), .Y(ss0_n15) );
  NAND2_X0P5A_A12TR ss0_u18 ( .A(ss0_n6), .B(ss0_n15), .Y(ss0_n16) );
  INV_X0P5B_A12TR ss0_u17 ( .A(button_num[3]), .Y(ss0_n19) );
  INV_X0P5B_A12TR ss0_u16 ( .A(button_num[2]), .Y(ss0_n20) );
  AO21A1AI2_X0P5M_A12TR ss0_u15 ( .A0(ss0_n19), .A1(ss0_n20), .B0(n4), .C0(
        ss0_n21), .Y(ss0_n5) );
  NOR2_X0P5A_A12TR ss0_u14 ( .A(ss0_n18), .B(ss0_n5), .Y(ss0_n17) );
  MXIT2_X0P5M_A12TR ss0_u13 ( .A(ss0_n15), .B(ss0_n16), .S0(ss0_n17), .Y(
        ss0_n430) );
  NAND2_X0P5A_A12TR ss0_u12 ( .A(ss0_n6), .B(ss0_n11), .Y(ss0_n12) );
  NOR2_X0P5A_A12TR ss0_u11 ( .A(ss0_n14), .B(ss0_n5), .Y(ss0_n13) );
  MXIT2_X0P5M_A12TR ss0_u10 ( .A(ss0_n11), .B(ss0_n12), .S0(ss0_n13), .Y(
        ss0_n431) );
  NAND2_X0P5A_A12TR ss0_u9 ( .A(ss0_n6), .B(ss0_n7), .Y(ss0_n8) );
  NOR2_X0P5A_A12TR ss0_u8 ( .A(ss0_n10), .B(ss0_n5), .Y(ss0_n9) );
  MXIT2_X0P5M_A12TR ss0_u7 ( .A(ss0_n7), .B(ss0_n8), .S0(ss0_n9), .Y(ss0_n432)
         );
  INV_X0P5B_A12TR ss0_u6 ( .A(ss_state_0[0]), .Y(ss0_n1) );
  NAND2_X0P5A_A12TR ss0_u5 ( .A(ss0_n6), .B(ss0_n1), .Y(ss0_n2) );
  NOR2_X0P5A_A12TR ss0_u4 ( .A(ss0_n4), .B(ss0_n5), .Y(ss0_n3) );
  MXIT2_X0P5M_A12TR ss0_u3 ( .A(ss0_n1), .B(ss0_n2), .S0(ss0_n3), .Y(ss0_n433)
         );
  DFFQ_X1M_A12TR ss0_selection_state_reg_3_ ( .D(ss0_n430), .CK(osc_clk), .Q(
        ss_state_0[3]) );
  DFFQ_X1M_A12TR ss0_selection_state_reg_0_ ( .D(ss0_n433), .CK(osc_clk), .Q(
        ss_state_0[0]) );
  DFFQ_X1M_A12TR ss0_selection_state_reg_6_ ( .D(ss0_n427), .CK(osc_clk), .Q(
        ss_state_0[6]) );
  DFFQ_X1M_A12TR ss0_selection_state_reg_9_ ( .D(ss0_n424), .CK(osc_clk), .Q(
        ss_state_0[9]) );
  DFFQ_X1M_A12TR ss0_selection_state_reg_8_ ( .D(ss0_n425), .CK(osc_clk), .Q(
        ss_state_0[8]) );
  DFFQ_X1M_A12TR ss0_selection_state_reg_1_ ( .D(ss0_n432), .CK(osc_clk), .Q(
        ss_state_0[1]) );
  DFFQ_X1M_A12TR ss0_selection_state_reg_4_ ( .D(ss0_n429), .CK(osc_clk), .Q(
        ss_state_0[4]) );
  DFFQ_X1M_A12TR ss0_selection_state_reg_11_ ( .D(ss0_n422), .CK(osc_clk), .Q(
        ss_state_0[11]) );
  DFFQ_X1M_A12TR ss0_selection_state_reg_2_ ( .D(ss0_n431), .CK(osc_clk), .Q(
        ss_state_0[2]) );
  DFFQ_X1M_A12TR ss0_selection_state_reg_5_ ( .D(ss0_n428), .CK(osc_clk), .Q(
        ss_state_0[5]) );
  DFFQ_X1M_A12TR ss0_selection_state_reg_10_ ( .D(ss0_n423), .CK(osc_clk), .Q(
        ss_state_0[10]) );
  DFFQ_X1M_A12TR ss0_selection_state_reg_7_ ( .D(ss0_n426), .CK(osc_clk), .Q(
        ss_state_0[7]) );
  INV_X0P5B_A12TR ss1_u105 ( .A(ss_state_1[11]), .Y(ss1_n47) );
  INV_X0P5B_A12TR ss1_u104 ( .A(ss_state_1[7]), .Y(ss1_n32) );
  INV_X0P5B_A12TR ss1_u103 ( .A(ss_state_1[8]), .Y(ss1_n35) );
  XNOR2_X0P5M_A12TR ss1_u102 ( .A(ss1_n32), .B(ss1_n35), .Y(ss1_n81) );
  INV_X0P5B_A12TR ss1_u101 ( .A(ss1_n81), .Y(ss1_n91) );
  AOI22_X0P5M_A12TR ss1_u100 ( .A0(ss_state_1[6]), .A1(ss1_n91), .B0(
        ss_state_1[7]), .B1(ss_state_1[8]), .Y(ss1_n62) );
  XOR2_X0P5M_A12TR ss1_u99 ( .A(ss_state_1[6]), .B(ss1_n91), .Y(ss1_n84) );
  INV_X0P5B_A12TR ss1_u98 ( .A(ss_state_1[10]), .Y(ss1_n44) );
  XNOR2_X0P5M_A12TR ss1_u97 ( .A(ss1_n44), .B(ss1_n47), .Y(ss1_n80) );
  INV_X0P5B_A12TR ss1_u96 ( .A(ss1_n80), .Y(ss1_n90) );
  XOR2_X0P5M_A12TR ss1_u95 ( .A(ss_state_1[9]), .B(ss1_n90), .Y(ss1_n85) );
  NAND2_X0P5A_A12TR ss1_u94 ( .A(ss1_n84), .B(ss1_n85), .Y(ss1_n64) );
  AOI22_X0P5M_A12TR ss1_u93 ( .A0(ss_state_1[10]), .A1(ss_state_1[11]), .B0(
        ss_state_1[9]), .B1(ss1_n90), .Y(ss1_n63) );
  CGEN_X1M_A12TR ss1_u92 ( .A(ss1_n62), .B(ss1_n64), .CI(ss1_n63), .CO(ss1_n75) );
  INV_X0P5B_A12TR ss1_u91 ( .A(ss_state_1[4]), .Y(ss1_n22) );
  INV_X0P5B_A12TR ss1_u90 ( .A(ss_state_1[5]), .Y(ss1_n26) );
  XOR2_X0P5M_A12TR ss1_u89 ( .A(ss1_n22), .B(ss1_n26), .Y(ss1_n88) );
  AOI22_X0P5M_A12TR ss1_u88 ( .A0(ss_state_1[5]), .A1(ss_state_1[4]), .B0(
        ss1_n88), .B1(ss_state_1[3]), .Y(ss1_n86) );
  INV_X0P5B_A12TR ss1_u87 ( .A(ss_state_1[1]), .Y(ss1_n7) );
  INV_X0P5B_A12TR ss1_u86 ( .A(ss_state_1[2]), .Y(ss1_n11) );
  XOR2_X0P5M_A12TR ss1_u85 ( .A(ss1_n7), .B(ss1_n11), .Y(ss1_n89) );
  AOI22_X0P5M_A12TR ss1_u84 ( .A0(ss_state_1[2]), .A1(ss_state_1[1]), .B0(
        ss1_n89), .B1(ss_state_1[0]), .Y(ss1_n87) );
  XNOR2_X0P5M_A12TR ss1_u83 ( .A(ss1_n87), .B(ss1_n86), .Y(ss1_n73) );
  XOR2_X0P5M_A12TR ss1_u82 ( .A(ss_state_1[0]), .B(ss1_n89), .Y(ss1_n82) );
  XOR2_X0P5M_A12TR ss1_u81 ( .A(ss_state_1[3]), .B(ss1_n88), .Y(ss1_n83) );
  NAND2_X0P5A_A12TR ss1_u80 ( .A(ss1_n82), .B(ss1_n83), .Y(ss1_n74) );
  OAI22_X0P5M_A12TR ss1_u79 ( .A0(ss1_n86), .A1(ss1_n87), .B0(ss1_n73), .B1(
        ss1_n74), .Y(ss1_n60) );
  INV_X0P5B_A12TR ss1_u78 ( .A(ss1_n60), .Y(ss1_n76) );
  XOR2_X0P5M_A12TR ss1_u77 ( .A(ss1_n74), .B(ss1_n73), .Y(ss1_n77) );
  OAI21_X0P5M_A12TR ss1_u76 ( .A0(ss1_n84), .A1(ss1_n85), .B0(ss1_n64), .Y(
        ss1_n68) );
  OAI21_X0P5M_A12TR ss1_u75 ( .A0(ss1_n82), .A1(ss1_n83), .B0(ss1_n74), .Y(
        ss1_n69) );
  NOR2_X0P5A_A12TR ss1_u74 ( .A(ss1_n68), .B(ss1_n69), .Y(ss1_n70) );
  INV_X0P5B_A12TR ss1_u73 ( .A(ss_state_1[6]), .Y(ss1_n29) );
  OAI22_X0P5M_A12TR ss1_u72 ( .A0(ss1_n35), .A1(ss1_n32), .B0(ss1_n81), .B1(
        ss1_n29), .Y(ss1_n78) );
  INV_X0P5B_A12TR ss1_u71 ( .A(ss_state_1[9]), .Y(ss1_n41) );
  OAI22_X0P5M_A12TR ss1_u70 ( .A0(ss1_n80), .A1(ss1_n41), .B0(ss1_n47), .B1(
        ss1_n44), .Y(ss1_n79) );
  XNOR3_X0P5M_A12TR ss1_u69 ( .A(ss1_n64), .B(ss1_n78), .C(ss1_n79), .Y(
        ss1_n71) );
  CGENI_X1M_A12TR ss1_u68 ( .A(ss1_n77), .B(ss1_n70), .CI(ss1_n71), .CON(
        ss1_n59) );
  CGEN_X1M_A12TR ss1_u67 ( .A(ss1_n75), .B(ss1_n76), .CI(ss1_n59), .CO(ss1_n54) );
  XNOR2_X0P5M_A12TR ss1_u66 ( .A(ss1_n73), .B(ss1_n74), .Y(ss1_n72) );
  XOR3_X0P5M_A12TR ss1_u65 ( .A(ss1_n71), .B(ss1_n70), .C(ss1_n72), .Y(ss1_n65) );
  AO21_X0P5M_A12TR ss1_u64 ( .A0(ss1_n68), .A1(ss1_n69), .B0(ss1_n70), .Y(
        ss1_n67) );
  OA211_X0P5M_A12TR ss1_u63 ( .A0(max_selections[1]), .A1(ss1_n65), .B0(
        ss1_n67), .C0(max_selections[0]), .Y(ss1_n66) );
  AOI21_X0P5M_A12TR ss1_u62 ( .A0(ss1_n65), .A1(max_selections[1]), .B0(
        ss1_n66), .Y(ss1_n56) );
  CGENI_X1M_A12TR ss1_u61 ( .A(ss1_n62), .B(ss1_n63), .CI(ss1_n64), .CON(
        ss1_n61) );
  XNOR3_X0P5M_A12TR ss1_u60 ( .A(ss1_n59), .B(ss1_n60), .C(ss1_n61), .Y(
        ss1_n57) );
  AO1B2_X0P5M_A12TR ss1_u59 ( .B0(ss1_n57), .B1(ss1_n56), .A0N(
        max_selections[2]), .Y(ss1_n58) );
  OAI21_X0P5M_A12TR ss1_u58 ( .A0(ss1_n56), .A1(ss1_n57), .B0(ss1_n58), .Y(
        ss1_n55) );
  AND2_X0P5M_A12TR ss1_u57 ( .A(ss1_n54), .B(ss1_n55), .Y(ss1_n53) );
  INV_X0P5B_A12TR ss1_u56 ( .A(n4), .Y(ss1_n51) );
  OAI221_X0P5M_A12TR ss1_u55 ( .A0(max_selections[3]), .A1(ss1_n53), .B0(
        ss1_n54), .B1(ss1_n55), .C0(ss1_n51), .Y(ss1_n52) );
  INV_X0P5B_A12TR ss1_u54 ( .A(ss1_n52), .Y(ss1_n6) );
  NAND2_X0P5A_A12TR ss1_u53 ( .A(ss1_n6), .B(ss1_n47), .Y(ss1_n48) );
  AOI21_X0P5M_A12TR ss1_u52 ( .A0(button_num[0]), .A1(button_num[1]), .B0(n4), 
        .Y(ss1_n18) );
  INV_X0P5B_A12TR ss1_u51 ( .A(ss_selector[1]), .Y(ss1_n50) );
  AO21A1AI2_X0P5M_A12TR ss1_u50 ( .A0(button_num[3]), .A1(button_num[2]), .B0(
        ss1_n50), .C0(ss1_n51), .Y(ss1_n21) );
  OAI21_X0P5M_A12TR ss1_u49 ( .A0(n4), .A1(button_num[3]), .B0(ss1_n21), .Y(
        ss1_n38) );
  NOR2_X0P5A_A12TR ss1_u48 ( .A(ss1_n18), .B(ss1_n38), .Y(ss1_n49) );
  MXIT2_X0P5M_A12TR ss1_u47 ( .A(ss1_n47), .B(ss1_n48), .S0(ss1_n49), .Y(
        ss1_n103) );
  NAND2_X0P5A_A12TR ss1_u46 ( .A(ss1_n6), .B(ss1_n44), .Y(ss1_n45) );
  INV_X0P5B_A12TR ss1_u45 ( .A(button_num[0]), .Y(ss1_n39) );
  AOI21_X0P5M_A12TR ss1_u44 ( .A0(ss1_n39), .A1(button_num[1]), .B0(n4), .Y(
        ss1_n14) );
  NOR2_X0P5A_A12TR ss1_u43 ( .A(ss1_n14), .B(ss1_n38), .Y(ss1_n46) );
  MXIT2_X0P5M_A12TR ss1_u42 ( .A(ss1_n44), .B(ss1_n45), .S0(ss1_n46), .Y(
        ss1_n102) );
  NAND2_X0P5A_A12TR ss1_u41 ( .A(ss1_n6), .B(ss1_n41), .Y(ss1_n42) );
  INV_X0P5B_A12TR ss1_u40 ( .A(button_num[1]), .Y(ss1_n40) );
  AOI21_X0P5M_A12TR ss1_u39 ( .A0(ss1_n40), .A1(button_num[0]), .B0(n4), .Y(
        ss1_n10) );
  NOR2_X0P5A_A12TR ss1_u38 ( .A(ss1_n10), .B(ss1_n38), .Y(ss1_n43) );
  MXIT2_X0P5M_A12TR ss1_u37 ( .A(ss1_n41), .B(ss1_n42), .S0(ss1_n43), .Y(
        ss1_n101) );
  NAND2_X0P5A_A12TR ss1_u36 ( .A(ss1_n6), .B(ss1_n35), .Y(ss1_n36) );
  AOI21_X0P5M_A12TR ss1_u35 ( .A0(ss1_n39), .A1(ss1_n40), .B0(n4), .Y(ss1_n4)
         );
  NOR2_X0P5A_A12TR ss1_u34 ( .A(ss1_n4), .B(ss1_n38), .Y(ss1_n37) );
  MXIT2_X0P5M_A12TR ss1_u33 ( .A(ss1_n35), .B(ss1_n36), .S0(ss1_n37), .Y(
        ss1_n100) );
  NAND2_X0P5A_A12TR ss1_u32 ( .A(ss1_n6), .B(ss1_n32), .Y(ss1_n33) );
  OAI21_X0P5M_A12TR ss1_u31 ( .A0(n4), .A1(button_num[2]), .B0(ss1_n21), .Y(
        ss1_n25) );
  NOR2_X0P5A_A12TR ss1_u30 ( .A(ss1_n18), .B(ss1_n25), .Y(ss1_n34) );
  MXIT2_X0P5M_A12TR ss1_u29 ( .A(ss1_n32), .B(ss1_n33), .S0(ss1_n34), .Y(
        ss1_n99) );
  NAND2_X0P5A_A12TR ss1_u28 ( .A(ss1_n6), .B(ss1_n29), .Y(ss1_n30) );
  NOR2_X0P5A_A12TR ss1_u27 ( .A(ss1_n14), .B(ss1_n25), .Y(ss1_n31) );
  MXIT2_X0P5M_A12TR ss1_u26 ( .A(ss1_n29), .B(ss1_n30), .S0(ss1_n31), .Y(
        ss1_n98) );
  NAND2_X0P5A_A12TR ss1_u25 ( .A(ss1_n6), .B(ss1_n26), .Y(ss1_n27) );
  NOR2_X0P5A_A12TR ss1_u24 ( .A(ss1_n10), .B(ss1_n25), .Y(ss1_n28) );
  MXIT2_X0P5M_A12TR ss1_u23 ( .A(ss1_n26), .B(ss1_n27), .S0(ss1_n28), .Y(
        ss1_n97) );
  NAND2_X0P5A_A12TR ss1_u22 ( .A(ss1_n6), .B(ss1_n22), .Y(ss1_n23) );
  NOR2_X0P5A_A12TR ss1_u21 ( .A(ss1_n4), .B(ss1_n25), .Y(ss1_n24) );
  MXIT2_X0P5M_A12TR ss1_u20 ( .A(ss1_n22), .B(ss1_n23), .S0(ss1_n24), .Y(
        ss1_n96) );
  INV_X0P5B_A12TR ss1_u19 ( .A(ss_state_1[3]), .Y(ss1_n15) );
  NAND2_X0P5A_A12TR ss1_u18 ( .A(ss1_n6), .B(ss1_n15), .Y(ss1_n16) );
  INV_X0P5B_A12TR ss1_u17 ( .A(button_num[3]), .Y(ss1_n19) );
  INV_X0P5B_A12TR ss1_u16 ( .A(button_num[2]), .Y(ss1_n20) );
  AO21A1AI2_X0P5M_A12TR ss1_u15 ( .A0(ss1_n19), .A1(ss1_n20), .B0(n4), .C0(
        ss1_n21), .Y(ss1_n5) );
  NOR2_X0P5A_A12TR ss1_u14 ( .A(ss1_n18), .B(ss1_n5), .Y(ss1_n17) );
  MXIT2_X0P5M_A12TR ss1_u13 ( .A(ss1_n15), .B(ss1_n16), .S0(ss1_n17), .Y(
        ss1_n95) );
  NAND2_X0P5A_A12TR ss1_u12 ( .A(ss1_n6), .B(ss1_n11), .Y(ss1_n12) );
  NOR2_X0P5A_A12TR ss1_u11 ( .A(ss1_n14), .B(ss1_n5), .Y(ss1_n13) );
  MXIT2_X0P5M_A12TR ss1_u10 ( .A(ss1_n11), .B(ss1_n12), .S0(ss1_n13), .Y(
        ss1_n94) );
  NAND2_X0P5A_A12TR ss1_u9 ( .A(ss1_n6), .B(ss1_n7), .Y(ss1_n8) );
  NOR2_X0P5A_A12TR ss1_u8 ( .A(ss1_n10), .B(ss1_n5), .Y(ss1_n9) );
  MXIT2_X0P5M_A12TR ss1_u7 ( .A(ss1_n7), .B(ss1_n8), .S0(ss1_n9), .Y(ss1_n93)
         );
  INV_X0P5B_A12TR ss1_u6 ( .A(ss_state_1[0]), .Y(ss1_n1) );
  NAND2_X0P5A_A12TR ss1_u5 ( .A(ss1_n6), .B(ss1_n1), .Y(ss1_n2) );
  NOR2_X0P5A_A12TR ss1_u4 ( .A(ss1_n4), .B(ss1_n5), .Y(ss1_n3) );
  MXIT2_X0P5M_A12TR ss1_u3 ( .A(ss1_n1), .B(ss1_n2), .S0(ss1_n3), .Y(ss1_n92)
         );
  DFFQ_X1M_A12TR ss1_selection_state_reg_3_ ( .D(ss1_n95), .CK(osc_clk), .Q(
        ss_state_1[3]) );
  DFFQ_X1M_A12TR ss1_selection_state_reg_0_ ( .D(ss1_n92), .CK(osc_clk), .Q(
        ss_state_1[0]) );
  DFFQ_X1M_A12TR ss1_selection_state_reg_6_ ( .D(ss1_n98), .CK(osc_clk), .Q(
        ss_state_1[6]) );
  DFFQ_X1M_A12TR ss1_selection_state_reg_9_ ( .D(ss1_n101), .CK(osc_clk), .Q(
        ss_state_1[9]) );
  DFFQ_X1M_A12TR ss1_selection_state_reg_8_ ( .D(ss1_n100), .CK(osc_clk), .Q(
        ss_state_1[8]) );
  DFFQ_X1M_A12TR ss1_selection_state_reg_1_ ( .D(ss1_n93), .CK(osc_clk), .Q(
        ss_state_1[1]) );
  DFFQ_X1M_A12TR ss1_selection_state_reg_4_ ( .D(ss1_n96), .CK(osc_clk), .Q(
        ss_state_1[4]) );
  DFFQ_X1M_A12TR ss1_selection_state_reg_11_ ( .D(ss1_n103), .CK(osc_clk), .Q(
        ss_state_1[11]) );
  DFFQ_X1M_A12TR ss1_selection_state_reg_2_ ( .D(ss1_n94), .CK(osc_clk), .Q(
        ss_state_1[2]) );
  DFFQ_X1M_A12TR ss1_selection_state_reg_5_ ( .D(ss1_n97), .CK(osc_clk), .Q(
        ss_state_1[5]) );
  DFFQ_X1M_A12TR ss1_selection_state_reg_10_ ( .D(ss1_n102), .CK(osc_clk), .Q(
        ss_state_1[10]) );
  DFFQ_X1M_A12TR ss1_selection_state_reg_7_ ( .D(ss1_n99), .CK(osc_clk), .Q(
        ss_state_1[7]) );
  INV_X0P5B_A12TR ss2_u105 ( .A(ss_state_2[11]), .Y(ss2_n47) );
  INV_X0P5B_A12TR ss2_u104 ( .A(ss_state_2[7]), .Y(ss2_n32) );
  INV_X0P5B_A12TR ss2_u103 ( .A(ss_state_2[8]), .Y(ss2_n35) );
  XNOR2_X0P5M_A12TR ss2_u102 ( .A(ss2_n32), .B(ss2_n35), .Y(ss2_n81) );
  INV_X0P5B_A12TR ss2_u101 ( .A(ss2_n81), .Y(ss2_n91) );
  AOI22_X0P5M_A12TR ss2_u100 ( .A0(ss_state_2[6]), .A1(ss2_n91), .B0(
        ss_state_2[7]), .B1(ss_state_2[8]), .Y(ss2_n62) );
  XOR2_X0P5M_A12TR ss2_u99 ( .A(ss_state_2[6]), .B(ss2_n91), .Y(ss2_n84) );
  INV_X0P5B_A12TR ss2_u98 ( .A(ss_state_2[10]), .Y(ss2_n44) );
  XNOR2_X0P5M_A12TR ss2_u97 ( .A(ss2_n44), .B(ss2_n47), .Y(ss2_n80) );
  INV_X0P5B_A12TR ss2_u96 ( .A(ss2_n80), .Y(ss2_n90) );
  XOR2_X0P5M_A12TR ss2_u95 ( .A(ss_state_2[9]), .B(ss2_n90), .Y(ss2_n85) );
  NAND2_X0P5A_A12TR ss2_u94 ( .A(ss2_n84), .B(ss2_n85), .Y(ss2_n64) );
  AOI22_X0P5M_A12TR ss2_u93 ( .A0(ss_state_2[10]), .A1(ss_state_2[11]), .B0(
        ss_state_2[9]), .B1(ss2_n90), .Y(ss2_n63) );
  CGEN_X1M_A12TR ss2_u92 ( .A(ss2_n62), .B(ss2_n64), .CI(ss2_n63), .CO(ss2_n75) );
  INV_X0P5B_A12TR ss2_u91 ( .A(ss_state_2[4]), .Y(ss2_n22) );
  INV_X0P5B_A12TR ss2_u90 ( .A(ss_state_2[5]), .Y(ss2_n26) );
  XOR2_X0P5M_A12TR ss2_u89 ( .A(ss2_n22), .B(ss2_n26), .Y(ss2_n88) );
  AOI22_X0P5M_A12TR ss2_u88 ( .A0(ss_state_2[5]), .A1(ss_state_2[4]), .B0(
        ss2_n88), .B1(ss_state_2[3]), .Y(ss2_n86) );
  INV_X0P5B_A12TR ss2_u87 ( .A(ss_state_2[1]), .Y(ss2_n7) );
  INV_X0P5B_A12TR ss2_u86 ( .A(ss_state_2[2]), .Y(ss2_n11) );
  XOR2_X0P5M_A12TR ss2_u85 ( .A(ss2_n7), .B(ss2_n11), .Y(ss2_n89) );
  AOI22_X0P5M_A12TR ss2_u84 ( .A0(ss_state_2[2]), .A1(ss_state_2[1]), .B0(
        ss2_n89), .B1(ss_state_2[0]), .Y(ss2_n87) );
  XNOR2_X0P5M_A12TR ss2_u83 ( .A(ss2_n87), .B(ss2_n86), .Y(ss2_n73) );
  XOR2_X0P5M_A12TR ss2_u82 ( .A(ss_state_2[0]), .B(ss2_n89), .Y(ss2_n82) );
  XOR2_X0P5M_A12TR ss2_u81 ( .A(ss_state_2[3]), .B(ss2_n88), .Y(ss2_n83) );
  NAND2_X0P5A_A12TR ss2_u80 ( .A(ss2_n82), .B(ss2_n83), .Y(ss2_n74) );
  OAI22_X0P5M_A12TR ss2_u79 ( .A0(ss2_n86), .A1(ss2_n87), .B0(ss2_n73), .B1(
        ss2_n74), .Y(ss2_n60) );
  INV_X0P5B_A12TR ss2_u78 ( .A(ss2_n60), .Y(ss2_n76) );
  XOR2_X0P5M_A12TR ss2_u77 ( .A(ss2_n74), .B(ss2_n73), .Y(ss2_n77) );
  OAI21_X0P5M_A12TR ss2_u76 ( .A0(ss2_n84), .A1(ss2_n85), .B0(ss2_n64), .Y(
        ss2_n68) );
  OAI21_X0P5M_A12TR ss2_u75 ( .A0(ss2_n82), .A1(ss2_n83), .B0(ss2_n74), .Y(
        ss2_n69) );
  NOR2_X0P5A_A12TR ss2_u74 ( .A(ss2_n68), .B(ss2_n69), .Y(ss2_n70) );
  INV_X0P5B_A12TR ss2_u73 ( .A(ss_state_2[6]), .Y(ss2_n29) );
  OAI22_X0P5M_A12TR ss2_u72 ( .A0(ss2_n35), .A1(ss2_n32), .B0(ss2_n81), .B1(
        ss2_n29), .Y(ss2_n78) );
  INV_X0P5B_A12TR ss2_u71 ( .A(ss_state_2[9]), .Y(ss2_n41) );
  OAI22_X0P5M_A12TR ss2_u70 ( .A0(ss2_n80), .A1(ss2_n41), .B0(ss2_n47), .B1(
        ss2_n44), .Y(ss2_n79) );
  XNOR3_X0P5M_A12TR ss2_u69 ( .A(ss2_n64), .B(ss2_n78), .C(ss2_n79), .Y(
        ss2_n71) );
  CGENI_X1M_A12TR ss2_u68 ( .A(ss2_n77), .B(ss2_n70), .CI(ss2_n71), .CON(
        ss2_n59) );
  CGEN_X1M_A12TR ss2_u67 ( .A(ss2_n75), .B(ss2_n76), .CI(ss2_n59), .CO(ss2_n54) );
  XNOR2_X0P5M_A12TR ss2_u66 ( .A(ss2_n73), .B(ss2_n74), .Y(ss2_n72) );
  XOR3_X0P5M_A12TR ss2_u65 ( .A(ss2_n71), .B(ss2_n70), .C(ss2_n72), .Y(ss2_n65) );
  AO21_X0P5M_A12TR ss2_u64 ( .A0(ss2_n68), .A1(ss2_n69), .B0(ss2_n70), .Y(
        ss2_n67) );
  OA211_X0P5M_A12TR ss2_u63 ( .A0(max_selections[1]), .A1(ss2_n65), .B0(
        ss2_n67), .C0(max_selections[0]), .Y(ss2_n66) );
  AOI21_X0P5M_A12TR ss2_u62 ( .A0(ss2_n65), .A1(max_selections[1]), .B0(
        ss2_n66), .Y(ss2_n56) );
  CGENI_X1M_A12TR ss2_u61 ( .A(ss2_n62), .B(ss2_n63), .CI(ss2_n64), .CON(
        ss2_n61) );
  XNOR3_X0P5M_A12TR ss2_u60 ( .A(ss2_n59), .B(ss2_n60), .C(ss2_n61), .Y(
        ss2_n57) );
  AO1B2_X0P5M_A12TR ss2_u59 ( .B0(ss2_n57), .B1(ss2_n56), .A0N(
        max_selections[2]), .Y(ss2_n58) );
  OAI21_X0P5M_A12TR ss2_u58 ( .A0(ss2_n56), .A1(ss2_n57), .B0(ss2_n58), .Y(
        ss2_n55) );
  AND2_X0P5M_A12TR ss2_u57 ( .A(ss2_n54), .B(ss2_n55), .Y(ss2_n53) );
  INV_X0P5B_A12TR ss2_u56 ( .A(n4), .Y(ss2_n51) );
  OAI221_X0P5M_A12TR ss2_u55 ( .A0(max_selections[3]), .A1(ss2_n53), .B0(
        ss2_n54), .B1(ss2_n55), .C0(ss2_n51), .Y(ss2_n52) );
  INV_X0P5B_A12TR ss2_u54 ( .A(ss2_n52), .Y(ss2_n6) );
  NAND2_X0P5A_A12TR ss2_u53 ( .A(ss2_n6), .B(ss2_n47), .Y(ss2_n48) );
  AOI21_X0P5M_A12TR ss2_u52 ( .A0(button_num[0]), .A1(button_num[1]), .B0(n4), 
        .Y(ss2_n18) );
  INV_X0P5B_A12TR ss2_u51 ( .A(ss_selector[2]), .Y(ss2_n50) );
  AO21A1AI2_X0P5M_A12TR ss2_u50 ( .A0(button_num[3]), .A1(button_num[2]), .B0(
        ss2_n50), .C0(ss2_n51), .Y(ss2_n21) );
  OAI21_X0P5M_A12TR ss2_u49 ( .A0(n4), .A1(button_num[3]), .B0(ss2_n21), .Y(
        ss2_n38) );
  NOR2_X0P5A_A12TR ss2_u48 ( .A(ss2_n18), .B(ss2_n38), .Y(ss2_n49) );
  MXIT2_X0P5M_A12TR ss2_u47 ( .A(ss2_n47), .B(ss2_n48), .S0(ss2_n49), .Y(
        ss2_n103) );
  NAND2_X0P5A_A12TR ss2_u46 ( .A(ss2_n6), .B(ss2_n44), .Y(ss2_n45) );
  INV_X0P5B_A12TR ss2_u45 ( .A(button_num[0]), .Y(ss2_n39) );
  AOI21_X0P5M_A12TR ss2_u44 ( .A0(ss2_n39), .A1(button_num[1]), .B0(n4), .Y(
        ss2_n14) );
  NOR2_X0P5A_A12TR ss2_u43 ( .A(ss2_n14), .B(ss2_n38), .Y(ss2_n46) );
  MXIT2_X0P5M_A12TR ss2_u42 ( .A(ss2_n44), .B(ss2_n45), .S0(ss2_n46), .Y(
        ss2_n102) );
  NAND2_X0P5A_A12TR ss2_u41 ( .A(ss2_n6), .B(ss2_n41), .Y(ss2_n42) );
  INV_X0P5B_A12TR ss2_u40 ( .A(button_num[1]), .Y(ss2_n40) );
  AOI21_X0P5M_A12TR ss2_u39 ( .A0(ss2_n40), .A1(button_num[0]), .B0(n4), .Y(
        ss2_n10) );
  NOR2_X0P5A_A12TR ss2_u38 ( .A(ss2_n10), .B(ss2_n38), .Y(ss2_n43) );
  MXIT2_X0P5M_A12TR ss2_u37 ( .A(ss2_n41), .B(ss2_n42), .S0(ss2_n43), .Y(
        ss2_n101) );
  NAND2_X0P5A_A12TR ss2_u36 ( .A(ss2_n6), .B(ss2_n35), .Y(ss2_n36) );
  AOI21_X0P5M_A12TR ss2_u35 ( .A0(ss2_n39), .A1(ss2_n40), .B0(n4), .Y(ss2_n4)
         );
  NOR2_X0P5A_A12TR ss2_u34 ( .A(ss2_n4), .B(ss2_n38), .Y(ss2_n37) );
  MXIT2_X0P5M_A12TR ss2_u33 ( .A(ss2_n35), .B(ss2_n36), .S0(ss2_n37), .Y(
        ss2_n100) );
  NAND2_X0P5A_A12TR ss2_u32 ( .A(ss2_n6), .B(ss2_n32), .Y(ss2_n33) );
  OAI21_X0P5M_A12TR ss2_u31 ( .A0(n4), .A1(button_num[2]), .B0(ss2_n21), .Y(
        ss2_n25) );
  NOR2_X0P5A_A12TR ss2_u30 ( .A(ss2_n18), .B(ss2_n25), .Y(ss2_n34) );
  MXIT2_X0P5M_A12TR ss2_u29 ( .A(ss2_n32), .B(ss2_n33), .S0(ss2_n34), .Y(
        ss2_n99) );
  NAND2_X0P5A_A12TR ss2_u28 ( .A(ss2_n6), .B(ss2_n29), .Y(ss2_n30) );
  NOR2_X0P5A_A12TR ss2_u27 ( .A(ss2_n14), .B(ss2_n25), .Y(ss2_n31) );
  MXIT2_X0P5M_A12TR ss2_u26 ( .A(ss2_n29), .B(ss2_n30), .S0(ss2_n31), .Y(
        ss2_n98) );
  NAND2_X0P5A_A12TR ss2_u25 ( .A(ss2_n6), .B(ss2_n26), .Y(ss2_n27) );
  NOR2_X0P5A_A12TR ss2_u24 ( .A(ss2_n10), .B(ss2_n25), .Y(ss2_n28) );
  MXIT2_X0P5M_A12TR ss2_u23 ( .A(ss2_n26), .B(ss2_n27), .S0(ss2_n28), .Y(
        ss2_n97) );
  NAND2_X0P5A_A12TR ss2_u22 ( .A(ss2_n6), .B(ss2_n22), .Y(ss2_n23) );
  NOR2_X0P5A_A12TR ss2_u21 ( .A(ss2_n4), .B(ss2_n25), .Y(ss2_n24) );
  MXIT2_X0P5M_A12TR ss2_u20 ( .A(ss2_n22), .B(ss2_n23), .S0(ss2_n24), .Y(
        ss2_n96) );
  INV_X0P5B_A12TR ss2_u19 ( .A(ss_state_2[3]), .Y(ss2_n15) );
  NAND2_X0P5A_A12TR ss2_u18 ( .A(ss2_n6), .B(ss2_n15), .Y(ss2_n16) );
  INV_X0P5B_A12TR ss2_u17 ( .A(button_num[3]), .Y(ss2_n19) );
  INV_X0P5B_A12TR ss2_u16 ( .A(button_num[2]), .Y(ss2_n20) );
  AO21A1AI2_X0P5M_A12TR ss2_u15 ( .A0(ss2_n19), .A1(ss2_n20), .B0(n4), .C0(
        ss2_n21), .Y(ss2_n5) );
  NOR2_X0P5A_A12TR ss2_u14 ( .A(ss2_n18), .B(ss2_n5), .Y(ss2_n17) );
  MXIT2_X0P5M_A12TR ss2_u13 ( .A(ss2_n15), .B(ss2_n16), .S0(ss2_n17), .Y(
        ss2_n95) );
  NAND2_X0P5A_A12TR ss2_u12 ( .A(ss2_n6), .B(ss2_n11), .Y(ss2_n12) );
  NOR2_X0P5A_A12TR ss2_u11 ( .A(ss2_n14), .B(ss2_n5), .Y(ss2_n13) );
  MXIT2_X0P5M_A12TR ss2_u10 ( .A(ss2_n11), .B(ss2_n12), .S0(ss2_n13), .Y(
        ss2_n94) );
  NAND2_X0P5A_A12TR ss2_u9 ( .A(ss2_n6), .B(ss2_n7), .Y(ss2_n8) );
  NOR2_X0P5A_A12TR ss2_u8 ( .A(ss2_n10), .B(ss2_n5), .Y(ss2_n9) );
  MXIT2_X0P5M_A12TR ss2_u7 ( .A(ss2_n7), .B(ss2_n8), .S0(ss2_n9), .Y(ss2_n93)
         );
  INV_X0P5B_A12TR ss2_u6 ( .A(ss_state_2[0]), .Y(ss2_n1) );
  NAND2_X0P5A_A12TR ss2_u5 ( .A(ss2_n6), .B(ss2_n1), .Y(ss2_n2) );
  NOR2_X0P5A_A12TR ss2_u4 ( .A(ss2_n4), .B(ss2_n5), .Y(ss2_n3) );
  MXIT2_X0P5M_A12TR ss2_u3 ( .A(ss2_n1), .B(ss2_n2), .S0(ss2_n3), .Y(ss2_n92)
         );
  DFFQ_X1M_A12TR ss2_selection_state_reg_3_ ( .D(ss2_n95), .CK(osc_clk), .Q(
        ss_state_2[3]) );
  DFFQ_X1M_A12TR ss2_selection_state_reg_0_ ( .D(ss2_n92), .CK(osc_clk), .Q(
        ss_state_2[0]) );
  DFFQ_X1M_A12TR ss2_selection_state_reg_6_ ( .D(ss2_n98), .CK(osc_clk), .Q(
        ss_state_2[6]) );
  DFFQ_X1M_A12TR ss2_selection_state_reg_9_ ( .D(ss2_n101), .CK(osc_clk), .Q(
        ss_state_2[9]) );
  DFFQ_X1M_A12TR ss2_selection_state_reg_8_ ( .D(ss2_n100), .CK(osc_clk), .Q(
        ss_state_2[8]) );
  DFFQ_X1M_A12TR ss2_selection_state_reg_1_ ( .D(ss2_n93), .CK(osc_clk), .Q(
        ss_state_2[1]) );
  DFFQ_X1M_A12TR ss2_selection_state_reg_4_ ( .D(ss2_n96), .CK(osc_clk), .Q(
        ss_state_2[4]) );
  DFFQ_X1M_A12TR ss2_selection_state_reg_11_ ( .D(ss2_n103), .CK(osc_clk), .Q(
        ss_state_2[11]) );
  DFFQ_X1M_A12TR ss2_selection_state_reg_2_ ( .D(ss2_n94), .CK(osc_clk), .Q(
        ss_state_2[2]) );
  DFFQ_X1M_A12TR ss2_selection_state_reg_5_ ( .D(ss2_n97), .CK(osc_clk), .Q(
        ss_state_2[5]) );
  DFFQ_X1M_A12TR ss2_selection_state_reg_10_ ( .D(ss2_n102), .CK(osc_clk), .Q(
        ss_state_2[10]) );
  DFFQ_X1M_A12TR ss2_selection_state_reg_7_ ( .D(ss2_n99), .CK(osc_clk), .Q(
        ss_state_2[7]) );
  INV_X0P5B_A12TR ss3_u105 ( .A(ss_state_3[11]), .Y(ss3_n47) );
  INV_X0P5B_A12TR ss3_u104 ( .A(ss_state_3[7]), .Y(ss3_n32) );
  INV_X0P5B_A12TR ss3_u103 ( .A(ss_state_3[8]), .Y(ss3_n35) );
  XNOR2_X0P5M_A12TR ss3_u102 ( .A(ss3_n32), .B(ss3_n35), .Y(ss3_n81) );
  INV_X0P5B_A12TR ss3_u101 ( .A(ss3_n81), .Y(ss3_n91) );
  AOI22_X0P5M_A12TR ss3_u100 ( .A0(ss_state_3[6]), .A1(ss3_n91), .B0(
        ss_state_3[7]), .B1(ss_state_3[8]), .Y(ss3_n62) );
  XOR2_X0P5M_A12TR ss3_u99 ( .A(ss_state_3[6]), .B(ss3_n91), .Y(ss3_n84) );
  INV_X0P5B_A12TR ss3_u98 ( .A(ss_state_3[10]), .Y(ss3_n44) );
  XNOR2_X0P5M_A12TR ss3_u97 ( .A(ss3_n44), .B(ss3_n47), .Y(ss3_n80) );
  INV_X0P5B_A12TR ss3_u96 ( .A(ss3_n80), .Y(ss3_n90) );
  XOR2_X0P5M_A12TR ss3_u95 ( .A(ss_state_3[9]), .B(ss3_n90), .Y(ss3_n85) );
  NAND2_X0P5A_A12TR ss3_u94 ( .A(ss3_n84), .B(ss3_n85), .Y(ss3_n64) );
  AOI22_X0P5M_A12TR ss3_u93 ( .A0(ss_state_3[10]), .A1(ss_state_3[11]), .B0(
        ss_state_3[9]), .B1(ss3_n90), .Y(ss3_n63) );
  CGEN_X1M_A12TR ss3_u92 ( .A(ss3_n62), .B(ss3_n64), .CI(ss3_n63), .CO(ss3_n75) );
  INV_X0P5B_A12TR ss3_u91 ( .A(ss_state_3[4]), .Y(ss3_n22) );
  INV_X0P5B_A12TR ss3_u90 ( .A(ss_state_3[5]), .Y(ss3_n26) );
  XOR2_X0P5M_A12TR ss3_u89 ( .A(ss3_n22), .B(ss3_n26), .Y(ss3_n88) );
  AOI22_X0P5M_A12TR ss3_u88 ( .A0(ss_state_3[5]), .A1(ss_state_3[4]), .B0(
        ss3_n88), .B1(ss_state_3[3]), .Y(ss3_n86) );
  INV_X0P5B_A12TR ss3_u87 ( .A(ss_state_3[1]), .Y(ss3_n7) );
  INV_X0P5B_A12TR ss3_u86 ( .A(ss_state_3[2]), .Y(ss3_n11) );
  XOR2_X0P5M_A12TR ss3_u85 ( .A(ss3_n7), .B(ss3_n11), .Y(ss3_n89) );
  AOI22_X0P5M_A12TR ss3_u84 ( .A0(ss_state_3[2]), .A1(ss_state_3[1]), .B0(
        ss3_n89), .B1(ss_state_3[0]), .Y(ss3_n87) );
  XNOR2_X0P5M_A12TR ss3_u83 ( .A(ss3_n87), .B(ss3_n86), .Y(ss3_n73) );
  XOR2_X0P5M_A12TR ss3_u82 ( .A(ss_state_3[0]), .B(ss3_n89), .Y(ss3_n82) );
  XOR2_X0P5M_A12TR ss3_u81 ( .A(ss_state_3[3]), .B(ss3_n88), .Y(ss3_n83) );
  NAND2_X0P5A_A12TR ss3_u80 ( .A(ss3_n82), .B(ss3_n83), .Y(ss3_n74) );
  OAI22_X0P5M_A12TR ss3_u79 ( .A0(ss3_n86), .A1(ss3_n87), .B0(ss3_n73), .B1(
        ss3_n74), .Y(ss3_n60) );
  INV_X0P5B_A12TR ss3_u78 ( .A(ss3_n60), .Y(ss3_n76) );
  XOR2_X0P5M_A12TR ss3_u77 ( .A(ss3_n74), .B(ss3_n73), .Y(ss3_n77) );
  OAI21_X0P5M_A12TR ss3_u76 ( .A0(ss3_n84), .A1(ss3_n85), .B0(ss3_n64), .Y(
        ss3_n68) );
  OAI21_X0P5M_A12TR ss3_u75 ( .A0(ss3_n82), .A1(ss3_n83), .B0(ss3_n74), .Y(
        ss3_n69) );
  NOR2_X0P5A_A12TR ss3_u74 ( .A(ss3_n68), .B(ss3_n69), .Y(ss3_n70) );
  INV_X0P5B_A12TR ss3_u73 ( .A(ss_state_3[6]), .Y(ss3_n29) );
  OAI22_X0P5M_A12TR ss3_u72 ( .A0(ss3_n35), .A1(ss3_n32), .B0(ss3_n81), .B1(
        ss3_n29), .Y(ss3_n78) );
  INV_X0P5B_A12TR ss3_u71 ( .A(ss_state_3[9]), .Y(ss3_n41) );
  OAI22_X0P5M_A12TR ss3_u70 ( .A0(ss3_n80), .A1(ss3_n41), .B0(ss3_n47), .B1(
        ss3_n44), .Y(ss3_n79) );
  XNOR3_X0P5M_A12TR ss3_u69 ( .A(ss3_n64), .B(ss3_n78), .C(ss3_n79), .Y(
        ss3_n71) );
  CGENI_X1M_A12TR ss3_u68 ( .A(ss3_n77), .B(ss3_n70), .CI(ss3_n71), .CON(
        ss3_n59) );
  CGEN_X1M_A12TR ss3_u67 ( .A(ss3_n75), .B(ss3_n76), .CI(ss3_n59), .CO(ss3_n54) );
  XNOR2_X0P5M_A12TR ss3_u66 ( .A(ss3_n73), .B(ss3_n74), .Y(ss3_n72) );
  XOR3_X0P5M_A12TR ss3_u65 ( .A(ss3_n71), .B(ss3_n70), .C(ss3_n72), .Y(ss3_n65) );
  AO21_X0P5M_A12TR ss3_u64 ( .A0(ss3_n68), .A1(ss3_n69), .B0(ss3_n70), .Y(
        ss3_n67) );
  OA211_X0P5M_A12TR ss3_u63 ( .A0(max_selections[1]), .A1(ss3_n65), .B0(
        ss3_n67), .C0(max_selections[0]), .Y(ss3_n66) );
  AOI21_X0P5M_A12TR ss3_u62 ( .A0(ss3_n65), .A1(max_selections[1]), .B0(
        ss3_n66), .Y(ss3_n56) );
  CGENI_X1M_A12TR ss3_u61 ( .A(ss3_n62), .B(ss3_n63), .CI(ss3_n64), .CON(
        ss3_n61) );
  XNOR3_X0P5M_A12TR ss3_u60 ( .A(ss3_n59), .B(ss3_n60), .C(ss3_n61), .Y(
        ss3_n57) );
  AO1B2_X0P5M_A12TR ss3_u59 ( .B0(ss3_n57), .B1(ss3_n56), .A0N(
        max_selections[2]), .Y(ss3_n58) );
  OAI21_X0P5M_A12TR ss3_u58 ( .A0(ss3_n56), .A1(ss3_n57), .B0(ss3_n58), .Y(
        ss3_n55) );
  AND2_X0P5M_A12TR ss3_u57 ( .A(ss3_n54), .B(ss3_n55), .Y(ss3_n53) );
  INV_X0P5B_A12TR ss3_u56 ( .A(n4), .Y(ss3_n51) );
  OAI221_X0P5M_A12TR ss3_u55 ( .A0(max_selections[3]), .A1(ss3_n53), .B0(
        ss3_n54), .B1(ss3_n55), .C0(ss3_n51), .Y(ss3_n52) );
  INV_X0P5B_A12TR ss3_u54 ( .A(ss3_n52), .Y(ss3_n6) );
  NAND2_X0P5A_A12TR ss3_u53 ( .A(ss3_n6), .B(ss3_n47), .Y(ss3_n48) );
  AOI21_X0P5M_A12TR ss3_u52 ( .A0(button_num[0]), .A1(button_num[1]), .B0(n4), 
        .Y(ss3_n18) );
  INV_X0P5B_A12TR ss3_u51 ( .A(ss_selector[3]), .Y(ss3_n50) );
  AO21A1AI2_X0P5M_A12TR ss3_u50 ( .A0(button_num[3]), .A1(button_num[2]), .B0(
        ss3_n50), .C0(ss3_n51), .Y(ss3_n21) );
  OAI21_X0P5M_A12TR ss3_u49 ( .A0(n4), .A1(button_num[3]), .B0(ss3_n21), .Y(
        ss3_n38) );
  NOR2_X0P5A_A12TR ss3_u48 ( .A(ss3_n18), .B(ss3_n38), .Y(ss3_n49) );
  MXIT2_X0P5M_A12TR ss3_u47 ( .A(ss3_n47), .B(ss3_n48), .S0(ss3_n49), .Y(
        ss3_n103) );
  NAND2_X0P5A_A12TR ss3_u46 ( .A(ss3_n6), .B(ss3_n44), .Y(ss3_n45) );
  INV_X0P5B_A12TR ss3_u45 ( .A(button_num[0]), .Y(ss3_n39) );
  AOI21_X0P5M_A12TR ss3_u44 ( .A0(ss3_n39), .A1(button_num[1]), .B0(n4), .Y(
        ss3_n14) );
  NOR2_X0P5A_A12TR ss3_u43 ( .A(ss3_n14), .B(ss3_n38), .Y(ss3_n46) );
  MXIT2_X0P5M_A12TR ss3_u42 ( .A(ss3_n44), .B(ss3_n45), .S0(ss3_n46), .Y(
        ss3_n102) );
  NAND2_X0P5A_A12TR ss3_u41 ( .A(ss3_n6), .B(ss3_n41), .Y(ss3_n42) );
  INV_X0P5B_A12TR ss3_u40 ( .A(button_num[1]), .Y(ss3_n40) );
  AOI21_X0P5M_A12TR ss3_u39 ( .A0(ss3_n40), .A1(button_num[0]), .B0(n4), .Y(
        ss3_n10) );
  NOR2_X0P5A_A12TR ss3_u38 ( .A(ss3_n10), .B(ss3_n38), .Y(ss3_n43) );
  MXIT2_X0P5M_A12TR ss3_u37 ( .A(ss3_n41), .B(ss3_n42), .S0(ss3_n43), .Y(
        ss3_n101) );
  NAND2_X0P5A_A12TR ss3_u36 ( .A(ss3_n6), .B(ss3_n35), .Y(ss3_n36) );
  AOI21_X0P5M_A12TR ss3_u35 ( .A0(ss3_n39), .A1(ss3_n40), .B0(n4), .Y(ss3_n4)
         );
  NOR2_X0P5A_A12TR ss3_u34 ( .A(ss3_n4), .B(ss3_n38), .Y(ss3_n37) );
  MXIT2_X0P5M_A12TR ss3_u33 ( .A(ss3_n35), .B(ss3_n36), .S0(ss3_n37), .Y(
        ss3_n100) );
  NAND2_X0P5A_A12TR ss3_u32 ( .A(ss3_n6), .B(ss3_n32), .Y(ss3_n33) );
  OAI21_X0P5M_A12TR ss3_u31 ( .A0(n4), .A1(button_num[2]), .B0(ss3_n21), .Y(
        ss3_n25) );
  NOR2_X0P5A_A12TR ss3_u30 ( .A(ss3_n18), .B(ss3_n25), .Y(ss3_n34) );
  MXIT2_X0P5M_A12TR ss3_u29 ( .A(ss3_n32), .B(ss3_n33), .S0(ss3_n34), .Y(
        ss3_n99) );
  NAND2_X0P5A_A12TR ss3_u28 ( .A(ss3_n6), .B(ss3_n29), .Y(ss3_n30) );
  NOR2_X0P5A_A12TR ss3_u27 ( .A(ss3_n14), .B(ss3_n25), .Y(ss3_n31) );
  MXIT2_X0P5M_A12TR ss3_u26 ( .A(ss3_n29), .B(ss3_n30), .S0(ss3_n31), .Y(
        ss3_n98) );
  NAND2_X0P5A_A12TR ss3_u25 ( .A(ss3_n6), .B(ss3_n26), .Y(ss3_n27) );
  NOR2_X0P5A_A12TR ss3_u24 ( .A(ss3_n10), .B(ss3_n25), .Y(ss3_n28) );
  MXIT2_X0P5M_A12TR ss3_u23 ( .A(ss3_n26), .B(ss3_n27), .S0(ss3_n28), .Y(
        ss3_n97) );
  NAND2_X0P5A_A12TR ss3_u22 ( .A(ss3_n6), .B(ss3_n22), .Y(ss3_n23) );
  NOR2_X0P5A_A12TR ss3_u21 ( .A(ss3_n4), .B(ss3_n25), .Y(ss3_n24) );
  MXIT2_X0P5M_A12TR ss3_u20 ( .A(ss3_n22), .B(ss3_n23), .S0(ss3_n24), .Y(
        ss3_n96) );
  INV_X0P5B_A12TR ss3_u19 ( .A(ss_state_3[3]), .Y(ss3_n15) );
  NAND2_X0P5A_A12TR ss3_u18 ( .A(ss3_n6), .B(ss3_n15), .Y(ss3_n16) );
  INV_X0P5B_A12TR ss3_u17 ( .A(button_num[3]), .Y(ss3_n19) );
  INV_X0P5B_A12TR ss3_u16 ( .A(button_num[2]), .Y(ss3_n20) );
  AO21A1AI2_X0P5M_A12TR ss3_u15 ( .A0(ss3_n19), .A1(ss3_n20), .B0(n4), .C0(
        ss3_n21), .Y(ss3_n5) );
  NOR2_X0P5A_A12TR ss3_u14 ( .A(ss3_n18), .B(ss3_n5), .Y(ss3_n17) );
  MXIT2_X0P5M_A12TR ss3_u13 ( .A(ss3_n15), .B(ss3_n16), .S0(ss3_n17), .Y(
        ss3_n95) );
  NAND2_X0P5A_A12TR ss3_u12 ( .A(ss3_n6), .B(ss3_n11), .Y(ss3_n12) );
  NOR2_X0P5A_A12TR ss3_u11 ( .A(ss3_n14), .B(ss3_n5), .Y(ss3_n13) );
  MXIT2_X0P5M_A12TR ss3_u10 ( .A(ss3_n11), .B(ss3_n12), .S0(ss3_n13), .Y(
        ss3_n94) );
  NAND2_X0P5A_A12TR ss3_u9 ( .A(ss3_n6), .B(ss3_n7), .Y(ss3_n8) );
  NOR2_X0P5A_A12TR ss3_u8 ( .A(ss3_n10), .B(ss3_n5), .Y(ss3_n9) );
  MXIT2_X0P5M_A12TR ss3_u7 ( .A(ss3_n7), .B(ss3_n8), .S0(ss3_n9), .Y(ss3_n93)
         );
  INV_X0P5B_A12TR ss3_u6 ( .A(ss_state_3[0]), .Y(ss3_n1) );
  NAND2_X0P5A_A12TR ss3_u5 ( .A(ss3_n6), .B(ss3_n1), .Y(ss3_n2) );
  NOR2_X0P5A_A12TR ss3_u4 ( .A(ss3_n4), .B(ss3_n5), .Y(ss3_n3) );
  MXIT2_X0P5M_A12TR ss3_u3 ( .A(ss3_n1), .B(ss3_n2), .S0(ss3_n3), .Y(ss3_n92)
         );
  DFFQ_X1M_A12TR ss3_selection_state_reg_3_ ( .D(ss3_n95), .CK(osc_clk), .Q(
        ss_state_3[3]) );
  DFFQ_X1M_A12TR ss3_selection_state_reg_0_ ( .D(ss3_n92), .CK(osc_clk), .Q(
        ss_state_3[0]) );
  DFFQ_X1M_A12TR ss3_selection_state_reg_6_ ( .D(ss3_n98), .CK(osc_clk), .Q(
        ss_state_3[6]) );
  DFFQ_X1M_A12TR ss3_selection_state_reg_9_ ( .D(ss3_n101), .CK(osc_clk), .Q(
        ss_state_3[9]) );
  DFFQ_X1M_A12TR ss3_selection_state_reg_8_ ( .D(ss3_n100), .CK(osc_clk), .Q(
        ss_state_3[8]) );
  DFFQ_X1M_A12TR ss3_selection_state_reg_1_ ( .D(ss3_n93), .CK(osc_clk), .Q(
        ss_state_3[1]) );
  DFFQ_X1M_A12TR ss3_selection_state_reg_4_ ( .D(ss3_n96), .CK(osc_clk), .Q(
        ss_state_3[4]) );
  DFFQ_X1M_A12TR ss3_selection_state_reg_11_ ( .D(ss3_n103), .CK(osc_clk), .Q(
        ss_state_3[11]) );
  DFFQ_X1M_A12TR ss3_selection_state_reg_2_ ( .D(ss3_n94), .CK(osc_clk), .Q(
        ss_state_3[2]) );
  DFFQ_X1M_A12TR ss3_selection_state_reg_5_ ( .D(ss3_n97), .CK(osc_clk), .Q(
        ss_state_3[5]) );
  DFFQ_X1M_A12TR ss3_selection_state_reg_10_ ( .D(ss3_n102), .CK(osc_clk), .Q(
        ss_state_3[10]) );
  DFFQ_X1M_A12TR ss3_selection_state_reg_7_ ( .D(ss3_n99), .CK(osc_clk), .Q(
        ss_state_3[7]) );
  INV_X0P5B_A12TR ss4_u105 ( .A(ss_state_4[11]), .Y(ss4_n47) );
  INV_X0P5B_A12TR ss4_u104 ( .A(ss_state_4[7]), .Y(ss4_n32) );
  INV_X0P5B_A12TR ss4_u103 ( .A(ss_state_4[8]), .Y(ss4_n35) );
  XNOR2_X0P5M_A12TR ss4_u102 ( .A(ss4_n32), .B(ss4_n35), .Y(ss4_n81) );
  INV_X0P5B_A12TR ss4_u101 ( .A(ss4_n81), .Y(ss4_n91) );
  AOI22_X0P5M_A12TR ss4_u100 ( .A0(ss_state_4[6]), .A1(ss4_n91), .B0(
        ss_state_4[7]), .B1(ss_state_4[8]), .Y(ss4_n62) );
  XOR2_X0P5M_A12TR ss4_u99 ( .A(ss_state_4[6]), .B(ss4_n91), .Y(ss4_n84) );
  INV_X0P5B_A12TR ss4_u98 ( .A(ss_state_4[10]), .Y(ss4_n44) );
  XNOR2_X0P5M_A12TR ss4_u97 ( .A(ss4_n44), .B(ss4_n47), .Y(ss4_n80) );
  INV_X0P5B_A12TR ss4_u96 ( .A(ss4_n80), .Y(ss4_n90) );
  XOR2_X0P5M_A12TR ss4_u95 ( .A(ss_state_4[9]), .B(ss4_n90), .Y(ss4_n85) );
  NAND2_X0P5A_A12TR ss4_u94 ( .A(ss4_n84), .B(ss4_n85), .Y(ss4_n64) );
  AOI22_X0P5M_A12TR ss4_u93 ( .A0(ss_state_4[10]), .A1(ss_state_4[11]), .B0(
        ss_state_4[9]), .B1(ss4_n90), .Y(ss4_n63) );
  CGEN_X1M_A12TR ss4_u92 ( .A(ss4_n62), .B(ss4_n64), .CI(ss4_n63), .CO(ss4_n75) );
  INV_X0P5B_A12TR ss4_u91 ( .A(ss_state_4[4]), .Y(ss4_n22) );
  INV_X0P5B_A12TR ss4_u90 ( .A(ss_state_4[5]), .Y(ss4_n26) );
  XOR2_X0P5M_A12TR ss4_u89 ( .A(ss4_n22), .B(ss4_n26), .Y(ss4_n88) );
  AOI22_X0P5M_A12TR ss4_u88 ( .A0(ss_state_4[5]), .A1(ss_state_4[4]), .B0(
        ss4_n88), .B1(ss_state_4[3]), .Y(ss4_n86) );
  INV_X0P5B_A12TR ss4_u87 ( .A(ss_state_4[1]), .Y(ss4_n7) );
  INV_X0P5B_A12TR ss4_u86 ( .A(ss_state_4[2]), .Y(ss4_n11) );
  XOR2_X0P5M_A12TR ss4_u85 ( .A(ss4_n7), .B(ss4_n11), .Y(ss4_n89) );
  AOI22_X0P5M_A12TR ss4_u84 ( .A0(ss_state_4[2]), .A1(ss_state_4[1]), .B0(
        ss4_n89), .B1(ss_state_4[0]), .Y(ss4_n87) );
  XNOR2_X0P5M_A12TR ss4_u83 ( .A(ss4_n87), .B(ss4_n86), .Y(ss4_n73) );
  XOR2_X0P5M_A12TR ss4_u82 ( .A(ss_state_4[0]), .B(ss4_n89), .Y(ss4_n82) );
  XOR2_X0P5M_A12TR ss4_u81 ( .A(ss_state_4[3]), .B(ss4_n88), .Y(ss4_n83) );
  NAND2_X0P5A_A12TR ss4_u80 ( .A(ss4_n82), .B(ss4_n83), .Y(ss4_n74) );
  OAI22_X0P5M_A12TR ss4_u79 ( .A0(ss4_n86), .A1(ss4_n87), .B0(ss4_n73), .B1(
        ss4_n74), .Y(ss4_n60) );
  INV_X0P5B_A12TR ss4_u78 ( .A(ss4_n60), .Y(ss4_n76) );
  XOR2_X0P5M_A12TR ss4_u77 ( .A(ss4_n74), .B(ss4_n73), .Y(ss4_n77) );
  OAI21_X0P5M_A12TR ss4_u76 ( .A0(ss4_n84), .A1(ss4_n85), .B0(ss4_n64), .Y(
        ss4_n68) );
  OAI21_X0P5M_A12TR ss4_u75 ( .A0(ss4_n82), .A1(ss4_n83), .B0(ss4_n74), .Y(
        ss4_n69) );
  NOR2_X0P5A_A12TR ss4_u74 ( .A(ss4_n68), .B(ss4_n69), .Y(ss4_n70) );
  INV_X0P5B_A12TR ss4_u73 ( .A(ss_state_4[6]), .Y(ss4_n29) );
  OAI22_X0P5M_A12TR ss4_u72 ( .A0(ss4_n35), .A1(ss4_n32), .B0(ss4_n81), .B1(
        ss4_n29), .Y(ss4_n78) );
  INV_X0P5B_A12TR ss4_u71 ( .A(ss_state_4[9]), .Y(ss4_n41) );
  OAI22_X0P5M_A12TR ss4_u70 ( .A0(ss4_n80), .A1(ss4_n41), .B0(ss4_n47), .B1(
        ss4_n44), .Y(ss4_n79) );
  XNOR3_X0P5M_A12TR ss4_u69 ( .A(ss4_n64), .B(ss4_n78), .C(ss4_n79), .Y(
        ss4_n71) );
  CGENI_X1M_A12TR ss4_u68 ( .A(ss4_n77), .B(ss4_n70), .CI(ss4_n71), .CON(
        ss4_n59) );
  CGEN_X1M_A12TR ss4_u67 ( .A(ss4_n75), .B(ss4_n76), .CI(ss4_n59), .CO(ss4_n54) );
  XNOR2_X0P5M_A12TR ss4_u66 ( .A(ss4_n73), .B(ss4_n74), .Y(ss4_n72) );
  XOR3_X0P5M_A12TR ss4_u65 ( .A(ss4_n71), .B(ss4_n70), .C(ss4_n72), .Y(ss4_n65) );
  AO21_X0P5M_A12TR ss4_u64 ( .A0(ss4_n68), .A1(ss4_n69), .B0(ss4_n70), .Y(
        ss4_n67) );
  OA211_X0P5M_A12TR ss4_u63 ( .A0(max_selections[1]), .A1(ss4_n65), .B0(
        ss4_n67), .C0(max_selections[0]), .Y(ss4_n66) );
  AOI21_X0P5M_A12TR ss4_u62 ( .A0(ss4_n65), .A1(max_selections[1]), .B0(
        ss4_n66), .Y(ss4_n56) );
  CGENI_X1M_A12TR ss4_u61 ( .A(ss4_n62), .B(ss4_n63), .CI(ss4_n64), .CON(
        ss4_n61) );
  XNOR3_X0P5M_A12TR ss4_u60 ( .A(ss4_n59), .B(ss4_n60), .C(ss4_n61), .Y(
        ss4_n57) );
  AO1B2_X0P5M_A12TR ss4_u59 ( .B0(ss4_n57), .B1(ss4_n56), .A0N(
        max_selections[2]), .Y(ss4_n58) );
  OAI21_X0P5M_A12TR ss4_u58 ( .A0(ss4_n56), .A1(ss4_n57), .B0(ss4_n58), .Y(
        ss4_n55) );
  AND2_X0P5M_A12TR ss4_u57 ( .A(ss4_n54), .B(ss4_n55), .Y(ss4_n53) );
  INV_X0P5B_A12TR ss4_u56 ( .A(n4), .Y(ss4_n51) );
  OAI221_X0P5M_A12TR ss4_u55 ( .A0(max_selections[3]), .A1(ss4_n53), .B0(
        ss4_n54), .B1(ss4_n55), .C0(ss4_n51), .Y(ss4_n52) );
  INV_X0P5B_A12TR ss4_u54 ( .A(ss4_n52), .Y(ss4_n6) );
  NAND2_X0P5A_A12TR ss4_u53 ( .A(ss4_n6), .B(ss4_n47), .Y(ss4_n48) );
  AOI21_X0P5M_A12TR ss4_u52 ( .A0(button_num[0]), .A1(button_num[1]), .B0(n4), 
        .Y(ss4_n18) );
  INV_X0P5B_A12TR ss4_u51 ( .A(ss_selector[4]), .Y(ss4_n50) );
  AO21A1AI2_X0P5M_A12TR ss4_u50 ( .A0(button_num[3]), .A1(button_num[2]), .B0(
        ss4_n50), .C0(ss4_n51), .Y(ss4_n21) );
  OAI21_X0P5M_A12TR ss4_u49 ( .A0(n4), .A1(button_num[3]), .B0(ss4_n21), .Y(
        ss4_n38) );
  NOR2_X0P5A_A12TR ss4_u48 ( .A(ss4_n18), .B(ss4_n38), .Y(ss4_n49) );
  MXIT2_X0P5M_A12TR ss4_u47 ( .A(ss4_n47), .B(ss4_n48), .S0(ss4_n49), .Y(
        ss4_n103) );
  NAND2_X0P5A_A12TR ss4_u46 ( .A(ss4_n6), .B(ss4_n44), .Y(ss4_n45) );
  INV_X0P5B_A12TR ss4_u45 ( .A(button_num[0]), .Y(ss4_n39) );
  AOI21_X0P5M_A12TR ss4_u44 ( .A0(ss4_n39), .A1(button_num[1]), .B0(n4), .Y(
        ss4_n14) );
  NOR2_X0P5A_A12TR ss4_u43 ( .A(ss4_n14), .B(ss4_n38), .Y(ss4_n46) );
  MXIT2_X0P5M_A12TR ss4_u42 ( .A(ss4_n44), .B(ss4_n45), .S0(ss4_n46), .Y(
        ss4_n102) );
  NAND2_X0P5A_A12TR ss4_u41 ( .A(ss4_n6), .B(ss4_n41), .Y(ss4_n42) );
  INV_X0P5B_A12TR ss4_u40 ( .A(button_num[1]), .Y(ss4_n40) );
  AOI21_X0P5M_A12TR ss4_u39 ( .A0(ss4_n40), .A1(button_num[0]), .B0(n4), .Y(
        ss4_n10) );
  NOR2_X0P5A_A12TR ss4_u38 ( .A(ss4_n10), .B(ss4_n38), .Y(ss4_n43) );
  MXIT2_X0P5M_A12TR ss4_u37 ( .A(ss4_n41), .B(ss4_n42), .S0(ss4_n43), .Y(
        ss4_n101) );
  NAND2_X0P5A_A12TR ss4_u36 ( .A(ss4_n6), .B(ss4_n35), .Y(ss4_n36) );
  AOI21_X0P5M_A12TR ss4_u35 ( .A0(ss4_n39), .A1(ss4_n40), .B0(n4), .Y(ss4_n4)
         );
  NOR2_X0P5A_A12TR ss4_u34 ( .A(ss4_n4), .B(ss4_n38), .Y(ss4_n37) );
  MXIT2_X0P5M_A12TR ss4_u33 ( .A(ss4_n35), .B(ss4_n36), .S0(ss4_n37), .Y(
        ss4_n100) );
  NAND2_X0P5A_A12TR ss4_u32 ( .A(ss4_n6), .B(ss4_n32), .Y(ss4_n33) );
  OAI21_X0P5M_A12TR ss4_u31 ( .A0(n4), .A1(button_num[2]), .B0(ss4_n21), .Y(
        ss4_n25) );
  NOR2_X0P5A_A12TR ss4_u30 ( .A(ss4_n18), .B(ss4_n25), .Y(ss4_n34) );
  MXIT2_X0P5M_A12TR ss4_u29 ( .A(ss4_n32), .B(ss4_n33), .S0(ss4_n34), .Y(
        ss4_n99) );
  NAND2_X0P5A_A12TR ss4_u28 ( .A(ss4_n6), .B(ss4_n29), .Y(ss4_n30) );
  NOR2_X0P5A_A12TR ss4_u27 ( .A(ss4_n14), .B(ss4_n25), .Y(ss4_n31) );
  MXIT2_X0P5M_A12TR ss4_u26 ( .A(ss4_n29), .B(ss4_n30), .S0(ss4_n31), .Y(
        ss4_n98) );
  NAND2_X0P5A_A12TR ss4_u25 ( .A(ss4_n6), .B(ss4_n26), .Y(ss4_n27) );
  NOR2_X0P5A_A12TR ss4_u24 ( .A(ss4_n10), .B(ss4_n25), .Y(ss4_n28) );
  MXIT2_X0P5M_A12TR ss4_u23 ( .A(ss4_n26), .B(ss4_n27), .S0(ss4_n28), .Y(
        ss4_n97) );
  NAND2_X0P5A_A12TR ss4_u22 ( .A(ss4_n6), .B(ss4_n22), .Y(ss4_n23) );
  NOR2_X0P5A_A12TR ss4_u21 ( .A(ss4_n4), .B(ss4_n25), .Y(ss4_n24) );
  MXIT2_X0P5M_A12TR ss4_u20 ( .A(ss4_n22), .B(ss4_n23), .S0(ss4_n24), .Y(
        ss4_n96) );
  INV_X0P5B_A12TR ss4_u19 ( .A(ss_state_4[3]), .Y(ss4_n15) );
  NAND2_X0P5A_A12TR ss4_u18 ( .A(ss4_n6), .B(ss4_n15), .Y(ss4_n16) );
  INV_X0P5B_A12TR ss4_u17 ( .A(button_num[3]), .Y(ss4_n19) );
  INV_X0P5B_A12TR ss4_u16 ( .A(button_num[2]), .Y(ss4_n20) );
  AO21A1AI2_X0P5M_A12TR ss4_u15 ( .A0(ss4_n19), .A1(ss4_n20), .B0(n4), .C0(
        ss4_n21), .Y(ss4_n5) );
  NOR2_X0P5A_A12TR ss4_u14 ( .A(ss4_n18), .B(ss4_n5), .Y(ss4_n17) );
  MXIT2_X0P5M_A12TR ss4_u13 ( .A(ss4_n15), .B(ss4_n16), .S0(ss4_n17), .Y(
        ss4_n95) );
  NAND2_X0P5A_A12TR ss4_u12 ( .A(ss4_n6), .B(ss4_n11), .Y(ss4_n12) );
  NOR2_X0P5A_A12TR ss4_u11 ( .A(ss4_n14), .B(ss4_n5), .Y(ss4_n13) );
  MXIT2_X0P5M_A12TR ss4_u10 ( .A(ss4_n11), .B(ss4_n12), .S0(ss4_n13), .Y(
        ss4_n94) );
  NAND2_X0P5A_A12TR ss4_u9 ( .A(ss4_n6), .B(ss4_n7), .Y(ss4_n8) );
  NOR2_X0P5A_A12TR ss4_u8 ( .A(ss4_n10), .B(ss4_n5), .Y(ss4_n9) );
  MXIT2_X0P5M_A12TR ss4_u7 ( .A(ss4_n7), .B(ss4_n8), .S0(ss4_n9), .Y(ss4_n93)
         );
  INV_X0P5B_A12TR ss4_u6 ( .A(ss_state_4[0]), .Y(ss4_n1) );
  NAND2_X0P5A_A12TR ss4_u5 ( .A(ss4_n6), .B(ss4_n1), .Y(ss4_n2) );
  NOR2_X0P5A_A12TR ss4_u4 ( .A(ss4_n4), .B(ss4_n5), .Y(ss4_n3) );
  MXIT2_X0P5M_A12TR ss4_u3 ( .A(ss4_n1), .B(ss4_n2), .S0(ss4_n3), .Y(ss4_n92)
         );
  DFFQ_X1M_A12TR ss4_selection_state_reg_3_ ( .D(ss4_n95), .CK(osc_clk), .Q(
        ss_state_4[3]) );
  DFFQ_X1M_A12TR ss4_selection_state_reg_0_ ( .D(ss4_n92), .CK(osc_clk), .Q(
        ss_state_4[0]) );
  DFFQ_X1M_A12TR ss4_selection_state_reg_6_ ( .D(ss4_n98), .CK(osc_clk), .Q(
        ss_state_4[6]) );
  DFFQ_X1M_A12TR ss4_selection_state_reg_9_ ( .D(ss4_n101), .CK(osc_clk), .Q(
        ss_state_4[9]) );
  DFFQ_X1M_A12TR ss4_selection_state_reg_8_ ( .D(ss4_n100), .CK(osc_clk), .Q(
        ss_state_4[8]) );
  DFFQ_X1M_A12TR ss4_selection_state_reg_1_ ( .D(ss4_n93), .CK(osc_clk), .Q(
        ss_state_4[1]) );
  DFFQ_X1M_A12TR ss4_selection_state_reg_4_ ( .D(ss4_n96), .CK(osc_clk), .Q(
        ss_state_4[4]) );
  DFFQ_X1M_A12TR ss4_selection_state_reg_11_ ( .D(ss4_n103), .CK(osc_clk), .Q(
        ss_state_4[11]) );
  DFFQ_X1M_A12TR ss4_selection_state_reg_2_ ( .D(ss4_n94), .CK(osc_clk), .Q(
        ss_state_4[2]) );
  DFFQ_X1M_A12TR ss4_selection_state_reg_5_ ( .D(ss4_n97), .CK(osc_clk), .Q(
        ss_state_4[5]) );
  DFFQ_X1M_A12TR ss4_selection_state_reg_10_ ( .D(ss4_n102), .CK(osc_clk), .Q(
        ss_state_4[10]) );
  DFFQ_X1M_A12TR ss4_selection_state_reg_7_ ( .D(ss4_n99), .CK(osc_clk), .Q(
        ss_state_4[7]) );
  INV_X0P5B_A12TR ss5_u105 ( .A(ss_state_5[11]), .Y(ss5_n47) );
  INV_X0P5B_A12TR ss5_u104 ( .A(ss_state_5[7]), .Y(ss5_n32) );
  INV_X0P5B_A12TR ss5_u103 ( .A(ss_state_5[8]), .Y(ss5_n35) );
  XNOR2_X0P5M_A12TR ss5_u102 ( .A(ss5_n32), .B(ss5_n35), .Y(ss5_n81) );
  INV_X0P5B_A12TR ss5_u101 ( .A(ss5_n81), .Y(ss5_n91) );
  AOI22_X0P5M_A12TR ss5_u100 ( .A0(ss_state_5[6]), .A1(ss5_n91), .B0(
        ss_state_5[7]), .B1(ss_state_5[8]), .Y(ss5_n62) );
  XOR2_X0P5M_A12TR ss5_u99 ( .A(ss_state_5[6]), .B(ss5_n91), .Y(ss5_n84) );
  INV_X0P5B_A12TR ss5_u98 ( .A(ss_state_5[10]), .Y(ss5_n44) );
  XNOR2_X0P5M_A12TR ss5_u97 ( .A(ss5_n44), .B(ss5_n47), .Y(ss5_n80) );
  INV_X0P5B_A12TR ss5_u96 ( .A(ss5_n80), .Y(ss5_n90) );
  XOR2_X0P5M_A12TR ss5_u95 ( .A(ss_state_5[9]), .B(ss5_n90), .Y(ss5_n85) );
  NAND2_X0P5A_A12TR ss5_u94 ( .A(ss5_n84), .B(ss5_n85), .Y(ss5_n64) );
  AOI22_X0P5M_A12TR ss5_u93 ( .A0(ss_state_5[10]), .A1(ss_state_5[11]), .B0(
        ss_state_5[9]), .B1(ss5_n90), .Y(ss5_n63) );
  CGEN_X1M_A12TR ss5_u92 ( .A(ss5_n62), .B(ss5_n64), .CI(ss5_n63), .CO(ss5_n75) );
  INV_X0P5B_A12TR ss5_u91 ( .A(ss_state_5[4]), .Y(ss5_n22) );
  INV_X0P5B_A12TR ss5_u90 ( .A(ss_state_5[5]), .Y(ss5_n26) );
  XOR2_X0P5M_A12TR ss5_u89 ( .A(ss5_n22), .B(ss5_n26), .Y(ss5_n88) );
  AOI22_X0P5M_A12TR ss5_u88 ( .A0(ss_state_5[5]), .A1(ss_state_5[4]), .B0(
        ss5_n88), .B1(ss_state_5[3]), .Y(ss5_n86) );
  INV_X0P5B_A12TR ss5_u87 ( .A(ss_state_5[1]), .Y(ss5_n7) );
  INV_X0P5B_A12TR ss5_u86 ( .A(ss_state_5[2]), .Y(ss5_n11) );
  XOR2_X0P5M_A12TR ss5_u85 ( .A(ss5_n7), .B(ss5_n11), .Y(ss5_n89) );
  AOI22_X0P5M_A12TR ss5_u84 ( .A0(ss_state_5[2]), .A1(ss_state_5[1]), .B0(
        ss5_n89), .B1(ss_state_5[0]), .Y(ss5_n87) );
  XNOR2_X0P5M_A12TR ss5_u83 ( .A(ss5_n87), .B(ss5_n86), .Y(ss5_n73) );
  XOR2_X0P5M_A12TR ss5_u82 ( .A(ss_state_5[0]), .B(ss5_n89), .Y(ss5_n82) );
  XOR2_X0P5M_A12TR ss5_u81 ( .A(ss_state_5[3]), .B(ss5_n88), .Y(ss5_n83) );
  NAND2_X0P5A_A12TR ss5_u80 ( .A(ss5_n82), .B(ss5_n83), .Y(ss5_n74) );
  OAI22_X0P5M_A12TR ss5_u79 ( .A0(ss5_n86), .A1(ss5_n87), .B0(ss5_n73), .B1(
        ss5_n74), .Y(ss5_n60) );
  INV_X0P5B_A12TR ss5_u78 ( .A(ss5_n60), .Y(ss5_n76) );
  XOR2_X0P5M_A12TR ss5_u77 ( .A(ss5_n74), .B(ss5_n73), .Y(ss5_n77) );
  OAI21_X0P5M_A12TR ss5_u76 ( .A0(ss5_n84), .A1(ss5_n85), .B0(ss5_n64), .Y(
        ss5_n68) );
  OAI21_X0P5M_A12TR ss5_u75 ( .A0(ss5_n82), .A1(ss5_n83), .B0(ss5_n74), .Y(
        ss5_n69) );
  NOR2_X0P5A_A12TR ss5_u74 ( .A(ss5_n68), .B(ss5_n69), .Y(ss5_n70) );
  INV_X0P5B_A12TR ss5_u73 ( .A(ss_state_5[6]), .Y(ss5_n29) );
  OAI22_X0P5M_A12TR ss5_u72 ( .A0(ss5_n35), .A1(ss5_n32), .B0(ss5_n81), .B1(
        ss5_n29), .Y(ss5_n78) );
  INV_X0P5B_A12TR ss5_u71 ( .A(ss_state_5[9]), .Y(ss5_n41) );
  OAI22_X0P5M_A12TR ss5_u70 ( .A0(ss5_n80), .A1(ss5_n41), .B0(ss5_n47), .B1(
        ss5_n44), .Y(ss5_n79) );
  XNOR3_X0P5M_A12TR ss5_u69 ( .A(ss5_n64), .B(ss5_n78), .C(ss5_n79), .Y(
        ss5_n71) );
  CGENI_X1M_A12TR ss5_u68 ( .A(ss5_n77), .B(ss5_n70), .CI(ss5_n71), .CON(
        ss5_n59) );
  CGEN_X1M_A12TR ss5_u67 ( .A(ss5_n75), .B(ss5_n76), .CI(ss5_n59), .CO(ss5_n54) );
  XNOR2_X0P5M_A12TR ss5_u66 ( .A(ss5_n73), .B(ss5_n74), .Y(ss5_n72) );
  XOR3_X0P5M_A12TR ss5_u65 ( .A(ss5_n71), .B(ss5_n70), .C(ss5_n72), .Y(ss5_n65) );
  AO21_X0P5M_A12TR ss5_u64 ( .A0(ss5_n68), .A1(ss5_n69), .B0(ss5_n70), .Y(
        ss5_n67) );
  OA211_X0P5M_A12TR ss5_u63 ( .A0(max_selections[1]), .A1(ss5_n65), .B0(
        ss5_n67), .C0(max_selections[0]), .Y(ss5_n66) );
  AOI21_X0P5M_A12TR ss5_u62 ( .A0(ss5_n65), .A1(max_selections[1]), .B0(
        ss5_n66), .Y(ss5_n56) );
  CGENI_X1M_A12TR ss5_u61 ( .A(ss5_n62), .B(ss5_n63), .CI(ss5_n64), .CON(
        ss5_n61) );
  XNOR3_X0P5M_A12TR ss5_u60 ( .A(ss5_n59), .B(ss5_n60), .C(ss5_n61), .Y(
        ss5_n57) );
  AO1B2_X0P5M_A12TR ss5_u59 ( .B0(ss5_n57), .B1(ss5_n56), .A0N(
        max_selections[2]), .Y(ss5_n58) );
  OAI21_X0P5M_A12TR ss5_u58 ( .A0(ss5_n56), .A1(ss5_n57), .B0(ss5_n58), .Y(
        ss5_n55) );
  AND2_X0P5M_A12TR ss5_u57 ( .A(ss5_n54), .B(ss5_n55), .Y(ss5_n53) );
  INV_X0P5B_A12TR ss5_u56 ( .A(n4), .Y(ss5_n51) );
  OAI221_X0P5M_A12TR ss5_u55 ( .A0(max_selections[3]), .A1(ss5_n53), .B0(
        ss5_n54), .B1(ss5_n55), .C0(ss5_n51), .Y(ss5_n52) );
  INV_X0P5B_A12TR ss5_u54 ( .A(ss5_n52), .Y(ss5_n6) );
  NAND2_X0P5A_A12TR ss5_u53 ( .A(ss5_n6), .B(ss5_n47), .Y(ss5_n48) );
  AOI21_X0P5M_A12TR ss5_u52 ( .A0(button_num[0]), .A1(button_num[1]), .B0(n4), 
        .Y(ss5_n18) );
  INV_X0P5B_A12TR ss5_u51 ( .A(ss_selector[5]), .Y(ss5_n50) );
  AO21A1AI2_X0P5M_A12TR ss5_u50 ( .A0(button_num[3]), .A1(button_num[2]), .B0(
        ss5_n50), .C0(ss5_n51), .Y(ss5_n21) );
  OAI21_X0P5M_A12TR ss5_u49 ( .A0(n4), .A1(button_num[3]), .B0(ss5_n21), .Y(
        ss5_n38) );
  NOR2_X0P5A_A12TR ss5_u48 ( .A(ss5_n18), .B(ss5_n38), .Y(ss5_n49) );
  MXIT2_X0P5M_A12TR ss5_u47 ( .A(ss5_n47), .B(ss5_n48), .S0(ss5_n49), .Y(
        ss5_n103) );
  NAND2_X0P5A_A12TR ss5_u46 ( .A(ss5_n6), .B(ss5_n44), .Y(ss5_n45) );
  INV_X0P5B_A12TR ss5_u45 ( .A(button_num[0]), .Y(ss5_n39) );
  AOI21_X0P5M_A12TR ss5_u44 ( .A0(ss5_n39), .A1(button_num[1]), .B0(n4), .Y(
        ss5_n14) );
  NOR2_X0P5A_A12TR ss5_u43 ( .A(ss5_n14), .B(ss5_n38), .Y(ss5_n46) );
  MXIT2_X0P5M_A12TR ss5_u42 ( .A(ss5_n44), .B(ss5_n45), .S0(ss5_n46), .Y(
        ss5_n102) );
  NAND2_X0P5A_A12TR ss5_u41 ( .A(ss5_n6), .B(ss5_n41), .Y(ss5_n42) );
  INV_X0P5B_A12TR ss5_u40 ( .A(button_num[1]), .Y(ss5_n40) );
  AOI21_X0P5M_A12TR ss5_u39 ( .A0(ss5_n40), .A1(button_num[0]), .B0(n4), .Y(
        ss5_n10) );
  NOR2_X0P5A_A12TR ss5_u38 ( .A(ss5_n10), .B(ss5_n38), .Y(ss5_n43) );
  MXIT2_X0P5M_A12TR ss5_u37 ( .A(ss5_n41), .B(ss5_n42), .S0(ss5_n43), .Y(
        ss5_n101) );
  NAND2_X0P5A_A12TR ss5_u36 ( .A(ss5_n6), .B(ss5_n35), .Y(ss5_n36) );
  AOI21_X0P5M_A12TR ss5_u35 ( .A0(ss5_n39), .A1(ss5_n40), .B0(n4), .Y(ss5_n4)
         );
  NOR2_X0P5A_A12TR ss5_u34 ( .A(ss5_n4), .B(ss5_n38), .Y(ss5_n37) );
  MXIT2_X0P5M_A12TR ss5_u33 ( .A(ss5_n35), .B(ss5_n36), .S0(ss5_n37), .Y(
        ss5_n100) );
  NAND2_X0P5A_A12TR ss5_u32 ( .A(ss5_n6), .B(ss5_n32), .Y(ss5_n33) );
  OAI21_X0P5M_A12TR ss5_u31 ( .A0(n4), .A1(button_num[2]), .B0(ss5_n21), .Y(
        ss5_n25) );
  NOR2_X0P5A_A12TR ss5_u30 ( .A(ss5_n18), .B(ss5_n25), .Y(ss5_n34) );
  MXIT2_X0P5M_A12TR ss5_u29 ( .A(ss5_n32), .B(ss5_n33), .S0(ss5_n34), .Y(
        ss5_n99) );
  NAND2_X0P5A_A12TR ss5_u28 ( .A(ss5_n6), .B(ss5_n29), .Y(ss5_n30) );
  NOR2_X0P5A_A12TR ss5_u27 ( .A(ss5_n14), .B(ss5_n25), .Y(ss5_n31) );
  MXIT2_X0P5M_A12TR ss5_u26 ( .A(ss5_n29), .B(ss5_n30), .S0(ss5_n31), .Y(
        ss5_n98) );
  NAND2_X0P5A_A12TR ss5_u25 ( .A(ss5_n6), .B(ss5_n26), .Y(ss5_n27) );
  NOR2_X0P5A_A12TR ss5_u24 ( .A(ss5_n10), .B(ss5_n25), .Y(ss5_n28) );
  MXIT2_X0P5M_A12TR ss5_u23 ( .A(ss5_n26), .B(ss5_n27), .S0(ss5_n28), .Y(
        ss5_n97) );
  NAND2_X0P5A_A12TR ss5_u22 ( .A(ss5_n6), .B(ss5_n22), .Y(ss5_n23) );
  NOR2_X0P5A_A12TR ss5_u21 ( .A(ss5_n4), .B(ss5_n25), .Y(ss5_n24) );
  MXIT2_X0P5M_A12TR ss5_u20 ( .A(ss5_n22), .B(ss5_n23), .S0(ss5_n24), .Y(
        ss5_n96) );
  INV_X0P5B_A12TR ss5_u19 ( .A(ss_state_5[3]), .Y(ss5_n15) );
  NAND2_X0P5A_A12TR ss5_u18 ( .A(ss5_n6), .B(ss5_n15), .Y(ss5_n16) );
  INV_X0P5B_A12TR ss5_u17 ( .A(button_num[3]), .Y(ss5_n19) );
  INV_X0P5B_A12TR ss5_u16 ( .A(button_num[2]), .Y(ss5_n20) );
  AO21A1AI2_X0P5M_A12TR ss5_u15 ( .A0(ss5_n19), .A1(ss5_n20), .B0(n4), .C0(
        ss5_n21), .Y(ss5_n5) );
  NOR2_X0P5A_A12TR ss5_u14 ( .A(ss5_n18), .B(ss5_n5), .Y(ss5_n17) );
  MXIT2_X0P5M_A12TR ss5_u13 ( .A(ss5_n15), .B(ss5_n16), .S0(ss5_n17), .Y(
        ss5_n95) );
  NAND2_X0P5A_A12TR ss5_u12 ( .A(ss5_n6), .B(ss5_n11), .Y(ss5_n12) );
  NOR2_X0P5A_A12TR ss5_u11 ( .A(ss5_n14), .B(ss5_n5), .Y(ss5_n13) );
  MXIT2_X0P5M_A12TR ss5_u10 ( .A(ss5_n11), .B(ss5_n12), .S0(ss5_n13), .Y(
        ss5_n94) );
  NAND2_X0P5A_A12TR ss5_u9 ( .A(ss5_n6), .B(ss5_n7), .Y(ss5_n8) );
  NOR2_X0P5A_A12TR ss5_u8 ( .A(ss5_n10), .B(ss5_n5), .Y(ss5_n9) );
  MXIT2_X0P5M_A12TR ss5_u7 ( .A(ss5_n7), .B(ss5_n8), .S0(ss5_n9), .Y(ss5_n93)
         );
  INV_X0P5B_A12TR ss5_u6 ( .A(ss_state_5[0]), .Y(ss5_n1) );
  NAND2_X0P5A_A12TR ss5_u5 ( .A(ss5_n6), .B(ss5_n1), .Y(ss5_n2) );
  NOR2_X0P5A_A12TR ss5_u4 ( .A(ss5_n4), .B(ss5_n5), .Y(ss5_n3) );
  MXIT2_X0P5M_A12TR ss5_u3 ( .A(ss5_n1), .B(ss5_n2), .S0(ss5_n3), .Y(ss5_n92)
         );
  DFFQ_X1M_A12TR ss5_selection_state_reg_3_ ( .D(ss5_n95), .CK(osc_clk), .Q(
        ss_state_5[3]) );
  DFFQ_X1M_A12TR ss5_selection_state_reg_0_ ( .D(ss5_n92), .CK(osc_clk), .Q(
        ss_state_5[0]) );
  DFFQ_X1M_A12TR ss5_selection_state_reg_6_ ( .D(ss5_n98), .CK(osc_clk), .Q(
        ss_state_5[6]) );
  DFFQ_X1M_A12TR ss5_selection_state_reg_9_ ( .D(ss5_n101), .CK(osc_clk), .Q(
        ss_state_5[9]) );
  DFFQ_X1M_A12TR ss5_selection_state_reg_8_ ( .D(ss5_n100), .CK(osc_clk), .Q(
        ss_state_5[8]) );
  DFFQ_X1M_A12TR ss5_selection_state_reg_1_ ( .D(ss5_n93), .CK(osc_clk), .Q(
        ss_state_5[1]) );
  DFFQ_X1M_A12TR ss5_selection_state_reg_4_ ( .D(ss5_n96), .CK(osc_clk), .Q(
        ss_state_5[4]) );
  DFFQ_X1M_A12TR ss5_selection_state_reg_11_ ( .D(ss5_n103), .CK(osc_clk), .Q(
        ss_state_5[11]) );
  DFFQ_X1M_A12TR ss5_selection_state_reg_2_ ( .D(ss5_n94), .CK(osc_clk), .Q(
        ss_state_5[2]) );
  DFFQ_X1M_A12TR ss5_selection_state_reg_5_ ( .D(ss5_n97), .CK(osc_clk), .Q(
        ss_state_5[5]) );
  DFFQ_X1M_A12TR ss5_selection_state_reg_10_ ( .D(ss5_n102), .CK(osc_clk), .Q(
        ss_state_5[10]) );
  DFFQ_X1M_A12TR ss5_selection_state_reg_7_ ( .D(ss5_n99), .CK(osc_clk), .Q(
        ss_state_5[7]) );
  INV_X0P5B_A12TR ss6_u105 ( .A(ss_state_6[11]), .Y(ss6_n47) );
  INV_X0P5B_A12TR ss6_u104 ( .A(ss_state_6[7]), .Y(ss6_n32) );
  INV_X0P5B_A12TR ss6_u103 ( .A(ss_state_6[8]), .Y(ss6_n35) );
  XNOR2_X0P5M_A12TR ss6_u102 ( .A(ss6_n32), .B(ss6_n35), .Y(ss6_n81) );
  INV_X0P5B_A12TR ss6_u101 ( .A(ss6_n81), .Y(ss6_n91) );
  AOI22_X0P5M_A12TR ss6_u100 ( .A0(ss_state_6[6]), .A1(ss6_n91), .B0(
        ss_state_6[7]), .B1(ss_state_6[8]), .Y(ss6_n62) );
  XOR2_X0P5M_A12TR ss6_u99 ( .A(ss_state_6[6]), .B(ss6_n91), .Y(ss6_n84) );
  INV_X0P5B_A12TR ss6_u98 ( .A(ss_state_6[10]), .Y(ss6_n44) );
  XNOR2_X0P5M_A12TR ss6_u97 ( .A(ss6_n44), .B(ss6_n47), .Y(ss6_n80) );
  INV_X0P5B_A12TR ss6_u96 ( .A(ss6_n80), .Y(ss6_n90) );
  XOR2_X0P5M_A12TR ss6_u95 ( .A(ss_state_6[9]), .B(ss6_n90), .Y(ss6_n85) );
  NAND2_X0P5A_A12TR ss6_u94 ( .A(ss6_n84), .B(ss6_n85), .Y(ss6_n64) );
  AOI22_X0P5M_A12TR ss6_u93 ( .A0(ss_state_6[10]), .A1(ss_state_6[11]), .B0(
        ss_state_6[9]), .B1(ss6_n90), .Y(ss6_n63) );
  CGEN_X1M_A12TR ss6_u92 ( .A(ss6_n62), .B(ss6_n64), .CI(ss6_n63), .CO(ss6_n75) );
  INV_X0P5B_A12TR ss6_u91 ( .A(ss_state_6[4]), .Y(ss6_n22) );
  INV_X0P5B_A12TR ss6_u90 ( .A(ss_state_6[5]), .Y(ss6_n26) );
  XOR2_X0P5M_A12TR ss6_u89 ( .A(ss6_n22), .B(ss6_n26), .Y(ss6_n88) );
  AOI22_X0P5M_A12TR ss6_u88 ( .A0(ss_state_6[5]), .A1(ss_state_6[4]), .B0(
        ss6_n88), .B1(ss_state_6[3]), .Y(ss6_n86) );
  INV_X0P5B_A12TR ss6_u87 ( .A(ss_state_6[1]), .Y(ss6_n7) );
  INV_X0P5B_A12TR ss6_u86 ( .A(ss_state_6[2]), .Y(ss6_n11) );
  XOR2_X0P5M_A12TR ss6_u85 ( .A(ss6_n7), .B(ss6_n11), .Y(ss6_n89) );
  AOI22_X0P5M_A12TR ss6_u84 ( .A0(ss_state_6[2]), .A1(ss_state_6[1]), .B0(
        ss6_n89), .B1(ss_state_6[0]), .Y(ss6_n87) );
  XNOR2_X0P5M_A12TR ss6_u83 ( .A(ss6_n87), .B(ss6_n86), .Y(ss6_n73) );
  XOR2_X0P5M_A12TR ss6_u82 ( .A(ss_state_6[0]), .B(ss6_n89), .Y(ss6_n82) );
  XOR2_X0P5M_A12TR ss6_u81 ( .A(ss_state_6[3]), .B(ss6_n88), .Y(ss6_n83) );
  NAND2_X0P5A_A12TR ss6_u80 ( .A(ss6_n82), .B(ss6_n83), .Y(ss6_n74) );
  OAI22_X0P5M_A12TR ss6_u79 ( .A0(ss6_n86), .A1(ss6_n87), .B0(ss6_n73), .B1(
        ss6_n74), .Y(ss6_n60) );
  INV_X0P5B_A12TR ss6_u78 ( .A(ss6_n60), .Y(ss6_n76) );
  XOR2_X0P5M_A12TR ss6_u77 ( .A(ss6_n74), .B(ss6_n73), .Y(ss6_n77) );
  OAI21_X0P5M_A12TR ss6_u76 ( .A0(ss6_n84), .A1(ss6_n85), .B0(ss6_n64), .Y(
        ss6_n68) );
  OAI21_X0P5M_A12TR ss6_u75 ( .A0(ss6_n82), .A1(ss6_n83), .B0(ss6_n74), .Y(
        ss6_n69) );
  NOR2_X0P5A_A12TR ss6_u74 ( .A(ss6_n68), .B(ss6_n69), .Y(ss6_n70) );
  INV_X0P5B_A12TR ss6_u73 ( .A(ss_state_6[6]), .Y(ss6_n29) );
  OAI22_X0P5M_A12TR ss6_u72 ( .A0(ss6_n35), .A1(ss6_n32), .B0(ss6_n81), .B1(
        ss6_n29), .Y(ss6_n78) );
  INV_X0P5B_A12TR ss6_u71 ( .A(ss_state_6[9]), .Y(ss6_n41) );
  OAI22_X0P5M_A12TR ss6_u70 ( .A0(ss6_n80), .A1(ss6_n41), .B0(ss6_n47), .B1(
        ss6_n44), .Y(ss6_n79) );
  XNOR3_X0P5M_A12TR ss6_u69 ( .A(ss6_n64), .B(ss6_n78), .C(ss6_n79), .Y(
        ss6_n71) );
  CGENI_X1M_A12TR ss6_u68 ( .A(ss6_n77), .B(ss6_n70), .CI(ss6_n71), .CON(
        ss6_n59) );
  CGEN_X1M_A12TR ss6_u67 ( .A(ss6_n75), .B(ss6_n76), .CI(ss6_n59), .CO(ss6_n54) );
  XNOR2_X0P5M_A12TR ss6_u66 ( .A(ss6_n73), .B(ss6_n74), .Y(ss6_n72) );
  XOR3_X0P5M_A12TR ss6_u65 ( .A(ss6_n71), .B(ss6_n70), .C(ss6_n72), .Y(ss6_n65) );
  AO21_X0P5M_A12TR ss6_u64 ( .A0(ss6_n68), .A1(ss6_n69), .B0(ss6_n70), .Y(
        ss6_n67) );
  OA211_X0P5M_A12TR ss6_u63 ( .A0(max_selections[1]), .A1(ss6_n65), .B0(
        ss6_n67), .C0(max_selections[0]), .Y(ss6_n66) );
  AOI21_X0P5M_A12TR ss6_u62 ( .A0(ss6_n65), .A1(max_selections[1]), .B0(
        ss6_n66), .Y(ss6_n56) );
  CGENI_X1M_A12TR ss6_u61 ( .A(ss6_n62), .B(ss6_n63), .CI(ss6_n64), .CON(
        ss6_n61) );
  XNOR3_X0P5M_A12TR ss6_u60 ( .A(ss6_n59), .B(ss6_n60), .C(ss6_n61), .Y(
        ss6_n57) );
  AO1B2_X0P5M_A12TR ss6_u59 ( .B0(ss6_n57), .B1(ss6_n56), .A0N(
        max_selections[2]), .Y(ss6_n58) );
  OAI21_X0P5M_A12TR ss6_u58 ( .A0(ss6_n56), .A1(ss6_n57), .B0(ss6_n58), .Y(
        ss6_n55) );
  AND2_X0P5M_A12TR ss6_u57 ( .A(ss6_n54), .B(ss6_n55), .Y(ss6_n53) );
  INV_X0P5B_A12TR ss6_u56 ( .A(n4), .Y(ss6_n51) );
  OAI221_X0P5M_A12TR ss6_u55 ( .A0(max_selections[3]), .A1(ss6_n53), .B0(
        ss6_n54), .B1(ss6_n55), .C0(ss6_n51), .Y(ss6_n52) );
  INV_X0P5B_A12TR ss6_u54 ( .A(ss6_n52), .Y(ss6_n6) );
  NAND2_X0P5A_A12TR ss6_u53 ( .A(ss6_n6), .B(ss6_n47), .Y(ss6_n48) );
  AOI21_X0P5M_A12TR ss6_u52 ( .A0(button_num[0]), .A1(button_num[1]), .B0(n4), 
        .Y(ss6_n18) );
  INV_X0P5B_A12TR ss6_u51 ( .A(ss_selector[6]), .Y(ss6_n50) );
  AO21A1AI2_X0P5M_A12TR ss6_u50 ( .A0(button_num[3]), .A1(button_num[2]), .B0(
        ss6_n50), .C0(ss6_n51), .Y(ss6_n21) );
  OAI21_X0P5M_A12TR ss6_u49 ( .A0(n4), .A1(button_num[3]), .B0(ss6_n21), .Y(
        ss6_n38) );
  NOR2_X0P5A_A12TR ss6_u48 ( .A(ss6_n18), .B(ss6_n38), .Y(ss6_n49) );
  MXIT2_X0P5M_A12TR ss6_u47 ( .A(ss6_n47), .B(ss6_n48), .S0(ss6_n49), .Y(
        ss6_n103) );
  NAND2_X0P5A_A12TR ss6_u46 ( .A(ss6_n6), .B(ss6_n44), .Y(ss6_n45) );
  INV_X0P5B_A12TR ss6_u45 ( .A(button_num[0]), .Y(ss6_n39) );
  AOI21_X0P5M_A12TR ss6_u44 ( .A0(ss6_n39), .A1(button_num[1]), .B0(n4), .Y(
        ss6_n14) );
  NOR2_X0P5A_A12TR ss6_u43 ( .A(ss6_n14), .B(ss6_n38), .Y(ss6_n46) );
  MXIT2_X0P5M_A12TR ss6_u42 ( .A(ss6_n44), .B(ss6_n45), .S0(ss6_n46), .Y(
        ss6_n102) );
  NAND2_X0P5A_A12TR ss6_u41 ( .A(ss6_n6), .B(ss6_n41), .Y(ss6_n42) );
  INV_X0P5B_A12TR ss6_u40 ( .A(button_num[1]), .Y(ss6_n40) );
  AOI21_X0P5M_A12TR ss6_u39 ( .A0(ss6_n40), .A1(button_num[0]), .B0(n4), .Y(
        ss6_n10) );
  NOR2_X0P5A_A12TR ss6_u38 ( .A(ss6_n10), .B(ss6_n38), .Y(ss6_n43) );
  MXIT2_X0P5M_A12TR ss6_u37 ( .A(ss6_n41), .B(ss6_n42), .S0(ss6_n43), .Y(
        ss6_n101) );
  NAND2_X0P5A_A12TR ss6_u36 ( .A(ss6_n6), .B(ss6_n35), .Y(ss6_n36) );
  AOI21_X0P5M_A12TR ss6_u35 ( .A0(ss6_n39), .A1(ss6_n40), .B0(n4), .Y(ss6_n4)
         );
  NOR2_X0P5A_A12TR ss6_u34 ( .A(ss6_n4), .B(ss6_n38), .Y(ss6_n37) );
  MXIT2_X0P5M_A12TR ss6_u33 ( .A(ss6_n35), .B(ss6_n36), .S0(ss6_n37), .Y(
        ss6_n100) );
  NAND2_X0P5A_A12TR ss6_u32 ( .A(ss6_n6), .B(ss6_n32), .Y(ss6_n33) );
  OAI21_X0P5M_A12TR ss6_u31 ( .A0(n4), .A1(button_num[2]), .B0(ss6_n21), .Y(
        ss6_n25) );
  NOR2_X0P5A_A12TR ss6_u30 ( .A(ss6_n18), .B(ss6_n25), .Y(ss6_n34) );
  MXIT2_X0P5M_A12TR ss6_u29 ( .A(ss6_n32), .B(ss6_n33), .S0(ss6_n34), .Y(
        ss6_n99) );
  NAND2_X0P5A_A12TR ss6_u28 ( .A(ss6_n6), .B(ss6_n29), .Y(ss6_n30) );
  NOR2_X0P5A_A12TR ss6_u27 ( .A(ss6_n14), .B(ss6_n25), .Y(ss6_n31) );
  MXIT2_X0P5M_A12TR ss6_u26 ( .A(ss6_n29), .B(ss6_n30), .S0(ss6_n31), .Y(
        ss6_n98) );
  NAND2_X0P5A_A12TR ss6_u25 ( .A(ss6_n6), .B(ss6_n26), .Y(ss6_n27) );
  NOR2_X0P5A_A12TR ss6_u24 ( .A(ss6_n10), .B(ss6_n25), .Y(ss6_n28) );
  MXIT2_X0P5M_A12TR ss6_u23 ( .A(ss6_n26), .B(ss6_n27), .S0(ss6_n28), .Y(
        ss6_n97) );
  NAND2_X0P5A_A12TR ss6_u22 ( .A(ss6_n6), .B(ss6_n22), .Y(ss6_n23) );
  NOR2_X0P5A_A12TR ss6_u21 ( .A(ss6_n4), .B(ss6_n25), .Y(ss6_n24) );
  MXIT2_X0P5M_A12TR ss6_u20 ( .A(ss6_n22), .B(ss6_n23), .S0(ss6_n24), .Y(
        ss6_n96) );
  INV_X0P5B_A12TR ss6_u19 ( .A(ss_state_6[3]), .Y(ss6_n15) );
  NAND2_X0P5A_A12TR ss6_u18 ( .A(ss6_n6), .B(ss6_n15), .Y(ss6_n16) );
  INV_X0P5B_A12TR ss6_u17 ( .A(button_num[3]), .Y(ss6_n19) );
  INV_X0P5B_A12TR ss6_u16 ( .A(button_num[2]), .Y(ss6_n20) );
  AO21A1AI2_X0P5M_A12TR ss6_u15 ( .A0(ss6_n19), .A1(ss6_n20), .B0(n4), .C0(
        ss6_n21), .Y(ss6_n5) );
  NOR2_X0P5A_A12TR ss6_u14 ( .A(ss6_n18), .B(ss6_n5), .Y(ss6_n17) );
  MXIT2_X0P5M_A12TR ss6_u13 ( .A(ss6_n15), .B(ss6_n16), .S0(ss6_n17), .Y(
        ss6_n95) );
  NAND2_X0P5A_A12TR ss6_u12 ( .A(ss6_n6), .B(ss6_n11), .Y(ss6_n12) );
  NOR2_X0P5A_A12TR ss6_u11 ( .A(ss6_n14), .B(ss6_n5), .Y(ss6_n13) );
  MXIT2_X0P5M_A12TR ss6_u10 ( .A(ss6_n11), .B(ss6_n12), .S0(ss6_n13), .Y(
        ss6_n94) );
  NAND2_X0P5A_A12TR ss6_u9 ( .A(ss6_n6), .B(ss6_n7), .Y(ss6_n8) );
  NOR2_X0P5A_A12TR ss6_u8 ( .A(ss6_n10), .B(ss6_n5), .Y(ss6_n9) );
  MXIT2_X0P5M_A12TR ss6_u7 ( .A(ss6_n7), .B(ss6_n8), .S0(ss6_n9), .Y(ss6_n93)
         );
  INV_X0P5B_A12TR ss6_u6 ( .A(ss_state_6[0]), .Y(ss6_n1) );
  NAND2_X0P5A_A12TR ss6_u5 ( .A(ss6_n6), .B(ss6_n1), .Y(ss6_n2) );
  NOR2_X0P5A_A12TR ss6_u4 ( .A(ss6_n4), .B(ss6_n5), .Y(ss6_n3) );
  MXIT2_X0P5M_A12TR ss6_u3 ( .A(ss6_n1), .B(ss6_n2), .S0(ss6_n3), .Y(ss6_n92)
         );
  DFFQ_X1M_A12TR ss6_selection_state_reg_3_ ( .D(ss6_n95), .CK(osc_clk), .Q(
        ss_state_6[3]) );
  DFFQ_X1M_A12TR ss6_selection_state_reg_0_ ( .D(ss6_n92), .CK(osc_clk), .Q(
        ss_state_6[0]) );
  DFFQ_X1M_A12TR ss6_selection_state_reg_6_ ( .D(ss6_n98), .CK(osc_clk), .Q(
        ss_state_6[6]) );
  DFFQ_X1M_A12TR ss6_selection_state_reg_9_ ( .D(ss6_n101), .CK(osc_clk), .Q(
        ss_state_6[9]) );
  DFFQ_X1M_A12TR ss6_selection_state_reg_8_ ( .D(ss6_n100), .CK(osc_clk), .Q(
        ss_state_6[8]) );
  DFFQ_X1M_A12TR ss6_selection_state_reg_1_ ( .D(ss6_n93), .CK(osc_clk), .Q(
        ss_state_6[1]) );
  DFFQ_X1M_A12TR ss6_selection_state_reg_4_ ( .D(ss6_n96), .CK(osc_clk), .Q(
        ss_state_6[4]) );
  DFFQ_X1M_A12TR ss6_selection_state_reg_11_ ( .D(ss6_n103), .CK(osc_clk), .Q(
        ss_state_6[11]) );
  DFFQ_X1M_A12TR ss6_selection_state_reg_2_ ( .D(ss6_n94), .CK(osc_clk), .Q(
        ss_state_6[2]) );
  DFFQ_X1M_A12TR ss6_selection_state_reg_5_ ( .D(ss6_n97), .CK(osc_clk), .Q(
        ss_state_6[5]) );
  DFFQ_X1M_A12TR ss6_selection_state_reg_10_ ( .D(ss6_n102), .CK(osc_clk), .Q(
        ss_state_6[10]) );
  DFFQ_X1M_A12TR ss6_selection_state_reg_7_ ( .D(ss6_n99), .CK(osc_clk), .Q(
        ss_state_6[7]) );
  INV_X0P5B_A12TR ss7_u105 ( .A(ss_state_7[11]), .Y(ss7_n47) );
  INV_X0P5B_A12TR ss7_u104 ( .A(ss_state_7[7]), .Y(ss7_n32) );
  INV_X0P5B_A12TR ss7_u103 ( .A(ss_state_7[8]), .Y(ss7_n35) );
  XNOR2_X0P5M_A12TR ss7_u102 ( .A(ss7_n32), .B(ss7_n35), .Y(ss7_n81) );
  INV_X0P5B_A12TR ss7_u101 ( .A(ss7_n81), .Y(ss7_n91) );
  AOI22_X0P5M_A12TR ss7_u100 ( .A0(ss_state_7[6]), .A1(ss7_n91), .B0(
        ss_state_7[7]), .B1(ss_state_7[8]), .Y(ss7_n62) );
  XOR2_X0P5M_A12TR ss7_u99 ( .A(ss_state_7[6]), .B(ss7_n91), .Y(ss7_n84) );
  INV_X0P5B_A12TR ss7_u98 ( .A(ss_state_7[10]), .Y(ss7_n44) );
  XNOR2_X0P5M_A12TR ss7_u97 ( .A(ss7_n44), .B(ss7_n47), .Y(ss7_n80) );
  INV_X0P5B_A12TR ss7_u96 ( .A(ss7_n80), .Y(ss7_n90) );
  XOR2_X0P5M_A12TR ss7_u95 ( .A(ss_state_7[9]), .B(ss7_n90), .Y(ss7_n85) );
  NAND2_X0P5A_A12TR ss7_u94 ( .A(ss7_n84), .B(ss7_n85), .Y(ss7_n64) );
  AOI22_X0P5M_A12TR ss7_u93 ( .A0(ss_state_7[10]), .A1(ss_state_7[11]), .B0(
        ss_state_7[9]), .B1(ss7_n90), .Y(ss7_n63) );
  CGEN_X1M_A12TR ss7_u92 ( .A(ss7_n62), .B(ss7_n64), .CI(ss7_n63), .CO(ss7_n75) );
  INV_X0P5B_A12TR ss7_u91 ( .A(ss_state_7[4]), .Y(ss7_n22) );
  INV_X0P5B_A12TR ss7_u90 ( .A(ss_state_7[5]), .Y(ss7_n26) );
  XOR2_X0P5M_A12TR ss7_u89 ( .A(ss7_n22), .B(ss7_n26), .Y(ss7_n88) );
  AOI22_X0P5M_A12TR ss7_u88 ( .A0(ss_state_7[5]), .A1(ss_state_7[4]), .B0(
        ss7_n88), .B1(ss_state_7[3]), .Y(ss7_n86) );
  INV_X0P5B_A12TR ss7_u87 ( .A(ss_state_7[1]), .Y(ss7_n7) );
  INV_X0P5B_A12TR ss7_u86 ( .A(ss_state_7[2]), .Y(ss7_n11) );
  XOR2_X0P5M_A12TR ss7_u85 ( .A(ss7_n7), .B(ss7_n11), .Y(ss7_n89) );
  AOI22_X0P5M_A12TR ss7_u84 ( .A0(ss_state_7[2]), .A1(ss_state_7[1]), .B0(
        ss7_n89), .B1(ss_state_7[0]), .Y(ss7_n87) );
  XNOR2_X0P5M_A12TR ss7_u83 ( .A(ss7_n87), .B(ss7_n86), .Y(ss7_n73) );
  XOR2_X0P5M_A12TR ss7_u82 ( .A(ss_state_7[0]), .B(ss7_n89), .Y(ss7_n82) );
  XOR2_X0P5M_A12TR ss7_u81 ( .A(ss_state_7[3]), .B(ss7_n88), .Y(ss7_n83) );
  NAND2_X0P5A_A12TR ss7_u80 ( .A(ss7_n82), .B(ss7_n83), .Y(ss7_n74) );
  OAI22_X0P5M_A12TR ss7_u79 ( .A0(ss7_n86), .A1(ss7_n87), .B0(ss7_n73), .B1(
        ss7_n74), .Y(ss7_n60) );
  INV_X0P5B_A12TR ss7_u78 ( .A(ss7_n60), .Y(ss7_n76) );
  XOR2_X0P5M_A12TR ss7_u77 ( .A(ss7_n74), .B(ss7_n73), .Y(ss7_n77) );
  OAI21_X0P5M_A12TR ss7_u76 ( .A0(ss7_n84), .A1(ss7_n85), .B0(ss7_n64), .Y(
        ss7_n68) );
  OAI21_X0P5M_A12TR ss7_u75 ( .A0(ss7_n82), .A1(ss7_n83), .B0(ss7_n74), .Y(
        ss7_n69) );
  NOR2_X0P5A_A12TR ss7_u74 ( .A(ss7_n68), .B(ss7_n69), .Y(ss7_n70) );
  INV_X0P5B_A12TR ss7_u73 ( .A(ss_state_7[6]), .Y(ss7_n29) );
  OAI22_X0P5M_A12TR ss7_u72 ( .A0(ss7_n35), .A1(ss7_n32), .B0(ss7_n81), .B1(
        ss7_n29), .Y(ss7_n78) );
  INV_X0P5B_A12TR ss7_u71 ( .A(ss_state_7[9]), .Y(ss7_n41) );
  OAI22_X0P5M_A12TR ss7_u70 ( .A0(ss7_n80), .A1(ss7_n41), .B0(ss7_n47), .B1(
        ss7_n44), .Y(ss7_n79) );
  XNOR3_X0P5M_A12TR ss7_u69 ( .A(ss7_n64), .B(ss7_n78), .C(ss7_n79), .Y(
        ss7_n71) );
  CGENI_X1M_A12TR ss7_u68 ( .A(ss7_n77), .B(ss7_n70), .CI(ss7_n71), .CON(
        ss7_n59) );
  CGEN_X1M_A12TR ss7_u67 ( .A(ss7_n75), .B(ss7_n76), .CI(ss7_n59), .CO(ss7_n54) );
  XNOR2_X0P5M_A12TR ss7_u66 ( .A(ss7_n73), .B(ss7_n74), .Y(ss7_n72) );
  XOR3_X0P5M_A12TR ss7_u65 ( .A(ss7_n71), .B(ss7_n70), .C(ss7_n72), .Y(ss7_n65) );
  AO21_X0P5M_A12TR ss7_u64 ( .A0(ss7_n68), .A1(ss7_n69), .B0(ss7_n70), .Y(
        ss7_n67) );
  OA211_X0P5M_A12TR ss7_u63 ( .A0(max_selections[1]), .A1(ss7_n65), .B0(
        ss7_n67), .C0(max_selections[0]), .Y(ss7_n66) );
  AOI21_X0P5M_A12TR ss7_u62 ( .A0(ss7_n65), .A1(max_selections[1]), .B0(
        ss7_n66), .Y(ss7_n56) );
  CGENI_X1M_A12TR ss7_u61 ( .A(ss7_n62), .B(ss7_n63), .CI(ss7_n64), .CON(
        ss7_n61) );
  XNOR3_X0P5M_A12TR ss7_u60 ( .A(ss7_n59), .B(ss7_n60), .C(ss7_n61), .Y(
        ss7_n57) );
  AO1B2_X0P5M_A12TR ss7_u59 ( .B0(ss7_n57), .B1(ss7_n56), .A0N(
        max_selections[2]), .Y(ss7_n58) );
  OAI21_X0P5M_A12TR ss7_u58 ( .A0(ss7_n56), .A1(ss7_n57), .B0(ss7_n58), .Y(
        ss7_n55) );
  AND2_X0P5M_A12TR ss7_u57 ( .A(ss7_n54), .B(ss7_n55), .Y(ss7_n53) );
  INV_X0P5B_A12TR ss7_u56 ( .A(n4), .Y(ss7_n51) );
  OAI221_X0P5M_A12TR ss7_u55 ( .A0(max_selections[3]), .A1(ss7_n53), .B0(
        ss7_n54), .B1(ss7_n55), .C0(ss7_n51), .Y(ss7_n52) );
  INV_X0P5B_A12TR ss7_u54 ( .A(ss7_n52), .Y(ss7_n6) );
  NAND2_X0P5A_A12TR ss7_u53 ( .A(ss7_n6), .B(ss7_n47), .Y(ss7_n48) );
  AOI21_X0P5M_A12TR ss7_u52 ( .A0(button_num[0]), .A1(button_num[1]), .B0(n4), 
        .Y(ss7_n18) );
  INV_X0P5B_A12TR ss7_u51 ( .A(ss_selector[7]), .Y(ss7_n50) );
  AO21A1AI2_X0P5M_A12TR ss7_u50 ( .A0(button_num[3]), .A1(button_num[2]), .B0(
        ss7_n50), .C0(ss7_n51), .Y(ss7_n21) );
  OAI21_X0P5M_A12TR ss7_u49 ( .A0(n4), .A1(button_num[3]), .B0(ss7_n21), .Y(
        ss7_n38) );
  NOR2_X0P5A_A12TR ss7_u48 ( .A(ss7_n18), .B(ss7_n38), .Y(ss7_n49) );
  MXIT2_X0P5M_A12TR ss7_u47 ( .A(ss7_n47), .B(ss7_n48), .S0(ss7_n49), .Y(
        ss7_n103) );
  NAND2_X0P5A_A12TR ss7_u46 ( .A(ss7_n6), .B(ss7_n44), .Y(ss7_n45) );
  INV_X0P5B_A12TR ss7_u45 ( .A(button_num[0]), .Y(ss7_n39) );
  AOI21_X0P5M_A12TR ss7_u44 ( .A0(ss7_n39), .A1(button_num[1]), .B0(n4), .Y(
        ss7_n14) );
  NOR2_X0P5A_A12TR ss7_u43 ( .A(ss7_n14), .B(ss7_n38), .Y(ss7_n46) );
  MXIT2_X0P5M_A12TR ss7_u42 ( .A(ss7_n44), .B(ss7_n45), .S0(ss7_n46), .Y(
        ss7_n102) );
  NAND2_X0P5A_A12TR ss7_u41 ( .A(ss7_n6), .B(ss7_n41), .Y(ss7_n42) );
  INV_X0P5B_A12TR ss7_u40 ( .A(button_num[1]), .Y(ss7_n40) );
  AOI21_X0P5M_A12TR ss7_u39 ( .A0(ss7_n40), .A1(button_num[0]), .B0(n4), .Y(
        ss7_n10) );
  NOR2_X0P5A_A12TR ss7_u38 ( .A(ss7_n10), .B(ss7_n38), .Y(ss7_n43) );
  MXIT2_X0P5M_A12TR ss7_u37 ( .A(ss7_n41), .B(ss7_n42), .S0(ss7_n43), .Y(
        ss7_n101) );
  NAND2_X0P5A_A12TR ss7_u36 ( .A(ss7_n6), .B(ss7_n35), .Y(ss7_n36) );
  AOI21_X0P5M_A12TR ss7_u35 ( .A0(ss7_n39), .A1(ss7_n40), .B0(n4), .Y(ss7_n4)
         );
  NOR2_X0P5A_A12TR ss7_u34 ( .A(ss7_n4), .B(ss7_n38), .Y(ss7_n37) );
  MXIT2_X0P5M_A12TR ss7_u33 ( .A(ss7_n35), .B(ss7_n36), .S0(ss7_n37), .Y(
        ss7_n100) );
  NAND2_X0P5A_A12TR ss7_u32 ( .A(ss7_n6), .B(ss7_n32), .Y(ss7_n33) );
  OAI21_X0P5M_A12TR ss7_u31 ( .A0(n4), .A1(button_num[2]), .B0(ss7_n21), .Y(
        ss7_n25) );
  NOR2_X0P5A_A12TR ss7_u30 ( .A(ss7_n18), .B(ss7_n25), .Y(ss7_n34) );
  MXIT2_X0P5M_A12TR ss7_u29 ( .A(ss7_n32), .B(ss7_n33), .S0(ss7_n34), .Y(
        ss7_n99) );
  NAND2_X0P5A_A12TR ss7_u28 ( .A(ss7_n6), .B(ss7_n29), .Y(ss7_n30) );
  NOR2_X0P5A_A12TR ss7_u27 ( .A(ss7_n14), .B(ss7_n25), .Y(ss7_n31) );
  MXIT2_X0P5M_A12TR ss7_u26 ( .A(ss7_n29), .B(ss7_n30), .S0(ss7_n31), .Y(
        ss7_n98) );
  NAND2_X0P5A_A12TR ss7_u25 ( .A(ss7_n6), .B(ss7_n26), .Y(ss7_n27) );
  NOR2_X0P5A_A12TR ss7_u24 ( .A(ss7_n10), .B(ss7_n25), .Y(ss7_n28) );
  MXIT2_X0P5M_A12TR ss7_u23 ( .A(ss7_n26), .B(ss7_n27), .S0(ss7_n28), .Y(
        ss7_n97) );
  NAND2_X0P5A_A12TR ss7_u22 ( .A(ss7_n6), .B(ss7_n22), .Y(ss7_n23) );
  NOR2_X0P5A_A12TR ss7_u21 ( .A(ss7_n4), .B(ss7_n25), .Y(ss7_n24) );
  MXIT2_X0P5M_A12TR ss7_u20 ( .A(ss7_n22), .B(ss7_n23), .S0(ss7_n24), .Y(
        ss7_n96) );
  INV_X0P5B_A12TR ss7_u19 ( .A(ss_state_7[3]), .Y(ss7_n15) );
  NAND2_X0P5A_A12TR ss7_u18 ( .A(ss7_n6), .B(ss7_n15), .Y(ss7_n16) );
  INV_X0P5B_A12TR ss7_u17 ( .A(button_num[3]), .Y(ss7_n19) );
  INV_X0P5B_A12TR ss7_u16 ( .A(button_num[2]), .Y(ss7_n20) );
  AO21A1AI2_X0P5M_A12TR ss7_u15 ( .A0(ss7_n19), .A1(ss7_n20), .B0(n4), .C0(
        ss7_n21), .Y(ss7_n5) );
  NOR2_X0P5A_A12TR ss7_u14 ( .A(ss7_n18), .B(ss7_n5), .Y(ss7_n17) );
  MXIT2_X0P5M_A12TR ss7_u13 ( .A(ss7_n15), .B(ss7_n16), .S0(ss7_n17), .Y(
        ss7_n95) );
  NAND2_X0P5A_A12TR ss7_u12 ( .A(ss7_n6), .B(ss7_n11), .Y(ss7_n12) );
  NOR2_X0P5A_A12TR ss7_u11 ( .A(ss7_n14), .B(ss7_n5), .Y(ss7_n13) );
  MXIT2_X0P5M_A12TR ss7_u10 ( .A(ss7_n11), .B(ss7_n12), .S0(ss7_n13), .Y(
        ss7_n94) );
  NAND2_X0P5A_A12TR ss7_u9 ( .A(ss7_n6), .B(ss7_n7), .Y(ss7_n8) );
  NOR2_X0P5A_A12TR ss7_u8 ( .A(ss7_n10), .B(ss7_n5), .Y(ss7_n9) );
  MXIT2_X0P5M_A12TR ss7_u7 ( .A(ss7_n7), .B(ss7_n8), .S0(ss7_n9), .Y(ss7_n93)
         );
  INV_X0P5B_A12TR ss7_u6 ( .A(ss_state_7[0]), .Y(ss7_n1) );
  NAND2_X0P5A_A12TR ss7_u5 ( .A(ss7_n6), .B(ss7_n1), .Y(ss7_n2) );
  NOR2_X0P5A_A12TR ss7_u4 ( .A(ss7_n4), .B(ss7_n5), .Y(ss7_n3) );
  MXIT2_X0P5M_A12TR ss7_u3 ( .A(ss7_n1), .B(ss7_n2), .S0(ss7_n3), .Y(ss7_n92)
         );
  DFFQ_X1M_A12TR ss7_selection_state_reg_3_ ( .D(ss7_n95), .CK(osc_clk), .Q(
        ss_state_7[3]) );
  DFFQ_X1M_A12TR ss7_selection_state_reg_0_ ( .D(ss7_n92), .CK(osc_clk), .Q(
        ss_state_7[0]) );
  DFFQ_X1M_A12TR ss7_selection_state_reg_6_ ( .D(ss7_n98), .CK(osc_clk), .Q(
        ss_state_7[6]) );
  DFFQ_X1M_A12TR ss7_selection_state_reg_9_ ( .D(ss7_n101), .CK(osc_clk), .Q(
        ss_state_7[9]) );
  DFFQ_X1M_A12TR ss7_selection_state_reg_8_ ( .D(ss7_n100), .CK(osc_clk), .Q(
        ss_state_7[8]) );
  DFFQ_X1M_A12TR ss7_selection_state_reg_1_ ( .D(ss7_n93), .CK(osc_clk), .Q(
        ss_state_7[1]) );
  DFFQ_X1M_A12TR ss7_selection_state_reg_4_ ( .D(ss7_n96), .CK(osc_clk), .Q(
        ss_state_7[4]) );
  DFFQ_X1M_A12TR ss7_selection_state_reg_11_ ( .D(ss7_n103), .CK(osc_clk), .Q(
        ss_state_7[11]) );
  DFFQ_X1M_A12TR ss7_selection_state_reg_2_ ( .D(ss7_n94), .CK(osc_clk), .Q(
        ss_state_7[2]) );
  DFFQ_X1M_A12TR ss7_selection_state_reg_5_ ( .D(ss7_n97), .CK(osc_clk), .Q(
        ss_state_7[5]) );
  DFFQ_X1M_A12TR ss7_selection_state_reg_10_ ( .D(ss7_n102), .CK(osc_clk), .Q(
        ss_state_7[10]) );
  DFFQ_X1M_A12TR ss7_selection_state_reg_7_ ( .D(ss7_n99), .CK(osc_clk), .Q(
        ss_state_7[7]) );
  NOR3_X0P5A_A12TR mux_8to1_u70 ( .A(contest_num[1]), .B(contest_num[2]), .C(
        contest_num[0]), .Y(mux_8to1_n11) );
  INV_X0P5B_A12TR mux_8to1_u69 ( .A(contest_num[0]), .Y(mux_8to1_n57) );
  NOR3_X0P5A_A12TR mux_8to1_u68 ( .A(contest_num[1]), .B(contest_num[2]), .C(
        mux_8to1_n57), .Y(mux_8to1_n12) );
  AOI22_X0P5M_A12TR mux_8to1_u67 ( .A0(ss_state_0[0]), .A1(mux_8to1_n11), .B0(
        ss_state_1[0]), .B1(mux_8to1_n12), .Y(mux_8to1_n53) );
  INV_X0P5B_A12TR mux_8to1_u66 ( .A(contest_num[1]), .Y(mux_8to1_n58) );
  NOR3_X0P5A_A12TR mux_8to1_u65 ( .A(contest_num[0]), .B(contest_num[2]), .C(
        mux_8to1_n58), .Y(mux_8to1_n9) );
  NOR3_X0P5A_A12TR mux_8to1_u64 ( .A(mux_8to1_n58), .B(contest_num[2]), .C(
        mux_8to1_n57), .Y(mux_8to1_n10) );
  AOI22_X0P5M_A12TR mux_8to1_u63 ( .A0(ss_state_2[0]), .A1(mux_8to1_n9), .B0(
        ss_state_3[0]), .B1(mux_8to1_n10), .Y(mux_8to1_n54) );
  AND3_X0P5M_A12TR mux_8to1_u62 ( .A(mux_8to1_n57), .B(mux_8to1_n58), .C(
        contest_num[2]), .Y(mux_8to1_n7) );
  AND3_X0P5M_A12TR mux_8to1_u61 ( .A(contest_num[2]), .B(mux_8to1_n58), .C(
        contest_num[0]), .Y(mux_8to1_n8) );
  AOI22_X0P5M_A12TR mux_8to1_u60 ( .A0(ss_state_4[0]), .A1(mux_8to1_n7), .B0(
        ss_state_5[0]), .B1(mux_8to1_n8), .Y(mux_8to1_n55) );
  AND3_X0P5M_A12TR mux_8to1_u59 ( .A(contest_num[1]), .B(mux_8to1_n57), .C(
        contest_num[2]), .Y(mux_8to1_n5) );
  AND3_X0P5M_A12TR mux_8to1_u58 ( .A(contest_num[2]), .B(contest_num[1]), .C(
        contest_num[0]), .Y(mux_8to1_n6) );
  AOI22_X0P5M_A12TR mux_8to1_u57 ( .A0(ss_state_6[0]), .A1(mux_8to1_n5), .B0(
        ss_state_7[0]), .B1(mux_8to1_n6), .Y(mux_8to1_n56) );
  NAND4_X0P5A_A12TR mux_8to1_u56 ( .A(mux_8to1_n53), .B(mux_8to1_n54), .C(
        mux_8to1_n55), .D(mux_8to1_n56), .Y(current_contest_state[0]) );
  AOI22_X0P5M_A12TR mux_8to1_u55 ( .A0(ss_state_0[10]), .A1(mux_8to1_n11), 
        .B0(ss_state_1[10]), .B1(mux_8to1_n12), .Y(mux_8to1_n49) );
  AOI22_X0P5M_A12TR mux_8to1_u54 ( .A0(ss_state_2[10]), .A1(mux_8to1_n9), .B0(
        ss_state_3[10]), .B1(mux_8to1_n10), .Y(mux_8to1_n50) );
  AOI22_X0P5M_A12TR mux_8to1_u53 ( .A0(ss_state_4[10]), .A1(mux_8to1_n7), .B0(
        ss_state_5[10]), .B1(mux_8to1_n8), .Y(mux_8to1_n51) );
  AOI22_X0P5M_A12TR mux_8to1_u52 ( .A0(ss_state_6[10]), .A1(mux_8to1_n5), .B0(
        ss_state_7[10]), .B1(mux_8to1_n6), .Y(mux_8to1_n52) );
  NAND4_X0P5A_A12TR mux_8to1_u51 ( .A(mux_8to1_n49), .B(mux_8to1_n50), .C(
        mux_8to1_n51), .D(mux_8to1_n52), .Y(current_contest_state[10]) );
  AOI22_X0P5M_A12TR mux_8to1_u50 ( .A0(ss_state_0[11]), .A1(mux_8to1_n11), 
        .B0(ss_state_1[11]), .B1(mux_8to1_n12), .Y(mux_8to1_n45) );
  AOI22_X0P5M_A12TR mux_8to1_u49 ( .A0(ss_state_2[11]), .A1(mux_8to1_n9), .B0(
        ss_state_3[11]), .B1(mux_8to1_n10), .Y(mux_8to1_n46) );
  AOI22_X0P5M_A12TR mux_8to1_u48 ( .A0(ss_state_4[11]), .A1(mux_8to1_n7), .B0(
        ss_state_5[11]), .B1(mux_8to1_n8), .Y(mux_8to1_n47) );
  AOI22_X0P5M_A12TR mux_8to1_u47 ( .A0(ss_state_6[11]), .A1(mux_8to1_n5), .B0(
        ss_state_7[11]), .B1(mux_8to1_n6), .Y(mux_8to1_n48) );
  NAND4_X0P5A_A12TR mux_8to1_u46 ( .A(mux_8to1_n45), .B(mux_8to1_n46), .C(
        mux_8to1_n47), .D(mux_8to1_n48), .Y(current_contest_state[11]) );
  AOI22_X0P5M_A12TR mux_8to1_u45 ( .A0(ss_state_0[1]), .A1(mux_8to1_n11), .B0(
        ss_state_1[1]), .B1(mux_8to1_n12), .Y(mux_8to1_n41) );
  AOI22_X0P5M_A12TR mux_8to1_u44 ( .A0(ss_state_2[1]), .A1(mux_8to1_n9), .B0(
        ss_state_3[1]), .B1(mux_8to1_n10), .Y(mux_8to1_n42) );
  AOI22_X0P5M_A12TR mux_8to1_u43 ( .A0(ss_state_4[1]), .A1(mux_8to1_n7), .B0(
        ss_state_5[1]), .B1(mux_8to1_n8), .Y(mux_8to1_n43) );
  AOI22_X0P5M_A12TR mux_8to1_u42 ( .A0(ss_state_6[1]), .A1(mux_8to1_n5), .B0(
        ss_state_7[1]), .B1(mux_8to1_n6), .Y(mux_8to1_n44) );
  NAND4_X0P5A_A12TR mux_8to1_u41 ( .A(mux_8to1_n41), .B(mux_8to1_n42), .C(
        mux_8to1_n43), .D(mux_8to1_n44), .Y(current_contest_state[1]) );
  AOI22_X0P5M_A12TR mux_8to1_u40 ( .A0(ss_state_0[2]), .A1(mux_8to1_n11), .B0(
        ss_state_1[2]), .B1(mux_8to1_n12), .Y(mux_8to1_n37) );
  AOI22_X0P5M_A12TR mux_8to1_u39 ( .A0(ss_state_2[2]), .A1(mux_8to1_n9), .B0(
        ss_state_3[2]), .B1(mux_8to1_n10), .Y(mux_8to1_n38) );
  AOI22_X0P5M_A12TR mux_8to1_u38 ( .A0(ss_state_4[2]), .A1(mux_8to1_n7), .B0(
        ss_state_5[2]), .B1(mux_8to1_n8), .Y(mux_8to1_n39) );
  AOI22_X0P5M_A12TR mux_8to1_u37 ( .A0(ss_state_6[2]), .A1(mux_8to1_n5), .B0(
        ss_state_7[2]), .B1(mux_8to1_n6), .Y(mux_8to1_n40) );
  NAND4_X0P5A_A12TR mux_8to1_u36 ( .A(mux_8to1_n37), .B(mux_8to1_n38), .C(
        mux_8to1_n39), .D(mux_8to1_n40), .Y(current_contest_state[2]) );
  AOI22_X0P5M_A12TR mux_8to1_u35 ( .A0(ss_state_0[3]), .A1(mux_8to1_n11), .B0(
        ss_state_1[3]), .B1(mux_8to1_n12), .Y(mux_8to1_n33) );
  AOI22_X0P5M_A12TR mux_8to1_u34 ( .A0(ss_state_2[3]), .A1(mux_8to1_n9), .B0(
        ss_state_3[3]), .B1(mux_8to1_n10), .Y(mux_8to1_n34) );
  AOI22_X0P5M_A12TR mux_8to1_u33 ( .A0(ss_state_4[3]), .A1(mux_8to1_n7), .B0(
        ss_state_5[3]), .B1(mux_8to1_n8), .Y(mux_8to1_n35) );
  AOI22_X0P5M_A12TR mux_8to1_u32 ( .A0(ss_state_6[3]), .A1(mux_8to1_n5), .B0(
        ss_state_7[3]), .B1(mux_8to1_n6), .Y(mux_8to1_n36) );
  NAND4_X0P5A_A12TR mux_8to1_u31 ( .A(mux_8to1_n33), .B(mux_8to1_n34), .C(
        mux_8to1_n35), .D(mux_8to1_n36), .Y(current_contest_state[3]) );
  AOI22_X0P5M_A12TR mux_8to1_u30 ( .A0(ss_state_0[4]), .A1(mux_8to1_n11), .B0(
        ss_state_1[4]), .B1(mux_8to1_n12), .Y(mux_8to1_n29) );
  AOI22_X0P5M_A12TR mux_8to1_u29 ( .A0(ss_state_2[4]), .A1(mux_8to1_n9), .B0(
        ss_state_3[4]), .B1(mux_8to1_n10), .Y(mux_8to1_n30) );
  AOI22_X0P5M_A12TR mux_8to1_u28 ( .A0(ss_state_4[4]), .A1(mux_8to1_n7), .B0(
        ss_state_5[4]), .B1(mux_8to1_n8), .Y(mux_8to1_n31) );
  AOI22_X0P5M_A12TR mux_8to1_u27 ( .A0(ss_state_6[4]), .A1(mux_8to1_n5), .B0(
        ss_state_7[4]), .B1(mux_8to1_n6), .Y(mux_8to1_n32) );
  NAND4_X0P5A_A12TR mux_8to1_u26 ( .A(mux_8to1_n29), .B(mux_8to1_n30), .C(
        mux_8to1_n31), .D(mux_8to1_n32), .Y(current_contest_state[4]) );
  AOI22_X0P5M_A12TR mux_8to1_u25 ( .A0(ss_state_0[5]), .A1(mux_8to1_n11), .B0(
        ss_state_1[5]), .B1(mux_8to1_n12), .Y(mux_8to1_n25) );
  AOI22_X0P5M_A12TR mux_8to1_u24 ( .A0(ss_state_2[5]), .A1(mux_8to1_n9), .B0(
        ss_state_3[5]), .B1(mux_8to1_n10), .Y(mux_8to1_n26) );
  AOI22_X0P5M_A12TR mux_8to1_u23 ( .A0(ss_state_4[5]), .A1(mux_8to1_n7), .B0(
        ss_state_5[5]), .B1(mux_8to1_n8), .Y(mux_8to1_n27) );
  AOI22_X0P5M_A12TR mux_8to1_u22 ( .A0(ss_state_6[5]), .A1(mux_8to1_n5), .B0(
        ss_state_7[5]), .B1(mux_8to1_n6), .Y(mux_8to1_n28) );
  NAND4_X0P5A_A12TR mux_8to1_u21 ( .A(mux_8to1_n25), .B(mux_8to1_n26), .C(
        mux_8to1_n27), .D(mux_8to1_n28), .Y(current_contest_state[5]) );
  AOI22_X0P5M_A12TR mux_8to1_u20 ( .A0(ss_state_0[6]), .A1(mux_8to1_n11), .B0(
        ss_state_1[6]), .B1(mux_8to1_n12), .Y(mux_8to1_n21) );
  AOI22_X0P5M_A12TR mux_8to1_u19 ( .A0(ss_state_2[6]), .A1(mux_8to1_n9), .B0(
        ss_state_3[6]), .B1(mux_8to1_n10), .Y(mux_8to1_n22) );
  AOI22_X0P5M_A12TR mux_8to1_u18 ( .A0(ss_state_4[6]), .A1(mux_8to1_n7), .B0(
        ss_state_5[6]), .B1(mux_8to1_n8), .Y(mux_8to1_n23) );
  AOI22_X0P5M_A12TR mux_8to1_u17 ( .A0(ss_state_6[6]), .A1(mux_8to1_n5), .B0(
        ss_state_7[6]), .B1(mux_8to1_n6), .Y(mux_8to1_n24) );
  NAND4_X0P5A_A12TR mux_8to1_u16 ( .A(mux_8to1_n21), .B(mux_8to1_n22), .C(
        mux_8to1_n23), .D(mux_8to1_n24), .Y(current_contest_state[6]) );
  AOI22_X0P5M_A12TR mux_8to1_u15 ( .A0(ss_state_0[7]), .A1(mux_8to1_n11), .B0(
        ss_state_1[7]), .B1(mux_8to1_n12), .Y(mux_8to1_n17) );
  AOI22_X0P5M_A12TR mux_8to1_u14 ( .A0(ss_state_2[7]), .A1(mux_8to1_n9), .B0(
        ss_state_3[7]), .B1(mux_8to1_n10), .Y(mux_8to1_n18) );
  AOI22_X0P5M_A12TR mux_8to1_u13 ( .A0(ss_state_4[7]), .A1(mux_8to1_n7), .B0(
        ss_state_5[7]), .B1(mux_8to1_n8), .Y(mux_8to1_n19) );
  AOI22_X0P5M_A12TR mux_8to1_u12 ( .A0(ss_state_6[7]), .A1(mux_8to1_n5), .B0(
        ss_state_7[7]), .B1(mux_8to1_n6), .Y(mux_8to1_n20) );
  NAND4_X0P5A_A12TR mux_8to1_u11 ( .A(mux_8to1_n17), .B(mux_8to1_n18), .C(
        mux_8to1_n19), .D(mux_8to1_n20), .Y(current_contest_state[7]) );
  AOI22_X0P5M_A12TR mux_8to1_u10 ( .A0(ss_state_0[8]), .A1(mux_8to1_n11), .B0(
        ss_state_1[8]), .B1(mux_8to1_n12), .Y(mux_8to1_n13) );
  AOI22_X0P5M_A12TR mux_8to1_u9 ( .A0(ss_state_2[8]), .A1(mux_8to1_n9), .B0(
        ss_state_3[8]), .B1(mux_8to1_n10), .Y(mux_8to1_n14) );
  AOI22_X0P5M_A12TR mux_8to1_u8 ( .A0(ss_state_4[8]), .A1(mux_8to1_n7), .B0(
        ss_state_5[8]), .B1(mux_8to1_n8), .Y(mux_8to1_n15) );
  AOI22_X0P5M_A12TR mux_8to1_u7 ( .A0(ss_state_6[8]), .A1(mux_8to1_n5), .B0(
        ss_state_7[8]), .B1(mux_8to1_n6), .Y(mux_8to1_n16) );
  NAND4_X0P5A_A12TR mux_8to1_u6 ( .A(mux_8to1_n13), .B(mux_8to1_n14), .C(
        mux_8to1_n15), .D(mux_8to1_n16), .Y(current_contest_state[8]) );
  AOI22_X0P5M_A12TR mux_8to1_u5 ( .A0(ss_state_0[9]), .A1(mux_8to1_n11), .B0(
        ss_state_1[9]), .B1(mux_8to1_n12), .Y(mux_8to1_n1) );
  AOI22_X0P5M_A12TR mux_8to1_u4 ( .A0(ss_state_2[9]), .A1(mux_8to1_n9), .B0(
        ss_state_3[9]), .B1(mux_8to1_n10), .Y(mux_8to1_n2) );
  AOI22_X0P5M_A12TR mux_8to1_u3 ( .A0(ss_state_4[9]), .A1(mux_8to1_n7), .B0(
        ss_state_5[9]), .B1(mux_8to1_n8), .Y(mux_8to1_n3) );
  AOI22_X0P5M_A12TR mux_8to1_u2 ( .A0(ss_state_6[9]), .A1(mux_8to1_n5), .B0(
        ss_state_7[9]), .B1(mux_8to1_n6), .Y(mux_8to1_n4) );
  NAND4_X0P5A_A12TR mux_8to1_u1 ( .A(mux_8to1_n1), .B(mux_8to1_n2), .C(
        mux_8to1_n3), .D(mux_8to1_n4), .Y(current_contest_state[9]) );
endmodule

