-- copied from SAVANT

entity ISCAS1 is
   generic (tpd_hl : time := 1 fs;
            tpd_lh : time := 1 fs);
end;

use std.textio.all;

Architecture DEMO of ISCAS1 is

signal I10 : bit;
signal I11 : bit;
signal I12 : bit;
signal I13 : bit;
signal I14 : bit;
signal O10 : bit;
signal O11 : bit;
signal I2 : bit;
signal INTERP0 : bit := '0';
signal INTERP1 : bit := '0';
signal INTERP2 : bit := '0';
signal INTERP3 : bit := '0';
signal OUTPI0 : bit := '0';
signal OUTPI1 : bit := '0';

begin

   NAND0: process(I10, I12)
        variable val,ex_value : bit := '0';
    begin
        val := not(I10 and I12);

        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                INTERP0 <= val after 1 fs;
            when '1' =>
                INTERP0 <= val after 1 fs;
            when others =>
          end case;
        end if;  
    end process;

   NAND1: process(I12, I13)
        variable val,ex_value : bit := '0';
    begin
        val := not(I12 and I13);

        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                INTERP1 <= val after 1 fs;
            when '1' =>
                INTERP1 <= val after 1 fs;
            when others =>
          end case;
        end if;  
    end process;

  NAND2: process(I11, INTERP1)
        variable val,ex_value : bit := '0';
    begin
        val := not(I11 and INTERP1);

        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                INTERP2 <= val after 1 fs;
            when '1' =>
                INTERP2 <= val after 1 fs;
            when others =>
          end case;
        end if;  
    end process;

  NAND3: process(INTERP1, I14)
        variable val,ex_value : bit := '0';
    begin
        val := not(INTERP1 and I14);

        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                INTERP3 <= val after 1 fs;
            when '1' =>
                INTERP3 <= val after 1 fs;
            when others =>
          end case;
        end if;  
    end process;

  NAND4: process(INTERP0, INTERP2)
        variable val,ex_value : bit := '0';
    begin
        val := not(INTERP0 and INTERP2);

        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                OUTPI0 <= val after 1 fs;
            when '1' =>
                OUTPI0 <= val after 1 fs;
            when others =>
          end case;
        end if;  
    end process;

  NAND5: process(INTERP2, INTERP3)
        variable val,ex_value : bit := '0';
    begin
        val := not(INTERP2 and INTERP3);

        if val /= ex_value then
          ex_value := val;
          case val is 
            when '0' =>
                OUTPI1 <= val after 1 fs;
            when '1' =>
                OUTPI1 <= val after 1 fs;
            when others =>
          end case;
        end if;  
    end process;

  BUFFER_OUT: process(OUTPI0, OUTPI1)
    begin
	O10 <= OUTPI0;
	O11 <= OUTPI1;
    end process;

p:process

 file test_in : text open READ_MODE is "iscas1.in";
 variable L : line;
 variable x : bit_vector(0 to 4);
 variable y : bit;
 begin
  while(not(endfile(test_in))) loop
   readline(test_in,L);
   read(L,x);
   read(L,y);
   I10 <= x(0);
   I11 <= x(1);
   I12 <= x(2);
   I13 <= x(3);
   I14 <= x(4);
   I2 <= y; 
   wait for 5 fs;
 end loop;
 wait;
end process;

printout:process
 variable outline : LINE;
 variable S1 : string(1 to 6) := " I1 = ";
 variable S2 : string(1 to 6) := " I2 = ";
 variable S3 : string(1 to 5) := "OP = ";
 file outfile: text open WRITE_MODE is "iscas1.out";

begin
 wait for 5 fs;
 write(outline, now);
 writeline(outfile,outline);
 write(outline,S1);
 write(outline,I10);
 write(outline,I11);
 write(outline,I12);
 write(outline,I13);
 write(outline,I14);
 write(outline,S2);
 write(outline,I2);
 writeline(outfile,outline);
 write(outline,S3);
 write(outline,O10);
 write(outline,O11);
 writeline(outfile,outline);
 wait on I10, I11, I12, I13, I14, I2;
end process;

end DEMO;
