-- ---------------------------------------------------------------
--   File name : nlb6252.vhd
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
--   Part:          nlb6252
--
--   Description:   Quad 2-bit Synchronous Multiplexer with Reset
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
ENTITY nlb6252 IS
    GENERIC (
    -- tipd delays: interconnect path delays
            tipd_DA         : VitalDelayType01 := (0 ns, 0 ns);
            tipd_DB         : VitalDelayType01 := (0 ns, 0 ns);
            tipd_CLK        : VitalDelayType01 := (0 ns, 0 ns);
            tipd_R          : VitalDelayType01 := (0 ns, 0 ns);
    -- tpd delays:
            tpd_CLK_Q       : VitalDelayType01 := (1 ns, 1 ns);
            tpd_CLK_COUT    : VitalDelayType01 := (1 ns, 1 ns);
    -- tsetup values: setup times
            tsetup_D_CLK    : VitalDelayType := -1 ns;
    -- thold values: hold times, typical values default
            thold_D_CLK     : VitalDelayType := 1 ns;
    -- trelease values: release/recovery times
			trelease_R_CLK	: VitalDelayType := 1 ns;
    -- ticd values: delayed clock times for negative timing constraints
        	ticd_CLK        : VitalDelayType := 1 ns;
    -- tpw values: pulse widths
            tpw_hi_min_R    : VitalDelayType := 1 ns;
    -- tperiod_min: minimum clock period
            tperiod_min_CLK : VitalDelayType := 1 ns;
    -- generic control parameters
            XGenerationOn   : Boolean := TRUE;
            TimingChecksOn  : Boolean := FALSE;
			TimingModel		: STRING := "UNIT";
            InstancePath    : STRING := "*" );
    PORT (
            -- 0 denotes pull-down resistor
            DA              : IN STD_LOGIC := '0';
            DB              : IN STD_LOGIC := '0';
            CLK             : IN STD_LOGIC := '0';
            R               : IN STD_LOGIC := '0';
            COUT            : OUT STD_LOGIC := 'U';
            COUTNeg         : OUT STD_LOGIC := 'U';
            Q               : OUT STD_LOGIC := 'U';
            QNeg            : OUT STD_LOGIC := 'U'  );
    ATTRIBUTE VITAL_LEVEL0 of nlb6252 : ENTITY IS TRUE;
END nlb6252;

------------------------------------------------------------------
--   ARCHITECTURE DECLARATION
------------------------------------------------------------------
ARCHITECTURE vhdl_behavioral of nlb6252 IS
    ATTRIBUTE VITAL_LEVEL1 of vhdl_behavioral : ARCHITECTURE IS TRUE;

    SIGNAL DA_ipd           : std_ulogic := 'X';
    SIGNAL DB_ipd           : std_ulogic := 'X';
    SIGNAL CLK_ipd          : std_ulogic := 'X';
    SIGNAL CLK_dly          : std_ulogic := 'X';
    SIGNAL R_ipd            : std_ulogic := 'X';
    SIGNAL Qint             : std_ulogic := 'X';
    SIGNAL COUTint          : std_ulogic := 'X';

