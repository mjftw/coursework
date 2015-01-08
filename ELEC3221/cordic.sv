module arithmetic_shifter #(parameter i = 0)
(
	input logic [15:0] in,
	output logic [15:0] out
);
	always
	begin
		out[14:0] = in[15:1] >> i;
		out[15] = in[15];
	end
endmodule

module cordic_blk #(parameter i = 0) //cordic block
(
	input logic [15:0] x_in, [15:0] y_in, [15:0] z_in, reset, clk, start, valid_in,
	output logic [15:0] x_out, [15:0] y_out, [15:0] z_out, valid_out
);
	logic x_shifted [15:0], y_shifted [15:0];
	logic x_int [15:0], y_int [15:0], z_int [15:0];
	
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
			y_int = y_in + x_shifter;
		else
			y_int = y_in - x_shifter;
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
		else if(start)
		begin
			x_out <= x_int;
			y_out <= y_int;
			z_out <= z_int;		
			valid_out <= valid_in;
		end
	end
	
	function [15:0] atan_LUT; //LUTout = atan(2^(-i)) in degrees
		input i [3:0];
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
	input logic [15:0] x, [15:0] y, [15:0] theta, reset, clk, start,
	output logic [15:0] xprime, [15:0] yprime, data_out_rot
);

	wire [15:0] x_regs [15:1];
	wire [15:0] y_regs [15:1];
	wire [15:0] z_regs [15:1];
	wire [15:1] valid_flags;
	
	//wire z_out [15:0];
	
	cordic_blk #(0) blk_0(.x_in(x), .y_in(y), .z_in(theta), .x_out(x_regs[1]), .y_out(y_regs[1]), .z_out(z_regs[1]),
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
	
	cordic_blk #(15) blk_15(.x_in(x_regs[15), .y_in(y_regs[15]), .z_in(z_regs[15]), .x_out(xprime), .y_out(yprime)/*, .z_out(z_out)*/,
	.reset(reset), .clk(clk), .start(start), .valid_in(valid_flags[15]), .valid_out(data_out_rot));

endmodule






