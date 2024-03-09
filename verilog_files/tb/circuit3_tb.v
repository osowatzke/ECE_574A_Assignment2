`timescale 1ns/1ns

module circuit3_tb();

    localparam CLK_PERIOD = 10;     // Clock period in timesteps
    localparam RESET_TIME = 100;    // Reset time in timesteps
    localparam AVG_DELAY  = 1;      // Delay in cycles from reset being deasserted to avg being valid
    
    wire Clk;
    wire Rst;
    
    reg  signed [15:0]   a, b, c, d, e, f, g, h;
    reg  signed [ 7:0]   sa;
    wire signed [15:0]   avgref, avgmeas;
    wire                 avgvalid;
    wire                 avgerr;
    
    clk_gen #(.CLK_PERIOD(CLK_PERIOD)) clk_gen_i(Clk);
    rst_gen #(.RESET_TIME(RESET_TIME)) rst_gen_i(Rst);
    
    always @(posedge(Clk)) a <= Rst ? 0 : $random;
    always @(posedge(Clk)) b <= Rst ? 0 : $random;
    always @(posedge(Clk)) c <= Rst ? 0 : $random;
    always @(posedge(Clk)) d <= Rst ? 0 : $random;
    always @(posedge(Clk)) e <= Rst ? 0 : $random;
    always @(posedge(Clk)) f <= Rst ? 0 : $random;
    always @(posedge(Clk)) g <= Rst ? 0 : $random;
    always @(posedge(Clk)) h <= Rst ? 0 : $random;
    always @(posedge(Clk)) sa <= Rst ? 0 : $random;

    valid_gen #(.DELAY(AVG_DELAY)) valid_gen_i(avgvalid, Clk, Rst);
    
    circuit3 ref_circuit(.a(a), .b(b), .c(c), .d(d), .e(e), .f(f), .g(g), .h(h), .sa(sa), .avg(avgref), .Clk(Clk), .Rst(Rst));

    circuit3_autogen autogen_circuit(.a(a), .b(b), .c(c), .d(d), .e(e), .f(f), .g(g), .h(h), .sa(sa), .avg(avgmeas), .Clk(Clk), .Rst(Rst));

    serror_monitor #(.DATAWIDTH(16)) serror_monitor_i(avgmeas, avgref, avgvalid, avgerr, Clk, Rst);
    
endmodule