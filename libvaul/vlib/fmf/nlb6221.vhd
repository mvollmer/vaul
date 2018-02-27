-- ---------------------------------------------------------------
--   File name : nlb6221.vhd
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
--   Part:          NLB6221
--
--   Description:   Quad D-Flip/flop with Master Reset
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
ENTITY nlb6221 IS
    GENERIC (
    -- tipd delays: interconnect path delays
            tipd_D          : VitalDelayType01 := (0 ns, 0 ns);
            tipd_DNeg       : VitalDelayType01 := (0 ns, 0 ns);
            tipd_CLKA       : VitalDelayType01 := (0 ns, 0 ns);
            tipd_CLKB       : VitalDelayType01 := (0 ns, 0 ns);
            tipd_MR         : VitalDelayType01 := (0 ns, 0 ns);
    -- tpd delays: prop delays
            tpd_CLK_Q       : VitalDelayType01 := (1 ns, 1 ns);
            tpd_MR_Q        : VitalDelayType01 := (1 ns, 1 ns);
    -- tsetup values: setup times
            tsetup_D_CLK    : VitalDelayType := 1 ns;
    -- thold values: hold times
            thold_D_CLK     : VitalDelayType := 1 ns;
    -- trelease values: release times
            trelease_MR_CLK : VitalDelayType := 1 ns;
    -- tpw values: pulse widths
            tpw_hi_min_MR   : VitalDelayType := 1 ns;
    -- tperiod_min: minimum clock period
            tperiod_min_CLK : VitalDelayType := 1 ns;
    -- generic control parameters
            XGenerationOn   : Boolean  := TRUE;
            TimingChecksOn  : Boolean  := FALSE;
			TimingModel		: STRING := "UNIT";
            InstancePath    : STRING  := "*" );
    PORT (
            -- 0 denotes pull-down resistor, 1 pull-up
            D               : IN  STD_LOGIC := '0';
            DNeg            : IN  STD_LOGIC := '1';
            CLKA            : IN  STD_LOGIC := '0';
            CLKB            : IN  STD_LOGIC := '0';
            MR              : IN  STD_LOGIC := '0';
            Q               : OUT  STD_LOGIC := 'U';
            QNeg            : OUT  STD_LOGIC := 'U';
            VBB             : OUT  STD_LOGIC := 'W'  ) ;
    ATTRIBUTE VITAL_LEVEL0 of nlb6221 : ENTITY IS TRUE;
END nlb6221;

------------------------------------------------------------------
--   ARCHITECTURE DECLARATION
------------------------------------------------------------------
ARCHITECTURE vhdl_behavioral of nlb6221 IS
    ATTRIBUTE VITAL_LEVEL1 of vhdl_behavioral : ARCHITECTURE IS TRUE;

    SIGNAL D_ipd            : std_ulogic := 'X';
    SIGNAL DNeg_ipd          : std_ulogic := 'X';
    SIGNAL CLKA_ipd         : std_ulogic := 'X';
    SIGNAL CLKB_ipd         : std_ulogic := 'X';
    SIGNAL CLKint           : std_ulogic := 'X';
    SIGNAL Dint             : std_ulogic := 'X';
    SIGNAL MR_ipd           : std_ulogic := 'X';
    SIGNAL Qint             : std_ulogic := 'X';

