`timescale 1ns/1ns

module SREG(q, d, Clk, Rst);

    parameter   DATAWIDTH = 32;

    input Clk;
    input Rst;

    input signed [DATAWIDTH-1:0] d;
    output reg signed [DATAWIDTH-1:0] q;

    always @(posedge Clk) begin
        if (Rst == 1'b1)
            q <= 0;
        else
            q <= d;
    end

endmodule
