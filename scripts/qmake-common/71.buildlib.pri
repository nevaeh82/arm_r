#
# build import lib.
#

win32-msvc2010 {
 QMAKE_LFLAGS  += /IMPLIB:$${DESTDIR}/$${TARGET}$$version_maj().lib
}

