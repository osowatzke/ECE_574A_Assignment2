`timescale 1ns / 1ns

module ucircuit3(a, b, c, Clk, Rst);

        input Clk, Rst;
        input  [ 31:0] a;
        input  [ 15:0] b;

        output [ 7:0] c;

        wire   [ 7:0] cwire;
        
        assign a_8 = a[7:0];
        assign b_8 = b[7:0];

        ADD    #(.DATAWIDTH( 8)) ADD_1(a_8, b_8, cwire);                // cwire = a + b
        REG    #(.DATAWIDTH( 8)) REG_1(c, cwire, Clk, Rst);             // c = cwire

endmodule
