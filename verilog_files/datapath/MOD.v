`timescale 1ns/1ns

module MOD(a, b, rem);

    parameter DATAWIDTH = 32;

    input [DATAWIDTH-1:0] a;
    input [DATAWIDTH-1:0] b;
    
    output reg [DATAWIDTH-1:0] rem;
    
    always @(a,b) begin
        rem <= a % b;
    end
endmodule
