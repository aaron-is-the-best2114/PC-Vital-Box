[Version]
Class=IEXPRESS
SEDVersion=3
[Options]
PackagePurpose=InstallApp
ShowInstallProgramWindow=0
HideExtractAnimation=0
UseLongFileName=0
InsideCompressed=0
CAB_FixedSize=0
CAB_ResvCodeSigning=0
RebootMode=A
InstallPrompt=%InstallPrompt%
DisplayLicense=%DisplayLicense%
FinishMessage=%FinishMessage%
TargetName=%TargetName%
FriendlyName=%FriendlyName%
AppLaunched=%AppLaunched%
PostInstallCmd=%PostInstallCmd%
AdminQuietInstCmd=%AdminQuietInstCmd%
UserQuietInstCmd=%UserQuietInstCmd%
SourceFiles=SourceFiles
[Strings]
InstallPrompt=You are about to install the software for the PC Vital Box. This will change system settings and will require a restart when finished to properly install.
DisplayLicense=C:\Program Files (x86)\VitalBox\LICENSE
FinishMessage=Congradulations!! You have successfully installed the software for the PC Vital Box. If you have any issues, please contact me through the GitHub repository.
TargetName=C:\Users\Aaron\Downloads\VitalBoxInstaller.EXE
FriendlyName=VitalBox Installer
AppLaunched=install.bat
PostInstallCmd=<None>
AdminQuietInstCmd=
UserQuietInstCmd=
FILE0="LICENSE"
FILE1="main.cpp"
FILE2="main.exe"
FILE3="PC-Vital-Box.lnk"
FILE4="start.cpp"
FILE5="start.exe"
FILE6="uninstall.bat"
FILE7="watchdog.cpp"
FILE8="watchdog.exe"
FILE9="serial.zip"
FILE10="install.bat"
[SourceFiles]
SourceFiles0=C:\Program Files (x86)\VitalBox\
[SourceFiles0]
%FILE0%=
%FILE1%=
%FILE2%=
%FILE3%=
%FILE4%=
%FILE5%=
%FILE6%=
%FILE7%=
%FILE8%=
%FILE9%=
%FILE10%=
