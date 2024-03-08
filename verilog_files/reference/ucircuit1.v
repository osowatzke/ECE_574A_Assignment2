`timescale 1ns / 1ns

module ucircuit1(a, b, c, Clk, Rst);

    input Clk, Rst;
    input  [ 7:0] a, b;

    output [ 7:0] c;

    wire   [ 7:0] cwire;

    ADD    #(.DATAWIDTH( 8)) ADD_1(a, b, cwire);                // cwire = a + b
    REG    #(.DATAWIDTH( 8)) REG_1(c, cwire, Clk, Rst);         // c = cwire

endmodule
