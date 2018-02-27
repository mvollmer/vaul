--*- text -*--

library ieee;

use std.math.all;
use ieee.std_logic_1164.all;
use ieee.vital_timing.all;

package body beispiel is

    constant pi : real := 3.14;

   procedure p(x : in string);

    procedure q is
    begin
	p ( "huhu" & "hallo" & "huhu" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo" & "hallo", '0');
    end;

end package body beispiel;
