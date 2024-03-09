`timescale 1ns/1ns

module circuit4(a, b, c, z, x, Clk, Rst);

    input  Clk, Rst;
    input  signed [63:0] a, b, c;

    output signed [31:0] z, x;

    wire signed [63:0] d, e, f, g, h;
    wire dLTe, dEQe;
    wire [63:0] dLTe_64, dEQe_64;
    wire signed [63:0] greg, hreg, x_64, z_64;
    wire signed [31:0] xrin, zrin;
    wire signed [31:0] xrin_32, zrin_32;

    assign dLTe_64 = {{63{1'b0}}, dLTe};
    assign dEQe_64 = {{63{1'b0}}, dEQe};
    assign xrin_32 = xrin[31:0];
    assign zrin_32 = zrin[31:0];

    SADD    #(.DATAWIDTH(64)) ADD_1(a, b, d);                // d = a + b, signed addition    
    SADD    #(.DATAWIDTH(64)) ADD_2(a, c, e);                // e = a + c, signed addition 
    SSUB    #(.DATAWIDTH(64)) SUB_3(a, b, f);                // f = a - b, signed subtraction
    SCOMP   #(.DATAWIDTH(64)) COMP_1(d, e, , , dEQe);        // dEQe = d == e, signed comparison

    // Using gt output of modified comparator for d < e
    SCOMP   #(.DATAWIDTH(64)) COMP_2(d, e, , dLTe, );        // dLTe = d < e, signed comparison
    SMUX2x1 #(.DATAWIDTH(64)) MUX2x1_1(e, d, dLTe, g);       // g = dLTe ? d : e, signed multiplexing
    SMUX2x1 #(.DATAWIDTH(64)) MUX2x1_2(f, g, dEQe, h);       // h = dEQe ? g : f, signed multiplexing
    SREG    #(.DATAWIDTH(64)) REG_1(greg, g, Clk, Rst);      // greg = g, signed register
    SREG    #(.DATAWIDTH(64)) REG_2(hreg, h, Clk, Rst);      // hreg = h, signed register
    SSHL    #(.DATAWIDTH(64)) SHL_1(hreg, dLTe_64, xrin);    // xrin = hreg << dLTe, signed shift
    SSHR    #(.DATAWIDTH(64)) SHR_1(greg, dEQe_64, zrin);    // zrin = greg >> dEQe, signed shift
    SREG    #(.DATAWIDTH(32)) REG_3(x, xrin_32, Clk, Rst);   // x = xrin, signed register
    SREG    #(.DATAWIDTH(32)) REG_4(z, zrin_32, Clk, Rst);   // z = zrin, signed register

endmodule
