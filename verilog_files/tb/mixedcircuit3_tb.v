`timescale 1ns/1ns

module mixedcircuit3_tb();

    localparam CLK_PERIOD = 10;     // Clock period in timesteps
    localparam RESET_TIME = 100;    // Reset time in timesteps
    localparam C_DELAY    = 1;      // Delay in cycles from reset being deasserted to C being valid
    
    wire Clk;
    wire Rst;
    
    reg  signed [31:0] a;
    reg         [15:0] b;
    wire signed [ 7:0] cref;
    wire signed [ 7:0] cmeas;
    wire               cvalid;
    wire               cerr;
    
    clk_gen #(.CLK_PERIOD(CLK_PERIOD)) clk_gen_i(Clk);
    rst_gen #(.RESET_TIME(RESET_TIME)) rst_gen_i(Rst);
    
    always @(posedge(Clk)) a <= Rst ? 0 : $random;
    always @(posedge(Clk)) b <= Rst ? 0 : $random;

    valid_gen #(.DELAY(C_DELAY)) valid_gen_i(cvalid, Clk, Rst);
    
    mixedcircuit3 ref_circuit(.a(a), .b(b), .c(cref), .Clk(Clk), .Rst(Rst));

    mixedcircuit3_autogen autogen_circuit(.a(a), .b(b), .c(cmeas), .Clk(Clk), .Rst(Rst));

    serror_monitor #(.DATAWIDTH(8)) serror_monitor_i(cmeas, cref, cvalid, cerr, Clk, Rst);
    
endmodule