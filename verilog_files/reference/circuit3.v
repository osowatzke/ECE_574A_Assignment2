`timescale 1ns / 1ns

module circuit3(a, b, c, d, e, f, g, h, sa, avg, Clk, Rst);
    
    input  Clk, Rst;
    input  [7:0] sa;
    input  signed [15:0] a, b, c, d, e, f, g, h;

    output signed [15:0] avg;

    wire   signed [31:0] l00, l01, l02, l03, l10, l11, l2, l2div2, l2div4, l2div8;
    wire   signed [31:0] sa_32, avg_32; 
    wire   signed [15:0] l00_16, l01_16, l02_16, l03_16; 

    // Explicitly using zero-extension here as per original structure
    assign l00 = {{16{1'b0}}, l00_16};
    assign l01 = {{16{1'b0}}, l01_16};
    assign l02 = {{16{1'b0}}, l02_16};
    assign l03 = {{16{1'b0}}, l03_16};
    assign sa_32 = {{24{1'b0}}, sa};
    assign avg = avg_32[15:0];

    // Signed additions for input pairs, extended results to 32 bits
    SADD   #(.DATAWIDTH(16)) ADD_1(a, b, l00_16);             // l00 = a + b
    SADD   #(.DATAWIDTH(16)) ADD_2(c, d, l01_16);             // l01 = c + d
    SADD   #(.DATAWIDTH(16)) ADD_3(e, f, l02_16);             // l02 = e + f
    SADD   #(.DATAWIDTH(16)) ADD_4(g, h, l03_16);             // l03 = g + h

    // Adding zero-extended results for intermediate sums
    SADD   #(.DATAWIDTH(32)) ADD_5(l00, l01, l10);            // l10 = l00 + l01
    SADD   #(.DATAWIDTH(32)) ADD_6(l02, l03, l11);            // l11 = l02 + l03
    SADD   #(.DATAWIDTH(32)) ADD_7(l10, l11, l2);             // l2 = l10 + l11

    // Right shifts for average calculation, assuming logical shifts
    SHR    #(.DATAWIDTH(32)) SHR_1(l2, sa_32, l2div2);        // l2div2 = l2 >> sa
    SHR    #(.DATAWIDTH(32)) SHR_2(l2div2, sa_32, l2div4);    // l2div4 = l2div2 >> sa
    SHR    #(.DATAWIDTH(32)) SHR_3(l2div4, sa_32, l2div8);    // l2div8 = l2div4 >> sa

    // Registering the final output
    SREG   #(.DATAWIDTH(32)) REG_1(avg_32, l2div8, Clk, Rst); // avg = l2div8

endmodule
