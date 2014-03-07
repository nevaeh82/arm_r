;NSIS_SkinCrafter_Plugin
;Basic Example Script

Function .onInit
    ;User defined skin
    SetOutPath $TEMP
    File /oname=Skinastic.skf "LEDWidget.skf"
    NSIS_SkinCrafter_Plugin::skin /NOUNLOAD $TEMP\LEDWidget.skf
    Delete $TEMP\LEDWidget.skf

    ;Default Skin
    ;NSIS_SkinCrafter_Plugin::skin /NOUNLOAD
FunctionEnd

Function .onGUIEnd
	NSIS_SkinCrafter_Plugin::destroy
FunctionEnd

Function un.onInit
    ;User defined skin
    ;SetOutPath $TEMP
    ;File /oname=Skinastic.skf "LEDWidget.skf"
    ;NSIS_SkinCrafter_Plugin::skin /NOUNLOAD $TEMP\LEDWidget.skf
    ;Delete $TEMP\LEDWidget.skf

    ;Default Skin
    NSIS_SkinCrafter_Plugin::skin /NOUNLOAD
FunctionEnd

Function un.onGUIEnd
	NSIS_SkinCrafter_Plugin::destroy
FunctionEnd

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;General

  ;Name and file
  Name "NSIS SkinCrafter Plugin Test"
  OutFile "sample.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\NSIS SkinCrafter Plugin Test"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\NSIS SkinCrafter Plugin" ""

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Dummy Section" SecDummy

  SetOutPath "$INSTDIR"

  ;ADD YOUR OWN FILES HERE...

  ;Store installation folder
  WriteRegStr HKCU "Software\NSIS SkinCrafter Plugin" "" $INSTDIR

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecDummy ${LANG_ENGLISH} "A test section."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\NSIS SkinCrafter Plugin"

SectionEnd