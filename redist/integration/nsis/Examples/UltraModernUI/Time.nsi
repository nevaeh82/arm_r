;NSIS Ultra Modern User Interface
;Big example Script


;--------------------------------
;General

  ;Name and file
  Name "UltraModernUI TimeTest"
  OutFile "TimeTest.exe"
  SetCompressor /FINAL lzma

;  XPStyle on
;  BrandingText "UltraModernUI for NSIS"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\UltraModernUI Test"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\UltraModernUI Test" ""

;--------------------------------
;Interface Settings

!include "UMUI.nsh"


	!define MUI_ABORTWARNING
	!define MUI_UNABORTWARNING

	!define UMUI_USE_ALTERNATE_PAGE
	!define UMUI_USE_UNALTERNATE_PAGE

;--------------------------------
;Pages

	!define MUI_PAGE_CUSTOMFUNCTION_SHOW WelSetTime
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Contrib\UltraModernUI\License.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
	!define MUI_PAGE_CUSTOMFUNCTION_SHOW IFSetTime
!insertmacro MUI_PAGE_INSTFILES
	!define MUI_PAGE_CUSTOMFUNCTION_SHOW ISSetTime
!insertmacro MUI_PAGE_FINISH
	!define MUI_PAGE_CUSTOMFUNCTION_SHOW ISSetTime
!insertmacro UMUI_PAGE_ABORT


!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH
!insertmacro UMUI_UNPAGE_ABORT


Function WelSetTime
!insertmacro UMUI_LEFT_SETTIME 3
FunctionEnd

Function IFSetTime
!insertmacro UMUI_LEFT_SETTIME 2
FunctionEnd

Function ISSetTime
!insertmacro UMUI_LEFT_SETTIME 0
FunctionEnd


;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"


;--------------------------------
;Installer Sections

Section "Dummy Section" SecDummy

  sleep 10000 ;Imagine that this instruction take one minute
  
  ;Refresh the time
  !insertmacro UMUI_LEFT_SETTIME 1

  sleep 10000 ;Imagine that this instruction take one minute again

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


;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecDummy ${LANG_ENGLISH} "A test section."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
