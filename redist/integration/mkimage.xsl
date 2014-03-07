<?xml version="1.0" encoding="windows-1251" ?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns="http://www.w3.org/TR/xhtml1/strict">

<xsl:output
   method="text"
   encoding="cp1251"
/>

<xsl:template match="/">
  <xsl:apply-templates/> 
</xsl:template>

<xsl:template match="/imagedef">
!define PW_NAME '<xsl:value-of  select="product/@name"/>'
!define PW_VERSION '<xsl:value-of select="product/@version"/>'
Name "${PW_NAME} ${PW_VERSION}"
Caption "УСТАНОВКА $(^Name)"

# Defines
!define REGKEY "SOFTWARE\${PW_NAME}"
!define VERSION <xsl:value-of select="product/@version"/>
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
OutFile <xsl:value-of select="imagefile/@path"/>

InstallDir "<xsl:value-of select="installdir/@path"/>"
CRCCheck on
XPStyle on
ShowInstDetails show
VIProductVersion 4.0.0.0
VIAddVersionKey /LANG=${LANG_RUSSIAN} ProductName '<xsl:value-of select="product/@code"/>'
VIAddVersionKey ProductVersion "${VERSION}"
VIAddVersionKey /LANG=${LANG_RUSSIAN} CompanyName "${COMPANY}"
VIAddVersionKey /LANG=${LANG_RUSSIAN} FileVersion "<xsl:value-of select="product/@version"/>"
VIAddVersionKey /LANG=${LANG_RUSSIAN} FileDescription "<xsl:value-of select="product/@code"/>"
VIAddVersionKey /LANG=${LANG_RUSSIAN} LegalCopyright "PostWin Software"
InstallDirRegKey HKLM "${REGKEY}" Path
ShowUninstDetails show

ReserveFile "${NSISDIR}\Plugins\InstallOptions.dll"
ReserveFile "${NSISDIR}\Plugins\BGImage.dll"

Section ProductSection SEC0001
	SetOverwrite on
	SetOutPath $INSTDIR
	<xsl:for-each select="files">
		File /r <xsl:value-of select="@path"/>
	</xsl:for-each>
	ClearErrors
	WriteRegStr HKLM "${REGKEY}\Components" ProductSection 1
	<xsl:for-each select="link">
		CreateShortcut "<xsl:value-of select="@name"/>.lnk" "<xsl:value-of select="@target"/>"
	</xsl:for-each>
	
	<xsl:for-each select="postinstall/execute">
		ExecWait '<xsl:value-of select="@command"/>' $0
		StrCmp $0 "0" 0 +4
		DetailPrint "<xsl:value-of select="@description"/> success"
		Goto +2    
		DetailPrint "<xsl:value-of select="@description"/> error"
		DetailPrint "<xsl:value-of select="@description"/> done"
	</xsl:for-each>
SectionEnd

</xsl:template>


</xsl:stylesheet>
