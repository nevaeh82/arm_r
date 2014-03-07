;NSIS Ultra Modern User Interface
;Big example Script


;--------------------------------
;General

  ;Name and file
  Name "UltraModernUI LeftMessageBoxTest"
  OutFile "LeftMessageBox.exe"
  SetCompressor /FINAL lzma

;  XPStyle on
;  BrandingText "UltraModernUI for NSIS"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\UltraModernUI Test"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\UltraModernUI Test" ""

;--------------------------------
;Interface Settings

;!define UMUI_USE_BG "wxp"
;!define UMUI_NO_BOTTOMIMAGE
!define UMUI_SKIN "purple"

!include "UMUI.nsh"


	!define MUI_ABORTWARNING
	!define MUI_UNABORTWARNING

;--------------------------------
;Pages

  Var MSGRESULT


!insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Contrib\UltraModernUI\License.txt"
!insertmacro MUI_PAGE_DIRECTORY

	!define UMUI_LEFTMESSAGEBOX_VAR MSGRESULT
	!define UMUI_LEFTMESSAGEBOX_LEFTFUNC msg_function
!insertmacro UMUI_PAGE_LEFTMESSAGEBOX 3 "Confirm Installation" "Are you sure you want to continue the installation?"

!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_UNPAGE_CONFIRM

	!define UMUI_LEFT_MESSAGEBOX_VAR MSGRESULT
	!define UMUI_LEFT_MESSAGEBOX_LEFTFUNC un.msg_function
!insertmacro UMUI_UNPAGE_LEFTMESSAGEBOX 3 "Confirm Uninstallation" "Are you sure you want to continue the uninstallation?"

!insertmacro MUI_UNPAGE_INSTFILES

Function msg_function
	;if NO
	StrCmp $MSGRESULT 3 0 +2
		;Goto Back Page
		SendMessage $HWNDPARENT "0x408" "-1" ""
FunctionEnd

Function un.msg_function
	;if NO
	StrCmp $MSGRESULT 3 0 +2
		;Goto Back Page
		SendMessage $HWNDPARENT "0x408" "-1" ""
FunctionEnd

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"


;--------------------------------
;Installer Sections

Section "Dummy Section" SecDummy

  SetOutPath "$INSTDIR"
  
  ;ADD YOUR OWN FILES HERE...
  sleep 10000
  ;Store installation folder
  WriteRegStr HKCU "Software\UltraModernUI Test" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

Section Uninstall

	DeleteRegKey HKCU "Software\UltraModernUI Test"
	
	Delete "$INSTDIR\Uninstall.exe"
	RMDir "$INSTDIR"
	Sleep 1000
SectionEnd
