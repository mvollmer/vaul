--=============================================================================
--This electronic directory is copyright (c) 1991 by Sveriges Mekanfoerbund 
--and Bernt Arbegard
--The contents are donated to the public domain as long as a) this copyright
--message is retained with any file and b) the file(s) are freely distributed
--and not sold. There is no warranty of any kind, implied or otherwise, as to
--the reliability, expected results, or usability of any information in this
--directory.
--=============================================================================
--This file contains all necessary VHDL code for running the examples 
--described in chapter 3 of the report "Analog and Mixed Analog-Digital 
--Design Using VHDL". The report (written in English) can be ordered from 
--Industrilitteratur AB
--Box 5506
--S-114 85 STOCKHOLM, Sweden,
--or on telephone no. +46 8 783 8214, or via Fax +46 8 783 0519.
--The report number is "Mekanresultat 91001" and the price is approx
--420 SEK +tax.
--In addition to standard VHDL, the mathematical functions SQRT and EXP are
--needed. This file is adopted to the STD.MATH library available in
--the MINT system from the Swedish Institute of Microelectronics (IM). 
--For other VHDL simulators the reference to the mathematical library
--might have to be changed in the code.
--The VHDL code must be placed in the library WORK.MEKAN1. If not,
--som USE statements have to be modified in the code.
--In MINT run ANALYZE on the entire file and then BUILD for the configuration
--you like to simulate. The CONFIGUTATIONs FrontPage, Figure15, Figure16 and
--Figure17 correpond to the figures in the report.
--Use alway 1ns (or less) simulation resolution.
--The code in this file is somewhat different from the one shown in
--appendix 4 in the report. The functionality is however the same.
--Bug reports and suggested enhancements/extensions are gratefully received
--by Bernt Arbegard on Email: arbegard@baccus.ericsson.se
--or to my company address 
--Radiosystem Sweden AB
--Att: Bernt Arbegard
--Box 11
--S-164 93 KISTA, Sweden
--Good (analog) luck!
--=============================================================================
--***********************
--Start of Mekan1 PACKAGE 
--***********************
PACKAGE Mekan1 IS

--CONSTANT value assignments and FUNCTION definitions are
--made in the PACKAGE BODY.
SUBTYPE  PosREAL    IS REAL RANGE 0.0 TO REAL'HIGH;
SUBTYPE  PosINTEGER IS INTEGER RANGE 0 TO INTEGER'HIGH;
CONSTANT TimeRes : TIME; --Time resolution in physical type TIME
CONSTANT RealRes : REAL; --Time resolution in decimal seconds as REAL
CONSTANT ZT      : TIME; --Zero time
CONSTANT OutAccDef : PosREAL; --Default output absolute accuracy
CONSTANT RelAccDef : PosREAL; --Default output relative accuracy
CONSTANT OutResDef : PosREAL; --Default output resistance
CONSTANT OutCondDef: PosREAL; --Default output conductance
CONSTANT InResDef  : PosREAL; --Default INOUT resistance
CONSTANT InCondDef : PosREAL; --Default INOUT conductance
CONSTANT InVoltDef :    REAL; --Default INOUT voltage

TYPE AnSig IS
  RECORD
    AnSigAmpl  : REAL ;    --Voltage at time AnSigStart
    AnSigGrad  : REAL ;    --Gradient between AnSigStart and AnSigStop
    AnSigStart : TIME ;    --Start time for signal representation
    AnSigStop  : TIME ;    --Stop time for signal representation
    AnSigCond  : PosREAL ; --Driving/loading conductance of the port
--  AnSigCap   : PosREAL ; --Loading capacitance (not yet implemented)
  END RECORD;

CONSTANT AnSigDefIn:  AnSig; --Default value of AnSig input ports/signals
CONSTANT AnSigDefOut: AnSig; --Default value of AnSig output ports/signals

FUNCTION RealToTime (RealIn: REAL) RETURN TIME;
FUNCTION TimeToReal (TimeIn: TIME) RETURN REAL;
FUNCTION GetVolt (AnSigIn: AnSig; TimeIn: TIME) RETURN REAL;

END Mekan1;  
--*********************
--End of Mekan1 PACKAGE 
--*********************
--****************************
--Start of Mekan1 PACKAGE BODY 
--****************************
PACKAGE BODY Mekan1 IS

CONSTANT TimeRes   : TIME:=1.0 ns;            --Time resolution in TIME
CONSTANT RealRes   : REAL:=1.0E-9;           --Time resolution in REAL
CONSTANT ZT        : TIME:=0*TimeRes;        --Zero time
CONSTANT OutAccDef : PosREAL:=1.0E-3;        --Default output abs. accuracy
CONSTANT RelAccDef : PosREAL:=1.0E-2;        --Default output rel. accuracy
CONSTANT OutResDef : PosREAL:=1.0E3;         --Default output resistance
CONSTANT OutCondDef: PosREAL:=1.0/OutResDef; --Default output conductance
CONSTANT InResDef  : PosREAL:=1.0E6;         --Default INOUT resistance
CONSTANT InCondDef : PosREAL:=1.0/InResDef;  --Default INOUT conductance
CONSTANT InVoltDef : REAL:=0.0;              --Default INOUT voltage

--Default value of AnSig intput ports/signals
CONSTANT AnSigDefIn: AnSig:=(InVoltDef,0.0,ZT,ZT,InCondDef); 
--Default value of AnSig output ports/signals
CONSTANT AnSigDefOut: AnSig:=(0.0,0.0,ZT,ZT,OutCondDef); 

FUNCTION GetVolt (AnSigIn: AnSig; TimeIn : TIME) RETURN REAL IS
-- Returns the voltage at time TimeIn for the signal AnSigIn.
VARIABLE AmplIn,GradIn,StartIn,RealIn,VoltOut : REAL:=0.0 ;

  BEGIN
    --Check for consistency in StartTime and StopTime.
    ASSERT TimeIn>=AnSigIn.AnSigStart
      REPORT "Error in FUNCTION GetVolt. TimeIn<AnSigIn.AnSigStart."
      SEVERITY ERROR;
    --The following check will be excluded if event reduction is implemented.
    ASSERT TimeIn<=AnSigIn.AnSigStop OR AnSigIn.AnSigStop=ZT
      REPORT "Warning in FUNCTION GetVolt. TimeIn>AnSigIn.AnSigStop."
      SEVERITY WARNING;

    --Calculates voltage at TIME TimeIn.
    AmplIn:=AnSigIn.AnSigAmpl;
    GradIn:=AnSigIn.AnSigGrad;
    StartIn:=RealRes*REAL(AnSigIn.AnSigStart/TimeRes);
    RealIn:=RealRes*REAL(TimeIn/TimeRes);
    VoltOut:=AmplIn+(GradIn*(RealIn-StartIn));
    RETURN VoltOut;
  END GetVolt;

FUNCTION RealToTime (RealIn: REAL) RETURN TIME IS
-- Converts REAL time representation (decimal seconds) to physical type TIME.
-- Direct conversion using TimeRes and RealRes is however more effective.
VARIABLE OutTime: TIME:=ZT ;
VARIABLE IntTime,Sign: INTEGER:=0 ;
VARIABLE AbsIn: PosREAL:=0.0 ;
  BEGIN
    AbsIn:=ABS(RealIn);
    IF AbsIn/RealRes >2.0E9 THEN
      ASSERT AbsIn/RealRes<=2.0E9
        REPORT "Warning in function RealToTime. ABS(RealIn)/RealRes>2.0E9."
        SEVERITY WARNING;
      AbsIn:=2.0E9*RealRes; --To avoid overflow in TIME
    END IF; 
    IF AbsIn>1.0 THEN
      IntTime:=INTEGER(RealIn);
      OutTime:=IntTime*1.0 sec;
      RETURN OutTime;
    ELSIF AbsIn>1.0E-3 THEN
      IntTime:=INTEGER(RealIn*1.0E3);
      OutTime:=IntTime*1.0 ms;
      RETURN OutTime;
    ELSIF AbsIn>1.0E-6 THEN
      IntTime:=INTEGER(RealIn*1.0E6);
      OutTime:=IntTime*1.0 us;
      RETURN OutTime;
    ELSIF AbsIn>1.0E-9 THEN
      IntTime:=INTEGER(RealIn*1.0E9);
      OutTime:=IntTime*1.0 ns;
      RETURN OutTime;
    ELSIF AbsIn>1.0E-12 THEN
      IntTime:=INTEGER(RealIn*1.0E12);
      OutTime:=IntTime*1.0 ps;
      RETURN OutTime;
    ELSE
      IntTime:=INTEGER(RealIn*1.0E15);
      OutTime:=IntTime*1.0 fs;
      RETURN OutTime;
    END IF;
  END RealToTime;

FUNCTION TimeToReal (TimeIn: TIME) RETURN REAL IS
-- Converts physical type TIME to REAL time representation (decimal seconds).
-- Direct conversion using TimeRes and RealRes is however more effective.
VARIABLE IntTime: INTEGER:=0;
VARIABLE RealTime : REAL:=0.0;
VARIABLE AbsIn: TIME:=ZT ;
  BEGIN
    AbsIn:=ABS(TimeIn);
    IF AbsIn>1000000 sec THEN
      IntTime:=(TimeIn/1.0 sec);
      RealTime:=REAL(IntTime);
      RETURN RealTime;
    ELSIF AbsIn>1000 sec THEN
      IntTime:=(TimeIn/1.0 ms);
      RealTime:=1.0E-3*REAL(IntTime);
      RETURN RealTime;
    ELSIF AbsIn>1.0 sec THEN
      IntTime:=(TimeIn/1.0 us);
      RealTime:=1.0E-6*REAL(IntTime);
      RETURN RealTime;
    ELSIF AbsIn>1.0 ms THEN
      IntTime:=(TimeIn/1.0 ns);
      RealTime:=1.0E-9*REAL(IntTime);
      RETURN RealTime;
    ELSIF AbsIn>1.0 us THEN
      IntTime:=(TimeIn/1.0 ps);
      RealTime:=1.0E-12*REAL(IntTime);
      RETURN RealTime;
    ELSE
      IntTime:=(TimeIn/1.0 fs);
      RealTime:=1.0E-15*REAL(IntTime);
      RETURN RealTime;
    END IF;
  END TimeToReal;

END Mekan1;
--**************************
--End of Mekan1 PACKAGE BODY 
--**************************
--***************************************
--Start of Triangle wave generator ENTITY
--***************************************
USE WORK.Mekan1.ALL;

ENTITY TriGen1 IS
  GENERIC (TriFreq : PosREAL:= 1.0E3;     --Triangle wave frequency in Hz
           TriAmpl : PosREAL:= 1.0;       --Triangle wave amplitude in Volts
           OutRes  : PosREAL:= OutResDef; --Output port driving resistance
           TriDelay: PosREAL:= 0.0);      --Delay of start of wave in Seconds

  PORT (TriOut: OUT ANSIG:=AnSigDefOut);

END TriGen1;
--*************************************
--End of Triangle wave generator ENTITY
--*************************************
--********************************************
--Start of Time Resolution Check module ENTITY
--********************************************
USE WORK.Mekan1.ALL;

ENTITY TimeCheck1 IS

  PORT (Dummy: IN ANSIG:=AnSigDefIn);

END;
--******************************************
--End of Time Resolution Check module ENTITY
--******************************************
--**************************************************
--Start of Time Resolution Check module ARCHITECTURE
--**************************************************
ARCHITECTURE Simple OF TimeCheck1 IS
-- Checks if simulation time resolution is greater then TimeRes.
-- This can cause unwanted truncations in the analog models
-- and give erroneous simulation results.

  BEGIN
    PROCESS
      VARIABLE TempTime: TIME:=ZT ;
      VARIABLE TempInt: INTEGER:=0 ; 
      BEGIN
        TempTime:=NOW;
        WAIT FOR TimeRes;
        TempInt:=(NOW-TempTime)/TimeRes;
        ASSERT TempInt=1
          REPORT "TIME resolution > TimeRes. Simulation results might be unreliable!"
          SEVERITY WARNING;
        WAIT;
    END PROCESS;
