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
	
	wire signed [15:0] x_regs [15:1];
	wire signed [15:0] y_regs [15:1];
	wire signed [15:0] z_regs [15:1];
	wire signed valid_flags [15:1];
	
	//wire z_out [15:0];
	
	
	cordic_blk #(0) blk_0(.x_in(x), .y_in(y), .z_in(theta), .x_out(x_regs[1]), .y_out(y_regs[1]), .z_out(z_regs[1]),
	.reset(reset), .clk(clk), .start(start), .valid_in(start), .valid_out(valid_flags[1]));	
	
	genvar i;
	generate
		for(i=2; i<15; i++)
		begin
			cordic_blk #(i-1) blk_1(.x_in(x_regs[i-1]), .y_in(y_regs[i]), .z_in(z_regs[i-1]), .x_out(x_regs[i]), .y_out(y_regs[i]), .z_out(z_regs[i]),
			.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[i-1]), .valid_out(valid_flags[i]));
		end
	endgenerate
	
	cordic_blk #(15) blk_15(.x_in(x_regs[15]), .y_in(y_regs[15]), .z_in(z_regs[15]), .x_out(xprime), .y_out(yprime)/*, .z_out(z_out)*/,
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