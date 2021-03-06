#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <QString>
#include <QStringList>

const QString getConfigDir();
const QString getInstallDir();
QStringList getSystemPath();
void setSystemPath(const QStringList& path);
void gnosticSetenv(const QString& variable, const QString& value);

//! Functions which just didn't fit anywhere else...  I'm using this file to
//! hide away ugly OS dependent stuff...

#ifdef Q_OS_WIN
# include <windows.h>
# ifndef _SHOBJ_H
#  include <shlobj.h>
#  include <QLibrary>
# endif
QString getWin32SpecialDirPath(int csidlId);
#endif

#endif // FILEUTILS_HPP
