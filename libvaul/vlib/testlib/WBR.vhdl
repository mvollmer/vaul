package WBR is

    type std_ulogic is ('U','X','0','1','Z','W','L','H','-');
    type std_ulogic_vector is array(natural range <>) of std_ulogic;
    subtype std_logic is std_ulogic;
    type std_logic_vector is array(natural range <>) of std_logic;

    type unsigned is array(natural range <>) of std_logic;

  constant k_length: natural := 5;
  constant b_length: natural := 8;
  constant pr_length: natural := 13;
  constant NUL: natural := 0;

  subtype k_std_logic_vector  is std_logic_vector (1 downto 0);
  subtype k_unsigned  is unsigned (k_length-1 downto 0);

  subtype b_std_logic_vector  is std_logic_vector ((b_length) downto NUL);
  subtype b_unsigned  is unsigned (b_length-1 downto 0);

  subtype pr_std_logic_vector  is std_logic_vector (pr_length-1 downto 0);
  subtype pr_unsigned  is unsigned (pr_length-1 downto 0);

end WBR;


package body WBR is


end WBR;
