INCLUDEPATH += $$PWD
LIBS += $$staticReference( $$basename(PWD) )
REFERENCES += $$basename(PWD)

include(../../specs/Solver.pri)
include(../../specs/protobuf.pri)
include(../../specs/qxt.pri)
include(../../specs/CISCommonLib.pri)
include(../ARM_R_Common/reference.pri)

