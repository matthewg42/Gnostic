; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{EBBD90E5-890F-4622-A0CA-4DF76C582ACE}
AppName=Gnostic
AppVerName=Gnostic 1.0 alpha
AppPublisher=Matthew Gates
AppPublisherURL=http://porpoisehead.net/
AppSupportURL=http://porpoisehead.net/mysw/index.php?pgid=gnostic
AppUpdatesURL=http://porpoisehead.net/mysw/index.php?pgid=gnostic
WizardSmallImageFile=E:\Projects\Gnostic\images\ouroboros64x64.bmp
WizardImageFile=E:\Projects\Gnostic\images\wizard.bmp
WizardImageStretch=no
WizardImageBackColor=clBlack
DefaultDirName={pf}\Gnostic
DisableDirPage=yes
DefaultGroupName=Gnostic
LicenseFile=E:\Projects\Gnostic\LICENSE.txt
OutputBaseFilename=gnostic_setup
SetupIconFile=E:\Projects\Gnostic\gnostic.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\media\e\Projects\Gnostic-build-desktop\release\Gnostic.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\2010.05\mingw\bin\mingwm10.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\2010.05\mingw\bin\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\2010.05\qt\bin\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\2010.05\qt\bin\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\2010.05\qwt-6.0.0-rc5\lib\qwt.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Gnostic\LICENSE.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "E:\Projects\Gnostic\gnostic.ico"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Gnostic"; Filename: "{app}\Gnostic.exe"; IconFilename: "{app}\gnostic.ico"
Name: "{group}\{cm:ProgramOnTheWeb,Gnostic}"; Filename: "http://porpoisehead.net/mysw/index.php?pgid=gnostic"
Name: "{commondesktop}\Gnostic"; Filename: "{app}\Gnostic.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\Gnostic.exe"; Description: "{cm:LaunchProgram,Gnostic}"; Flags: nowait postinstall skipifsilent

