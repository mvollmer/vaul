-- ---------------------------------------------------------------
--   File name : nlb6211.vhd
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
--      V1.0            rev3        95 NOV 17   Initial release
-- ---------------------------------------------------------------
--   PART DESCRIPTION :
--
--   Library:       NLB
--   Technology:    ECL
--   Part:          NLB6211
--
--   Description:   Triple 2 Fan-Out Buffer
--
------------------------------------------------------------------

LIBRARY IEEE;   USE IEEE.std_logic_1164.ALL;
                USE IEEE.VITAL_primitives.all;
                USE IEEE.VITAL_timing.all;
LIBRARY FMF;    USE FMF.ecl_package.all;

------------------------------------------------------------------
--   ENTITY DECLARATION
------------------------------------------------------------------
ENTITY nlb6211 IS
    GENERIC (
    -- tipd delays: interconnect path delays
            tipd_D          : VitalDelayType01 := (0 ns, 0 ns);
            tipd_ENeg       : VitalDelayType01 := (0 ns, 0 ns);
    -- tpd delays: typical values default
            tpd_D_Q         : VitalDelayType01 := (1 ns, 1 ns);
            tpd_ENeg_Q      : VitalDelayType01 := (1 ns, 1 ns);
    -- generic control parameters
            XGenerationOn   : Boolean  := TRUE;
			TimingModel		: STRING := "UNIT";
            InstancePath    : STRING  := "*" );
    PORT (
            -- 0 denotes pull-down resistor
            D               : IN   STD_LOGIC := '0';
            ENeg            : IN   STD_LOGIC := '0';
            QA              : OUT   STD_LOGIC := 'U';
            QB              : OUT   STD_LOGIC := 'U';
            QANeg           : OUT   STD_LOGIC := 'U';
            QBNeg           : OUT   STD_LOGIC := 'U'
		);
    ATTRIBUTE VITAL_LEVEL0 of nlb6211 : ENTITY IS TRUE;
END nlb6211;

------------------------------------------------------------------
--   ARCHITECTURE DECLARATION
------------------------------------------------------------------
ARCHITECTURE vhdl_behavioral of nlb6211 IS
    ATTRIBUTE VITAL_LEVEL1 of vhdl_behavioral : ARCHITECTURE IS TRUE;

    SIGNAL D_ipd            : std_ulogic := 'X';
    SIGNAL ENeg_ipd         : std_ulogic := 'X';
    SIGNAL Qint             : std_ulogic := 'X';

BEGIN

    ---------------------------------------------------------------
    -- Wire delays
    ---------------------------------------------------------------
    WireDelay : BLOCK
    BEGIN   

        w1: VitalWireDelay (D_ipd, D, tipd_D);
        w2: VitalWireDelay (ENEg_ipd, ENeg, tipd_ENeg);

    END BLOCK;

    --------------------------------------------------------------- 
    -- Concurrent procedure calls 
    --------------------------------------------------------------- 
    a1: VitalOR2 (q => Qint, a => D_ipd, b => ENeg_ipd,
                  tpd_a_q => tpd_D_Q,
                  tpd_b_q => tpd_ENeg_Q);

    a2: VitalBUF (q => QA, a => Qint, ResultMap => ECL_wired_or_rmap);
    a3: VitalBUF (q => QB, a => Qint, ResultMap => ECL_wired_or_rmap);
    a4: VitalINV (q => QANeg, a => Qint, ResultMap => ECL_wired_or_rmap);
    a5: VitalINV (q => QBNeg, a => Qint, ResultMap => ECL_wired_or_rmap);

END vhdl_behavioral;
