#include "lang.h"
#include <locale.h>
#include <cstring>

Lang::StaticText Lang::TransMsg[] = {
  { "de", "Lookup", "Verzeichnis" },
  { "de", "Path:", "Pfad:" },
  { "de", "Files:", "Dateien:" },
  { "de", "Search Options:", "Suchoptionen:" },
  { "de", "Case Insensitive", "Groß/Kleinschreibung wird ignoriert." },
  { "de", "Regular Expression", "Regulärer Ausdruck" },
  { "de", "Search Options:", "Suchoptionen:" },
  { "de", "Go", "Los" },
  { "de", "Stop", "Anhalten" },
  { "de", "Search:", "Suche:" },
  { "de", "Result Options:", "Optionen Ergebnisse:" },
  { "de", "Number of lines\naround the keyword", "Anzahl der Zeilen vor und\nnach dem Schlüsselwort." },
  { "de", "Target Directory", "Zielverzeichnis" },
  { "de", "Highlight Keyword", "Suchwort hervorheben" },
  { "de", "C and C++ Source files", "C und C++ Quelldateien" },
  { "de", "C and C++ Header files", "C und C++ Headerdateien" },
  { "de", "C and C++ Source and Header files", "Alle C und C++ Dateien" },
  { "de", "RC Files", "RC Dateien" },
  { "de", "Open with default App", "Öffne Datei mit Standartapplikation" },
  { "de", "Open Command window", "Öffne eine Konsole" },
  { "de", "Copy Filename", "Kopiere den Dateinamen" },
  { "de", "Search %d", "Suche %d" },
  { "de", "&New Search", "&Neue Suche" },
  { "de", "&Settings", "&Einstellungen" },
  { "de", "&Quit the application", "&Programm Beenden" },
  { "de", "&Program", "&Programm" },
  { "de", "&About SupaGrep...", "&Über SupaGrep" },  
  { "de", "&Help", "&Hilfe" },
  { "de", "Start %s", "Starte %s" },
  { "de", "Open File with %s", "Öffne Datei mit %s" },
  { "de", "About SupaGrep", "Über SupaGrep" },
  { "de", 
	"SupaGrep version " VERSION ",\n"
	"Copyright (C) 2007 by Martin Oberzalek kingleo@gmx.at\n"
	"SupaGrep comes with ABSOLUTELY NO WARRANTY;\n"
	"This is free software, and you are welcome to redistribute it\n"
	"under certain conditions (GPL); Hove a look at COPYING for details.", 

	"SupaGrep Version " VERSION ",\n"
	"Copyright (C) 2007 by Martin Oberzalek kingleo@gmx.at\n"
	"Es wird KEINERLEI HAFTUNG für dieses Programm übernommen!\n"
	"Dieses Programm ist Freie Software lizensiert unter der GPL.\n"
	"Sie können dieses Programm unter einigen Bedingung, die sie bitte\n"
	"der beiliegenden Datei COPYING entnehmen, weiter vertrieben."
  },
  { NULL, NULL, NULL },
};

Lang::Lang()
{
#ifdef WIN32            
  char *loc = setlocale( LC_ALL, NULL );
#else
  char *loc = setlocale( LC_MESSAGES, NULL );
#endif
  
  if( loc == NULL )
	return;

#ifdef WIN32
  if( strstr( loc, "German" ) != NULL ) 
  {
      cl = "de_";
      if( strstr( loc, "Austria" ) != NULL )
      {
          cl = "de_AT";
      }    
  }          
#else
  cl = loc;
#endif  

  // printf( "locale: %s\n", loc );

  for( unsigned i=0; TransMsg[i].lang; i++ )
	{
	  if( std::strstr( cl, TransMsg[i].lang ) != NULL )
		{
		  StaticText st;
		  st.lang = TransMsg[i].lang;
		  st.orig = TransMsg[i].orig;
		  st.result = TransMsg[i].result;
		  messages.push_back( st );
		}
	}
}


const char * Lang::translate( const char * msg )
{
  for( unsigned i = 0; i < messages.size(); i++ ) {
	if( strstr(cl, messages[i].lang ) != NULL ) {
	  if( strcmp( messages[i].orig, msg ) == 0 ) {
		return messages[i].result;
	  }
	}
  }


  return msg;
}
