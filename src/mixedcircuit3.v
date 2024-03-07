`timescale 1ns/1ns

module mixedcircuit3(a, b, c, Clk, Rst);

    input Clk, Rst;
    input signed [31:0] a;
    input [15:0] b;
    
    output signed [7:0] c;
    
    wire signed [7:0] cwire;
    wire signed [7:0] a_8;
    wire signed [7:0] b_signed_8;
    
    assign a_8 = a[7:0];
    assign b_signed_8 = $signed(b[7:0]);
    
    SADD #(.DATAWIDTH(8)) SADD_1(a_8, b_signed_8, cwire);
    SREG #(.DATAWIDTH(8)) SREG_1(c, cwire, Clk, Rst);
    
endmodule
