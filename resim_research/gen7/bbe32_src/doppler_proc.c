/*===========================================================================*/
/**
 * @file doppler_proc.c
 *
 * Application interface mapping for Doppler Process module
 *
 *------------------------------------------------------------------------------
 *
 * Copyright (C) 2020 Aptiv. All rights reserved.
 * Aptiv Sensitive Business � Restricted Aptiv information. Do not disclose
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 * Application interface mapping for Doppler Process module
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
#include "api/cfar.h"
#include "api/cfar_types.h"
#include "api/dd_spt.h"
#include "api/doppler_types.h"
#include "api/firstpass.h"
#include "api/firstpass_types.h"
#include "api/profiling_helpers.h"
#include "api/range_types.h"
#include "api/second_pass.h"
#include "api/second_pass_types.h"
#include "api/timing_helpers.h"
#include "bb_cfg.h"
#include "doppler_proc.h"
#include "fp_if.h"
#include "ipc_data.h"
#include "ipc_dsp.h"
#include "main_application.h"
#include "mem_pool.h"
#include "radar_look_types.h"
#include "radar_sw_config.h"
#include "range_proc.h"
#include "rdd_stream.h"
#include "reuse.h"
#include "smc_cal.h"
#include "spbb_conversions.h"
#include "spbb_math.h"
#include "spbb_typedefs.h"
#include "sweep_bw.h"
#include "sweep_bw_types.h"
#ifdef CDC_ENABLE
   #include "cdc_if.h"
   #include "cdc_packing.h"
#endif
#include "sp_if.h"
#include "sweep_bw_if.h"
/* ========================================================================== */
/*                                 Macros                                     */
/* ========================================================================== */

#define MAX_DOPPLER_PROC_OFFLINE_OVERRUN_TIME_US (0xFFFFFFFFU) /*Max DFFT RDD timeout in offline mode*/
#define MAX_DOPPLER_PROC_OVERRUN_TIME_TICKS      ((uint32_t)(MAX_DOPPLER_PROC_OVERRUN_TIME_US / CORE_TIMER_CLOCK_PERIOD_US))

#define MAX_NUM_OF_LOOKS ((uint8_t)NUM_LOOKS)

#if SPBB_DDM_TX_CHANNELS
   #define DDMA_FLAG true
#else
   /* Should never come here, disabling DDMA by default*/
   #define DDMA_FLAG false
#endif

#ifdef CDC_ENABLE
   #define CDC_CFG_INIT(x) Cdc_Cfg_Init(x)
#else
   #define CDC_CFG_INIT(x) true
#endif

/*Pertaining to First pass*/
#define RUN_LOG_FRAC_BIN_EST (0x02U)
#define SPE_GAIN_INCREMENT   (0.1F)
#define SFW_ENABLE           (0x02U)

/* The Rbin Scaling is padded along with NCI PDMA in this location of NCI Output buffer */
#define RBIN_SCALE_LOCATION (uint16_t)(SPBB_MAX_DOPPLER_FFT_SIZE + 1U)
#define REST_RBIN_LOCATION  (uint16_t)(SPBB_MAX_DOPPLER_FFT_SIZE)
/* Used for Bin Undergone Rest Logging*/
#define REST_PATTERN (0xA5A5U)

#if defined(Integration_Testing) && defined(Anglefinding_IT)
   #include "dsp_integration_test.h"
   #define SIT_D2M_MSG_BUFFER(d2m_msg_ptr) SIT_D2M_Msg_Buffer_Provider(d2m_msg_ptr)
#else
   #define SIT_D2M_MSG_BUFFER(d2m_msg_ptr) /* No operation */
#endif

/*===========================================================================*
 * Local Object Definitions
 *===========================================================================*/
/*===========================================================================*
 * Local Type Declarations
 *===========================================================================*/
typedef enum SP_Ctrl_Scan_Tag
{
   SP_SCAN_SHORT,
   SP_SCAN_MEDIUM,
   SP_SCAN_LONG,
   SP_NUMBER_OF_SCAN_TYPES
} SP_Ctrl_Scan_T;

typedef enum SP_Ctrl_Dwell_Tag
{
   SP_MEDIUM_LOOK,
   SP_LONG_LOOK,
   SP_NUMBER_OF_DWELL_TYPES
} SP_Ctrl_Dwell_T;

/* ========================================================================== */
/*                           External variables                               */
/* ========================================================================== */

/* ========================================================================== */
/*                            Structures                                      */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
////////////////////////////////////////////////////////////////////////
// This variable for this is on SRAM and this pointer is acquired via IPC
/////////////////////////////////////////////////////////////////////////
BV_Comp_Type_T (*dfft_bv_output_buffer)[BV_CIRCULAR_BUFF_DEPTH][SPBB_MAX_DOPPLER_FFT_SIZE][SPBB_CDM_TX_CHANNELS];

/* Circular NCI DSP internal buffer*/
/* Per rbin scaling and Rest Bin Information is augmented at the end of NCI data for that range bin. This was originally 16bytes.
 * so instead of that, NCI_PAD_BINS is added as a multiple of 32bytes( i.e. based on XCHAL_BBEN_SIMD_WIDTH),in order to keep this
 * definition 32byte aligned. Also supports access for vectorization.
 */
NCI_Buff_T (*rdop_avg_data)[NCI_CIRCULAR_BUFF_DEPTH];
rdop_avg_t *rdop_avg_data_ptr[NCI_CIRCULAR_BUFF_DEPTH];

#ifdef ENABLE_PROFILE_TIMING_OUTPUTS_DOPPLER

   #ifdef ENABLE_PER_BIN_TIMING
/** Store the microseconds spent for each run of the HWA and the final call to RDD (hence +1) **/
volatile uint32_t Doppler_Proc_Per_Bin_Timing[SPBB_MAX_RANGE_BINS + 2U];
volatile uint32_t Doppler_Proc_Setup_Timing;
volatile uint32_t Doppler_Proc_Cleanup_Timing;
volatile uint32_t Doppler_Proc_Active_Time;
   #endif

   #ifdef ENABLE_MODULEWISE_TIMING
typedef struct RDD_Modulewise_Time_Tag
{
   volatile uint32_t per_bin_timing[SPBB_MAX_RANGE_BINS + 2U];
   volatile uint32_t timing_total; // Module Total execution Time across all rbin
   volatile uint32_t time_total_max;
} RDD_Modulewise_Time_T;

RDD_Modulewise_Time_T Doppler_Modulewise_Time = {0};
RDD_Modulewise_Time_T Cfar_Modulewise_Time    = {0};
RDD_Modulewise_Time_T SweepBW_Modulewise_Time = {0};
RDD_Modulewise_Time_T FP_Modulewise_Time      = {0};
      #ifdef CDC_ENABLE
RDD_Modulewise_Time_T CDC_Modulewise_Time = {0};
      #endif
volatile uint32_t SP_Timing_Total; // SecondPass Total execution Time in last rbin
volatile uint32_t SP_Time_Total_Max;
   #endif
#endif

RDD_Internal_Buff_T *p_rdd_int_buff                    = NULL;
rdop_avg_t nf_est_look[NUM_LOOKS][SPBB_MAX_RANGE_BINS] = {0};

#ifdef CDC_ENABLE
uint16_t Cdc_Dbin_Count;
uint16_t *Cdc_Dbin_Cnt_Ptr;
#endif

SP_Ctrl_Dwell_T Dwell_Types[NUM_LOOKS] = {SP_LONG_LOOK, SP_MEDIUM_LOOK, SP_LONG_LOOK, SP_MEDIUM_LOOK};
SP_Ctrl_Scan_T Scan_Types[NUM_LOOKS]   = {SP_SCAN_LONG, SP_SCAN_LONG, SP_SCAN_MEDIUM, SP_SCAN_MEDIUM};

/* ========================================================================== */
/*                            Static Variables                                */
/* ========================================================================== */

/* Variables specific to Dopp/RDD TimeOut Limit */
static volatile uint32_t Doppler_Proc_Overrun_Counter           = 0U;
static volatile uint32_t Doppler_Proc_Active_Time_Max           = 0U;
static volatile uint32_t Doppler_Proc_Active_Ticks_Max          = 0U;
static volatile uint32_t Doppler_Proc_SPT_Error_Count           = 0U;
static volatile uint32_t Doppler_Proc_Initialization_Fail_Count = 0U;

/* Used for Max Vary Xput test mode*/
static uint16_t Rest_Sim_Cnt = 0U;
/* This gives the Count of REST Bins per Look */
static uint16_t Rest_Counter = 0U;
/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

