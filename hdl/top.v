module top (
	input clk, 
	input rstn,
	input start,
	input use_prev_cv,
	input [15:0][31:0] data_i,
	input [159:0] cv,
	output busy,
	output reg out_valid,
	output [159:0] cv_next
	);
	
	
	wire load_int;
	wire start_int;
	wire out_valid_int;
	wire [31:0] data_int;

	sha1_exec sha1_exec (.clk(clk), .reset(~rstn), .start(start_int),
                        .data_in(data_int), .load_in(load_int),
                        .cv(cv), .use_prev_cv(use_prev_cv), .busy(busy), 
                        .out_valid(out_valid_int), .cv_next(cv_next));

	sequencer sequencer (.clk_i(clk), .rst_i(rstn), .data_i(data_i), .start_i(start),
						.out_valid_i(out_valid_int), .start_o(start_int), .load_o(load_int), .data_o(data_int));

	always @(posedge clk, negedge rstn, posedge start_int) begin
		if (rstn == 1'b0 || start == 1'b1) begin
			
			out_valid <= 1'b0;

		end else begin

			if (out_valid_int == 1'b1) begin
				out_valid <= 1'b1;
			end

		end
	end

endmodule