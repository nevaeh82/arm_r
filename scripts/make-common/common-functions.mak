
windows-path  =$(subst /,\,$(1))
unix-path     =$(subst \,/,$(1))
escape-slash  =$(subst /,\/,$(1))

separator     = $(info --------------------------------------------------------------------- )
begin-build   = $(call separator)$(info $(1))$(call separator)


ifeq "$(OS)" "Windows_NT"
	native-path = $(windows-path)
else
	native-path = $(unix-path)
endif