static bool Appl_Doppler_Process_Init(Radar_Look_T look_id);
static bool __attribute__((noinline)) Appl_Rdd_Process_Init(Radar_Look_T look_id);
static bool Cfar_Init(Radar_Look_T look_id);
static bool Spt_Wait(void);
static void Update_Rdd_Instrumentation_Variables(D2M_Msg_T *d2m_msg_ptr);
static uint8_t Read_Per_Rbin_Scaling_Value(rdop_avg_t **rdop_avg_data_ptr);
static void Read_Bin_Undergone_Rest_Value(rdop_avg_t **rdop_avg_data_ptr, RDD_Data_T *p_rdd_data, uint16_t range_idx);
static void Circular_Buffer_Init(void);
#ifdef ENABLE_MODULEWISE_TIMING
static void Modulewise_Time_Calc(RDD_Modulewise_Time_T *modulewise_time, uint32_t range_index, uint32_t cycles_diff);
static void Modulewise_Time_Calc_Last(RDD_Modulewise_Time_T *modulewise_time, uint32_t range_index, uint32_t cycles_diff);
static inline void Profiling_Reset_And_Start(uint32_t *module_cycle_prev);
static inline void Profiling_Stop(RDD_Modulewise_Time_T *Modulewise_Time, uint16_t range_idx, uint32_t module_cycle_prev);
static inline void Profiling_Stop_Last(RDD_Modulewise_Time_T *Modulewise_Time, uint16_t range_idx, uint32_t module_cycle_prev);
static inline void Profiling_Start(uint32_t *module_cycle_prev);
#endif
#ifdef ENABLE_PER_BIN_TIMING
static inline void Profiling_Stop_Per_Bin(uint32_t *cycle_count_prev, uint16_t range_idx);
#endif
#ifdef SPBB_SCM_ENABLE
static void Reset_SCM_Internals(RDD_Internal_Buff_T *p_rdd_int_buff);
#endif
static inline void Update_Debug_Stream_Check_Dopp_Overrun(Rfft_Debug_Data_T *p_rdd_debug_stream_data,
                                                          M2D_One_Time_Msg_T *m2d_bbe_one_time_msg_ptr, Rfft_Data_T *p_rfft_data,
                                                          Radar_Look_T look_id, uint32_t doppler_rdd_runtime,
                                                          bool doppler_proc_retval, RDD_Data_T *p_rdd_data);

/*===========================================================================*/
/*                   #define function-like macros                            */
/*===========================================================================*/
#ifdef ENABLE_MODULEWISE_TIMING
   #define PROFILING_RESET_AND_START(x) Profiling_Reset_And_Start(x)
   #define PROFILING_STOP(p, q, r)      Profiling_Stop(p, q, r)
   #define PROFILING_STOP_LAST(p, q, r) Profiling_Stop_Last(p, q, r)
   #define PROFILING_START(x)           Profiling_Start(x)
#else
   #define PROFILING_RESET_AND_START(x)
   #define PROFILING_STOP(p, q, r)
   #define PROFILING_STOP_LAST(p, q, r)
   #define PROFILING_START(x)
#endif

#ifdef ENABLE_PER_BIN_TIMING
   #define PROFILING_STOP_PER_BIN(p, q) Profiling_Stop_Per_Bin(p, q)
#else
   #define PROFILING_STOP_PER_BIN(p, q)
#endif

/*===========================================================================*
 * Function Definitions
 *===========================================================================*/

/**
 *  @b Description
 *  @n
 *      Initialize the CFAR method for given look ID
 *
 *  @param[in]  look_id
 *      Look ID for which to configure the Sweep BW calculations.
 *
 * @return @ref bool indicating the success/failure.
 */
static bool Cfar_Init(Radar_Look_T look_id)
{
   /* Creating a local cfar config variable to copy the config value from smc*/
   Cfar_Config_T cfar_cfg = {0};

   /* Following values are copied from SMC buffer*/
   cfar_cfg.k_sig_mult_v              = k_signal_multiplier_SMC[look_id];
   cfar_cfg.k_scrub_factor            = k_scrub_factor_SMC[look_id];
   cfar_cfg.k_total_num_CFAR_segments = k_total_num_cfar_segments_SMC[look_id];
   cfar_cfg.k_num_segs_min            = k_num_segs_min_SMC[look_id];
   cfar_cfg.doppler_fft_size          = SPBB_MAX_DOPPLER_FFT_SIZE;
   cfar_cfg.k_CFAR_LUT                = k_cfar_lut_SMC[look_id];
   cfar_cfg.k_CFAR_LUT_2              = k_cfar_lut_2_SMC[look_id];
   cfar_cfg.k_cfar_lut_idx            = k_cfar_lut_idx_SMC[look_id];

   return (CFAR_PROCESS_NO_ERROR == Cfar_Process_Init(&cfar_cfg));
} /* End of Cfar_Init() */

/**
 *  @b Description
 *  @n
 *      Execute the CFAR method
 *
 *  @param[in] ridx current range index
 *  @param[in] p_rdd_data rdd output buffer pointer
 *  @param[in] : **rdop_avg_ptr
 *
 */
void Appl_Cfar_Execute(uint16_t ridx, RDD_Data_T *p_rdd_data, rdop_avg_t **rdop_avg_ptr)
{
   Cfar_Input_T cfar_in   = {0}; /* Local bufer for cfar input */
   Cfar_Output_T cfar_out = {0}; /* Local bufer for cfar output */

   Cfar_Error_T cfar_error = CFAR_PROCESS_NO_ERROR; /* Setting the cfar initial error as error free */

   /* Pointing the cfar inputs*/
   cfar_in.p_rdop_avg_nci = rdop_avg_ptr[NCI_NEXT_BUFFER_DEPTH];
   cfar_in.rbin           = ridx;

   /* Pointing the cfar output buffer to rdd IPC buffer */
   cfar_out.p_cfar_corr_coeff = &p_rdd_data->cfar_corr_coeff[ridx];
   cfar_out.p_nf_est          = &p_rdd_data->cfar_nf_est[ridx];
   cfar_out.p_cfar_threshold  = &p_rdd_data->cfar_thold[ridx];
   /* Cfar main process is triggered*/
   cfar_error = Cfar_Process_Execute(&cfar_in, &cfar_out);
} /*End of Appl_Cfar_Execute()*/

static bool Spt_Wait(void)
{
   return (Dd_Spt_Check_Kernel_Execute_Done());
}

static void Circular_Buffer_Init(void)
{
   uint16_t k;

   for (k = 0U; k < NCI_CIRCULAR_BUFF_DEPTH; k++)
   {
      rdop_avg_data_ptr[k] = &(*rdop_avg_data)[k][0];
   }
}

#ifdef SPBB_SCM_ENABLE
/**
 *  @b Description
 *  @n
 *      Reset the Saturation Counter measure's internal flags.
 *
 *  @param[in]  *p_rdd_int_buffer
 *             pointer to RDD_Internal_Buff_T
 *  @param[out]  None
 *
 */
static void Reset_SCM_Internals(RDD_Internal_Buff_T *p_rdd_int_buffer)
{
   p_rdd_int_buffer->scm_targets_saturation_flag = CODED_FALSE;
   p_rdd_int_buffer->scm_prioritization_flag     = CODED_FALSE;
   p_rdd_int_buffer->scm_run_per_rbin            = CODED_FALSE;
}
#endif

void __attribute__((noinline)) Circular_Buffer_Update(void)
{
   rdop_avg_t **pp_rdop_avg_data;
   rdop_avg_t *p_rdop_avg_data;
   uint16_t k;

   /* Rotate rdop avg  history */
   pp_rdop_avg_data = &rdop_avg_data_ptr[0];
   p_rdop_avg_data  = pp_rdop_avg_data[0];

   for (k = 0; k < NCI_CIRCULAR_BUFF_DEPTH - 1; k++)
   {
      pp_rdop_avg_data[k] = pp_rdop_avg_data[k + 1];
   }
   pp_rdop_avg_data[NCI_CIRCULAR_BUFF_DEPTH - 1] = p_rdop_avg_data;
}

/**
 *  @b Description
 *  @n
 *   This function returns the pointer of nf_est stored in one of 4 look buffer based on look id
 *  @param[in1] : lookid -> Look id
 *  @param[out] : Address of nf_est stored in one of 4 look buffer
 **/
rdop_avg_t *Get_NF_est_look_ptr(Radar_Look_T look_id)
{
   return (&(nf_est_look[look_id][0]));
}

