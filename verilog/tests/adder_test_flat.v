
module adder_test ( in_a, in_b, in_c, in_d, sum_out );
  input [7:0] in_a;
  input [7:0] in_b;
  input [7:0] in_c;
  input [7:0] in_d;
  output [9:0] sum_out;
  wire   sum_two_8_, sum_two_7_, sum_two_6_, sum_two_5_, sum_two_4_,
         sum_two_3_, sum_two_2_, sum_two_1_, sum_two_0_, sum_one_8_,
         sum_one_7_, sum_one_6_, sum_one_5_, sum_one_4_, sum_one_3_,
         sum_one_2_, sum_one_1_, sum_one_0_, add_0_root_add_21_n1,
         add_2_root_add_21_n1, add_1_root_add_21_n1;
  wire   [8:2] add_0_root_add_21_carry;
  wire   [7:2] add_2_root_add_21_carry;
  wire   [7:2] add_1_root_add_21_carry;

  AND2_X1M_A12TR add_0_root_add_21_u2 ( .A(sum_one_0_), .B(sum_two_0_), .Y(
        add_0_root_add_21_n1) );
  XOR2_X1M_A12TR add_0_root_add_21_u1 ( .A(sum_one_0_), .B(sum_two_0_), .Y(
        sum_out[0]) );
  ADDF_X1M_A12TR add_0_root_add_21_u1_1 ( .A(sum_two_1_), .B(sum_one_1_), .CI(
        add_0_root_add_21_n1), .CO(add_0_root_add_21_carry[2]), .S(sum_out[1])
         );
  ADDF_X1M_A12TR add_0_root_add_21_u1_2 ( .A(sum_two_2_), .B(sum_one_2_), .CI(
        add_0_root_add_21_carry[2]), .CO(add_0_root_add_21_carry[3]), .S(
        sum_out[2]) );
  ADDF_X1M_A12TR add_0_root_add_21_u1_3 ( .A(sum_two_3_), .B(sum_one_3_), .CI(
        add_0_root_add_21_carry[3]), .CO(add_0_root_add_21_carry[4]), .S(
        sum_out[3]) );
  ADDF_X1M_A12TR add_0_root_add_21_u1_4 ( .A(sum_two_4_), .B(sum_one_4_), .CI(
        add_0_root_add_21_carry[4]), .CO(add_0_root_add_21_carry[5]), .S(
        sum_out[4]) );
  ADDF_X1M_A12TR add_0_root_add_21_u1_5 ( .A(sum_two_5_), .B(sum_one_5_), .CI(
        add_0_root_add_21_carry[5]), .CO(add_0_root_add_21_carry[6]), .S(
        sum_out[5]) );
  ADDF_X1M_A12TR add_0_root_add_21_u1_6 ( .A(sum_two_6_), .B(sum_one_6_), .CI(
        add_0_root_add_21_carry[6]), .CO(add_0_root_add_21_carry[7]), .S(
        sum_out[6]) );
  ADDF_X1M_A12TR add_0_root_add_21_u1_7 ( .A(sum_two_7_), .B(sum_one_7_), .CI(
        add_0_root_add_21_carry[7]), .CO(add_0_root_add_21_carry[8]), .S(
        sum_out[7]) );
  ADDF_X1M_A12TR add_0_root_add_21_u1_8 ( .A(sum_two_8_), .B(sum_one_8_), .CI(
        add_0_root_add_21_carry[8]), .CO(sum_out[9]), .S(sum_out[8]) );
  AND2_X1M_A12TR add_2_root_add_21_u2 ( .A(in_b[0]), .B(in_a[0]), .Y(
        add_2_root_add_21_n1) );
  XOR2_X1M_A12TR add_2_root_add_21_u1 ( .A(in_b[0]), .B(in_a[0]), .Y(
        sum_two_0_) );
  ADDF_X1M_A12TR add_2_root_add_21_u1_1 ( .A(in_a[1]), .B(in_b[1]), .CI(
        add_2_root_add_21_n1), .CO(add_2_root_add_21_carry[2]), .S(sum_two_1_)
         );
  ADDF_X1M_A12TR add_2_root_add_21_u1_2 ( .A(in_a[2]), .B(in_b[2]), .CI(
        add_2_root_add_21_carry[2]), .CO(add_2_root_add_21_carry[3]), .S(
        sum_two_2_) );
  ADDF_X1M_A12TR add_2_root_add_21_u1_3 ( .A(in_a[3]), .B(in_b[3]), .CI(
        add_2_root_add_21_carry[3]), .CO(add_2_root_add_21_carry[4]), .S(
        sum_two_3_) );
  ADDF_X1M_A12TR add_2_root_add_21_u1_4 ( .A(in_a[4]), .B(in_b[4]), .CI(
        add_2_root_add_21_carry[4]), .CO(add_2_root_add_21_carry[5]), .S(
        sum_two_4_) );
  ADDF_X1M_A12TR add_2_root_add_21_u1_5 ( .A(in_a[5]), .B(in_b[5]), .CI(
        add_2_root_add_21_carry[5]), .CO(add_2_root_add_21_carry[6]), .S(
        sum_two_5_) );
  ADDF_X1M_A12TR add_2_root_add_21_u1_6 ( .A(in_a[6]), .B(in_b[6]), .CI(
        add_2_root_add_21_carry[6]), .CO(add_2_root_add_21_carry[7]), .S(
        sum_two_6_) );
  ADDF_X1M_A12TR add_2_root_add_21_u1_7 ( .A(in_a[7]), .B(in_b[7]), .CI(
        add_2_root_add_21_carry[7]), .CO(sum_two_8_), .S(sum_two_7_) );
  AND2_X1M_A12TR add_1_root_add_21_u2 ( .A(in_d[0]), .B(in_c[0]), .Y(
        add_1_root_add_21_n1) );
  XOR2_X1M_A12TR add_1_root_add_21_u1 ( .A(in_d[0]), .B(in_c[0]), .Y(
        sum_one_0_) );
  ADDF_X1M_A12TR add_1_root_add_21_u1_1 ( .A(in_c[1]), .B(in_d[1]), .CI(
        add_1_root_add_21_n1), .CO(add_1_root_add_21_carry[2]), .S(sum_one_1_)
         );
  ADDF_X1M_A12TR add_1_root_add_21_u1_2 ( .A(in_c[2]), .B(in_d[2]), .CI(
        add_1_root_add_21_carry[2]), .CO(add_1_root_add_21_carry[3]), .S(
        sum_one_2_) );
  ADDF_X1M_A12TR add_1_root_add_21_u1_3 ( .A(in_c[3]), .B(in_d[3]), .CI(
        add_1_root_add_21_carry[3]), .CO(add_1_root_add_21_carry[4]), .S(
        sum_one_3_) );
  ADDF_X1M_A12TR add_1_root_add_21_u1_4 ( .A(in_c[4]), .B(in_d[4]), .CI(
        add_1_root_add_21_carry[4]), .CO(add_1_root_add_21_carry[5]), .S(
        sum_one_4_) );
  ADDF_X1M_A12TR add_1_root_add_21_u1_5 ( .A(in_c[5]), .B(in_d[5]), .CI(
        add_1_root_add_21_carry[5]), .CO(add_1_root_add_21_carry[6]), .S(
        sum_one_5_) );
  ADDF_X1M_A12TR add_1_root_add_21_u1_6 ( .A(in_c[6]), .B(in_d[6]), .CI(
        add_1_root_add_21_carry[6]), .CO(add_1_root_add_21_carry[7]), .S(
        sum_one_6_) );
  ADDF_X1M_A12TR add_1_root_add_21_u1_7 ( .A(in_c[7]), .B(in_d[7]), .CI(
        add_1_root_add_21_carry[7]), .CO(sum_one_8_), .S(sum_one_7_) );
endmodule

