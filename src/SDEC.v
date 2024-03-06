`timescale 1ns / 1ns

module SDEC(a,d);

    parameter DATAWIDTH = 32;
    input signed [DATAWIDTH-1:0] a;

    output reg signed [DATAWIDTH-1:0] d;

    always @(a) begin
        d <= a-1;
    end
endmodule
