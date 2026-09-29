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
   float32_T k_stla_crit_zone_Q_QH_line; /**<Distance between STLA CTA critical zones borders QH and Q   - second level behind rear bumper*/
   float32_T k_stla_crit_zone_N_Q_line; /**<Distance between STLA CTA critical zones borders N and Q  - first level behind rear bumper*/
   float32_T k_stla_crit_zone_D_C_line; /**<Distance between STLA CTA critical zones borders D and C - driver longitudinal position */
   float32_T k_stla_crit_zone_G_E_line; /**<Distance between STLA CTA critical zones borders G(L) and E(J) - outer lateral borders */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Cta_Customer_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_stla_crit_zone_G_E_line; /**<Distance between STLA CTA critical zones borders G(L) and E(J) - outer lateral borders */
   float32_T k_stla_crit_zone_D_C_line; /**<Distance between STLA CTA critical zones borders D and C - driver longitudinal position */
   float32_T k_stla_crit_zone_N_Q_line; /**<Distance between STLA CTA critical zones borders N and Q  - first level behind rear bumper*/
   float32_T k_stla_crit_zone_Q_QH_line; /**<Distance between STLA CTA critical zones borders QH and Q   - second level behind rear bumper*/
} Cta_Customer_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* CTA_CUSTOMER_CALIBRATION_T_H */
