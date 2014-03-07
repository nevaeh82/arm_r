#
# integration with Qmake part.


DESTDIR_TARGET = $(MAKE_DESTDIR_TARGET)
DESTDIR    := $(dir $(call unix-path,$(DESTDIR_TARGET)) ).
TARGET_EXT := $(notdir $(DESTDIR_TARGET))
TARGET     := $(basename $(TARGET_EXT))

