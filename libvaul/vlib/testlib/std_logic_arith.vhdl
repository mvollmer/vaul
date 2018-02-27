package std_logic_arith is

    type UNSIGNED is array (NATURAL range <>) of BIT;

    function "="(L: UNSIGNED; R: UNSIGNED) return BOOLEAN;
    function "/="(L: UNSIGNED; R: UNSIGNED) return BOOLEAN;

end std_logic_arith;
