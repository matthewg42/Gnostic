#-------------------------------------------------
#
# Project created by QtCreator 2011-03-01T21:57:26
#
#-------------------------------------------------

# For Qwt... adjust according to where it is installed...
unix:QWT_ROOT  = /opt/qwt
win32:QWT_ROOT = C:/Qt/Qwt-6.0.0-rc5
INCLUDEPATH   += $${QWT_ROOT}/include
LIBS          +=  -L$${QWT_ROOT}/lib

qtAddLibrary(qwt)

win32 {
    contains(QWT_CONFIG, QwtDll) {
	DEFINES    += QT_DLL QWT_DLL
    }
}

QT       += core gui network
CONFIG   += qwt
TARGET   = Gnostic
TEMPLATE = app

INCLUDEPATH += src
INCLUDEPATH += src/gui
INCLUDEPATH += src/transports
INCLUDEPATH += src/displays
INCLUDEPATH += src/utility
INCLUDEPATH += src/monitors

SOURCES += \
    src/main.cpp \
    src/utility/GnosticApp.cpp \
    src/utility/GnosticParser.cpp \
    src/transports/Transport.cpp \
    src/transports/PlinkSshTransport.cpp \
    src/transports/OpenSshTransport.cpp \
    src/transports/DummyTransport.cpp \
    src/transports/LocalTransport.cpp \
    src/transports/LocalTransportConfigWidget.cpp \
    src/transports/AbstractSshTransport.cpp \
    src/transports/SshTransportConfigWidget.cpp \
    src/displays/DataDisplay.cpp \
    src/displays/DataDisplayConfigWidget.cpp \
    src/displays/LineCounterDisplay.cpp \
    src/displays/LineCounterDisplayConfigWidget.cpp \
    src/displays/TailerDisplay.cpp \
    src/displays/TailerDisplayConfigWidget.cpp \
    src/gui/TransportEditorForm.cpp \ 
    src/gui/DataDisplayEditorForm.cpp \
    src/gui/GnosticMainWindow.cpp \
    src/gui/TransportConfigWidget.cpp \
    src/gui/PasswordDialog.cpp \
    src/utility/FileUtils.cpp \
    src/displays/TimeGraphDisplayConfigWidget.cpp \
    src/displays/TimeGraphDisplay.cpp \
    src/gui/PenStyleWidget.cpp \
    src/monitors/RemoteMonitor.cpp \
    src/monitors/RemoteCommand.cpp \
    src/gui/RemoteCommandConfigWidget.cpp \
    src/gui/RemoteMonitorEditorForm.cpp

HEADERS += \
    src/config.hpp \
    src/utility/GnosticApp.hpp \
    src/utility/GnosticParser.hpp \
    src/transports/Transport.hpp \
    src/transports/PlinkSshTransport.hpp \
    src/transports/OpenSshTransport.hpp \
    src/transports/DummyTransport.hpp \
    src/transports/LocalTransport.hpp \
    src/transports/LocalTransportConfigWidget.hpp \
    src/transports/AbstractSshTransport.hpp\
    src/transports/SshTransportConfigWidget.hpp \
    src/displays/DataDisplay.hpp \
    src/displays/DataDisplayConfigWidget.hpp \
    src/displays/LineCounterDisplay.hpp \
    src/displays/LineCounterDisplayConfigWidget.hpp \
    src/displays/TailerDisplay.hpp \
    src/displays/TailerDisplayConfigWidget.hpp \
    src/gui/TransportEditorForm.hpp \
    src/gui/DataDisplayEditorForm.hpp \
    src/gui/GnosticMainWindow.hpp \
    src/gui/TransportConfigWidget.hpp \
    src/gui/PasswordDialog.hpp \
    src/utility/FileUtils.hpp \
    src/displays/TimeGraphDisplayConfigWidget.hpp \
    src/displays/TimeGraphDisplay.hpp \
    src/gui/PenStyleWidget.hpp \
    src/monitors/RemoteMonitor.hpp \
    src/monitors/RemoteCommand.hpp \
    src/gui/RemoteCommandConfigWidget.hpp \
    src/gui/RemoteMonitorEditorForm.hpp

FORMS += \
    src/transports/LocalTransportConfigWidget.ui \
    src/transports/SshTransportConfigWidget.ui \
    src/displays/LineCounterDisplay.ui \ 
    src/displays/LineCounterDisplayConfigWidget.ui \
    src/displays/TailerDisplay.ui \
    src/displays/TailerDisplayConfigWidget.ui \
    src/gui/DataDisplayEditorForm.ui \
    src/gui/GnosticMainWindow.ui \
    src/gui/PasswordDialog.ui \
    src/gui/TransportEditorForm.ui \
    src/displays/TimeGraphDisplayConfigWidget.ui \
    src/displays/TimeGraphDisplay.ui \
    src/gui/PenStyleWidget.ui \
    src/gui/RemoteCommandConfigWidget.ui \
    src/gui/RemoteMonitorEditorForm.ui

OTHER_FILES += \
    README

RESOURCES += \
    resources.qrc