#ifdef CDC_ENABLE
/**
 *  @b Description
 *  @n
 *   This function returns the pointer of cdc_dbin_array stored in Internal buffer
 *  @param[out] : Address of cdc_dbin_array stored in Internal buffer
 **/
uint16_t *Get_Cdc_Dbin_Array_Buffer_Ptr(void)
{
   return &p_rdd_int_buff->cdc_dbin_array[0];
}
#endif

Memory_Pool_Return_T Rdd_Memory_Init(Mem_Pool_T *mem_pool)
{
   Memory_Pool_Return_T retVal1 = MEM_POOL_FAILURE;
   Memory_Pool_Return_T retVal2 = MEM_POOL_FAILURE;

   // Size is NCI_CIRCULAR_BUFF_DEPTH * MAX_DOPPLER_FFT_SIZE * sizeof(rdop_avg_t)
   retVal1 = Mem_Pool_Alloc(mem_pool, sizeof(*rdop_avg_data), (void **)&rdop_avg_data);

   if (retVal1 == MEM_POOL_SUCCESS)
   {
      // retVal = Mem_Pool_Alloc(mem_pool, sizeof(RDD_Internal_Buff_T), (void **)&BB_RDD_INT_BUFF_PTR);
      retVal2 = Mem_Pool_Alloc(mem_pool, sizeof(RDD_Internal_Buff_T), (void **)&p_rdd_int_buff);
   }

   return retVal2;
}
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
#ifdef ENABLE_MODULEWISE_TIMING
/**
 *  @b Description
 *  @n
 *   This function computes the profiling time and updates these values to input structure.
 *  @param[in1] : *Modulewise_Time
 *  @param[in1] : range_index
 *  @param[in1] : cycles_diff
 *  @param[out]
 *  returns none
 **/
static void Modulewise_Time_Calc(RDD_Modulewise_Time_T *modulewise_time, uint32_t range_index, uint32_t cycles_diff)
{
   modulewise_time->per_bin_timing[range_index] = Timing_Helpers_Get_Time_Nanosec(cycles_diff);
   modulewise_time->timing_total += modulewise_time->per_bin_timing[range_index];
}

/**
 *  @b Description
 *  @n
 *   This function computes the profiling time for the last case and updates these values to input structure.
 *  @param[in1] : *Modulewise_Time
 *  @param[in1] : range_index
 *  @param[in1] : cycles_diff
 *  @param[out]
 *  returns none
 **/
static void Modulewise_Time_Calc_Last(RDD_Modulewise_Time_T *modulewise_time, uint32_t range_index, uint32_t cycles_diff)
{
   modulewise_time->per_bin_timing[range_index] = Timing_Helpers_Get_Time_Nanosec(cycles_diff);
   modulewise_time->timing_total += modulewise_time->per_bin_timing[range_index];
   if (modulewise_time->timing_total > modulewise_time->time_total_max)
   {
      modulewise_time->time_total_max = modulewise_time->timing_total;
   }
}

/**
 *  @b Description
 *  @n
 *   This function resets the profiling parameters and starts profiling
 *  @param[in1] : *module_cycle_prev
 *  @param[out]
 *  returns none
 **/
static inline void Profiling_Reset_And_Start(uint32_t *module_cycle_prev)
{
   Doppler_Modulewise_Time.timing_total = 0U;
   SweepBW_Modulewise_Time.timing_total = 0U;
   Cfar_Modulewise_Time.timing_total    = 0U;
   FP_Modulewise_Time.timing_total      = 0U;
   #ifdef CDC_ENABLE
   CDC_Modulewise_Time.timing_total = 0U;
   #endif
   *module_cycle_prev = Timing_Helpers_Get_Timestamp();
}

/**
 *  @b Description
 *  @n
 *   This function computes the profiling time
 *  @param[in1] : *Modulewise_Time
 *  @param[in1] : range_idx
 *  @param[in1] : module_cycle_prev
 *  @param[out]
 *  returns none
 **/
static inline void Profiling_Stop(RDD_Modulewise_Time_T *Modulewise_Time, uint16_t range_idx, uint32_t module_cycle_prev)
{
   uint32_t module_cycle_count;

   module_cycle_count = Timing_Helpers_Get_Timestamp();
   Modulewise_Time_Calc(Modulewise_Time, range_idx, module_cycle_count - module_cycle_prev);
}

/**
 *  @b Description
 *  @n
 *   This function computes the profiling time for the last case
 *  @param[in1] : *Modulewise_Time
 *  @param[in1] : range_idx
 *  @param[in1] : module_cycle_prev
 *  @param[out]
 *  returns none
 **/
static inline void Profiling_Stop_Last(RDD_Modulewise_Time_T *Modulewise_Time, uint16_t range_idx, uint32_t module_cycle_prev)
{
   uint32_t module_cycle_count;

   module_cycle_count = Timing_Helpers_Get_Timestamp();
   Modulewise_Time_Calc_Last(Modulewise_Time, range_idx, module_cycle_count - module_cycle_prev);
}

/**
 *  @b Description
 *  @n
 *   This function starts profiling
 *  @param[in1] : *module_cycle_prev
 *  @param[out]
 *  returns none
 **/
static inline void Profiling_Start(uint32_t *module_cycle_prev)
{
   *module_cycle_prev = Timing_Helpers_Get_Timestamp();
}
#endif

#ifdef ENABLE_PER_BIN_TIMING
/**
 *  @b Description
 *  @n
 *   This function computes per bin profiling time
 *  @param[in1] : *cycle_count_prev
 *  @param[in1] : range_idx
 *  @param[out]
 *  returns none
 **/
static inline void Profiling_Stop_Per_Bin(uint32_t *cycle_count_prev, uint16_t range_idx)
{
   uint32_t cycle_count;

   cycle_count                            = Timing_Helpers_Get_Timestamp();
   Doppler_Proc_Per_Bin_Timing[range_idx] = Timing_Helpers_Get_Time_Microsec(cycle_count - (*cycle_count_prev));
   *cycle_count_prev                      = cycle_count;
}
#endif

/**
 *  @b Description
 *  @n
 *      bool(Radar_Look_T look_id) placeholder for initializing the doppler processing configuration based on the look_id.
 *
 *      (Radar_Look_T look_id, Doppler_Process_Input_T *doppler_process_input, Doppler_Process_Output_T* doppler_process_output)
 *
 */
static bool Appl_Doppler_Process_Init(Radar_Look_T look_id)
{
   Doppler_Process_Config_T dopp_cfg = {0};
   uint8_t look_index                = (uint8_t)look_id;
   bool ret_val                      = true;
   bool rest_flag[MAX_NUM_OF_LOOKS]  = {SPBB_REST_ENABLE_LOOK_A, SPBB_REST_ENABLE_LOOK_B, SPBB_REST_ENABLE_LOOK_C,
                                       SPBB_REST_ENABLE_LOOK_D, SPBB_REST_ENABLE_LOOK_E};

   look_index = (look_index < MAX_NUM_OF_LOOKS) ? look_index : 0U;

   dopp_cfg.look_id   = look_id;
   dopp_cfg.num_chirp = K_FRAME[look_index];
   dopp_cfg.dfft_size = SPBB_MAX_DOPPLER_FFT_SIZE;

   dopp_cfg.rest_flag = rest_flag[look_index];

   dopp_cfg.ddma_flag = DDMA_FLAG;

   if (!p_rdd_int_buff->xcp_rdd_vary_xput_flag)
   {
      dopp_cfg.rest_thold_factor = (float32_t)k_rest_threshold_factor_SMC[look_index];
   }
   else
   {
      dopp_cfg.rest_thold_factor = 0.0F;
   }

   ret_val = (DOPPLER_PROCESS_NO_ERROR == Doppler_Process_Init(&dopp_cfg));

   return ret_val;
}

/**
 *  @b Description
 *  @n
 *      bool(Radar_Look_T look_id) placeholder for initializing the RDD processing configuration based on the look_id.
 *
 *      (Radar_Look_T look_id, I/O structure pointers for the RDD blocks)
 *
 *
 */
static bool __attribute__((noinline)) Appl_Rdd_Process_Init(Radar_Look_T look_id)
{
   bool ret_val = true;

   /* Call the initializations for the BBs here. */
   ret_val = Sweep_Bw_Init(look_id);
   ret_val = ret_val && Cfar_Init(look_id);
   ret_val = ret_val && First_Pass_Process_Init(look_id);
   ret_val = ret_val && Second_Pass_Init(look_id);
   ret_val = ret_val && CDC_CFG_INIT(look_id);

   return ret_val;
}

