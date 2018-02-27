-- ---------------------------------------------------------------
--   File name : nlb6220.vhd
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
--   Part:          NLB6220
--
--   Description:   Dual D Flip-Flop with Set and Reset
--
------------------------------------------------------------------

LIBRARY IEEE;   USE IEEE.std_logic_1164.ALL;
                USE IEEE.VITAL_primitives.all;
                USE IEEE.VITAL_timing.all;
LIBRARY FMF;	USE FMF.ecl_package.all;
				USE FMF.ff_package.all;

------------------------------------------------------------------
--   ENTITY DECLARATION
------------------------------------------------------------------
ENTITY nlb6220 IS
    GENERIC (
    -- tipd delays: interconnect path delays
            tipd_S          : VitalDelayType01 := (0 ns, 0 ns);
            tipd_R          : VitalDelayType01 := (0 ns, 0 ns);
            tipd_CLK        : VitalDelayType01 := (0 ns, 0 ns);
            tipd_CLKNeg     : VitalDelayType01 := (0 ns, 0 ns);
            tipd_D          : VitalDelayType01 := (0 ns, 0 ns);
            tipd_DNeg       : VitalDelayType01 := (0 ns, 0 ns);
    -- tpd delays: prop delays, typical values default
            tpd_CLK_Q       : VitalDelayType01 := (1 ns, 1 ns);
            tpd_S_Q         : VitalDelayType01 := (1 ns, 1 ns);
            tpd_R_Q         : VitalDelayType01 := (1 ns, 1 ns);
    -- tsetup values: setup times, typical values default
            tsetup_D_CLK    : VitalDelayType := 1 ns;
    -- thold values: hold times, typical values default
            thold_D_CLK     : VitalDelayType := 1 ns;
    -- trelease values: release times
            trelease_S_CLK  : VitalDelayType := 1 ns;
            trelease_R_CLK  : VitalDelayType := 1 ns;
    -- tpw values: pulse widths
            tpw_hi_min_S    : VitalDelayType := 1 ns;
            tpw_hi_min_R    : VitalDelayType := 1 ns;
    -- tperiod_min: minimum clock period = 1/max freq
            tperiod_min_CLK : VitalDelayType := 1 ns;
    -- generic control parameters
            TimingChecksOn  : BOOLEAN := TRUE;
            XGenerationOn   : BOOLEAN := TRUE;
			TimingModel		: STRING := "UNIT";
            InstancePath    : STRING := "*" );
    PORT (
            -- 0 denotes pull-down resistor, 1 pull-up
            CLK             : IN  STD_LOGIC := '0';
            CLKNeg          : IN  STD_LOGIC := '1';
            S               : IN  STD_LOGIC := '0';
            R               : IN  STD_LOGIC := '0';
            D               : IN  STD_LOGIC := '0';
            DNeg            : IN  STD_LOGIC := '1';
            Q               : OUT  STD_LOGIC := 'U';
            QNeg            : OUT  STD_LOGIC := 'U';
            VBB             : OUT  STD_LOGIC := 'W'  );
    ATTRIBUTE VITAL_LEVEL0 of nlb6220 : ENTITY IS TRUE;
END nlb6220;

------------------------------------------------------------------
--   ARCHITECTURE DECLARATION
------------------------------------------------------------------
ARCHITECTURE vhdl_behavioral of nlb6220 IS
    ATTRIBUTE Vital_level1 of vhdl_behavioral : ARCHITECTURE IS TRUE;

    SIGNAL CLK_ipd          : std_ulogic := 'X';
    SIGNAL CLKNeg_ipd       : std_ulogic := 'X';
    SIGNAL S_ipd            : std_ulogic := 'X';
    SIGNAL R_ipd            : std_ulogic := 'X';
    SIGNAL D_ipd            : std_ulogic := 'X';
    SIGNAL DNeg_ipd         : std_ulogic := 'X';
    SIGNAL Dint             : std_ulogic := 'X';
    SIGNAL CLKint           : std_ulogic := 'X';
    SIGNAL Qint             : std_ulogic := 'X';

