`timescale 1ns/1ns

module REG(q, d, Clk, Rst);

    parameter   DATAWIDTH = 32;

    input Clk;
    input Rst;

    input [DATAWIDTH-1:0] d;
    output reg [DATAWIDTH-1:0] q;

    always @(posedge Clk) begin
        if (Rst == 1)
            q <= 0;
        else
            q <= d;
    end

endmodule