BEGIN

    ---------------------------------------------------------------
    -- Wire delays
    ---------------------------------------------------------------
    WireDelay : BLOCK
    BEGIN

        w1: VitalWireDelay (D_ipd, D, tipd_D);
        w2: VitalWireDelay (DNeg_ipd, DNeg, tipd_DNeg);
        w3: VitalWireDelay (CLKA_ipd, CLKA, tipd_CLKA);
        w4: VitalWireDelay (CLKB_ipd, CLKB, tipd_CLKB);
        w5: VitalWireDelay (MR_ipd, MR, tipd_MR);

    END BLOCK;

    ---------------------------------------------------------------
    -- Concurrent procedure calls
    ---------------------------------------------------------------
    a1: VitalOR2(q => CLKint, a => CLKB_ipd, b => CLKA_ipd);

    a2: VitalBUF(q => Q, a => Qint, ResultMap => ECL_wired_or_rmap);
    a3: VitalINV(q => QNeg, a => Qint, ResultMap => ECL_wired_or_rmap);
 
    ---------------------------------------------------------------
    -- D inputs Process
    ---------------------------------------------------------------
    Dinputs : PROCESS (D_ipd, DNeg_ipd)

        -- Functionality Results Variables
        VARIABLE Dint_zd        : std_ulogic := 'X';
 
        -- Output Glitch Detection Variables
        VARIABLE D_GlitchData   : VitalGlitchDataType;
 
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
            GlitchData      => D_GlitchData );
 
    END PROCESS;

    ---------------------------------------------------------------
    -- Behavior Process
    ---------------------------------------------------------------
    VITALBehavior : PROCESS (Dint, CLKint, MR_ipd)

        -- Timing Check Variables
        VARIABLE Tviol_Dint_CLKint	: X01 := '0';
        VARIABLE TD_Dint_CLKint		: VitalTimingDataType;

        VARIABLE Rviol_MR_CLKint	: X01 := '0';
        VARIABLE TD_MR_CLKint		: VitalTimingDataType;

        VARIABLE Pviol_CLKint  	: X01 := '0';
        VARIABLE PD_CLKint		: VitalPeriodDataType := VitalPeriodDataInit;

        VARIABLE Pviol_MR       : X01 := '0';
        VARIABLE PD_MR			: VitalPeriodDataType := VitalPeriodDataInit;

        VARIABLE Violation      : X01 := '0';

        -- Functionality Results Variables
        VARIABLE Q_zd           : std_ulogic := 'X';
        VARIABLE PrevData       : std_logic_vector(1 to 4);

        -- Output Glitch Detection Variables
        VARIABLE Q_GlitchData   : VitalGlitchDataType;

    BEGIN

        ---------------------------------------------------------------
        -- Timing Check Section
        ---------------------------------------------------------------
        IF (TimingChecksOn) THEN

            VitalSetupHoldCheck (
                TestSignal      =>  Dint,
                TestSignalName  => "Dint",
                RefSignal       =>  CLKint,
                RefSignalName   =>  "CLKint",
                SetupHigh       =>  tsetup_D_CLK,
                SetupLow        =>  tsetup_D_CLK,
                HoldHigh        =>  thold_D_CLK,
                HoldLow         =>  thold_D_CLK,
                CheckEnabled    =>  (MR_ipd = '0'),
                RefTransition   =>  '/',
                HeaderMsg       =>  InstancePath & "/nlb6221",
                TimingData      =>  TD_Dint_CLKint,
                Violation       =>  Tviol_Dint_CLKint );

            VitalRecoveryRemovalCheck (
                TestSignal      => MR_ipd,
                TestSignalName  => "MR_ipd",
                RefSignal       => CLKint,
                RefSignalName   => "CLKint",
                Recovery        => trelease_MR_CLK,
                ActiveLow       => FALSE,
                CheckEnabled    => TRUE,
                RefTransition   => '/',
                HeaderMsg       => InstancePath & "/nlb6221",
                TimingData      => TD_MR_CLKint,
                Violation       => Rviol_MR_CLKint );

            VitalPeriodPulseCheck (
                TestSignal      =>  CLKint,
                TestSignalName  =>  "CLKint",
                Period          =>  tperiod_min_CLK,
                HeaderMsg       =>  InstancePath & "/nlb6221",
                CheckEnabled    =>  TRUE,
                PeriodData      =>  PD_CLKint,
                Violation       =>  Pviol_CLKint );

            VitalPeriodPulseCheck (
                TestSignal      =>  MR_ipd,
                TestSignalName  =>  "MR_ipd",
                PulseWidthHigh  =>  tpw_hi_min_MR,
                HeaderMsg       =>  InstancePath & "/nlb6221",
                CheckEnabled    =>  TRUE,
                PeriodData      =>  PD_MR,
                Violation       =>  Pviol_MR );

        END IF; -- Timing Check Section

        --------------------------------------------------------------
        -- Functionality Section
        --------------------------------------------------------------
        Violation   :=  Tviol_Dint_CLKint or Rviol_MR_CLKint or
                        Pviol_CLKint or Pviol_MR;

        VitalStateTable ( 
            StateTable      => DFFR_tab,
            DataIn          => (Violation, CLKint, Dint, MR_ipd),
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
            	1 => (InputChangeTime   => MR_ipd'LAST_EVENT,
               		  PathDelay         => tpd_MR_Q,
               		  PathCondition     => (MR_ipd = '1')) ),
            GlitchData		=> Q_GlitchData );
 
    END PROCESS;

END vhdl_behavioral;
