;NSIS Ultra Modern User Interface

;--------------------------------
;General

  !define VERSION "1.00b1"
  !define NAME "UltraModernUI"

;--------------------------------
;Configuration

  Name "${NAME} ${VERSION}"
  OutFile "..\..\UltraModernUI ${VERSION}.exe"
  SetCompressor /FINAL lzma

  ;XPStyle on
  BrandingText "UltraModernUI version ${VERSION} for NSIS"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\NSIS"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKLM Software\NSIS ""

;--------------------------------
;Include UltraModernUI

  !include "UMUI.nsh"


;--------------------------------
;Interface Settings
	
;	!define UMUI_USE_BG "wxp"
;	!define UMUI_SKIN "green"
	
	!define MUI_ABORTWARNING
	!define MUI_UNABORTWARNING

	!define UMUI_USE_ALTERNATE_PAGE
	!define UMUI_USE_UNALTERNATE_PAGE
	
;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKLM" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\NSIS" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "UMUI Installer Language"

;--------------------------------
;Pages

  Var STARTMENU_FOLDER


	!define MUI_PAGE_CUSTOMFUNCTION_SHOW WelSetTime
!insertmacro MUI_PAGE_WELCOME

    !define MUI_LICENSEPAGE_CHECKBOX
!insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Contrib\UltraModernUI\License.txt"

	!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM" 
	!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\NSIS" 
	!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
	!define MUI_STARTMENUPAGE_DEFAULTFOLDER "NSIS\Contrib"
!insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER

	!define UMUI_CONFIRMPAGE_TEXTBOX confirm_function
!insertmacro UMUI_PAGE_CONFIRM
  
	!define MUI_PAGE_CUSTOMFUNCTION_SHOW IFSetTime
!insertmacro MUI_PAGE_INSTFILES

	!define MUI_PAGE_CUSTOMFUNCTION_SHOW ISSetTime
    !define MUI_FINISHPAGE_SHOWREADME "${NSISDIR}\Contrib\UltraModernUI\Readme.html"
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

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "French"
  !insertmacro MUI_LANGUAGE "English"

  !insertmacro MUI_LANGUAGE "German"
  !insertmacro MUI_LANGUAGE "Spanish"
  !insertmacro MUI_LANGUAGE "SimpChinese"
  !insertmacro MUI_LANGUAGE "TradChinese"
  !insertmacro MUI_LANGUAGE "Japanese"
  !insertmacro MUI_LANGUAGE "Korean"
  !insertmacro MUI_LANGUAGE "Italian"
  !insertmacro MUI_LANGUAGE "Dutch"
  !insertmacro MUI_LANGUAGE "Danish"
  !insertmacro MUI_LANGUAGE "Swedish"
  !insertmacro MUI_LANGUAGE "Norwegian"
  !insertmacro MUI_LANGUAGE "Finnish"
  !insertmacro MUI_LANGUAGE "Greek"
  !insertmacro MUI_LANGUAGE "Russian"
  !insertmacro MUI_LANGUAGE "Portuguese"
  !insertmacro MUI_LANGUAGE "PortugueseBR"
  !insertmacro MUI_LANGUAGE "Polish"
  !insertmacro MUI_LANGUAGE "Ukrainian"
  !insertmacro MUI_LANGUAGE "Czech"
  !insertmacro MUI_LANGUAGE "Slovak"
  !insertmacro MUI_LANGUAGE "Croatian"
  !insertmacro MUI_LANGUAGE "Bulgarian"
  !insertmacro MUI_LANGUAGE "Hungarian"
  !insertmacro MUI_LANGUAGE "Thai"
  !insertmacro MUI_LANGUAGE "Romanian"
  !insertmacro MUI_LANGUAGE "Latvian"
  !insertmacro MUI_LANGUAGE "Macedonian"
  !insertmacro MUI_LANGUAGE "Estonian"
  !insertmacro MUI_LANGUAGE "Turkish"
  !insertmacro MUI_LANGUAGE "Lithuanian"
  !insertmacro MUI_LANGUAGE "Catalan"
  !insertmacro MUI_LANGUAGE "Slovenian"
  !insertmacro MUI_LANGUAGE "Serbian"
  !insertmacro MUI_LANGUAGE "SerbianLatin"
  !insertmacro MUI_LANGUAGE "Arabic"
  !insertmacro MUI_LANGUAGE "Farsi"
  !insertmacro MUI_LANGUAGE "Hebrew"
  !insertmacro MUI_LANGUAGE "Indonesian"
  !insertmacro MUI_LANGUAGE "Mongolian"
  !insertmacro MUI_LANGUAGE "Luxembourgish"
  !insertmacro MUI_LANGUAGE "Albanian"
  !insertmacro MUI_LANGUAGE "Breton"
  !insertmacro MUI_LANGUAGE "Belarusian"
  !insertmacro MUI_LANGUAGE "Icelandic"
  
    !insertmacro MUI_RESERVEFILE_LANGDLL