END;
--************************************************
--End of Time Resolution Check module ARCHITECTURE
--************************************************
--*********************************************
--Start of Triangle wave generator ARCHITECTURE
--*********************************************

ARCHITECTURE Simple OF TriGen1 IS
--KWait=1/4 of the periode time of the wave
CONSTANT KWait: TIME:=TimeRes*INTEGER(0.25/(TriFreq*RealRes));
--KGrad=Gradient value of the wave
CONSTANT KGrad: REAL:=TriAmpl/(RealRes*REAL(KWait/TimeRes)) ; 
--KDelay=Delay time before the triangle wave will start. Default=0.0 sec.
CONSTANT KDelay: TIME:=TimeRes*INTEGER(TriDelay/RealRes);
CONSTANT OutCond: PosREAL:=1.0/OutRes;

BEGIN
  PROCESS
    VARIABLE NextEvent  : TIME:=ZT ;
    BEGIN
      IF KDelay>ZT THEN
        --Wait for TriDelay;
        NextEvent:= NOW+KDelay;
        TriOut<=(0.0,0.0,NOW,NextEvent,OutCond);
        WAIT FOR KDelay;
      END IF;
      -- First quarter of periode (Gradient >0)
      NextEvent:= NOW+KWait;
      TriOut<=(0.0,KGrad,NOW,NextEvent,OutCond);
      WAIT FOR KWait ;

      Loop1:
      LOOP
        -- Half periode with gradient<0
        NextEvent:= NOW+2*KWait;
        TriOut<=(TriAmpl,-KGrad,NOW,NextEvent,OutCond);
        WAIT FOR 2*KWait;
        -- Half periode with gradient>0
        NextEvent:= NOW+2*KWait;
        TriOut<=(-TriAmpl,KGrad,NOW,NextEvent,OutCond);
        WAIT FOR 2*KWait;
      END LOOP Loop1;

  END PROCESS;
END;
--*******************************************
--End of Triangle wave generator ARCHITECTURE
--*******************************************
--**********************************
--Start of TriGen1 Test Bench ENTITY
--**********************************
USE WORK.Mekan1.ALL;

ENTITY TriTst1 IS
--No external ports used
END;
--********************************
--End of TriGen1 Test Bench ENTITY
--********************************
--**********************************
--Start of SqwGen1 Test Bench ENTITY
--**********************************
USE WORK.Mekan1.ALL;

ENTITY SqwTst1 IS
--No external ports used
END;
--********************************
--End of SqwGen1 Test Bench ENTITY
--********************************
--*******************************************
--Start of Amp2Td1Tri Test Bench ARCHITECTURE
--*******************************************
ARCHITECTURE Amp2Td1Tri OF TriTst1 IS

COMPONENT TriGen1

GENERIC (TriDelay : PosREAL);
PORT (TriOut  : OUT AnSig);

END COMPONENT;

COMPONENT Amp2

PORT (AnSigIn  : IN AnSig;
      AnSigOut : OUT AnSig);

END COMPONENT;

COMPONENT Td1

PORT (AnSigIn  : IN AnSig;
      AnSigOut : OUT AnSig);

END COMPONENT;

COMPONENT AnSigProbe

PORT (AnSigIn   : IN AnSig;
      AmplOut  : OUT REAL;
      GradOut  : OUT REAL;
      StartOut : OUT TIME;
      StopOut  : OUT TIME;
      CondOut  : OUT PosREAL);

END COMPONENT;

COMPONENT TimeCheck1

PORT (Dummy : IN AnSig);

END COMPONENT;

SIGNAL TriOut,Amp2Out,Td1Out : AnSig:=AnSigDefOut;
SIGNAL AmplOut1,GradOut1     : REAL:=0.0;
SIGNAL StartOut1,StopOut1    : TIME:=ZT;
SIGNAL CondOut1              : PosREAL:=0.0;
SIGNAL AmplOut2,GradOut2     : REAL:=0.0;
SIGNAL StartOut2,StopOut2    : TIME:=ZT;
SIGNAL CondOut2              : PosREAL:=0.0;
SIGNAL AmplOut3,GradOut3     : REAL:=0.0;
SIGNAL StartOut3,StopOut3    : TIME:=ZT;
SIGNAL CondOut3              : PosREAL:=0.0;

BEGIN

Source:TriGen1
GENERIC MAP (1.0E-3)
PORT MAP (TriOut);

Amplifier:Amp2
PORT MAP (TriOut,
          Amp2Out);

Topdet:Td1
PORT MAP (Amp2Out,
          Td1Out);

Probe1:AnSigProbe
PORT MAP (TriOut,
          AmplOut1,
          GradOut1,
          StartOut1,
          StopOut1,
          CondOut1);

Probe2:AnSigProbe
PORT MAP (Amp2Out,
          AmplOut2,
          GradOut2,
          StartOut2,
          StopOut2,
          CondOut2);

Probe3:AnSigProbe
PORT MAP (Td1Out,
          AmplOut3,
          GradOut3,
          StartOut3,
          StopOut3,
          CondOut3);

TCModule:TimeCheck1
PORT MAP (TriOut);

END Amp2Td1Tri;
--*****************************************
--End of Amp2Td1Tri Test Bench ARCHITECTURE
--*****************************************
--****************************************
--Start of Amp2Tri Test Bench ARCHITECTURE
--****************************************
ARCHITECTURE Amp2Tri OF TriTst1 IS

COMPONENT TriGen1

PORT (TriOut  : OUT AnSig);

END COMPONENT;

COMPONENT Amp2

PORT (AnSigIn  : IN AnSig;
      AnSigOut : OUT AnSig);

END COMPONENT;

COMPONENT AnSigProbe

PORT (AnSigIn   : IN AnSig;
      AmplOut  : OUT REAL;
      GradOut  : OUT REAL;
      StartOut : OUT TIME;
      StopOut  : OUT TIME;
      CondOut  : OUT PosREAL);

END COMPONENT;

