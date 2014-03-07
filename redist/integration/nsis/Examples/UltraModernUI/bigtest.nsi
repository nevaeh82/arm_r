;NSIS Ultra Modern User Interface
;Big example Script


;--------------------------------
;General

  ;Name and file
  Name "UltraModernUI BigTest"
  OutFile "BigTest.exe"
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
!define UMUI_SKIN "red"

;  !define MUI_COMPONENTSPAGE_NODESC
!define MUI_COMPONENTSPAGE_SMALLDESC
!include "UMUI.nsh"


	!define MUI_ABORTWARNING
	!define MUI_UNABORTWARNING

	!define UMUI_USE_ALTERNATE_PAGE
	!define UMUI_USE_UNALTERNATE_PAGE

;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\UltraModernUI Test" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------
;Pages

  Var STARTMENU_FOLDER
  Var MSGRESULT



	!define MUI_PAGE_CUSTOMFUNCTION_SHOW WelSetTime
!insertmacro MUI_PAGE_WELCOME

  Page custom CustomPageA
    
  !define MUI_LICENSEPAGE_CHECKBOX
!insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Contrib\UltraModernUI\License.txt"


!insertmacro MUI_PAGE_COMPONENTS

!insertmacro MUI_PAGE_DIRECTORY

  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\UltraModernUI Test" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
!insertmacro MUI_PAGE_STARTMENU "Application" $STARTMENU_FOLDER

  !define UMUI_CONFIRMPAGE_TEXTBOX confirm_function
!insertmacro UMUI_PAGE_CONFIRM

	!define UMUI_LEFTMESSAGEBOX_VAR MSGRESULT
	!define UMUI_LEFTMESSAGEBOX_LEFTFUNC msg_function
!insertmacro UMUI_PAGE_LEFTMESSAGEBOX 3 "Confirm Installation" "Are you sure you want to continue the installation?"

	!define MUI_PAGE_CUSTOMFUNCTION_SHOW IFSetTime
	!define UMUI_INSTFILEPAGE_ENABLE_CANCEL_BUTTON cancel_function
!insertmacro MUI_PAGE_INSTFILES

	!define MUI_PAGE_CUSTOMFUNCTION_SHOW ISSetTime
	!define MUI_FINISHPAGE_RUN "C:\Notepad.exe"
	!define MUI_FINISHPAGE_SHOWREADME "C:\Notepad.exe"
    !define MUI_FINISHPAGE_LINK "UltraModernUI Home Page"
    !define MUI_FINISHPAGE_LINK_LOCATION "http://www.ultramodernui.com4.ws"
!insertmacro MUI_PAGE_FINISH

	!define MUI_PAGE_CUSTOMFUNCTION_SHOW ISSetTime
    !define UMUI_ABORTPAGE_LINK "UltraModernUI Home Page"
    !define UMUI_ABORTPAGE_LINK_LOCATION "http://www.ultramodernui.com4.ws"
!insertmacro UMUI_PAGE_ABORT


	!define MUI_PAGE_CUSTOMFUNCTION_SHOW un.WelSetTime
!insertmacro MUI_UNPAGE_WELCOME

!insertmacro MUI_UNPAGE_CONFIRM

	!define UMUI_LEFT_MESSAGEBOX_VAR MSGRESULT
	!define UMUI_LEFT_MESSAGEBOX_LEFTFUNC un.msg_function
!insertmacro UMUI_UNPAGE_LEFTMESSAGEBOX 3 "Confirm Uninstallation" "Are you sure you want to continue the uninstallation?"

	!define MUI_PAGE_CUSTOMFUNCTION_SHOW un.IFSetTime
!insertmacro MUI_UNPAGE_INSTFILES

	!define MUI_PAGE_CUSTOMFUNCTION_SHOW un.ISSetTime
	!define MUI_FINISHPAGE_LINK "UltraModernUI Home Page"
	!define MUI_FINISHPAGE_LINK_LOCATION "http://www.ultramodernui.com4.ws"
!insertmacro MUI_UNPAGE_FINISH

	!define MUI_PAGE_CUSTOMFUNCTION_SHOW un.ISSetTime
	!define UMUI_ABORTPAGE_LINK "UltraModernUI Home Page"
	!define UMUI_ABORTPAGE_LINK_LOCATION "http://www.ultramodernui.com4.ws"
!insertmacro UMUI_UNPAGE_ABORT


Function cancel_function
MessageBox MB_OK "bouuu"
FunctionEnd

Function confirm_function
	!insertmacro UMUI_CONFIRMPAGE_TEXTBOX_ADDLINE "$(UMUI_INSTCONFIRM_TEXTBOX_DESTINATION_LOCATION)"
	!insertmacro UMUI_CONFIRMPAGE_TEXTBOX_ADDLINE "      $INSTDIR"
	!insertmacro UMUI_CONFIRMPAGE_TEXTBOX_ADDLINE ""
	!insertmacro UMUI_CONFIRMPAGE_TEXTBOX_ADDLINE "$(UMUI_INSTCONFIRM_TEXTBOX_START_MENU_FLODER)"
	!insertmacro UMUI_CONFIRMPAGE_TEXTBOX_ADDLINE "      $STARTMENU_FOLDER"
FunctionEnd


Function WelSetTime
!insertmacro UMUI_LEFT_SETTIME 2
FunctionEnd

Function IFSetTime
!insertmacro UMUI_LEFT_SETTIME 1
FunctionEnd

Function ISSetTime
!insertmacro UMUI_LEFT_SETTIME 0
FunctionEnd

Function un.WelSetTime
!insertmacro UMUI_LEFT_SETTIME 2
FunctionEnd

Function un.IFSetTime
!insertmacro UMUI_LEFT_SETTIME 1
FunctionEnd

Function un.ISSetTime
!insertmacro UMUI_LEFT_SETTIME 0
FunctionEnd

Function CustomPageA
  !insertmacro MUI_INSTALLOPTIONS_EXTRACT "ioA.ini"
  !insertmacro MUI_HEADER_TEXT "def" "abc"
  !insertmacro MUI_INSTALLOPTIONS_DISPLAY "ioA.ini"
FunctionEnd

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

  !insertmacro MUI_LANGUAGE "French"
  !insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_RESERVEFILE_LANGDLL


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


;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecDummy ${LANG_ENGLISH} "A test section."
  LangString DESC_SecDummy ${LANG_FRENCH} "Une section de test."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd

Function .onInit

  !insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd