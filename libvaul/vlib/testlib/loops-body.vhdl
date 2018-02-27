--*- text -*-

package loops is
end;

package body loops is

    subtype short_string_index is natural range 0 to 20;
    subtype short_string is STRING ( short_string_index );

    function hash( str : short_string; size : integer ) return integer is
	variable h : integer;
    begin
	for i in short_string_index'LEFT to short_string_index'RIGHT loop
	    h := h + 1 + CHARACTER'POS(str(i));
	end loop;
	return h mod size;
    end;

end;
