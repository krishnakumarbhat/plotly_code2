#include <string>
#include "sfl_wrapper.h"

/* headers for FBK */
#include "fbk_core_calibration_t.h"
#include "fbk_iface.h"
#include "fbk_index_lookup.h"
#include "fbk_instance.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_ref_point.h"
#include "fbk_vehicle_data_t.h"

/* headers for PA */
#include "pa_context.h"
#include "pa_obj_in.h"
#include "pa_shared_types.h"
#include "pa_vehicle_in.h"

/* headers for ML Library */
#include "ml_angle_range.h"
#include "ml_angle_t.h"
#include "ml_checked_rounding.h"
#include "ml_interval.h"
#include "ml_math.h"
#include "ml_math_infinity_silent.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_angle.h"
#include "ml_vector_2d_t.h"

/* headers for PT */
#include "pt_core_calibration_t.h"
#include "pt_iface.h"

/* headers for CTA */
#include "cta_core_calibration_t.h"
#include "cta_iface.h"
#include "cta_input_t.h"

#include "cta_types.h"

/* headers for CED */
#include "ced_core_calibration_t.h"
#include "ced_iface.h"
#include "ced_input_t.h"

/* headers for LCDA */
#include "lcda_core_calibration_t.h"
#include "lcda_iface.h"
#include "lcda_input_t.h"

#include "lcda_types.h"

/* headers for RECW */
#include "recw_core_calibration_t.h"
#include "recw_iface.h"
#include "recw_input_t.h"

#include "recw_types.h"

/* headers for SCW */
#include "scw_core_calibration_t.h"
#include "scw_iface.h"
#include "scw_input_t.h"

#include "scw_types.h"

/* headers for TA */
#include "ta_core_calibration_t.h"
#include "ta_iface.h"
#include "ta_input_t.h"

#include "ta_types.h"

/* headers for LTB */
#include "ltb_core_calibration.h"
#include "ltb_iface.h"
#include "ltb_input_t.h"

#include "ltb_types.h"

/* headers for ESA */
#include "esa_core_calibration_t.h"
#include "esa_iface.h"
#include "esa_input_t.h"

#include "esa_types.h"

/* Local Preprocessor constants */
#define SFL_Convert_KMPH_TO_MPS(x)     ((x) * 0.277778f)
#define SFL_Convert_CM_TO_M(x)         ((x) * 0.01f)
#define SFL_REVERSE_DIRECTION_VEH_SIGN (-1.0f)

#define SFL_EXECUTION_TIME              (0.05f)
#define RCTA_FFSM_OP_STATE_OPERATIONAL  (3u)
#define CFG_Length_Fov_RCTA             (60.0f)
#define CFG_Object_Heading_Min_RCTA     (0.7854f)
#define CFG_Object_Heading_Max_RCTA     (2.356f)
#define CFG_Alert_Hold_Time_RCTA        (0.15f)
#define RCTA_SFL_CUST_COORD_ORIENTATION (90.0f)

#define CED_FFSM_OP_STATE_OPERATIONAL             (3u)
#define CFG_Ego_Speed_Max_CED                     (0.833f)
#define CFG_Object_Heading_Max_CED                (0.6f)
#define CFG_Funnel_Zone_Width_CED                 (2.3f)
#define CFG_Funnel_Zone_Length_CED                (60.0f)
#define CFG_Ego_Lane_Widthh_CED                   (1.8f)
#define CED_REF_POINT_FUNNEL_CHECK_DEFAULT        (0u)
#define CED_REF_POINT_FUNNEL_CHECK_FRONT_BUMPER   (1u)
#define CED_REF_POINT_FUNNEL_CHECK_NEAREST_CORNER (2u)

#define LCDA_FFSM_OP_STATE_OPERATIONAL            (3u)
#define CFG_BSW_X0_HYS_LCDA                       (1.0f)
#define CFG_BSW_X1_HYS_LCDA                       (1.0f)
#define CFG_BSW_Y0_HYS_LCDA                       (0.5f)
#define CFG_BSW_Y1_HYS_LCDA                       (1.0f)
#define CFG_LCW_X0_LCDA                           (0.0f)
#define CFG_LCW_Y_WIDTH1_LCDA                     (2.5f)
#define CFG_LCW_Y0_HYS_LCDA                       (0.5f)
#define CFG_LCW_Y1_HYS_LCDA                       (0.1f)
#define k_cvw_max_curvi_heading_abs_LCDA          (15.011494f)
#define k_bsw_max_heading_abs_LCDA                (44.977187f)
#define CFG_BSW_SOT_MIN_WARN_DURATION_LCDA        (0.5f)
#define CFG_BSW_OPERATIONAL_SPEED_HYSTERESIS_LCDA (1.0f)
#define CFG_TTC_TOL_LCDA                          (0.5f)
#define BSW_CVW_MIN_LONG_SPEED_KMPH               (6.0f)
#define LCDA_ZONE_CHECK_METHOD_OVERLAP            (0u)
#define LCDA_ZONE_CHECK_METHOD_REF_POINT          (1u)

static Fbk_Instance_T SFL_Fbk_Instance;
static Fbk_Output_T SFL_Fbk_Output;
static Pa_Context_T SFL_Pa_Context;

static Pa_Data_T SFL_Pa_Data;
static Pa_Data_T *SFL_Pa_Data_GPtr = &SFL_Pa_Data;

static Pt_Instance_T SFL_Pt_Instance;
static Pt_Output_T SFL_Pt_Output;
static Pt_Output_T *SFL_Pt_Output_GPtr = &SFL_Pt_Output;

static Cta_Input_T SFL_Cta_Input;
static Cta_Instance_T SFL_Cta_Instance;
static Cta_Output_T SFL_Cta_Output;

static Ced_Instance_T SFL_Ced_Instance;
static Ced_Input_T SFL_Ced_Input;
static Ced_Output_T SFL_Ced_Output;

static Lcda_Instance_T SFL_Lcda_Instance;
static Lcda_Input_T SFL_Lcda_Input;
static Lcda_Output_T SFL_Lcda_Output;

static Recw_Instance_T SFL_Recw_Instance;
static Recw_Input_T SFL_Recw_Input;
static Recw_Output_T SFL_Recw_Output;

static Scw_Instance_T SFL_Scw_Instance;
static Scw_Input_T SFL_Scw_Input;
static Scw_Output_T SFL_Scw_Output;

static Ta_Instance_T SFL_Ta_Instance;
static Ta_Input_T SFL_Ta_Input;
static Ta_Output_T SFL_Ta_Output;

static Ltb_Instance_T SFL_Ltb_Instance;
static Ltb_Input_T SFL_Ltb_Input;
static Ltb_Output_T SFL_Ltb_Output;

static Esa_Instance_T SFL_Esa_Instance;
static Esa_Input_T SFL_Esa_Input;
static Esa_Output_T SFL_Esa_Output;

/**************************************************************************************************\
* Local Function Prototypes
\**************************************************************************************************/
Fbk_Output_T *FeatureFunctionGetOutputPtr();
static void SFLFbkCustCalUpdateInit(Fbk_Core_Calibration_T *SFL_Fbk_Cal_Ptr);
static void SFLFbkInit(Pa_Context_T *SFL_Pa_Context_Ptr, Fbk_Instance_T *SFL_Fbk_Instance_Ptr, Pa_Data_T *SFL_Pa_Data_Ptr);
static void SFLFbkRun(Fbk_Instance_T *SFL_Fbk_Instance_Ptr, Fbk_Output_T *SFL_Fbk_Output_Ptr, Pa_Context_T *SFL_Pa_Context_Ptr);

static void SFLPtInit(Pt_Instance_T *SFL_Pt_Instance_Ptr);
static void SFLPtRun(Pt_Instance_T *SFL_Pt_Instance_Ptr, Pt_Output_T *SFL_Pt_Output_Ptr, const Fbk_Output_T *SFL_Fbk_Output_Ptr);
static void SFLPtCustCalUpdateInit(Pt_Core_Calibration_T *SFL_Pt_Cal_Ptr);

static void SFLCtaCustCalUpdateInit(Cta_Core_Calibration_T *SFL_Cta_Cal_Ptr);
static void SFLCtaCust_CalUpdatePeriodic(Cta_Core_Calibration_T *SFL_Cta_Cal_Ptr);
static void SFLCtaInit(Cta_Instance_T *SFL_Cta_Instance_Ptr);
static void SFLCtaMain(Cta_Input_T *SFL_Cta_Input_Ptr, const Fbk_Output_T *SFL_Fbk_Output_Ptr,
                       const Pt_Output_T *SFL_Pt_Output_Ptr, Cta_Instance_T *SFL_Cta_Instance_Ptr,
                       Cta_Output_T *SFL_Cta_Output_Ptr, const Pa_Context_T *SFL_Pa_Context_Ptr);

static void SFLCedCustCalUpdateInit(Ced_Core_Calibration_T *SFL_Ced_Cal_Ptr);
static void SFL_Ced_Cust_Cal_Update_Periodic(Ced_Core_Calibration_T *SFL_Ced_Cal_Ptr);
static void SFLCedInit(Ced_Instance_T *SFL_Ced_Instance_Ptr);
static void SFLCedMain(Ced_Instance_T *SFL_Ced_Instance_Ptr, Ced_Input_T *SFL_Ced_Input_Ptr, Ced_Output_T *SFL_Ced_Output_Ptr,
                       const Fbk_Output_T *SFL_Fbk_Output_Ptr, const Pt_Output_T *SFL_Pt_Output_Ptr);

