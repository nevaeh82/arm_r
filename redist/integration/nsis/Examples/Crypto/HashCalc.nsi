; HashCalc.nsi
; Example usage of Crypto plugin in recent NSIS versions (tested using 2.0)
; by GAG [Jaguar]
!define HashStr "String to be hashed"
!define HashFile "$WINDIR\notepad.exe"
!define Sep01 "*** *** *** *** *** *** *** *** "
!define Page_CryptoTest "HashCalc.ini"

!include "StrFunc.nsh"
# declare used functions
${StrNSISToIO}

Var hwnd ; Window handle of the custom page
; Hash:
Var sHash
Var sAlg
Var sData2Hash
Var sFile2Hash
Var sAbout
Var sLog

Name "Crypto Hash Calculator::Crypto Plugin Test"
Caption "Crypto Hash Calculator"
OutFile "HashCalc.exe"

SetCompressor lzma

;!define MUI_ABORTWARNING
!define MUI_ICON "lock.ico"
!define MUI_COMPONENTSPAGE_SMALLDESC

!include "MUI.nsh"

; Components page
;!insertmacro MUI_PAGE_COMPONENTS
; Testing page
Page Custom PageCryptoTest PageLeaveCryptoTest
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; Reserve files
ReserveFile "${Page_CryptoTest}"
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

ShowInstDetails show

BrandingText "Crypto Hash Calculator"

Function .onInit
   InitPluginsDir
  !insertmacro MUI_INSTALLOPTIONS_EXTRACT "${Page_CryptoTest}"
FunctionEnd

;~~~
Function PageCryptoTest
  !insertmacro MUI_HEADER_TEXT "Calculate hash" "Choose hash algorithm, type string and press 'Calculate'"
  Crypto::About ;/NOUNLOAD
  Pop $0
  ${StrNSISToIO} $sAbout $0
  ;StrCpy $sAbout $0
  !insertmacro MUI_INSTALLOPTIONS_WRITE "${Page_CryptoTest}" "Field 3" "ListItems" "MD5|SHA1|MD2|MD4|SSL3|MAC|HMAC|ErrorTest"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "${Page_CryptoTest}" "Field 5" "State" "$sAbout\r\n\r\nReady.\r\n"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "${Page_CryptoTest}" "Field 4" "State" "${HashStr}"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "${Page_CryptoTest}" "Field 2" "State" "${HashFile}" ;"$CMDLINE"

  ;!insertmacro MUI_INSTALLOPTIONS_WRITE "${Page_CryptoTest}" "Field " "Flags" "NOTIFY"
  InstallOptions::initDialog /NOUNLOAD "$PLUGINSDIR\${Page_CryptoTest}"
  ; In this mode InstallOptions returns the window handle
  Pop $hwnd
  InstallOptions::show
  Pop $0
FunctionEnd

;~~~
Function PageLeaveCryptoTest
!insertmacro MUI_INSTALLOPTIONS_READ $0 "${Page_CryptoTest}" "Settings" "State"
  StrCmp $0 0 CalcHash ; Calculate button
  StrCmp $0 6 Text ; Text
  StrCmp $0 7 Text ; File
  StrCmp $0 8 About ; About
  Abort ; Return to the page

CalcHash:
  !insertmacro MUI_INSTALLOPTIONS_READ $R0 "${Page_CryptoTest}" "Field 3" "State" ;Algo
  StrCpy $sAlg $R0

  !insertmacro MUI_INSTALLOPTIONS_READ $0 "${Page_CryptoTest}" "Field 6" "State"
  StrCmp $0 0 HashFile

;HashData:
  !insertmacro MUI_INSTALLOPTIONS_READ $R0 "${Page_CryptoTest}" "Field 4" "State" ;String to hash
  StrCmp $R0 "" 0 +3
  MessageBox MB_ICONEXCLAMATION|MB_OK "Please enter string to be hashed!"
  Abort
  StrCpy $sData2Hash $R0
  StrLen $1 $sData2Hash
  Crypto::HashData "$sAlg" "$sData2Hash" ;${HashStr} ; "$R0" ;
  Pop $0
  StrCpy $sHash $0
  StrCpy $sLog "Hash of $1 bytes string '$sData2Hash':$\r$\n$\r$\n$sAlg: [$sHash]"
  GoTo SetHash

HashFile:
  !insertmacro MUI_INSTALLOPTIONS_READ $R0 "${Page_CryptoTest}" "Field 2" "State" ;File to hash
  StrCmp $R0 "" 0 +3
  MessageBox MB_ICONEXCLAMATION|MB_OK "Please select file to be hashed!"
  Abort
  StrCpy $sFile2Hash $R0
  StrLen $1 $sData2Hash
  Crypto::HashFile "$sAlg" "$sFile2Hash" ;${HashStr} ; "$R0" ;
  Pop $0
  StrCpy $sHash $0
  StrCpy $sLog "Hash of file '$sFile2Hash':$\r$\n$\r$\n$sAlg: [$sHash]"
  ;GoTo SetHash

SetHash:
  GetDlgItem $5 $hwnd 1205
  SendMessage $5 ${WM_SETTEXT} 0 "STR:$sLog"
Abort

Text:
  !insertmacro MUI_INSTALLOPTIONS_READ $0 "${Page_CryptoTest}" "Field 7" "State"
  GetDlgItem $1 $hwnd 1201 ; PathRequest control (1200 + field 2 - 1)
  EnableWindow $1 $0
  GetDlgItem $1 $hwnd 1202 ; ... button (the following control)
  EnableWindow $1 $0
  ; Text
  IntOp $2 $0 !
  GetDlgItem $1 $hwnd 1204 ; Text
  EnableWindow $1 $2
  ; Add the disabled flag too so when we return to this page it's disabled again
  ;StrCmp $0 0 0 +4
Abort

About:
  Crypto::About
  Pop $0
  StrCpy $sAbout $0
  GetDlgItem $5 $hwnd 1205
  SendMessage $5 ${WM_SETTEXT} 0 "STR:$sAbout"
Abort

FunctionEnd

Section ""
SectionEnd
