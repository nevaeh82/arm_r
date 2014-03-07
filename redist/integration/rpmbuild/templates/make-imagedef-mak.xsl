<?xml version="1.0" encoding="windows-1251" ?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns="http://www.w3.org/TR/xhtml1/strict">

<xsl:output
   method="text"
   encoding="utf-8"
/>

<xsl:template match="/">
  <xsl:apply-templates/> 
</xsl:template>

<xsl:template match="/imagedef">


productname = <xsl:value-of select="product/@code"/>
pkgname     = <xsl:value-of select="concat(product/@code, '-', product/@version)"/>
pkgimage    = <xsl:value-of select="translate(imagefile/@path, '\', '/')"/>
pkgroot     = <xsl:value-of select="linux/approot/@path"/>
pkgfiles    = <xsl:value-of select="translate(files/@path, '\','/')"/>

</xsl:template>
</xsl:stylesheet>