static void SFLLcdaCustCalUpdateInit(Lcda_Core_Calibration_T *SFL_Lcda_Cal_Ptr);
static void SFLLcdaCustCalUpdatePeriodic(Lcda_Core_Calibration_T *SFL_Lcda_Cal_Ptr);
static void SFLLcdaInit(Lcda_Instance_T *SFL_Lcda_Instance_Ptr);
static void SFLLcdaMain(Lcda_Instance_T *SFL_Lcda_Instance_Ptr, Lcda_Input_T *SFL_Lcda_Input_Ptr,
                        const Fbk_Output_T *SFL_Fbk_Output_Ptr, Lcda_Output_T *SFL_Lcda_Output_Ptr);

static void SFLRecwCustCalUpdateInit(Recw_Core_Calibration_T *SFL_Recw_Cal_Ptr);
static void SFLRecwCustCalUpdatePeriodic(Recw_Core_Calibration_T *SFL_Recw_Cal_Ptr);
static void SFLRecwInit(Recw_Instance_T *SFL_Recw_Instance_Ptr);
static void SFLRecwMain(Recw_Instance_T *SFL_Recw_Instance_Ptr, Recw_Input_T *SFL_Recw_Input_Ptr,
                        const Fbk_Output_T *SFL_Fbk_Output_Ptr, Recw_Output_T *SFL_Recw_Output_Ptr);

static void SFLScwCustCalUpdateInit(Scw_Core_Calibration_T *SFL_Scw_Cal_Ptr);
static void SFLScwCustCalUpdatePeriodic(Scw_Core_Calibration_T *SFL_Scw_Cal_Ptr);
static void SFLScwInit(Scw_Instance_T *SFL_Scw_Instance_Ptr);
static void SFLScwMain(Scw_Instance_T *SFL_Scw_Instance_Ptr, Scw_Input_T *SFL_Scw_Input_Ptr,
                       const Fbk_Output_T *SFL_Fbk_Output_Ptr, Scw_Output_T *SFL_Scw_Output_Ptr);

static void SFLTaCustCalUpdateInit(Ta_Core_Calibration_T *SFL_Ta_Cal_Ptr);
static void SFLTaCustCalUpdatePeriodic(Ta_Core_Calibration_T *SFL_Ta_Cal_Ptr);
static void SFLTaInit(Ta_Instance_T *SFL_Ta_Instance_Ptr);
static void SFLTaMain(Ta_Instance_T *SFL_Ta_Instance_Ptr, Ta_Input_T *SFL_Ta_Input_Ptr,
                      const Fbk_Output_T *SFL_Fbk_Output_Ptr, Ta_Output_T *SFL_Ta_Output_Ptr);

static void SFLLtbCustCalUpdateInit(Ltb_Core_Calibration_T *SFL_Ltb_Cal_Ptr);
static void SFLLtbCustCalUpdatePeriodic(Ltb_Core_Calibration_T *SFL_Ltb_Cal_Ptr);
static void SFLLtbInit(Ltb_Instance_T *SFL_Ltb_Instance_Ptr);
static void SFLLtbMain(Ltb_Instance_T *SFL_Ltb_Instance_Ptr, Ltb_Input_T *SFL_Ltb_Input_Ptr,
                       const Fbk_Output_T *SFL_Fbk_Output_Ptr, Ltb_Output_T *SFL_Ltb_Output_Ptr);

static void SFL_Esa_Cust_Cal_Update_Init(Esa_Core_Calibration_T *SFL_Esa_Cal_Ptr);
static void SFL_Esa_Cust_Cal_Update_Periodic(Esa_Core_Calibration_T *SFL_Esa_Cal_Ptr);
static void SFLEsaInit(Esa_Instance_T *SFL_Esa_Instance_Ptr);
static void SFLEsaMain(Esa_Instance_T *SFL_Esa_Instance_Ptr, Esa_Input_T *SFL_Esa_Input_Ptr,
                       const Fbk_Output_T *SFL_Fbk_Output_Ptr, Esa_Output_T *SFL_Esa_Output_Ptr);

extern "C" Cta_Output_T *Cta_Get_Output_Ptr(void) {
   return &SFL_Cta_Output;
}

extern "C" Ta_Output_T *Ta_Get_Output_Ptr(void) {
   return &SFL_Ta_Output;
}

extern "C" Scw_Output_T *Scw_Get_Output_Ptr(void) {
   return &SFL_Scw_Output;
}

extern "C" Recw_Output_T *Recw_Get_Output_Ptr(void) {
   return &SFL_Recw_Output;
}

extern "C" Ltb_Output_T *Ltb_Get_Output_Ptr(void) {
   return &SFL_Ltb_Output;
}

extern "C" Lcda_Output_T *Lcda_Get_Output_Ptr(void) {
   return &SFL_Lcda_Output;
}

extern "C" Esa_Output_T *Esa_Get_Output_Ptr(void) {
   return &SFL_Esa_Output;
}

extern "C" Ced_Output_T *Ced_Get_Output_Ptr(void) {
   return &SFL_Ced_Output;
}

extern "C" Pt_Output_T *Pt_Get_Output_Ptr(void) {
   return &SFL_Pt_Output;
}

extern "C" Lcda_Instance_T *Lcda_Get_Instance_Ptr(void) {
   return &SFL_Lcda_Instance;
}

extern "C" Cta_Instance_T *Cta_Get_Instance_Ptr(void) {
   return &SFL_Cta_Instance;
}

extern "C" Ced_Instance_T *Ced_Get_Instance_Ptr(void) {
   return &SFL_Ced_Instance;
}

extern "C" Esa_Instance_T *Esa_Get_Instance_Ptr(void) {
   return &SFL_Esa_Instance;
}

extern "C" Ltb_Instance_T *Ltb_Get_Instance_Ptr(void) {
   return &SFL_Ltb_Instance;
}

extern "C" Pt_Instance_T *Pt_Get_Instance_Ptr(void) {
   return &SFL_Pt_Instance;
}

extern "C" Recw_Instance_T *Recw_Get_Instance_Ptr(void) {
   return &SFL_Recw_Instance;
}

extern "C" Scw_Instance_T *Scw_Get_Instance_Ptr(void) {
   return &SFL_Scw_Instance;
}

extern "C" Ta_Instance_T *Ta_Get_Instance_Ptr(void) {
   return &SFL_Ta_Instance;
}

static void SFLFbkCustCalUpdateInit(Fbk_Core_Calibration_T *SFL_Fbk_Cal_Ptr) {
   if (nullptr != SFL_Fbk_Cal_Ptr) {
      /* Hardcoded same as in baseline project (SWEET400) */
      SFL_Fbk_Cal_Ptr->k_fbk_host_trail_max_recording_speed = 15.0f;
      SFL_Fbk_Cal_Ptr->k_fbk_host_trail_dist_separation     = 5.0f;
      SFL_Fbk_Cal_Ptr->k_fbk_host_trail_heading_separation  = 0.2617f;
   }
}

static void SFLFbkInit(Pa_Context_T *SFL_Pa_Context_Ptr, Fbk_Instance_T *SFL_Fbk_Instance_Ptr, Pa_Data_T *SFL_Pa_Data_Ptr) {
   if ((nullptr != SFL_Pa_Context_Ptr) && (nullptr != SFL_Fbk_Instance_Ptr) && (nullptr != SFL_Pa_Data_Ptr)) {
      (void)Fbk_Init_Platform(SFL_Fbk_Instance_Ptr, SFL_Pa_Data_Ptr);
      //(void)SFLFbkCustCalUpdateInit(&SFL_Fbk_Instance_Ptr->calibration);
      SFL_Pa_Context_Ptr->p_data = SFL_Pa_Data_Ptr;
   }
}

static void SFLPtCustCalUpdateInit(Pt_Core_Calibration_T *SFL_Pt_Cal_Ptr) {
   if (nullptr != SFL_Pt_Cal_Ptr) {
      /* Hardcoded same as in baseline project (SWEET400) */
      SFL_Pt_Cal_Ptr->k_pt_kill_path_exceed_dist_thres              = 30.0f;
      SFL_Pt_Cal_Ptr->k_pt_lower_lim_obj_orient_lat                 = 0.65f;
      SFL_Pt_Cal_Ptr->k_pt_upper_lim_obj_orient_lat                 = 2.49f;
      SFL_Pt_Cal_Ptr->k_pt_apply_move_point_min_yaw_rate            = 0.016f;
      SFL_Pt_Cal_Ptr->k_pt_move_point_yaw_rate_thres_calc_ego_shift = 0.00001f;
      SFL_Pt_Cal_Ptr->k_pt_min_exist_prob_to_be_valid               = 0.85f;
      SFL_Pt_Cal_Ptr->k_pt_dist_obj_to_path_conf_lut[0]             = 1.0f;
      SFL_Pt_Cal_Ptr->k_pt_dist_obj_to_path_conf_lut[1]             = 0.8f;
      SFL_Pt_Cal_Ptr->k_pt_dist_obj_to_path_conf_lut[2]             = 0.68f;
      SFL_Pt_Cal_Ptr->k_pt_dist_obj_to_path_conf_lut[3]             = 0.40f;
      SFL_Pt_Cal_Ptr->k_pt_dist_obj_to_path_conf_lut[4]             = 0.10f;
      SFL_Pt_Cal_Ptr->k_pt_start_of_lane_change_processing          = 4u;
      SFL_Pt_Cal_Ptr->k_pt_end_of_lane_change_processing            = 28u;
   }
}

