#include "lang.h"
#include <locale.h>
#include <cstring>
#include <utf8_util.h>

using namespace Tools;

Lang::StaticText Lang::TransMsg[] = {
  { u8"de", u8"Lookup", u8"Verzeichnis" },
  { u8"de", u8"Path:", u8"Pfad:" },
  { u8"de", u8"Files:", u8"Dateien:" },
  { u8"de", u8"Search Options:", u8"Suchoptionen:" },
  { u8"de", u8"Case Insensitive", u8"Groß/Kleinschreibung wird ignoriert." },
  { u8"de", u8"Regular Expression", u8"Regulärer Ausdruck" },
  { u8"de", u8"Search Options:", u8"Suchoptionen:" },
  { u8"de", u8"Go", u8"Los" },
  { u8"de", u8"Stop", u8"Anhalten" },
  { u8"de", u8"Search:", u8"Suche:" },
  { u8"de", u8"Result Options:", u8"Optionen Ergebnisse:" },
  { u8"de", u8"Number of lines\naround the keyword", u8"Anzahl der Zeilen vor und\nnach dem Schlüsselwort." },
  { u8"de", u8"Target Directory", u8"Zielverzeichnis" },
  { u8"de", u8"Highlight Keyword", u8"Suchwort hervorheben" },
  { u8"de", u8"C and C++ Source files", u8"C und C++ Quelldateien" },
  { u8"de", u8"C and C++ Header files", u8"C und C++ Headerdateien" },
  { u8"de", u8"C and C++ Source and Header files", u8"Alle C und C++ Dateien" },
  { u8"de", u8"RC Files", u8"RC Dateien" },
  { u8"de", u8"Open with default App", u8"Öffne Datei mit Standartapplikation" },
  { u8"de", u8"Open Command window", u8"Öffne eine Konsole" },
  { u8"de", u8"Copy Filename", u8"Kopiere den Dateinamen" },
  { u8"de", u8"Search %d", u8"Suche %d" },
  { u8"de", u8"&New Search", u8"&Neue Suche" },
  { u8"de", u8"&Settings", u8"&Einstellungen" },
  { u8"de", u8"&Quit the application", u8"&Programm Beenden" },
  { u8"de", u8"&Program", u8"&Programm" },
  { u8"de", u8"&About SupaGrep...", u8"&Über SupaGrep" },
  { u8"de", u8"&Help", u8"&Hilfe" },
  { u8"de", u8"Start %s", u8"Starte %s" },
  { u8"de", u8"Open File with %s", u8"Öffne Datei mit %s" },
  { u8"de", u8"About SupaGrep", u8"Über SupaGrep" },
  { u8"de",
	u8"SupaGrep version " VERSION ",\n"
	"Copyright (C) 2007 - 2023 by Martin Oberzalek kingleo@gmx.at\n"
	"SupaGrep comes with ABSOLUTELY NO WARRANTY;\n"
	"This is free software, and you are welcome to redistribute it\n"
	"under certain conditions (GPL); Hove a look at COPYING for details.", 

	u8"SupaGrep Version " VERSION ",\n"
	"Copyright (C) 2007 - 2023 by Martin Oberzalek kingleo@gmx.at\n"
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
  cl = (char8_t*)loc;
#endif  

  // printf( "locale: %s\n", loc );

  auto pos_dot = cl.find( '_' );
  std::u8string lang;

  if( pos_dot != std::u8string::npos ) {
	  lang = cl.substr(0,pos_dot);
  }

  for( unsigned i=0; TransMsg[i].lang; i++ )
	{
	  if( std::u8string( TransMsg[i].lang ).starts_with( lang ) )
		{
		  StaticText st;
		  st.lang = TransMsg[i].lang;
		  st.orig = TransMsg[i].orig;
		  st.result = TransMsg[i].result;

		  st.worig = Utf8Util::utf8toWString( TransMsg[i].orig );
		  st.wresult = Utf8Util::utf8toWString( TransMsg[i].result );

		  messages.push_back( st );
		}
	}
}


const char * Lang::translate( const char * msg )
{
  for( unsigned i = 0; i < messages.size(); i++ ) {
	  if( strcmp( (char*)messages[i].orig, msg ) == 0 ) {
		return (char*)messages[i].result;
	  }
  }

  return msg;
}

std::wstring Lang::translate( const std::wstring & msg )
{
  for( unsigned i = 0; i < messages.size(); i++ ) {
	  if( messages[i].worig == msg ) {
		return messages[i].wresult;
	  }
  }

  return msg;
}