SIGNAL TriOut,Amp2Out        : AnSig:=AnSigDefOut;
SIGNAL AmplOut1,GradOut1     : REAL:=0.0;
SIGNAL StartOut1,StopOut1    : TIME:=ZT;
SIGNAL CondOut1              : PosREAL:=0.0;
SIGNAL AmplOut2,GradOut2     : REAL:=0.0;
SIGNAL StartOut2,StopOut2    : TIME:=ZT;
SIGNAL CondOut2              : PosREAL:=0.0;

BEGIN

Source:TriGen1
PORT MAP (TriOut);

Amplifier:Amp2
PORT MAP (TriOut,
          Amp2Out);

Probe1:AnSigProbe
PORT MAP (TriOut,
          AmplOut1,
          GradOut1,
          StartOut1,
          StopOut1,
          CondOut1);

Probe2:AnSigProbe
PORT MAP (Amp2Out,
          AmplOut2,
          GradOut2,
          StartOut2,
          StopOut2,
          CondOut2);

END Amp2Tri;

--**************************************
--End of Amp2Tri Test Bench ARCHITECTURE
--**************************************
--***************************************
--Start of Td1Tri Test Bench ARCHITECTURE
--***************************************
ARCHITECTURE Td1Tri OF TriTst1 IS

COMPONENT TriGen1

PORT (TriOut  : OUT AnSig);

END COMPONENT;

COMPONENT Td1
GENERIC (Tau,OutAcc,RelAcc : PosREAL );
PORT (AnSigIn  : IN AnSig;
      AnSigOut : OUT AnSig);

END COMPONENT;

COMPONENT AnSigProbe

PORT (AnSigIn   : IN AnSig;
      AmplOut  : OUT REAL;
      GradOut  : OUT REAL;
      StartOut : OUT TIME;
      StopOut  : OUT TIME;
      CondOut  : OUT PosREAL);

END COMPONENT;

SIGNAL TriOut,Td1Out         : AnSig:=AnSigDefOut;
SIGNAL AmplOut1,GradOut1     : REAL:=0.0;
SIGNAL StartOut1,StopOut1    : TIME:=ZT;
SIGNAL CondOut1              : PosREAL:=0.0;
SIGNAL AmplOut2,GradOut2     : REAL:=0.0;
SIGNAL StartOut2,StopOut2    : TIME:=ZT;
SIGNAL CondOut2              : PosREAL:=0.0;

BEGIN

Source:TriGen1
PORT MAP (TriOut);

Topdet:Td1
GENERIC MAP (1.0E-3,1.0E-3,1.0E-3)
PORT MAP (TriOut,
          Td1Out);

Probe1:AnSigProbe
PORT MAP (TriOut,
          AmplOut1,
          GradOut1,
          StartOut1,
          StopOut1,
          CondOut1);

Probe2:AnSigProbe
PORT MAP (Td1Out,
          AmplOut2,
          GradOut2,
          StartOut2,
          StopOut2,
          CondOut2);

END Td1Tri;
--*************************************
--End of Td1Tri Test Bench ARCHITECTURE
--*************************************
--*************************
--Start of Amplifier ENTITY
--*************************
USE WORK.Mekan1.ALL;

ENTITY Amp2 IS
  GENERIC (DCGain  : PosREAL:=1.0;
           Bandw   : PosREAL:=1.0E3;
           InRes   : PosREAL:=InResDef;
           InVolt  : PosREAL:=InVoltDef;
           OutRes  : PosREAL:=OutResDef;
           OutAcc  : PosREAL:=OutAccDef;
           RelAcc  : PosREAL:=RelAccDef);