static void SFLPtInit(Pt_Instance_T *SFL_Pt_Instance_Ptr) {

   if (nullptr != SFL_Pt_Instance_Ptr) {
      (void)Pt_Init_Platform(SFL_Pt_Instance_Ptr);

      //(void)SFLPtCustCalUpdateInit(&SFL_Pt_Instance_Ptr->calibration);
   }
}

static void SFLCtaCustCalUpdateInit(Cta_Core_Calibration_T *SFL_Cta_Cal_Ptr) {
   /* Calibration parameters from Customer(configure directly in SW) */
   SFL_Cta_Cal_Ptr->k_cta_max_length_fov                = CFG_Length_Fov_RCTA;
   SFL_Cta_Cal_Ptr->k_cta_cycle_count_hold_true_warning = static_cast<uint8_t>(Ml_Roundf(CFG_Alert_Hold_Time_RCTA / SFL_EXECUTION_TIME));

   /* Hardcoded based on customer requirements */
   SFL_Cta_Cal_Ptr->k_cta_f_calc_ttc_ego_side_enabled                     = FBK_FALSE;
   SFL_Cta_Cal_Ptr->k_cta_f_use_heading_for_relative_velocity_calculation = FBK_FALSE;

   /* Hardcoded same as in baseline project (SWEET400) */
   SFL_Cta_Cal_Ptr->k_cta_min_object_age_check_valid                         = 4u;
   SFL_Cta_Cal_Ptr->k_cta_f_use_front_corners_dist_stop                      = FBK_FALSE;
   SFL_Cta_Cal_Ptr->k_cta_speed_criticality_level[0][0]                      = 0.0f;
   SFL_Cta_Cal_Ptr->k_cta_speed_criticality_level[0][1]                      = 0.0f;
   SFL_Cta_Cal_Ptr->k_cta_speed_criticality_level[1][0]                      = 0.0f;
   SFL_Cta_Cal_Ptr->k_cta_speed_criticality_level[1][1]                      = 0.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_lat[0]                                   = 0.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_lat[1]                                   = 0.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_lat[2]                                   = 50.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_lat[3]                                   = 50.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_lat[4]                                   = 0.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_lat[5]                                   = 0.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_lat[6]                                   = 0.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_lat[7]                                   = 0.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_long[0]                                  = 9.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_long[1]                                  = -14.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_long[2]                                  = -39.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_long[3]                                  = 34.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_long[4]                                  = 0.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_long[5]                                  = 0.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_long[6]                                  = 0.0f;
   SFL_Cta_Cal_Ptr->k_cta_butterfly_long[7]                                  = 0.0f;
   SFL_Cta_Cal_Ptr->k_cta_stop_alert_ttc                                     = 0.0f;
   SFL_Cta_Cal_Ptr->k_cta_min_lateral_approach_speed                         = 0.68f;
   SFL_Cta_Cal_Ptr->k_cta_min_ttc_additional_mature_qualification            = 3.5f;
   SFL_Cta_Cal_Ptr->k_cta_max_object_eclipse_for_level_qualification         = 0.4f;
   SFL_Cta_Cal_Ptr->k_cta_sensor_fov_border[0]                               = 0.890118f;
   SFL_Cta_Cal_Ptr->k_cta_sensor_fov_border[1]                               = 02.46091f;
   SFL_Cta_Cal_Ptr->k_cta_max_heading_variance                               = 3.0f;
   SFL_Cta_Cal_Ptr->k_cta_range_to_path_segment_ghost_qualif                 = 6.0f;
   SFL_Cta_Cal_Ptr->k_cta_f_adapt_intersect_lines_by_steering_angle          = FBK_FALSE;
   SFL_Cta_Cal_Ptr->k_cta_f_adapt_intersect_lines_by_obj_heading             = FBK_FALSE;
   SFL_Cta_Cal_Ptr->k_cta_f_apply_heading_compensation_on_intersection_point = FBK_FALSE;
   SFL_Cta_Cal_Ptr->k_cta_f_use_ghost_detector                               = FBK_TRUE;
   SFL_Cta_Cal_Ptr->k_cta_f_use_object_min_object_age_in_cycles              = FBK_FALSE;
   SFL_Cta_Cal_Ptr->k_cta_f_enable_thres_crit_level_reset                    = FBK_TRUE;
   SFL_Cta_Cal_Ptr->k_cta_enable_modes[0]                                    = FBK_TRUE;
   SFL_Cta_Cal_Ptr->k_cta_enable_modes[1]                                    = FBK_FALSE;
   SFL_Cta_Cal_Ptr->k_cta_min_object_age_thres                               = 3u;
   SFL_Cta_Cal_Ptr->k_cta_cycles_coasted_to_ignore                           = 6u;
   SFL_Cta_Cal_Ptr->k_cta_min_mature_cycles_level_qualifiction               = 3u;
   SFL_Cta_Cal_Ptr->k_cta_additional_qualification_mature_cycles             = 2u;
   SFL_Cta_Cal_Ptr->k_cta_min_age_obj_outside_sensor_fov                     = 10u;
   SFL_Cta_Cal_Ptr->k_cta_min_qual_age_obj_crossing_paths                    = 20u;
}

static void SFLCtaCust_CalUpdatePeriodic(Cta_Core_Calibration_T *SFL_Cta_Cal_Ptr) {
   SFL_Vehicle_Output_T *sfl_vehicle_ptr = GetSFLVehiclePtr();

   SFL_Cta_Cal_Ptr->k_cta_ego_abs_speed_max                      = SFL_Convert_KMPH_TO_MPS(10);
   SFL_Cta_Cal_Ptr->k_cta_min_speed                              = SFL_Convert_KMPH_TO_MPS(4);
   SFL_Cta_Cal_Ptr->k_cta_max_speed                              = SFL_Convert_KMPH_TO_MPS(360);
   SFL_Cta_Cal_Ptr->k_cta_angles_zone_definition[0]              = Fbk_Deg_To_Rad(RCTA_SFL_CUST_COORD_ORIENTATION - 45.0F);
   SFL_Cta_Cal_Ptr->k_cta_angles_zone_definition[1]              = Fbk_Deg_To_Rad(135.0F - RCTA_SFL_CUST_COORD_ORIENTATION);
   SFL_Cta_Cal_Ptr->k_cta_heading_range[0]                       = Fbk_Deg_To_Rad(45);
   SFL_Cta_Cal_Ptr->k_cta_heading_range[1]                       = Fbk_Deg_To_Rad(135);
   SFL_Cta_Cal_Ptr->k_cta_ttc_criticality_level[0][0]            = 3.0F + (SFL_Cta_Cal_Ptr->k_cta_cycle_count_suppress_true_warning * SFL_EXECUTION_TIME);
   SFL_Cta_Cal_Ptr->k_cta_ttc_criticality_level[0][1]            = 3.0F + (SFL_Cta_Cal_Ptr->k_cta_cycle_count_suppress_true_warning * SFL_EXECUTION_TIME);
   SFL_Cta_Cal_Ptr->k_cta_ttc_criticality_level[1][0]            = 3.0F + (SFL_Cta_Cal_Ptr->k_cta_cycle_count_suppress_true_warning * SFL_EXECUTION_TIME);
   SFL_Cta_Cal_Ptr->k_cta_ttc_criticality_level[1][1]            = 3.0F + (SFL_Cta_Cal_Ptr->k_cta_cycle_count_suppress_true_warning * SFL_EXECUTION_TIME);
   SFL_Cta_Cal_Ptr->k_cta_min_long_point_criticality_level[0][0] = 6.0F;
   SFL_Cta_Cal_Ptr->k_cta_min_long_point_criticality_level[0][1] = 6.0F;
   SFL_Cta_Cal_Ptr->k_cta_min_long_point_criticality_level[1][0] = 6.0F;
   SFL_Cta_Cal_Ptr->k_cta_min_long_point_criticality_level[1][1] = 6.0F;
   SFL_Cta_Cal_Ptr->k_cta_max_long_point_criticality_level[0][0] = -FBK_ONE_F * 1.5F;
   SFL_Cta_Cal_Ptr->k_cta_max_long_point_criticality_level[0][1] = -FBK_ONE_F * 1.5F;
   SFL_Cta_Cal_Ptr->k_cta_max_long_point_criticality_level[1][0] = (-FBK_ONE_F * sfl_vehicle_ptr->host_length) + 1.5F;
   SFL_Cta_Cal_Ptr->k_cta_max_long_point_criticality_level[1][1] = (-FBK_ONE_F * sfl_vehicle_ptr->host_length) + 1.5F;
}

