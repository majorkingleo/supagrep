; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "SupaGrep"
#define MyAppVersion "1.0"
#define MyAppPublisher "Martin Oberzalek"
#define MyAppURL "http://kingleo.pages.at"
#define MyAppExeName "SupaGrep.exe"

[Setup]
AppId={{F8A2923A-5F2D-47DD-9772-DDE80F9B4B4D}
AppName={#MyAppName}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppVersion={#MyAppVersion}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=true
LicenseFile=COPYING
OutputBaseFilename=supagrep_setup-{#MyAppVersion}
Compression=bzip
SolidCompression=no
InternalCompressLevel=ultra
ShowLanguageDialog=auto
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog
WizardStyle=modern

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Files]
Source: supagrep.exe; DestDir: {app}; Flags: ignoreversion
Source: dist\qrc.exe; DestDir: {app}; Flags: ignoreversion
Source: dist\qch.exe; DestDir: {app}; Flags: ignoreversion
Source: dist\qh.exe; DestDir: {app}; Flags: ignoreversion
Source: dist\qc.exe; DestDir: {app}; Flags: ignoreversion
Source: addon\WinVi32.exe; DestDir: {app}; Flags: ignoreversion
Source: addon\Adie.stx; DestDir: {app}; Flags: ignoreversion
Source: addon\adie.exe; DestDir: {app}; Flags: ignoreversion
Source: COPYING; DestDir: {app}; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: addpath.exe; DestDir: {app}; Tasks: ; Languages: ; Flags: ignoreversion
Source: dist\libbz2-1.dll; DestDir: {app}
Source: dist\libFOX-1.7-0.dll; DestDir: {app}
Source: dist\libgcc_s_seh-1.dll; DestDir: {app}
Source: dist\libjpeg-8.dll; DestDir: {app}
Source: dist\libpng16-16.dll; DestDir: {app}
Source: dist\libstdc++-6.dll; DestDir: {app}
Source: dist\libtiff-5.dll; DestDir: {app}
Source: dist\libwinpthread-1.dll; DestDir: {app}
Source: dist\libz.dll; DestDir: {app}

[Icons]
Name: {group}\{#MyAppName}; Filename: {app}\{#MyAppExeName}
Name: {commondesktop}\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: desktopicon
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: quicklaunchicon

[Run]
Filename: {app}\AddPath.exe; Parameters: {app}; WorkingDir: {app}; Description: Automatically add the {app} directory to the PATH enviroment; StatusMsg: Automatically add the {app} directory to the PATH enviroment; Flags: postinstall; Tasks: ; Languages: 
Filename: {app}\{#MyAppExeName}; Description: {cm:LaunchProgram,{#MyAppName}}; Flags: nowait postinstall skipifsilent

[UninstallRun]
Filename: {app}\AddPath.exe; Parameters: -r {app}; WorkingDir: {app}
