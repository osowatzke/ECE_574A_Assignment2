`timescale 1ns/1ns

module circuit6_tb();

    localparam CLK_PERIOD = 10;     // Clock period in timesteps
    localparam RESET_TIME = 100;    // Reset time in timesteps
    localparam Z_DELAY    = 1;      // Delay in cycles from reset being deasserted to Z being valid
    
    wire Clk;
    wire Rst;
    
    reg  signed [63:0]   a, b, c, zero;
    wire signed [63:0]   zref, zmeas;
    wire                 zvalid;
    wire                 zerr;
    
    clk_gen #(.CLK_PERIOD(CLK_PERIOD)) clk_gen_i(Clk);
    rst_gen #(.RESET_TIME(RESET_TIME)) rst_gen_i(Rst);
    
    always @(posedge(Clk)) a <= Rst ? 0 : $random;
    always @(posedge(Clk)) b <= Rst ? 0 : $random;
    always @(posedge(Clk)) c <= Rst ? 0 : $random;
    always @(posedge(Clk)) zero <= Rst ? 0 : $random;

    valid_gen #(.DELAY(Z_DELAY)) valid_gen_j(zvalid, Clk, Rst);
    
    circuit6 ref_circuit(.a(a), .b(b), .c(c), .zero(zero), .z(zref), .Clk(Clk), .Rst(Rst));

    circuit6_autogen autogen_circuit(.a(a), .b(b), .c(c), .zero(zero), .z(zmeas), .Clk(Clk), .Rst(Rst));

    serror_monitor #(.DATAWIDTH(64)) serror_monitor_i(zmeas, zref, zvalid, zerr, Clk, Rst);
    
endmodule