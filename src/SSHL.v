`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/05/2024 10:28:04 PM
// Design Name: 
// Module Name: SSHL
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module SSHL(a,sh_amt,d);

    parameter DATAWIDTH = 32;
    input signed [DATAWIDTH-1:0] a;
    input [DATAWIDTH-1:0] sh_amt;

    output reg signed [DATAWIDTH-1:0] d;

    always @(a,sh_amt) begin
        d <= a << sh_amt;
    end
endmodule
