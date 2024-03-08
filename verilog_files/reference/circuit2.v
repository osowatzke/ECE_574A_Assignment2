`timescale 1ns / 1ns

module circuit2(a, b, c, z, x, Clk, Rst);

    input  Clk, Rst;
    input  signed [31:0] a, b, c;
    output signed [31:0] z, x;
    wire   signed [31:0] d, e, f, g, h, xwire, zwire;
    wire   [31:0] dLTe_32, dEQe_32;
    wire   dLTe, dEQe;

    // Zero-extend the comparison results to 32-bit for shifting
    assign dLTe_32 = {{31{1'b0}}, dLTe};
    assign dEQe_32 = {{31{1'b0}}, dEQe};

    SADD   #(.DATAWIDTH(32)) ADD_1(a, b, d);                // d = a + b, signed addition
    SADD   #(.DATAWIDTH(32)) ADD_2(a, c, e);                // e = a + c, signed addition
    SSUB   #(.DATAWIDTH(32)) SUB_1(a, b, f);                // f = a - b, signed subtraction
    
    SCOMP  #(.DATAWIDTH(32)) COMP_1(d, e, dLTe, , dEQe);    // dLTe = d < e, dEQe = d == e, signed comparison
    SMUX2x1#(.DATAWIDTH(32)) MUX2x1_1(e, d, dLTe, g);       // g = dLTe ? d : e, signed multiplexing
    SMUX2x1#(.DATAWIDTH(32)) MUX2x1_2(f, g, dEQe, h);       // h = dEQe ? g : f, signed multiplexing
    SSHL   #(.DATAWIDTH(32)) SHL_1(g, dLTe_32, xwire);      // xwire = g << dLTe, signed shift left
    SSHR   #(.DATAWIDTH(32)) SHR_1(h, dEQe_32, zwire);      // zwire = h >> dEQe, signed shift right
    SREG   #(.DATAWIDTH(32)) REG_1(x, xwire, Clk, Rst);     // x = xwire, signed register
    SREG   #(.DATAWIDTH(32)) REG_2(z, zwire, Clk, Rst);     // z = zwire, signed register

endmodule
