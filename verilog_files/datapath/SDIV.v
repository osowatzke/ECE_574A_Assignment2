`timescale 1ns/1ns

module SDIV(a, b, qout);

    parameter DATAWIDTH = 32;
    input signed [DATAWIDTH-1:0] a;
    input signed [DATAWIDTH-1:0] b;

    output reg signed [DATAWIDTH-1:0] qout;

    always @(a, b) begin
        qout <= a / b; // Perform signed division
    end
endmodule
