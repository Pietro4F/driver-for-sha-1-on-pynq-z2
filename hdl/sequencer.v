module sequencer (
	input clk_i,
	input rst_i,
	input [15:0][31:0] data_i,
	input start_i,
	input out_valid_i,
	output reg start_o,
	output reg load_o,
	output [31:0] data_o
);


	parameter      IDLE = 2'b00;
	parameter      TX = 2'b01;
	parameter      START = 2'b10;
	parameter      WAIT = 2'b11;
	reg [1:0]     state; 
	reg [1:0]      next_state;


	reg cnt_ce;
	reg cnt_tc;


	always @(posedge clk_i, negedge rst_i)
	begin

		if (rst_i == 1'b0) begin
			state = IDLE;
		end else begin
			state = next_state;
		end
	end

	always @(state, start_i, out_valid_i, cnt_tc) begin

		case (state)
				IDLE : begin
					if (start_i == 1'b1) begin
						next_state = TX;
					end else begin
						next_state = IDLE;
					end
				end // case: IDLE

				TX : begin
					if (cnt_tc == 1'b1) begin
						next_state = START;
					end else begin
						next_state = TX;
					end
				end // case: TX

				START : begin
					next_state = WAIT;
				end // case: START

				WAIT : begin
					if (out_valid_i == 1'b1) begin
						next_state = IDLE;
					end else begin
						next_state = WAIT;
					end
				end // case: WAIT
				
				default : begin
					next_state = IDLE;
				end

			endcase
	end

	always @(state)
	begin

		start_o = 1'b0;       
		load_o = 1'b0;
		cnt_ce = 1'b0;

		case (state)
				IDLE : begin
					start_o = 1'b0;       
					load_o = 1'b0;
					cnt_ce = 1'b0;
				end // case: IDLE

				TX : begin       
					load_o = 1'b1;
					cnt_ce = 1'b1;
				end // case: TX

				START : begin
					start_o = 1'b1;  
					load_o = 1'b0;
					cnt_ce = 1'b0;
				end // case: START

				WAIT : begin
					start_o = 1'b0;  
				end // case: WAIT
				
				default : begin
					start_o = 1'b0;       
					load_o = 1'b0;
				end

			endcase
   
	end  

reg [3:0] cnt;

always @(posedge clk_i, negedge rst_i) begin
	
	if (rst_i == 0) begin
		cnt = 4'hF;
	end else if (cnt_ce == 1'b1) begin
		
		cnt = cnt - 1;

		if (cnt == 4'd0) begin
			cnt_tc = 1'b1;
		end else begin
			cnt_tc = 1'b0;
		end

	end

end
	 
	assign data_o = data_i[cnt];

endmodule;