static void SFLCtaInit(Cta_Instance_T *SFL_Cta_Instance_Ptr) {
   if (nullptr != SFL_Cta_Instance_Ptr) {
      (void)Cta_Init_Platform(SFL_Cta_Instance_Ptr);

      //(void)SFLCtaCustCalUpdateInit(&SFL_Cta_Instance_Ptr->calibration);
   }
}

static void SFLCedCustCalUpdateInit(Ced_Core_Calibration_T *SFL_Ced_Cal_Ptr) {
   /* Calibration parameters (configure directly in SW) */
   SFL_Ced_Cal_Ptr->k_ced_funnel_zone_width                 = CFG_Funnel_Zone_Width_CED;
   SFL_Ced_Cal_Ptr->k_ced_funnel_zone_length                = CFG_Funnel_Zone_Length_CED;
   SFL_Ced_Cal_Ptr->k_ced_ego_lane_width                    = CFG_Ego_Lane_Widthh_CED;
   SFL_Ced_Cal_Ptr->k_ced_object_heading_abs_angle_max      = CFG_Object_Heading_Max_CED;
   SFL_Ced_Cal_Ptr->k_ced_alert_holding_obj_abs_heading_max = CFG_Object_Heading_Max_CED;
   SFL_Ced_Cal_Ptr->k_ced_ego_abs_speed_max                 = CFG_Ego_Speed_Max_CED;

   /* Hardcoded based on customer requirements */
   SFL_Ced_Cal_Ptr->k_ced_f_choose_ref_point_funnel_check = CED_REF_POINT_FUNNEL_CHECK_FRONT_BUMPER;
   SFL_Ced_Cal_Ptr->k_ced_f_third_warning_level_enable    = FBK_FALSE;

   /* Hardcoded same as in baseline project (SWEET400) */
   SFL_Ced_Cal_Ptr->k_ced_object_age_min                                      = 3u;
   SFL_Ced_Cal_Ptr->k_ced_min_cycles_for_path_match_for_no_suppress           = 3u;
   SFL_Ced_Cal_Ptr->k_ced_suppress_alert_object_age_max                       = 20u;
   SFL_Ced_Cal_Ptr->k_ced_allow_opposite_side_alerts                          = FBK_FALSE;
   SFL_Ced_Cal_Ptr->k_ced_alert_qualifying_cycles_slow_objects                = 6u;
   SFL_Ced_Cal_Ptr->k_ced_alert_qualifying_cycles                             = 3u;
   SFL_Ced_Cal_Ptr->k_ced_f_object_lat_on_one_side_of_border                  = FBK_FALSE;
   SFL_Ced_Cal_Ptr->k_ced_lat_pos_of_border                                   = 0.0f;
   SFL_Ced_Cal_Ptr->k_ced_f_allow_coasted_object_alerts                       = FBK_FALSE;
   SFL_Ced_Cal_Ptr->k_ced_f_allow_ego_lane_alerts                             = FBK_FALSE;
   SFL_Ced_Cal_Ptr->k_ced_f_handle_both_side_alerts_as_object_side            = FBK_TRUE;
   SFL_Ced_Cal_Ptr->k_ced_f_path_tracking_enable                              = FBK_FALSE;
   SFL_Ced_Cal_Ptr->k_ced_f_suppress_alert_holding_for_obj_below_min_ttp      = FBK_FALSE;
   SFL_Ced_Cal_Ptr->k_ced_f_suppress_alert_holding_for_uncritical_objects     = FBK_TRUE;
   SFL_Ced_Cal_Ptr->k_ced_f_enable_heading_exp_moving_average                 = FBK_TRUE;
   SFL_Ced_Cal_Ptr->k_ced_object_vel_max                                      = 80.0f;
   SFL_Ced_Cal_Ptr->k_ced_object_long_vel_rel_max                             = 79.0f;
   SFL_Ced_Cal_Ptr->k_ced_suppress_range_to_nearest_path_max                  = 3.5f;
   SFL_Ced_Cal_Ptr->k_ced_suppress_pt_heading_diff_ced_alert_max              = 0.35f;
   SFL_Ced_Cal_Ptr->k_ced_ego_lane_parking_maneuver_speed                     = 4.5f;
   SFL_Ced_Cal_Ptr->k_ced_ego_lane_parking_range                              = 20.0f;
   SFL_Ced_Cal_Ptr->k_ced_slow_objects_long_vel_max                           = 0.0f;
   SFL_Ced_Cal_Ptr->k_ced_offset_to_path_weight                               = 1.0f;
   SFL_Ced_Cal_Ptr->k_ced_object_min_dist_to_crash_line_for_path_match        = 10.0f;
   SFL_Ced_Cal_Ptr->k_ced_object_width_safety_margin_for_critical_path_match  = 0.30f;
   SFL_Ced_Cal_Ptr->k_ced_object_width_safety_margin_for_active_alert         = 0.30f;
   SFL_Ced_Cal_Ptr->k_ced_alert_ttp_min[0]                                    = 0.0f;
   SFL_Ced_Cal_Ptr->k_ced_alert_ttp_min[1]                                    = 0.0f;
   SFL_Ced_Cal_Ptr->k_ced_second_warning_pred_lat_dist_max                    = 2.3f;
   SFL_Ced_Cal_Ptr->k_ced_object_heading_exp_moving_average_alpha             = 0.50f;
   SFL_Ced_Cal_Ptr->k_ced_object_max_width_increase_factor_without_path_match = 1.5f;
   SFL_Ced_Cal_Ptr->k_ced_object_max_width_increase_factor_with_path_match    = 0.0f;
   SFL_Ced_Cal_Ptr->k_ced_object_lat_vel_max                                  = 10.0f;
   SFL_Ced_Cal_Ptr->k_ced_object_long_vel_rel_min                             = 0.0f;
   SFL_Ced_Cal_Ptr->k_ced_object_existence_probability_min                    = 0.95f;
   SFL_Ced_Cal_Ptr->k_ced_object_heading_predicted_weight                     = 1.0f;
   SFL_Ced_Cal_Ptr->k_ced_object_acceleration_weight                          = 0.0f;
}

static void SFL_Ced_Cust_Cal_Update_Periodic(Ced_Core_Calibration_T *SFL_Ced_Cal_Ptr) {
   SFL_Vehicle_Output_T *sfl_vehicle_ptr = GetSFLVehiclePtr();

   SFL_Ced_Cal_Ptr->k_ced_f_second_warning_level_enable                    = 1u;
   SFL_Ced_Cal_Ptr->k_ced_collision_zone_width                             = 1.5F;
   SFL_Ced_Cal_Ptr->k_ced_object_long_vel_min                              = SFL_Convert_KMPH_TO_MPS(7.0F);
   SFL_Ced_Cal_Ptr->k_ced_alert_holding_obj_long_vel_min                   = SFL_Convert_KMPH_TO_MPS(7.0F);
   SFL_Ced_Cal_Ptr->k_ced_first_warning_ttc_threshold[FBK_SIDE_REAR]       = 3.8F;
   SFL_Ced_Cal_Ptr->k_ced_second_warning_ttc_threshold[FBK_SIDE_REAR]      = 2.3F;
   SFL_Ced_Cal_Ptr->k_ced_alert_holding_cycles                             = static_cast<uint8_t>(Ml_Roundf(1.0F / SFL_EXECUTION_TIME));
   float32_T temp_nonzero_denominator                                      = Enforce_Nonzero(sfl_vehicle_ptr->host_length, THRESHOLD_IS_ZERO);
   SFL_Ced_Cal_Ptr->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR] = FBK_ONE_F - ((SFL_Convert_CM_TO_M(0)) / temp_nonzero_denominator);
}

