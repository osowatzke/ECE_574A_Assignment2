`timescale 1ns/1ns

module circuit1(a, b, c, Clk, Rst);

    input signed [7:0] a;
    input signed [7:0] b;
    input signed [7:0] c;
    input Clk;
    input Rst;

    output signed [7:0] z;
    output signed [15:0] x;

    wire signed [7:0] d;
    wire signed [7:0] e;
    wire signed [15:0] f;
    wire g;
    wire signed [15:0] xwire;
    wire signed [15:0] cast_a;
    wire signed [15:0] cast_c;
    wire signed [15:0] cast_d;
    wire signed [7:0] reg_z;

    assign cast_a = {{8{a[7]}}, a};
    assign cast_c = {{8{c[7]}}, c};
    assign cast_d = {{8{d[7]}}, d};

    SADD #(.DATAWIDTH(8)) SADD0(.a(a), .b(b), .sum(d));
    SADD #(.DATAWIDTH(8)) SADD1(.a(a), .b(c), .sum(e));
    SCOMP #(.DATAWIDTH(8)) SCOMP0(.a(d), .b(e), .gt(g));
    SMUX2x1 #(.DATAWIDTH(8)) SMUX2x10(.a(e), .b(d), .sel(g), .d(z));
    SMUL #(.DATAWIDTH(16)) SMUL0(.a(cast_a), .b(cast_c), .prod(f));
    SSUB #(.DATAWIDTH(16)) SSUB0(.a(f), .b(cast_d), .diff(xwire));
    SREG #(.DATAWIDTH(16)) SREG0(.Clk(Clk), .Rst(Rst), .d(xwire), .q(x));
    SREG #(.DATAWIDTH(8)) SREG1(.Clk(Clk), .Rst(Rst), .d(z), .q(reg_z));

endmodule