BEGIN

    ---------------------------------------------------------------
    -- Wire delays
    ---------------------------------------------------------------
    WireDelay : BLOCK
    BEGIN

        w1: VitalWireDelay (CLK_ipd, CLK, tipd_CLK);
        w2: VitalWireDelay (CLKNeg_ipd, CLKNeg, tipd_CLKNeg);
        w3: VitalWireDelay (S_ipd, S, tipd_S);
        w4: VitalWireDelay (R_ipd, R, tipd_R);
        w5: VitalWireDelay (D_ipd, D, tipd_D);
        w6: VitalWireDelay (DNeg_ipd, DNeg, tipd_DNeg);

    END BLOCK;

    ---------------------------------------------------------------
    --  Concurrent procedure calls
    ---------------------------------------------------------------
    a1: VitalBUF (q => Q, a => Qint, ResultMap => ECL_wired_or_rmap);
    a2: VitalINV (q => QNeg, a => Qint, ResultMap => ECL_wired_or_rmap);

    ---------------------------------------------------------------
    -- D inputs Process
    ---------------------------------------------------------------
    Dinputs : PROCESS (D_ipd, DNeg_ipd)

        -- Functionality Results Variables
		VARIABLE Dint_zd		: std_ulogic := 'X';

        -- Output Glitch Detection Variables
		VARIABLE D_GlitchData	: VitalGlitchDataType;

	BEGIN

        --------------------------------------------------------------
        -- Functionality Section
        --------------------------------------------------------------
		Dint_zd := ECL_s_or_d_inputs_tab(D_ipd, DNeg_ipd);

        --------------------------------------------------------------
        -- (Dummy) Path Delay Section
        --------------------------------------------------------------
        VitalPathDelay (
            OutSignal       => Dint,
            OutSignalName   => "Dint",
            OutTemp         => Dint_zd,
            Paths           => ( 0 => (0 ps, VitalZeroDelay, FALSE)),
			GlitchData		=> D_GlitchData );

	END PROCESS;

    ---------------------------------------------------------------
    -- ECL Clock Process
    ---------------------------------------------------------------
    ECLclock : PROCESS (CLK_ipd, CLKNeg_ipd)

        -- Functionality Results Variables
		VARIABLE Mode			: X01 := 'X';
        VARIABLE CLKint_zd      : std_ulogic := 'X';
        VARIABLE PrevData       : std_logic_vector(1 to 3);

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
			GlitchData		=> CLK_GlitchData );

	END PROCESS;

    ---------------------------------------------------------------
    -- VitalBehavior Process
    ---------------------------------------------------------------
    VitalBehavior : PROCESS (CLKint, Dint, S_ipd, R_ipd)

        -- Timing Check Variables
        VARIABLE Tviol_Dint_CLKint  : X01 := '0';
        VARIABLE TD_Dint_CLKint     : VitalTimingDataType;
        
        VARIABLE Rviol_S_CLKint 	: X01 := '0';
        VARIABLE TD_S_CLKint  		: VitalTimingDataType;

        VARIABLE Rviol_R_CLKint 	: X01 := '0';
        VARIABLE TD_R_CLKint  		: VitalTimingDataType;

        VARIABLE Pviol_CLKint   : X01 := '0';
        VARIABLE PD_CLKint      : VitalPeriodDataType := VitalPeriodDataInit;
        
        VARIABLE Pviol_S        : X01 := '0';
        VARIABLE PD_S           : VitalPeriodDataType := VitalPeriodDataInit;
        
        VARIABLE Pviol_R        : X01 := '0';
        VARIABLE PD_R           : VitalPeriodDataType := VitalPeriodDataInit;

        VARIABLE Violation      : X01 := '0';
        
        -- Functionality Results Variables
        VARIABLE Q_zd           : std_ulogic := 'X';
        VARIABLE PrevData       : std_logic_vector(1 to 5);

        -- Output Glitch Detection Variables
        VARIABLE Q_GlitchData   : VitalGlitchDataType;

    BEGIN

        ---------------------------------------------------------------
        -- Timing Check Section
        ---------------------------------------------------------------
        IF (TimingChecksOn) THEN

            VitalSetupHoldCheck (
                TestSignal      => Dint,
                TestSignalName  => "D",
                RefSignal       => CLKint,
                RefSignalName   => "CLKint",
                SetupHigh       => tsetup_D_CLK,
                SetupLow        => tsetup_D_CLK,
                HoldHigh        => thold_D_CLK,
                HoldLow         => thold_D_CLK,
                CheckEnabled    => (S_ipd = '0') and (R_ipd = '0'),
                RefTransition   => '/',
                HeaderMsg       => InstancePath & "/NLB6220",
                TimingData      => TD_Dint_CLKint,
                Violation       => Tviol_Dint_CLKint );

            VitalRecoveryRemovalCheck (
                TestSignal      => S_ipd,
                TestSignalName  => "S_ipd",
                RefSignal       => CLKint,
                RefSignalName   => "CLKint",
                Recovery        => trelease_S_CLK,
                ActiveLow       => FALSE,
                CheckEnabled    => TRUE,
                RefTransition   => '/',
                HeaderMsg       => InstancePath & "/NLB6220",
                TimingData      => TD_S_CLKint,
                Violation       => Rviol_S_CLKint );
 
            VitalRecoveryRemovalCheck (
                TestSignal      => R_ipd,
                TestSignalName  => "R_ipd",
                RefSignal       => CLKint,
                RefSignalName   => "CLKint",
                Recovery        => trelease_R_CLK,
                ActiveLow       => FALSE,
                CheckEnabled    => TRUE,
                RefTransition   => '/',
                HeaderMsg       => InstancePath & "/NLB6220",
                TimingData      => TD_R_CLKint,
                Violation       => Rviol_R_CLKint );
 
            VitalPeriodPulseCheck (
                TestSignal      => CLKint,
                TestSignalName  => "CLKint",
                Period          => tperiod_min_CLK,
                HeaderMsg       => InstancePath & "/NLB6220",
                CheckEnabled    => TRUE,
                PeriodData      => PD_CLKint,
                Violation       => Pviol_CLKint );

            VitalPeriodPulseCheck (
                TestSignal      => S_ipd,
                TestSignalName  => "S_ipd",
                PulseWidthHigh  => tpw_hi_min_S,
                HeaderMsg       => InstancePath & "/NLB6220",
                CheckEnabled    => TRUE,
                PeriodData      => PD_S,
                Violation       => Pviol_S );

            VitalPeriodPulseCheck (
                TestSignal      => R_ipd,
                TestSignalName  => "R_ipd",
                PulseWidthHigh  => tpw_hi_min_R,
                HeaderMsg       => InstancePath & "/NLB6220",
                CheckEnabled    => TRUE,
                PeriodData      => PD_R,
                Violation       => Pviol_R );

        END IF; -- Timing Check Section

        --------------------------------------------------------------
        -- Functionality Section
        --------------------------------------------------------------
        Violation := Tviol_Dint_CLKint or Pviol_CLKint or
                     Rviol_S_CLKint or Rviol_R_CLKint or Pviol_S or Pviol_R;

        VitalStateTable (
            StateTable      => DFFSR_tab,
            DataIn          => (Violation, CLKint, Dint, S_ipd, R_ipd),
            Result          => Q_zd,
            PreviousDataIn  => PrevData );

        --------------------------------------------------------------
        -- Path Delay Section
        --------------------------------------------------------------
        VitalPathDelay01 (
            OutSignal       =>  Qint,
            OutSignalName   =>  "Qint",
            OutTemp         =>  Q_zd,
            Paths           => (
            	0 => (InputChangeTime   => CLKint'LAST_EVENT,
               		  PathDelay         => tpd_CLK_Q,
               		  PathCondition     => (CLKint = '1')),
	            1 => (InputChangeTime   => S_ipd'LAST_EVENT,
   	                  PathDelay         => tpd_S_Q,
   	                  PathCondition     => (S_ipd = '1')),
   	        	2 => (InputChangeTime   => R_ipd'LAST_EVENT,
   	                  PathDelay         => tpd_R_Q,
   	                  PathCondition     => (R_ipd = '1')) ),
            GlitchData		=> Q_GlitchData );

    END PROCESS;

END vhdl_behavioral;
