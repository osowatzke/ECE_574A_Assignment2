`timescale 1ns / 1ns

module SSHL(a,sh_amt,d);

    parameter DATAWIDTH = 32;
    input signed [DATAWIDTH-1:0] a;
    input [DATAWIDTH-1:0] sh_amt;

    output reg signed [DATAWIDTH-1:0] d;

    always @(a,sh_amt) begin
        d <= a << sh_amt;
    end
endmodule
