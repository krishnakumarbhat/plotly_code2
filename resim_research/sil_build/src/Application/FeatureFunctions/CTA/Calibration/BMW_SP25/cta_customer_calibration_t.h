# ifndef CTA_CUSTOMER_CALIBRATION_T_H
# define CTA_CUSTOMER_CALIBRATION_T_H

/**
* @file cta_customer_calibration_t.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in cta_cal.xml.
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

/* Macros for dimension size for all array variables */


/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_CUSTOMER_CALIBRATION_SIZE (28u)

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
   boolean_T k_bmw_sp25_banner_criteria_check; /**<Banner Criteria Check for CTB Activation*/
   float32_T k_bmw_sp25_banner_time; /**<Banner Time for Braking State*/
   float32_T k_bmw_sp25_banner_criteria_check_time; /**<Banner Criteria Check Time*/
   float32_T k_bmw_sp25_ego_abs_speed_max_hys; /**<Hysteresis for ego speed max for activation*/
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Cta_Customer_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_bmw_sp25_ego_abs_speed_max_hys; /**<Hysteresis for ego speed max for activation*/
   float32_T k_bmw_sp25_banner_criteria_check_time; /**<Banner Criteria Check Time*/
   float32_T k_bmw_sp25_banner_time; /**<Banner Time for Braking State*/
   boolean_T k_bmw_sp25_banner_criteria_check; /**<Banner Criteria Check for CTB Activation*/
   uint8_t k_unused_padding_byte_0; /**<Padded byte for byte packing of 4*/
   uint8_t k_unused_padding_byte_1; /**<Padded byte for byte packing of 4*/
   uint8_t k_unused_padding_byte_2; /**<Padded byte for byte packing of 4*/
} Cta_Customer_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* CTA_CUSTOMER_CALIBRATION_T_H */
