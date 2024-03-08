`timescale 1ns/1ns

module circuit2(a, b, c, Clk, Rst);

    input signed [31:0] a;
    input signed [31:0] b;
    input signed [31:0] c;
    input Clk;
    input Rst;

    output signed [31:0] z;
    output signed [31:0] x;

    wire signed [31:0] d;
    wire signed [31:0] e;
    wire signed [31:0] f;
    wire signed [31:0] g;
    wire signed [31:0] h;
    wire dLTe;
    wire dEQe;
    wire signed [31:0] zwire;
    wire signed [31:0] xwire;
    wire [31:0] cast_dLTe;
    wire [31:0] cast_dEQe;

    assign cast_dLTe = {{31{1'b0}}, dLTe};
    assign cast_dEQe = {{31{1'b0}}, dEQe};

    SADD #(.DATAWIDTH(32)) SADD0(.a(a), .b(b), .sum(d));
    SADD #(.DATAWIDTH(32)) SADD1(.a(a), .b(c), .sum(e));
    SSUB #(.DATAWIDTH(32)) SSUB0(.a(a), .b(b), .diff(f));
    SCOMP #(.DATAWIDTH(32)) SCOMP0(.a(d), .b(e), .eq(dEQe));
    SCOMP #(.DATAWIDTH(32)) SCOMP1(.a(d), .b(e), .lt(dLTe));
    SMUX2x1 #(.DATAWIDTH(32)) SMUX2x10(.a(e), .b(d), .sel(dLTe), .d(g));
    SMUX2x1 #(.DATAWIDTH(32)) SMUX2x11(.a(f), .b(g), .sel(dEQe), .d(h));
    SSHL #(.DATAWIDTH(32)) SSHL0(.a(g), .sh_amt(cast_dLTe), .d(xwire));
    SSHR #(.DATAWIDTH(32)) SSHR0(.a(h), .sh_amt(cast_dEQe), .d(zwire));
    SREG #(.DATAWIDTH(32)) SREG0(.Clk(Clk), .Rst(Rst), .d(xwire), .q(x));
    SREG #(.DATAWIDTH(32)) SREG1(.Clk(Clk), .Rst(Rst), .d(zwire), .q(z));

endmodule