void Update_Look_Data(Radar_Look_T look_id, D2M_Msg_T *d2m_msg_ptr)
{
   Look_Data_T *p_look_data = &d2m_msg_ptr->payload.rdd_stream_data.look_data;

   /** Update Look information */
   p_look_data->look_id          = (uint16_t)Get_Current_Lookid();
   p_look_data->scan_index       = Get_Current_Scanindex();
   p_look_data->look_index       = Get_Current_Lookindex();
   p_look_data->dwell_type       = Dwell_Types[look_id];
   p_look_data->scan_type        = Scan_Types[look_id];
   p_look_data->rbin_res         = k_rbin_res_SMC[look_id];
   p_look_data->dbin_res         = k_dbin_res_SMC[look_id];
   p_look_data->vua              = k_vua_SMC[look_id];
   p_look_data->range_coverage   = (u9p7_T)(k_detection_range_max_SMC[look_id] - k_detection_range_min_SMC[look_id]);
   p_look_data->doppler_coverage = (s7p8_T)(k_detection_range_rate_max_SMC[look_id] - k_detection_range_rate_min_SMC[look_id]);
}

/**
 *  @b Description
 *  @n
 *      Perform doppler and RDD pipelined processing.
 *
 *
 *  @retval retVal
 *      true for success, false for an error
 */
