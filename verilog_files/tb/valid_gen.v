module valid_gen(valid, clk, rst);
    
    parameter DELAY = 1;
    
    input clk;
    input rst;
    
    output valid;
    
    delay_gen #(.DELAY(DELAY)) delay_gen_inst(valid, 1'b1, clk, rst);

endmodule