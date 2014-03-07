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
Name: <xsl:value-of  select="product/@code"/>
Version: <xsl:value-of  select="product/@version"/>
Release: 0
Vendor: JSC TRK
Summary: Special software. <xsl:value-of select="product/summary"/>
License: Proprietary
Group: Application
Source0: %{name}-%{version}.tar
BuildRoot: %{_topdir}/BUILD/%{name}-buildroot
BuildArch: i386
<xsl:for-each select="linux/requires/require">
	<xsl:if test="@package = 'gisdesigner10_qt4'">
AutoReqProv: no
	</xsl:if>
Requires: <xsl:value-of select="@package"/>
</xsl:for-each>
<xsl:for-each select="linux/provides/provide">
Provides: <xsl:value-of  select="@package"/>
</xsl:for-each>

%description

Special software.

%prep

%setup -c -n %{name}-%{version}

%build

%install
[ -d ${RPM_BUILD_ROOT} ] &amp;&amp; rm -rf ${RPM_BUILD_ROOT}
/bin/mkdir -p ${RPM_BUILD_ROOT}
/bin/cp -axv ${RPM_BUILD_DIR}/%{name}-%{version}/* ${RPM_BUILD_ROOT}/


%post
<xsl:for-each select="linux/postinstall/execute">
<xsl:value-of select="@script"/>
</xsl:for-each>

%postun

%clean

%files
%defattr(-,root,root)
/*

%define date    %(echo `LC_ALL="C" date +"%a %b %d %Y"`)

%changelog

* %{date} User &lt;support@nttnpp.ru&gt;
- Packed to RPM.
</xsl:template>
</xsl:stylesheet>
