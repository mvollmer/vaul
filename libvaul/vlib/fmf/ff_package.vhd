-- ---------------------------------------------------------------
--   File name : ff_package.vhd
-- ---------------------------------------------------------------
--  Copyright (C) 1995 Free Model Foundation
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms og the GNU General Public License version 2 as
--  published by the Free Software Foundation.
--
--	 MODIFICATION HISTORY :
--
--      version no: |   author: |   mod. date: |    changes made:
--      V1.0            rev3        95 SEP 22     Initial release
--
LIBRARY IEEE;
USE     IEEE.Std_Logic_1164.ALL;
USE     IEEE.VITAL_primitives.all;
USE	IEEE.VITAL_timing.all;

PACKAGE ff_package IS

    --************************************************************

	-- Key to table names:
	-- <ff identifier>FF<N if clock active low>[S or R]<N if active low>
	-- [M]<# of mux inputs><QN if outputs inverted>_tab

    ---------------------------------------------------------------
	-- STANDARD D_TYPE STATE TABLES
	-- These tables have robust truth tables that function according
	-- to the previous state of the D_f/f.
    ---------------------------------------------------------------

    ---------------------------------------------------------------
	-- D-flip/flop with Set and Reset both active high
    ---------------------------------------------------------------
    CONSTANT DFFSR_tab : VitalStateTableType := (

        -- -------INPUTS-----------|PREV-|-OUTPUT--
        -- Viol CLK   D    S    R  | QI  | Q'    --
        ---------------------------|-----|---------
        (  'X', '-', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'B', '-', 'X', '0', '1', '1'), -- set unknown
        (  '-', '/', '1', 'X', '0', '1', '1'), -- set unknown
        (  '-', '-', '-', 'X', '-', '-', 'X'), -- set unknown
        (  '-', 'B', '-', '0', 'X', '0', '0'), -- reset unknown
        (  '-', '/', '0', '0', 'X', '0', '0'), -- reset unknown
        (  '-', '-', '-', '-', 'X', '-', 'X'), -- reset unknown
        (  '-', '-', '-', '1', '1', '-', 'X'), -- both asserted
        (  '-', '-', '-', '1', '0', '-', '1'), -- set asserted
        (  '-', '-', '-', '0', '1', '-', '0'), -- reset asserted
        (  '-', 'X', '0', '0', '0', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '0', '0', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '0', '0', '-', 'X'), -- clk unknown
        (  '-', '/', '0', '0', '0', '-', '0'), -- active clock edge
        (  '-', '/', '1', '0', '0', '-', '1'), -- active clock edge
        (  '-', '/', '-', '0', '0', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', '-', 'S')  -- default
 
        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
	-- D-flip/flop with Set and Reset both active high and active-low clk
    ---------------------------------------------------------------
    CONSTANT DFFNSR_tab : VitalStateTableType := (

        -- -------INPUTS-----------|PREV-|-OUTPUT--
        -- Viol CLK   D    S    R  | QI  | Q'    --
        ---------------------------|-----|---------
        (  'X', '-', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'B', '-', 'X', '0', '1', '1'), -- set unknown
        (  '-', '\', '1', 'X', '0', '1', '1'), -- set unknown
        (  '-', '-', '-', 'X', '-', '-', 'X'), -- set unknown
        (  '-', 'B', '-', '0', 'X', '0', '0'), -- reset unknown
        (  '-', '\', '0', '0', 'X', '0', '0'), -- reset unknown
        (  '-', '-', '-', '-', 'X', '-', 'X'), -- reset unknown
        (  '-', '-', '-', '1', '1', '-', 'X'), -- both asserted
        (  '-', '-', '-', '1', '0', '-', '1'), -- set asserted
        (  '-', '-', '-', '0', '1', '-', '0'), -- reset asserted
        (  '-', 'X', '0', '0', '0', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '0', '0', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '0', '0', '-', 'X'), -- clk unknown
        (  '-', '\', '0', '0', '0', '-', '0'), -- active clock edge
        (  '-', '\', '1', '0', '0', '-', '1'), -- active clock edge
        (  '-', '\', '-', '0', '0', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', '-', 'S')  -- default
 
        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
	-- D-flip/flop with Reset active high
    ---------------------------------------------------------------
    CONSTANT DFFR_tab : VitalStateTableType  := (

        ---------INPUTS------|PREV-|-OUTPUT--
        -- Viol CLK   D    R | QI  | Q'    --
        ---------------------|-----|---------
        (  'X', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'B', '-', 'X', '0', '0'), -- reset unknown
        (  '-', '/', '0', 'X', '0', '0'), -- reset unknown
        (  '-', '-', '-', 'X', '-', 'X'), -- reset unknown
        (  '-', '-', '-', '1', '-', '0'), -- reset asserted
        (  '-', 'X', '0', '0', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '0', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '0', '-', 'X'), -- clk unknown
        (  '-', '/', '0', '0', '-', '0'), -- active clock edge
        (  '-', '/', '1', '0', '-', '1'), -- active clock edge
        (  '-', '/', '-', '0', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
	-- D-flip/flop with Reset active high and active-low clock
    ---------------------------------------------------------------
    CONSTANT DFFNR_tab : VitalStateTableType  := (

        ---------INPUTS------|PREV-|-OUTPUT--
        -- Viol CLK   D    R | QI  | Q'    --
        ---------------------|-----|---------
        (  'X', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'B', '-', 'X', '0', '0'), -- reset unknown
        (  '-', '\', '0', 'X', '0', '0'), -- reset unknown
        (  '-', '-', '-', 'X', '-', 'X'), -- reset unknown
        (  '-', '-', '-', '1', '-', '0'), -- reset asserted
        (  '-', 'X', '0', '0', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '0', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '0', '-', 'X'), -- clk unknown
        (  '-', '\', '0', '0', '-', '0'), -- active clock edge
        (  '-', '\', '1', '0', '-', '1'), -- active clock edge
        (  '-', '\', '-', '0', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
	-- D-flip/flop with Reset active high, active-low clock, and
	-- inverting outputs
    ---------------------------------------------------------------
    CONSTANT DFFNRQN_tab : VitalStateTableType  := (

        ---------INPUTS------|PREV-|-OUTPUT--
        -- Viol CLK   D    R | QI  | Q'    --
        ---------------------|-----|---------
        (  'X', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'B', '-', 'X', '1', '1'), -- reset unknown
        (  '-', '\', '0', 'X', '1', '1'), -- reset unknown
        (  '-', '-', '-', 'X', '-', 'X'), -- reset unknown
        (  '-', '-', '-', '1', '-', '1'), -- reset asserted
        (  '-', 'X', '0', '0', '1', '1'), -- clk unknown
        (  '-', 'X', '1', '0', '0', '0'), -- clk unknown
        (  '-', 'X', '-', '0', '-', 'X'), -- clk unknown
        (  '-', '\', '0', '0', '-', '1'), -- active clock edge
        (  '-', '\', '1', '0', '-', '0'), -- active clock edge
        (  '-', '\', '-', '0', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition
    ---------------------------------------------------------------
	-- D-flip/flop with Reset active low
    ---------------------------------------------------------------
    CONSTANT DFFRN_tab : VitalStateTableType  := (

        ---------INPUTS------|PREV-|-OUTPUT--
        -- Viol CLK   D    R | QI  | Q'    --
        ---------------------|-----|---------
        (  'X', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'B', '-', 'X', '0', '0'), -- reset unknown
        (  '-', '/', '0', 'X', '0', '0'), -- reset unknown
        (  '-', '-', '-', 'X', '-', 'X'), -- reset unknown
        (  '-', '-', '-', '0', '-', '0'), -- reset asserted
        (  '-', 'X', '0', '1', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '1', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '1', '-', 'X'), -- clk unknown
        (  '-', '/', '0', '1', '-', '0'), -- active clock edge
        (  '-', '/', '1', '1', '-', '1'), -- active clock edge
        (  '-', '/', '-', '1', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
	-- D-flip/flop with Reset active low and active-low clock
    ---------------------------------------------------------------
    CONSTANT DFFNRN_tab : VitalStateTableType  := (

        ---------INPUTS------|PREV-|-OUTPUT--
        -- Viol CLK   D    R | QI  | Q'    --
        ---------------------|-----|---------
        (  'X', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'B', '-', 'X', '0', '0'), -- reset unknown
        (  '-', '\', '0', 'X', '0', '0'), -- reset unknown
        (  '-', '-', '-', 'X', '-', 'X'), -- reset unknown
        (  '-', '-', '-', '0', '-', '0'), -- reset asserted
        (  '-', 'X', '0', '1', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '1', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '1', '-', 'X'), -- clk unknown
        (  '-', '\', '0', '1', '-', '0'), -- active clock edge
        (  '-', '\', '1', '1', '-', '1'), -- active clock edge
        (  '-', '\', '-', '1', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
	-- D-flip/flop with Set active high
    ---------------------------------------------------------------
    CONSTANT DFFS_tab : VitalStateTableType  := (

        ---------INPUTS------|PREV-|-OUTPUT--
        -- Viol CLK   D    S | QI  | Q'    --
        ---------------------|-----|---------
        (  'X', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'B', '-', 'X', '1', '1'), -- set unknown
        (  '-', '/', '1', 'X', '1', '1'), -- set unknown
        (  '-', '-', '-', 'X', '-', 'X'), -- set unknown
        (  '-', '-', '-', '1', '-', '1'), -- set asserted
        (  '-', 'X', '0', '0', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '0', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '0', '-', 'X'), -- clk unknown
        (  '-', '/', '0', '0', '-', '0'), -- active clock edge
        (  '-', '/', '1', '0', '-', '1'), -- active clock edge
        (  '-', '/', '-', '0', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
	-- D-flip/flop with Set active high and active-low clock
    ---------------------------------------------------------------
    CONSTANT DFFNS_tab : VitalStateTableType  := (

        ---------INPUTS------|PREV-|-OUTPUT--
        -- Viol CLK   D    S | QI  | Q'    --
        ---------------------|-----|---------
        (  'X', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'B', '-', 'X', '1', '1'), -- set unknown
        (  '-', '\', '1', 'X', '1', '1'), -- set unknown
        (  '-', '-', '-', 'X', '-', 'X'), -- set unknown
        (  '-', '-', '-', '1', '-', '1'), -- set asserted
        (  '-', 'X', '0', '0', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '0', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '0', '-', 'X'), -- clk unknown
        (  '-', '\', '0', '0', '-', '0'), -- active clock edge
        (  '-', '\', '1', '0', '-', '1'), -- active clock edge
        (  '-', '\', '-', '0', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
	-- D-flip/flop with Set active low
    ---------------------------------------------------------------
    CONSTANT DFFSN_tab : VitalStateTableType  := (

        ---------INPUTS------|PREV-|-OUTPUT--
        -- Viol CLK   D    S | QI  | Q'    --
        ---------------------|-----|---------
        (  'X', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'B', '-', 'X', '1', '1'), -- set unknown
        (  '-', '/', '1', 'X', '1', '1'), -- set unknown
        (  '-', '-', '-', 'X', '-', 'X'), -- set unknown
        (  '-', '-', '-', '0', '-', '1'), -- set asserted
        (  '-', 'X', '0', '1', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '1', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '1', '-', 'X'), -- clk unknown
        (  '-', '/', '0', '1', '-', '0'), -- active clock edge
        (  '-', '/', '1', '1', '-', '1'), -- active clock edge
        (  '-', '/', '-', '1', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
	-- D-flip/flop with Set active low and active-low clock
    ---------------------------------------------------------------
    CONSTANT DFFNSN_tab : VitalStateTableType  := (

        ---------INPUTS------|PREV-|-OUTPUT--
        -- Viol CLK   D    S | QI  | Q'    --
        ---------------------|-----|---------
        (  'X', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'B', '-', 'X', '1', '1'), -- set unknown
        (  '-', '\', '1', 'X', '1', '1'), -- set unknown
        (  '-', '-', '-', 'X', '-', 'X'), -- set unknown
        (  '-', '-', '-', '0', '-', '1'), -- set asserted
        (  '-', 'X', '0', '1', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '1', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '1', '-', 'X'), -- clk unknown
        (  '-', '\', '0', '1', '-', '0'), -- active clock edge
        (  '-', '\', '1', '1', '-', '1'), -- active clock edge
        (  '-', '\', '-', '1', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
	-- D-flip/flop with 2-input mux on D inputs with select
    ---------------------------------------------------------------
    CONSTANT DFFM2_tab : VitalStateTableType  := (

        ---------INPUTS-----------|PREV-|-OUTPUT--
        -- Viol CLK   D0   D1   S | QI  | Q'    --
        --------------------------|-----|---------
        (  'X', '-', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'X', '0', '-', '0', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '-', '0', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '0', '1', '0', '0'), -- clk unknown
        (  '-', 'X', '-', '1', '1', '1', '1'), -- clk unknown
        (  '-', 'X', '0', '0', 'X', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '1', 'X', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '-', '-', '-', 'X'), -- clk unknown
        (  '-', '/', '0', '-', '0', '-', '0'), -- active clock edge 
        (  '-', '/', '1', '-', '0', '-', '1'), -- active clock edge
        (  '-', '/', '-', '-', '0', '-', 'X'), -- active clock edge
        (  '-', '/', '-', '0', '1', '-', '0'), -- active clock edge
        (  '-', '/', '-', '1', '1', '-', '1'), -- active clock edge
        (  '-', '/', '-', '-', '1', '-', 'X'), -- active clock edge
        (  '-', '/', '0', '0', 'X', '-', '0'), -- active clock edge
        (  '-', '/', '1', '1', 'X', '-', '1'), -- active clock edge
        (  '-', '/', '-', '-', '-', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
	-- D-flip/flop with 2-input mux on D inputs with select, active-low clock
    ---------------------------------------------------------------
    CONSTANT DFFNM2_tab : VitalStateTableType  := (

        ---------INPUTS-----------|PREV-|-OUTPUT--
        -- Viol CLK   D0   D1   S | QI  | Q'    --
        --------------------------|-----|---------
        (  'X', '-', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'X', '0', '-', '0', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '-', '0', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '0', '1', '0', '0'), -- clk unknown
        (  '-', 'X', '-', '1', '1', '1', '1'), -- clk unknown
        (  '-', 'X', '0', '0', 'X', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '1', 'X', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '-', '-', '-', 'X'), -- clk unknown
        (  '-', '\', '0', '-', '0', '-', '0'), -- active clock edge 
        (  '-', '\', '1', '-', '0', '-', '1'), -- active clock edge
        (  '-', '\', '-', '-', '0', '-', 'X'), -- active clock edge
        (  '-', '\', '-', '0', '1', '-', '0'), -- active clock edge
        (  '-', '\', '-', '1', '1', '-', '1'), -- active clock edge
        (  '-', '\', '-', '-', '1', '-', 'X'), -- active clock edge
        (  '-', '\', '0', '0', 'X', '-', '0'), -- active clock edge
        (  '-', '\', '1', '1', 'X', '-', '1'), -- active clock edge
        (  '-', '\', '-', '-', '-', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
	-- D-flip/flop  
    ---------------------------------------------------------------
    CONSTANT DFF_tab : VitalStateTableType  := (

        ----INPUTS-------|PREV-|-OUTPUT--
        -- Viol CLK   D  | QI  | Q'    --
        -----------------|-----|---------
        (  'X', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'X', '0', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '-', 'X'), -- clk unknown
        (  '-', '/', '0', '-', '0'), -- active clock edge
        (  '-', '/', '1', '-', '1'), -- active clock edge
        (  '-', '/', '-', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
	-- D-flip/flop with inverting outputs 
    ---------------------------------------------------------------
    CONSTANT DFFQN_tab : VitalStateTableType  := (

        ----INPUTS-------|PREV-|-OUTPUT--
        -- Viol CLK   D  | QI  | Q'    --
        -----------------|-----|---------
        (  'X', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'X', '0', '1', '1'), -- clk unknown
        (  '-', 'X', '1', '0', '0'), -- clk unknown
        (  '-', 'X', '-', '-', 'X'), -- clk unknown
        (  '-', '/', '0', '-', '1'), -- active clock edge
        (  '-', '/', '1', '-', '0'), -- active clock edge
        (  '-', '/', '-', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
    -- D-flip/flop with active-low clock
    ---------------------------------------------------------------
    CONSTANT DFFN_tab : VitalStateTableType  := (

        ----INPUTS-------|PREV-|-OUTPUT--
        -- Viol CLK   D  | QI  | Q'    --
        -----------------|-----|---------
        (  'X', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'X', '0', '0', '0'), -- clk unknown
        (  '-', 'X', '1', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '-', 'X'), -- clk unknown
        (  '-', '\', '0', '-', '0'), -- active clock edge
        (  '-', '\', '1', '-', '1'), -- active clock edge
        (  '-', '\', '-', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
    -- D-flip/flop with active-low clock and inverting outputs
    ---------------------------------------------------------------
    CONSTANT DFFNQN_tab : VitalStateTableType  := (

        ----INPUTS-------|PREV-|-OUTPUT--
        -- Viol CLK   D  | QI  | Q'    --
        -----------------|-----|---------
        (  'X', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'X', '0', '1', '1'), -- clk unknown
        (  '-', 'X', '1', '0', '0'), -- clk unknown
        (  '-', 'X', '-', '-', 'X'), -- clk unknown
        (  '-', '\', '0', '-', '1'), -- active clock edge
        (  '-', '\', '1', '-', '0'), -- active clock edge
        (  '-', '\', '-', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    --************************************************************
    ---------------------------------------------------------------
	-- J/K Flip/flop STATE TABLES
	-- These tables have robust truth tables that function according
	-- to the previous state of the J/K f/f.
    ---------------------------------------------------------------

    ---------------------------------------------------------------
    -- J/K flip/flop with Reset active high 
    ---------------------------------------------------------------
    CONSTANT JKFFR_tab : VitalStateTableType  := (

        ----INPUTS-----------------|PREV-|-OUTPUT--
        -- Viol CLK   J    K    R  | QI  | Q'    --
        ---------------------------|-----|---------
        (  'X', '-', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'B', '-', '-', 'X', '0', '0'), -- reset unknown
        (  '-', '/', '0', '-', 'X', '0', '0'), -- reset unknown
        (  '-', '-', '-', '-', 'X', '-', 'X'), -- reset unknown
        (  '-', '-', '-', '-', '1', '-', '0'), -- reset asserted
        (  '-', 'X', '0', '-', '0', '0', '0'), -- clk unknown
        (  '-', 'X', '-', '1', '0', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '-', '0', '-', 'X'), -- clk unknown
        (  '-', '/', '0', '0', '0', '-', '0'), -- active clock edge
        (  '-', '/', '1', '1', '0', '-', '1'), -- active clock edge
        (  '-', '/', '0', '-', '0', '0', '0'), -- active clock edge
        (  '-', '/', '1', '-', '0', '0', '1'), -- active clock edge
        (  '-', '/', '-', '0', '0', '1', '0'), -- active clock edge
        (  '-', '/', '-', '1', '0', '1', '1'), -- active clock edge
        (  '-', '/', '-', '-', '0', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
    -- J/K flip/flop with Reset active high and active-low clock 
    ---------------------------------------------------------------
    CONSTANT JKFFNR_tab : VitalStateTableType  := (

        ----INPUTS-----------------|PREV-|-OUTPUT--
        -- Viol CLK   J    K    R  | QI  | Q'    --
        ---------------------------|-----|---------
        (  'X', '-', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'B', '-', '-', 'X', '0', '0'), -- reset unknown
        (  '-', '\', '0', '-', 'X', '0', '0'), -- reset unknown
        (  '-', '-', '-', '-', 'X', '-', 'X'), -- reset unknown
        (  '-', '-', '-', '-', '1', '-', '0'), -- reset asserted
        (  '-', 'X', '0', '-', '0', '0', '0'), -- clk unknown
        (  '-', 'X', '-', '1', '0', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '-', '0', '-', 'X'), -- clk unknown
        (  '-', '\', '0', '0', '0', '-', '0'), -- active clock edge
        (  '-', '\', '1', '1', '0', '-', '1'), -- active clock edge
        (  '-', '\', '0', '-', '0', '0', '0'), -- active clock edge
        (  '-', '\', '1', '-', '0', '0', '1'), -- active clock edge
        (  '-', '\', '-', '0', '0', '1', '0'), -- active clock edge
        (  '-', '\', '-', '1', '0', '1', '1'), -- active clock edge
        (  '-', '\', '-', '-', '0', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
    -- J/K flip/flop 
    ---------------------------------------------------------------
    CONSTANT JKFF_tab : VitalStateTableType  := (

        ----INPUTS------------|PREV-|-OUTPUT--
        -- Viol CLK   J    K  | QI  | Q'    --
        ----------------------|-----|---------
        (  'X', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'X', '0', '-', '0', '0'), -- clk unknown
        (  '-', 'X', '-', '1', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '-', '-', 'X'), -- clk unknown
        (  '-', '/', '0', '0', '-', '0'), -- active clock edge
        (  '-', '/', '1', '1', '-', '1'), -- active clock edge
        (  '-', '/', '0', '-', '0', '0'), -- active clock edge
        (  '-', '/', '1', '-', '0', '1'), -- active clock edge
        (  '-', '/', '-', '0', '1', '0'), -- active clock edge
        (  '-', '/', '-', '1', '1', '1'), -- active clock edge
        (  '-', '/', '-', '-', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
    -- J/K flip/flop with active-low clock
    ---------------------------------------------------------------
    CONSTANT JKFFN_tab : VitalStateTableType  := (

        ----INPUTS------------|PREV-|-OUTPUT--
        -- Viol CLK   J    K  | QI  | Q'    --
        ----------------------|-----|---------
        (  'X', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'X', '0', '-', '0', '0'), -- clk unknown
        (  '-', 'X', '-', '1', '1', '1'), -- clk unknown
        (  '-', 'X', '-', '-', '-', 'X'), -- clk unknown
        (  '-', '\', '0', '0', '-', '0'), -- active clock edge
        (  '-', '\', '1', '1', '-', '1'), -- active clock edge
        (  '-', '\', '0', '-', '0', '0'), -- active clock edge
        (  '-', '\', '1', '-', '0', '1'), -- active clock edge
        (  '-', '\', '-', '0', '1', '0'), -- active clock edge
        (  '-', '\', '-', '1', '1', '1'), -- active clock edge
        (  '-', '\', '-', '-', '-', 'X'), -- active clock edge
        (  '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    --************************************************************
    ---------------------------------------------------------------
	-- Other flip/flop STATE TABLES
    ---------------------------------------------------------------
    --************************************************************

    ---------------------------------------------------------------
    -- Toggle flip/flop with no reset (unknown state -> 0 on clock)
	-- Used where user doesn't care about phase of output, such as
	-- for clock dividers
    ---------------------------------------------------------------
    CONSTANT TFF_tab : VitalStateTableType  := (

        ----INPUTS--|PREV-|-OUTPUT--
        -- Viol CLK | QI  | Q'    --
        ------------|-----|---------
        (  'X', '-', '-', 'X'), -- timing violation
        (  '-', 'X', '-', 'X'), -- clk unknown
        (  '-', '/', 'X', '0'), -- active clock edge
        (  '-', '/', '0', '1'), -- active clock edge
        (  '-', '/', '1', '0'), -- active clock edge
        (  '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
    -- Toggle flip/flop with Reset active high
    ---------------------------------------------------------------
    CONSTANT TFFR_tab : VitalStateTableType  := (

        ----INPUTS-------|PREV-|-OUTPUT--
        -- Viol CLK   R  | QI  | Q'    --
        -----------------|-----|---------
        (  'X', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'B', 'X', '0', '0'), -- reset unknown 
        (  '-', '/', 'X', '1', '0'), -- reset unknown 
        (  '-', '-', 'X', '-', 'X'), -- reset unknown 
        (  '-', '-', '1', '-', '0'), -- reset active 
        (  '-', 'X', '0', '-', 'X'), -- clk unknown
        (  '-', '/', '0', 'X', 'X'), -- active clock edge
        (  '-', '/', '0', '0', '1'), -- active clock edge
        (  '-', '/', '0', '1', '0'), -- active clock edge
        (  '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
    -- Toggle flip/flop with Set and Reset both active high
    ---------------------------------------------------------------
    CONSTANT TFFSR_tab : VitalStateTableType  := (

        ----INPUTS-----------|PREV-|-OUTPUT--
        -- Viol CLK   S   R  | QI  | Q'    --
        ---------------------|-----|---------
        (  'X', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'B', 'X', '0', '1', '1'), -- set unknown 
        (  '-', '/', 'X', '0', '0', '1'), -- set unknown 
        (  '-', '-', 'X', '-', '-', 'X'), -- set unknown 
        (  '-', 'B', '0', 'X', '0', '0'), -- reset unknown 
        (  '-', '/', '0', 'X', '1', '0'), -- reset unknown 
        (  '-', '-', '-', 'X', '-', 'X'), -- reset unknown 
        (  '-', '-', '1', '1', '-', 'X'), -- both asserted 
		(  '-', '-', '1', '0', '-', '1'), -- set asserted 
		(  '-', '-', '0', '1', '-', '0'), -- reset asserted 
        (  '-', 'X', '0', '0', '-', 'X'), -- clk unknown
        (  '-', '/', '0', '0', 'X', 'X'), -- active clock edge
        (  '-', '/', '0', '0', '0', '1'), -- active clock edge
        (  '-', '/', '0', '0', '1', '0'), -- active clock edge
        (  '-', '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
    -- Simple register without previous states
    ---------------------------------------------------------------
    CONSTANT DREG_tab : VitalStateTableType  := (

        ----INPUTS-------|-OUTPUT--
        -- Viol CLK   D  | Q'    --
        -----------------|---------
        (  'X', '-', '-', 'X'), -- timing violation
        (  '-', 'X', '-', 'X'), -- clk unknown
        (  '-', '/', '0', '0'), -- active clock edge
        (  '-', '/', '1', '1'), -- active clock edge
        (  '-', '/', '-', 'X'), -- active clock edge
        (  '-', '-', '-', 'S')  -- default
 
        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
    -- Simple register without previous states, inverting outputs
    ---------------------------------------------------------------
    CONSTANT DREGQN_tab : VitalStateTableType  := (

        ----INPUTS-------|-OUTPUT--
        -- Viol CLK   D  | Q'    --
        -----------------|---------
        (  'X', '-', '-', 'X'), -- timing violation
        (  '-', 'X', '-', 'X'), -- clk unknown
        (  '-', '/', '0', '1'), -- active clock edge
        (  '-', '/', '1', '0'), -- active clock edge
        (  '-', '/', '-', 'X'), -- active clock edge
        (  '-', '-', '-', 'S')  -- default
 
        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------
    -- Latch
	---------------------------------------------------------------
    CONSTANT LAT_tab : VitalStateTableType  := (

        ----INPUTS-------|PREV-|-OUTPUT--
        -- Viol LEN   D  | QI  | Q'    --
        -----------------|-----|---------
        (  'X', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'X', '0', '0', '0'), -- len unknown
        (  '-', 'X', '1', '1', '1'), -- len unknown
        (  '-', 'X', '-', '-', 'X'), -- len unknown
        (  '-', '1', '0', '-', '0'), -- Latch enabled
        (  '-', '1', '1', '-', '1'), -- Latch enabled
        (  '-', '1', '-', '-', 'X'), -- Latch enabled unknown D
        (  '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition
 
    ---------------------------------------------------------------
    -- Latch with inverting outputs
    ---------------------------------------------------------------
    CONSTANT LATQN_tab : VitalStateTableType  := (

        ----INPUTS-------|PREV-|-OUTPUT--
        -- Viol LEN   D  | QI  | Q'    --
        -----------------|-----|---------
        (  'X', '-', '-', '-', 'X'), -- timing violation
        (  '-', 'X', '0', '1', '1'), -- len unknown
        (  '-', 'X', '1', '0', '0'), -- len unknown
        (  '-', 'X', '-', '-', 'X'), -- len unknown
        (  '-', '1', '0', '-', '1'), -- Latch enabled
        (  '-', '1', '1', '-', '0'), -- Latch enabled
        (  '-', '1', '-', '-', 'X'), -- Latch enabled unknown D
        (  '-', '-', '-', '-', 'S')  -- default

        ); -- end of VitalStateTableType definition

	---------------------------------------------------------------
	-- Latch with Reset active high
	---------------------------------------------------------------
    CONSTANT LATR_tab : VitalStateTableType  := (
 
        ---------INPUTS------|PREV-|-OUTPUT--
        -- Viol LEN   D    R | QI  | Q'    --
        ---------------------|-----|---------
        (  'X', '-', '-', '-', '-', 'X'), -- timing violation
        (  '-', '1', '0', 'X', '0', '0'), -- reset unknown
        (  '-', '0', '-', 'X', '0', '0'), -- reset unknown
        (  '-', '-', '-', 'X', '-', 'X'), -- reset unknown
        (  '-', '-', '-', '1', '-', '0'), -- reset asserted
        (  '-', 'X', '0', '0', '0', '0'), -- len unknown
        (  '-', 'X', '1', '0', '1', '1'), -- len unknown
        (  '-', 'X', '-', '0', '-', 'X'), -- len unknown
        (  '-', '1', '0', '0', '-', '0'), -- Latch enabled
        (  '-', '1', '1', '0', '-', '1'), -- Latch enabled
        (  '-', '1', '-', '0', '-', 'X'), -- Latch enabled, unknown D
        (  '-', '-', '-', '-', '-', 'S')  -- default
 
        ); -- end of VitalStateTableType definition

    ---------------------------------------------------------------------------

END ff_package;
