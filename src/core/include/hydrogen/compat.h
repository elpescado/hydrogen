#ifndef H2C_COMPAT
#define H2C_COMPAT

#include <QtGlobal>

#ifdef Q_CC_MSVC

/* MSVC defines min()/max() macros, which collide with std::numberic_limits<>::max()
   Defining NOMINMAX disables that */
#  define NOMINMAX

/* MSVC does not provide str(n)casecmp, but does provide stri(n)cmp that do the same */
#  define strcasecmp _stricmp
#  define strncasecmp _strnicmp

#endif

#endif
