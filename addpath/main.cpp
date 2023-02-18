#include <windows.h>
#include <iostream>
#include "string_utils.h"

using namespace Tools;

#define SILENT( x ) 

static int add_path( const std::string & ext_path, 
                     HKEY skey,
                     const std::string & var,
                     bool remove )
{
    HKEY key;
            
    LONG ret = RegOpenKeyEx( skey, 
                             var.c_str(), 
                             0, 
                             KEY_READ + KEY_WRITE, 
                             &key ); 
                             
    if( ret != ERROR_SUCCESS ) {
        SILENT( std::cerr << "Cannot query key!" << std::endl );
        return 1;
    }                         

    const int BUFSIZE=1024;

    TCHAR buffer[BUFSIZE];
    DWORD dwBufLen=BUFSIZE*sizeof(TCHAR);

    ret = RegQueryValueEx( key, TEXT("Path"), NULL, NULL, (LPBYTE)buffer, &dwBufLen );
    
    if( ret != ERROR_SUCCESS ) {
        SILENT( std::cerr << "Cannot query value!" << std::endl );        
        return 2;
    }

    std::vector<std::string> sl = split_string( buffer, ";" );
    
    bool found = false;
    
    for( unsigned i = 0; i < sl.size(); i++ )
    {
         if( icase_cmp(sl[i],ext_path) ) {
             found = true;
             break;
         }    
    }

    if( !remove && found ) {
        // Pfad existiert bereits.. nix zum tuan!                        
        RegCloseKey( key ); 
        return 0;
    }

    std::string path;
    
    if( !remove ) {
        path = buffer;
        path +=';' + ext_path;
    } else {
    
        for( unsigned i = 0; i < sl.size(); i++ )
        {
          if( sl[i] == ext_path ) {
              continue;
          }   
             
          if( path.empty() ) {
              path = sl[i];
          } else {
             path += ';' + sl[i];
          }   
        }
    }

    ret = RegSetValueEx( key, TEXT("Path"), 0, REG_EXPAND_SZ, (LPBYTE)path.c_str(), path.size() );

    if( ret != ERROR_SUCCESS ) {
        SILENT( std::cerr << "cannot set value!" << std::endl );        
        RegCloseKey( key );         
        return 3;
    }        

    RegCloseKey( key ); 
    return 0;
}

int main( int argc, char **argv )
{   
    if( argc <= 1 ) {
        return 1;    
    }

    bool remove = false;
    int count = 1;
    
    if( argc >= 3 ) {
        if( icase_cmp( argv[1], "-r" ) ) {
            remove = true;
            count++;
        }    
    }
    
    std::string ext_path = argv[count];
    int rv;
    
    if( ( rv = add_path( ext_path, 
          HKEY_LOCAL_MACHINE, 
          "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment",
          remove ) ) == 0 ) {
                 
        SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0,
        (LPARAM) "Environment", SMTO_ABORTIFHUNG,
        5000, 0);
        return 0;
    }

    // try current user change
    SILENT( std::cout << "done1" << std::endl );
    
    if( ( rv = add_path( ext_path, 
                         HKEY_CURRENT_USER, 
                         "Environment",
                         remove ) ) != 0 ) {
        return rv;
    }

    DWORD dwReturnValue;
                 
    SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0,
    (LPARAM) "Environment", SMTO_ABORTIFHUNG,
    5000, 0);
    
    SILENT( std::cout << "done2" << std::endl );
    
    return 0;
}
