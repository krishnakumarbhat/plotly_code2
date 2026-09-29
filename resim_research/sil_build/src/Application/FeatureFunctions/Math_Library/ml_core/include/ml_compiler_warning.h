#ifndef ML_COMPILER_WARNING_H
#define ML_COMPILER_WARNING_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

/**
 * \defgroup compiler_warning Compiler Warning Macros
 * Macros helping with suppressing and issuing compiler warnings.
 */

/* Since this file defines a bunch of function-like macros the QAC check
"A function could probably be used instead of this function-like macro."
Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

#define STRINGIZE_HELPER(x) #x
#define STRINGIZE(x) STRINGIZE_HELPER(x)
#define WARNING(desc) message(__FILE__ "(" STRINGIZE(__LINE__) ") : Warning: " #desc)
/**
 * The macro As_Compiler_Warning is meant to issue a warning to the user.
 * \param desc The warning to be issued at compile time
 * \ingroup compiler_warning
 * \sdd{WI-14089}
 */
#define As_Compiler_Warning(desc) __pragma(WARNING(desc))

#ifdef _MSC_VER
 /**
 * Msvs_Disable_Warning will disable the warning wrn till Msvs_Enable_Warning is called (or end of file)
 * \param wrn Number of the warning to be suppressed
 * \ingroup compiler_warning
 * \sdd{WI-14090}
 */
#define Msvs_Disable_Warning(wrn) __pragma(warning(disable: wrn))

 /**
 * Msvs_Disable_Warning will disable the warning wrn till Msvs_Enable_Warning is called (or end of file)
 * \param wrn Number of the warning to be suppressed
 * \ingroup compiler_warning
 * \sdd{WI-14091}
 */
#define Msvs_Enable_Warning(wrn) __pragma(warning(default: wrn))
#else
 /**
 * Msvs_Disable_Warning will disable the warning wrn till Msvs_Enable_Warning is called (or end of file)
 * \param warning Number of the warning to be suppressed
 * \ingroup compiler_warning
 * \sdd{WI-14090}
 */
#define Msvs_Disable_Warning(warning)

 /**
 * Msvs_Disable_Warning will disable the warning wrn till Msvs_Enable_Warning is called (or end of file)
 * \param warning Number of the warning to be suppressed
 * \ingroup compiler_warning
 * \sdd{WI-14091}
 */
#define Msvs_Enable_Warning(warning)
#endif

#ifdef __cplusplus
}
#endif
#endif
