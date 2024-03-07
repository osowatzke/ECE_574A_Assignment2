`timescale 1ns/1ns

module SMUX2x1(a, b, sel, d);

    parameter DATAWIDTH = 32;
    input signed [DATAWIDTH-1:0] a;
    input signed [DATAWIDTH-1:0] b;
    input sel;

    output reg signed [DATAWIDTH-1:0] d;

    always @(a, b, sel)
        d = sel ? b : a;
endmodule