bool __attribute__((noinline)) Appl_Doppler_Rdd_Processing(Radar_Look_T look_id)
{
   bool doppler_proc_retval          = true;
   uint16_t range_idx                = 0U;
   bool spt_wait_done                = true;
   Doppler_Process_Input_T dopp_in   = {0};
   Doppler_Process_Output_T dopp_out = {0};
   Sweep_Bw_Process_Input_T sweep_bw_input;
   Sweep_Bw_Process_Output_T sweep_bw_output;
   uint32_t max_dopp_proc_overrun_time_us = MAX_DOPPLER_PROC_OVERRUN_TIME_US;
   uint32_t cycle_count_prev              = 0U;
#ifdef ENABLE_PER_BIN_TIMING
   uint32_t cycle_count = 0U;
#endif
#ifdef ENABLE_MODULEWISE_TIMING
   uint32_t module_cycle_prev = 0U;
#endif
   M2D_One_Time_Msg_T *m2d_bbe_one_time_msg_ptr = Get_IPC_M2D_One_Time_Buffer();
   dfft_bv_output_buffer = (BV_Comp_Type_T(*)[BV_CIRCULAR_BUFF_DEPTH][SPBB_MAX_DOPPLER_FFT_SIZE][SPBB_CDM_TX_CHANNELS])
                              m2d_bbe_one_time_msg_ptr->doppler_proc_output_sram_address;
   Range_Process_OutputDataCube_T *range_input_ptr =
      (Range_Process_OutputDataCube_T *)m2d_bbe_one_time_msg_ptr->range_proc_output_sram_address;

   D2M_Msg_T *d2m_msg_ptr                     = Get_DP_RDD_IPC_D2M_Msg_Buffer();
   RDD_Data_T *p_rdd_data                     = &d2m_msg_ptr->payload.rdd_stream_data.rdd_data;
   Rfft_Data_T *p_rfft_data                   = &d2m_msg_ptr->payload.rdd_stream_data.rfft_data;
   Stream_Hdr_T *p_stream_hdr                 = &d2m_msg_ptr->payload.rdd_stream_data.stream_hdr;
   Stream_Hdr_T *p_rdd_debug_stream_hdr       = &d2m_msg_ptr->payload.rdd_debug_stream_data.stream_hdr;
   Rfft_Debug_Data_T *p_rdd_debug_stream_data = &d2m_msg_ptr->payload.rdd_debug_stream_data.rfft_debug_data;
   Look_Data_T *p_look_data                   = &d2m_msg_ptr->payload.rdd_stream_data.look_data;

#ifdef CDC_ENABLE
   Cdc_Debug_Data_T *p_cdc_debug_stream_data = &d2m_msg_ptr->payload.rdd_debug_stream_data.cdc_debug_data;
   Cdc_Dbin_Cnt_Ptr                          = &Cdc_Dbin_Count;
#endif
   /* Pointing SweepBW output pointers to IPC SweepBW buffers */
   sweep_bw_output.p_bwdep_cr_resp           = &p_rdd_data->bwdep_cr_resp[0];
   sweep_bw_output.p_bwdep_hvc_thold         = &p_rdd_data->bwdep_hvc_thold[0];
   sweep_bw_output.p_bwdep_mb_thold          = &p_rdd_data->bwdep_mb_thold[0];
   sweep_bw_output.p_bwdep_sensitivity_thold = &p_rdd_data->bwdep_sensitivity_thold[0];

   if (Offline_Mode_Flag_DSP)
   {
      sweep_bw_input.bwdep_temperature = 50;
      max_dopp_proc_overrun_time_us    = MAX_DOPPLER_PROC_OFFLINE_OVERRUN_TIME_US;
   }
   else
   {
      sweep_bw_input.bwdep_temperature = d2m_msg_ptr->payload.ipc_m2d_payload.mmic_Info.avg_mmic_onchip_temp;
   }

   sweep_bw_input.p_scale_per_rbin = &p_rdd_data->per_rbin_scale[0];

   Rest_Counter = 0U;

   /** Clear the IPC RDD buffers*/
   Reset_IPC_RDD_Buffer();

#ifdef SPBB_SCM_ENABLE
   Reset_SCM_Internals(p_rdd_int_buff);
#endif

   Update_Rdd_Instrumentation_Variables(d2m_msg_ptr);
   /* Map the MMIC temperature to RDD stream for logging*/
   p_rdd_data->bwdep_temperature = sweep_bw_input.bwdep_temperature;

   void (*dfft_rbin0_spt_fn)(void)    = (void (*)(void))m2d_bbe_one_time_msg_ptr->spt_kernel_address[DFFT_SPT_FIRST_RBIN_KERNEL];
   void (*dfft_rbin_spt_fn)(void)     = (void (*)(void))m2d_bbe_one_time_msg_ptr->spt_kernel_address[DFFT_SPT_RBIN_KERNEL];
   void (*dfft_rbinLast_spt_fn)(void) = (void (*)(void))m2d_bbe_one_time_msg_ptr->spt_kernel_address[DFFT_SPT_LAST_RBIN_KERNEL];

   dopp_in.dfft_spt_api[0] = dfft_rbin0_spt_fn;    // Initialize for dfft rbin 0 spt call
   dopp_in.dfft_spt_api[1] = dfft_rbin_spt_fn;     // Initialize for dfft rbin 1 spt call
   dopp_in.dfft_spt_api[2] = dfft_rbinLast_spt_fn; // Initialize for dfft last rbin  spt call

   dopp_in.look_type                     = look_id;
   dopp_in.num_of_chirps                 = K_FRAME[look_id];
   dopp_in.rest_Start_Rbin_smc           = k_rest_start_rbin_SMC[look_id];
   dopp_in.rest_End_Rbin_smc             = k_rest_end_rbin_SMC[look_id];
   dopp_in.no_of_rbins_for_rest_smc_copy = k_rest_num_bins_SMC[look_id];
   dopp_in.min_range_shift = Min_Range_Shift; /* Reading the Min Range Shift, which is updated by the Range Process Module */

/* SAF85xx supports upto 4 Transmit channels, so the following code is written to support upto 4 DDMA carrier frequencies */
/* As of now, Gen7v2-FLR7 uses two DDM carrier frequencies which is collapsed into a single doppler bin shift value */
#ifdef VARIANT_ES1
   dopp_in.k_ddma_first_doppler_bin_shift  = 0U;
   dopp_in.k_ddma_second_doppler_bin_shift = (uint16_t)(SPBB_MAX_DOPPLER_FFT_SIZE - k_ddma_doppler_bin_shift_SMC[look_id]);
#elif VARIANT_ES2
   dopp_in.k_ddma_first_doppler_bin_shift  = 0U;
   dopp_in.k_ddma_second_doppler_bin_shift = (uint16_t)(k_ddma_doppler_bin_shift_SMC[look_id]);
#endif

   if (p_rdd_int_buff->xcp_rdd_vary_xput_flag)
   {
      if (p_rdd_int_buff->xcp_max_static_target_case_enable)
      {
         Rest_Sim_Cnt = k_rest_num_bins_SMC[look_id];
      }
      else
      {
         Rest_Sim_Cnt = (uint16_t)((Rest_Sim_Cnt % (uint16_t)(dopp_in.no_of_rbins_for_rest_smc_copy)) + 1U);
      }
      dopp_in.no_of_rbins_for_rest_smc_copy = Rest_Sim_Cnt;
   }
   else
   {
      Rest_Sim_Cnt = 0U;
   }

   dopp_out.rest_bin_count = 0U;

   uint32_t doppler_proc_start_timestamp = Timing_Helpers_Get_Timestamp();
   uint32_t doppler_rdd_runtime          = 0U;

#ifdef ENABLE_PER_BIN_TIMING

   cycle_count      = doppler_proc_start_timestamp;
   cycle_count_prev = cycle_count;
#endif
   Circular_Buffer_Init();
   doppler_proc_retval = Appl_Doppler_Process_Init(look_id) && Appl_Rdd_Process_Init(look_id);

   if (doppler_proc_retval)
   {
#ifdef ENABLE_PER_BIN_TIMING
      cycle_count               = Timing_Helpers_Get_Timestamp();
      Doppler_Proc_Setup_Timing = Timing_Helpers_Get_Time_Microsec(cycle_count - cycle_count_prev);
      cycle_count_prev          = cycle_count;
#endif

      range_idx                                = 0U;
      dopp_in.rfft_compressed_output_base_addr = (uint64_t *)(&(*range_input_ptr)[0]);
      dopp_in.rfft_compressed_output_addr      = (uint64_t *)(&(*range_input_ptr)[range_idx + 1]);
      dopp_in.range_idx                        = range_idx;

      dopp_out.p_dfft_output       = (DP_OutputDataCube_Ptr_T)(*dfft_bv_output_buffer)[range_idx % BV_CIRCULAR_BUFF_DEPTH][0];
      dopp_out.p_nci_output        = (NCI_Buff_Ptr_T)(*rdop_avg_data)[range_idx % NCI_CIRCULAR_BUFF_DEPTH];
      dopp_out.p_min_chirp_scaling = &p_rdd_data->min_chirp_scaling;

      PROFILING_RESET_AND_START(&module_cycle_prev);

      doppler_proc_retval =
         (DOPPLER_PROCESS_NO_ERROR == Doppler_Process_Execute(&dopp_in, &dopp_out)); // Does doppler processing for Rbin0.

      PROFILING_STOP(&Doppler_Modulewise_Time, range_idx, module_cycle_prev);
      PROFILING_START(&module_cycle_prev);
      /* Calculate first set of LUT for Sweep Bandwidth*/
      (void)Sweep_Bw_LUT_Calc_First(&sweep_bw_output);
#ifdef ENABLE_MODULEWISE_TIMING
      SweepBW_Modulewise_Time.per_bin_timing[RBIN0] =
         Timing_Helpers_Get_Time_Nanosec(Timing_Helpers_Get_Timestamp() - module_cycle_prev);
#endif
      spt_wait_done = spt_wait_done && Spt_Wait();

      PROFILING_STOP_PER_BIN(&cycle_count_prev, range_idx);

      ++range_idx; // range_idx = 1;
      dopp_in.rfft_compressed_output_addr = (uint64_t *)(&(*range_input_ptr)[range_idx + 1]);
      dopp_in.range_idx                   = range_idx;

      dopp_out.p_dfft_output = (DP_OutputDataCube_Ptr_T)(*dfft_bv_output_buffer)[(range_idx - 1) % BV_CIRCULAR_BUFF_DEPTH][0];
      dopp_out.p_nci_output  = (NCI_Buff_Ptr_T)(*rdop_avg_data)[(range_idx - 1) % NCI_CIRCULAR_BUFF_DEPTH];

      PROFILING_START(&module_cycle_prev);

      doppler_proc_retval =
         (DOPPLER_PROCESS_NO_ERROR ==
          Doppler_Process_Execute(&dopp_in, &dopp_out)); // Does doppler processing for Rbin1 and transfers Rbin0 data.

      PROFILING_STOP(&Doppler_Modulewise_Time, range_idx, module_cycle_prev);

      Circular_Buffer_Update();
      PROFILING_START(&module_cycle_prev);

      /*Calulate the second set of LUT for Sweep Bandwidth*/
      (void)Sweep_Bw_LUT_Calc_Second(&sweep_bw_output);
#ifdef ENABLE_MODULEWISE_TIMING
      SweepBW_Modulewise_Time.per_bin_timing[RBIN0] +=
         Timing_Helpers_Get_Time_Nanosec(Timing_Helpers_Get_Timestamp() - module_cycle_prev);
      SweepBW_Modulewise_Time.timing_total += SweepBW_Modulewise_Time.per_bin_timing[RBIN0];
#endif
      spt_wait_done = spt_wait_done && Spt_Wait();

      PROFILING_STOP_PER_BIN(&cycle_count_prev, range_idx);

      ++range_idx; // range_idx = 2;
      dopp_in.rfft_compressed_output_addr = (uint64_t *)(&(*range_input_ptr)[range_idx + 1]);
      dopp_in.range_idx                   = range_idx;

      dopp_out.p_dfft_output = (DP_OutputDataCube_Ptr_T)(*dfft_bv_output_buffer)[(range_idx - 1) % BV_CIRCULAR_BUFF_DEPTH][0];
      dopp_out.p_nci_output  = (NCI_Buff_Ptr_T)(*rdop_avg_data)[(range_idx - 1) % NCI_CIRCULAR_BUFF_DEPTH];

      PROFILING_START(&module_cycle_prev);

      doppler_proc_retval =
         (DOPPLER_PROCESS_NO_ERROR ==
          Doppler_Process_Execute(&dopp_in, &dopp_out)); // Does doppler processing for Rbin2 and transfers Rbin1 data.

      PROFILING_STOP(&Doppler_Modulewise_Time, range_idx, module_cycle_prev);
      PROFILING_START(&module_cycle_prev);

      Appl_Cfar_Execute(RBIN0, p_rdd_data, &rdop_avg_data_ptr[0]);

      PROFILING_STOP(&Cfar_Modulewise_Time, RBIN0, module_cycle_prev);

      /* Reading the Per range bin scaling value from the NCI Buffer */
      p_rdd_data->per_rbin_scale[RBIN0] = Read_Per_Rbin_Scaling_Value(&rdop_avg_data_ptr[0]);
      /* Reading the Bin Undergone Rest value from the NCI Buffer */
      Read_Bin_Undergone_Rest_Value(&rdop_avg_data_ptr[0], p_rdd_data, RBIN0);

      PROFILING_START(&module_cycle_prev);

      First_Pass_Execute_No_Detections(RBIN0, p_rdd_data, look_id, &rdop_avg_data_ptr[0], p_rdd_int_buff,
                                       Get_NF_est_look_ptr(look_id), &d2m_msg_ptr->payload.ipc_m2d_payload.xcp_Info,
                                       d2m_msg_ptr); // Do MPRB and FP thold for Rbin0

      PROFILING_STOP(&FP_Modulewise_Time, RBIN0, module_cycle_prev);

      Circular_Buffer_Update();
      spt_wait_done = spt_wait_done && Spt_Wait();

      PROFILING_STOP_PER_BIN(&cycle_count_prev, range_idx);

      ++range_idx; // range_idx = 3;
      dopp_in.rfft_compressed_output_addr = (uint64_t *)(&(*range_input_ptr)[range_idx + 1]);
      dopp_in.range_idx                   = range_idx;

      dopp_out.p_dfft_output = (DP_OutputDataCube_Ptr_T)(*dfft_bv_output_buffer)[(range_idx - 1) % BV_CIRCULAR_BUFF_DEPTH][0];
      dopp_out.p_nci_output  = (NCI_Buff_Ptr_T)(*rdop_avg_data)[(range_idx - 1) % NCI_CIRCULAR_BUFF_DEPTH];

      PROFILING_START(&module_cycle_prev);

      doppler_proc_retval =
         (DOPPLER_PROCESS_NO_ERROR ==
          Doppler_Process_Execute(&dopp_in, &dopp_out)); // Does doppler processing for Rbin3 and transfers Rbin2 data.

      PROFILING_STOP(&Doppler_Modulewise_Time, range_idx, module_cycle_prev);
      PROFILING_START(&module_cycle_prev);

      Appl_Cfar_Execute(RBIN1, p_rdd_data, &rdop_avg_data_ptr[0]);

      PROFILING_STOP(&Cfar_Modulewise_Time, RBIN1, module_cycle_prev);

      /* Reading the Per range bin scaling value from the NCI Buffer */
      p_rdd_data->per_rbin_scale[RBIN1] = Read_Per_Rbin_Scaling_Value(&rdop_avg_data_ptr[0]);
      /* Reading the Bin Undergone Rest value from the NCI Buffer */
      Read_Bin_Undergone_Rest_Value(&rdop_avg_data_ptr[0], p_rdd_data, RBIN1);
      /*Run SweepBw for RBIN1*/
      sweep_bw_input.rbinIdx            = RBIN1;
      sweep_bw_input.bwdep_sample_shift = p_rdd_data->min_chirp_scaling;

      PROFILING_START(&module_cycle_prev);

      (void)Sweep_Bw_Calc(&sweep_bw_input, &sweep_bw_output);

      PROFILING_STOP(&SweepBW_Modulewise_Time, RBIN1, module_cycle_prev);
      PROFILING_START(&module_cycle_prev);

      First_Pass_Execute_No_Detections(RBIN1, p_rdd_data, look_id, &rdop_avg_data_ptr[0], p_rdd_int_buff,
                                       Get_NF_est_look_ptr(look_id), &d2m_msg_ptr->payload.ipc_m2d_payload.xcp_Info,
                                       d2m_msg_ptr); // Do MPRB and FP thold for Rbin1

      PROFILING_STOP(&FP_Modulewise_Time, RBIN1, module_cycle_prev);

#ifdef CDC_ENABLE
      PROFILING_START(&module_cycle_prev);

      (void)Appl_Cdc_Process_Execute(RBIN0, &rdop_avg_data_ptr[0]);
      Cdc_Packing(RBIN0, Cdc_Dbin_Count, &rdop_avg_data_ptr[0], SPBB_MAX_DOPPLER_FFT_SIZE);

      PROFILING_STOP(&CDC_Modulewise_Time, RBIN0, module_cycle_prev);
#endif

      Circular_Buffer_Update();
      spt_wait_done = spt_wait_done && Spt_Wait();

      PROFILING_STOP_PER_BIN(&cycle_count_prev, range_idx);

      // Invalidate the entire BV memory, in case it is still cached.
      // To this point, only the SPT has been writing this memory, no reads should have been done.
      xthal_dcache_block_writeback_inv((void *)dfft_bv_output_buffer, sizeof(*dfft_bv_output_buffer));

      ++range_idx; // range_idx = 4;
      for (; range_idx <= SPBB_MAX_RANGE_BINS; ++range_idx)
      {
         uint16_t cfar_fp_thold_range_idx =
            (uint16_t)(range_idx - 2U); // CFAR and FP Thold work two bins behind the doppler processing
         uint16_t fp_detections_range_idx = (uint16_t)(range_idx - 3U); // Detections work three bins behind the doppler processing
         sweep_bw_input.rbinIdx           = cfar_fp_thold_range_idx;
         dopp_in.rfft_compressed_output_addr = (uint64_t *)(&(*range_input_ptr)[range_idx + 1]);
         dopp_in.range_idx                   = range_idx;

         dopp_out.p_dfft_output = (DP_OutputDataCube_Ptr_T)(*dfft_bv_output_buffer)[(range_idx - 1) % BV_CIRCULAR_BUFF_DEPTH][0];
         dopp_out.p_nci_output  = (NCI_Buff_Ptr_T)(*rdop_avg_data)[(range_idx - 1) % NCI_CIRCULAR_BUFF_DEPTH];

         PROFILING_START(&module_cycle_prev);
         doppler_proc_retval = (DOPPLER_PROCESS_NO_ERROR == Doppler_Process_Execute(&dopp_in, &dopp_out));

         PROFILING_STOP_LAST(&Doppler_Modulewise_Time, range_idx, module_cycle_prev);
         PROFILING_START(&module_cycle_prev);

         Appl_Cfar_Execute(cfar_fp_thold_range_idx, p_rdd_data, &rdop_avg_data_ptr[0]);

         PROFILING_STOP(&Cfar_Modulewise_Time, cfar_fp_thold_range_idx, module_cycle_prev);
         /* Reading the Per range bin scaling value from the NCI Buffer */
         p_rdd_data->per_rbin_scale[cfar_fp_thold_range_idx] = Read_Per_Rbin_Scaling_Value(&rdop_avg_data_ptr[0]);
         /* Reading the Bin Undergone Rest value from the NCI Buffer */

         Read_Bin_Undergone_Rest_Value(&rdop_avg_data_ptr[0], p_rdd_data, cfar_fp_thold_range_idx);

         PROFILING_START(&module_cycle_prev);
         (void)Sweep_Bw_Calc(&sweep_bw_input, &sweep_bw_output);

         PROFILING_STOP(&SweepBW_Modulewise_Time, cfar_fp_thold_range_idx, module_cycle_prev);
         PROFILING_START(&module_cycle_prev);
         /* @todo Based on FP implementation, one of these will be removed. f*/
         First_Pass_Execute(fp_detections_range_idx, d2m_msg_ptr, look_id, &rdop_avg_data_ptr[0], p_rdd_int_buff,
                            Get_NF_est_look_ptr(look_id), &d2m_msg_ptr->payload.ipc_m2d_payload.xcp_Info
#ifdef SPBB_CDC_ENABLE
                            ,
                            Get_Cdc_Dbin_Array_Buffer_Ptr(), Cdc_Dbin_Cnt_Ptr
#endif
         ); // Do normalization/mprb, then fp_thold, then do detections

         PROFILING_STOP(&FP_Modulewise_Time, fp_detections_range_idx + 1, module_cycle_prev);
#ifdef CDC_ENABLE
         PROFILING_START(&module_cycle_prev);
         Cdc_Packing(fp_detections_range_idx, Cdc_Dbin_Count, &rdop_avg_data_ptr[0], SPBB_MAX_DOPPLER_FFT_SIZE);
         PROFILING_STOP(&CDC_Modulewise_Time, fp_detections_range_idx, module_cycle_prev);
#endif

         Circular_Buffer_Update();

         if (!Spt_Wait()) // Spt error encountered.
         {
            range_idx = SPBB_MAX_RANGE_BINS + 2;
            Doppler_Proc_SPT_Error_Count++;
            break;
         }
         else // Spt completed without error.
         {
            xthal_dcache_block_writeback_inv((void *)dopp_out.p_dfft_output, sizeof(*dopp_out.p_dfft_output));
         }

         if ((Timing_Helpers_Get_Timestamp() - doppler_proc_start_timestamp) >= MAX_DOPPLER_PROC_OVERRUN_TIME_TICKS)
         {
            range_idx = SPBB_MAX_RANGE_BINS + 2;
            break;
         }
         else
         {
            /* Do Nothing */
         }

         PROFILING_STOP_PER_BIN(&cycle_count_prev, range_idx);
      }

      /**
       * @brief Process the last bin of the data.
       *
       * Doppler processing execution is called (SPBB_MAX_RANGE_BINS + 1) times, with the last time
       * resulting in just a memory copy.
       *
       * Here, then, two things must be done to complete CFAR and First Pass:
       *    1. Process the last range bin for CFAR and FP thold
       *    2. Process the detections for the second to last range bin
       *
       */
      if (range_idx == (SPBB_MAX_RANGE_BINS + 1))
      {
         PROFILING_START(&module_cycle_prev);
         Appl_Cfar_Execute(SPBB_MAX_RANGE_BINS - 1, p_rdd_data, &rdop_avg_data_ptr[0]);
         PROFILING_STOP_LAST(&Cfar_Modulewise_Time, SPBB_MAX_RANGE_BINS - 1, module_cycle_prev);

         /* Reading the Per range bin scaling value from the NCI Buffer */
         p_rdd_data->per_rbin_scale[SPBB_MAX_RANGE_BINS - 1] = Read_Per_Rbin_Scaling_Value(&rdop_avg_data_ptr[0]);
         /* Reading the Bin Undergone Rest value from the NCI Buffer */
         Read_Bin_Undergone_Rest_Value(&rdop_avg_data_ptr[0], p_rdd_data, SPBB_MAX_RANGE_BINS - 1);
         /* Run Sweep BW for last RBIN*/
         sweep_bw_input.rbinIdx = SPBB_MAX_RANGE_BINS - 1;

         PROFILING_START(&module_cycle_prev);
         (void)Sweep_Bw_Calc(&sweep_bw_input, &sweep_bw_output);
         (void)Sweep_Bw_RBIN0_Scaling(&sweep_bw_input, &sweep_bw_output);

         PROFILING_STOP_LAST(&SweepBW_Modulewise_Time, SPBB_MAX_RANGE_BINS - 1, module_cycle_prev);
         PROFILING_START(&module_cycle_prev);

         First_Pass_Execute(SPBB_MAX_RANGE_BINS - 2, d2m_msg_ptr, look_id, &rdop_avg_data_ptr[0], p_rdd_int_buff,
                            Get_NF_est_look_ptr(look_id), &d2m_msg_ptr->payload.ipc_m2d_payload.xcp_Info

#ifdef SPBB_CDC_ENABLE
                            ,
                            Get_Cdc_Dbin_Array_Buffer_Ptr(), Cdc_Dbin_Cnt_Ptr
#endif
         ); // Do normalization/mprb, then fp_thold, then do detections

#ifdef SPBB_SCM_ENABLE
         p_rdd_data->rdd1_target_saturation_data.rdd1_targets_saturation_flag = p_rdd_int_buff->scm_targets_saturation_flag;
         p_rdd_data->rdd1_target_saturation_data.rdd1_max_rbin_processed      = p_rdd_int_buff->scm_max_rbin_processed;
         p_rdd_data->rdd1_target_saturation_data.rdd1_max_detectable_range    = p_rdd_int_buff->scm_max_detectable_range;
         p_rdd_data->rdd1_target_saturation_data.rdd1_scm_prioritization_flag = p_rdd_int_buff->scm_prioritization_flag;
#endif
         PROFILING_STOP_LAST(&FP_Modulewise_Time, SPBB_MAX_RANGE_BINS - 1, module_cycle_prev);
#ifdef CDC_ENABLE
         PROFILING_START(&module_cycle_prev);
         Cdc_Packing(SPBB_MAX_RANGE_BINS - 2, Cdc_Dbin_Count, &rdop_avg_data_ptr[0], SPBB_MAX_DOPPLER_FFT_SIZE);
         PROFILING_STOP(&CDC_Modulewise_Time, SPBB_MAX_RANGE_BINS - 2U, module_cycle_prev);
#endif
         PROFILING_START(&module_cycle_prev);
         Second_Pass_Execute(p_rdd_data, &d2m_msg_ptr->payload.xcp_data, &d2m_msg_ptr->payload.rdd_stream_data.rfft_data,
                             &d2m_msg_ptr->payload.ipc_m2d_payload.veh_data.veh_params);
#ifdef ENABLE_MODULEWISE_TIMING
         SP_Timing_Total = Timing_Helpers_Get_Time_Nanosec(Timing_Helpers_Get_Timestamp() - module_cycle_prev);
         if (SP_Timing_Total > SP_Time_Total_Max)
         {
            SP_Time_Total_Max = SP_Timing_Total;
         }
#endif
#ifdef CDC_ENABLE
         PROFILING_START(&module_cycle_prev);
         Circular_Buffer_Update();
         (void)Appl_Cdc_Process_Execute(SPBB_MAX_RANGE_BINS - 1U, &rdop_avg_data_ptr[0]);
         Cdc_Packing(SPBB_MAX_RANGE_BINS - 1, Cdc_Dbin_Count, &rdop_avg_data_ptr[0], SPBB_MAX_DOPPLER_FFT_SIZE);
         PROFILING_STOP_LAST(&CDC_Modulewise_Time, SPBB_MAX_RANGE_BINS - 1U, module_cycle_prev);
#endif

         PROFILING_STOP_PER_BIN(&cycle_count_prev, range_idx);
      }
      else
      {
         doppler_proc_retval = false;
      }

      /* Update the maximum execution time of the doppler/rdd processing. */
      doppler_rdd_runtime = Timing_Helpers_Get_Timestamp() - doppler_proc_start_timestamp;
      if (doppler_rdd_runtime > Doppler_Proc_Active_Ticks_Max)
      {
         Doppler_Proc_Active_Time_Max  = Timing_Helpers_Get_Time_Microsec(doppler_rdd_runtime);
         Doppler_Proc_Active_Ticks_Max = doppler_rdd_runtime;
      }
      else
      {
         /* Nothing*/
      }

      p_rdd_data->rdd_time     = Timing_Helpers_Get_Time_Microsec(doppler_rdd_runtime);
      p_rdd_data->rdd_time_max = Doppler_Proc_Active_Time_Max;

      Update_Debug_Stream_Check_Dopp_Overrun(p_rdd_debug_stream_data, m2d_bbe_one_time_msg_ptr, p_rfft_data, look_id,
                                             doppler_rdd_runtime, doppler_proc_retval, p_rdd_data);

#ifdef ENABLE_PER_BIN_TIMING
      cycle_count                 = Timing_Helpers_Get_Timestamp();
      Doppler_Proc_Cleanup_Timing = Timing_Helpers_Get_Time_Microsec(cycle_count - cycle_count_prev);
      Doppler_Proc_Active_Time    = Timing_Helpers_Get_Time_Microsec(cycle_count - doppler_proc_start_timestamp);
#endif
   }
   else
   {
      /* Mechanism to count errors in the initialization of doppler/RDD processing initialization */
      Doppler_Proc_Initialization_Fail_Count++;
   }

   /* Update REST count */
   d2m_msg_ptr->payload.rdd_stream_data.look_data.rest_count = Rest_Counter;

   /** Update Diag information*/
   d2m_msg_ptr->payload.diag_data.ipc_m2d_buffer_err_cnt      = IPC_M2D_Buffer_Err_Cnt;
   d2m_msg_ptr->payload.diag_data.dfft_rdd_xput_overrun_cnt   = (uint16_t)Doppler_Proc_Overrun_Counter;
   d2m_msg_ptr->payload.diag_data.spt_write_axi_error_counter = Get_Spt_Write_Axi_Error_Counter();
   d2m_msg_ptr->payload.diag_data.spt_write_lu_error_counter  = Get_Spt_Write_Lu_Error_Counter();

   /** Update RDD Stream header at the end.*/
   p_stream_hdr->scan_index     = p_look_data->scan_index;
   p_stream_hdr->size           = sizeof(Rdd_Stream_T);
   p_stream_hdr->module_time_ms = p_rdd_data->rdd_time;

   p_rdd_debug_stream_hdr->scan_index     = p_look_data->scan_index;
   p_rdd_debug_stream_hdr->size           = sizeof(Rdd_Debug_Stream_T);
   p_rdd_debug_stream_hdr->module_time_ms = p_rdd_data->rdd_time;

#ifdef CDC_ENABLE
   p_cdc_debug_stream_data->total_cdc_max_ethernet_frames_across_look = Cdc_Log_Param.cdc_look_num_enet_frames_max;
   p_cdc_debug_stream_data->total_cdc_ethernet_frames_per_look        = Cdc_Log_Param.cdc_look_num_enet_frames;
   p_cdc_debug_stream_data->total_cdc_max_bins_across_look            = Cdc_Log_Param.cdc_look_total_bins_max;
   p_cdc_debug_stream_data->total_cdc_bins_per_look                   = Cdc_Log_Param.cdc_look_total_bins;
   p_cdc_debug_stream_data->max_cdc_record_across_rbin_across_look    = Cdc_Log_Param.cdc_look_bins_per_rbin_max;
   p_cdc_debug_stream_data->max_cdc_record_across_rbin_per_look       = Cdc_Log_Param.cdc_bins_per_rbin_max;
#endif

   /*Passing address to verify Integration Testing; only runs when Integration_Testing macro is enabled*/
   SIT_D2M_MSG_BUFFER(d2m_msg_ptr);

   return doppler_proc_retval;
}

