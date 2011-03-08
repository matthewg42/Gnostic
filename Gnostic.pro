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
    LocalTransportConfigWidget.cpp \
    TransportConfig.cpp \
    TransportConfigWidget.cpp \
    SshTransport.cpp \
    SshTransportConfigWidget.cpp \
    PasswordDialog.cpp

HEADERS  += GnosticMainWindow.hpp \
    Transport.hpp \
    GnosticApp.hpp \
    config.hpp \
    LocalTransport.hpp \
    LocalTransportConfigWidget.hpp \
    TransportConfig.hpp \
    TransportConfigWidget.hpp \
    SshTransport.hpp \
    SshTransportConfigWidget.hpp \
    PasswordDialog.hpp

FORMS    += GnosticMainWindow.ui \
    LocalTransportConfigWidget.ui \
    TransportConfig.ui \
    SshTransportConfigWidget.ui \
    PasswordDialog.ui

OTHER_FILES += \
    README \
    gnostic.res
