module rotational_cordic_test;
	logic x [15:0], y [15:0], theta [15:0], reset, clk, start,
	xprime [15:0], yprime [15:0], data_out_rot;
	
	rotational_cordic cordic (.*);
	
	initial //clock generation
	begin
		clk = '0;
		forever #5ns clk = ~clk;
	end
	
	initial //set up inputs
	begin
		x = 16'd0;
		y = 16'd1;
		theta = 16'd20;
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