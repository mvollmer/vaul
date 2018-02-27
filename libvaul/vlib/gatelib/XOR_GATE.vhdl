entity XOR_GATE is
    port ( i1, i2 : in bit;
    	   o      : out bit );
end;

architecture behaviour of XOR_GATE is
begin
    o <= i1 xor i2;
end;
