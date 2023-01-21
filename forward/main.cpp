#include <iostream>
#include <string>

#include <windows.h>
#include <tchar.h>

#include "../tools/string_utils.h"

using namespace Tools;

std::string getCurrentDirectory(){
  TCHAR buffer[PATH_MAX];
  if(GetCurrentDirectory(PATH_MAX,buffer)) return buffer;
}

int do_system( const std::string & cmd )
{
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  LPTSTR szCmdline=_tcsdup(TEXT(cmd.c_str()));
  LPTSTR szCwd=_tcsdup(TEXT(getCurrentDirectory().c_str()));

  ZeroMemory( &si, sizeof(si) );
  // si.wShowWindow = SW_SHOWMINNOACTIVE;
  // si.dwFlags = STARTF_USESHOWWINDOW;
  si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );

  if( !CreateProcess( NULL, szCmdline, NULL, NULL, false, 0, NULL, szCwd, &si, &
pi ) )
  {
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    lpDisplayBuf = LocalAlloc(LMEM_ZEROINIT,
        (strlen((char*)lpMsgBuf)+40)*sizeof(TCHAR));

    wsprintf((char*)lpDisplayBuf,
        TEXT("error %d: %s"),
        dw, lpMsgBuf);

      std::cout << (char*) lpDisplayBuf  << std::endl;
              

      LocalFree(lpMsgBuf);
      LocalFree(lpDisplayBuf);
      // ExitProcess(dw);
      return 0;
      }
  return 1;    
}

int main( int argc, char **argv )
{
    std::string cmd = "supagrep.exe";
    std::string files;
    std::string search;
    
    if( argc <= 0 ) {
       return ( do_system(  cmd ) ? 0 : 1 );
    }    
    
    if( argc >= 1 ) {
        std::string ext = argv[0];
        ext = ext.substr( 1 ); // erster Buchstabe weg
        
        if( icase_cmp( ext, "h" ) ) {
            files = "*.h,*.hh";    
        } else if( icase_cmp( ext, "ch" ) ) {
            files = "*.h,*.hh,*.c,*.cc,*.cpp";
        } else if( icase_cmp( ext, "c" ) ) {
            files = "*.c,*.cc,*.cpp";
        } else if( icase_cmp( ext, "rc" ) ) {
            files = "*.rc";            
        } else if( icase_cmp( ext, "log" ) ) {
            files = "log.*";
        }

        for( int i = 1; i < argc; i++ )
        {
             cmd += " " + std::string(argv[i]);
        }     

        if( !files.empty() ) {
            cmd += " " + files;
        }            
        return ( do_system(  cmd ) ? 0 : 1 );
    }    
    
    return 0;
}
