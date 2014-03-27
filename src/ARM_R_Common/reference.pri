# Define library reference handle for rule:  DirName = TARGET

#include (../specs/qxt.pri)

INCLUDEPATH += $$PWD
LIBS += $$staticReference( $$basename(PWD) )
REFERENCES += $$basename(PWD)


