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
		if(i == 0) //z datapath
			z_int = z_in + atan_LUT(i);
		else
		begin
			if(z_out[15] == 0)
				z_int = z_in - atan_LUT(i);
			else
				z_int = z_in + atan_LUT(i);
		end
		
		if(z_out[15] == 0) // x datapath
			x_int = x_in - y_shifted;
		else
			x_int = x_in + y_shifted;

		if(z_out[15] == 0) // y datapath
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
			0:  atan_LUT=16'b0011001001000011; 	//45.000000
			1:  atan_LUT=16'b0001110110101100; 	//22.500000
			2:  atan_LUT=16'b0000111110101101; 	//11.250000
			3:  atan_LUT=16'b0000011111110101; 	// 5.625000
			4:  atan_LUT=16'b0000001111111110; 	// 2.812500
			5:  atan_LUT=16'b0000000111111111; 	// 1.406250
			6:  atan_LUT=16'b0000000011111111; 	// 0.703125
			7:  atan_LUT=16'b0000000001111111; 	// 0.351562
			8:  atan_LUT=16'b0000000000111111; 	// 0.175781
			9:  atan_LUT=16'b0000000000011111; 	// 0.087891
			10: atan_LUT=16'b0000000000001111; 	// 0.043945
			11: atan_LUT=16'b0000000000000111; 	// 0.021973
			12: atan_LUT=16'b0000000000000011; 	// 0.010986
			13: atan_LUT=16'b0000000000000001; 	// 0.005493
			14: atan_LUT=16'b0000000000000000; 	// 0.002747
			15: atan_LUT=16'b0000000000000000; 	// 0.001373
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
	
endmodule