BEGIN

    ---------------------------------------------------------------
    -- Wire delays
    ---------------------------------------------------------------
    WireDelay : BLOCK
    BEGIN

        w1: VitalWireDelay (DA_ipd, DA, tipd_DA);
        w2: VitalWireDelay (DB_ipd, DB, tipd_DB);
        w3: VitalWireDelay (CLK_ipd, CLK, tipd_CLK);
        w4: VitalWireDelay (R_ipd, R, tipd_R);
 
    END BLOCK;

    ---------------------------------------------------------------
    -- Negative Timing Constraint Delays
    ---------------------------------------------------------------
    SignalDelay : BLOCK
    BEGIN

		s1: VitalSignalDelay (CLK_dly, CLK_ipd, ticd_CLK);

	END BLOCK;

    ---------------------------------------------------------------
    -- Concurrent procedure calls
    ---------------------------------------------------------------
    a1: VitalBUF (q => Q, a => Qint, ResultMap => ECL_wired_or_rmap);
    a2: VitalINV (q => QNeg, a => Qint, ResultMap => ECL_wired_or_rmap);
    a3: VitalBUF (q => COUT, a => COUTint, ResultMap => ECL_wired_or_rmap);
    a4: VitalINV (q => COUTNeg, a => COUTint, ResultMap => ECL_wired_or_rmap);

    ---------------------------------------------------------------
    -- Behavior Process
    ---------------------------------------------------------------
    VITALBehavior : PROCESS (CLK_dly, R_ipd, DA_ipd, DB_ipd)

        -- Timing Check Variables
        VARIABLE Tviol_DA_CLK   : X01 := '0';
        VARIABLE TD_DA_CLK      : VitalTimingDataType;

        VARIABLE Tviol_DB_CLK   : X01 := '0';
        VARIABLE TD_DB_CLK      : VitalTimingDataType;

        VARIABLE Rviol_R_CLK    : X01 := '0';
        VARIABLE TD_R_CLK       : VitalTimingDataType;

        VARIABLE Pviol_CLK      : X01 := '0';
        VARIABLE PD_CLK         : VitalPeriodDataType := VitalPeriodDataInit;

        VARIABLE Pviol_R        : X01 := '0';
        VARIABLE PD_R           : VitalPeriodDataType := VitalPeriodDataInit;

        VARIABLE Violation      : X01 := '0';
        VARIABLE ViolationA     : X01 := '0';
        VARIABLE ViolationB     : X01 := '0';
 
        -- Functionality Results Variables
        VARIABLE PrevData       : std_logic_vector(1 to 3);
        VARIABLE PrevDataD      : std_logic_vector(1 to 3);
        VARIABLE PrevDataDA     : std_logic_vector(1 to 4);
        VARIABLE PrevDataDB     : std_logic_vector(1 to 4);
        VARIABLE PrevDataMS     : std_logic_vector(1 to 3);
        VARIABLE PrevDataCOUT   : std_logic_vector(1 to 3);

		VARIABLE Dint			: std_ulogic := 'X';
		VARIABLE DA_int			: std_ulogic := 'X';
		VARIABLE DB_int			: std_ulogic := 'X';
        VARIABLE CLK_div_2      : std_ulogic := 'X';
		VARIABLE MuxSelect		: std_ulogic := 'X';

        VARIABLE COUT_zd        : std_ulogic := 'X';
		VARIABLE Q_zd			: std_ulogic := 'X';

        -- Output Glitch Detection Variables
        VARIABLE Q_GlitchData       : VitalGlitchDataType;
        VARIABLE COUT_GlitchData    : VitalGlitchDataType;

    BEGIN

        ---------------------------------------------------------------
        -- Timing Check Section
        ---------------------------------------------------------------
        IF (TimingChecksOn) THEN

            VitalSetupHoldCheck (
                TestSignal      => DA_ipd,
                TestSignalName  => "DA_ipd",
                RefSignal       => CLK_dly,
                RefSignalName   => "CLK_dly",
                RefDelay        => ticd_CLK,
                SetupHigh       => tsetup_D_CLK,
                SetupLow        => tsetup_D_CLK,
                HoldHigh        => thold_D_CLK,
                HoldLow         => thold_D_CLK,
                CheckEnabled    => (CLK_div_2 = '1' and R_ipd = '0'),
                RefTransition   => '/',
                HeaderMsg       => InstancePath & "/nlb6252",
                TimingData      => TD_DA_CLK,
                Violation       => Tviol_DA_CLK );

            VitalSetupHoldCheck (
                TestSignal      => DB_ipd,
                TestSignalName  => "DB_ipd",
                RefSignal       => CLK_dly,
                RefSignalName   => "CLK_dly",
                RefDelay        => ticd_CLK,
                SetupHigh       => tsetup_D_CLK,
                SetupLow        => tsetup_D_CLK,
                HoldHigh        => thold_D_CLK,
                HoldLow         => thold_D_CLK,
                CheckEnabled    => (CLK_div_2 = '0' and R_ipd = '0'),
                RefTransition   => '/',   
                HeaderMsg       => InstancePath & "/nlb6252",
                TimingData      => TD_DB_CLK,
                Violation       => Tviol_DB_CLK );

            VitalRecoveryRemovalCheck (
                TestSignal      => R_ipd,
                TestSignalName  => "R_ipd",
                RefSignal       => CLK_dly,
                RefSignalName   => "CLK_dly",
                RefDelay        => ticd_CLK,
                Recovery        => trelease_R_CLK,
                ActiveLow       => FALSE,
                CheckEnabled    => TRUE,
                RefTransition   => '/',
                HeaderMsg       => InstancePath & "/nlb6252",
                TimingData      => TD_R_CLK,
                Violation       => Rviol_R_CLK );

            VitalPeriodPulseCheck (
                TestSignal      =>  CLK_dly,
                TestSignalName  =>  "CLK_dly",
                Period          =>  tperiod_min_CLK,
                HeaderMsg       =>  InstancePath & "/nlb6252",
                CheckEnabled    =>  TRUE,
                PeriodData      =>  PD_CLK,
                Violation       =>  Pviol_CLK );

            VitalPeriodPulseCheck (
                TestSignal      =>  R_ipd,
                TestSignalName  =>  "R_ipd",
                PulseWidthHigh  =>  tpw_hi_min_R,
                HeaderMsg       =>  InstancePath & "/nlb6252",
                CheckEnabled    =>  TRUE,
                PeriodData      =>  PD_R,
                Violation       =>  Pviol_R );

        END IF; -- Timing Check Section

        --------------------------------------------------------------
        -- Functionality Section
        --------------------------------------------------------------
        Violation := Pviol_CLK or Pviol_R or Rviol_R_CLK;
        ViolationA := Tviol_DA_CLK or Pviol_R;
        ViolationB := Tviol_DB_CLK or Pviol_R;
 
		-- Output f/fs with mux input, order important

		Dint := VitalMux2 (
					data0	=> DB_int,
					data1	=> DA_int,	-- High MuxSelect selects DA
					dselect	=> MuxSelect );

        VitalStateTable (
            StateTable      => DFF_tab,
            DataIn          => (Pviol_CLK, CLK_dly, Dint),
            Result          => Q_zd,
            PreviousDataIn  => PrevDataD );

        VitalStateTable (
            StateTable      => DFF_tab,
            DataIn          => (Pviol_CLK, CLK_dly, MuxSelect),
            Result          => Cout_zd,
            PreviousDataIn  => PrevDataCOUT );

		-- Input f/fs with clock divider, order important
		-- Violations are split up according to impact on logic diagram
		-- (faxed from KBK)

        VitalStateTable (
            StateTable      => TFFR_tab,
            DataIn          => (Violation, CLK_dly, R_ipd),
            Result          => CLK_div_2,
            PreviousDataIn  => PrevData );

        VitalStateTable (
            StateTable      => DFFR_tab,
            DataIn          => (ViolationA, CLK_div_2, DA_ipd, R_ipd),
            Result          => DA_int,
            PreviousDataIn  => PrevDataDA );

        VitalStateTable (
            StateTable      => DFFNR_tab,
            DataIn          => (ViolationB, CLK_div_2, DB_ipd, R_ipd),
            Result          => DB_int,
            PreviousDataIn  => PrevDataDB );

		-- MuxSelect f/f, order: unimportant - active low clock, and inputs
		-- and outputs on active high clock

        VitalStateTable (
            StateTable      => DFFN_tab,
            DataIn          => (Pviol_CLK, CLK_dly, CLK_div_2),
            Result          => MuxSelect,
            PreviousDataIn  => PrevDataMS );

        --------------------------------------------------------------
        -- Path Delay Section
        --------------------------------------------------------------
        VitalPathDelay01 (
            OutSignal       =>  Qint,
            OutSignalName   =>  "Qint",
            OutTemp         =>  Q_zd,
            Paths           => (
	            0 => (InputChangeTime   => CLK_dly'LAST_EVENT,
					  PathDelay         => tpd_CLK_Q,
   	                  PathCondition     => (CLK_dly = '1')) ),
            GlitchData		=> Q_GlitchData );

        VitalPathDelay01 (
            OutSignal       =>  COUTint,
            OutSignalName   =>  "COUTint",
            OutTemp         =>  COUT_zd,
            Paths           => (
	            0 => (InputChangeTime   => CLK_dly'LAST_EVENT,
                      PathDelay         => tpd_CLK_COUT,
                      PathCondition     => (CLK_dly = '1')) ),
            GlitchData		=> COUT_GlitchData );

    END PROCESS;

END vhdl_behavioral;
