module arithmetic_shifter #(parameter i = 0)
(
	input logic signed [15:0] in,
	output logic signed [15:0] out
);
	always_comb
	begin
		out = in >>> i;
	end
endmodule

module cordic_blk #(parameter i = 0) //cordic block
(
	input logic signed [15:0] x_in, [15:0] y_in, [15:0] z_in,
	input logic reset, clk, start, valid_in,
	output logic signed [15:0] x_out, [15:0] y_out, [15:0] z_out,
	output logic valid_out
);
	logic signed [15:0] y_shifted;
	logic signed [15:0] x_int;
	logic signed [15:0] y_int;
	logic signed [15:0] z_int;
	
	arithmetic_shifter #(i) x_shifter(.in(x_in), .out(x_shifted));
	arithmetic_shifter #(i) y_shifter(.in(y_in), .out(y_shifted));

	always_comb
	begin
		if(z_int[15] == 0) // z datapath
			z_int = z_in - atan_LUT(i);
		else
			z_int = z_in + atan_LUT(i);
		
		if(z_int[15] == 0) // x datapath
			x_int = x_in - y_shifted;
		else
			x_int = x_in + y_shifted;

		if(z_int[15] == 0) // y datapath
			y_int = y_in + x_shifted;
		else
			y_int = y_in - x_shifted;
	end
	
	always_ff @(posedge clk)
	begin
		if(reset) //*TODO* may need to be asynchronus?
		begin
			x_out <= 0;
			y_out <= 0;
			z_out <= 0;
			valid_out <= 0;
		end
		else
		begin
			x_out <= x_int;
			y_out <= y_int;
			z_out <= z_int;		
			valid_out <= valid_in;
		end
	end
	
	function [15:0] atan_LUT; //LUTout = atan(2^(-i)) in degrees
		input [3:0] i ;
		case(i)									//LUTout =
			0:  atan_LUT=16'b0010110100000000; 	//45.000000
			1:  atan_LUT=16'b0001101010010000; 	//26.565012
			2:  atan_LUT=16'b0000111000001001; 	//14.036243
			3:  atan_LUT=16'b0000011100100000;	// 7.125016
			4:  atan_LUT=16'b0000001110010011; 	// 3.576334
			5:  atan_LUT=16'b0000000111001010; 	// 1.789911
			6:  atan_LUT=16'b0000000011100101; 	// 0.895174
			7:  atan_LUT=16'b0000000001110010; 	// 0.447614
			8:  atan_LUT=16'b0000000000111001; 	// 0.223811
			9:  atan_LUT=16'b0000000000011100; 	// 0.111906
			10: atan_LUT=16'b0000000000001110; 	// 0.055953
			11: atan_LUT=16'b0000000000000111; 	// 0.027976
			12: atan_LUT=16'b0000000000000011; 	// 0.013988
			13: atan_LUT=16'b0000000000000001; 	// 0.006994
			14: atan_LUT=16'b0000000000000000; 	// 0.003497
			15: atan_LUT=16'b0000000000000000; 	// 0.001749
		endcase
	endfunction
endmodule

module rotational_cordic //acts as top level interconnect for all the cordic blocks *TODO* scale output
(
	input logic signed [15:0] x, [15:0] y, [15:0] theta,
	input logic	reset, clk, start,
	output logic signed [15:0] xprime, [15:0] yprime,
	output logic data_out_rot
);

	wire signed [15:0] x_regs [15:1];
	wire signed [15:0] y_regs [15:1];
	wire signed [15:0] z_regs [15:1];
	wire signed valid_flags [15:1];
	
	wire signed [15:0] xprime_unscaled, yprime_unscaled;
	
	//wire z_out [15:0];
	
	cordic_blk #(0) blk_0(.x_in(x), .y_in(y), .z_in(theta), .x_out(x_regs[1]), .y_out(y_regs[1]), .z_out(z_regs[1]),
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
	
endmodule