static void SFLLcdaCustCalUpdateInit(Lcda_Core_Calibration_T *SFL_Lcda_Cal_Ptr) {
   float32_T temp_nonzero_denominator = 0.0f;

   /* Calibration parameters (configure directly in SW) */
   SFL_Lcda_Cal_Ptr->k_bsw_zone_front_ego_side_x_hys  = CFG_BSW_X0_HYS_LCDA;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_front_ego_side_y_hys  = CFG_BSW_Y0_HYS_LCDA;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_rear_outer_side_x_hys = CFG_BSW_X1_HYS_LCDA;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_rear_outer_side_y_hys = CFG_BSW_Y1_HYS_LCDA;

   temp_nonzero_denominator                                          = Enforce_Nonzero(SFL_Lcda_Cal_Ptr->k_lcda_min_lane_width, THRESHOLD_IS_ZERO);
   SFL_Lcda_Cal_Ptr->k_cvw_zone_y_hys[0]                             = (CFG_LCW_Y0_HYS_LCDA / temp_nonzero_denominator);
   SFL_Lcda_Cal_Ptr->k_cvw_zone_y_hys[1]                             = (CFG_LCW_Y0_HYS_LCDA / temp_nonzero_denominator);
   SFL_Lcda_Cal_Ptr->k_cvw_zone_y_hys[2]                             = (CFG_LCW_Y1_HYS_LCDA / temp_nonzero_denominator);
   SFL_Lcda_Cal_Ptr->k_cvw_zone_y_hys[3]                             = (CFG_LCW_Y1_HYS_LCDA / temp_nonzero_denominator);
   SFL_Lcda_Cal_Ptr->k_cvw_zone_y_hys[4]                             = (CFG_LCW_Y0_HYS_LCDA / temp_nonzero_denominator);
   SFL_Lcda_Cal_Ptr->k_cvw_zone_y_hys[5]                             = (CFG_LCW_Y0_HYS_LCDA / temp_nonzero_denominator);
   SFL_Lcda_Cal_Ptr->k_cvw_ttc_hys                                   = CFG_TTC_TOL_LCDA;
   SFL_Lcda_Cal_Ptr->k_cvw_max_curvi_heading_abs                     = Fbk_Deg_To_Rad(k_cvw_max_curvi_heading_abs_LCDA);
   SFL_Lcda_Cal_Ptr->k_bsw_max_heading_abs                           = Fbk_Deg_To_Rad(k_bsw_max_heading_abs_LCDA);
   SFL_Lcda_Cal_Ptr->k_bsw_suppress_late_warning_max_time_till_leave = CFG_BSW_SOT_MIN_WARN_DURATION_LCDA;

   /* Hardcoded based on customer requirements */
   SFL_Lcda_Cal_Ptr->k_lcda_f_enable_obj_in_ego_lane_check = FBK_TRUE;
   SFL_Lcda_Cal_Ptr->k_bsw_min_obj_long_vel                = SFL_Convert_KMPH_TO_MPS(BSW_CVW_MIN_LONG_SPEED_KMPH);
   SFL_Lcda_Cal_Ptr->k_cvw_min_obj_curvi_long_vel          = SFL_Convert_KMPH_TO_MPS(BSW_CVW_MIN_LONG_SPEED_KMPH);
   SFL_Lcda_Cal_Ptr->k_lcda_zone_check_method              = LCDA_ZONE_CHECK_METHOD_REF_POINT;

   /* Hardcoded same as in baseline project (SWEET400) */
   SFL_Lcda_Cal_Ptr->k_lcda_host_activation_speed_min            = 4.166667f;
   SFL_Lcda_Cal_Ptr->k_lcda_host_activation_speed_min_hys        = 0.28f;
   SFL_Lcda_Cal_Ptr->k_cvw_gap_bridge                            = 0.0f;
   SFL_Lcda_Cal_Ptr->k_cvw_candidate_ttc                         = 5.5f;
   SFL_Lcda_Cal_Ptr->k_lcda_lane_change_intention_vel_lat_thresh = 0.5f;
   SFL_Lcda_Cal_Ptr->k_bsw_fallback_rel_vel_thres                = -5.555556f;
   SFL_Lcda_Cal_Ptr->k_bsw_uses_cvw_alert_state_enabled          = FBK_FALSE;
   SFL_Lcda_Cal_Ptr->k_bsw_alert_track_age                       = 255u;
   SFL_Lcda_Cal_Ptr->k_cvw_alert_holding_cycles                  = 3u;
   SFL_Lcda_Cal_Ptr->k_bsw_alert_holding_cycles                  = 3u;
   SFL_Lcda_Cal_Ptr->k_cvw_min_mature_cycles                     = 3u;
   SFL_Lcda_Cal_Ptr->k_lcda_min_track_age                        = 2u;
   SFL_Lcda_Cal_Ptr->k_bsw_min_mature_cycles                     = 3u;
   SFL_Lcda_Cal_Ptr->k_bsw_enable_trailer_zone_extension         = FBK_FALSE;
   SFL_Lcda_Cal_Ptr->k_bsw_use_curvi_coordinates                 = FBK_TRUE;
   SFL_Lcda_Cal_Ptr->k_bsw_overlap_area_check_enable             = FBK_TRUE;
   SFL_Lcda_Cal_Ptr->k_lcda_ego_lane_check_center_point_only     = FBK_TRUE;
   SFL_Lcda_Cal_Ptr->k_lcda_f_enable_alert_obj_in_ego_lane       = FBK_FALSE;
   SFL_Lcda_Cal_Ptr->k_lka_ov_zone_width                         = 3.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_x0                                    = 1.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_x_length                              = -7.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_y0                                    = 0.5f;
   SFL_Lcda_Cal_Ptr->k_bsw_y_width                               = 3.5f;
   SFL_Lcda_Cal_Ptr->k_cvw_zone_y_hys_min                        = 0.0f;
   SFL_Lcda_Cal_Ptr->k_cvw_zone_y_hys_max                        = 5.0f;
   SFL_Lcda_Cal_Ptr->k_cvw_zone_y[0]                             = 1.5f;
   SFL_Lcda_Cal_Ptr->k_cvw_zone_y[1]                             = 1.4f;
   SFL_Lcda_Cal_Ptr->k_cvw_zone_y[2]                             = 1.4f;
   SFL_Lcda_Cal_Ptr->k_cvw_zone_y[3]                             = 0.6f;
   SFL_Lcda_Cal_Ptr->k_cvw_zone_y[4]                             = 0.6f;
   SFL_Lcda_Cal_Ptr->k_cvw_zone_y[5]                             = 0.5f;
   SFL_Lcda_Cal_Ptr->k_cvw_zone_x[0]                             = -5.0f;
   SFL_Lcda_Cal_Ptr->k_cvw_zone_x[1]                             = -40.0f;
   SFL_Lcda_Cal_Ptr->k_cvw_zone_x[2]                             = -90.0f;
   SFL_Lcda_Cal_Ptr->k_cvw_zone_x[3]                             = -90.0f;
   SFL_Lcda_Cal_Ptr->k_cvw_zone_x[4]                             = -40.0f;
   SFL_Lcda_Cal_Ptr->k_cvw_zone_x[5]                             = -5.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_max_heading_abs                       = 0.785f;
   SFL_Lcda_Cal_Ptr->k_bsw_fallback_rel_vel_thres_hys            = 10.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_overlap_area_threshold                = 0.0f;
   /*SFL_Lcda_Cal_Ptr->k_bsw_zone_y_hys_outer_border[0] = 0.13f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y_hys_outer_border[1] = 0.13f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y_hys_outer_border[2] = 0.13f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y_hys_inner_border[0] = 0.13f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y_hys_inner_border[1] = 0.13f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y_hys_inner_border[2] = 0.13f;*/
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_y_hys[0]                          = 0.4f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_y_hys[1]                          = 0.4f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_y_hys[2]                          = 0.4f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_y_hys[3]                          = -0.4f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_y_hys[4]                          = -0.4f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_y_hys[5]                          = -0.4f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_x_hys[0]                          = 1.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_x_hys[1]                          = 0.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_x_hys[2]                          = -1.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_x_hys[3]                          = -1.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_x_hys[4]                          = 0.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_x_hys[5]                          = 1.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_y[0]                              = 5.4f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_y[1]                              = 5.4f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_y[2]                              = 5.4f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_y[3]                              = 1.75f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_y[4]                              = 1.75f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_y[5]                              = 1.75f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_x[0]                              = 0.8f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_x[1]                              = 0.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_x[2]                              = -3.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_x[3]                              = -3.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_x[4]                              = 0.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_fixed_zone_x[5]                              = 0.8f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y_hys_max                               = 2.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y_hys_min                               = 0.1f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y_hys[0]                                = 0.4f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y_hys[1]                                = 0.4f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y_hys[2]                                = 0.4f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y_hys[3]                                = 0.4f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y_hys[4]                                = 0.4f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y_hys[5]                                = 0.4f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_x_hys[0]                                = 1.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_x_hys[1]                                = 0.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_x_hys[2]                                = -1.0;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_x_hys[3]                                = -1.0;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_x_hys[4]                                = 0.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_x_hys[5]                                = 1.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y[0]                                    = 1.5f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y[1]                                    = 1.5f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y[2]                                    = 1.5f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y[3]                                    = 0.5f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y[4]                                    = 0.5f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_y[5]                                    = 0.5f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_x[0]                                    = 0.8f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_x[1]                                    = 0.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_x[2]                                    = -3.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_x[3]                                    = -3.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_x[4]                                    = 0.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_x[5]                                    = 0.8f;
   SFL_Lcda_Cal_Ptr->k_lcda_ego_lane_effective_lane_width_factor        = 0.95f;
   SFL_Lcda_Cal_Ptr->k_lcda_zone_intersect_critical_point_lateral_ratio = 0.5f;
   SFL_Lcda_Cal_Ptr->k_lcda_distance_traveled_scale_factor              = 0.6f;
   SFL_Lcda_Cal_Ptr->k_lcda_host_activation_speed_max_hys               = 1.0f;
   SFL_Lcda_Cal_Ptr->k_lcda_host_activation_speed_max                   = 100.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_trailer_zone_ext_safety_margin               = 0.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_lane_change_intention_pos_long_thres         = 5.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_lane_change_intention_pos_lat_thres          = 0.3f;

   /* Not available in SWEET400. Hardcoded same as SFL Generic(24_PI2_S3)*/
   SFL_Lcda_Cal_Ptr->k_bsw_obj_max_rel_vel_hys                          = 2.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_obj_max_rel_vel_thresh                       = 55.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_trailer_zone_ext_safety_margin_hys           = 0.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_max_obj_long_vel_hysteresis                  = 0.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_min_obj_long_vel_hysteresis                  = 0.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_max_obj_long_vel                             = 100.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_max_heading_abs_hysteresis                   = 0.0f;
   SFL_Lcda_Cal_Ptr->k_bsw_line_to_stop_TOS_alert                       = -1.30f;
   SFL_Lcda_Cal_Ptr->k_bsw_n_line_position_for_long_object_sot_scenario = -100.0f;
}

