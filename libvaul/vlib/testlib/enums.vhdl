package enums is

    type std_ulogic is ( '0', '1', 'X' );
    type huhu is ( 'Z' );

    subtype std_logic is std_ulogic;

    function "and" ( a, b : in std_logic ) return std_logic;

end;

package body enums is

    procedure my_and( a, b : in std_logic; c : out std_logic ) is
    begin
	c := a and b;
    end;

    function "and" ( a, b : in std_logic ) return std_logic is
    begin
	a := 'Z';
	if (a = 'X') and (b = '1') then
	    return '1';
	else
	    return '0';
	end if;
    end;

end;

use work.enums.all;

entity enum_test is
    port ( a, b : in std_logic; c : out std_logic );

begin

    -- process is begin
	c <= a and b;
    -- end process;

end;
