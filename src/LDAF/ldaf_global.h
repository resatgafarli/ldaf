/******************************************************
Author: resat.gafarli@gmail.com
File: ldaf_global
Date: 5/30/2017
License: GPL-3.0
*******************************************************/
#ifndef LDAF_GLOBAL_H
#define LDAF_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LDAF_LIBRARY)
#  define LDAFSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LDAFSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LDAF_GLOBAL_H
