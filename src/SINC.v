`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/05/2024 10:28:04 PM
// Design Name: 
// Module Name: SINC
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


module SINC(a,d);

    parameter DATAWIDTH = 32;
    input signed [DATAWIDTH-1:0] a;

    output reg signed [DATAWIDTH-1:0] d;

    always @(a) begin
        d <= a+1;
    end
endmodule
