`timescale 1ns/1ns

module SMUL(a, b, prod);

    parameter DATAWIDTH = 32;
    input signed [DATAWIDTH-1:0] a;
    input signed [DATAWIDTH-1:0] b;

    // The product width should be double the DATAWIDTH to handle the full range of signed multiplication
    output reg signed [2*DATAWIDTH-1:0] prod;

    always @(a, b) begin
        prod <= a * b;
    end
endmodule
