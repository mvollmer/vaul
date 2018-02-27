-- ---------------------------------------------------------------
--   File name : nlb6214.vhd
-- ---------------------------------------------------------------
--  Copyright (C) 1995 Free Model Foundation
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License version 2 as
--  published by the Free Software Foundation.
--
--   MODIFICATION HISTORY :
--
--      version no: |   author: |   mod. date: |    changes made:
--      V1.0            rev3        95 SEP 23   Initial release
--      V1.1            rev3        95 NOV 17   changed "_N" to "Neg" in
--                                              port names
--      V1.2         R. Munden      96 FEB 22   Added VBB port
-- ---------------------------------------------------------------
--   PART DESCRIPTION :
--
--   Library:       NLB
--   Technology:    ECL
--   Part:          NLB6214
--
--   Description:   Quint Line Receiver
--
------------------------------------------------------------------

LIBRARY IEEE;   USE IEEE.std_logic_1164.ALL;
                USE IEEE.VITAL_primitives.all;
                USE IEEE.VITAL_timing.all;
LIBRARY FMF;    USE FMF.ecl_package.all;

------------------------------------------------------------------
--   ENTITY DECLARATION
------------------------------------------------------------------
ENTITY nlb6214 IS
    GENERIC (
    -- tipd delays: interconnect path delays
            tipd_A          : VitalDelayType01 := (0 ns, 0 ns);
            tipd_ANeg       : VitalDelayType01 := (0 ns, 0 ns);
    -- tpd delays: typical values default
            tpd_A_Y         : VitalDelayType01 := (1 ns, 1 ns);
    -- generic control parameters
            XGenerationOn   : Boolean  := TRUE;
            TimingModel     : STRING := "UNIT";
            InstancePath    : STRING  := "*" );
    port (    
            -- 0 denotes pull-down resistor
            A               : IN   STD_LOGIC := '0';
            ANeg            : IN   STD_LOGIC := '0';
            Y1              : OUT   STD_LOGIC := 'U';
            Y1Neg           : OUT   STD_LOGIC := 'U';
            VBB             : OUT  STD_LOGIC := 'W' );
    ATTRIBUTE VITAL_LEVEL0 of nlb6214 : ENTITY IS TRUE;
END nlb6214;

------------------------------------------------------------------
--   ARCHITECTURE DECLARATION
------------------------------------------------------------------
ARCHITECTURE vhdl_behavioral of nlb6214 IS
    ATTRIBUTE VITAL_LEVEL1 of vhdl_behavioral : ARCHITECTURE IS TRUE;

    SIGNAL A_ipd            : std_ulogic := 'X';
    SIGNAL ANeg_ipd         : std_ulogic := 'X';
	SIGNAL Aint				: std_ulogic := 'X';
 
BEGIN

    ---------------------------------------------------------------
    -- Wire delays
    ---------------------------------------------------------------
    WireDelay : BLOCK
    BEGIN
 
         w1: VitalWireDelay (A_ipd, A, tipd_A);
         w2: VitalWireDelay (ANeg_ipd, ANeg, tipd_ANeg);

    END BLOCK;

    ---------------------------------------------------------------
    -- Concurrent procedure calls
    ---------------------------------------------------------------
	a1: VitalINV (q => Y1Neg, a => Aint, tpd_a_q => tpd_A_Y,
				  ResultMap => ECL_wired_or_rmap);

	a2: VitalBUF (q => Y1, a => Aint, tpd_a_q => tpd_A_Y,
				  ResultMap => ECL_wired_or_rmap);

    ---------------------------------------------------------------
    -- Behavior Process
    ---------------------------------------------------------------
    ECLclock : PROCESS (A_ipd, ANeg_ipd)

        -- Functionality Results Variables
        VARIABLE Mode           : X01 := 'X';
        VARIABLE Aint_zd      	: std_ulogic := 'X';
        VARIABLE PrevData		: std_logic_vector(1 to 3);
 
        -- Output Glitch Detection Variables
        VARIABLE A_GlitchData	: VitalGlitchDataType;
 
    BEGIN
 
        --------------------------------------------------------------
        -- Functionality Section
        --------------------------------------------------------------
        Mode := ECL_diff_mode_tab(A_ipd, ANeg_ipd);

        VitalStateTable (
            StateTable      => ECL_clk_tab,
            DataIn          => (A_ipd, ANeg_ipd, Mode ),
            Result          => Aint_zd,
            PreviousDataIn  => PrevData );
 
        --------------------------------------------------------------
        -- (Dummy) Path Delay Section
        --------------------------------------------------------------
        VitalPathDelay (
            OutSignal       =>  Aint,
            OutSignalName   =>  "Aint",
            OutTemp         =>  Aint_zd,
            Paths           => ( 0 => (0 ps, VitalZeroDelay, FALSE)),
            GlitchData      => A_GlitchData );
 
    END PROCESS;

END vhdl_behavioral;
