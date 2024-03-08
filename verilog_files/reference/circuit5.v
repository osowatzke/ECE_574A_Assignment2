`timescale 1ns / 1ns

module circuit5(a, b, c, d, zero, z, Clk, Rst);

    input  Clk, Rst;
    input  signed [63:0] a, b, c, d, zero;
    output signed [63:0] z;

    wire   signed [63:0] e, f, g, zwire;
    wire   gEQz;

    SDIV   #(.DATAWIDTH(64)) DIV_1(a, b, e);                // e = a / b, signed division
    SDIV   #(.DATAWIDTH(64)) DIV_2(c, d, f);                // f = c / d, signed division
    SMOD   #(.DATAWIDTH(64)) MOD_1(a, b, g);                // g = a % b, signed modulo
    SCOMP  #(.DATAWIDTH(64)) COMP_1(g, zero, , ,gEQz);      // gEQz = g == zero, signed comparison
    SMUX2x1#(.DATAWIDTH(64)) MUX2x1_1(f, e, gEQz, zwire);   // zwire = gEQz ? e : f, signed multiplexing
    SREG   #(.DATAWIDTH(64)) REG_1(z, zwire, Clk, Rst);     // z = zwire, signed register

endmodule
