module moore ( innsig, insig, outsig, test);
input insig, innsig;
output outsig, test;
wire n5;
XNOR2_X0P5M_A12TR U1 ( .A(insig), .B(innsig), .Y(n5) );
NAND2_X0P5A_A12TR U2 ( .A(n5), .B(insig), .Y(outsig) );
NAND2_X0P5A_A12TR U3 ( .A(innsig), .B(n5), .Y(test) );

endmodule

