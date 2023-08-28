/*

This code is licensed under a Creative Commons Attribution-NonCommercial 4.0 International License.
https://creativecommons.org/licenses/by-nc/4.0/

Developed by:
- Pietro Fagnani	Politecnico di Torino, Italy - pietro.fagnani@studenti.polito.it
- Luca D'Elia		Politecnico di Torino, Italy - luca.delia@studenti.polito.it
- Marco Massetti	Politecnico di Torino, Italy - marco.massetti@studenti.polito.it
- Jacopo Serra		Politecnico di Torino, Italy - serra.jacopo@studenti.polito.it
- Massimo Emiliano	Politecnico di Torino, Italy - massimo.emiliano@studenti.polito.it
- Gioele Sineo		Politecnico di Torino, Italy - gioele.sineo@studenti.polito.it

There is no warranty

*/

module top (
	input clk, 
	input rstn,
	input start,
	input use_prev_cv,
	input [15:0][31:0] data_i,
	input [159:0] cv,
	output busy,
	output out_valid,
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
						.out_valid_i(out_valid_int), .start_o(start_int), .load_o(load_int), .data_o(data_int), .out_valid_o(out_valid));


endmodule