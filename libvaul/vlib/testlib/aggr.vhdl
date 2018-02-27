package aggr_test is
end package;

package body aggr_test is

    type node is record
	name : string;
	count : integer;
    end record;

    type tupel is record
	a, b : integer; 
    end record;

    procedure f is
	variable n : node;
	variable name : bit_vector(1 to 4);
	variable count : integer := 5;
	variable a, b : integer;
    begin
	count := 4;
	n := ( name => "huhu", count => 4);
	name := "huhu";
	name := ( 0 | 2 => '0', 1 | 3 => '1' );
	( name, count ) := n;
	( a, b ) := tupel'(( b, a ));
    end;

end; 