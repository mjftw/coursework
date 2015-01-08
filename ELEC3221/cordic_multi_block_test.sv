module cordic_multi_blk_test;
	logic signed [15:0] x_in, y_in, z_in, xprime, yprime;
	logic reset, clk, start, data_out_rot;
	
	wire signed [15:0] x_regs [15:1];
	wire signed [15:0] y_regs [15:1];
	wire signed [15:0] z_regs [15:1];
	wire signed valid_flags [15:1];
	
	wire signed [15:0] xprime_unscaled, yprime_unscaled;
	
	//wire z_out [15:0];
	
	
	cordic_blk #(0) blk_0(.x_in(x_in), .y_in(y_in), .z_in(z_in), .x_out(x_regs[1]), .y_out(y_regs[1]), .z_out(z_regs[1]),
	.reset(reset), .clk(clk), .start(start), .valid_in(start), .valid_out(valid_flags[1]));	
	
	genvar i;
	generate
		for(i=1; i<15; i++)
		begin
			cordic_blk #(i) blk_i(.x_in(x_regs[i]), .y_in(y_regs[i]), .z_in(z_regs[i]), .x_out(x_regs[i+1]), .y_out(y_regs[i+1]), .z_out(z_regs[i+1]),
			.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[i]), .valid_out(valid_flags[i+1]));
		end
	endgenerate
	
	cordic_blk #(15) blk_15(.x_in(x_regs[15]), .y_in(y_regs[15]), .z_in(z_regs[15]), .x_out(xprime_unscaled), .y_out(yprime_unscaled)/*, .z_out(z_out)*/,
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[15]), .valid_out(data_out_rot));
	
	always_comb
	begin
		xprime = xprime_unscaled * 0.607;
		yprime = yprime_unscaled * 0.607;
	end
	
	
	initial //clock generation
	begin
		clk = '0;
		forever #5ns clk = ~clk;
	end
	
	initial //set up input data
	begin
		x_in = 16'b0000000100000000;
		y_in = 16'b0000000100000000;
		z_in = 16'b0001111000000000;
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