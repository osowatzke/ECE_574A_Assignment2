`timescale 1ns/1ns

module circuit4_autogen(a, b, c, z, x, Clk, Rst);

    input signed [63:0] a;
    input signed [63:0] b;
    input signed [63:0] c;
    input Clk;
    input Rst;

    output signed [31:0] z;
    output signed [31:0] x;

    wire signed [63:0] d;
    wire signed [63:0] e;
    wire signed [63:0] f;
    wire signed [63:0] g;
    wire signed [63:0] h;
    wire dLTe;
    wire dEQe;
    wire signed [63:0] xrin;
    wire signed [63:0] zrin;
    wire signed [63:0] greg;
    wire signed [63:0] hreg;
    wire [63:0] cast_dLTe;
    wire [63:0] cast_dEQe;
    wire signed [31:0] cast_xrin;
    wire signed [31:0] cast_zrin;

    assign cast_dLTe = {{63{1'b0}}, dLTe};
    assign cast_dEQe = {{63{1'b0}}, dEQe};
    assign cast_xrin = xrin[31:0];
    assign cast_zrin = zrin[31:0];

    SADD #(.DATAWIDTH(64)) SADD0(.a(a), .b(b), .sum(d));
    SADD #(.DATAWIDTH(64)) SADD1(.a(a), .b(c), .sum(e));
    SSUB #(.DATAWIDTH(64)) SSUB0(.a(a), .b(b), .diff(f));
    SCOMP #(.DATAWIDTH(64)) SCOMP0(.a(d), .b(e), .eq(dEQe));
    SCOMP #(.DATAWIDTH(64)) SCOMP1(.a(d), .b(e), .lt(dLTe));
    SMUX2x1 #(.DATAWIDTH(64)) SMUX2x10(.a(e), .b(d), .sel(dLTe), .d(g));
    SMUX2x1 #(.DATAWIDTH(64)) SMUX2x11(.a(f), .b(g), .sel(dEQe), .d(h));
    SREG #(.DATAWIDTH(64)) SREG0(.Clk(Clk), .Rst(Rst), .d(g), .q(greg));
    SREG #(.DATAWIDTH(64)) SREG1(.Clk(Clk), .Rst(Rst), .d(h), .q(hreg));
    SSHL #(.DATAWIDTH(64)) SSHL0(.a(hreg), .sh_amt(cast_dLTe), .d(xrin));
    SSHR #(.DATAWIDTH(64)) SSHR0(.a(greg), .sh_amt(cast_dEQe), .d(zrin));
    SREG #(.DATAWIDTH(32)) SREG2(.Clk(Clk), .Rst(Rst), .d(cast_xrin), .q(x));
    SREG #(.DATAWIDTH(32)) SREG3(.Clk(Clk), .Rst(Rst), .d(cast_zrin), .q(z));

endmodule
