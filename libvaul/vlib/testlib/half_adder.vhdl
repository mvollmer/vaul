--*- text -*-

entity half_adder is
    port ( x, y : in bit;
	   s, c : out bit );
end;

library gatelib;

architecture structure of half_adder is

    use gatelib.gates.all;

    for L1 : XOR_GATE use configuration gatelib.xor_conf
		      port map ( i1, i2, o );

    for L2 : AND_GATE use entity gatelib.and_gate(behaviour)
		      port map ( i1, i2, o );

begin
    L1: XOR_GATE port map ( x, y, s );
    L2: AND_GATE port map ( x, y, c );
end;
