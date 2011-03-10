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
INCLUDEPATH += displays

SOURCES += \
    main.cpp \
    GnosticApp.cpp \
    GnosticParser.cpp \
    transports/Transport.cpp \
    transports/LocalTransport.cpp \
    transports/PlinkSshTransport.cpp \
    transports/AbstractSshTransport.cpp \
    transports/OpenSshTransport.cpp \
    displays/DataDisplay.cpp \
    displays/LineCounterDisplay.cpp \
    gui/GnosticMainWindow.cpp \
    gui/PasswordDialog.cpp \
    gui/TransportConfigWidget.cpp \
    gui/LocalTransportConfigWidget.cpp \
    gui/SshTransportConfigWidget.cpp \
    gui/TransportEditorForm.cpp \
    gui/DataDisplayConfigWidget.cpp \
    gui/LineCounterDisplayConfigWidget.cpp \
    gui/DataDisplayEditorForm.cpp \
    transports/DummyTransport.cpp \
    displays/TailerDisplay.cpp \
    gui/TailerDisplayConfigWidget.cpp

HEADERS += \
    config.hpp \
    GnosticApp.hpp \
    GnosticParser.hpp \
    transports/Transport.hpp \
    transports/LocalTransport.hpp \
    transports/PlinkSshTransport.hpp \
    transports/AbstractSshTransport.hpp\
    transports/OpenSshTransport.hpp \
    displays/DataDisplay.hpp \
    displays/LineCounterDisplay.hpp \
    gui/GnosticMainWindow.hpp \
    gui/PasswordDialog.hpp \
    gui/TransportConfigWidget.hpp \
    gui/LocalTransportConfigWidget.hpp \
    gui/SshTransportConfigWidget.hpp \
    gui/TransportEditorForm.hpp \
    gui/DataDisplayConfigWidget.hpp \
    gui/LineCounterDisplayConfigWidget.hpp \
    gui/DataDisplayEditorForm.hpp \
    transports/DummyTransport.hpp \
    displays/TailerDisplay.hpp \
    gui/TailerDisplayConfigWidget.hpp

FORMS += \
    gui/GnosticMainWindow.ui \
    gui/PasswordDialog.ui \
    gui/LocalTransportConfigWidget.ui \
    gui/SshTransportConfigWidget.ui \
    gui/TransportEditorForm.ui \
    gui/LineCounterDisplayConfigWidget.ui \
    gui/DataDisplayEditorForm.ui \
    displays/LineCounterDisplay.ui \ 
    displays/TailerDisplay.ui \
    gui/TailerDisplayConfigWidget.ui

OTHER_FILES += \
    README

RESOURCES += \
    resources.qrc
