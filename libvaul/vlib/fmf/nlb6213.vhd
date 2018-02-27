-- ---------------------------------------------------------------
--   File name : nlb6213.vhd
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
--   Part:          NLB6213
--
--   Description:   1 to 10 clock distributor with enable d flip/flop
--
------------------------------------------------------------------

LIBRARY IEEE;   USE IEEE.std_logic_1164.ALL;
                USE IEEE.VITAL_primitives.all;
                USE IEEE.VITAL_timing.all;
LIBRARY FMF;    USE FMF.ecl_package.all;
				USE FMF.ff_package.all;

------------------------------------------------------------------
--   ENTITY DECLARATION
------------------------------------------------------------------
ENTITY nlb6213 IS
    GENERIC (
    -- tipd delays: interconnect path delays
			tipd_EN1Neg     : VitalDelayType01 := (0 ns, 0 ns);
            tipd_EN2Neg     : VitalDelayType01 := (0 ns, 0 ns);
            tipd_EN3Neg     : VitalDelayType01 := (0 ns, 0 ns);
            tipd_CLK        : VitalDelayType01 := (0 ns, 0 ns);
            tipd_CLKNeg     : VitalDelayType01 := (0 ns, 0 ns);
    -- tpd delays: typical values default
            tpd_CLK_Q       : VitalDelayType01 := (1 ns, 1 ns);
    -- tsetup values: setup times, typical values default
            tsetup_EN_CLK   : VitalDelayType := 1 ns;
    -- thold values: hold times, typical values default
            thold_EN_CLK    : VitalDelayType :=  1 ns;
    -- generic control parameters
			TimingChecksOn  : BOOLEAN := FALSE;
			XGenerationOn	: Boolean := TRUE;
			TimingModel		: STRING := "UNIT";
           	InstancePath    : STRING := "*"  );
    PORT (
			-- 0 denotes pull-down resistor, 1 pull-up
            EN1Neg			: IN    std_logic := '0';
            EN2Neg			: IN    std_logic := '0';
            EN3Neg			: IN    std_logic := '0';
            CLK				: IN    std_logic := '0';
            CLKNeg			: IN    std_logic := '1'; 
            Q0				: OUT   std_logic := 'U';
            Q0Neg			: OUT   std_logic := 'U';
            Q1				: OUT   std_logic := 'U';
            Q1Neg			: OUT   std_logic := 'U';
            Q2				: OUT   std_logic := 'U';
            Q2Neg			: OUT   std_logic := 'U';
            Q3				: OUT   std_logic := 'U';
            Q3Neg			: OUT   std_logic := 'U';
            Q4				: OUT   std_logic := 'U';
            Q4Neg			: OUT   std_logic := 'U';
            Q5				: OUT   std_logic := 'U';
            Q5Neg			: OUT   std_logic := 'U';
            Q6				: OUT   std_logic := 'U';
            Q6Neg			: OUT   std_logic := 'U';
            Q7				: OUT   std_logic := 'U';
            Q7Neg			: OUT   std_logic := 'U';
            Q8				: OUT   std_logic := 'U';
            Q8Neg			: OUT   std_logic := 'U';
            Q9				: OUT   std_logic := 'U';
            Q9Neg			: OUT   std_logic := 'U'; 
			VBB				: OUT   std_logic := 'W' );
    ATTRIBUTE VITAL_LEVEL0 of nlb6213 : ENTITY IS TRUE;
END nlb6213;

------------------------------------------------------------------
--   ARCHITECTURE DECLARATION
------------------------------------------------------------------
ARCHITECTURE vhdl_behavioral OF nlb6213 IS
    ATTRIBUTE VITAL_LEVEL1 of vhdl_behavioral : ARCHITECTURE IS TRUE;

    SIGNAL EN1Neg_ipd		: std_ulogic := 'X';
    SIGNAL EN2Neg_ipd       : std_ulogic := 'X';
    SIGNAL EN3Neg_ipd       : std_ulogic := 'X';
    SIGNAL CLK_ipd          : std_ulogic := 'X';
    SIGNAL CLKNeg_ipd       : std_ulogic := 'X';
	SIGNAL CLKint			: std_ulogic := 'X';
	SIGNAL Eint				: std_ulogic := 'X';
	SIGNAL Qint				: std_ulogic := 'X';

