; Inno Setup installer script for Red Dot Forever 1.04

[Setup]
AppName=Red Dot Forever
AppId=Red Dot Forever
AppVerName=Red Dot Forever 1.04
AppVersion=1.04
DefaultDirName={pf}\Red Dot Forever
DefaultGroupName=Red Dot Forever
DisableProgramGroupPage=yes
UninstallDisplayIcon={app}\reddot.exe
Compression=lzma
DisableReadyPage=yes
DisableStartupPrompt=yes

[Tasks]
Name: desktopicon; Description: "Create a &desktop icon"

[Files]
Source: "reddot.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "readme.html"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Red Dot Forever"; Filename: "{app}\reddot.exe"
Name: "{group}\Read Me"; Filename: "{app}\readme.html"
Name: "{group}\Uninstall"; Filename: "{uninstallexe}"
Name: "{userdesktop}\Red Dot Forever"; Filename: "{app}\reddot.exe"; Tasks: desktopicon

[Registry]
Root: HKCU; Subkey: "Software\Red Dot Forever"; Flags: uninsdeletekey
Root: HKCU; Subkey: "Software\Red Dot Forever"; Flags: noerror; ValueType: string; ValueName: "InstallDir"; ValueData: "{app}"

[Run]
Filename: "{app}\reddot.exe"; Description: "Run Red Dot Forever right now"; Flags: postinstall nowait skipifsilent
Filename: "{app}\readme.html"; Description: "Read the manual"; Flags: postinstall shellexec skipifsilent unchecked
