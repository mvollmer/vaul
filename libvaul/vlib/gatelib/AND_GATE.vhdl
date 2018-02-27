entity AND_GATE is
    generic ( delay : DELAY_LENGTH := 2ns );
    port ( i1, i2 : in bit;
           o      : out bit );
end;

architecture behaviour of AND_GATE is
begin
    o <= i1 and i2 after delay;
end;