;--------------------------------
;Installer Sections

Section "Dummy Section" SecDummy

  SetOutPath "$INSTDIR\Contrib\UltraModernUI\"
  File /r "..\..\Contrib\UltraModernUI\"

  SetOutPath "$INSTDIR\Examples\UltraModernUI\"
  File "*.nsi"
  File "*.ini"
    
  SetOutPath "$INSTDIR\Contrib\UIs\UltraModernUI\"
  File "..\..\Contrib\UIs\UltraModernUI\*.exe"
  
  SetOutPath "$INSTDIR\Contrib\Graphics\UltraModernUI\"
  File "..\..\Contrib\Graphics\UltraModernUI\*.*"
  
  SetOutPath "$INSTDIR\Include\"
  File "..\..\Include\UMUI.nsh"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
     SetOutPath "$SMPROGRAMS\$STARTMENU_FOLDER\"
     CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\UltraModernUI Readme.lnk" "$INSTDIR\Contrib\UltraModernUI\Readme.html"
     CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall UltraModernUI.lnk" "$INSTDIR\UninstallUMUI.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\UninstallUMUI.exe"
  
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "DisplayName" "$(^Name)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}" "UninstallString" '"$INSTDIR\UninstallUMUI.exe"'

SectionEnd


Section Uninstall

	Delete "$INSTDIR\UninstallUMUI.exe"
    Delete "$INSTDIR\Include\UMUI.nsh"
    RMDir "$INSTDIR\Include"
	RMDir /r "$INSTDIR\Contrib\UltraModernUI\"
	RMDir "$INSTDIR\Contrib"
	RMDir /r "$INSTDIR\Examples\UltraModernUI\"
	RMDir "$INSTDIR\Examples"
	RMDir /r "$INSTDIR\Contrib\UIs\UltraModernUI\"
	RMDir "$INSTDIR\Contrib\UIs"
	RMDir /r "$INSTDIR\Contrib\Graphics\UltraModernUI\"
	RMDir "$INSTDIR\Contrib\Graphics"
	RMDir "$INSTDIR\Contrib"
	RMDir "$INSTDIR"
	
  !insertmacro MUI_STARTMENU_GETFOLDER Application $STARTMENU_FOLDER
 
	Delete "$SMPROGRAMS\$STARTMENU_FOLDER\UltraModernUI Readme.lnk"
	Delete "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall UltraModernUI.lnk"
	RMDir "$SMPROGRAMS\$STARTMENU_FOLDER"
	RMDir "$SMPROGRAMS\$STARTMENU_FOLDER\.."
	
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${NAME}"
    
SectionEnd



Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
      
FunctionEnd

Function .onInit

    ReadRegStr $0 HKLM Software\NSIS ""
	IfErrors 0 NotNSIS

	MessageBox MB_OK|MB_ICONSTOP "NSIS is not installed on your computer.$\r$\nPlease, install NSIS (http://nsis.sourceforge.net) and then re-execute this install.$\n$\rThis install will stop."
	Quit
	
	NotNSIS:
	ClearErrors

  !insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd