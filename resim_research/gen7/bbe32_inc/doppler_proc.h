#ifndef DOPPLER_PROC_H
#define DOPPLER_PROC_H
/**
 * @file doppler_proc.h
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2021 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 * Doppler processing wrapper definitions.
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
#include "api/doppler_types.h"
#include "ipc_data.h"
#include "mem_pool.h"
#include "radar_look_types.h"
#include "spbb_typedefs.h"

#ifdef __cplusplus
extern "C"
{      /* ! Inclusion of header files should NOT be inside the extern "C" block */
#endif /* __cplusplus */

/*===========================================================================*
 * Exported Preprocessor #define Constants
 *===========================================================================*/
/** Max allowed Doppler + RDD processing time in microseconds */
#define MAX_DOPPLER_PROC_OVERRUN_TIME_US (30000U)

   /*===========================================================================*
    * Exported Preprocessor #define MACROS
    *===========================================================================*/

   /*===========================================================================*
    * Exported Type Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Object Declarations
    *===========================================================================*/
   extern volatile uint16_t XCP_RDD_Vary_Xput_Flag;
   extern volatile uint16_t XCP_Max_Static_Target_Case_Enable;

   /* Circular BV Buffer  */
   extern BV_Comp_Type_T (*dfft_bv_output_buffer)[BV_CIRCULAR_BUFF_DEPTH][MAX_DOPPLER_FFT_SIZE][SPBB_CDM_TX_CHANNELS];

   /*===========================================================================*
    * Exported Function Prototypes
    *===========================================================================*/
   bool __attribute__((noinline)) Appl_Doppler_Rdd_Processing(Radar_Look_T look_id);
   rdop_avg_t *Get_NF_est_look_ptr(Radar_Look_T look_id);
   void Update_Look_Data(Radar_Look_T look_id, D2M_Msg_T *d2m_msg_ptr);
#ifdef CDC_ENABLE
   uint16_t *Get_Cdc_Dbin_Array_Buffer_Ptr(void);
#endif
   Memory_Pool_Return_T Rdd_Memory_Init(Mem_Pool_T *mem_pool);

   /*===========================================================================*
    * Exported Inline Function Definitions and #define Function-Like Macros
    *===========================================================================*/

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/** @} doxygen end group */
#endif /* MAIN_APPLICATION_H_ */
