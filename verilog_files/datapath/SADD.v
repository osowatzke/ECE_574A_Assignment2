`timescale 1ns / 1ns

module SADD(a, b, sum);

    parameter DATAWIDTH = 32;
    input signed [DATAWIDTH-1:0] a;
    input signed [DATAWIDTH-1:0] b;

    output reg signed [DATAWIDTH-1:0] sum;

    always @(a,b) begin
        sum <= a+b;
    end
endmodule
