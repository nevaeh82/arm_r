INCLUDEPATH += $$SolutionDir/redist/PwLogger-$$HOST/include
LIBS        += -L$$SolutionDir/redist/PwLogger-$$HOST/lib_$$SPEC
LIBS        += $$library( PwLoggerLib )

win32{
    LIBS += -lole32
}

