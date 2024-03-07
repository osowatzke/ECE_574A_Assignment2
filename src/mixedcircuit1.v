`timescale 1ns/1ns

module mixedcircuit1(a, b, c, Clk, Rst);

    input Clk, Rst;
    input signed [7:0] a;
    input [7:0] b;
    
    output signed [7:0] c;
    
    wire signed [7:0] b_signed;
    wire signed [7:0] cwire;
    
    assign b_signed = $signed(b);
    
    SADD #(.DATAWIDTH(8)) SADD_1(a, b_signed, cwire);
    SREG #(.DATAWIDTH(8)) SREG_1(c, cwire, Clk, Rst);
    
endmodule
