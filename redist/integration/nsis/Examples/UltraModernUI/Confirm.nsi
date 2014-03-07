;NSIS Ultra Modern User Interface
;Big example Script


;--------------------------------
;General

  ;Name and file
  Name "UltraModernUI Confirm"
  OutFile "Confirm.exe"
  SetCompressor /FINAL lzma

;  XPStyle on
;  BrandingText "UltraModernUI for NSIS"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\UltraModernUI Test"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\UltraModernUI Test" ""

;--------------------------------
;UltraModern Include

!include "UMUI.nsh"
!include "WinMessages.nsh"
;--------------------------------
;Interface Settings
!define UMUI_SKIN "red"
	!define MUI_ABORTWARNING
	!define MUI_UNABORTWARNING

;--------------------------------
;Pages

  Var STARTMENU_FOLDER


!insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Contrib\UltraModernUI\License.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\UltraModernUI Test" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
!insertmacro MUI_PAGE_STARTMENU "Application" $STARTMENU_FOLDER

  !define UMUI_CONFIRMPAGE_TEXTBOX confirm_function
!insertmacro UMUI_PAGE_CONFIRM

!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES


Function confirm_function
	!insertmacro UMUI_CONFIRMPAGE_TEXTBOX_ADDLINE "$(UMUI_INSTCONFIRM_TEXTBOX_DESTINATION_LOCATION)"
	!insertmacro UMUI_CONFIRMPAGE_TEXTBOX_ADDLINE "      $INSTDIR"
	!insertmacro UMUI_CONFIRMPAGE_TEXTBOX_ADDLINE ""
	
  ;Only if StartMenu Floder is selected
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
	!insertmacro UMUI_CONFIRMPAGE_TEXTBOX_ADDLINE "$(UMUI_INSTCONFIRM_TEXTBOX_START_MENU_FLODER)"
	!insertmacro UMUI_CONFIRMPAGE_TEXTBOX_ADDLINE "      $STARTMENU_FOLDER"
  !insertmacro MUI_STARTMENU_WRITE_END
FunctionEnd


  
;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"


;--------------------------------
;Installer Sections

Section "Dummy Section" SecDummy

  SetOutPath "$INSTDIR"
  
  ;ADD YOUR OWN FILES HERE...
  sleep 1000
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
  LangString DESC_SecDummy ${LANG_FRENCH} "Une section de test."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
