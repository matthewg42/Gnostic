#-------------------------------------------------
#
# Project created by QtCreator 2011-03-01T21:57:26
#
#-------------------------------------------------

QT       += core gui network

TARGET   = Gnostic
TEMPLATE = app

SOURCES += main.cpp                   \
	   GnosticMainWindow.cpp      \
	   Transport.cpp              \
	   GnosticApp.cpp             \
	   LocalTransport.cpp         \
	   PasswordDialog.cpp         \
	   PlinkSshTransport.cpp      \
	   AbstractSshTransport.cpp   \
	   OpenSshTransport.cpp

HEADERS += GnosticMainWindow.hpp      \
	   Transport.hpp              \
	   GnosticApp.hpp             \
	   config.hpp                 \
	   LocalTransport.hpp         \
	   PasswordDialog.hpp         \
	   PlinkSshTransport.hpp      \
	   AbstractSshTransport.hpp   \
	   OpenSshTransport.hpp

FORMS   += GnosticMainWindow.ui       \
	   PasswordDialog.ui

OTHER_FILES += README gnostic.res
