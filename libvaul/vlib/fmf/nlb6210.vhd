-- ---------------------------------------------------------------
--   File name : nlb6210.vhd
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
-- ---------------------------------------------------------------
--   PART DESCRIPTION :
--
--   Library:       NLB
--   Technology:    ECL
--   Part:          NLB6210
--
--   Description:   4 fan-out buffer with ENABLE and complementary
--                  outputs
--
------------------------------------------------------------------

LIBRARY IEEE;   USE IEEE.std_logic_1164.ALL;
                USE IEEE.VITAL_primitives.all;
                USE IEEE.VITAL_timing.all;
LIBRARY FMF;    USE FMF.ecl_package.all;

------------------------------------------------------------------
--   ENTITY DECLARATION
------------------------------------------------------------------
ENTITY nlb6210 IS
    GENERIC (
    -- tipd delays: interconnect path delays
            tipd_D          : VitalDelayType01 := (0 ns, 0 ns);
            tipd_ENeg       : VitalDelayType01 := (0 ns, 0 ns);
    -- tpd delays: 
            tpd_D_Q         : VitalDelayType01 := (1 ns, 1 ns);
            tpd_ENeg_Q      : VitalDelayType01 := (1 ns, 1 ns);
    -- generic control parameters
            XGenerationOn   : Boolean  := TRUE;
            InstancePath    : STRING  := "*" );
    PORT (
            -- 0 denotes pull-down resistor
            D               : IN   STD_LOGIC := '0';
            ENeg            : IN   STD_LOGIC := '0';
            QA              : OUT   STD_LOGIC := 'U';
            QB              : OUT   STD_LOGIC := 'U';
            QC              : OUT   STD_LOGIC := 'U';
            QD              : OUT   STD_LOGIC := 'U';
            QANeg           : OUT   STD_LOGIC := 'U';
            QBNeg           : OUT   STD_LOGIC := 'U';
            QCNeg           : OUT   STD_LOGIC := 'U';
            QDNeg           : OUT   STD_LOGIC := 'U'  );
    ATTRIBUTE VITAL_LEVEL0 of nlb6210 : ENTITY IS TRUE;
END nlb6210;

------------------------------------------------------------------
--   ARCHITECTURE DECLARATION
------------------------------------------------------------------
ARCHITECTURE vhdl_behavioral of nlb6210 IS
    ATTRIBUTE VITAL_LEVEL1 of vhdl_behavioral : ARCHITECTURE IS TRUE;

    SIGNAL D_ipd            : std_ulogic := 'X';
    SIGNAL ENeg_ipd         : std_ulogic := 'X';
    SIGNAL Qint    	        : std_ulogic := 'X';

BEGIN

    ---------------------------------------------------------------
    -- Wire delays
    ---------------------------------------------------------------
    WireDelay : BLOCK
    BEGIN   

        w1: VitalWireDelay (D_ipd, D, tipd_D);
        w2: VitalWireDelay (ENeg_ipd, ENeg, tipd_ENeg);

    END BLOCK;

    --------------------------------------------------------------- 
    -- Concurrent procedure calls 
    --------------------------------------------------------------- 
    a1: VitalOR2 (q => Qint, a => D_ipd, b => ENeg_ipd,
                  tpd_a_q => tpd_D_Q,
                  tpd_b_q => tpd_ENeg_Q);

    a2: VitalBUF (q => QA, a => Qint, ResultMap => ECL_wired_or_rmap);
    a3: VitalBUF (q => QB, a => Qint, ResultMap => ECL_wired_or_rmap);
    a4: VitalBUF (q => QC, a => Qint, ResultMap => ECL_wired_or_rmap);
    a5: VitalBUF (q => QD, a => Qint, ResultMap => ECL_wired_or_rmap);
    a6: VitalINV (q => QANeg, a => Qint, ResultMap => ECL_wired_or_rmap);
    a7: VitalINV (q => QBNeg, a => Qint, ResultMap => ECL_wired_or_rmap);
    a8: VitalINV (q => QCNeg, a => Qint, ResultMap => ECL_wired_or_rmap);
    a9: VitalINV (q => QDNeg, a => Qint, ResultMap => ECL_wired_or_rmap);

END vhdl_behavioral;
