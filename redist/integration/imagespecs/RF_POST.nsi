Name 'RF_POST'
Caption "”—“¿ÕŒ¬ ¿ $(^Name)"

# Defines
!define REGKEY "SOFTWARE\$(^Name)"
!define VERSION 1.0.0.0
!define COMPANY "PostWin Software"
!define URL ""

# MUI defines
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\XPUI-install.ico"
!define MUI_FINISHPAGE_NOAUTOCLOSE
!define MUI_UNFINISHPAGE_NOAUTOCLOSE
!define MUI_INSTFILESPAGE_COLORS "00FF00 000000" ;Multiple settings

; MUI Settings / Header
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_LEFT
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Nsis Header Graphics\cdfloppy1.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "${NSISDIR}\Contrib\Nsis Header Graphics\cdfloppy1.bmp"
 
; MUI Settings / Wizard
!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\BMPS\cdandfloppysblue2.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\BMPS\cdandfloppysblue2.bmp"

# Included files
!include MUI.nsh
!include FileFunc.nsh
!include Sections.nsh
!include defines.nsh


# Installer pages

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

# Installer languages
!insertmacro MUI_LANGUAGE Russian

!insertmacro GetParameters
!insertmacro GetOptions
# Installer attributes
InstProgressFlags smooth colored
BrandingText /TRIMRIGHT "PostWin Software" 
OutFile ..\rfpost_build_rel\image\SetupRfPost.exe

InstallDir "c:\PostWin Software\RF_POST"
CRCCheck on
XPStyle on
ShowInstDetails show
VIProductVersion 4.0.0.0
VIAddVersionKey /LANG=${LANG_RUSSIAN} ProductName 'RF_POST'
VIAddVersionKey ProductVersion "${VERSION}"
VIAddVersionKey /LANG=${LANG_RUSSIAN} CompanyName "${COMPANY}"
VIAddVersionKey /LANG=${LANG_RUSSIAN} FileVersion "1.0.0.0"
VIAddVersionKey /LANG=${LANG_RUSSIAN} FileDescription "RF_POST"
VIAddVersionKey /LANG=${LANG_RUSSIAN} LegalCopyright "PostWin Software"
InstallDirRegKey HKLM "${REGKEY}" Path
ShowUninstDetails show

ReserveFile "${NSISDIR}\Plugins\InstallOptions.dll"
ReserveFile "${NSISDIR}\Plugins\BGImage.dll"

Section -RF_POSTSectoin SEC0001
	SetOverwrite on
	SetOutPath $INSTDIR
	File ..\rfpost_build_rel\bin\*
	ClearErrors
	WriteRegStr HKLM "${REGKEY}\Components" RF_POSTSectoin 1
	CreateShortcut "$DESKTOP\RF_POST.lnk" "$INSTDIR\rfserver.exe"
SectionEnd
