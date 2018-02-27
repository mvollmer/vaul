-- ---------------------------------------------------------------
--   File name : nlb6241.vhd
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
--   Part:          nlb6241
--
--   Description:   4-bit ripple counter with enable
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
ENTITY nlb6241 IS
    GENERIC (
    -- tipd delays: interconnect path delays
            tipd_ENeg		: VitalDelayType01 := (0 ns, 0 ns);
            tipd_CLK		: VitalDelayType01 := (0 ns, 0 ns);
            tipd_CLKNeg		: VitalDelayType01 := (0 ns, 0 ns);
            tipd_R			: VitalDelayType01 := (0 ns, 0 ns);
            tipd_S0			: VitalDelayType01 := (0 ns, 0 ns);
            tipd_S1			: VitalDelayType01 := (0 ns, 0 ns);
            tipd_S2			: VitalDelayType01 := (0 ns, 0 ns);
            tipd_S3			: VitalDelayType01 := (0 ns, 0 ns);
    -- tpd delays: prop delays, typical values default
	-- From SST ECL Logic Data Book 1994
            tpd_CLK_Q0      : VitalDelayType01 := (1 ns, 1 ns);
            tpd_CLK_Q1      : VitalDelayType01 := (1 ns, 1 ns);
            tpd_CLK_Q2      : VitalDelayType01 := (1 ns, 1 ns);
            tpd_CLK_Q3      : VitalDelayType01 := (1 ns, 1 ns);
            tpd_R_Q         : VitalDelayType01 := (1 ns, 1 ns);
            tpd_S_Q         : VitalDelayType01 := (1 ns, 1 ns);
    -- trelease values: release times
            trelease_S0_CLK : VitalDelayType := 1 ns;
            trelease_S1_CLK : VitalDelayType := -1 ns;
            trelease_S2_CLK : VitalDelayType := -1 ns;
            trelease_S3_CLK : VitalDelayType := -1 ns;
            trelease_R_CLK  : VitalDelayType := 1 ns;
            trelease_ENeg_CLK: VitalDelayType := 1 ns;
    -- tpw values: pulse widths
            tpw_hi_min_R    : VitalDelayType := 1 ns;
            tpw_hi_min_S    : VitalDelayType := 1 ns;
    -- tperiod_min: minimum clock period
            tperiod_min_CLK : VitalDelayType := 1 ns;
    -- ticd values: delayed clock times for negative timing constraints
        	ticd_CLK        : VitalDelayType := 1 ns;
        	ticd_CLKNeg     : VitalDelayType := 1 ns;
    -- generic control parameters
            XGenerationOn   : Boolean  := TRUE;
            TimingChecksOn  : Boolean  := FALSE;
			TimingModel		: STRING := "UNIT";
            InstancePath    : STRING  := "*" );
    PORT (
            -- 0 denotes pull-down resistor, 1 pull-up
            ENeg            : IN  STD_LOGIC := '1';
            CLK             : IN  STD_LOGIC := '0';
            CLKNeg          : IN  STD_LOGIC := '0';
            R               : IN  STD_LOGIC := '0';
            S0              : IN  STD_LOGIC := '0';
            S1              : IN  STD_LOGIC := '0';
            S2              : IN  STD_LOGIC := '0';
            S3              : IN  STD_LOGIC := '0';
            Q0              : OUT  STD_LOGIC := 'U';
            Q0Neg           : OUT  STD_LOGIC := 'U';
            Q1              : OUT  STD_LOGIC := 'U';
            Q1Neg           : OUT  STD_LOGIC := 'U';
            Q2              : OUT  STD_LOGIC := 'U';
            Q2Neg           : OUT  STD_LOGIC := 'U';
            Q3              : OUT  STD_LOGIC := 'U';
            Q3Neg           : OUT  STD_LOGIC := 'U';
			VBB				: OUT  STD_LOGIC := 'W' );
    ATTRIBUTE VITAL_LEVEL0 of nlb6241 : ENTITY IS TRUE;
END nlb6241;

