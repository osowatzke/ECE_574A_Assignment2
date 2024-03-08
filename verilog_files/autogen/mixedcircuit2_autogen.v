`timescale 1ns/1ns

module mixedcircuit2_autogen(a, b, c, Clk, Rst);

    input signed [7:0] a;
    input [15:0] b;
    input Clk;
    input Rst;

    output signed [31:0] c;

    wire signed [31:0] cwire;
    wire signed [31:0] cast_a;
    wire signed [31:0] cast_b;

    assign cast_a = {{24{a[7]}}, a};
    assign cast_b = $signed({{16{1'b0}}, b});

    SADD #(.DATAWIDTH(32)) SADD0(.a(cast_a), .b(cast_b), .sum(cwire));
    SREG #(.DATAWIDTH(32)) SREG0(.Clk(Clk), .Rst(Rst), .d(cwire), .q(c));

endmodule
