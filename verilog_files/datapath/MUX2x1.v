`timescale 1ns/1ns

module MUX2x1(a,b,sel,d);

    parameter DATAWIDTH = 32;
    input [DATAWIDTH-1:0] a;
    input [DATAWIDTH-1:0] b;
    input sel;

    output reg [DATAWIDTH-1:0] d;

    always @(a,b,sel)
        d = sel ? b : a;
endmodule
