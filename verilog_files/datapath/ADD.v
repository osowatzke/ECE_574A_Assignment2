`timescale 1ns/1ns

module ADD(a, b, sum);

    parameter DATAWIDTH = 32;
    input [DATAWIDTH-1:0] a;
    input [DATAWIDTH-1:0] b;

    output reg [DATAWIDTH-1:0] sum;

    always @(a,b) begin
        sum <= a+b;
    end
endmodule
