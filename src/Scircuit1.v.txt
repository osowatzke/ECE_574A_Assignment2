module Scircuit1(a, b, c, z, x, clk, rst);

    input  clk, rst;
    input  signed [7:0] a, b, c;

    output signed [7:0] z;
    output signed [15:0] x;

    wire g;
    wire signed [7:0] d, e, zwire;
    wire signed [15:0] f;
    wire signed [15:0] xwire;
    wire signed [15:0] a_16, c_16, d_16;

    // Extend a, c, d to 16 bits using sign extension
    assign a_16 = {{8{a[7]}}, a};  // Replicate the sign bit of 'a' for the upper 8 bits
    assign c_16 = {{8{c[7]}}, c};  // Replicate the sign bit of 'c' for the upper 8 bits
    assign d_16 = {{8{d[7]}}, d};  // Replicate the sign bit of 'd' for the upper 8 bits

    SADD   #(.DATAWIDTH(8)) ADD_1(a, b, d);                        // d = a + b, signed addition
    SADD   #(.DATAWIDTH(8)) ADD_2(a, c, e);                        // e = a + c, signed addition

    SCOMP  #(.DATAWIDTH(8)) COMP_1(d, e, , g, );                   // g = d > e, signed comparison
    SMUX2x1#(.DATAWIDTH(8)) MUX2x1_1(e, d, g, zwire);              // z = g ? d : e, signed multiplexing

    SREG   #(.DATAWIDTH(8)) REG_1(z, zwire, clk, rst);             // z = zwire with register, signed

    SMUL   #(.DATAWIDTH(16)) MUL_1(a_16, c_16, f);                 // f = a * c, signed multiplication
    SSUB   #(.DATAWIDTH(16)) SUB_1(f, d_16, xwire);                // xwire = f - d, signed subtraction
    SREG   #(.DATAWIDTH(16)) REG_2(x, xwire, clk, rst);            // x = xwire with register, signed

endmodule