static void SFLLcdaCustCalUpdatePeriodic(Lcda_Core_Calibration_T *SFL_Lcda_Cal_Ptr) {
   SFL_Vehicle_Output_T *sfl_vehicle_ptr = GetSFLVehiclePtr();
   float host_length                     = sfl_vehicle_ptr->host_length;
   float host_half_width                 = sfl_vehicle_ptr->host_width / 2;

   SFL_Lcda_Cal_Ptr->k_bsw_zone_front_ego_side_x  = -2.66F;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_front_ego_side_y  = 0.5F + host_half_width;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_rear_outer_side_x = -3.0F - host_length;
   SFL_Lcda_Cal_Ptr->k_bsw_zone_rear_outer_side_y = 3.0F + host_half_width;
   SFL_Lcda_Cal_Ptr->k_cvw_x0                     = CFG_LCW_X0_LCDA - host_length;
   SFL_Lcda_Cal_Ptr->k_cvw_x_length0              = -70.0F;
   SFL_Lcda_Cal_Ptr->k_cvw_x_length1              = -30.0F;
   SFL_Lcda_Cal_Ptr->k_cvw_y0                     = (0.5F) + host_half_width;
   SFL_Lcda_Cal_Ptr->k_cvw_y1                     = (3.0F) + host_half_width;
   SFL_Lcda_Cal_Ptr->k_cvw_y_width0               = (2.5F);
   SFL_Lcda_Cal_Ptr->k_cvw_y_width1               = (2.5F);
   SFL_Lcda_Cal_Ptr->k_bsw_enable_dynspeed_zone   = 0U;
   SFL_Lcda_Cal_Ptr->k_bsw_dynzone_speed[0]       = 0.0F;
   SFL_Lcda_Cal_Ptr->k_bsw_dynzone_speed[1]       = 8.3F;
   SFL_Lcda_Cal_Ptr->k_bsw_dynzone_speed[2]       = 11.1F;
   SFL_Lcda_Cal_Ptr->k_bsw_dynzone_speed[3]       = 22.2F;
   SFL_Lcda_Cal_Ptr->k_bsw_dynzone_speed[4]       = 55.0F;
   SFL_Lcda_Cal_Ptr->k_bsw_dynzone_speed[5]       = 69.4F;
   SFL_Lcda_Cal_Ptr->k_bsw_dynzone_range[0]       = -FBK_ONE_F * (-3.0F);
   SFL_Lcda_Cal_Ptr->k_bsw_dynzone_range[1]       = -FBK_ONE_F * (-3.0F);
   SFL_Lcda_Cal_Ptr->k_bsw_dynzone_range[2]       = -FBK_ONE_F * (0.0F);
   SFL_Lcda_Cal_Ptr->k_bsw_dynzone_range[3]       = -FBK_ONE_F * (0.0F);
   SFL_Lcda_Cal_Ptr->k_bsw_dynzone_range[4]       = -FBK_ONE_F * (10.0F);
   SFL_Lcda_Cal_Ptr->k_bsw_dynzone_range[5]       = -FBK_ONE_F * (10.0F);
   SFL_Lcda_Cal_Ptr->k_cvw_ttc                    = 3.0F;

   SFL_Lcda_Cal_Ptr->k_slc_zone_x[0] = 0.0F;
   SFL_Lcda_Cal_Ptr->k_slc_zone_x[1] = -40.0F;
   SFL_Lcda_Cal_Ptr->k_slc_zone_x[2] = -40.0F;
   SFL_Lcda_Cal_Ptr->k_slc_zone_x[3] = -40.0F;
   SFL_Lcda_Cal_Ptr->k_slc_zone_x[4] = -40.0F;
   SFL_Lcda_Cal_Ptr->k_slc_zone_x[5] = 0.0F;

   SFL_Lcda_Cal_Ptr->k_slc_zone_y[0] = 8.0F; // 0.5 is getting subtracted in lcda code
   SFL_Lcda_Cal_Ptr->k_slc_zone_y[1] = 4.0F;
   SFL_Lcda_Cal_Ptr->k_slc_zone_y[2] = 3.0F + host_half_width;
   SFL_Lcda_Cal_Ptr->k_slc_zone_y[3] = 0.5F + host_half_width;
   SFL_Lcda_Cal_Ptr->k_slc_zone_y[4] = host_half_width;
   SFL_Lcda_Cal_Ptr->k_slc_zone_y[5] = host_half_width;

   SFL_Lcda_Cal_Ptr->k_lcda_min_lane_width = 1.0F;
   SFL_Lcda_Cal_Ptr->k_lcda_max_lane_width = 10.0F;
}

static void SFLCedInit(Ced_Instance_T *SFL_Ced_Instance_Ptr) {
   if (nullptr != SFL_Ced_Instance_Ptr) {
      (void)Ced_Init_Platform(SFL_Ced_Instance_Ptr);

      //(void)SFLCedCustCalUpdateInit(&SFL_Ced_Instance_Ptr->calibration);
   }
}

static void SFLLcdaInit(Lcda_Instance_T *SFL_Lcda_Instance_Ptr) {
   if (nullptr != SFL_Lcda_Instance_Ptr) {
      (void)Lcda_Init_Platform(SFL_Lcda_Instance_Ptr);

      //(void)SFLLcdaCustCalUpdateInit(&SFL_Lcda_Instance_Ptr->calibration);
   }
}

Fbk_Output_T *FeatureFunctionGetOutputPtr() {
   return &SFL_Fbk_Output;
}

static void SFLFbkRun(Fbk_Instance_T *SFL_Fbk_Instance_Ptr, Fbk_Output_T *SFL_Fbk_Output_Ptr, Pa_Context_T *SFL_Pa_Context_Ptr) {
   SFL_Vehicle_Output_T *sfl_vehicle_ptr = GetSFLVehiclePtr();
   SFL_Olp_Objects_Log_T *sfl_object_ptr = GetSFLObjectPtr();

   if ((nullptr != SFL_Fbk_Instance_Ptr) && (nullptr != SFL_Fbk_Output_Ptr) && (nullptr != SFL_Pa_Context_Ptr)) {
      SFL_Pa_Context_Ptr->p_data->time_diff_to_last_cycle = 0.05F; // hardcoded
      /*Populate vehicle and object data to SFL*/
      memcpy(&SFL_Pa_Context_Ptr->p_data->vehicle_data, sfl_vehicle_ptr, sizeof(SFL_Vehicle_Output_T));
      memcpy(&SFL_Pa_Context_Ptr->p_data->object_data[0], &sfl_object_ptr->obj[0], PA_OBJ_NUMBER_OF_OBJECTS * sizeof(SFL_Olp_Extended_Objects_T));
      (void)Fbk_Run_Platform(SFL_Fbk_Instance_Ptr, SFL_Fbk_Output_Ptr, SFL_Pa_Context_Ptr);
   }
}

static void SFLPtRun(Pt_Instance_T *SFL_Pt_Instance_Ptr, Pt_Output_T *SFL_Pt_Output_Ptr, const Fbk_Output_T *SFL_Fbk_Output_Ptr) {
   if ((nullptr != SFL_Pt_Instance_Ptr) && (nullptr != SFL_Pt_Output_Ptr) && (nullptr != SFL_Fbk_Output_Ptr)) {
      (void)Pt_Run_Platform(SFL_Pt_Instance_Ptr, SFL_Pt_Output_Ptr, SFL_Fbk_Output_Ptr);
   }
}

static void SFLCtaMain(Cta_Input_T *SFL_Cta_Input_Ptr, const Fbk_Output_T *SFL_Fbk_Output_Ptr,
                       const Pt_Output_T *SFL_Pt_Output_Ptr, Cta_Instance_T *SFL_Cta_Instance_Ptr,
                       Cta_Output_T *SFL_Cta_Output_Ptr, const Pa_Context_T *SFL_Pa_Context_Ptr) {
   SFL_Vehicle_Output_T *sfl_vehicle_ptr = GetSFLVehiclePtr();
   if ((nullptr != SFL_Cta_Input_Ptr) && (nullptr != SFL_Fbk_Output_Ptr) &&
       (nullptr != SFL_Pt_Output_Ptr) && (nullptr != SFL_Cta_Instance_Ptr) && (nullptr != SFL_Cta_Output_Ptr) && (nullptr != SFL_Pa_Context_Ptr)) {
      SFL_Cta_Input_Ptr->f_cta_enable = FBK_TRUE;
      // SFL_Cta_Input_Ptr->f_rear_cta_enable = sfl_vehicle_ptr->host_speed < 0 ? FBK_TRUE : FBK_FALSE;
      // SFL_Cta_Input_Ptr->f_front_cta_enable = sfl_vehicle_ptr->host_speed > 0 ? FBK_TRUE : FBK_FALSE;
      SFL_Cta_Input_Ptr->f_rear_cta_enable  = FBK_TRUE;
      SFL_Cta_Input_Ptr->f_front_cta_enable = FBK_TRUE;

      //(void)SFLCtaCust_CalUpdatePeriodic(&SFL_Cta_Instance_Ptr->calibration);
      (void)Cta_Run_Platform(SFL_Cta_Input_Ptr, SFL_Fbk_Output_Ptr, SFL_Pt_Output_Ptr, SFL_Cta_Instance_Ptr, SFL_Cta_Output_Ptr);
      //	memcpy(Cta_Get_Output_Ptr(), SFL_Cta_Output_Ptr, sizeof(Cta_Output_T));
   }
}

