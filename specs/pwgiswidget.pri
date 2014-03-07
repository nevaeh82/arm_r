include(pwgiswidget-common.pri)
include(pwgiswidget-logger.pri)


QT       += core gui network webkit

PWGISWIDGET_INCLUDEPATH = $$PWD/../redist/3rdparty/PwGis/include
PWGISWIDGET_LIBDIR      = $$PWD/../redist/3rdparty/PwGis/lib_$$SPEC



INCLUDEPATH += $$PWGISWIDGET_INCLUDEPATH
INCLUDEPATH += $$PWGISWIDGET_INCLUDEPATH/PwGis

PWGISWIDGET_LIBS        += -L$$PWGISWIDGET_LIBDIR
PWGISWIDGET_LIBS        += $$pwgisLibrary( PwGisClient, 1)
PWGISWIDGET_LIBS        += $$pwgisLibrary( PwGisTacticMap, 1)

LIBS += $$PWGISWIDGET_LIBS


pwgisDeployDependencies($$PWGISWIDGET_LIBS)
