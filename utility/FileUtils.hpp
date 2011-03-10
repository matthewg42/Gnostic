#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <QString>

const QString getConfigDir();
const QString getInstallDir();


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
