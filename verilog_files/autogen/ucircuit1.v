`timescale 1ns/1ns

module ucircuit1(a, b, Clk, Rst);

    input [7:0] a;
    input [7:0] b;
    input Clk;
    input Rst;

    output [7:0] c;

    wire [7:0] cwire;

    ADD #(.DATAWIDTH(8)) ADD0(.a(a), .b(b), .sum(cwire));
    REG #(.DATAWIDTH(8)) REG0(.Clk(Clk), .Rst(Rst), .d(cwire), .q(c));

endmodule
