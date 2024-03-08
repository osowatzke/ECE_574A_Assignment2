`timescale 1ns/1ns

module INC(a,d);

    parameter DATAWIDTH = 32;
    input [DATAWIDTH-1:0] a;

    output reg [DATAWIDTH-1:0] d;

    always @(a) begin
        d <= a+1;
    end
endmodule
