; CryptoTest.nsi
; Example usage of Crypto plugin in recent NSIS versions (tested using 2.0)
; by GAG [Jaguar]
!define HashStr "String to be hashed"
!define HashFile "$WINDIR\notepad.exe"
!define Sep01 "*** *** *** *** *** *** *** *** "

Name "Crypto Plugin Test"
Caption "Crypto Plugin Test"
OutFile "CryptoTest.exe"
Page Components
Page InstFiles

; Show details
ShowInstDetails show

BrandingText "Crypto Plugin Test"

InstType "String hashes"
InstType "All hashes"
InstType "MD5 Hash"

SubSection /e "MD5"
Section "String Data"
  SectionIn 1 2 3
  DetailPrint "${Sep01}"
  DetailPrint "MD5 String Hash"
  DetailPrint ""
  DetailPrint "Crypto::HashData 'MD5' '${HashStr}'"
  Crypto::HashData "MD5" "${HashStr}"
  Pop $0
  DetailPrint "Result:"
  DetailPrint "String: '${HashStr}'"
  DetailPrint "MD5: [$0]"
  DetailPrint ""
SectionEnd

Section "File"
  SectionIn 2
  DetailPrint "${Sep01}"
  DetailPrint "MD5 File Hash"
  DetailPrint ""
  DetailPrint "Crypto::HashFile 'MD5' '${HashFile}'"
  Crypto::HashFile "MD5" "${HashFile}"
  Pop $0
  DetailPrint "Result:"
  DetailPrint "File: '${HashFile}'"
  DetailPrint "MD5: [$0]"
  DetailPrint ""
SectionEnd
SubSectionEnd

SubSection /e "SHA1"
Section "String Data"
  SectionIn 1 2
  DetailPrint "${Sep01}"
  DetailPrint "SHA1 String Hash"
  DetailPrint ""
  DetailPrint "Crypto::HashData 'SHA1' '${HashStr}'"
  Crypto::HashData "SHA1" "${HashStr}"
  Pop $0
  DetailPrint "Result:"
  DetailPrint "String: '${HashStr}'"
  DetailPrint "SHA1: [$0]"
  DetailPrint ""
SectionEnd

Section "File"
  SectionIn 2
  DetailPrint "${Sep01}"
  DetailPrint "SHA1 File Hash"
  DetailPrint ""
  DetailPrint "Crypto::HashFile 'SHA1' '${HashFile}'"
  Crypto::HashFile "SHA1" "${HashFile}"
  Pop $0
  DetailPrint "Result:"
  DetailPrint "File: '${HashFile}'"
  DetailPrint "SHA1: [$0]"
  DetailPrint ""
SectionEnd
SubSectionEnd

Section "Errors test"
  DetailPrint "${Sep01}"
  DetailPrint "Errors test"
  DetailPrint ""
  DetailPrint "Crypto::HashData '' ''"
  Crypto::HashData "" ""
  Pop $0
  DetailPrint "Result:"
  DetailPrint "$0"
  DetailPrint ""
  DetailPrint "Crypto::HashData 'unknown_algo' ''"
  Crypto::HashData "unknown_algo" ""
  Pop $0
  DetailPrint "Result:"
  DetailPrint "$0"
SectionEnd
