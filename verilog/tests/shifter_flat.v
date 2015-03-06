
module shifter ( ina, shift, out );
  input [7:0] ina;
  input [2:0] shift;
  output [7:0] out;
  wire   n21, n22, n23, n24, n25, n26, n27, n28, n29, n30, n31, n32, n33, n34,
         n35, n36, n37, n38, n39;

  MXIT2_X0P5M_A12TS u30 ( .A(n21), .B(n22), .S0(shift[0]), .Y(out[7]) );
  AOI221_X0P5M_A12TS u31 ( .A0(ina[5]), .A1(n23), .B0(ina[7]), .B1(n24), .C0(
        n25), .Y(n21) );
  AO1B2_X0P5M_A12TS u32 ( .B0(n26), .B1(ina[3]), .A0N(n27), .Y(n25) );
  NAND3_X0P5A_A12TS u33 ( .A(shift[1]), .B(ina[1]), .C(shift[2]), .Y(n27) );
  MXIT2_X0P5M_A12TS u34 ( .A(n22), .B(n28), .S0(shift[0]), .Y(out[6]) );
  AND2_X0P5M_A12TS u35 ( .A(n29), .B(n30), .Y(n22) );
  AOI32_X0P5M_A12TS u36 ( .A0(shift[2]), .A1(shift[1]), .A2(ina[0]), .B0(ina[2]), .B1(n26), .Y(n30) );
  AOI22_X0P5M_A12TS u37 ( .A0(ina[4]), .A1(n23), .B0(ina[6]), .B1(n24), .Y(n29)
         );
  MXIT2_X0P5M_A12TS u38 ( .A(n28), .B(n31), .S0(shift[0]), .Y(out[5]) );
  AOI222_X0P5M_A12TS u39 ( .A0(ina[3]), .A1(n23), .B0(ina[1]), .B1(n26), .C0(
        ina[5]), .C1(n24), .Y(n28) );
  MXIT2_X0P5M_A12TS u40 ( .A(n31), .B(n32), .S0(shift[0]), .Y(out[4]) );
  AOI222_X0P5M_A12TS u41 ( .A0(n23), .A1(ina[2]), .B0(n26), .B1(ina[0]), .C0(
        n24), .C1(ina[4]), .Y(n31) );
  NOR2B_X0P5M_A12TS u42 ( .AN(shift[2]), .B(shift[1]), .Y(n26) );
  NOR2B_X0P5M_A12TS u43 ( .AN(shift[1]), .B(shift[2]), .Y(n23) );
  MXIT2_X0P5M_A12TS u44 ( .A(n32), .B(n33), .S0(shift[0]), .Y(out[3]) );
  OR2_X0P5M_A12TS u45 ( .A(n34), .B(shift[2]), .Y(n32) );
  MXIT2_X0P5M_A12TS u46 ( .A(ina[3]), .B(ina[1]), .S0(shift[1]), .Y(n34) );
  MXIT2_X0P5M_A12TS u47 ( .A(n33), .B(n35), .S0(shift[0]), .Y(out[2]) );
  NAND2_X0P5A_A12TS u48 ( .A(n24), .B(ina[1]), .Y(n35) );
  OR2_X0P5M_A12TS u49 ( .A(n36), .B(shift[2]), .Y(n33) );
  MXIT2_X0P5M_A12TS u50 ( .A(ina[2]), .B(ina[0]), .S0(shift[1]), .Y(n36) );
  NOR2_X0P5A_A12TS u51 ( .A(n37), .B(n38), .Y(out[1]) );
  MXIT2_X0P5M_A12TS u52 ( .A(ina[1]), .B(ina[0]), .S0(shift[0]), .Y(n38) );
  NOR3_X0P5A_A12TS u53 ( .A(n39), .B(shift[0]), .C(n37), .Y(out[0]) );
  INV_X0P5B_A12TS u54 ( .A(n24), .Y(n37) );
  NOR2_X0P5A_A12TS u55 ( .A(shift[1]), .B(shift[2]), .Y(n24) );
  INV_X0P5B_A12TS u56 ( .A(ina[0]), .Y(n39) );
endmodule