BEGIN

    ---------------------------------------------------------------
    -- Wire delays
    ---------------------------------------------------------------
    WireDelay : BLOCK
    BEGIN
 
	    w1: VitalWireDelay (EN1Neg_ipd, EN1Neg, tipd_EN1Neg);
   	    w2: VitalWireDelay (EN2Neg_ipd, EN2Neg, tipd_EN2Neg);
       	w3: VitalWireDelay (EN3Neg_ipd, EN3Neg, tipd_EN3Neg);
       	w4: VitalWireDelay (CLK_ipd, CLK, tipd_CLK);
       	w5: VitalWireDelay (CLKNeg_ipd, CLKNeg, tipd_CLKNeg);

    END BLOCK;

    ---------------------------------------------------------------
    -- Concurrent procedure calls
    ---------------------------------------------------------------
	a1: VitalBUF (q => Q0, a => Qint, ResultMap => ECL_wired_or_rmap);
	a2: VitalBUF (q => Q1, a => Qint, ResultMap => ECL_wired_or_rmap);
	a3: VitalBUF (q => Q2, a => Qint, ResultMap => ECL_wired_or_rmap);
	a4: VitalBUF (q => Q3, a => Qint, ResultMap => ECL_wired_or_rmap);
	a5: VitalBUF (q => Q4, a => Qint, ResultMap => ECL_wired_or_rmap);
	a6: VitalBUF (q => Q5, a => Qint, ResultMap => ECL_wired_or_rmap);
	a7: VitalBUF (q => Q6, a => Qint, ResultMap => ECL_wired_or_rmap);
	a8: VitalBUF (q => Q7, a => Qint, ResultMap => ECL_wired_or_rmap);
	a9: VitalBUF (q => Q8, a => Qint, ResultMap => ECL_wired_or_rmap);
	a10: VitalBUF (q => Q9, a => Qint, ResultMap => ECL_wired_or_rmap);
	a11: VitalINV (q => Q0Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a12: VitalINV (q => Q1Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a13: VitalINV (q => Q2Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a14: VitalINV (q => Q3Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a15: VitalINV (q => Q4Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a16: VitalINV (q => Q5Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a17: VitalINV (q => Q6Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a18: VitalINV (q => Q7Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a19: VitalINV (q => Q8Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a20: VitalINV (q => Q9Neg, a => Qint, ResultMap => ECL_wired_or_rmap);

    a21: VitalOR3 (q => Eint, a => EN1Neg_ipd, b => EN2Neg_ipd, c => EN3Neg_ipd);

    ---------------------------------------------------------------
    -- ECL Clock Process
    ---------------------------------------------------------------
    ECLclock : PROCESS (CLK_ipd, CLKNeg_ipd)

        -- Functionality Results Variables
        VARIABLE Mode           : X01 := 'X';
        VARIABLE CLKint_zd      : std_ulogic := 'X';
        VARIABLE PrevData       : std_logic_vector(1 to 4);

        -- Output Glitch Detection Variables
        VARIABLE CLK_GlitchData : VitalGlitchDataType;
 
    BEGIN
 
        --------------------------------------------------------------
        -- Functionality Section
        --------------------------------------------------------------
        Mode := ECL_diff_mode_tab(CLK_ipd, CLKNeg_ipd);

        VitalStateTable (
            StateTable      => ECL_clk_tab,
            DataIn          => (CLK_ipd, CLKNeg_ipd, Mode ),
            Result          => CLKint_zd,
            PreviousDataIn  => PrevData );
 
        --------------------------------------------------------------
        -- (Dummy) Path Delay Section
        --------------------------------------------------------------
        VitalPathDelay (
            OutSignal       => CLKint,
            OutSignalName   => "CLKint",
            OutTemp         => CLKint_zd,
            Paths           => ( 0 => (0 ps, VitalZeroDelay, FALSE)),
            GlitchData      => CLK_GlitchData );

    END PROCESS;
 
    ---------------------------------------------------------------
    -- Behavior Process
    ---------------------------------------------------------------
    VITALBehavior : PROCESS (Eint, CLKint)

        -- Timing Check Variables
        VARIABLE Tviol_E_CLK	: X01 := '0';
        VARIABLE TD_E_CLK		: VitalTimingDataType;

        VARIABLE Violation		: X01 := '0';

        -- Functionality Results Variables
        VARIABLE PrevData		: std_logic_vector(1 to 3);
		VARIABLE Q0_zd			: std_ulogic := 'X';

		VARIABLE Qint_zd		: std_ulogic := 'X';

        -- Output Glitch Detection Variables
        VARIABLE Qint_GlitchData : VitalGlitchDataType;

    BEGIN

        ---------------------------------------------------------------
        -- Timing Check Section
        ---------------------------------------------------------------
        IF (TimingChecksOn) THEN

            VitalSetupHoldCheck ( 
                TestSignal		=> Eint,
				TestSignalName	=> "Eint",
                RefSignal		=> CLKint,
				RefSignalName	=> "CLKint",
                SetupHigh		=> tsetup_EN_CLK,
                SetupLow		=> tsetup_EN_CLK,
                HoldHigh		=> thold_EN_CLK,
                HoldLow			=> thold_EN_CLK,
                CheckEnabled	=> TRUE,
                RefTransition	=> '/',
                HeaderMsg		=> InstancePath,
                TimingData		=> TD_E_CLK,
                Violation		=> Tviol_E_CLK );

        END IF; -- End of Timing Check Section

        --------------------------------------------------------------
        -- Functionality Section
        --------------------------------------------------------------
        Violation := Tviol_E_CLK;

        VitalStateTable (
			StateTable     => DFFQN_tab,
        	DataIn         => (Violation, CLKint, Eint),
        	Result         => Q0_zd,
        	PreviousDataIn => PrevData );

		Qint_zd := VitalAND2 (a => CLKint, b => Q0_zd);

        --------------------------------------------------------------
        -- Path Delay Section
        --------------------------------------------------------------
        VitalPathDelay01 ( 
			OutSignal       =>  Qint,
			OutSignalName   =>  "Qint",
			OutTemp			=>  Qint_zd, 
            Paths           => (
				0 => (InputChangeTime	=> CLKint'LAST_EVENT,
					  PathDelay			=> tpd_CLK_Q,
					  PathCondition		=> TRUE ) ),
            GlitchData		=> Qint_GlitchData );

    END PROCESS;

END vhdl_behavioral;
