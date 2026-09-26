#ifndef SIL_WRAPPER_H
#define SIL_WRAPPER_H

// Headers
#include "dc_version.h"
#include "dc_config.h"
#include "dc_input_data.h"
#include "dc_output_data.h"

// Functions are exposed between FW and DC
#if defined _MSC_VER
#define RECU_SIL_API __declspec(dllexport)
#elif defined __GNUC__
#define RECU_SIL_API __attribute((visibility("default")))
#endif

#ifdef __cplusplus
#define EXTERN_C extern "C"
#endif

typedef enum {
   E_SIL_RUN_OK = 0,
   E_SIL_RUN_ERR,
   E_SIL_INIT_ERR,
   E_SIL_PENDING_STATE
} sil_status_e;

/* API's exposed between FW and DC Note same names should be used as in FW */
EXTERN_C RECU_SIL_API sil_status_e RECU_SiL_Init(void *init_param);
EXTERN_C RECU_SIL_API sil_status_e RECU_SiL_Execute(const void *input_rec, const void *output_rec);
EXTERN_C RECU_SIL_API sil_status_e RECU_SiL_Reset();
EXTERN_C RECU_SIL_API sil_status_e RECU_SiL_GetVersion(DC_Version_Info_T **ppVersion);
EXTERN_C RECU_SIL_API sil_status_e RECU_SiL_SetFolderPath(const char *pLogFolder);
EXTERN_C RECU_SIL_API sil_status_e RECU_SiL_Exit(void *pLogFolder, bool endFile);

#endif // SIL_WRAPPER_H
