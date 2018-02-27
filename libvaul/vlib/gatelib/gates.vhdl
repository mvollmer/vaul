package gates is

    component AND_GATE is
	port ( i1, i2 : in bit;
	       o      : out bit );
    end component;

    component XOR_GATE is
	port ( i1, i2 : in bit;
	       o      : out bit );
    end component;

    constant infinity : integer;

    component bill is
	generic ( greediness : integer := infinity);
    end component;

end;
