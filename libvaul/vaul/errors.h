/* error handling

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

#ifndef VAUL_ERRORS_H
#define VAUL_ERRORS_H

#include <vaul/common.h>

#include <stdio.h>
#include <errno.h>

//	Fehlermeldungen bestehen aus einem Code, der den Fehler grob einordnet,
//	und einem String als genauere Fehlerbeschreibung.
//	
enum {
	vhdlerr_first_error = -5000,

	// Allgemeine Fehler
	vhdlerr_not_implemented,
		// Übergangsweise gibt's die gewünschte Funktionalität leider
		// nicht; komm' später noch mal wieder.
	vhdlerr_usage,
		// Funktion im falschen Kontext aufgerufen, Dein Fehler

	// Datei Fehler
	vhdlerr_timeout,
		// Die Datei ist durch andere Programme blockiert
	vhdlerr_eof,
		// Dateiende erreicht, obwohl noch was kommen sollte

	// Parser Fehler
	vhdlerr_errors_detected,
		// Die gelesene VHDL Datei war nicht ganz fehlerfrei
	
	// Library Fehler
	vhdlerr_lib_corrupt,
		// Die Library ist nicht in Ordnung
	vhdlerr_out_of_date,
		// Ein Verweis zeigt auf etwas, das sich inzwischen geändert hat
	vhdlerr_frozen
		// Library ist gefroren

	// und alle 'errnos', also ENOENT, EPERM, ...
};

//	vaul_error_source dient zum Speichern von Fehlermeldungen
//	und wird als Basis für alle Klassen verwendet die
//	Fehler erzeugen.
//
struct vaul_error_source {

	vaul_error_source();
	virtual ~vaul_error_source();

	bool is_error()				{ return get_error() != 0; }
	int get_error()				{ return error_code; }
	char *get_error_desc()		{ return error_desc; }

	void print_err(FILE *f = stderr, char *msg = NULL);
	void print_err(char *msg);
		// gibt die Fehlermeldung zusammen mit dem Programmnamen
		// auf f (oder stderr) aus.

protected:
	void clear_error();
		// error_code = 0; error_desc = "";

	// Funktionen, um Fehlermeldungen zu speichern.
	//	(fmt... bedeutet einen String, wie ihn printf(fmt, ...) ausgeben
	//	würde)
	//
	void set_error();
		// error_code = errno, error_desc = strerror(errno);
	void set_error(char *fmt, ...);
		// error_code = errno, error_desc = fmt... strerror(errno);
	void set_error(int code, char *fmt, ...);
		// error_code = code; error_desc = fmt...;
	void set_error(vaul_error_source *, char *fmt = "", ...);
	void set_error(vaul_error_source &, char *fmt = "", ...);
		// übernimmt komplette Fehlermeldung und setzt fmt... vor
		// die alte Fehlerbeschreibung.

private:
	int error_code;
	char *error_desc;

	void set_error_desc(char *);
	void more_error_desc(char *);
};

#endif // VAUL_ERRORS_H

