`timescale 1ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 03/05/2024 10:28:04 PM
// Design Name: 
// Module Name: SCOMP
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


module SCOMP(a,b,gt,lt,eq);

    parameter DATAWIDTH = 32;
    input signed [DATAWIDTH-1:0] a;
    input signed [DATAWIDTH-1:0] b;

    output reg gt,lt,eq;

    // Modified comparator to align with comments in sample behavioral netlist.
    // Comparator sets gt = 1 when b > a and lt = 1 when b < a.
    // Validated behavior with Dr. Tosi during office hours.
    always @(a,b) begin
        gt <= 0; lt <= 0; eq <= 0;
        
        if (b>a) begin
            gt <= 1;
        end
        else if (b<a) begin
            lt <= 1;
        end
        else begin
            eq <= 1;
        end
    end
endmodule