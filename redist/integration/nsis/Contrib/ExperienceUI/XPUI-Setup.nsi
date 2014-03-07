; ExperienceUI for NSIS 1.11
; Setup Script
; Copyright © 2004-2006 Dan Fuhry aka dandaman32

; This program is free software; you redistribute and/or modify it
; under the terms of the zlib/libpng license.
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; merchantability or fitness for a particular purpose.  See the
; zlib/libpng license for details.

; You should have received a copy of the zlib/libpng license with
; this program; if not, visit http://xpui.sf.net/docs/?start=legal.htm.

; This script requires the following add-ons and libraries in order to compile:
; * ExperienceUI for NSIS SDK v1.11 or later - http://xpui.sourceforge.net/
; * Component Manager 0.2 or later - http://xpui.sourceforge.net/page/compmgr/

;-------------------------------------
; First Things First

SetCompressor /FINAL /SOLID lzma

;-------------------------------------
; Declarations

Name "ExperienceUI for NSIS"
Caption "ExperienceUI SDK Setup"
OutFile "..\..\ExperienceUI-1.11.exe"
CheckBitmap "${NSISDIR}\Contrib\Graphics\Checks\Modern.bmp"
!define /date XPUI_BUILD_ID "%H%M-%y%m%d"
!define /date XPUI_BUILD_ID_DECIMAL "%y%m%d.%H%M"
!define XPUI_SYSDIR "."

InstType "Full Install"
InstType "Standard Install"
InstType "Minimal Install"

;-------------------------------------
; ExperienceUI Settings

!define XPUI_DIRECTORYPAGE_TEXT "Setup will install $(^Name) in the following folder. To install in this folder, leave the text below as-is. To install in a different folder, enter one below, or click Browse. $_CLICK$\n$\nNOTE: You need to have write access to the directory 2 levels up from the install directory,$\ni. e. if you install in C:\Program Files\NSIS\Contrib\ExperienceUI, you need to have write access to C:\Program Files\NSIS."
!define XPUI_ABORTWARNING
!define XPUI_UNABORTWARNING
!define XPUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\XPUI-Install.ico"
!define XPUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\XPUI-UnInstall.ico"
!define XPUI_BRANDINGTEXT "ExperienceUI for NSIS ${XPUI_VERSION}"
!define XPUI_SKIN "Windows XP"
!define XPUI_LICENSEPAGE_CHECKBOX
!define XPUI_DISABLEBG
!define XPUI_FINISHPAGE_DOCS
!define XPUI_FINISHPAGE_DOCS_FILE "$INSTDIR\..\..\Docs\ExperienceUI\open_popup.hta"
!define XPUI_VERBOSE 4

!define CM_STARTMENU_REG_ROOT HKLM
!define CM_STARTMENU_REG_KEY  "Software\NSIS\ExperienceUI"
!define CM_STARTMENU_REG_VALUE "StartMenuFolder"

!include XPUI.nsh
!include CM.nsh

;-------------------------------------
; Compile-time Options

; Define to include the "A Better Installer" splash screen (adds a whopping 100KB)
  !define XPUI_SETUP_SPLASH

;-------------------------------------
; Variables

Var SMFOLDER
Var INSTALL_EX
Var INSTALL_DOCS
Var INSTALL_UTILS

!insertmacro XPUI_PAGE_STARTMENU_INIT App $SMFOLDER

;-------------------------------------
; Version Info

VIProductVersion "1.11.${XPUI_BUILD_ID_DECIMAL}"
VIAddVersionKey /LANG=1033 "FileVersion" "1.11.${XPUI_BUILD_ID}"
VIAddVersionKey /LANG=1033 "ProductVersion" "1.11.${XPUI_BUILD_ID}"
VIAddVersionKey /LANG=1033 "ProductName" "ExperienceUI for NSIS"
VIAddVersionKey /LANG=1033 "Comments" "This installer was written by Dan Fuhry using Nullsoft Scriptable Install System (http://nsis.sourceforge.net)"
VIAddVersionKey /LANG=1033 "CompanyName" "Dan Fuhry"
VIAddVersionKey /LANG=1033 "LegalTrademarks" "ExperienceUI SDK by Dan Fuhry. Copyright © 2004-2005 Dashboard Software Ltd."
VIAddVersionKey /LANG=1033 "LegalCopyright" "Copyright © 2004-2005 Dan Fuhry"
VIAddVersionKey /LANG=1033 "FileDescription" "ExperienceUI SDK Setup/Maintenance Program"
VIAddVersionKey /LANG=1033 "SpecialBuild" "ExperienceUI for NSIS Software Development Kit Setup, built on ${__TIMESTAMP__} (${XPUI_BUILD_ID})"

;-------------------------------------
; Initialization

