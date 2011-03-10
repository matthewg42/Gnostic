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
    transports/PlinkSshTransport.cpp \
    transports/OpenSshTransport.cpp \
    transports/DummyTransport.cpp \
    transports/LocalTransport.cpp \
    transports/LocalTransportConfigWidget.cpp \
    transports/AbstractSshTransport.cpp \
    transports/SshTransportConfigWidget.cpp \
    displays/DataDisplay.cpp \
    displays/DataDisplayConfigWidget.cpp \
    displays/LineCounterDisplay.cpp \
    displays/LineCounterDisplayConfigWidget.cpp \
    displays/TailerDisplay.cpp \
    displays/TailerDisplayConfigWidget.cpp \
    gui/TransportEditorForm.cpp \ 
    gui/DataDisplayEditorForm.cpp \
    gui/GnosticMainWindow.cpp \
    gui/TransportConfigWidget.cpp \
    gui/PasswordDialog.cpp

HEADERS += \
    config.hpp \
    GnosticApp.hpp \
    GnosticParser.hpp \
    transports/Transport.hpp \
    transports/PlinkSshTransport.hpp \
    transports/OpenSshTransport.hpp \
    transports/DummyTransport.hpp \
    transports/LocalTransport.hpp \
    transports/LocalTransportConfigWidget.hpp \
    transports/AbstractSshTransport.hpp\
    transports/SshTransportConfigWidget.hpp \
    displays/DataDisplay.hpp \
    displays/DataDisplayConfigWidget.hpp \
    displays/LineCounterDisplay.hpp \
    displays/LineCounterDisplayConfigWidget.hpp \
    displays/TailerDisplay.hpp \
    displays/TailerDisplayConfigWidget.hpp \
    gui/TransportEditorForm.hpp \
    gui/DataDisplayEditorForm.hpp \
    gui/GnosticMainWindow.hpp \
    gui/TransportConfigWidget.hpp \
    gui/PasswordDialog.hpp

FORMS += \
    transports/LocalTransportConfigWidget.ui \
    transports/SshTransportConfigWidget.ui \
    displays/LineCounterDisplay.ui \ 
    displays/LineCounterDisplayConfigWidget.ui \
    displays/TailerDisplay.ui \
    displays/TailerDisplayConfigWidget.ui \
    gui/DataDisplayEditorForm.ui \
    gui/GnosticMainWindow.ui \
    gui/PasswordDialog.ui \
    gui/TransportEditorForm.ui

OTHER_FILES += \
    README

RESOURCES += \
    resources.qrc
