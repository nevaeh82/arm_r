INCLUDEPATH += $$PWD
LIBS += $$staticReference( $$basename(PWD) )
REFERENCES += $$basename(PWD)

LIBS += -luser32

DEFINES += UNICODE
DEFINES += _UNICODE
