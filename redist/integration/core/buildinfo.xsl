<?xml version="1.0" encoding="windows-1251" ?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns="http://www.w3.org/TR/xhtml1/strict">

<xsl:output
   method="text"
   encoding="utf-8"
/>
<xsl:template match="/">

<xsl:if test="$UP='BUILD'">
<xsl:variable name="NEXT_MAJOR_NUMBER" select="$MAJOR_NUMBER"/>
<xsl:variable name="NEXT_MINOR_NUMBER" select="$MINOR_NUMBER"/>
<xsl:variable name="NEXT_BUILD_NUMBER" select="$BUILD_NUMBER + 1"/>

BUILD_NUMBER   = <xsl:value-of select="$NEXT_BUILD_NUMBER"/>
BUILD_VERSION  = <xsl:value-of select="$NEXT_MAJOR_NUMBER"/>.<xsl:value-of select="$NEXT_MINOR_NUMBER"/>.<xsl:value-of select="$NEXT_BUILD_NUMBER"/>
BUILD_REVISION = <xsl:value-of select="$TRUNK_REVISION"/>

</xsl:if>

<xsl:if test="$UP='MINOR'">
<xsl:variable name="NEXT_MAJOR_NUMBER" select="$MAJOR_NUMBER"/>
<xsl:variable name="NEXT_MINOR_NUMBER" select="$MINOR_NUMBER + 1"/>
<xsl:variable name="NEXT_BUILD_NUMBER" select="0"/>


BUILD_NUMBER   = <xsl:value-of select="$NEXT_BUILD_NUMBER"/>
BUILD_VERSION  = <xsl:value-of select="$NEXT_MAJOR_NUMBER"/>.<xsl:value-of select="$NEXT_MINOR_NUMBER"/>.<xsl:value-of select="$NEXT_BUILD_NUMBER"/>
BUILD_REVISION = <xsl:value-of select="$TRUNK_REVISION"/>

</xsl:if>

<xsl:if test="$UP='MAJOR'">
<xsl:variable name="NEXT_MAJOR_NUMBER" select="$MAJOR_NUMBER +1"/>
<xsl:variable name="NEXT_MINOR_NUMBER" select="0"/>
<xsl:variable name="NEXT_BUILD_NUMBER" select="0"/>

BUILD_NUMBER   = <xsl:value-of select="$NEXT_BUILD_NUMBER"/>
BUILD_VERSION  = <xsl:value-of select="$NEXT_MAJOR_NUMBER"/>.<xsl:value-of select="$NEXT_MINOR_NUMBER"/>.<xsl:value-of select="$NEXT_BUILD_NUMBER"/>
BUILD_REVISION = <xsl:value-of select="$TRUNK_REVISION"/>

</xsl:if>

</xsl:template>

</xsl:stylesheet>
