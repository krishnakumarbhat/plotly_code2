
#ifndef AS_BIN_WRITER_LIB_EXPORT_H
#define AS_BIN_WRITER_LIB_EXPORT_H

/*********************************************
 ** This file usually is generated          **
 ** by cmake. For backwards                 **
 ** compatibility a Visual Studio for       **
 ** Windows version of this file is         **
 ** present in the ports/visualC12 folder.  **
 ** This file shall only be used in windows **
 ** environments that do not use cmake.     **
 *********************************************
 */

#ifdef AS_BIN_WRITER_LIB_STATIC_DEFINE
#define AS_BIN_WRITER_LIB_EXPORT
#define AS_BIN_WRITER_LIB_NO_EXPORT
#else
#ifndef AS_BIN_WRITER_LIB_EXPORT
#ifdef AS_bin_writer_lib_EXPORTS
/* We are building this library */
#define AS_BIN_WRITER_LIB_EXPORT __declspec(dllexport)
#else
/* We are using this library */
#ifdef MSVC
#define AS_BIN_WRITER_LIB_EXPORT __declspec(dllimport)
#else
#define AS_BIN_WRITER_LIB_EXPORT
#endif
#endif
#endif

#ifndef AS_BIN_WRITER_LIB_NO_EXPORT
#define AS_BIN_WRITER_LIB_NO_EXPORT
#endif
#endif

#ifndef AS_BIN_WRITER_LIB_DEPRECATED
#define AS_BIN_WRITER_LIB_DEPRECATED __declspec(deprecated)
#endif

#ifndef AS_BIN_WRITER_LIB_DEPRECATED_EXPORT
#define AS_BIN_WRITER_LIB_DEPRECATED_EXPORT AS_BIN_WRITER_LIB_EXPORT AS_BIN_WRITER_LIB_DEPRECATED
#endif

#ifndef AS_BIN_WRITER_LIB_DEPRECATED_NO_EXPORT
#define AS_BIN_WRITER_LIB_DEPRECATED_NO_EXPORT AS_BIN_WRITER_LIB_NO_EXPORT AS_BIN_WRITER_LIB_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#ifndef AS_BIN_WRITER_LIB_NO_DEPRECATED
#define AS_BIN_WRITER_LIB_NO_DEPRECATED
#endif
#endif

#endif /* AS_BIN_WRITER_LIB_EXPORT_H */
