ifeq "$(OS)" "Windows_NT"

ifeq "$(SystemRoot)" ""
	SystemRoot := $(SYSTEMROOT)
	export SystemRoot
endif
ifeq "$(ProgramFiles)" ""
	ProgramFiles := $(PROGRAMFILES)
	export ProgramFiles
endif

   WinSDK = $(ProgramFiles)\Microsoft SDKs\Windows\v7.0A
   AXIMP  ="$(WinSDK)\bin\AxImp.exe"
   TLBIMP ="$(WinSDK)\bin\tlbimp.exe"
   msbuild="$(SystemRoot)\Microsoft.NET\Framework\v4.0.30319\msbuild.exe"
   VSRoot = $(VS100COMNTOOLS)\..\..
   devenv ="$(VSRoot)\common7\IDE\devenv.exe"


   python=$(PRODUCT_PLATFORM)\python\python.exe
   php=$(SolutionDir)\tools\php\php.exe 


   bin=$(PRODUCT_PLATFORM)\unxutils\usr\local\wbin
   mkdir=$(bin)\mkdir.exe -p
   copy=$(bin)\cp.exe -u 
   cp=$(copy)
   rm=$(bin)\rm.exe -f 
   mv=$(bin)\mv.exe -f 
   chmod=$(bin)\chmod.exe
   echo=cmd /c echo. 
   gawk=$(bin)\gawk.exe 
   find=$(bin)\find.exe
   sed=$(bin)\sed.exe
   cat=$(bin)\cat.exe
   xargs=$(bin)\xargs.exe
   ls=$(bin)\ls.exe

else 
   python=python
   mkdir=mkdir -p
   copy=cp -u 
   rm=rm -f 
   mv=mv -f 
   chmod=chmod
   cp=$(copy)
   echo=echo
   gawk=gawk
   find=find
   sed=sed
   xargs=xargs
   cat=cat
   php=php
   ls=ls
endif

