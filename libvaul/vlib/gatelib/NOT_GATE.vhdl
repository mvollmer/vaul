entity NOT_GATE is
    generic ( delay : DELAY_LENGTH := 2ns );
    port ( i : in bit;
	   o : out bit );
end;

architecture behaviour of NOT_GATE is
begin
    o <= not i after delay;
end;
