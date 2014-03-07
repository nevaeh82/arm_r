
export PATH

ifndef TEMP
TEMP=/tmp
endif

major = $(word 1, $(subst ., ,$1))
minor = $(word 2, $(subst ., ,$1))
build  = $(word 3, $(subst ., ,$1))

windows-path  =$(subst /,\,$(1))
unix-path     =$(subst \,/,$(1))

ifeq "$(OS)" "Windows_NT"
	PATH:=$(PATH);..\xmltools;xmltools

	echo   = $(if $(1),@echo $(1),@cmd /c echo.)
	random = $(eval $(shell cmd /c echo %RANDOM%))
	find = c:\bin\unix\usr\local\wbin\find.exe
	nul = nul
	svn=svn.exe
	sed=sed.exe
	gawk=gawk.exe
	hg=hg.exe
	xsltproc=xsltproc.exe
	S = \\

	copy          = copy "$(call windows-path,$(1))" "$(call windows-path,$(2))" /y
	copy_file     = $(call copy,$(1),$(2))
	copy_dir      = x$(call copy,$(1),$(2)) /s /q /i /j

	rm = rm.exe
	cp = cp.exe
else
	echo = echo
	find = find
	nul = /dev/null
	svn = svn
	sed = sed
	gawk = gawk
	hg   = hg
	xsltproc= xsltproc
	S = //

	copy          = cp -f $(3) $(call unix-path,$(1)) $(call unix-path,$(2))
	copy_file     = $(call copy,$(1),$(2))
	copy_dir      = $(call copy,$(1),$(2),-r)

	rm = rm
	cp = cp
endif


unix-path     = $(subst \,/,$(1))
message       = $(info )$(info -------------------------------- $(1) -------------------------- )
