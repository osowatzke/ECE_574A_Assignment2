`timescale 1ns/1ns

module circuit3(a, b, c, d, e, f, g, h, sa, Clk, Rst);

    input signed [15:0] a;
    input signed [15:0] b;
    input signed [15:0] c;
    input signed [15:0] d;
    input signed [15:0] e;
    input signed [15:0] f;
    input signed [15:0] g;
    input signed [15:0] h;
    input signed [7:0] sa;
    input Clk;
    input Rst;

    output signed [15:0] avg;

    wire signed [31:0] l00;
    wire signed [31:0] l01;
    wire signed [31:0] l02;
    wire signed [31:0] l03;
    wire signed [31:0] l10;
    wire signed [31:0] l11;
    wire signed [31:0] l2;
    wire signed [31:0] l2div2;
    wire signed [31:0] l2div4;
    wire signed [31:0] l2div8;
    wire signed [31:0] cast_a;
    wire signed [31:0] cast_b;
    wire signed [31:0] cast_c;
    wire signed [31:0] cast_d;
    wire signed [31:0] cast_e;
    wire signed [31:0] cast_f;
    wire signed [31:0] cast_g;
    wire signed [31:0] cast_h;
    wire [31:0] cast_sa;
    wire signed [15:0] cast_l2div8;

    assign cast_a = {{16{a[15]}}, a};
    assign cast_b = {{16{b[15]}}, b};
    assign cast_c = {{16{c[15]}}, c};
    assign cast_d = {{16{d[15]}}, d};
    assign cast_e = {{16{e[15]}}, e};
    assign cast_f = {{16{f[15]}}, f};
    assign cast_g = {{16{g[15]}}, g};
    assign cast_h = {{16{h[15]}}, h};
    assign cast_sa = $unsigned({{24{sa[7]}}, sa});
    assign cast_l2div8 = l2div8[15:0];

    SADD #(.DATAWIDTH(32)) SADD0(.a(cast_a), .b(cast_b), .sum(l00));
    SADD #(.DATAWIDTH(32)) SADD1(.a(cast_c), .b(cast_d), .sum(l01));
    SADD #(.DATAWIDTH(32)) SADD2(.a(cast_e), .b(cast_f), .sum(l02));
    SADD #(.DATAWIDTH(32)) SADD3(.a(cast_g), .b(cast_h), .sum(l03));
    SADD #(.DATAWIDTH(32)) SADD4(.a(l00), .b(l01), .sum(l10));
    SADD #(.DATAWIDTH(32)) SADD5(.a(l02), .b(l03), .sum(l11));
    SADD #(.DATAWIDTH(32)) SADD6(.a(l10), .b(l11), .sum(l2));
    SSHR #(.DATAWIDTH(32)) SSHR0(.a(l2), .sh_amt(cast_sa), .d(l2div2));
    SSHR #(.DATAWIDTH(32)) SSHR1(.a(l2div2), .sh_amt(cast_sa), .d(l2div4));
    SSHR #(.DATAWIDTH(32)) SSHR2(.a(l2div4), .sh_amt(cast_sa), .d(l2div8));
    SREG #(.DATAWIDTH(16)) SREG0(.Clk(Clk), .Rst(Rst), .d(cast_l2div8), .q(avg));

endmodule
