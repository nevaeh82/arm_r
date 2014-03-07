INCLUDEPATH += $$SolutionDir/redist/3rdparty/PwGis/include
LIBS        += -L$$SolutionDir/redist/3rdparty/PwGis/lib_$$SPEC
LIBS        += $$library( PwGis )

win32{
#    LIBS += -lole32
}

