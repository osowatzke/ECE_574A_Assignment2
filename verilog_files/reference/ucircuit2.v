`timescale 1ns / 1ns

module ucircuit2(a, b, c, Clk, Rst);

    input Clk, Rst;
    input  [ 7:0] a;
    input  [15:0] b;

    output [31:0] c;

    wire   [31:0] cwire;
    wire   [31:0] a_32, b_32;

    assign a_32 = {{24{1'b0}}, a};
    assign b_32 = {{16{1'b0}}, b};

    ADD    #(.DATAWIDTH( 32)) ADD_1(a_32, b_32, cwire);                // cwire = a + b
    REG    #(.DATAWIDTH( 32)) REG_1(c, cwire, Clk, Rst);               // c = cwire

endmodule
