#ifndef DGPS_INTERFACE_H
#define DGPS_INTERFACE_H

#ifdef SMValidation__BUILT_AS_STATIC
#define SMVALIDATION_EXPORT
#define SMVALIDATION_NO_EXPORT
#else
#ifndef SMVALIDATION_EXPORT
#if defined(SMValidation_EXPORTS) && defined(_WIN64)
/* We are building this library */
#define SMVALIDATION_EXPORT __declspec(dllexport)
#elif defined(_WIN64) && !defined(SMValidation_EXPORTS)
/* We are using this library */
#define SMVALIDATION_EXPORT __declspec(dllimport)
#elif defined(__GNUC__)
#define SMVALIDATION_EXPORT __attribute__((visibility("default")))
#endif
#endif
#endif

#if defined(__GNUC__)
#include <dlfcn.h>
typedef void *SMVALIDATION_HINSTANCE;
typedef char *SMVALIDATION_LPCSTR;
typedef void *SMVALIDATION_FARPROC;
#define __stdcall

#define SMValidation_GetProcAddress(x, y) dlsym(x, y)
#define SMValidation_LoadLibrary(x)       dlopen(x, RTLD_LAZY)
#define SMValidation_FreeLibrary(x)       dlclose(x)

#define SMVALIDATION_SHARED_LIB_EXTENSION ".so"
#define SMVALIDATION_PATH_SEPARATOR       "/"
#define SMVALIDATION_SHARED_LIBRARY       const_cast<char *>("libSMValidation.so")
#define SMVALIDATION_SHARED_LIBRARY_MRR   const_cast<char *>("libSMValidation_MRR.so")

#else
#include <windows.h>

typedef HINSTANCE SMVALIDATION_HINSTANCE;
typedef LPCSTR SMVALIDATION_LPCSTR;
typedef FARPROC SMVALIDATION_FARPROC;

#define SMValidation_GetProcAddress(x, y) GetProcAddress(x, y)
#define SMValidation_LoadLibrary(x)       LoadLibrary(x)
#define SMValidation_FreeLibrary(x)       FreeLibrary(x)

#define SMVALIDATION_SHARED_LIB_EXTENSION ".dll"
#define SMVALIDATION_PATH_SEPARATOR       "\\"
#define SMVALIDATION_SHARED_LIBRARY       const_cast<char *>("SMValidation.dll")
#define SMVALIDATION_SHARED_LIBRARY_MRR   const_cast<char *>("SMValidation_MRR.dll")

#endif

#define SMVALIDATION_INTERFACE_VERSION_MAJOR (1)
#define SMVALIDATION_INTERFACE_VERSION_MINOR (0)
#define SMVALIDATION_INTERFACE_VERSION_PATCH (0)

#include "SMValidationInterfaceDefinition.h"

typedef void (*DGPSinit)(const SMValidationInputConfig_T *init_config);
typedef void (*DGPSSetInputMDFfilePath)(const char *mdf_input_file);
typedef void (*DGPSRun)(const SMValidationRunConfig_T *dgps_run_config, SMValidationOutput_T *output_buff);
typedef void (*DGPSExit)();
typedef void (*DGPSReset)();
typedef void (*DGPSData)(DGPS_Data_T *dgps_data);

struct DgpsFunction_T {
   SMVALIDATION_HINSTANCE handler;
   DGPSinit dgps_init;
   DGPSRun dgps_run;
   DGPSExit dgps_exit;
   DGPSReset dgps_reset;
   DGPSData dgps_data;
};

/* Functions to expose */
extern "C"
{
   /* Get sm2 version */
   SMVALIDATION_EXPORT void SMValidationInit(const SMValidationInputConfig_T *input_config);
   /* Function to set mounting of radar */
   SMVALIDATION_EXPORT void SMValidationSetInputMDFfilePath(const char *mdf_input_file);
   /* Functions to run dgps */
   SMVALIDATION_EXPORT void SMValidationRun(const SMValidationRunConfig_T *run_config, SMValidationOutput_T *output_buff);
   /* Functions to copy dgps data */
   SMVALIDATION_EXPORT void SMValidationDGPSData(DGPS_Data_T *dgps_data);
   /* Functions to exit dgps */
   SMVALIDATION_EXPORT void SMValidationExit();
   /* Functions to rest dgps */
   SMVALIDATION_EXPORT void SMValidationReset();
};

#endif