--  PORT (AnSigIn: INOUT AnSig:=AnSigDefIn;
  PORT (AnSigIn:  IN  AnSig:=AnSigDefIn;
        AnSigOut: OUT AnSig:=AnSigDefOut);
END;
--***********************
--End of Amplifier ENTITY
--***********************
--****************************************************
--Start of Full Wave Rectifier and Top Detector ENTITY
--****************************************************
USE WORK.Mekan1.ALL;
USE STD.MATH.ALL;

ENTITY Td1 IS
  GENERIC (Tau     : PosREAL:=1.0E-3;
           InRes   : PosREAL:=1.0E6;
           InVolt  : PosREAL:=0.0;
           OutRes  : PosREAL:=1.0E3;
           OutAcc  : PosREAL:=1.0E-3;
           RelAcc  : PosREAL:=1.0E-2);

--  PORT (AnSigIn: INOUT AnSig:=AnSigDefIn;
  PORT (AnSigIn:  IN  AnSig:=AnSigDefIn;
        AnSigOut: OUT AnSig:=AnSigDefOut);
END;
--**************************************************
--End of Full Wave Rectifier and Top Detector ENTITY
--**************************************************
--*******************************
--Start of Amplifier ARCHITECTURE
--*******************************
USE STD.MATH.ALL;

ARCHITECTURE FastR OF Amp2 IS

 CONSTANT Ohmega  : PosREAL :=2.0*pi*Bandw;    --Bandwith in radians/sec.
 CONSTANT Tau     : PosREAL :=1.0/Ohmega;      --Time constant in seconds
 CONSTANT CondOut : PosREAL :=1.0/OutRes;    --Output port driving conductance
 CONSTANT VoltAcc : PosREAL :=OutAcc/RelAcc; --Amplitude limit for abs. acc.

 BEGIN
   PROCESS 
 VARIABLE AmplIn,GradIn,AmplOut,GradOut,DeltaAmpl  : REAL:=0.0;
 VARIABLE NewAmplOut,GradInRC,VoltIn : REAL:=0.0;
 VARIABLE ExpRC,AbsDeltaAmpl,DeltaReal,DeltaRealRel,TauSQ2 :PosREAL:=0.0;
 VARIABLE StartIn,StopIn,StartOut,StopOut,DeltaTime,MaxDeltaTime : TIME:= ZT;
   BEGIN 
       TauSQ2:=Tau*SQRT(2.0);
       WAIT ON AnSigIn; --Only used for the first event on the input
       Loop1:
       LOOP
         AmplIn:=AnSigIn.AnSigAmpl*DCGain;
         GradIn:=AnSigIn.AnSigGrad*DCGain;
         StartIn:=AnSigIn.AnSigStart;
         StopIn:=AnSigIn.AnSigStop;   
         AmplOut:=AmplOut+GradOut*RealRes*REAL((NOW-StartOut)/TimeRes);
         VoltIn:=DCGain*GetVolt(AnSigIn,NOW);
         MaxDeltaTime:=StopIn-Now;
         GradInRC:=GradIn*Tau;

       IF VoltIn=AmplOut AND GradIn=0.0 THEN
--       No further inernal events known 
         GradOut:=0.0;
         DeltaTime:=ZT;
         StartOut:=NOW;
         StopOut:=StopIn;
       ELSIF ABS(VoltIn-AmplOut)<=OutAcc AND GradIn=0.0 THEN
--       Last internal event
         DeltaReal:=TauSQ2;
         DeltaTime:=TimeRes*INTEGER(DeltaReal/RealRes);
         GradOut:=(VoltIn-AmplOut)/(RealRes*REAL(DeltaTime/TimeRes));
         StartOut:=NOW;
         StopOut:=NOW+DeltaTime;
       ELSE
         DeltaAmpl:=(VoltIn-AmplOut-GradInRC);
         AbsDeltaAmpl:=ABS(DeltaAmpl);
         DeltaReal:=TauSQ2; -- Max Step-time
         IF OutAcc<AbsDeltaAmpl THEN
--         Relative accuracy calculation
           IF VoltAcc <= (ABS(AmplOut)) THEN
           DeltaRealRel:=DeltaReal*SQRT(ABS(AmplOut)*RelAcc/AbsDeltaAmpl);
           ExpRC:=EXP(-Ohmega*DeltaRealRel);
           NewAmplOut:=AmplOut+DeltaAmpl*(1.0-ExpRC)+GradIn*DeltaRealRel;
             IF VoltAcc<=ABS(NewAmplOut) THEN
--             Realtive accuracy timstep
               DeltaReal:=DeltaRealRel;
             ELSE
--             Absolute accuracy timestep
               DeltaReal:=DeltaReal*SQRT(OutAcc/AbsDeltaAmpl);
             END IF;
           ELSE
--         Absolute accuracy timestep
           DeltaReal:=DeltaReal*SQRT(OutAcc/AbsDeltaAmpl);
           END IF;
         END IF;
         DeltaTime:=TimeRes*INTEGER(DeltaReal/RealRes);
         IF DeltaTime>MaxDeltaTime AND MaxDeltaTime>ZT THEN
             DeltaTime:=MaxDeltaTime;
         END IF;
--       Checks if DeltaTime=0
         IF DeltaTime<TimeRes THEN
           ASSERT DeltaTime>=TimeRes
             REPORT "Model Amp2. DeltaTime<TimeRes. DeltaTime <= TimeRes."
             SEVERITY WARNING;
           DeltaTime:=TimeRes;
         END IF;
--       Truncates the DeltaReal value
         DeltaReal:=RealRes*REAL(DeltaTime/TimeRes); 
         ExpRC:=EXP(-Ohmega*DeltaReal);
--       Predicts output voltage at time NOW+DeltaTime
         NewAmplOut:=AmplOut+DeltaAmpl*(1.0-ExpRC)+GradIn*DeltaReal;
         GradOut:=(NewAmplOut-AmplOut)/DeltaReal;
         StartOut:=NOW;
         StopOut:=NOW+DeltaTime;
       END IF;

       AnSigOut<=(AmplOut,GradOut,StartOut,StopOut,CondOut);

       IF DeltaTime=ZT OR DeltaTime=MaxDeltaTime THEN
         WAIT ON AnSigIn;
       ELSE 
         WAIT ON AnSigIn FOR DeltaTime;
       END IF;

     END LOOP Loop1;
   END PROCESS;
 END;
--*****************************
--End of Amplifier ARCHITECTURE
--*****************************
--**********************************************************
--Start of Full Wave Rectifier and Top Detector ARCHITECTURE
--**********************************************************
ARCHITECTURE FastR OF Td1 IS

 CONSTANT TauTime :    TIME :=TimeRes*INTEGER(Tau/RealRes);
 CONSTANT TauReal : PosREAL :=RealRes*REAL(TauTime/TimeRes);
 CONSTANT CondOut : PosREAL :=1.0/OutRes;
 CONSTANT VoltAcc : PosREAL :=OutAcc/RelAcc;
 BEGIN
   PROCESS 
     VARIABLE AmplIn,GradIn,VoltIn,AmplOut,GradOut,DeltaAmpl : REAL:=0.0;
     VARIABLE CrossRealUp,CrossRealDown,DeltaReal,AbsGradOut : REAL:=0.0;
     VARIABLE TauSQ2,NewAmplRel,DeltaRealRel : PosREAL:=0.0;
     VARIABLE StartIn,StartOut,StopIn,StopOut : TIME:= ZT ;
     VARIABLE DeltaTime,MaxDeltaIn : TIME:= ZT ;
     VARIABLE CrossTimeUp,CrossTimeDown : TIME:= ZT ;
     VARIABLE Mode : INTEGER:=0;
     BEGIN 
       TauSQ2:=Tau*SQRT(2.0);
       -- Calculations for relative accuracy
       ASSERT RelAcc<1.0 AND RelAcc>0.0
         REPORT "Model Td1(FastR). RelAcc must be >0.0 and < 1.0 !!"
         SEVERITY ERROR;
       DeltaRealRel:=Tau*SQRT(2.0*RelAcc);
       NewAmplRel:=EXP(-DeltaRealRel/Tau);

       WAIT ON AnSigIn; --Only used for the first event on the input
       Loop1:
       LOOP
         AmplIn:=AnSigIn.AnSigAmpl;
         GradIn:=AnSigIn.AnSigGrad;
         StartIn:=AnSigIn.AnSigStart;
         StopIn:=AnSigIn.AnSigStop;   
         AmplOut:=AmplOut+GradOut*RealRes*REAL((NOW-StartOut)/TimeRes);
         VoltIn:=GetVolt(AnSigIn,NOW);
         MaxDeltaIn:=StopIn-Now;

       IF ABS(VoltIn)=AmplOut AND GradIn=0.0 THEN
         Mode:=0; --The model is relaxed.
         DeltaTime:=ZT;
       ELSIF ABS(VoltIn)>=AmplOut AND VoltIn*GradIn>=0.0 THEN
         Mode:=1; --The ouput follows the input (increasing).
         DeltaTime:=MaxDeltaIn;
       ELSIF ABS(VoltIn)>=AmplOut AND ABS(GradIn)<Amplout/TauReal THEN
         Mode:=2; --The output follows the input (decreasing).
         CrossRealDown:=ABS(VoltIn/GradIn)-Tau;
         IF CrossRealDown>RealRes THEN
           CrossTimeDown:=TimeRes*INTEGER(CrossRealDown/RealRes);
           IF MaxDeltaIn >= CrossTimeDown THEN
             DeltaTime:=CrossTimeDown;
           ELSE
             DeltaTime:=MaxDeltaIn;
           END IF;
         END IF;
         --Checks if DeltaTime=0
         IF DeltaTime<TimeRes THEN
           ASSERT DeltaTime>=TimeRes
             REPORT "Model Td1. DeltaTime<TimeRes. DeltaTime <= TimeRes."
             SEVERITY WARNING;
           DeltaTime:=TimeRes;
           DeltaReal:=RealRes;
         END IF;
       ELSE  
         Mode:=-1; --The ouput follows a discharge curve.
         DeltaReal:=TauSQ2;
         IF AmplOut>OutAcc THEN
           -- Relative accuray calculation
           IF AmplOut*NewAmplRel>=VoltAcc THEN
             DeltaReal:=DeltaRealRel;
           ELSE  
             DeltaReal:=DeltaReal*SQRT(OutAcc/AmplOut);
           END IF;
         END IF;
         DeltaTime:=TimeRes*INTEGER(DeltaReal/RealRes);
         IF VoltIn*GradIn>0.0 THEN
           CrossRealUp:=(AmplOut-ABS(VoltIn))/(ABS(GradIn)+AmplOut/TauReal);
           IF CrossRealUp<RealRes THEN
             Mode:=-2; --The output (almost) follows the input.
             DeltaTime:=TimeRes;
             DeltaReal:=RealRes;
             DeltaAmpl:=Amplout-ABS(GetVolt(AnSigIn,NOW+TimeRes));
           ELSE 
           DeltaTime:=TimeRes*INTEGER(CrossRealUp/RealRes);
           END IF; 
         END IF; 
         IF DeltaTime>MaxDeltaIn AND MaxDeltaIn>=ZT THEN
             DeltaTime:=MaxDeltaIn;
         END IF;
         -- Checks for cross-over at small GradIn
         AbsGradOut:=ABS(AmplOut/TauReal);
         IF AbsGradOut>ABS(GradIn) AND VoltIn*GradIn<0.0 THEN
           CrossRealDown:=(AmplOut-ABS(VoltIn))/(AbsGradOut-ABS(GradIn));
           CrossTimeDown:=TimeRes*INTEGER(CrossRealDown/RealRes);
           IF CrossTimeDown=ZT THEN
             Mode:=-2; --The output (almost) follows the input (decreasing).
             DeltaTime:=MaxDeltaIn;
             DeltaReal:=RealRes*REAL(DeltaTime/TimeRes);
             DeltaAmpl:=Amplout-ABS(GetVolt(AnSigIn,NOW+MaxDeltaIn));
           ELSIF CrossTimeDown<DeltaTime THEN
             DeltaTime:=CrossTimeDown;
           END IF;
         END IF;
         --Checks if DeltaTime=0
         IF DeltaTime<TimeRes THEN
           ASSERT DeltaTime>=TimeRes
             REPORT "Model Td1. DeltaTime<TimeRes. DeltaTime <= TimeRes."
             SEVERITY WARNING;
           DeltaTime:=TimeRes;
           DeltaReal:=RealRes;
         END IF;
       END IF;


       IF Mode=1 THEN
         AmplOut:=ABS(VoltIn);
         GradOut:=ABS(GradIn);
         StartOut:=NOW;
         StopOut:=NOW+DeltaTime;
       ELSIF Mode=2 THEN
         AmplOut:=ABS(VoltIn);
         GradOut:=-(ABS(GradIn));
         StartOut:=NOW;
         StopOut:=NOW+DeltaTime;
       ELSIF Mode=-1 THEN
	 DeltaReal:=RealRes*REAL(DeltaTime/TimeRes);
	 GradOut:=AmplOut*(EXP(-DeltaReal/Tau)-1.0)/DeltaReal;
         StartOut:=NOW;
         StopOut:=NOW+DeltaTime;
       ELSIF Mode=-2 THEN
         GradOut:=-DeltaAmpl/DeltaReal;
         StartOut:=NOW;
         StopOut:=NOW+DeltaTime;
       ELSE 
         GradOut:=0.0;
         StartOut:=NOW;
         StopOut:=StopIn;
       END IF; 

       AnSigOut<=(AmplOut,GradOut,StartOut,StopOut,CondOut);

       IF DeltaTime=ZT OR DeltaTime=MaxDeltaIn THEN
         WAIT ON AnSigIn;
       ELSE 
         WAIT ON AnSigIn FOR DeltaTime;
       END IF;

     END LOOP Loop1;
   END PROCESS;
 END;
--********************************************************
--End of Full Wave Rectifier and Top Detector ARCHITECTURE
--********************************************************
--*************************************
--Start of Square Wave generator ENTITY
--*************************************
USE WORK.Mekan1.ALL;

ENTITY SqwGen1 IS
-- Square pulse or wave generator. SqRepeat=0.0 will give a single pulse.

  GENERIC (SqAmpl:      REAL:= 1.0;        --Pulse amplitude in Volts
           SqDelay:  PosREAL:= 1.0E-4;     --Delay time in Seconds
           SqRise:   PosREAL:= 1.0E-6;     --Rise time in Seconds
           SqHigh:   PosREAL:= 1.0E-3;     --High time in Seconds
           SqFall:   PosREAL:= 1.0E-6;     --Fall time in Seconds
           SqRepeat: PosREAL:= 0.0;        --Repeat time in Seconds
           SqRes:    PosREAL:= OutResDef); --Output resistance in Ohms

  PORT (SqwOut: OUT AnSig:=AnSigDefOut);

END;
--***********************************
--End of Square Wave generator ENTITY
--***********************************
--*******************************************
--Start of Square Wave generator ARCHITECTURE
--*******************************************
ARCHITECTURE Simple OF SqwGen1 IS

  CONSTANT RiseGrad: REAL:=SqAmpl/SqRise;
  CONSTANT FallGrad: REAL:=-SqAmpl/SqFall;
  CONSTANT OutCond: PosREAL:=1.0/SqRes;

  BEGIN
    PROCESS
      VARIABLE Ampl  : REAL:=0.0 ;
      VARIABLE Grad  : REAL:=0.0 ;
      VARIABLE Delay,Rise,High,Fall,Stop  : TIME:=ZT; 
      VARIABLE RepeatTime: TIME:=ZT;
      BEGIN
        IF SqRepeat/=0.0 THEN
          IF SqRepeat>(SqRise+SqHigh+SqFall) THEN
            RepeatTime:=TimeRes*INTEGER((SqRepeat-SqRise-SqHigh-SqFall)/RealRes);
          ELSE
            RepeatTime:=ZT;
          END IF;
        END IF;
        Delay:=TimeRes*INTEGER(SqDelay/RealRes);
        SqwOut<=(0.0,0.0,NOW,Delay,OutCond);  
        IF Delay>=TimeRes THEN
          WAIT FOR Delay;
        END IF;   

      Loop1:
      LOOP
        Rise:=TimeRes*INTEGER(SqRise/RealRes);
        Stop:=NOW+Rise;
        SqwOut<=(0.0,RiseGrad,NOW,Stop,OutCond); 
        WAIT FOR Rise;
        High:=TimeRes*INTEGER(SqHigh/RealRes);
        Stop:=NOW+High;
        SqwOut<=(SqAmpl,0.0,NOW,Stop,OutCond); 
        WAIT FOR High;
        Fall:=TimeRes*INTEGER(SqFall/RealRes);
        Stop:=NOW+Fall;
        SqwOut<=(SqAmpl,FallGrad,NOW,Stop,OutCond); 
        WAIT FOR Fall;
        IF SqRepeat=0.0 THEN
          SqwOut<=(0.0,0.0,NOW,ZT,OutCond); 
          EXIT Loop1;
        ELSE
          Stop:=NOW+RepeatTime;
          SqwOut<=(0.0,0.0,NOW,Stop,OutCond); 
          WAIT FOR RepeatTime;
        END IF;
      END LOOP Loop1;
    WAIT;
    END PROCESS;
  END;
--*****************************************
--End of Square Wave generator ARCHITECTURE
--*****************************************
--***************************
--Start of AnSig Probe ENTITY
--***************************
USE WORK.Mekan1.ALL;

ENTITY AnSigProbe IS
  PORT (AnSigIn: IN  AnSig:=AnSigDefIn ;
        AmplOut,GradOut: OUT REAL:=0.0;
        StartOut: OUT TIME:=ZT;
        StopOut: OUT TIME:=ZT;
        CondOut: OUT PosREAL:=OutCondDef);
END;
--*************************
--End of AnSig Probe ENTITY
--*************************
--********************************
--Start of AnSig Probe ARCITECTURE
--********************************
ARCHITECTURE Test OF AnSigProbe IS

BEGIN
  PROCESS (AnSigIn)

    BEGIN
      AmplOut<=AnSigIn.AnSigAmpl;
      GradOut<=AnSigIn.AnSigGrad;
      StartOut<=AnSigIn.AnSigStart;
      StopOut<=AnSigIn.AnSigStop;
      CondOut<=AnSigIn.AnSigCond;

    END PROCESS;
  END;
--********************************
--Start of AnSig Probe ARCITECTURE
--********************************
--*********************************
--Start of Amp2Td1Tri CONFIGURATION
--*********************************
CONFIGURATION Amp2Td1Tri OF TriTst1 IS

  FOR Amp2Td1Tri
    FOR ALL:AnSigProbe  USE ENTITY WORK.AnSigProbe(Test);
      END FOR;
    FOR Source:TriGen1  USE ENTITY WORK.TriGen1(Simple);
      END FOR;
    FOR Amplifier:Amp2 USE ENTITY WORK.Amp2(FastR);
      END FOR;
    FOR Topdet:Td1 USE ENTITY WORK.Td1(FastR);
      END FOR;
    FOR TCModule:TimeCheck1 USE ENTITY WORK.TimeCheck1(simple);
      END FOR;
  END FOR;
END;
--*******************************
--End of Amp2Td1Tri CONFIGURATION
--*******************************
--*******************************
--Start of Figure16 CONFIGURATION
--*******************************
CONFIGURATION Figure16 OF TriTst1 IS

  FOR Amp2Tri
    FOR ALL:AnSigProbe  USE ENTITY WORK.AnSigProbe(Test);
      END FOR;
    FOR Source:TriGen1  USE ENTITY WORK.TriGen1(Simple);
      END FOR;
    FOR Amplifier:Amp2 USE ENTITY WORK.Amp2(FastR);
      END FOR;
  END FOR;
END;
--*****************************
--End of Figure16 CONFIGURATION
--*****************************
--*******************************
--Start of Figure17 CONFIGURATION
--*******************************
CONFIGURATION Figure17 OF TriTst1 IS

  FOR Td1Tri
    FOR ALL:AnSigProbe  USE ENTITY WORK.AnSigProbe(Test);
      END FOR;
    FOR Source:TriGen1  USE ENTITY WORK.TriGen1(Simple);
      END FOR;
    FOR Topdet:Td1 USE ENTITY WORK.Td1(FastR);
      END FOR;
  END FOR;
END;
--*****************************
--End of Figure17 CONFIGURATION
--*****************************
--********************************
--Start of Amp2Td1Sqw ARCHITECTURE
--********************************
ARCHITECTURE Amp2Td1Sqw OF SqwTst1 IS

COMPONENT SqwGen1

PORT (SqwOut  : OUT AnSig);

END COMPONENT;

COMPONENT Amp2
GENERIC (DCGain,Bandw,OutAcc,RelAcc : PosREAL );
PORT (AnSigIn  : IN AnSig;
      AnsigOut : OUT AnSig);

END COMPONENT;

COMPONENT Td1
GENERIC (Tau,OutAcc,RelAcc : PosREAL );
PORT (AnSigIn  : IN AnSig;
      AnSigOut : OUT AnSig);

END COMPONENT;

COMPONENT AnSigProbe

PORT (AnSigIn   : IN AnSig:=(0.0,0.0,ZT,ZT,1.0E-3);
      AmplOut  : OUT REAL:=0.0;
      GradOut  : OUT REAL:=0.0;
      StartOut : OUT TIME:=ZT;
      StopOut  : OUT TIME:=ZT;
      CondOut  : OUT PosREAL:=0.0);

END COMPONENT;

SIGNAL SqwOut,Amplifier_Events,TopDet_Events : AnSig:=AnSigDefOut;
SIGNAL Signal_Source,GradOut1  : REAL:=0.0;
SIGNAL StartOut1,StopOut1 : TIME:=ZT;
SIGNAL CondOut1           : PosREAL:=0.0;
SIGNAL Amplifier_Output,GradOut2  : REAL:=0.0;
SIGNAL StartOut2,StopOut2 : TIME:=ZT;
SIGNAL CondOut2           : PosREAL:=0.0;
SIGNAL TopDet_Output,GradOut3  : REAL:=0.0;
SIGNAL StartOut3,StopOut3 : TIME:=ZT;
SIGNAL CondOut3           : PosREAL:=0.0;

BEGIN

Source:SqwGen1
PORT MAP (SqwOut);

Amplifier:Amp2
GENERIC MAP (1.0,1.0E3,1.0E-3,1.0E-2)
PORT MAP (SqwOut,
          Amplifier_Events);

Topdet:Td1
GENERIC MAP (0.3E-3,1.0E-3,1.0E-2)
PORT MAP (SqwOut,
          TopDet_Events);

Probe1:AnSigProbe
PORT MAP (SqwOut,
          Signal_Source,
          GradOut1,
          StartOut1,
          StopOut1,
          CondOut1);

Probe2:AnSigProbe
PORT MAP (Amplifier_Events,
          Amplifier_Output,
          GradOut2,
          StartOut2,
          StopOut2,
          CondOut2);
Probe3:AnSigProbe
PORT MAP (TopDet_Events,
          TopDet_Output,
          GradOut3,
          StartOut3,
          StopOut3,
          CondOut3);
END Amp2Td1Sqw;
--******************************
--End of Amp2Td1Sqw ARCHITECTURE
--******************************
--*****************************
--Start of Amp2Sqw ARCHITECTURE
--*****************************
ARCHITECTURE Amp2Sqw OF SqwTst1 IS

COMPONENT SqwGen1

PORT (SqwOut  : OUT AnSig);

END COMPONENT;

COMPONENT Amp2
GENERIC (DCGain,Bandw,OutAcc,RelAcc : PosREAL );
PORT (AnSigIn  : IN AnSig;
      AnsigOut : OUT AnSig);

END COMPONENT;

COMPONENT AnSigProbe

PORT (AnSigIn   : IN AnSig:=(0.0,0.0,ZT,ZT,1.0E-3);
      AmplOut  : OUT REAL:=0.0;
      GradOut  : OUT REAL:=0.0;
      StartOut : OUT TIME:=ZT;
      StopOut  : OUT TIME:=ZT;
      CondOut  : OUT PosREAL:=0.0);

END COMPONENT;

SIGNAL SqwOut,Amp2Out     : AnSig:=AnSigDefOut;
SIGNAL AmplOut1,GradOut1  : REAL:=0.0;
SIGNAL StartOut1,StopOut1 : TIME:=ZT;
SIGNAL CondOut1           : PosREAL:=0.0;
SIGNAL AmplOut2,GradOut2  : REAL:=0.0;
SIGNAL StartOut2,StopOut2 : TIME:=ZT;
SIGNAL CondOut2           : PosREAL:=0.0;

BEGIN

Source:SqwGen1
PORT MAP (SqwOut);

Amplifier:Amp2
GENERIC MAP (1.0,1.0E3,1.0E-3,1.0E-3)
PORT MAP (SqwOut,
          Amp2Out);

Probe1:AnSigProbe
PORT MAP (SqwOut,
          AmplOut1,
          GradOut1,
          StartOut1,
          StopOut1,
          CondOut1);

Probe2:AnSigProbe
PORT MAP (Amp2Out,
          AmplOut2,
          GradOut2,
          StartOut2,
          StopOut2,
          CondOut2);
END Amp2Sqw;
--***************************
--End of Amp2Sqw ARCHITECTURE
--***************************
--********************************
--Start of FrontPage CONFIGURATION
--********************************
CONFIGURATION FrontPage OF SqwTst1 IS

FOR Amp2Td1Sqw
     FOR ALL:AnSigProbe  USE ENTITY WORK.AnSigProbe(Test);
     END FOR;
     FOR Source:SqwGen1  USE ENTITY WORK.SqwGen1(Simple);
     END FOR;
     FOR Amplifier:Amp2 USE ENTITY WORK.Amp2(FastR);
     END FOR;
     FOR Topdet:Td1 USE ENTITY WORK.Td1(FastR);
     END FOR;
END FOR;
END;
--******************************
--End of FrontPage CONFIGURATION
--******************************
--*******************************
--Start of Figure15 CONFIGURATION
--*******************************
CONFIGURATION Figure15 OF SqwTst1 IS

FOR Amp2Sqw
     FOR ALL:AnSigProbe  USE ENTITY WORK.AnSigProbe(Test);
     END FOR;
     FOR Source:SqwGen1  USE ENTITY WORK.SqwGen1(Simple);
     END FOR;
     FOR Amplifier:Amp2 USE ENTITY WORK.Amp2(FastR);
     END FOR;
END FOR;
END;
--*****************************
--End of Figure15 CONFIGURATION
--*****************************
