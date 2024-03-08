`timescale 1ns/1ns

module DIV(a,b,qout);

    parameter DATAWIDTH = 32;
    input [DATAWIDTH-1:0] a;
    input [DATAWIDTH-1:0] b;

    output reg [DATAWIDTH-1:0] qout;

    always @(a,b) begin
        qout <= a/b;
    end
endmodule
