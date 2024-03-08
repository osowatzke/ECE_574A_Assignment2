`timescale 1ns/1ns

module mixedcircuit1(a, b, Clk, Rst);

    input signed [7:0] a;
    input [7:0] b;
    input Clk;
    input Rst;

    output signed [7:0] c;

    wire signed [7:0] cwire;
    wire signed [7:0] cast_b;

    assign cast_b = $signed(b);

    SADD #(.DATAWIDTH(8)) SADD0(.a(a), .b(cast_b), .sum(cwire));
    SREG #(.DATAWIDTH(8)) SREG0(.Clk(Clk), .Rst(Rst), .d(cwire), .q(c));

endmodule
