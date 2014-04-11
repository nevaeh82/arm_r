INCLUDEPATH += $$SolutionDir/redist/GraphicsIpp/include
LIBS += -L$$SolutionDir/redist/GraphicsIpp/lib_$$SPEC
LIBS += $$library( Graphics_ipp )
