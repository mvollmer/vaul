package standard is

    -- standard package as defined by LRM-93

    type BOOLEAN is ( FALSE, TRUE );	-- order is important for v2c to work
    type BIT is ( '0', '1' );

    type CHARACTER is (
	NUL,	SOH,	STX,	ETX,	EOT,	ENQ,	ACK,    BEL,
	BS,	HT,	LF,	VT,	FF,	CR,	SO,	SI,
	DLE,	DC1,	DC2,	DC3,	DC4,	NAK,	SYN,	ETB,
	CAN,	EM,	SUB,	ESC,	FSP,	GSP,	RSP,	USP,

	' ',	'!',	'"',	'#',	'$',	'%',	'&',	''',
	'(',	')',	'*',	'+',	',',	'-',	'.',	'/',
	'0',	'1',	'2',	'3',	'4',	'5',	'6',	'7',
	'8',	'9',	':',	';',	'<',	'=',	'>',	'?',

	'@',	'A',	'B',	'C',	'D',	'E',	'F',	'G',
	'H',	'I',	'J',	'K',	'L',	'M',	'N',	'O',
	'P',	'Q',	'R',	'S',	'T',	'U',	'V',	'W',
	'X',	'Y',	'Z',	'[',	'\',	']',	'^',	'_',

	'`',	'a',	'b',	'c',	'd',	'e',	'f',	'g',
	'h',	'i',	'j',	'k',	'l',	'm',	'n',	'o',
	'p',	'q',	'r',	's',	't',	'u',	'v',	'w',
	'x',	'y',	'z',	'{',	'|',	'}',	'~',	DEL

        -- und so weiter, und so weiter...
    );

    type SEVERITY_LEVEL is ( NOTE, WARNING, ERROR, FAILURE );
    
    type INTEGER is range -231 to 231;

    type REAL is range -200.0 to 200.0;

    type TIME is range -200 to 200 units
	fs;
	ps  = 1000 fs;
	ns  = 1000 ps;
	us  = 1000 ns;
	ms  = 1000 us;
	sec = 1000 ms;
	min = 60 sec;
	hr  = 60 min;
    end units;

    subtype DELAY_LENGTH is TIME range 0fs to 200fs;

    function NOW return TIME;	-- impure

    subtype NATURAL is INTEGER range 0 to 231;
    subtype POSITIVE is INTEGER range 1 to 231;

    type STRING is array (POSITIVE range <>) of CHARACTER;
    type BIT_VECTOR is array (NATURAL range <>) of BIT;

    type FILE_OPEN_KIND is (
	READ_MODE,
	WRITE_MODE,
	APPEND_MODE
    );

    type FILE_OPEN_STATUS is (
	OPEN_OK,
	STATUS_ERROR,
	NAME_ERROR,
	MODE_ERROR
    );

end;

package math is

    constant pi : real := 3.141;

    function sqrt ( x : real ) return real;
    function exp ( x : real ) return real;

end;

