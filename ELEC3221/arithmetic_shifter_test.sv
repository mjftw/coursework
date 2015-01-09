module arithmetic_shifter_test;		//TESTED and WORKING
	logic [16:0] in;
	logic [16:0] out;
	
	arithmetic_shifter #(2) shift (.*);
	
	initial
	begin
		in = 17'b00000000000000000;
		#10ns in = 17'b00001000001000000;
		#10ns in = 17'b11100001000000011;
		#10ns in = 17'b10000000000000000;
	end
	
endmodule