`timescale 1ns / 1ns

module circuit3(a, b, c, d, e, f, g, h, sa, avg, Clk, Rst);
    
    input Clk, Rst;
    input [7:0] sa;
    input signed [15:0] a, b, c, d, e, f, g, h;

    output signed [15:0] avg;

    wire        [31:0] sa_32;
    wire signed [31:0] l00, l01, l02, l03, l10, l11, l2, l2div2, l2div4, l2div8;
    wire signed [31:0] a_32, b_32, c_32, d_32, e_32, f_32, g_32, h_32;
    wire signed [15:0] l2div8_16;

    // Explicitly using zero-extension here as per original structure
    assign a_32 = {{16{a[15]}}, a};
    assign b_32 = {{16{b[15]}}, b};
    assign c_32 = {{16{c[15]}}, c};
    assign d_32 = {{16{d[15]}}, d};
    assign e_32 = {{16{e[15]}}, e};
    assign f_32 = {{16{f[15]}}, f};
    assign g_32 = {{16{g[15]}}, g};
    assign h_32 = {{16{h[15]}}, h};    
    assign sa_32 = {{24{1'b0}}, sa};
    assign l2div8_16 = l2div8[15:0];

    // Signed additions for input pairs, extended results to 32 bits
    SADD   #(.DATAWIDTH(32)) ADD_1(a_32, b_32, l00);          // l00 = a + b
    SADD   #(.DATAWIDTH(32)) ADD_2(c_32, d_32, l01);          // l01 = c + d
    SADD   #(.DATAWIDTH(32)) ADD_3(e_32, f_32, l02);          // l02 = e + f
    SADD   #(.DATAWIDTH(32)) ADD_4(g_32, h_32, l03);          // l03 = g + h

    // Adding zero-extended results for intermediate sums
    SADD   #(.DATAWIDTH(32)) ADD_5(l00, l01, l10);            // l10 = l00 + l01
    SADD   #(.DATAWIDTH(32)) ADD_6(l02, l03, l11);            // l11 = l02 + l03
    SADD   #(.DATAWIDTH(32)) ADD_7(l10, l11, l2);             // l2 = l10 + l11

    // Right shifts for average calculation, assuming logical shifts
    SSHR   #(.DATAWIDTH(32)) SHR_1(l2, sa_32, l2div2);        // l2div2 = l2 >> sa
    SSHR   #(.DATAWIDTH(32)) SHR_2(l2div2, sa_32, l2div4);    // l2div4 = l2div2 >> sa
    SSHR   #(.DATAWIDTH(32)) SHR_3(l2div4, sa_32, l2div8);    // l2div8 = l2div4 >> sa

    // Registering the final output
    SREG   #(.DATAWIDTH(16)) REG_1(avg, l2div8_16, Clk, Rst); // avg = l2div8

endmodule
