module arithmetic_shifter_test;		//TESTED and WORKING
	logic signed [15:0] in;
	logic signed [15:0] out;
	
	arithmetic_shifter #(1) shift (.*);
	
	initial
	begin
		in = 16'b0000000000000000;
		#10ns in = 16'b0001000001000000;
		#10ns in = 16'b1100001000000011;
		#10ns in = 16'b1000000000000000;
	end
	
endmodule