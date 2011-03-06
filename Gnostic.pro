#-------------------------------------------------
#
# Project created by QtCreator 2011-03-01T21:57:26
#
#-------------------------------------------------

QT       += core gui network

TARGET = Gnostic
TEMPLATE = app


SOURCES += main.cpp\
        GnosticMainWindow.cpp \
    Transport.cpp \
    GnosticApp.cpp \
    LocalTransport.cpp \
    LocalTransportConfigWidget.cpp

HEADERS  += GnosticMainWindow.hpp \
    Transport.hpp \
    GnosticApp.hpp \
    config.hpp \
    LocalTransport.hpp \
    LocalTransportConfigWidget.hpp

FORMS    += GnosticMainWindow.ui \
    LocalTransportConfigWidget.ui

OTHER_FILES +=
