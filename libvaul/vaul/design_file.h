/* parsing of VHDL text files

  Copyright (C) 1994-1996 University of Dortmund
  Department of Electrical Engineering, AG SIV
  All Rights Reserved.

  Permission to use, copy, modify and distribute this software and its
  documentation for non-commercial purposes is hereby granted,
  provided that both the copyright notice and this permission notice
  appear in all copies of the software, derivative works or modified
  versions, and any portions thereof, and that both notices appear in
  supporting documentation.

  THE UNIVERSITY OF DORTMUND ALLOWS FREE USE OF THIS SOFTWARE
  IN ITS "AS IS" CONDITION.  THE UNIVERSITY OF DORTMUND DISCLAIMS
  ANY LIABILITY OF ANY KIND FOR ANY DAMAGES WHATSOEVER RESULTING
  FROM THE USE OF THIS SOFTWARE.

  The University of Dortmund requests users of this software
  to return to

       Arbeitsgebiet Schaltungen der Signalverarbeitung
       Fakultät für Elektrotechnik
       Universität Dortmund
       44221 Dortmund, Germany

  or `msipc@nt.e-technik.uni-dortmund.de' any improvements or
  extensions that they make and grant the University of Dortmund
  the unlimited use and distribution of these changes.

  Read README.LICENSING for further information.

*/

#ifndef VAUL_DESIGN_FILE
#define VAUL_DESIGN_FILE

#include <vaul/errors.h>
#include <vaul/lexer.h>
#include <vaul/parser.h>

struct vaul_design_unit;

//  vaul_design_file kann textuelle VHDL Beschreibungen in vaul_design_units
//  umwandeln.
//
struct vaul_design_file : vaul_error_source {

    vaul_design_file(char *filename, FILE *f = NULL, bool verbose = false);
	// Öffnet die Datei <filename> zum Lesen, wenn 'f == NULL'.
	// Sonst wird aus <f> gelesen und <filename> wird nur für
	// Fehlermeldungen verwendet.

    ~vaul_design_file();

    vaul_design_unit *read_design_unit(vaul_libpool *pool);
	// Liest eine Design Unit. Meldungen über fehlerhaften
	// VHDL-Text werden nach 'stderr' ausgegeben und mitgezählt.
	// Siehe 'sync_vaul_errors()'.
	// "WORK" im VHDL-Text bezieht sich auf 'pool->worklib' und
	// andere benötigte VHDL-Libraries werden durch 'pool' gefunden.
	// Im Fehlerfall oder am Ende der Datei wird 'NULL' zurückgegeben.

    void sync_vaul_errors();
	// Produziert eine Fehlermeldung, falls 'read_design_unit'
	// vorher Fehler im VHDL-Text gefunden hat.

    void clear_vaul_errors();
	// Setzt die Anzahl von erkannten Fehlern im VHDL-Text auf 0.

    bool was_eof()		{ return parser.was_eof(); }
	// ermittelt, ob der letzte Aufruf von 'read_design_unit'
	// 'NULL' lieferte, weil der VHDL-Text zuende war.

    bool insert(vaul_libpool *pool);
	// Liest alle Design Units und fügt sie in 'pool->worklib' ein.
	// Jede gelesene Design Unit wird erst zum 'checkpoint' gegeben
	// und nur eingefügt wenn der zustimmt. Zum Schluss wird
	// 'sync_vaul_errors()' aufgerufen.

    virtual bool checkpoint(vaul_design_unit *du);
	// Kann überladen werden um ein Wort beim Lesen und Einfügen
	// mitzureden. Wird 'true' zurückgegeben, wird 'du' eingefügt,
	// sonst nicht. Siehe 'insert'.

private:
    vaul_lexer lexer;
    vaul_parser parser;
};

#endif // VAUL_DESIGN_FILE
