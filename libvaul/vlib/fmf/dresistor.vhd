--------------------------------------------------------------------------------
--   File Name : dresistor.vhd
-- ------------------------------------------------------------
--  Copyright (C) 1995 Free Model Foundation
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License version 2 as
--  published by the Free Software Foundation.
--
--   MODIFICATION HISTORY :
--
--      version no: |   author: |   mod. date: |    changes made:
--      V0.8            rev3        96 FEB 12   Initial release for comments
--
-- ------------------------------------------------------------
--   PART DESCRIPTION : Digital view of a resistor
--
--   Library:       MISC
--   Technology:    
--   Part:          RESISTOR
--
--------------------------------------------------------------------------------

library IEEE;	use IEEE.std_logic_1164.all;

--------------------------------------------------------------------------------
-- ENTITY DECLARATION
--------------------------------------------------------------------------------
ENTITY dresistor IS
	PORT (
		A		: INOUT std_logic := 'Z';
		B		: INOUT std_logic := 'Z'
	);
END dresistor;

--------------------------------------------------------------------------------
-- ARCHITECTURE DECLARATION
--------------------------------------------------------------------------------
ARCHITECTURE vhdl_behavioral OF dresistor IS

    TYPE stdlogic_map IS ARRAY(std_logic) OF std_logic;
	TYPE int_stdlogic_table IS ARRAY(std_logic, std_logic) OF integer;

	SIGNAL drive_A	: std_logic := 'Z';
	SIGNAL drive_B	: std_logic := 'Z';

    CONSTANT unres_table : int_stdlogic_table := (
    --      ---------------------------------------
    --      | U  X  0  1  Z  W  L  H  -       |   |
    --      ---------------------------------------
            ( 2, 0, 0, 0, 0, 0, 0, 0, 0 ), -- | U |
            ( 1, 2, 0, 0, 0, 0, 0, 0, 0 ), -- | X |
            ( 1, 1, 2, 0, 0, 0, 0, 0, 0 ), -- | 0 |
            ( 1, 1, 0, 2, 0, 0, 0, 0, 0 ), -- | 1 |
            ( 1, 1, 1, 1, 2, 1, 1, 1, 0 ), -- | Z |
            ( 1, 1, 1, 1, 0, 2, 0, 0, 0 ), -- | W |
            ( 1, 1, 1, 1, 0, 1, 2, 0, 0 ), -- | L |
            ( 1, 1, 1, 1, 0, 1, 0, 2, 0 ), -- | H |
            ( 1, 1, 0, 0, 0, 0, 0, 0, 2 )  -- | - |
        );

    CONSTANT res_drive_val : stdlogic_map :=

	--  -----------------------------------------------
	--  |  U    X    0    1    Z    W    L    H    -  |
	--  -----------------------------------------------
        ( 'W', 'W', 'L', 'H', 'Z', 'Z', 'L', 'H', 'W' );

BEGIN

	PROCESS

		VARIABLE state	: integer;

	BEGIN

		WAIT ON A'transaction;
		state := unres_table(drive_A,A);

		case state IS

			WHEN 0 =>
				assert false report "Illegal port A/drive_A combo"
				severity failure;

			WHEN 1 =>
				IF (res_drive_val(A) /= drive_B) THEN
					B <= res_drive_val(A);
					drive_B <= res_drive_val(A);
				END IF;

			WHEN others =>
				IF (drive_B /= 'Z' and drive_B /= res_drive_val(A)) THEN
					B <= res_drive_val(A);
					drive_B <= res_drive_val(A);
				END IF;

		END CASE;

	END PROCESS;

	PROCESS

		VARIABLE state	: integer;

	BEGIN

		WAIT ON B'transaction;
		state := unres_table(drive_B,B);

		case state IS

			WHEN 0 =>
				assert false report "Illegal port B/drive_B combo"
				severity failure;

			WHEN 1 =>
				IF (res_drive_val(B) /= drive_A) THEN
					A <= res_drive_val(B);
					drive_A <= res_drive_val(B);
				END IF;

			WHEN others =>
				IF (drive_A /= 'Z' and drive_A /= res_drive_val(B)) THEN
					A <= res_drive_val(B);
					drive_A <= res_drive_val(B);
				END IF;

		END CASE;

	END PROCESS;

END vhdl_behavioral;
