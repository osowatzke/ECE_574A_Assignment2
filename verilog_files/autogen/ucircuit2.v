`timescale 1ns/1ns

module ucircuit2(a, b, Clk, Rst);

    input [7:0] a;
    input [15:0] b;
    input Clk;
    input Rst;

    output [31:0] c;

    wire [31:0] cwire;
    wire [31:0] cast_a;
    wire [31:0] cast_b;

    assign cast_a = {{24{1'b0}}, a};
    assign cast_b = {{16{1'b0}}, b};

    ADD #(.DATAWIDTH(32)) ADD0(.a(cast_a), .b(cast_b), .sum(cwire));
    REG #(.DATAWIDTH(32)) REG0(.Clk(Clk), .Rst(Rst), .d(cwire), .q(c));

endmodule