Function .onInit
  !ifdef XPUI_SETUP_SPLASH
  InitPluginsDir
  SetOutPath $PLUGINSDIR
  File LargeLogo.gif
  newadvsplash::show /NOUNLOAD 2000 2000 2000 -1 "$PLUGINSDIR\LargeLogo.gif"
  Delete $PLUGINSDIR\LargeLogo.gif
  !endif
  ReadRegStr $0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\NSIS" "UninstallString"
  StrCpy $0 $0 "" 1
  StrCpy $0 $0 -17
  IfFileExists $0\MakeNSIS.exe FoundNSIS
    MessageBox MB_YESNO|MB_ICONEXCLAMATION "Setup was unable to find NSIS on your system.  The ExperienceUI SDK requires NSIS in order to function.$\n$\nDo you want to go to the NSIS website now?" IDNO +2
      ExecShell open "http://nsis.sourceforge.net/"
    Abort
  FoundNSIS:
  StrCpy $INSTDIR "$0\Contrib\ExperienceUI"
FunctionEnd

;-------------------------------------
; Sections

!verbose 0
!include ExperienceUIFileList.nsh
Section
  ReadRegStr $0 HKLM "Software\Microsoft\Windows NT\CurrentVersion" "RegisteredOwner"
  StrCmp $0 "Daniel C. Fuhry" +2
  MessageBox MB_OK|MB_ICONINFORMATION "Please take some time to read the ExperienceUI documentation because the ExperienceUI is different from other UIs and as a result requires a slightly different script layout."
SectionEnd
!verbose 4

SubSection "Downloadable extras"
  Section "Page background images - 187KB"
    DetailPrint "Downloading file: http://xpui.sf.net/bin/1.11/bgimages.eup (187KB, <1min)"
    download:
    nsisdl::download_quiet "http://xpui.sourceforge.net/bin/1.11/bgimages.eup" "$PLUGINSDIR\bg.eup"
    Pop $0
    StrCmp $0 success dlgood
      MessageBox MB_OK|MB_ICONEXCLAMATION "Error downloading patch file: $0"
      Return
    dlgood:
    DCryptDll::MD5Hash "FS" "$PLUGINSDIR\bg.eup" "--End--"
    Pop $0
      StrCmp $0 OK ok1
      MessageBox MB_OK|MB_ICONSTOP "Error verifying MD5 hash of download:$\n$0"
      Goto done
    ok1:
    Pop $0
    StrCmp $0 "83972708FD0EBC9373BDEF81EE006881" Patch ; Not the signature hash, just the regular MD5
      Pop $9
      Pop $9
      Pop $9
      Pop $9
      Pop $9
      Pop $9
      Pop $9
      Pop $9
      Pop $9 ; GET RID OF THE STACK!!!!
      StrCpy $0 ""
      StrCpy $1 ""
      StrCpy $2 ""
      StrCpy $3 ""
      StrCpy $4 ""
      messagebox::show MB_ICONEXCLAMATION|MB_TOPMOST|MB_DEFBUTTON1 "Bad Download" "0,103" "The ExperienceUI installer has detected that the MD5 hash of the official download and the hash of the downloaded file are different.$\n$\nThe most likely reason for this is a corrupt download.  If you want to try to download the file again, click Try Again.$\n$\nIt is also possible, though very unlikely, that someone has broken into the ExperienceUI web server and changed the patch file to something potentially malicious.  If you think that is the case, please press Skip.$\n$\nTo try to install the patch file anyway, click Ignore.$\n" "IDTRYAGAIN" "Skip" IDIGNORE
      Pop $1
      StrCmp $1 1 download
      StrCmp $1 2 done
    Patch:
      DetailPrint "Installing background image files..."
      ExecWait '"$INSTDIR\Utils\PatchInstaller.exe" $PLUGINSDIR\bg.eup /Silent'
    Done:
  SectionEnd
SubSectionEnd

!macro CM_UNINST
  Delete $INSTDIR\Utils\Updater.exe
  Delete $INSTDIR\Utils\PatchInstaller.exe
  SetOutPath "C:\" ; Reset the output dir and close any handles still open
  ${CMInclude} Dirlist # doing this three
  ${CMInclude} Dirlist # times to make sure
  ${CMInclude} Dirlist # everything's gone
  RMDir $INSTDIR # Will delete $INSTDIR if it's empty
!macroend

;-------------------------------------
; Pages

!verbose 0
!insertmacro CM_LANGUAGE English
!insertmacro CM_PAGES

!define XPUI_WELCOMEPAGE2_TEXT_TOP "Welcome to ExperienceUI 1.11."
!define XPUI_WELCOMEPAGE2_TEXT     "Welcome to the ExperienceUI, the user interface that makes NSIS the installer your programs truly deserve.\r\n\r\nClick Next to start.\r\n\r\nThis release is version 1.11 and it was built on ${__TIMESTAMP__}."

${Page} Welcome2
${LicensePage} License.rtf
${Page} Components
${Page} Directory
!insertmacro XPUI_PAGE_STARTMENU_SHOW App
${Page} InstConfirm
${Page} InstFiles
${Page} Finish
${Page} Abort

${UnPage} Welcome2
${UnPage} UnConfirm
${UnPage} InstConfirm
${UnPage} InstFiles
${UnPage} Finish
${UnPage} Abort

!insertmacro XPUI_LANGUAGE "English"
!insertmacro CM_FILES
!verbose 4