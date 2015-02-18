module cordic_top
(
	input logic signed [15:0] x, [15:0] y, [15:0] a, [15:0] b,
	input logic reset, clk, start,
	output logic signed [15:0] X, [15:0] Y,
	output logic data_out
);
	vectoring_cordic vec(
	rotational_cordic rot(