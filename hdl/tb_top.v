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

module tb_top ();
	

	reg clk, rstn, start, use_prev_cv;
	reg [15:0][31:0] data_i;
 	reg [159:0] cv;

	wire busy, out_valid;
 	wire [159:0] cv_next;

	reg [159:0] expected_result;


	initial
	begin
		
		clk = 0;
		start = 0;
		use_prev_cv = 0;

		// Bits of input 
		rstn = 0;
		expected_result = 160'h0;
		@(posedge clk);
		@(posedge clk);
		@(posedge clk);
		rstn = 1;
		@(posedge clk);
		@(negedge clk);

		$display("Test 1 Begun ...");
		@(posedge clk);
		data_i = {"abc", 8'h80, 416'd0, 61'd3, 3'd0};
		$display("  input is \"abc\"");
		expected_result = 160'ha9993e364706816aba3e25717850c26c9cd0d89d;
		cv = 160'h67452301EFCDAB8998BADCFE10325476C3D2E1F0;
		start = 1;
		@(posedge clk);
		@(negedge clk);
		@(negedge clk);
		@(posedge clk);
		@(negedge clk);
		@(posedge clk);

		while (busy == 1'd1)
		begin
			@(posedge clk);
		end
		$display("  result is %8h %8h %8h %8h %8h", cv_next[159:128], 
					cv_next[127:96], cv_next[95:64], cv_next[63:32], 
					cv_next[31:0]);
		if (cv_next == expected_result) 
		$display ("Passed...");
		else
		$display ("*** FAILED ***");
		$display("Test 1 Done ...");      




		$display("Test 2 Begun ...");
		@(posedge clk);
		data_i = {"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 8'h80, 56'd0};
		$display("  input is \"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq\"");
		expected_result = 160'h84983e441c3bd26ebaae4aa1f95129e5e54670f1;
		cv = 160'h67452301EFCDAB8998BADCFE10325476C3D2E1F0;
		use_prev_cv = 0;

		@(posedge clk);
		start = 1;
		@(posedge clk);
		start = 1;

		@(posedge clk);
		start = 0;

		@(posedge clk);
		start = 1;
		
		while (busy == 1'd1)
			begin
			@(posedge clk);
			end
		@(posedge clk);
		
		data_i = {448'd0, 61'd56, 3'd0};
		use_prev_cv = 1;

		//use_prev_cv = 1;
		@(posedge clk);
		start = 0;

		@(posedge clk);
		start = 1;
		//use_prev_cv = 0;
		while (busy == 1'd1)
			begin
			@(posedge clk);
			end
		$display("  result is %8h %8h %8h %8h %8h", cv_next[159:128], 
					cv_next[127:96], cv_next[95:64], cv_next[63:32], 
					cv_next[31:0]);
		if (cv_next == expected_result) 
			$display ("Passed...");
		else 
			$display ("*** FAILED ***");
		$display("Test 2 Done ...");  
	end

	always #5 clk = ~clk;


	top top (.clk(clk),.rstn(rstn), .start(start), .use_prev_cv(use_prev_cv),
			.data_i(data_i), .cv(cv), .busy(busy), .out_valid(out_valid), .cv_next(cv_next));


endmodule