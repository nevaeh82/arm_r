INCLUDEPATH += $$PWD
LIBS += $$staticReference( $$basename(PWD) )
REFERENCES += $$basename(PWD)

include(../../specs/tbb.pri)
include(../../specs/ipp.pri)
include(../../specs/qxt.pri)
include(../../specs/Solver.pri)
include(../../specs/CISCommonLib.pri)
include(../../specs/GraphicsIpp.pri)
include(../ARM_R_Common/reference.pri)
