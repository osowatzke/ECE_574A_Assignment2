`timescale 1ns/1ns

module mixedcircuit2(a, b, c, Clk, Rst);

    input Clk, Rst;
    input signed [7:0] a;
    input [15:0] b;
    
    output signed [31:0] c;
    
    wire signed [31:0] cwire;
    wire signed [31:0] a_32;
    wire signed [31:0] b_signed_32;
    
    assign a_32 = {{24{a[7]}}, a};
    assign b_signed_32 = $signed({{16{1'b0}}, b});
    
    SADD #(.DATAWIDTH(32)) SADD_1(a_32, b_signed_32, cwire);
    SREG #(.DATAWIDTH(32)) SREG_1(c, cwire, Clk, Rst);
    
endmodule
