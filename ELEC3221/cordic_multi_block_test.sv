module cordic_multi_blk_test;
	logic signed [15:0] x_in;
	logic signed [15:0] y_in;
	logic signed [15:0] z_in;
	logic reset;
	logic clk;
	logic start;
	logic signed [15:0] x_out;
	logic signed [15:0] y_out;
	logic signed [15:0] z_out;
	logic data_out_rot;
	
	wire signed [15:1] x_regs [15:0];
	wire signed [15:1] y_regs [15:0];
	wire signed [15:1] z_regs [15:0];
	wire signed [15:1] valid_flags;
	
	//wire z_out [15:0];
	
	cordic_blk #(0) blk_0(.x_in(x_in), .y_in(y_in), .z_in(z_in), .x_out(x_regs[1]), .y_out(y_regs[1]), .z_out(z_regs[1]),
	.reset(reset), .clk(clk), .start(start), .valid_in(start), .valid_out(valid_flags[1]));

	cordic_blk #(1) blk_1(.x_in(x_regs[1]), .y_in(y_regs[1]), .z_in(z_regs[1]), .x_out(x_regs[2]), .y_out(y_regs[2]), .z_out(z_regs[2]),
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[1]), .valid_out(valid_flags[2]));
	
	cordic_blk #(2) blk_2(.x_in(x_regs[2]), .y_in(y_regs[1]), .z_in(z_regs[2]), .x_out(x_regs[3]), .y_out(y_regs[3]), .z_out(z_regs[3]),
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[2]), .valid_out(valid_flags[3]));
	
	cordic_blk #(3) blk_3(.x_in(x_regs[3]), .y_in(y_regs[3]), .z_in(z_regs[3]), .x_out(x_regs[4]), .y_out(y_regs[4]), .z_out(z_regs[4]),
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[3]), .valid_out(valid_flags[4]));
	
	cordic_blk #(4) blk_4(.x_in(x_regs[4]), .y_in(y_regs[4]), .z_in(z_regs[4]), .x_out(x_regs[5]), .y_out(y_regs[5]), .z_out(z_regs[5]),
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[4]), .valid_out(valid_flags[5]));
	
	cordic_blk #(5) blk_5(.x_in(x_regs[5]), .y_in(y_regs[5]), .z_in(z_regs[5]), .x_out(x_regs[6]), .y_out(y_regs[6]), .z_out(z_regs[6]),
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[5]), .valid_out(valid_flags[6]));
	
	cordic_blk #(6) blk_6(.x_in(x_regs[6]), .y_in(y_regs[6]), .z_in(z_regs[6]), .x_out(x_regs[7]), .y_out(y_regs[7]), .z_out(z_regs[7]),
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[6]), .valid_out(valid_flags[7]));
	
	cordic_blk #(7) blk_7(.x_in(x_regs[7]), .y_in(y_regs[7]), .z_in(z_regs[7]), .x_out(x_regs[8]), .y_out(y_regs[8]), .z_out(z_regs[8]),
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[7]), .valid_out(valid_flags[8]));
	
	cordic_blk #(8) blk_8(.x_in(x_regs[8]), .y_in(y_regs[8]), .z_in(z_regs[8]), .x_out(x_regs[9]), .y_out(y_regs[9]), .z_out(z_regs[9]),
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[8]), .valid_out(valid_flags[9]));
	
	cordic_blk #(9) blk_9(.x_in(x_regs[9]), .y_in(y_regs[9]), .z_in(z_regs[9]), .x_out(x_regs[10]), .y_out(y_regs[10]), .z_out(z_regs[10]),
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[9]), .valid_out(valid_flags[10]));
	
	cordic_blk #(10) blk_10(.x_in(x_regs[10]), .y_in(y_regs[10]), .z_in(z_regs[10]), .x_out(x_regs[11]), .y_out(y_regs[11]), .z_out(z_regs[11]),
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[10]), .valid_out(valid_flags[11]));
	
	cordic_blk #(11) blk_11(.x_in(x_regs[11]), .y_in(y_regs[11]), .z_in(z_regs[11]), .x_out(x_regs[12]), .y_out(y_regs[12]), .z_out(z_regs[12]),
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[11]), .valid_out(valid_flags[12]));
	
	cordic_blk #(12) blk_12(.x_in(x_regs[12]), .y_in(y_regs[12]), .z_in(z_regs[12]), .x_out(x_regs[13]), .y_out(y_regs[13]), .z_out(z_regs[13]),
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[12]), .valid_out(valid_flags[13]));
	
	cordic_blk #(13) blk_13(.x_in(x_regs[13]), .y_in(y_regs[13]), .z_in(z_regs[13]), .x_out(x_regs[14]), .y_out(y_regs[14]), .z_out(z_regs[14]),
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[13]), .valid_out(valid_flags[14]));
	
	cordic_blk #(14) blk_14(.x_in(x_regs[14]), .y_in(y_regs[14]), .z_in(z_regs[14]), .x_out(x_regs[15]), .y_out(y_regs[15]), .z_out(z_regs[15]),
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[14]), .valid_out(valid_flags[15]));
	
	cordic_blk #(15) blk_15(.x_in(x_regs[15]), .y_in(y_regs[15]), .z_in(z_regs[15]), .x_out(xprime), .y_out(yprime), .z_out(z_out),
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[15]), .valid_out(data_out_rot));
	
	initial //clock generation
	begin
		clk = '0;
		forever #5ns clk = ~clk;
	end
	
	initial //set up input data
	begin
		x_in = 16'd0;
		y_in = 16'd1;
		z_in = 16'd20;
	end
	
	initial //reset signal
	begin
		reset = '0;
		#3ns reset = '1;
		#7ns reset = '0;
	end
	
	initial //start signal
	begin
		start = '0;
		#10ns start = '1;
		#70ns start = '0;
		#40ns start = '1;
	end
	
endmodule