#ifndef CHIYOPLUGINS_GLOBAL_H
#define CHIYOPLUGINS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CHIYOPLUGINS_LIBRARY)
#  define CHIYOPLUGINSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CHIYOPLUGINSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CHIYOPLUGINS_GLOBAL_H