static void SFLCedMain(Ced_Instance_T *SFL_Ced_Instance_Ptr, Ced_Input_T *SFL_Ced_Input_Ptr,
                       Ced_Output_T *SFL_Ced_Output_Ptr, const Fbk_Output_T *SFL_Fbk_Output_Ptr, const Pt_Output_T *SFL_Pt_Output_Ptr) {
   SFL_Vehicle_Output_T *sfl_vehicle_ptr = GetSFLVehiclePtr();
   if ((nullptr != SFL_Ced_Instance_Ptr) && (nullptr != SFL_Ced_Input_Ptr) &&
       (nullptr != SFL_Ced_Output_Ptr) && (nullptr != SFL_Fbk_Output_Ptr) && (nullptr != SFL_Pt_Output_Ptr)) {
      SFL_Ced_Input_Ptr->f_ced_enable     = FBK_TRUE;
      SFL_Ced_Input_Ptr->f_ced_rear_mode  = FBK_TRUE;
      SFL_Ced_Input_Ptr->f_ced_front_mode = FBK_TRUE;
      //(void)SFL_Ced_Cust_Cal_Update_Periodic(&SFL_Ced_Instance_Ptr->calibration);
      (void)Ced_Run_Platform(SFL_Ced_Instance_Ptr, SFL_Ced_Input_Ptr, SFL_Ced_Output_Ptr, SFL_Fbk_Output_Ptr, SFL_Pt_Output_Ptr);
      // memcpy(Ced_Get_Output_Ptr(), SFL_Ced_Output_Ptr, sizeof(Ced_Output_T));
   }
}

static void SFLLcdaMain(Lcda_Instance_T *SFL_Lcda_Instance_Ptr, Lcda_Input_T *SFL_Lcda_Input_Ptr,
                        const Fbk_Output_T *SFL_Fbk_Output_Ptr, Lcda_Output_T *SFL_Lcda_Output_Ptr) {
   boolean_T bsw_operational             = FBK_TRUE;
   SFL_Vehicle_Output_T *sfl_vehicle_ptr = GetSFLVehiclePtr();
   if ((nullptr != SFL_Lcda_Instance_Ptr) && (nullptr != SFL_Lcda_Input_Ptr) &&
       (nullptr != SFL_Fbk_Output_Ptr) && (nullptr != SFL_Lcda_Output_Ptr)) {
      // bsw_operational = (sfl_vehicle_ptr->prndl == SFL_VEH_PRNDL_STATE_DRIVE) ? FBK_TRUE : FBK_FALSE;
      SFL_Lcda_Input_Ptr->f_lcda_enable     = bsw_operational;
      SFL_Lcda_Input_Ptr->f_bsw_enable      = bsw_operational;
      SFL_Lcda_Input_Ptr->f_cvw_enable      = bsw_operational;
      SFL_Lcda_Input_Ptr->f_slc_enable      = bsw_operational;
      SFL_Lcda_Input_Ptr->f_elc_enable      = bsw_operational;
      SFL_Lcda_Input_Ptr->f_dropback_enable = FBK_FALSE;
      SFL_Lcda_Input_Ptr->f_fallback_enable = FBK_FALSE;
      //(void)SFLLcdaCustCalUpdatePeriodic(&SFL_Lcda_Instance_Ptr->calibration);
      (void)Lcda_Run_Platform(SFL_Lcda_Instance_Ptr, SFL_Lcda_Input_Ptr, SFL_Fbk_Output_Ptr, SFL_Lcda_Output_Ptr);
      // memcpy(Lcda_Get_Output_Ptr(), SFL_Lcda_Output_Ptr, sizeof(Lcda_Output_T));
   }
}

void InitFeatureFunction() {
   /*SFL Feature building kit init*/
   (void)SFLFbkInit(&SFL_Pa_Context, &SFL_Fbk_Instance, SFL_Pa_Data_GPtr);
   /*SFL Feature Functions init*/
   (void)SFLPtInit(&SFL_Pt_Instance);
   (void)SFLCtaInit(&SFL_Cta_Instance);
   (void)SFLCedInit(&SFL_Ced_Instance);
   (void)SFLLcdaInit(&SFL_Lcda_Instance);
   (void)SFLRecwInit(&SFL_Recw_Instance);
   (void)SFLScwInit(&SFL_Scw_Instance);
   (void)SFLTaInit(&SFL_Ta_Instance);
   (void)SFLLtbInit(&SFL_Ltb_Instance);
   (void)SFLEsaInit(&SFL_Esa_Instance);
}

void RunFeatureFunction() {
   /*SFL Feature building kit periodic execution*/
   (void)SFLFbkRun(&SFL_Fbk_Instance, &SFL_Fbk_Output, &SFL_Pa_Context);

   /*SFL Feature Functions periodic execution*/
   (void)SFLPtRun(&SFL_Pt_Instance, SFL_Pt_Output_GPtr, &SFL_Fbk_Output);
   (void)SFLCtaMain(&SFL_Cta_Input, &SFL_Fbk_Output, SFL_Pt_Output_GPtr, &SFL_Cta_Instance, &SFL_Cta_Output, &SFL_Pa_Context);
   (void)SFLCedMain(&SFL_Ced_Instance, &SFL_Ced_Input, &SFL_Ced_Output, &SFL_Fbk_Output, SFL_Pt_Output_GPtr);
   (void)SFLLcdaMain(&SFL_Lcda_Instance, &SFL_Lcda_Input, &SFL_Fbk_Output, &SFL_Lcda_Output);
   (void)SFLRecwMain(&SFL_Recw_Instance, &SFL_Recw_Input, &SFL_Fbk_Output, &SFL_Recw_Output);
   (void)SFLScwMain(&SFL_Scw_Instance, &SFL_Scw_Input, &SFL_Fbk_Output, &SFL_Scw_Output);
   (void)SFLTaMain(&SFL_Ta_Instance, &SFL_Ta_Input, &SFL_Fbk_Output, &SFL_Ta_Output);
   (void)SFLLtbMain(&SFL_Ltb_Instance, &SFL_Ltb_Input, &SFL_Fbk_Output, &SFL_Ltb_Output);
   (void)SFLEsaMain(&SFL_Esa_Instance, &SFL_Esa_Input, &SFL_Fbk_Output, &SFL_Esa_Output);
}

static void SFLRecwCustCalUpdateInit(Recw_Core_Calibration_T *SFL_Recw_Cal_Ptr) {
}

static void SFLRecwCustCalUpdatePeriodic(Recw_Core_Calibration_T *SFL_Recw_Cal_Ptr) {
}

static void SFLRecwInit(Recw_Instance_T *SFL_Recw_Instance_Ptr) {
   if (nullptr != SFL_Recw_Instance_Ptr) {
      (void)Recw_Init_Platform(SFL_Recw_Instance_Ptr);

      //(void)SFLRecwCustCalUpdateInit(&SFL_Recw_Instance_Ptr->calibration);
   }
}

static void SFLRecwMain(Recw_Instance_T *SFL_Recw_Instance_Ptr, Recw_Input_T *SFL_Recw_Input_Ptr,
                        const Fbk_Output_T *SFL_Fbk_Output_Ptr, Recw_Output_T *SFL_Recw_Output_Ptr) {
   boolean_T operational                 = FBK_TRUE;
   SFL_Vehicle_Output_T *sfl_vehicle_ptr = GetSFLVehiclePtr();
   // operational = (sfl_vehicle_ptr->prndl == SFL_VEH_PRNDL_STATE_DRIVE) ? FBK_TRUE : FBK_FALSE;

   if ((nullptr != SFL_Recw_Instance_Ptr) && (nullptr != SFL_Recw_Input_Ptr) &&
       (nullptr != SFL_Fbk_Output_Ptr) && (nullptr != SFL_Recw_Output_Ptr)) {
      SFL_Recw_Input_Ptr->f_recw_enable = operational;
      //	(void)SFLRecwCustCalUpdatePeriodic(&SFL_Recw_Instance_Ptr->calibration);
      (void)Recw_Run_Platform(SFL_Recw_Instance_Ptr, SFL_Recw_Input_Ptr, SFL_Fbk_Output_Ptr, SFL_Recw_Output_Ptr);
      //	memcpy(Recw_Get_Output_Ptr(), SFL_Recw_Output_Ptr, sizeof(Recw_Output_T));
   }
}

static void SFLScwCustCalUpdateInit(Scw_Core_Calibration_T *SFL_Scw_Cal_Ptr) {
}

static void SFLScwCustCalUpdatePeriodic(Scw_Core_Calibration_T *SFL_Scw_Cal_Ptr) {
   SFL_Vehicle_Output_T *sfl_vehicle_ptr = GetSFLVehiclePtr();
   float host_length                     = sfl_vehicle_ptr->host_length;
   float host_half_width                 = sfl_vehicle_ptr->host_width / 2;
}