/**
 *  @b Description
 *  @n
 *   This function returns the per rbin scaling value
 *  @param[in1] : **rdop_avg_ptr
 *  @param[out]
 *  returns the Per rbin scaling value
 *  rbin scaling value is stored in the last 16 bit from the SPT and the value is constrained to the last 8 bits of that value
 **/
static uint8_t Read_Per_Rbin_Scaling_Value(rdop_avg_t **rdop_avg_ptr)
{
   uint16_t *ptr_nci_depth = rdop_avg_ptr[NCI_NEXT_BUFFER_DEPTH];
   return (uint8_t)(ptr_nci_depth[RBIN_SCALE_LOCATION]);
}

/**
 *  @b Description
 *  @n
 *   This function to update the RDD instrumentation variables
 *  @param[in1]
 *  @param[out]
 *
 **/
static void Update_Rdd_Instrumentation_Variables(D2M_Msg_T *d2m_msg_ptr)
{
   XCP_Info_T *xcp_info_ptr = &d2m_msg_ptr->payload.ipc_m2d_payload.xcp_Info;
   XCP_Data_T *xcp_data_ptr = &d2m_msg_ptr->payload.xcp_data;

   p_rdd_int_buff->xcp_range_idx            = xcp_info_ptr->xcp_range_idx;
   p_rdd_int_buff->xcp_doppler_idx          = xcp_info_ptr->xcp_doppler_idx;
   p_rdd_int_buff->xcp_range_idx_beamvector = xcp_info_ptr->xcp_range_idx_beamvector;
   p_rdd_int_buff->xcp_spe_gain_max         = xcp_info_ptr->xcp_spe_gain_max;
   p_rdd_int_buff->xcp_spe_gain_min         = xcp_info_ptr->xcp_spe_gain_min;

   if (CODED_TRUE == xcp_info_ptr->xcp_rdd_vary_xput_flag)
   {
      p_rdd_int_buff->xcp_rdd_vary_xput_flag = true;

      /**Reset max per rbin target- unused try to remove passed as pointer to first pass*/
      xcp_data_ptr->xcp_max_per_rbin_targets = 0U;

      if (CODED_TRUE != xcp_info_ptr->xcp_max_static_target_case_enable)
      {
         p_rdd_int_buff->xcp_max_static_target_case_enable = false;
         static uint16_t max_fp_tgt_cnt_local              = SPBB_FP_MAX_DETECT;
         static uint16_t rbin_inx_start_local              = 0U;
         /** This is set to default value 5 to get the worst case scenario.
          * This can be set as needed and tested */
         p_rdd_int_buff->xcp_max_fp_per_rbin_target_cnt = 5U;

         if (0U == max_fp_tgt_cnt_local || max_fp_tgt_cnt_local > SPBB_FP_MAX_DETECT_RUNTIME)
         {
            max_fp_tgt_cnt_local               = SPBB_FP_MAX_DETECT_RUNTIME;
            p_rdd_int_buff->xcp_max_fp_tgt_cnt = max_fp_tgt_cnt_local;

            if (CODED_TRUE == xcp_info_ptr->xcp_spe_gain_vary_enable_flag)
            {
               static float32_t xcp_spe_gain_local = SPE_GAIN_INCREMENT;
               xcp_spe_gain_local += SPE_GAIN_INCREMENT;

               if (xcp_spe_gain_local > p_rdd_int_buff->xcp_spe_gain_max || xcp_spe_gain_local < p_rdd_int_buff->xcp_spe_gain_min)
               {
                  xcp_spe_gain_local = p_rdd_int_buff->xcp_spe_gain_min;
               }
               else
               {
                  /* Do Nothing */
               }
               p_rdd_int_buff->xcp_spe_gain = xcp_spe_gain_local;
            }
            else
            {
               p_rdd_int_buff->xcp_spe_gain = xcp_info_ptr->xcp_spe_gain;
            }
         }
         else
         {
            p_rdd_int_buff->xcp_max_fp_tgt_cnt = max_fp_tgt_cnt_local;
            max_fp_tgt_cnt_local--; // Minimum of 1 detection will be forced.
         }
         p_rdd_int_buff->xcp_rngidx_target_start = rbin_inx_start_local;
         if ((uint16_t)(SPBB_MAX_RANGE_BINS - 1U) == rbin_inx_start_local)
         {
            rbin_inx_start_local = 0U;
         }
         else
         {
            rbin_inx_start_local++;
         }
      }
      else
      {
         p_rdd_int_buff->xcp_max_static_target_case_enable = true;
         p_rdd_int_buff->xcp_spe_gain                      = xcp_info_ptr->xcp_spe_gain;
         p_rdd_int_buff->xcp_max_fp_per_rbin_target_cnt    = SPBB_FP_MAX_DETECT_RUNTIME;
         p_rdd_int_buff->xcp_rngidx_target_start           = 0U;
         p_rdd_int_buff->xcp_max_fp_tgt_cnt                = SPBB_FP_MAX_DETECT_RUNTIME;
      }
   }
   else
   {
      p_rdd_int_buff->xcp_rdd_vary_xput_flag         = false;
      p_rdd_int_buff->xcp_spe_gain                   = xcp_info_ptr->xcp_spe_gain;
      p_rdd_int_buff->xcp_max_fp_per_rbin_target_cnt = SPBB_FP_MAX_DETECT_RUNTIME;
      p_rdd_int_buff->xcp_rngidx_target_start        = 0U;
      p_rdd_int_buff->xcp_max_fp_tgt_cnt             = SPBB_FP_MAX_DETECT_RUNTIME;
   }
}

