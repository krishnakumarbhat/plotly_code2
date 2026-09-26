/*===========================================================================*/
/**
 * @file range_proc.c
 *
 * Application interface mapping for Range Process module
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2023 Aptiv. All rights reserved.
 * Aptiv Sensitive Business � Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 * Application interface mapping for Range Process module
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
#include <string.h>
/*===========================================================================*
 * Header Files
 *===========================================================================*/
#include "api/range.h"
#include "api/range_types.h"
#include "ipc_dsp.h"
#include "range_proc.h"
/*===========================================================================*
 * Local Preprocessor #define Constants
 *===========================================================================*/
/*===========================================================================*
 * Local Preprocessor #define MACROS
 *===========================================================================*/
#define PDC_BITWIDTH_MODE_1 1U
#define PDC_BITWIDTH_MODE_2 2U
#define SHIFT_BY_16_BIT     8U
#define SHIFT_BY_14_BIT     9U
/*===========================================================================*
 * Local Type Declarations
 *===========================================================================*/
uint8_t Min_Range_Shift;
/*===========================================================================*
 * Exported Const Object Definitions
 *===========================================================================*/

/*===========================================================================*
 * Local Object Definitions
 *===========================================================================*/

/*===========================================================================*
 * Local Function Prototypes
 *===========================================================================*/

/*===========================================================================*
 * Local Inline Function Definitions and Function-Like Macros
 *===========================================================================*/

/*===========================================================================*
 * Function Definitions
 *===========================================================================*/
/**
 *  @b Description
 *  @n Perform Range Processing Init.
 *
 * @param[in]
 *
 * @retval
 * bool ret_val -> true for success, False for failure
 *
 */
bool Appl_Range_Process_One_Time_Init(void)
{
   bool ret_val                             = true;
   Range_Process_Config_T range_process_cfg = {0};

   uint16_t i;
   uint8_t adc_shift_bits;
   D2M_Msg_T *d2m_msg_ptr               = Get_RP_IPC_D2M_Msg_Buffer();
   Rfft_Debug_Data_T *p_rfft_debug_data = &d2m_msg_ptr->payload.rdd_debug_stream_data.rfft_debug_data;

   range_process_cfg.idm_training_length              = k_idm_training_length_SMC;
   range_process_cfg.idm_rx_ch_num                    = k_gating_rx_channel_SMC - 1U;
   range_process_cfg.idm_dt_en                        = k_idm_double_threshold_enable_SMC;
   range_process_cfg.idm_per_scan_enable              = k_idm_per_scan_enable_SMC;
   range_process_cfg.idm_alpha_cfar                   = k_alpha_cfar_SMC;
   range_process_cfg.idm_per_scan_initial_percent     = k_idm_per_scan_initial_percent_SMC;
   range_process_cfg.idm_per_scan_ksigma              = k_idm_per_scan_ksigma_SMC;
   range_process_cfg.idm_per_scan_ksigma2             = k_idm_per_scan_ksigma2_SMC;
   range_process_cfg.idm_per_scan_chirp_group_indices = k_idm_per_scan_chirp_group_indices_SMC;
   range_process_cfg.idm_factory_mode_enable          = k_idm_per_scan_factory_mode_enable_SMC;

   for (i = 0U; i < NUM_LOOKS; i++)
   {
      range_process_cfg.num_samples[i] = M_FRAME[i];
      range_process_cfg.num_chirps[i]  = K_FRAME[i];
   }

   for (i = 0U; i < RP_IDM_GROUP_IDX; i++)
   {
      p_rfft_debug_data->idm_thold_upperlimit_prev_scan[i] = IDM_THOLD_UPPERLIMIT_DEFAULT;
   }

   /* In SPT, each data element of OPRAM is of size 24 bit length. So based on the input ADC data length, we left shift the data to
   occupy entire 24 bit.
   If input ADC data is 16 bit wide, shift value = 24 - 16 = 8 bits.
   If input ADC data is 14 bit wide, shift value should be 24-14 = 10 bits. But the RADAR Packet Processor Engine (PPE) would have
   already shift up 1 bit before feeding the data to the DSP, our SW needs to shift one more bit up to maintain the signal level.
   So shift val = 24-14-1 = 9 bits*/
   if (PDC_BITWIDTH_MODE_1 == rfeCfg_general_PdcBitwidth_SMC)
   {
      adc_shift_bits = SHIFT_BY_14_BIT; // 14bit ADC
   }
   else if (PDC_BITWIDTH_MODE_2 == rfeCfg_general_PdcBitwidth_SMC)
   {
      adc_shift_bits = SHIFT_BY_16_BIT; // 16 bit ADC
   }
   else
   {
      adc_shift_bits = SHIFT_BY_16_BIT; // default : 16 bit ADC
   }
   range_process_cfg.adc_shift_bits = adc_shift_bits;
   ret_val                          = (RANGE_PROCESS_NO_ERROR == Range_Process_One_Time_Init(&range_process_cfg));

   return ret_val;
}

