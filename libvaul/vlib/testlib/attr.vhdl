-- testing attributes

entity attr is
    port ( a : in bit; b : out bit );
end;

architecture a_a of attr is
begin
    process is
	variable x : NATURAL;
    begin
	x := POSITIVE'RIGHT - POSITIVE'LEFT;
	while true loop
	    if a'EVENT then
		b <= a'QUIET'STABLE'TRANSACTION'DELAYED(10sec);
	    end if;
	end loop;
    end process;
end;
