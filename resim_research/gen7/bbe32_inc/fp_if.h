#ifndef APPL_RDD_FP_IF_H
#define APPL_RDD_FP_IF_H
/*===========================================================================*/
/**
 * @file appl_rdd_fp_if.h
 *
 * @todo application RDD FP interface header file.
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2020 Aptiv. All rights reserved.
 *Aptiv Sensitve Business – Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 * @todo Add full description here
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
 *
 * @defgroup template Provide API description and define/delete next line
 * @ingroup <parent_API> (OPTIONAL USE if part of another API, else delete)
 * @{
 */
/*==========================================================================*/

/*===========================================================================*
 * Standard Header Files
 *===========================================================================*/

/*===========================================================================*
 * Other Header Files
 *===========================================================================*/
#include "api/firstpass.h"
#include "api/firstpass_types.h"
#include "bb_cfg.h"
#include "ipc_data.h"
#include "radar_look_types.h"
#include "rdd_stream.h"
#include "spbb_configuration.h"
#include "spbb_typedefs.h"

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
    * Exported Const Object Declarations
    *===========================================================================*/

   /*===========================================================================*
    * Exported Function Prototypes
    *===========================================================================*/
   /** RDD Internal buffer structure*/
   typedef struct RDD_Internal_Buff_Tag
   {
      uint16_t rdd1_nci_prev_scal[MAX_DOPPLER_FFT_SIZE] __attribute__((aligned(32)));
      uint16_t rdd1_nci_curr_scal[MAX_DOPPLER_FFT_SIZE] __attribute__((aligned(32)));
      uint16_t rdd1_nci_next_scal[MAX_DOPPLER_FFT_SIZE] __attribute__((aligned(32)));
      uint32_t rdd2_thold_zdb[SPBB_MAX_RANGE_BINS] __attribute__((aligned(32)));
      uint32_t rdd2_thold_hvc[SPBB_MAX_RANGE_BINS] __attribute__((aligned(32)));
      float32_t bwdep_range_attenuation[SPBB_MAX_RANGE_BINS] __attribute__((aligned(32)));
#ifdef CDC_ENABLE
      /* Dbins selected for CDC are stored in this buffer */
      uint16_t cdc_dbin_array[MAX_DOPPLER_FFT_SIZE] __attribute__((aligned(32)));
#endif
#ifdef SPBB_SCM_ENABLE
      float32_t scm_max_detectable_range;
      uint16_t scm_max_rbin_processed;
      uint16_t saturation_cm_dbin_diff[SPBB_FP_MAX_DETECT - 1U] __attribute__((aligned(32)));
      uint8_t scm_targets_saturation_flag;
      uint8_t scm_prioritization_flag;
      uint8_t scm_run_per_rbin;
#endif
      uint16_t xcp_range_idx;
      uint16_t xcp_doppler_idx;
      bool xcp_rdd_vary_xput_flag;
      bool xcp_max_static_target_case_enable;
      bool xcp_spe_gain_vary_enable_flag;
      float32_t xcp_spe_gain;
      float32_t xcp_spe_gain_min;
      float32_t xcp_spe_gain_max;
      uint16_t xcp_max_fp_tgt_cnt;
      uint16_t xcp_rngidx_target_start;
      uint16_t xcp_max_fp_per_rbin_target_cnt;
      uint16_t xcp_max_per_rbin_targets;
      uint16_t xcp_range_idx_beamvector;
   } RDD_Internal_Buff_T;

   bool First_Pass_Process_Init(Radar_Look_T look_id);
   void First_Pass_Execute_No_Detections(uint16_t ridx, RDD_Data_T *p_rdd_data, Radar_Look_T look_id,
                                         rdop_avg_t **rdop_avg_data_ptr, RDD_Internal_Buff_T *p_rdd_int_buff,
                                         rdop_avg_t *nf_est_look, XCP_Info_T *xcp_info_ptr, D2M_Msg_T *d2m_msg_ptr);
   void First_Pass_Execute(uint16_t ridx, D2M_Msg_T *d2m_msg_ptr, Radar_Look_T look_id, rdop_avg_t **rdop_avg_data_ptr,
                           RDD_Internal_Buff_T *p_rdd_int_buff, rdop_avg_t *nf_est_look, XCP_Info_T *xcp_info_ptr
#ifdef SPBB_CDC_ENABLE
                           ,
                           uint16_t *cdc_dbin_array, uint16_t *cdc_dbin_cnt_ptr
#endif
   );

   /*===========================================================================*
    * Exported Inline Function Definitions and #define Function-Like Macros
    *===========================================================================*/

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif
