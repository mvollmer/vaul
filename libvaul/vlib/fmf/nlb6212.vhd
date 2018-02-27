-- ---------------------------------------------------------------
--   File name : nlb6212.vhd
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
--   Part:          NLB6212
--
--   Description:   1 to 10 clock distributor with enable
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
ENTITY nlb6212 IS
    GENERIC (
    -- tipd delays: interconnect path delays
			tipd_EN1Neg     : VitalDelayType01 := (0 ns, 0 ns);
            tipd_EN2Neg     : VitalDelayType01 := (0 ns, 0 ns);
            tipd_EN3Neg     : VitalDelayType01 := (0 ns, 0 ns);
            tipd_CLK        : VitalDelayType01 := (0 ns, 0 ns);
            tipd_CLKNeg     : VitalDelayType01 := (0 ns, 0 ns);
    -- tpd delays:
            tpd_CLK_Q       : VitalDelayType01 := (1 ns, 1 ns);
    -- tsetup values: setup times, typical values default
            tsetup_ENNeg_CLK: VitalDelayType := 1 ns;
    -- thold values: hold times, typical values default
            thold_ENNeg_CLK : VitalDelayType :=  1 ns;
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
    ATTRIBUTE VITAL_LEVEL0 of nlb6212 : ENTITY IS TRUE;
END nlb6212;

------------------------------------------------------------------
--   ARCHITECTURE DECLARATION
------------------------------------------------------------------
ARCHITECTURE vhdl_behavioral OF nlb6212 IS
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
	a1: VITALBUF (q => Q0, a => Qint, ResultMap => ECL_wired_or_rmap);
	a2: VITALBUF (q => Q1, a => Qint, ResultMap => ECL_wired_or_rmap);
	a3: VITALBUF (q => Q2, a => Qint, ResultMap => ECL_wired_or_rmap);
	a4: VITALBUF (q => Q3, a => Qint, ResultMap => ECL_wired_or_rmap);
	a5: VITALBUF (q => Q4, a => Qint, ResultMap => ECL_wired_or_rmap);
	a6: VITALBUF (q => Q5, a => Qint, ResultMap => ECL_wired_or_rmap);
	a7: VITALBUF (q => Q6, a => Qint, ResultMap => ECL_wired_or_rmap);
	a8: VITALBUF (q => Q7, a => Qint, ResultMap => ECL_wired_or_rmap);
	a9: VITALBUF (q => Q8, a => Qint, ResultMap => ECL_wired_or_rmap);
	a10: VITALBUF (q => Q9, a => Qint, ResultMap => ECL_wired_or_rmap);
	a11: VITALINV (q => Q0Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a12: VITALINV (q => Q1Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a13: VITALINV (q => Q2Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a14: VITALINV (q => Q3Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a15: VITALINV (q => Q4Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a16: VITALINV (q => Q5Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a17: VITALINV (q => Q6Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a18: VITALINV (q => Q7Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a19: VITALINV (q => Q8Neg, a => Qint, ResultMap => ECL_wired_or_rmap);
	a20: VITALINV (q => Q9Neg, a => Qint, ResultMap => ECL_wired_or_rmap);

	-- DeMorgan equiv of 3-input active low input, active high output OR gate
    a21: VITALNAND3 (q => Eint,
					 a => EN1Neg_ipd, b => EN2Neg_ipd, c => EN3Neg_ipd);

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
        VARIABLE PrevData		: std_logic_vector(1 TO 3);
        VARIABLE Results		: std_logic_vector(1 TO 1) := (OTHERS=>'X');
		ALIAS Q0_zd				: std_ulogic IS Results(1);

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
				TestSignalName	=> "EN1Neg",
                RefSignal		=> CLKint,
				RefSignalName	=> "CLK",
                SetupHigh		=> tsetup_ENNeg_CLK,
                SetupLow		=> tsetup_ENNeg_CLK,
                HoldHigh		=> thold_ENNeg_CLK,
                HoldLow			=> thold_ENNeg_CLK,
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
			StateTable     => DREG_tab,
        	DataIn         => (Violation, CLKint, Eint),
        	NumStates      => 0,
        	Result         => Results, --> Q0_zd
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