/**
 *  @b Description
 *  @n Perform Range Processing Execute
 *
 * @param[in]
 *
 * @retval
 * bool ret_val -> true for success, False for failure
 *
 */
bool Appl_Range_Process_Execute(Radar_Look_T look_type, M2D_Msg_T *m2d_msg_ptr)
{
   bool ret_val                              = true;
   Range_Process_Input_T range_process_in    = {0};
   Range_Process_Output_T range_process_out  = {0};
   M2D_One_Time_Msg_T *m2d_one_time_init_ptr = Get_IPC_M2D_One_Time_Buffer();
   D2M_Msg_T *d2m_msg_ptr                    = Get_RP_IPC_D2M_Msg_Buffer();
   Rfft_Data_T *p_rfft_data                  = &d2m_msg_ptr->payload.rdd_stream_data.rfft_data;
   Rfft_Debug_Data_T *p_rfft_debug_data      = &d2m_msg_ptr->payload.rdd_debug_stream_data.rfft_debug_data;
   range_process_in.p_adc_input              = (int16_t *)(m2d_one_time_init_ptr->acq_buffer_sram_address);
   range_process_in.look_type                = look_type;
   range_process_in.range_spt_api = (void (*)(void))(m2d_one_time_init_ptr->spt_kernel_address[RANGE_PROC_INIT_EXECUTE_1024]);
   range_process_in.idm_thold_upperlimit_prev_scan = (uint32_t *)(m2d_one_time_init_ptr->idm_thold_upperlimit);
   range_process_in.xcp_enable_adc_log             = (uint16_t)m2d_msg_ptr->payload.xcp_Info.xcp_spt_log_parm_mode;
   range_process_in.xcp_channel_select             = m2d_msg_ptr->payload.xcp_Info.xcp_spt_log_channel_select;
   range_process_in.xcp_chirp_select               = m2d_msg_ptr->payload.xcp_Info.xcp_spt_log_chirp_range_num;
   range_process_in.xcp_look_select                = m2d_msg_ptr->payload.xcp_Info.xcp_spt_look_type;
   range_process_in.xcp_factory_mode_triggered     = m2d_msg_ptr->payload.xcp_Info.xcp_manf_operation_mode;
   range_process_out.p_idm_sram_address            = (Radar_Data32_T *)(m2d_one_time_init_ptr->idm_sram_address);
   range_process_out.p_idm_xcp_sram_address        = (Radar_Data16_T *)(m2d_one_time_init_ptr->idm_xcp_sram_address);
   range_process_out.p_output_data_cube            = (Output_DataCube_T *)(m2d_one_time_init_ptr->range_proc_output_sram_address);
   range_process_out.p_idm_cfar_thold              = &p_rfft_debug_data->idm_cfar_thold[0];
   range_process_out.p_idm_first_interference_chirp_adc_data = &p_rfft_debug_data->first_interfered_chirp[0];
   range_process_out.p_idm_gating_window                     = &p_rfft_debug_data->gating_window[0];
   range_process_out.p_idm_excision_samples                  = &p_rfft_data->interfered_samples_per_chirp[0];
   range_process_out.p_idm_first_chirp_idx                   = &p_rfft_debug_data->idm_first_chirp_idx;
   range_process_out.p_min_range_shift                       = (uint8_t *)&Min_Range_Shift;
   range_process_out.p_num_chirps_processed                  = &p_rfft_debug_data->num_chirps_processed;
   ret_val = (RANGE_PROCESS_NO_ERROR == Range_Process_Execute(&range_process_in, &range_process_out));

   return ret_val;
}
