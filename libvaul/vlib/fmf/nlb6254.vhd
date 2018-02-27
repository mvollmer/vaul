-- ---------------------------------------------------------------
--   File name : nlb6254.vhd
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
--      V1.1         R. Munden      96 FEB 17   removed unneeded tpd's
--
-- ---------------------------------------------------------------
--   PART DESCRIPTION :
--
--   Library:       NLB
--   Technology:    ECL
--   Part:          nlb6254
--
--   Description:   8-bit Mutiplexer
--
-----------------------------------------------------------------

LIBRARY IEEE;   USE IEEE.std_logic_1164.ALL;
                USE IEEE.VITAL_primitives.all;
                USE IEEE.VITAL_timing.all;
LIBRARY FMF;    USE FMF.ecl_package.all;
				USE FMF.ff_package.all;
				USE FMF.state_tab_package.all;

------------------------------------------------------------------
--   ENTITY DECLARATION
------------------------------------------------------------------
ENTITY nlb6254 IS
    GENERIC (
    -- tipd delays: interconnect path delays
            tipd_D0         : VitalDelayType01 := (0 ns, 0 ns);
            tipd_D1         : VitalDelayType01 := (0 ns, 0 ns);
            tipd_D2         : VitalDelayType01 := (0 ns, 0 ns);
            tipd_D3         : VitalDelayType01 := (0 ns, 0 ns);
            tipd_D4         : VitalDelayType01 := (0 ns, 0 ns);
            tipd_D5         : VitalDelayType01 := (0 ns, 0 ns);
            tipd_D6         : VitalDelayType01 := (0 ns, 0 ns);
            tipd_D7         : VitalDelayType01 := (0 ns, 0 ns);
            tipd_CLK        : VitalDelayType01 := (0 ns, 0 ns);
            tipd_CLKNeg     : VitalDelayType01 := (0 ns, 0 ns);
            tipd_R          : VitalDelayType01 := (0 ns, 0 ns);
    -- tpd delays:
            tpd_CLK_Q       : VitalDelayType01 := (1 ns, 1 ns);
            tpd_CLK_COUT    : VitalDelayType01 := (1 ns, 1 ns);
    -- tsetup values: setup times
	--      tsetup_D_CLK    : VitalDelayType := -1 ns;
    -- thold values: hold times
    --      thold_D_CLK     : VitalDelayType := 1 ns;
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
            D0              : IN STD_LOGIC := '0';
            D1              : IN STD_LOGIC := '0';
            D2              : IN STD_LOGIC := '0';
            D3              : IN STD_LOGIC := '0';
            D4              : IN STD_LOGIC := '0';
            D5              : IN STD_LOGIC := '0';
            D6              : IN STD_LOGIC := '0';
            D7              : IN STD_LOGIC := '0';
            CLK             : IN STD_LOGIC := '0';
            CLKNeg          : IN STD_LOGIC := '0';
            R               : IN STD_LOGIC := '0';
            COUT            : OUT STD_LOGIC := 'U';
            COUTNeg         : OUT STD_LOGIC := 'U';
            Q               : OUT STD_LOGIC := 'U';
            QNeg            : OUT STD_LOGIC := 'U';
			VBB				: OUT STD_LOGIC := 'W' );
    ATTRIBUTE VITAL_LEVEL0 of nlb6254 : ENTITY IS TRUE;
END nlb6254;

------------------------------------------------------------------
--   ARCHITECTURE DECLARATION
------------------------------------------------------------------
ARCHITECTURE vhdl_behavioral of nlb6254 IS
    ATTRIBUTE VITAL_LEVEL1 of vhdl_behavioral : ARCHITECTURE IS TRUE;

    SIGNAL D0_ipd           : std_ulogic := 'X';
    SIGNAL D1_ipd           : std_ulogic := 'X';
    SIGNAL D2_ipd           : std_ulogic := 'X';
    SIGNAL D3_ipd           : std_ulogic := 'X';
    SIGNAL D4_ipd           : std_ulogic := 'X';
    SIGNAL D5_ipd           : std_ulogic := 'X';
    SIGNAL D6_ipd           : std_ulogic := 'X';
    SIGNAL D7_ipd           : std_ulogic := 'X';
    SIGNAL CLK_ipd          : std_ulogic := 'X';
    SIGNAL CLKNeg_ipd       : std_ulogic := 'X';
    SIGNAL R_ipd            : std_ulogic := 'X';
    SIGNAL CLKint           : std_ulogic := 'X';
    SIGNAL Qint             : std_ulogic := 'X';
    SIGNAL COUTint          : std_ulogic := 'X';