------------------------------------------------------------------
--   ARCHITECTURE DECLARATION
------------------------------------------------------------------
ARCHITECTURE vhdl_behavioral of nlb6241 IS
    ATTRIBUTE VITAL_LEVEL1 of vhdl_behavioral : ARCHITECTURE IS TRUE;

    SIGNAL ENeg_ipd         : std_ulogic := 'X';
    SIGNAL CLK_ipd          : std_ulogic := 'X';
    SIGNAL CLKNeg_ipd       : std_ulogic := 'X';
    SIGNAL CLK_dly          : std_ulogic := 'X';
    SIGNAL CLKNeg_dly       : std_ulogic := 'X';
    SIGNAL R_ipd            : std_ulogic := 'X';
    SIGNAL S0_ipd           : std_ulogic := 'X';
    SIGNAL S1_ipd           : std_ulogic := 'X';
    SIGNAL S2_ipd           : std_ulogic := 'X';
    SIGNAL S3_ipd           : std_ulogic := 'X';
    SIGNAL CLKint           : std_ulogic := 'X';
    SIGNAL Q0int            : std_ulogic := 'X';
    SIGNAL Q1int            : std_ulogic := 'X';
    SIGNAL Q2int            : std_ulogic := 'X';
    SIGNAL Q3int            : std_ulogic := 'X';

