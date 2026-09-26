#ifndef APPL_FP_IFC_H
#define APPL_FP_IFC_H
/**
 * @file appl_fp.h
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2025 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 * Firstpass wrapper definitions.
 *
 * @note This file should only contain constant definitions and NO INCLUDE files.
 *
 * @section ABBR ABBREVIATIONS:
 *   - @todo List any abbreviations, precede each with a dash ('-').
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - @todo Update list of design document(s).
 *
 *   - Requirements Document(s):
 *     - @todo Update list of requirements document(s)
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - ESGW_4-2_PE-SWx_00-01-A02_EN - C Coding Standards [20120506]
 *     - @todo Update list of other applicable standards
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - @todo List of deviations from standards in this file, or "None".
 *
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/
#include <stdbool.h>

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "ipc_data.h"
#include "mem_pool.h"
#include "radar_sw_config.h"
#include "rdd_first_pass_types.h"

#ifdef __cplusplus
extern "C"
{      /* ! Inclusion of header files should NOT be inside the extern "C" block */
#endif /* __cplusplus */
       /*===========================================================================*
        * Exported Preprocessor #define Constants
        *===========================================================================*/

   /*===========================================================================*
    * Exported Preprocessor #define MACROS
    *===========================================================================*/

   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Object Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Function Prototypes
    *===========================================================================*/
   Memory_Pool_Return_T Appl_Rdd_Dram1_Memory_Init(Mem_Pool_T *mem_pool);
   Memory_Pool_Return_T Appl_Rdd_Sram1_Memory_Init(Mem_Pool_T *mem_pool);
   bool Appl_First_Pass_Execute(uint16_t ridx, D2M_Msg_T *d2m_msg_ptr,
                                BV_Comp_Type_T (*p_beam_vector_data)[NUM_CDM_CHANNELS][MAX_DOPPLER_FFT_SIZE]);
   bool Appl_Rdd_Fp_Init(Radar_Look_T look_id);
   void Update_Rdd_Instrumentation_Variables(void);
#ifdef CDC_ENABLE
   bool Appl_Cdc_Init(Radar_Look_T look_id);
   uint16_t *Get_Cdc_Dbin_Array_Buffer_Ptr(void);
   bool *Get_Ci_Flag_Array_Buffer_Ptr(void);
#endif
   /*===========================================================================*
    * Exported Inline Function Definitions and #define Function-Like Macros
    *===========================================================================*/

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/** @} doxygen end group */
#endif /* APPL_FP_IFC_H */
