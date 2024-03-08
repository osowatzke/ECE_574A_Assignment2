`timescale 1ns/1ns

module mixedcircuit3_autogen(a, b, c, Clk, Rst);

    input signed [31:0] a;
    input [15:0] b;
    input Clk;
    input Rst;

    output signed [7:0] c;

    wire signed [7:0] cwire;
    wire signed [7:0] cast_a;
    wire signed [7:0] cast_b;

    assign cast_a = a[7:0];
    assign cast_b = $signed(b[7:0]);

    SADD #(.DATAWIDTH(8)) SADD0(.a(cast_a), .b(cast_b), .sum(cwire));
    SREG #(.DATAWIDTH(8)) SREG0(.Clk(Clk), .Rst(Rst), .d(cwire), .q(c));

endmodule
