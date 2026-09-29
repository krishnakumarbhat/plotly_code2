# ifndef CED_CUSTOMER_CALIBRATION_T_H
# define CED_CUSTOMER_CALIBRATION_T_H

/**
* @file ced_customer_calibration_t.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in ced_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "ct_calibration_header_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for all calibrations */
/* Macros for array sizes for all array variables */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_THRESHOLD_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_HYSTERESIS_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_HONDA_SRR6_LONG_DIST_THRESHOLD_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_HONDA_ELATCH_ZONES_WIDTH_TABLE_ARRAY_SIZE_DIM0 (2u)

/* Macros for dimension size for all array variables */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_THRESHOLD_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_HYSTERESIS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_CED_HONDA_SRR6_LONG_DIST_THRESHOLD_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_K_HONDA_ELATCH_ZONES_WIDTH_TABLE_ARRAY_DIM_SIZE (1u)


/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_CUSTOMER_CALIBRATION_SIZE (60u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
   uint8_t k_unused_padding_byte_2; /**<Padded byte for byte packing of 4*/
   uint8_t k_unused_padding_byte_1; /**<Padded byte for byte packing of 4*/
   uint8_t k_unused_padding_byte_0; /**<Padded byte for byte packing of 4*/
   boolean_T k_ced_f_honda_use_alert_ttc_threshold; /**<Flag whether there should be ttc threshold check performed in post run*/
   float32_T k_ced_honda_object_acceleration_weight; /**<Weights the influence of the objects acceleration on the prediction and TTC calculation.*/
   float32_T k_honda_min_eratch_alert_duration; /**<The minimum duration of the eratch alert in Honda*/
   float32_T k_honda_elatch_zones_width_table[CED_K_HONDA_ELATCH_ZONES_WIDTH_TABLE_ARRAY_SIZE_DIM0]; /**<Honda table with door elatch zones width for EW_Zone_Information_T ew_elatch_sense_stt   */
   float32_T k_honda_min_alert_duration; /**<The minimum duration of the alert.*/
   float32_T k_ced_honda_srr6_long_dist_threshold[CED_K_CED_HONDA_SRR6_LONG_DIST_THRESHOLD_ARRAY_SIZE_DIM0]; /**<This parameter defines the longitudinal distance from host rear bumper to object reference point threshold for keeping alert active. [LEFT RIGHT]*/
   float32_T k_ced_honda_srr6_custom_ttc_alert_hysteresis[CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_HYSTERESIS_ARRAY_SIZE_DIM0]; /**<This parameter defines the custom TTC hysteresis for keeping alert active. [LEFT RIGHT]*/
   float32_T k_ced_honda_srr6_custom_ttc_alert_threshold[CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_THRESHOLD_ARRAY_SIZE_DIM0]; /**<This parameter defines the custom TTC threshold for keeping alert active. [LEFT RIGHT]*/
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Ced_Customer_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_ced_honda_srr6_custom_ttc_alert_threshold[CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_THRESHOLD_ARRAY_SIZE_DIM0]; /**<This parameter defines the custom TTC threshold for keeping alert active. [LEFT RIGHT]*/
   float32_T k_ced_honda_srr6_custom_ttc_alert_hysteresis[CED_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_HYSTERESIS_ARRAY_SIZE_DIM0]; /**<This parameter defines the custom TTC hysteresis for keeping alert active. [LEFT RIGHT]*/
   float32_T k_ced_honda_srr6_long_dist_threshold[CED_K_CED_HONDA_SRR6_LONG_DIST_THRESHOLD_ARRAY_SIZE_DIM0]; /**<This parameter defines the longitudinal distance from host rear bumper to object reference point threshold for keeping alert active. [LEFT RIGHT]*/
   float32_T k_honda_min_alert_duration; /**<The minimum duration of the alert.*/
   float32_T k_honda_elatch_zones_width_table[CED_K_HONDA_ELATCH_ZONES_WIDTH_TABLE_ARRAY_SIZE_DIM0]; /**<Honda table with door elatch zones width for EW_Zone_Information_T ew_elatch_sense_stt   */
   float32_T k_honda_min_eratch_alert_duration; /**<The minimum duration of the eratch alert in Honda*/
   float32_T k_ced_honda_object_acceleration_weight; /**<Weights the influence of the objects acceleration on the prediction and TTC calculation.*/
   boolean_T k_ced_f_honda_use_alert_ttc_threshold; /**<Flag whether there should be ttc threshold check performed in post run*/
   uint8_t k_unused_padding_byte_0; /**<Padded byte for byte packing of 4*/
   uint8_t k_unused_padding_byte_1; /**<Padded byte for byte packing of 4*/
   uint8_t k_unused_padding_byte_2; /**<Padded byte for byte packing of 4*/
} Ced_Customer_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* CED_CUSTOMER_CALIBRATION_T_H */