BEGIN

    ---------------------------------------------------------------
    -- Wire delays
    ---------------------------------------------------------------
    WireDelay : BLOCK
    BEGIN

        w1: VitalWireDelay (D0_ipd, D0, tipd_D0);
        w2: VitalWireDelay (D1_ipd, D1, tipd_D1);
        w3: VitalWireDelay (D2_ipd, D2, tipd_D2);
        w4: VitalWireDelay (D3_ipd, D3, tipd_D3);
        w5: VitalWireDelay (D4_ipd, D4, tipd_D4);
        w6: VitalWireDelay (D5_ipd, D5, tipd_D5);
        w7: VitalWireDelay (D6_ipd, D6, tipd_D6);
        w8: VitalWireDelay (D7_ipd, D7, tipd_D7);
        w9: VitalWireDelay (CLK_ipd, CLK, tipd_CLK);
        w10: VitalWireDelay (CLKNeg_ipd, CLKNeg, tipd_CLKNeg);
        w11: VitalWireDelay (R_ipd, R, tipd_R);
 
    END BLOCK;

    ---------------------------------------------------------------
    -- Concurrent procedure calls
    ---------------------------------------------------------------
    a1: VitalBUF (q => Q, a => Qint, ResultMap => ECL_wired_or_rmap);
    a2: VitalINV (q => QNeg, a => Qint, ResultMap => ECL_wired_or_rmap);
    a3: VitalBUF (q => COUT, a => COUTint, ResultMap => ECL_wired_or_rmap);
    a4: VitalINV (q => COUTNeg, a => COUTint, ResultMap => ECL_wired_or_rmap);

    ---------------------------------------------------------------
    -- ECL Clock Process
    ---------------------------------------------------------------
    ECLclock : PROCESS (CLK_ipd, CLKNeg_ipd)
   
        -- Functionality Results Variables
        VARIABLE Mode           : X01 := 'X';
        VARIABLE CLKint_zd      : std_ulogic := 'X';
        VARIABLE PrevData       : std_logic_vector(1 to 3);
   
        -- Glitch Detection Variables
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
    VITALBehavior : PROCESS (CLKint, R_ipd, D0_ipd, D1_ipd, D2_ipd, D3_ipd,
								D4_ipd, D5_ipd, D6_ipd, D7_ipd)

        -- Timing Check Variables
        -- VARIABLE Tviol_D_CLK    : X01 := '0';
        -- VARIABLE TD_D_CLK       : VitalTimingDataType;

        VARIABLE Pviol_CLK      : X01 := '0';
        VARIABLE PD_CLK         : VitalPeriodDataType := VitalPeriodDataInit;

        VARIABLE Pviol_R        : X01 := '0';
        VARIABLE PD_R           : VitalPeriodDataType := VitalPeriodDataInit;

        VARIABLE Violation      : X01 := '0';
 
        -- Functionality Results Variables
        VARIABLE PrevData       : std_logic_vector(1 to 3);
        VARIABLE PrevDataQ1     : std_logic_vector(1 to 3);
        VARIABLE PrevDataQ2     : std_logic_vector(1 to 3);
        VARIABLE PrevDataQ3     : std_logic_vector(1 to 3);
        VARIABLE PrevDataQ4     : std_logic_vector(1 to 3);
        VARIABLE PrevDataQ5     : std_logic_vector(1 to 3);
        VARIABLE PrevDataQ6     : std_logic_vector(1 to 3);
        VARIABLE PrevDataQ7     : std_logic_vector(1 to 3);
        VARIABLE PrevDataQ8     : std_logic_vector(1 to 3);

        VARIABLE PrevDataCOUT   : std_logic_vector(1 to 3);

        VARIABLE MuxSelect      : std_logic_vector(2 downto 0)
														 := (OTHERS => 'X');
		ALIAS  Cout_int			: std_ulogic IS MuxSelect(2);
		VARIABLE Cout_zd		: std_ulogic := 'X';
		VARIABLE Dint			: std_ulogic := 'X';

        VARIABLE Q1_zd          : std_ulogic := 'X';
        VARIABLE Q2_zd          : std_ulogic := 'X';
        VARIABLE Q3_zd          : std_ulogic := 'X';
        VARIABLE Q4_zd          : std_ulogic := 'X';
        VARIABLE Q5_zd          : std_ulogic := 'X';
        VARIABLE Q6_zd          : std_ulogic := 'X';
        VARIABLE Q7_zd          : std_ulogic := 'X';
        VARIABLE Q_zd           : std_ulogic := 'X';

        VARIABLE BLANK          : std_ulogic := '0';

        -- Output Glitch Detection Variables
        VARIABLE Q_GlitchData       : VitalGlitchDataType;
        VARIABLE COUT_GlitchData    : VitalGlitchDataType;

    BEGIN

        ---------------------------------------------------------------
        -- Timing Check Section
        ---------------------------------------------------------------
        IF (TimingChecksOn) THEN

