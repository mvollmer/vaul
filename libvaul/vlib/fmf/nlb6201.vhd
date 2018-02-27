-- ---------------------------------------------------------------
--   File name : nlb6201.vhd
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
--	    V1.1.1			R. Munden	96 Jan 31	Added TimingModel generic
-- ---------------------------------------------------------------
--   PART DESCRIPTION :
--
--   Library:       NLB
--   Technology:    ECL
--   Part:          NLB6201
--
--   Description:   3-input OR gate with complementary NOR output
--
------------------------------------------------------------------

LIBRARY IEEE;   USE IEEE.std_logic_1164.ALL;
                USE IEEE.VITAL_primitives.all;
                USE IEEE.VITAL_timing.all;
LIBRARY FMF;    USE FMF.ecl_package.all;

------------------------------------------------------------------
--   ENTITY DECLARATION
------------------------------------------------------------------
ENTITY nlb6201 IS
    GENERIC (
    -- tipd delays: interconnect path delays
            tipd_A          : VitalDelayType01 := (0 ns, 0 ns);
            tipd_B          : VitalDelayType01 := (0 ns, 0 ns);
            tipd_C          : VitalDelayType01 := (0 ns, 0 ns);
    -- tpd delays: typical values default
            tpd_A_Y         : VitalDelayType01 := (1 ns, 1 ns);
            tpd_B_Y         : VitalDelayType01 := (1 ns, 1 ns);
            tpd_C_Y         : VitalDelayType01 := (1 ns, 1 ns);
    -- generic control parameters
            XGenerationOn   : Boolean  := TRUE;
            TimingModel     : STRING := "UNIT";
            InstancePath    : STRING  := "*" );
    port (    
            -- 0 denotes pull-down resistor
            A               : IN   STD_LOGIC := '0';
            B               : IN   STD_LOGIC := '0';
            C               : IN   STD_LOGIC := '0';
            Y               : OUT   STD_LOGIC := 'U';
            YNeg            : OUT   STD_LOGIC := 'U'  );
    ATTRIBUTE VITAL_LEVEL0 of nlb6201 : ENTITY IS TRUE;
END nlb6201;

------------------------------------------------------------------
--   ARCHITECTURE DECLARATION
------------------------------------------------------------------
ARCHITECTURE vhdl_behavioral of nlb6201 IS
    ATTRIBUTE VITAL_LEVEL1 of vhdl_behavioral : ARCHITECTURE IS TRUE;

    SIGNAL A_ipd            : std_ulogic := 'X';
    SIGNAL B_ipd            : std_ulogic := 'X';
    SIGNAL C_ipd            : std_ulogic := 'X';

BEGIN

    ---------------------------------------------------------------
    -- Wire delays
    ---------------------------------------------------------------
    WireDelay : BLOCK
    BEGIN
 
         w1: VitalWireDelay (A_ipd, A, tipd_A);
         w2: VitalWireDelay (B_ipd, B, tipd_B);
         w3: VitalWireDelay (C_ipd, C, tipd_C);

    END BLOCK;

    ---------------------------------------------------------------
    -- Concurrent procedure calls
    ---------------------------------------------------------------
    a1: VitalOR3 (q => Y, a => A_ipd, b => B_ipd, c => C_ipd,
                  tpd_a_q => tpd_A_Y,
                  tpd_b_q => tpd_B_Y,
                  tpd_c_q => tpd_C_Y,
                  ResultMap => ECL_wired_or_rmap);

    a2: VitalNOR3 (q => YNeg, a => A_ipd, b => B_ipd, c => C_ipd,
                   tpd_a_q => tpd_A_Y,
                   tpd_b_q => tpd_B_Y,
                   tpd_c_q => tpd_C_Y, 
                   ResultMap => ECL_wired_or_rmap);

END vhdl_behavioral;
