INCLUDEPATH += $$PWD
LIBS += $$staticReference( $$basename(PWD) )
REFERENCES += $$basename(PWD)

include(../../specs/tbb.pri)
include(../../specs/ipp.pri)
include(../../specs/qxt.pri)
include(../../specs/CISCommonLib.pri)
include(../ARM_R_Common/reference.pri)
include(../Graphics_ipp/reference.pri)