--            VitalSetupHoldCheck (
 --               TestSignal      => DA_ipd,
  --              TestSignalName  => "DA_ipd",
   --             RefSignal       => CLK_ipd,
    --            RefSignalName   => "CLK_ipd",
     --           SetupHigh       => tsetup_D_CLK,
      --          SetupLow        => tsetup_D_CLK,
       --         HoldHigh        => thold_D_CLK,
        --        HoldLow         => thold_D_CLK,
         --       CheckEnabled    => (COUTint = '1'),
          --      RefTransition   => '/',
           --     HeaderMsg       => InstancePath & "/nlb6254",
            --    TimingData      => TD_DA_CLK,
             --   Violation       => Tviol_DA_CLK );

            VitalPeriodPulseCheck (
                TestSignal      =>  CLK_ipd,
                TestSignalName  =>  "CLK_ipd",
                Period          =>  tperiod_min_CLK,
                HeaderMsg       =>  InstancePath & "/nlb6254",
                CheckEnabled    =>  TRUE,
                PeriodData      =>  PD_CLK,
                Violation       =>  Pviol_CLK );

            VitalPeriodPulseCheck (
                TestSignal      =>  R_ipd,
                TestSignalName  =>  "R_ipd",
                PulseWidthHigh  =>  tpw_hi_min_R,
                HeaderMsg       =>  InstancePath & "/nlb6254",
                CheckEnabled    =>  TRUE,
                PeriodData      =>  PD_R,
                Violation       =>  Pviol_R );

        END IF; -- Timing Check Section

        --------------------------------------------------------------
        -- Functionality Section
        --------------------------------------------------------------
        Violation := Pviol_CLK or Pviol_R;

		-- Cout is delayed one half-clock to match Timing Diagram
		-- Order is not important since DFFN is active on clk falling edge

		VitalStateTable (
            StateTable      => DFFN_tab,
            DataIn          => (BLANK, CLKint, Cout_int),
			Result			=> Cout_zd,
			PreviousDataIn	=> PrevDataCOUT );

		-- Output f/fs with mux input, order important 

		Dint := VitalMux8 (
    		data			=> (D0_ipd, D1_ipd, D2_ipd, D3_ipd,
								D4_ipd, D5_ipd, D6_ipd, D7_ipd),
			dselect			=> MuxSelect );

		VitalStateTable (
			StateTable		=> DFF_tab,
            DataIn          => (BLANK, CLKint, Q7_zd),
            Result          => Q_zd,
            PreviousDataIn  => PrevDataQ8 );
			 
		VitalStateTable (
			StateTable		=> DFF_tab,
            DataIn          => (BLANK, CLKint, Q6_zd),
            Result          => Q7_zd,
            PreviousDataIn  => PrevDataQ7 );
			 
		VitalStateTable (
			StateTable		=> DFF_tab,
            DataIn          => (BLANK, CLKint, Q5_zd),
            Result          => Q6_zd,
            PreviousDataIn  => PrevDataQ6 );
			 
		VitalStateTable (
			StateTable		=> DFF_tab,
            DataIn          => (BLANK, CLKint, Q4_zd),
            Result          => Q5_zd,
            PreviousDataIn  => PrevDataQ5 );
			 
		VitalStateTable (
			StateTable		=> DFF_tab,
            DataIn          => (BLANK, CLKint, Q3_zd),
            Result          => Q4_zd,
            PreviousDataIn  => PrevDataQ4 );
			 
		VitalStateTable (
			StateTable		=> DFF_tab,
            DataIn          => (BLANK, CLKint, Q2_zd),
            Result          => Q3_zd,
            PreviousDataIn  => PrevDataQ3 );
			 
		VitalStateTable (
			StateTable		=> DFF_tab,
            DataIn          => (BLANK, CLKint, Q1_zd),
            Result          => Q2_zd,
            PreviousDataIn  => PrevDataQ2 );
			 
		VitalStateTable (
			StateTable		=> DFF_tab,
            DataIn          => (BLANK, CLKint, Dint),
            Result          => Q1_zd,
            PreviousDataIn  => PrevDataQ1 );
			 
		-- Lastly, do the State change, 0 -> 7
 
        VitalStateTable (
            StateTable      => st8R_tab,
            DataIn          => (Violation, CLKint, R_ipd),
            NumStates       => 3,
            Result          => MuxSelect, --> Cout_int is MuxSelect(2)
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
   	                  PathCondition     => (CLK_ipd = '1')) ),
            GlitchData		=> Q_GlitchData );

        VitalPathDelay01 (
            OutSignal       =>  COUTint,
            OutSignalName   =>  "COUTint",
            OutTemp         =>  COUT_zd,
            Paths           => (
	            0 => (InputChangeTime   => CLK_ipd'LAST_EVENT,
                      PathDelay         => tpd_CLK_COUT,
                      PathCondition     => (CLK_ipd = '1' and R_ipd = '0')) ),
            GlitchData		=> COUT_GlitchData );

    END PROCESS;

END vhdl_behavioral;
