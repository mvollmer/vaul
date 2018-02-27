-- ---------------------------------------------------------------
--   File name : nlb6253.vhd
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
--      V1.1         R. Munden      96 FEB 17   Removed unneeded tpd's
-- ---------------------------------------------------------------
--   PART DESCRIPTION :
--
--   Library:       NLB
--   Technology:    ECL
--   Part:          nlb6253
--
--   Description:   Quad 2-bit Synchronous Demultiplxer with Reset
--
------------------------------------------------------------------

LIBRARY IEEE;   USE IEEE.std_logic_1164.ALL;
                USE IEEE.VITAL_primitives.all;
                USE IEEE.VITAL_timing.all;
LIBRARY FMF;    USE FMF.ecl_package.all;

------------------------------------------------------------------
--   ENTITY DECLARATION
------------------------------------------------------------------
ENTITY nlb6253 IS
    GENERIC (
    -- tipd delays: interconnect path delays
            tipd_D          : VitalDelayType01 := (0 ns, 0 ns);
            tipd_CLK        : VitalDelayType01 := (0 ns, 0 ns);
            tipd_R          : VitalDelayType01 := (0 ns, 0 ns);
    -- tpd delays:
            tpd_CLK_Q       : VitalDelayType01 := (1 ns, 1 ns);
            tpd_CLK_COUT    : VitalDelayType01 := (1 ns, 1 ns);
    -- tsetup values: setup times
            tsetup_D_CLK    : VitalDelayType := 1 ns;
    -- thold values: hold times
            thold_D_CLK     : VitalDelayType := 1 ns;
    -- trelease values: release times
            trelease_R_CLK  : VitalDelayType := 1 ns;
    -- tpw values: pulse widths
            tpw_hi_min_R    : VitalDelayType := 1 ns;
    -- tperiod_min: minimum clock period
            tperiod_min_CLK : VitalDelayType := 1 ns;
    -- generic control parameters
            XGenerationOn   : Boolean  := TRUE;
            TimingChecksOn  : Boolean  := FALSE;
			TimingModel		: STRING := "UNIT";
            InstancePath    : STRING  := "*" );
    PORT (
            -- 0 denotes pull-down resistor
            D               : IN STD_LOGIC := '0';
            CLK             : IN STD_LOGIC := '0';
            R               : IN STD_LOGIC := '0';
            COUT            : OUT STD_LOGIC := 'U';
            COUTNeg         : OUT STD_LOGIC := 'U';
            QA              : OUT STD_LOGIC := 'U';
            QANeg           : OUT STD_LOGIC := 'U';
            QB              : OUT STD_LOGIC := 'U';
            QBNeg           : OUT STD_LOGIC := 'U'  );
    ATTRIBUTE VITAL_LEVEL0 of nlb6253 : ENTITY IS TRUE;
END nlb6253;

------------------------------------------------------------------
--   ARCHITECTURE DECLARATION
------------------------------------------------------------------
ARCHITECTURE vhdl_behavioral of nlb6253 IS
    ATTRIBUTE VITAL_LEVEL1 of vhdl_behavioral : ARCHITECTURE IS TRUE;

    SIGNAL D_ipd            : std_ulogic := 'X';
    SIGNAL CLK_ipd          : std_ulogic := 'X';
    SIGNAL R_ipd            : std_ulogic := 'X';
    SIGNAL QAint            : std_ulogic := 'X';
    SIGNAL QBint            : std_ulogic := 'X';
    SIGNAL COUTint          : std_ulogic := 'X';

