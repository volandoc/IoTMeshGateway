#ifndef SYSDEFS_H
#define SYSDEFS_H

#if defined(WIN32) || defined(_WIN32) || defined(_WIN64) || defined(__WIN32__)
#   define _FILE_SEPARATOR "\\"
#   define _PLUGIN_PREFIX ""
#else
#   define _FILE_SEPARATOR "/"
#   define _PLUGIN_PREFIX "lib"
#endif


#endif // SYSDEFS_H
