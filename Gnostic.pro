#-------------------------------------------------
#
# Project created by QtCreator 2011-03-01T21:57:26
#
#-------------------------------------------------

# For Qwt... adjust according to where it is installed...
unix:QWT_ROOT  = /opt/qwt
win32:QWT_ROOT = C:/Qt/Qwt-6.0.0-rc5
win32:DEFINES = QWT_API_VER_6
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

SOURCES += \
    src/main.cpp \
    src/GnosticApp.cpp \
    src/GnosticParser.cpp \
    src/Transport.cpp \
    src/PlinkSshTransport.cpp \
    src/OpenSshTransport.cpp \
    src/DummyTransport.cpp \
    src/LocalTransport.cpp \
    src/LocalTransportConfigWidget.cpp \
    src/AbstractSshTransport.cpp \
    src/SshTransportConfigWidget.cpp \
    src/DataDisplay.cpp \
    src/DataDisplayConfigWidget.cpp \
    src/LineCounterDisplay.cpp \
    src/LineCounterDisplayConfigWidget.cpp \
    src/TailerDisplay.cpp \
    src/TailerDisplayConfigWidget.cpp \
    src/TransportEditorForm.cpp \ 
    src/DataDisplayEditorForm.cpp \
    src/GnosticMainWindow.cpp \
    src/TransportConfigWidget.cpp \
    src/PasswordDialog.cpp \
    src/FileUtils.cpp \
    src/TimeGraphDisplayConfigWidget.cpp \
    src/TimeGraphDisplay.cpp \
    src/PenStyleWidget.cpp \
    src/RemoteMonitor.cpp \
    src/RemoteCommand.cpp \
    src/RemoteCommandConfigWidget.cpp \
    src/RemoteMonitorEditorForm.cpp \
    src/PathEditorForm.cpp \
    src/ConfigurationWindow.cpp

HEADERS += \
    src/config.hpp \
    src/GnosticApp.hpp \
    src/GnosticParser.hpp \
    src/Transport.hpp \
    src/PlinkSshTransport.hpp \
    src/OpenSshTransport.hpp \
    src/DummyTransport.hpp \
    src/LocalTransport.hpp \
    src/LocalTransportConfigWidget.hpp \
    src/AbstractSshTransport.hpp\
    src/SshTransportConfigWidget.hpp \
    src/DataDisplay.hpp \
    src/DataDisplayConfigWidget.hpp \
    src/LineCounterDisplay.hpp \
    src/LineCounterDisplayConfigWidget.hpp \
    src/TailerDisplay.hpp \
    src/TailerDisplayConfigWidget.hpp \
    src/TransportEditorForm.hpp \
    src/DataDisplayEditorForm.hpp \
    src/GnosticMainWindow.hpp \
    src/TransportConfigWidget.hpp \
    src/PasswordDialog.hpp \
    src/FileUtils.hpp \
    src/TimeGraphDisplayConfigWidget.hpp \
    src/TimeGraphDisplay.hpp \
    src/PenStyleWidget.hpp \
    src/RemoteMonitor.hpp \
    src/RemoteCommand.hpp \
    src/RemoteCommandConfigWidget.hpp \
    src/RemoteMonitorEditorForm.hpp \
    src/PathEditorForm.hpp \
    src/ConfigurationWindow.hpp

FORMS += \
    src/LocalTransportConfigWidget.ui \
    src/SshTransportConfigWidget.ui \
    src/LineCounterDisplay.ui \ 
    src/LineCounterDisplayConfigWidget.ui \
    src/TailerDisplay.ui \
    src/TailerDisplayConfigWidget.ui \
    src/DataDisplayEditorForm.ui \
    src/GnosticMainWindow.ui \
    src/PasswordDialog.ui \
    src/TransportEditorForm.ui \
    src/TimeGraphDisplayConfigWidget.ui \
    src/TimeGraphDisplay.ui \
    src/PenStyleWidget.ui \
    src/RemoteCommandConfigWidget.ui \
    src/RemoteMonitorEditorForm.ui \
    src/PathEditorForm.ui \
    src/ConfigurationWindow.ui

OTHER_FILES += \
    README

RESOURCES += \
    resources.qrc