BEGIN

    ---------------------------------------------------------------
    -- Wire delays
    ---------------------------------------------------------------
    WireDelay : BLOCK
    BEGIN

        w1: VitalWireDelay (D_ipd, D, tipd_D);
        w2: VitalWireDelay (CLK_ipd, CLK, tipd_CLK);
        w3: VitalWireDelay (R_ipd, R, tipd_R);

    END BLOCK;

    ---------------------------------------------------------------
    -- Concurrent procedure calls
    ---------------------------------------------------------------
    a1: VitalBUF (q => QA, a => QAint, ResultMap => ECL_wired_or_rmap);
    a2: VitalINV (q => QANeg, a => QAint, ResultMap => ECL_wired_or_rmap);
    a3: VitalBUF (q => QB, a => QBint, ResultMap => ECL_wired_or_rmap);
    a4: VitalINV (q => QBNeg, a => QBint, ResultMap => ECL_wired_or_rmap);
    a5: VitalBUF (q => COUT, a => COUTint, ResultMap => ECL_wired_or_rmap);
    a6: VitalINV (q => COUTNeg, a => COUTint, ResultMap => ECL_wired_or_rmap);

    ---------------------------------------------------------------
    -- Behavior Process
    ---------------------------------------------------------------
    VITALBehavior : PROCESS (CLK_ipd, D_ipd, R_ipd)

        CONSTANT sync_dmux_tab : VitalStateTableType := (

-- --INPUTS----------|--PREV--|-------OUTPUTS--------------
-- Viol CLK  D    R  | QI  ST | QI'  ST'  QA   QB   COUT --
---------------------|--------|----------------------------
( 'X', '-', '-', '-', '-', '-', '-', '1', 'X', 'X', 'X'), -- Timing violation
( '-', 'X', '-', '-', '-', '-', '-', '1', 'X', 'X', 'X'), -- unknown clock
( '-', '/', '-', 'X', '-', '-', 'X', '1', 'X', 'X', 'X'), -- sync reset unknown
( '-', '/', '-', '1', '-', '-', '0', '1', '0', '0', '0'), -- sync reset
( '-', '/', '0', '0', '-', '1', '0', '0', 'S', 'S', '1'), -- Clk in D
( '-', '/', '1', '0', '-', '1', '1', '0', 'S', 'S', '1'), -- Clk in D
( '-', '/', '0', '0', '0', '0', '0', '1', '0', '0', '0'), -- D -> QI,QB, QI->QA
( '-', '/', '0', '0', '1', '0', '0', '1', '1', '0', '0'), -- D -> QI,QB, QI->QA
( '-', '/', '1', '0', '0', '0', '1', '1', '0', '1', '0'), -- D -> QI,QB, QI->QA
( '-', '/', '1', '0', '1', '0', '1', '1', '1', '1', '0'), -- D -> QI,QB, QI->QA
( '-', '-', '-', '-', '-', '-', 'S', 'S', 'S', 'S', 'S')  -- default

        ); -- end of VitalStateTableType definition

        -- Timing Check Variables
        VARIABLE Tviol_D_CLK    : X01 := '0';
        VARIABLE TD_D_CLK       : VitalTimingDataType;

        VARIABLE Rviol_R_CLK    : X01 := '0';
        VARIABLE TD_R_CLK       : VitalTimingDataType;

        VARIABLE Pviol_CLK      : X01 := '0';
        VARIABLE PD_CLK         : VitalPeriodDataType := VitalPeriodDataInit;

        VARIABLE Pviol_R        : X01 := '0';
        VARIABLE PD_R           : VitalPeriodDataType := VitalPeriodDataInit;

        VARIABLE Violation      : X01 := '0';
 
        -- Functionality Results Variables
        VARIABLE PrevData       : std_logic_vector(1 to 4);
        VARIABLE Results        : std_logic_vector(1 to 5) := (OTHERS => 'X');
        ALIAS Qint_zd           : std_ulogic IS Results(1);
        ALIAS MuxState          : std_ulogic IS Results(2);
        ALIAS QA_zd             : std_ulogic IS Results(3);
        ALIAS QB_zd             : std_ulogic IS Results(4);
        ALIAS COUT_zd           : std_ulogic IS Results(5);

        -- Output Glitch Detection Variables
        VARIABLE QA_GlitchData      : VitalGlitchDataType;
        VARIABLE QB_GlitchData      : VitalGlitchDataType;
        VARIABLE COUT_GlitchData    : VitalGlitchDataType;

    BEGIN

        ---------------------------------------------------------------
        -- Timing Check Section
        ---------------------------------------------------------------
		IF (TimingChecksOn) THEN

            VitalSetupHoldCheck (
                TestSignal      =>  D_ipd,
                TestSignalName  => "D_ipd",
                RefSignal       =>  CLK_ipd,
                RefSignalName   =>  "CLK_ipd",
                SetupHigh       =>  tsetup_D_CLK,
                SetupLow        =>  tsetup_D_CLK,
                HoldHigh        =>  thold_D_CLK,
                HoldLow         =>  thold_D_CLK,
                CheckEnabled    =>  (R_ipd = '0'),
                RefTransition   =>  '/',
                HeaderMsg       =>  InstancePath & "/nlb6253",
                TimingData      =>  TD_D_CLK,
                Violation       =>  Tviol_D_CLK );

            VitalRecoveryRemovalCheck (
                TestSignal      => R_ipd,
                TestSignalName  => "R_ipd",
                RefSignal       => CLK_ipd,
                RefSignalName   => "CLK_ipd",
                Recovery        => trelease_R_CLK,
                ActiveLow       => FALSE,
                CheckEnabled    => TRUE,
                RefTransition   => '/',
                HeaderMsg       => InstancePath & "/nlb6253",
                TimingData      => TD_R_CLK,
                Violation       => Rviol_R_CLK );

            VitalPeriodPulseCheck (
                TestSignal      =>  CLK_ipd,
                TestSignalName  =>  "CLK_ipd",
                Period          =>  tperiod_min_CLK,
                HeaderMsg       =>  InstancePath & "/nlb6253",
                CheckEnabled    =>  TRUE,
                PeriodData      =>  PD_CLK,
                Violation       =>  Pviol_CLK );

            VitalPeriodPulseCheck (
                TestSignal      =>  R_ipd,
                TestSignalName  =>  "R_ipd",
                PulseWidthHigh  =>  tpw_hi_min_R,
                HeaderMsg       =>  InstancePath & "/nlb6253",
                CheckEnabled    =>  TRUE,
                PeriodData      =>  PD_R,
                Violation       =>  Pviol_R );

        END IF; -- Timing Check Section

        --------------------------------------------------------------
        -- Functionality Section
        --------------------------------------------------------------
        Violation   :=  Tviol_D_CLK or Rviol_R_CLK or Pviol_CLK or Pviol_R;
 
        VitalStateTable (
            StateTable      => sync_dmux_tab,
            DataIn          => (Violation, CLK_ipd, D_ipd, R_ipd),
            NumStates       => 2,
            Result          => Results,
            -- Results contains: Qint_zd, MuxState, QA_zd, QB_zd, COUT_zd
            PreviousDataIn  => PrevData );
     
        --------------------------------------------------------------
        -- Path Delay Section
        --------------------------------------------------------------
        VitalPathDelay01 (
            OutSignal       =>  QAint,
            OutSignalName   =>  "QAint",
            OutTemp         =>  QA_zd,
            Paths           => (
	            0 => (InputChangeTime   => CLK_ipd'LAST_EVENT,
                      PathDelay         => tpd_CLK_Q,
                      PathCondition     => (CLK_ipd = '1'))),
            GlitchData		=> QA_GlitchData );

        VitalPathDelay01 (
            OutSignal       =>  QBint,
            OutSignalName   =>  "QBint",
            OutTemp         =>  QB_zd,
            Paths           => (
                0 => (InputChangeTime   => CLK_ipd'LAST_EVENT,
                      PathDelay         => tpd_CLK_Q,
                      PathCondition     => (CLK_ipd = '1'))),
            GlitchData		=> QB_GlitchData );

        VitalPathDelay01 (
            OutSignal       =>  COUTint,
            OutSignalName   =>  "COUTint",
            OutTemp         =>  COUT_zd,
            Paths           => (
                0 => (InputChangeTime   => CLK_ipd'LAST_EVENT,
                      PathDelay         => tpd_CLK_COUT,
                      PathCondition     => (CLK_ipd = '1'))),
            GlitchData		=> COUT_GlitchData );

    END PROCESS;

END vhdl_behavioral;
