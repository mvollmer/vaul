-- ************************************************************************
--  Copyright (C) 1989, 1992 - University of Pittsburgh, Dept. Elect. Engr.
--  PICALAB: Pittsburgh Integrated Circuits Analysis Laboratory
-- ************************************************************************
--  Title: 	SN54S151 8-input Data Selector/Multiplexer 
--  Filename:   "SN54S151.vhdl"
--  Created:	Thur 19 October 1989
--  Author: 	Steve Frezza 	<frezza@jupiter.ee.pitt.edu>
--
--  Description: TI decoder/demultiplexer from "TTL Data Book for Design 
--               Engineers," 1st Ed. p 295
--  
--  Modification History:  Thur 16 January 1992 Comments added to provide 
--               guidance for compilation and clarity.
--
-- ************************************************************************
--
	          ----Input Descriptions:----------------
	    	  -- A, B, C are the Data Select Lines;
	    	  -- Strobe acts as the chip enable line;
	    	  -- D_0..D_7 are the input data lines;

      		  ---Output Descriptions:----------------
	    	  -- out_y & out_w are the output lines.
	    	  ----------------------------------------
		  
		  
--               The response vectors for the SN54S151 are:
--                  +--------------------------------+
--                  | Select  |Strobe |   Outputs    |
--                  +---------+-------+--------------+
--                  | C  B  A |   S   |  W      Y    |
--                  +---------+-------+--------------+
--                  | x  x  x |   H   |  L      H    |
--                  | L  L  L |   L   | D_0  inv(D_0)|
--                  | L  L  H |   L   | D_1  inv(D_1)|
--                  | L  H  L |   L   | D_2  inv(D_2)|
--                  | L  H  H |   L   | D_3  inv(D_3)|
--                  | H  L  L |   L   | D_4  inv(D_4)|
--                  | H  L  H |   L   | D_5  inv(D_5)|
--                  | H  H  L |   L   | D_6  inv(D_6)|
--                  | H  H  H |   L   | D_7  inv(D_7)|
--                  +---------+-------+--------------+
--
--
entity SN54S151 is  --  
    port (strobe, A, B, C : in bit;
    	  D : in bit_vector(7 downto 0);
	  outY, outW: out bit);
	  --
end SN54S151;
--
architecture str of SN54S151 is
--  
    type three_bits is record
	a, b, c : bit;
    end record;

    type blah is record
	a, b, c : bit;
    end record;

    signal strobe_bar, A_bar, B_bar, C_bar,
	   A_bar_bar, B_bar_bar, C_bar_bar : bit;
    signal X_bar : three_bits;
    signal t : bit_vector(7 downto 0);
    signal blub : blah;
--
--
begin
--
     strobe_bar <= not strobe;
     X_bar <= ( not A, not B, not C ) after 1 ns;
     ( A_bar, B_bar, C_bar ) <= X_bar;
     A_bar_bar <= not  X_bar.a after 1 ns;
     B_bar_bar <= not  X_bar.b after 1 ns;
     C_bar_bar <= not  X_bar.c after 1 ns;

     t(0) <= (D(0) and A_bar     and B_bar     and C_bar     and strobe_bar) after 2 ns;
     t(1) <= (D(1) and A_bar_bar and B_bar     and C_bar     and strobe_bar) after 2 ns;
     t(2) <= (D(2) and A_bar     and B_bar_bar and C_bar     and strobe_bar) after 2 ns;
     t(3) <= (D(3) and A_bar_bar and B_bar_bar and C_bar     and strobe_bar) after 2 ns;
     t(4) <= (D(4) and A_bar     and B_bar     and C_bar_bar and strobe_bar) after 2 ns;
     t(5) <= (D(5) and A_bar_bar and B_bar     and C_bar_bar and strobe_bar) after 2 ns;
     t(6) <= (D(6) and A_bar     and B_bar_bar and C_bar_bar and strobe_bar) after 2 ns;
     t(7) <= (D(7) and A_bar_bar and B_bar_bar and C_bar_bar and strobe_bar) after 2 ns;

     outY <= not (t(0) or t(1) or t(2) or t(3) or
     	          t(4) or t(5) or t(6) or t(7)) after 2 ns;
     outW <= not outY after 1 ns;
     
end  str;


---- Compilation and Simulation Notes: -------------------------------------------
-- To compile this file using vcomp:
--
-- vcomp -Oz -eSN54S151.str SN54S151.vhdl
--
-- To simulate this multiplexor, use the associated -init file:  "SN54S151-init"
--
-- vsim -i SN54S151-init SN54S151.ivf
--
-- The expected results for this simulation are stored in file "sn54s151.log"
--
-- A postscript image of the design can be found in "SN54S151.ps"
--
-----------------------------------------------------------------------------------
