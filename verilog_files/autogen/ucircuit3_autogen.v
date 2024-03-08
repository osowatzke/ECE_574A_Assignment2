`timescale 1ns/1ns

module ucircuit3_autogen(a, b, c, Clk, Rst);

    input [31:0] a;
    input [15:0] b;
    input Clk;
    input Rst;

    output [7:0] c;

    wire [7:0] cwire;
    wire [7:0] cast_a;
    wire [7:0] cast_b;

    assign cast_a = a[7:0];
    assign cast_b = b[7:0];

    ADD #(.DATAWIDTH(8)) ADD0(.a(cast_a), .b(cast_b), .sum(cwire));
    REG #(.DATAWIDTH(8)) REG0(.Clk(Clk), .Rst(Rst), .d(cwire), .q(c));

endmodule
