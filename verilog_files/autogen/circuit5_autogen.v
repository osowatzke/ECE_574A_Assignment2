`timescale 1ns/1ns

module circuit5_autogen(a, b, c, d, zero, z, Clk, Rst);

    input signed [63:0] a;
    input signed [63:0] b;
    input signed [63:0] c;
    input signed [63:0] d;
    input signed [63:0] zero;
    input Clk;
    input Rst;

    output signed [63:0] z;

    wire signed [63:0] e;
    wire signed [63:0] f;
    wire signed [63:0] g;
    wire signed [63:0] zwire;
    wire gEQz;

    SDIV #(.DATAWIDTH(64)) SDIV0(.a(a), .b(b), .qout(e));
    SDIV #(.DATAWIDTH(64)) SDIV1(.a(c), .b(d), .qout(f));
    SMOD #(.DATAWIDTH(64)) SMOD0(.a(a), .b(b), .rem(g));
    SCOMP #(.DATAWIDTH(64)) SCOMP0(.a(g), .b(zero), .eq(gEQz));
    SMUX2x1 #(.DATAWIDTH(64)) SMUX2x10(.a(f), .b(e), .sel(gEQz), .d(zwire));
    SREG #(.DATAWIDTH(64)) SREG0(.Clk(Clk), .Rst(Rst), .d(zwire), .q(z));

endmodule