BEGIN

    ---------------------------------------------------------------
    -- Wire delays
    ---------------------------------------------------------------
    WireDelay : BLOCK
    BEGIN

        w1: VitalWireDelay (ENeg_ipd, ENeg, tipd_ENeg);
        w2: VitalWireDelay (CLK_ipd, CLK, tipd_CLK);
        w3: VitalWireDelay (CLKNeg_ipd, CLKNeg, tipd_CLKNeg);
        w4: VitalWireDelay (R_ipd, R, tipd_R);
        w5: VitalWireDelay (S0_ipd, S0, tipd_S0);
        w6: VitalWireDelay (S1_ipd, S1, tipd_S1);
        w7: VitalWireDelay (S2_ipd, S2, tipd_S2);
        w8: VitalWireDelay (S3_ipd, S3, tipd_S3);

    END BLOCK;

    ---------------------------------------------------------------
    -- Negative Timing Constraint Delays
    ---------------------------------------------------------------
    SignalDelay : BLOCK
    BEGIN

        s1: VitalSignalDelay ( CLK_dly, CLK_ipd, ticd_CLK );
        s2: VitalSignalDelay ( CLKNeg_dly, CLKNeg_ipd, ticd_CLKNeg );

    END BLOCK;

    ---------------------------------------------------------------
    -- Concurrent procedure calls
    ---------------------------------------------------------------
    a1: VitalBUF(q => Q0, a => Q0int, ResultMap => ECL_wired_or_rmap);
    a2: VitalINV(q => Q0Neg, a => Q0int, ResultMap => ECL_wired_or_rmap);
    a3: VitalBUF(q => Q1, a => Q1int, ResultMap => ECL_wired_or_rmap);
    a4: VitalINV(q => Q1Neg, a => Q1int, ResultMap => ECL_wired_or_rmap);
    a5: VitalBUF(q => Q2, a => Q2int, ResultMap => ECL_wired_or_rmap);
    a6: VitalINV(q => Q2Neg, a => Q2int, ResultMap => ECL_wired_or_rmap);
    a7: VitalBUF(q => Q3, a => Q3int, ResultMap => ECL_wired_or_rmap);
    a8: VitalINV(q => Q3Neg, a => Q3int, ResultMap => ECL_wired_or_rmap);

    ---------------------------------------------------------------
    -- ECL Clock with Enable Process
    ---------------------------------------------------------------
    ECLclock : PROCESS (CLK_dly, CLKNeg_dly, ENeg_ipd)

        -- Functionality Results Variables
        VARIABLE Mode           : X01 := 'X';
        VARIABLE CLKint_pre     : std_ulogic := 'X';
        VARIABLE CLKint_zd      : std_ulogic := 'X';
        VARIABLE PrevData       : std_logic_vector(1 to 3);

        -- Output Glitch Detection Variables
        VARIABLE CLK_GlitchData : VitalGlitchDataType;

    BEGIN

        --------------------------------------------------------------
        -- Functionality Section
        --------------------------------------------------------------
        Mode := ECL_diff_mode_tab(CLK_dly, CLKNeg_dly);

        VitalStateTable (
            StateTable      => ECL_clk_tab,
            DataIn          => (CLK_dly, CLKNeg_dly, Mode ),
            Result          => CLKint_pre,
            PreviousDataIn  => PrevData );

		CLKint_zd := VitalOR2 (a => CLKint_pre, b => ENeg_ipd);
 
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
    VITALBehavior : PROCESS (CLKint, R_ipd, S0_ipd, S1_ipd, S2_ipd, S3_ipd)

        -- Timing Check Variables
        VARIABLE Rviol_R_CLK	: X01 := '0';
        VARIABLE TD_R_CLK		: VitalTimingDataType;

        VARIABLE Rviol_S0_CLK	: X01 := '0';
        VARIABLE TD_S0_CLK		: VitalTimingDataType;

        VARIABLE Rviol_S1_CLK	: X01 := '0';
        VARIABLE TD_S1_CLK		: VitalTimingDataType;

        VARIABLE Rviol_S2_CLK	: X01 := '0';
        VARIABLE TD_S2_CLK		: VitalTimingDataType;

        VARIABLE Rviol_S3_CLK	: X01 := '0';
        VARIABLE TD_S3_CLK		: VitalTimingDataType;

        VARIABLE Pviol_CLK  	: X01 := '0';
        VARIABLE PD_CLK			: VitalPeriodDataType := VitalPeriodDataInit;

        VARIABLE Pviol_R        : X01 := '0';
        VARIABLE PD_R			: VitalPeriodDataType := VitalPeriodDataInit;

        VARIABLE Pviol_S0       : X01 := '0';
        VARIABLE PD_S0			: VitalPeriodDataType := VitalPeriodDataInit;

        VARIABLE Pviol_S1       : X01 := '0';
        VARIABLE PD_S1			: VitalPeriodDataType := VitalPeriodDataInit;

        VARIABLE Pviol_S2       : X01 := '0';
        VARIABLE PD_S2			: VitalPeriodDataType := VitalPeriodDataInit;

        VARIABLE Pviol_S3       : X01 := '0';
        VARIABLE PD_S3			: VitalPeriodDataType := VitalPeriodDataInit;

        VARIABLE Violation      : X01 := '0';

        -- Functionality Results Variables
        VARIABLE Q0_zd           : std_ulogic := 'X';
        VARIABLE Q1_zd           : std_ulogic := 'X';
        VARIABLE Q2_zd           : std_ulogic := 'X';
        VARIABLE Q3_zd           : std_ulogic := 'X';
        VARIABLE PrevData0       : std_logic_vector(1 to 4);
        VARIABLE PrevData1       : std_logic_vector(1 to 4);
        VARIABLE PrevData2       : std_logic_vector(1 to 4);
        VARIABLE PrevData3       : std_logic_vector(1 to 4);

        -- Output Glitch Detection Variables
        VARIABLE Q0_GlitchData   : VitalGlitchDataType;
        VARIABLE Q1_GlitchData   : VitalGlitchDataType;
        VARIABLE Q2_GlitchData   : VitalGlitchDataType;
        VARIABLE Q3_GlitchData   : VitalGlitchDataType;

    BEGIN

        ---------------------------------------------------------------
        -- Timing Check Section
        ---------------------------------------------------------------
        IF (TimingChecksOn) THEN

            VitalRecoveryRemovalCheck (
                TestSignal      => R_ipd,
                TestSignalName  => "R_ipd",
                RefSignal       => CLKint,
                RefSignalName   => "CLKint",
                RefDelay        => ticd_CLK,
                Recovery        => trelease_R_CLK,
                ActiveLow       => FALSE,
                CheckEnabled    => TRUE,
                RefTransition   => '/',
                HeaderMsg       => InstancePath & "/nlb6241",
                TimingData      => TD_R_CLK,
                Violation       => Rviol_R_CLK );

            VitalRecoveryRemovalCheck (
                TestSignal      => S0_ipd,
                TestSignalName  => "S0_ipd",
                RefSignal       => CLKint,
                RefSignalName   => "CLKint",
                RefDelay        => ticd_CLK,
                Recovery        => trelease_S0_CLK,
                ActiveLow       => FALSE,
                CheckEnabled    => TRUE,
                RefTransition   => '/',
                HeaderMsg       => InstancePath & "/nlb6241",
                TimingData      => TD_S0_CLK,
                Violation       => Rviol_S0_CLK );

            VitalRecoveryRemovalCheck (
                TestSignal      => S1_ipd,
                TestSignalName  => "S1_ipd",
                RefSignal       => CLKint,
                RefSignalName   => "CLKint",
                RefDelay        => ticd_CLK,
                Recovery        => trelease_S1_CLK,
                ActiveLow       => FALSE,
                CheckEnabled    => TRUE,
                RefTransition   => '/',
                HeaderMsg       => InstancePath & "/nlb6241",
                TimingData      => TD_S1_CLK,
                Violation       => Rviol_S1_CLK );

            VitalRecoveryRemovalCheck (
                TestSignal      => S2_ipd,
                TestSignalName  => "S2_ipd",
                RefSignal       => CLKint,
                RefSignalName   => "CLKint",
                RefDelay        => ticd_CLK,
                Recovery        => trelease_S2_CLK,
                ActiveLow       => FALSE,
                CheckEnabled    => TRUE,
                RefTransition   => '/',
                HeaderMsg       => InstancePath & "/nlb6241",
                TimingData      => TD_S2_CLK,
                Violation       => Rviol_S2_CLK );

            VitalRecoveryRemovalCheck (
                TestSignal      => S3_ipd,
                TestSignalName  => "S3_ipd",
                RefSignal       => CLKint,
                RefSignalName   => "CLKint",
                RefDelay        => ticd_CLK,
                Recovery        => trelease_S3_CLK,
                ActiveLow       => FALSE,
                CheckEnabled    => TRUE,
                RefTransition   => '/',
                HeaderMsg       => InstancePath & "/nlb6241",
                TimingData      => TD_S3_CLK,
                Violation       => Rviol_S3_CLK );

            VitalPeriodPulseCheck (
                TestSignal      =>  CLKint,
                TestSignalName  =>  "CLKint",
                Period          =>  tperiod_min_CLK,
                HeaderMsg       =>  InstancePath & "/nlb6241",
                CheckEnabled    =>  TRUE,
                PeriodData      =>  PD_CLK,
                Violation       =>  Pviol_CLK );

            VitalPeriodPulseCheck (
                TestSignal      =>  R_ipd,
                TestSignalName  =>  "R_ipd",
                PulseWidthHigh  =>  tpw_hi_min_R,
                HeaderMsg       =>  InstancePath & "/nlb6241",
                CheckEnabled    =>  TRUE,
                PeriodData      =>  PD_R,
                Violation       =>  Pviol_R );

            VitalPeriodPulseCheck (
                TestSignal      =>  S0_ipd,
                TestSignalName  =>  "S0_ipd",
                PulseWidthHigh  =>  tpw_hi_min_S,
                HeaderMsg       =>  InstancePath & "/nlb6241",
                CheckEnabled    =>  TRUE,
                PeriodData      =>  PD_S0,
                Violation       =>  Pviol_S0 );

            VitalPeriodPulseCheck (
                TestSignal      =>  S1_ipd,
                TestSignalName  =>  "S1_ipd",
                PulseWidthHigh  =>  tpw_hi_min_S,
                HeaderMsg       =>  InstancePath & "/nlb6241",
                CheckEnabled    =>  TRUE,
                PeriodData      =>  PD_S1,
                Violation       =>  Pviol_S1 );

            VitalPeriodPulseCheck (
                TestSignal      =>  S2_ipd,
                TestSignalName  =>  "S2_ipd",
                PulseWidthHigh  =>  tpw_hi_min_S,
                HeaderMsg       =>  InstancePath & "/nlb6241",
                CheckEnabled    =>  TRUE,
                PeriodData      =>  PD_S2,
                Violation       =>  Pviol_S2 );

            VitalPeriodPulseCheck (
                TestSignal      =>  S3_ipd,
                TestSignalName  =>  "S3_ipd",
                PulseWidthHigh  =>  tpw_hi_min_S,
                HeaderMsg       =>  InstancePath & "/nlb6241",
                CheckEnabled    =>  TRUE,
                PeriodData      =>  PD_S3,
                Violation       =>  Pviol_S3 );

        END IF; -- Timing Check Section

        --------------------------------------------------------------
        -- Functionality Section
        --------------------------------------------------------------
        Violation   :=  Rviol_R_CLK or Rviol_S0_CLK or Rviol_S1_CLK or
						Rviol_S2_CLK or Rviol_S3_CLK or
                        Pviol_CLK or Pviol_R or Pviol_S0 or Pviol_S1 or
						Pviol_S2 or Pviol_S3;

        VitalStateTable ( 
            StateTable      => TFFSR_tab,
            DataIn          => (Violation, CLKint, S0_ipd, R_ipd),
            Result          => Q0_zd,
            PreviousDataIn  => PrevData0 );

        VitalStateTable ( 
            StateTable      => TFFSR_tab,
            DataIn          => (Violation, Q0_zd, S1_ipd, R_ipd),
            Result          => Q1_zd,
            PreviousDataIn  => PrevData1 );

        VitalStateTable ( 
            StateTable      => TFFSR_tab,
            DataIn          => (Violation, Q1_zd, S2_ipd, R_ipd),
            Result          => Q2_zd,
            PreviousDataIn  => PrevData2 );

        VitalStateTable ( 
            StateTable      => TFFSR_tab,
            DataIn          => (Violation, Q2_zd, S3_ipd, R_ipd),
            Result          => Q3_zd,
            PreviousDataIn  => PrevData3 );

        --------------------------------------------------------------
        -- Path Delay Section
        --------------------------------------------------------------
        VitalPathDelay01 (
            OutSignal       =>  Q0int,
            OutSignalName   =>  "Q0int",
            OutTemp         =>  Q0_zd,
            Paths           => (
            	0 => (InputChangeTime   => CLKint'LAST_EVENT,
               		  PathDelay         => tpd_CLK_Q0,
               		  PathCondition     => (CLKint = '1' and S0_ipd = '0' and
											R_ipd = '0')),
            	1 => (InputChangeTime   => S0_ipd'LAST_EVENT,
               		  PathDelay         => tpd_S_Q,
               		  PathCondition     => (S0_ipd = '1')),
            	2 => (InputChangeTime   => R_ipd'LAST_EVENT,
               		  PathDelay         => tpd_R_Q,
               		  PathCondition     => (R_ipd = '1')) ),
            GlitchData		=> Q0_GlitchData );
 
        VitalPathDelay01 (
            OutSignal       =>  Q1int,
            OutSignalName   =>  "Q1int",
            OutTemp         =>  Q1_zd,
            Paths           => (
            	0 => (InputChangeTime   => CLKint'LAST_EVENT,
               		  PathDelay         => tpd_CLK_Q1,
               		  PathCondition     => (CLKint = '1' and S1_ipd = '0' and
											R_ipd = '0')),
            	1 => (InputChangeTime   => S1_ipd'LAST_EVENT,
               		  PathDelay         => tpd_S_Q,
               		  PathCondition     => (S1_ipd = '1')),
            	2 => (InputChangeTime   => R_ipd'LAST_EVENT,
               		  PathDelay         => tpd_R_Q,
               		  PathCondition     => (R_ipd = '1')) ),
            GlitchData		=> Q1_GlitchData );
 
        VitalPathDelay01 (
            OutSignal       =>  Q2int,
            OutSignalName   =>  "Q2int",
            OutTemp         =>  Q2_zd,
            Paths           => (
            	0 => (InputChangeTime   => CLKint'LAST_EVENT,
               		  PathDelay         => tpd_CLK_Q2,
               		  PathCondition     => (CLKint = '1' and S2_ipd = '0' and
											R_ipd = '0')),
            	1 => (InputChangeTime   => S2_ipd'LAST_EVENT,
               		  PathDelay         => tpd_S_Q,
               		  PathCondition     => (S2_ipd = '1')),
            	2 => (InputChangeTime   => R_ipd'LAST_EVENT,
               		  PathDelay         => tpd_R_Q,
               		  PathCondition     => (R_ipd = '1')) ),
            GlitchData		=> Q2_GlitchData );
 
        VitalPathDelay01 (
            OutSignal       =>  Q3int,
            OutSignalName   =>  "Q3int",
            OutTemp         =>  Q3_zd,
            Paths           => (
            	0 => (InputChangeTime   => CLKint'LAST_EVENT,
               		  PathDelay         => tpd_CLK_Q3,
               		  PathCondition     => (CLKint = '1' and S3_ipd = '0' and
											R_ipd = '0')),
            	1 => (InputChangeTime   => S3_ipd'LAST_EVENT,
               		  PathDelay         => tpd_S_Q,
               		  PathCondition     => (S3_ipd = '1')),
            	2 => (InputChangeTime   => R_ipd'LAST_EVENT,
               		  PathDelay         => tpd_R_Q,
               		  PathCondition     => (R_ipd = '1')) ),
            GlitchData		=> Q3_GlitchData );
 
    END PROCESS;

END vhdl_behavioral;
