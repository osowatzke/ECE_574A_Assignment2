`timescale 1ns/1ns

module circuit1_tb();

    localparam CLK_PERIOD = 10;     // Clock period in timesteps
    localparam RESET_TIME = 100;    // Reset time in timesteps
    localparam C_DELAY    = 1;      // Delay in cycles from reset being deasserted to C being valid
    
    wire Clk;
    wire Rst;
    
    reg  signed [7:0]   a, b, c;
    wire signed [7:0]   zref, zmeas;
    wire                zvalid;
    wire                zerr;
    wire signed [15:0]  xref, xmeas;
    wire                xvalid;
    wire                xerr;
    
    clk_gen #(.CLK_PERIOD(CLK_PERIOD)) clk_gen_i(Clk);
    rst_gen #(.RESET_TIME(RESET_TIME)) rst_gen_i(Rst);
    
    always @(posedge(Clk)) a <= Rst ? 0 : $random;
    always @(posedge(Clk)) b <= Rst ? 0 : $random;
    always @(posedge(Clk)) c <= Rst ? 0 : $random;

    valid_gen #(.DELAY(C_DELAY)) valid_gen_i(xvalid, Clk, Rst);
    valid_gen #(.DELAY(C_DELAY)) valid_gen_j(zvalid, Clk, Rst);
    
    circuit1 ref_circuit(.a(a), .b(b), .c(c), .z(zref), .x(xref), .Clk(Clk), .Rst(Rst));

    circuit1_autogen autogen_circuit(.a(a), .b(b), .c(c), .z(zmeas), .x(xmeas), .Clk(Clk), .Rst(Rst));

    serror_monitor #(.DATAWIDTH(8)) serror_monitor_i(zmeas, zref, zvalid, zerr, Clk, Rst);
    serror_monitor #(.DATAWIDTH(16)) serror_monitor_j(xmeas, xref, xvalid, xerr, Clk, Rst);
    
endmodule
