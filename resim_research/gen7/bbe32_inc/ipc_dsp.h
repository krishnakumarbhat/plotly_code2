#ifndef IPC_DSP_H
#define IPC_DSP_H
/**
 * @file ipc_dsp.h
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2023 Aptiv. All rights reserved.
 * Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 * IPC function definitions for interaction with IPC.
 * @note This file should only contain constant definitions and NO INCLUDE files.
 *
 * @section ABBR ABBREVIATIONS:
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *   - Requirements Document(s):
 *   - Applicable Standards (in order of precedence: highest first):
 *     - ESGW_4-2_PE-SWx_00-01-A02_EN - C Coding Standards [20120506]
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 * @ updates to areas outside the scope of procedures:
 *   - Refer to module footer comment block.
 */
/*==========================================================================*/
/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/* ========================================================================== */
/*                         Xtensa Include Files                               */
/* ========================================================================== */
#ifdef __XTENSA__
   #include <xtensa/core-macros.h>
   #include <xtensa/tie/xt_sync.h>
#else
   #include "core-macros.h"
#endif

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "ipc_data.h"
#include "smc_cal.h"
#include "usc_cal.h"

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

   /** For BBE, need to define the trigger received as a uint32_t, so the L32AI instruction
    *  can be used to allow running with higher levels of optimization.
    */
   typedef enum
   {
      NO_TRIGGER = 0,
      TRIGGER_RECEIVED
   } Trigger_Received_T;

   /*===========================================================================*
    * Exported Const Object Declarations
    *===========================================================================*/
   /** Flag to indicate m7 to bbe trigger receievd*/
   extern uint32_t IPC_M7_To_BBE_Trigger_Received;
   /** Offline mode flag*/
   extern bool Offline_Mode_Flag_DSP;
   /** IPC M2D error counter*/
   extern uint16_t IPC_M2D_Buffer_Err_Cnt;

   extern uint16_t DSP_Current_Look_Index;
   extern uint32_t M7_Ack_Error_Counter_DSP;
   extern uint32_t A53_Ack_Error_Counter_DSP;
   extern uint32_t DSP_D2A_Msg_Header_Cnt;
   /*===========================================================================*
    * Exported Function Prototypes
    *===========================================================================*/
   void Appl_DSP_Process_Onetime_Init(void);
   void Send_Onetime_to_A53(void);
   void Send_Range_Proc_Complete(void);
   void Send_Doppler_RDD_Proc_Complete(void);
   void __attribute__((noinline)) Send_SP_Post_Proc_Complete_to_M7(void);
   void Send_SP_Post_Proc_Complete_to_A53(void);
   void Update_DSP_IPC_Notify(void);
   Radar_Look_T Get_Current_Lookid(void);
   uint16_t Get_Current_Scanindex(void);
   uint16_t Get_Current_Lookindex(void);
   void Reset_IPC_RDD_Buffer(void);
   void Reset_IPC_AF_Buffer(void);
   M2D_One_Time_Msg_T *Get_IPC_M2D_One_Time_Buffer(void);
   M2D_Msg_T *Get_RP_IPC_M2D_Buffer(void);
   D2M_Msg_T *Get_RP_IPC_D2M_Msg_Buffer(void);
   D2M_Msg_T *Get_DP_RDD_IPC_D2M_Msg_Buffer(void);
   D2M_Msg_T *Get_SP_Post_Proc_IPC_D2M_Msg_Buffer(void);
   void Appl_Range_Process_Trigger(void);
   void Update_Timestamp_Data(void);
   void Set_MMIC_Data_Discard_Info(void);

#ifdef IPC_BBE_FIXED_TRIGGER_ENABLE
   void Trigger_Timer_BBE_TO_M7(void);
   void Set_BBE_To_M7_Trigger_Timer(uint16_t range_proc_timeout_err);
#endif
   /*===========================================================================*
    * Exported Inline Function Definitions and #define Function-Like Macros
    *===========================================================================*/
   /**
    * @brief Wait indefinitely for a trigger signal to be set to triggered state.
    *
    **/
   static inline void Wait_For_Trigger(uint32_t *trigger_ptr)
   {
#ifndef UNIT_TEST
      while (NO_TRIGGER == XT_L32AI(trigger_ptr, 0))
         ;
#endif /* UNIT_TEST */
   }

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/** @} doxygen end group */
#endif /* IPC_DSP_H */