/**
 *  @b Description
 *  @n
 *   This function returns the Bin Undergone Rest value
 *  @param[in1] : **rdop_avg_ptr
 *  @param[in2] : *p_rdd_data
 *  @param[in3] : range_idx
 *  @param[out]
 *  returns none
 **/
static void Read_Bin_Undergone_Rest_Value(rdop_avg_t **rdop_avg_ptr, RDD_Data_T *p_rdd_data, uint16_t range_idx)
{
   uint16_t *ptr_nci_depth = rdop_avg_ptr[NCI_NEXT_BUFFER_DEPTH];
   if (REST_PATTERN == ptr_nci_depth[REST_RBIN_LOCATION])
   {
      p_rdd_data->rest_bin_proc_flag[Rest_Counter] = range_idx;
      Rest_Counter++;
   }
}

/**
 *  @b Description
 *  @n
 *   This function updates Debug stream and checks for doppler overrun
 *  @param[in] : m2d_bbe_one_time_msg_ptr, doppler_proc_retval
 *               look_id, doppler_rdd_runtime, p_rdd_data
 *  @param[out] : p_rdd_debug_stream_data, p_rfft_data, p_rdd_data,
 *                Doppler_Proc_Overrun_Counter
 *  returns none
 **/
static inline void Update_Debug_Stream_Check_Dopp_Overrun(Rfft_Debug_Data_T *p_rdd_debug_stream_data,
                                                          M2D_One_Time_Msg_T *m2d_bbe_one_time_msg_ptr, Rfft_Data_T *p_rfft_data,
                                                          Radar_Look_T look_id, uint32_t doppler_rdd_runtime,
                                                          bool doppler_proc_retval, RDD_Data_T *p_rdd_data)
{
   uint16_t idx                   = 0U;
   bool doppler_proc_overrun_flag = false;

   for (idx = 0; idx < SPBB_MAX_SAMPLES; idx++)
   {
      p_rdd_debug_stream_data->first_interfered_chirp[idx] =
         ((Radar_Data16_T *)m2d_bbe_one_time_msg_ptr->idm_xcp_sram_address)[idx].I_data;
      p_rdd_debug_stream_data->gating_window[idx] = ((Radar_Data16_T *)m2d_bbe_one_time_msg_ptr->idm_xcp_sram_address)[idx].Q_data;
   }

   for (idx = 0; idx < K_MAX; idx++)
   {
      p_rfft_data->interfered_samples_per_chirp[idx] =
         (uint16_t)(((Radar_Data32_T *)m2d_bbe_one_time_msg_ptr->idm_sram_address)[idx].Q_data);
      p_rdd_debug_stream_data->idm_cfar_thold[idx] = ((Radar_Data32_T *)m2d_bbe_one_time_msg_ptr->idm_sram_address)[idx].I_data;
      p_rdd_debug_stream_data->idm_cfar2_thold[idx] =
         ((Radar_Data32_T *)m2d_bbe_one_time_msg_ptr->idm_sram_address)[idx + MAX_DOPPLER_FFT_SIZE].I_data;
   }
   memcpy((void *)p_rdd_debug_stream_data->idm_thold_upperlimit_prev_scan, (void *)m2d_bbe_one_time_msg_ptr->idm_thold_upperlimit,
          sizeof(uint32_t) * RP_IDM_GROUP_IDX);
   if (doppler_rdd_runtime > MAX_DOPPLER_PROC_OVERRUN_TIME_TICKS)
   {
      Doppler_Proc_Overrun_Counter++;
      doppler_proc_overrun_flag = true;
   }
   else
   {
      /* Nothing*/
   }

   if (!doppler_proc_retval || doppler_proc_overrun_flag)
   {
      memset(&(p_rdd_data->rdd2_sp_fail_flag[0]), 1U, p_rdd_data->rdd1_num_detect);
      p_rdd_data->rdd1_num_detect = 0U;
      p_rdd_data->rdd2_num_detect = 0U;
   }
   else
   {
      /* Nothing*/
   }
}
