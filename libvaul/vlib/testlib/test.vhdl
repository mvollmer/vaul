package test is
end;

package body test is

    type c_arr is array(integer range <>, integer range <>) of character;

    procedure p(a : c_arr) is
    begin
    end;

    procedure test is
	variable s : c_arr(1 to 4, 2 to 5);
    begin	
	s := ( ( 'x', 'y', 'z' ), 3 => "huhu", 1 to 4 => "hallo" );	
	p( a(0) => "huhu", a(0, 1) => 'x' );
    end;

end;