static void SFLScwInit(Scw_Instance_T *SFL_Scw_Instance_Ptr) {
   if (nullptr != SFL_Scw_Instance_Ptr) {
      (void)Scw_Init_Platform(SFL_Scw_Instance_Ptr);

      //(void)SFLScwCustCalUpdateInit(&SFL_Scw_Instance_Ptr->calibration);
   }
}

static void SFLScwMain(Scw_Instance_T *SFL_Scw_Instance_Ptr, Scw_Input_T *SFL_Scw_Input_Ptr,
                       const Fbk_Output_T *SFL_Fbk_Output_Ptr, Scw_Output_T *SFL_Scw_Output_Ptr) {
   boolean_T operational                 = FBK_TRUE;
   SFL_Vehicle_Output_T *sfl_vehicle_ptr = GetSFLVehiclePtr();
   if ((nullptr != SFL_Scw_Instance_Ptr) && (nullptr != SFL_Scw_Input_Ptr) &&
       (nullptr != SFL_Fbk_Output_Ptr) && (nullptr != SFL_Scw_Output_Ptr)) {
      // operational = (sfl_vehicle_ptr->prndl == SFL_VEH_PRNDL_STATE_DRIVE) ? FBK_TRUE : FBK_FALSE;
      SFL_Scw_Input_Ptr->f_scw_enable           = operational;
      SFL_Scw_Input_Ptr->f_scw_enable_dynamic   = operational;
      SFL_Scw_Input_Ptr->f_scw_enable_guardrail = operational;
      // SFL_Scw_Input_Ptr->f_trailer_present
      // SFL_Scw_Input_Ptr->trailer_angle =
      // SFL_Scw_Input_Ptr->trailer_length
      // SFL_Scw_Input_Ptr->trailer_width

      //	(void)SFLScwCustCalUpdatePeriodic(&SFL_Scw_Instance_Ptr->calibration);
      (void)Scw_Run_Platform(SFL_Scw_Instance_Ptr, SFL_Scw_Input_Ptr, SFL_Fbk_Output_Ptr, SFL_Scw_Output_Ptr);
      //	memcpy(Scw_Get_Output_Ptr(), SFL_Scw_Output_Ptr, sizeof(Scw_Output_T));
   }
}

static void SFLTaCustCalUpdateInit(Ta_Core_Calibration_T *SFL_ta_Cal_Ptr) {
}

static void SFLTaCustCalUpdatePeriodic(Ta_Core_Calibration_T *SFL_ta_Cal_Ptr) {
   SFL_Vehicle_Output_T *sfl_vehicle_ptr = GetSFLVehiclePtr();
   float host_length                     = sfl_vehicle_ptr->host_length;
   float host_half_width                 = sfl_vehicle_ptr->host_width / 2;
}

static void SFLTaInit(Ta_Instance_T *SFL_ta_Instance_Ptr) {
   if (nullptr != SFL_ta_Instance_Ptr) {
      (void)Ta_Init_Platform(SFL_ta_Instance_Ptr);

      //(void)SFLTaCustCalUpdateInit(&SFL_ta_Instance_Ptr->calibration);
   }
}

static void SFLTaMain(Ta_Instance_T *SFL_ta_Instance_Ptr, Ta_Input_T *SFL_ta_Input_Ptr,
                      const Fbk_Output_T *SFL_Fbk_Output_Ptr, Ta_Output_T *SFL_ta_Output_Ptr) {
   boolean_T bsw_operational             = FBK_FALSE;
   SFL_Vehicle_Output_T *sfl_vehicle_ptr = GetSFLVehiclePtr();
   if ((nullptr != SFL_ta_Instance_Ptr) && (nullptr != SFL_ta_Input_Ptr) &&
       (nullptr != SFL_Fbk_Output_Ptr) && (nullptr != SFL_ta_Output_Ptr)) {
      // bsw_operational = (sfl_vehicle_ptr->prndl == SFL_VEH_PRNDL_STATE_DRIVE) ? FBK_TRUE : FBK_FALSE;
      SFL_ta_Input_Ptr->f_ta_enable  = bsw_operational;
      SFL_ta_Input_Ptr->f_fta_enable = bsw_operational;
      SFL_ta_Input_Ptr->f_rta_enable = bsw_operational;
      //	(void)SFLTaCustCalUpdatePeriodic(&SFL_ta_Instance_Ptr->calibration);
      (void)Ta_Run_Platform(SFL_ta_Instance_Ptr, SFL_ta_Input_Ptr, SFL_Fbk_Output_Ptr, SFL_ta_Output_Ptr);
      // memcpy(Ta_Get_Output_Ptr(), SFL_ta_Output_Ptr, sizeof(Ta_Output_T));
   }
}

static void SFLLtbCustCalUpdateInit(Ltb_Core_Calibration_T *SFL_Ltb_Cal_Ptr) {
}

static void SFLLtbCustCalUpdatePeriodic(Ltb_Core_Calibration_T *SFL_Ltb_Cal_Ptr) {
   SFL_Vehicle_Output_T *sfl_vehicle_ptr = GetSFLVehiclePtr();
   float host_length                     = sfl_vehicle_ptr->host_length;
   float host_half_width                 = sfl_vehicle_ptr->host_width / 2;
}

static void SFLLtbInit(Ltb_Instance_T *SFL_Ltb_Instance_Ptr) {
   if (nullptr != SFL_Ltb_Instance_Ptr) {
      (void)Ltb_Init_Platform(SFL_Ltb_Instance_Ptr);

      //	(void)SFLLtbCustCalUpdateInit(&SFL_Ltb_Instance_Ptr->calibration);
   }
}

static void SFLLtbMain(Ltb_Instance_T *SFL_Ltb_Instance_Ptr, Ltb_Input_T *SFL_Ltb_Input_Ptr,
                       const Fbk_Output_T *SFL_Fbk_Output_Ptr, Ltb_Output_T *SFL_Ltb_Output_Ptr) {
   boolean_T bsw_operational             = FBK_TRUE;
   SFL_Vehicle_Output_T *sfl_vehicle_ptr = GetSFLVehiclePtr();
   if ((nullptr != SFL_Ltb_Instance_Ptr) && (nullptr != SFL_Ltb_Input_Ptr) &&
       (nullptr != SFL_Fbk_Output_Ptr) && (nullptr != SFL_Ltb_Output_Ptr)) {
      // bsw_operational = (sfl_vehicle_ptr->prndl == SFL_VEH_PRNDL_STATE_DRIVE) ? FBK_TRUE : FBK_FALSE;
      SFL_Ltb_Input_Ptr->f_ltb_enable = bsw_operational;
      //	(void)SFLLtbCustCalUpdatePeriodic(&SFL_Ltb_Instance_Ptr->calibration);
      (void)Ltb_Run_Platform(SFL_Ltb_Instance_Ptr, SFL_Ltb_Input_Ptr, SFL_Fbk_Output_Ptr, SFL_Ltb_Output_Ptr);
      // memcpy(Ltb_Get_Output_Ptr(), SFL_Ltb_Output_Ptr, sizeof(Ltb_Output_T));
   }
}

static void SFL_Esa_Cust_Cal_Update_Init(Esa_Core_Calibration_T *SFL_Esa_Cal_Ptr) {
}

static void SFL_Esa_Cust_Cal_Update_Periodic(Esa_Core_Calibration_T *SFL_Esa_Cal_Ptr) {
}

static void SFLEsaInit(Esa_Instance_T *SFL_Esa_Instance_Ptr) {
   if (nullptr != SFL_Esa_Instance_Ptr) {
      (void)Esa_Init_Platform(SFL_Esa_Instance_Ptr);

      //(void)SFL_Esa_Cust_Cal_Update_Init(&SFL_Esa_Instance_Ptr->calibration);
   }
}

static void SFLEsaMain(Esa_Instance_T *SFL_Esa_Instance_Ptr, Esa_Input_T *SFL_Esa_Input_Ptr,
                       const Fbk_Output_T *SFL_Fbk_Output_Ptr, Esa_Output_T *SFL_Esa_Output_Ptr) {
   boolean_T bsw_operational             = FBK_TRUE;
   SFL_Vehicle_Output_T *sfl_vehicle_ptr = GetSFLVehiclePtr();
   if ((nullptr != SFL_Esa_Instance_Ptr) && (nullptr != SFL_Esa_Input_Ptr) &&
       (nullptr != SFL_Fbk_Output_Ptr) && (nullptr != SFL_Esa_Output_Ptr)) {
      // bsw_operational = (sfl_vehicle_ptr->prndl == SFL_VEH_PRNDL_STATE_DRIVE) ? FBK_TRUE : FBK_FALSE;
      SFL_Esa_Input_Ptr->f_esa_enabled = bsw_operational;
      //(void)SFL_Esa_Cust_Cal_Update_Periodic(&SFL_Esa_Instance_Ptr->calibration);
      (void)Esa_Run_Platform(SFL_Esa_Instance_Ptr, SFL_Esa_Input_Ptr, SFL_Fbk_Output_Ptr, SFL_Esa_Output_Ptr);
      // memcpy(Esa_Get_Output_Ptr(), SFL_Esa_Output_Ptr, sizeof(Esa_Output_T));
   }
}
