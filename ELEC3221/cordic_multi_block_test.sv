//`define ROTATE
`define VECTOR

`define K 0.60753

module cordic_multi_blk_test;
	logic signed [15:0] x_in, y_in, z_in;
	logic reset, clk, start;

	`ifdef ROTATE
		logic signed [15:0] xprime, yprime;
	 	logic data_out_rot;
	`endif
	`ifdef VECTOR
		logic signed [15:0] rootxy, atanba;	
		logic data_out_vec; 
	`endif	
	
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
	
	`ifdef ROTATE
		cordic_blk #(15) blk_15(.x_in(x_regs[15]), .y_in(y_regs[15]), .z_in(z_regs[15]), .x_out(xprime_unscaled), .y_out(yprime_unscaled),
		.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[15]), .valid_out(data_out_rot));
	`endif
	
	`ifdef VECTOR
		cordic_blk #(15) blk_15(.x_in(x_regs[15]), .y_in(y_regs[15]), .z_in(z_regs[15]), .x_out(xprime_unscaled), .z_out(atanba),
		.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[15]), .valid_out(data_out_rot));
	`endif
	
	always_comb
	begin
		`ifdef VECTOR
			rootxy = xprime_unscaled * `K;
		`endif
		`ifdef ROTATE
			xprime = xprime_unscaled * `K;
			yprime = yprime_unscaled * `K;
		`endif
	end
	
	
	initial //clock generation
	begin
		clk = '0;
		forever #5ns clk = ~clk;
	end
	
	initial //set up input data
	begin
		`ifdef	ROTATIONAL
			x_in = 16'b0000000000000000;	//0, 1, 20
			y_in = 16'b0000000100000000;
			z_in = 16'b0001010000000000;
			#20ns;
			x_in = 16'b0000000011100110;	//0.9, 1, -30
			y_in = 16'b0000000100000000;
			z_in = 16'b1110001000000000;
			#10ns
			x_in = 16'b0000000100000000;	//1, 0, -50
			y_in = 16'b0000000000000000;
			z_in = 16'b1100111000000000;
			#10ns;
			x_in = 16'b0000000001001100;	//0.3, 20, 45.5
			y_in = 16'b0001010000000000;
			z_in = 16'b0010110110000000;
			#10ns;
			x_in = 16'b0000000001001100;	//0.3, 0.5, -63
			y_in = 16'b0000000010000000;
			z_in = 16'b1100000100000000;
		`endif

		`ifdef VECTORING
			x_in = 16'b0000001000000000;	//2, 5, 0
			y_in = 16'b0000010100000000;
			z_in = 16'b0000000000000000;
			#20ns;
			x_in = 16'b0000000100000000;	//1, 1, 0
			y_in = 16'b0000000100000000;
			z_in = 16'b0000000000000000;
			#10ns;
			x_in = 16'b0000000000110011;	//0.2, 1, -30
			y_in = 16'b0000000100000000;
			z_in = 16'b1110001000000000;
			#10ns
			x_in = 16'b0000000001001100;	//0.3, 0.5, 20
			y_in = 16'b0000000010000000;
			z_in = 16'b0001010000000000;
			#10ns;
			x_in = 16'b0000000010000000;	//0.5, 0.3, 0
			y_in = 16'b0000000001001100;
			z_in = 16'b0000000000000000;
			#10ns;
			x_in = 16'b0000000000110011;	//0.2, 0.3, 0
			y_in = 16'b0000000001001100;
			z_in = 16'b0000000000000000;
		`endif
		
		#10ns start = '0;
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
	end
	
endmodule