#-------------------------------------------------
#
# Project created by QtCreator 2011-03-01T21:57:26
#
#-------------------------------------------------

QT       += core gui network

TARGET   = Gnostic
TEMPLATE = app

INCLUDEPATH += gui
INCLUDEPATH += transports

SOURCES += \
    main.cpp \
    GnosticApp.cpp \
    transports/Transport.cpp \
    transports/LocalTransport.cpp \
    transports/PlinkSshTransport.cpp \
    transports/AbstractSshTransport.cpp \
    transports/OpenSshTransport.cpp \
    gui/GnosticMainWindow.cpp \
    gui/PasswordDialog.cpp \
    gui/TransportConfigWidget.cpp \
    gui/LocalTransportConfigWidget.cpp \
    gui/SshTransportConfigWidget.cpp \
    gui/TransportEditorForm.cpp \
    GnosticParser.cpp \
    DataDisplay.cpp \
    DataDisplayConfigWidget.cpp \
    CounterDisplay.cpp \
    CounterDisplayConfigWidget.cpp \
    DataDisplayEditorForm.cpp

HEADERS += \
    config.hpp \
    GnosticApp.hpp \
    transports/Transport.hpp \
    transports/LocalTransport.hpp \
    transports/PlinkSshTransport.hpp \
    transports/AbstractSshTransport.hpp\
    transports/OpenSshTransport.hpp \
    gui/GnosticMainWindow.hpp \
    gui/PasswordDialog.hpp \
    gui/TransportConfigWidget.hpp \
    gui/LocalTransportConfigWidget.hpp \
    gui/SshTransportConfigWidget.hpp \
    gui/TransportEditorForm.hpp \
    GnosticParser.hpp \
    DataDisplay.hpp \
    DataDisplayConfigWidget.hpp \
    CounterDisplay.hpp \
    CounterDisplayConfigWidget.hpp \
    DataDisplayEditorForm.hpp

FORMS += \
    gui/GnosticMainWindow.ui \
    gui/PasswordDialog.ui \
    gui/LocalTransportConfigWidget.ui \
    gui/SshTransportConfigWidget.ui \
    gui/TransportEditorForm.ui \
    CounterDisplay.ui \
    CounterDisplayConfigWidget.ui \
    DataDisplayEditorForm.ui

OTHER_FILES += \
    README

RESOURCES += \
    resources.qrc
