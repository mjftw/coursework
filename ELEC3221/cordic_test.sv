module rotational_cordic_test;
	logic [15:0] x , [15:0] y, [15:0] theta, reset, clk, start,
	[15:0] xprime, [15:0] yprime, data_out_rot;
	
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