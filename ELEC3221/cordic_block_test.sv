module cordic_blk_test;
	logic signed [15:0] x_in;
	logic signed [15:0] y_in;
	logic signed [15:0] z_in;
	logic reset;
	logic clk;
	logic start;
	logic valid_in;
	logic signed [15:0] x_out;
	logic signed [15:0] y_out;
	logic signed [15:0] z_out;
	logic valid_out;
	
	cordic_blk #(0) blk_1 (.*);
	cordic_blk #(1 blk_2 (.*);
	
	initial //clock generation
	begin
		clk = '0;
		forever #5ns clk = ~clk;
	end
	
	initial //set up inputs
	begin
		x_in = 16'd0;
		y_in = 16'd1;
		z_in = 16'd20;
	end
	
	initial
	begin
		reset = '0;
		start = '0;
		#6ns reset = '1;
		#5ns reset = '0;
		#10ns start = '1;
	end
	
endmodule