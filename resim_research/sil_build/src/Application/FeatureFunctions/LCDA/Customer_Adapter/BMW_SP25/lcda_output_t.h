/**
 * @file lcda_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the BMW SRR5 output data structure for LCDA.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#ifndef LCDA_OUTPUT_T_H
#define LCDA_OUTPUT_T_H

#include "fbk_macros.h"
#include "lcda_bmw_sp25_types.h"
#include "pa_reuse.h"
/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief BMW SRR5 specific LCDA output structure.
 *
 * @SRS{}
 * @SAE{SF-2783}
 * @SDD{SF-6860}
 */
typedef struct
{
   uint8_t f_lcda_enabled; /* Flag indicating if LCDA is enabled */
   uint8_t f_bsw_enabled;  /* Flag indicating that the BSW subfunction is enabled */
   uint8_t f_cvw_enabled;  /* Flag indicating that the CVW subfunction is enabled */
   uint8_t f_slc_enabled;  /* Flag indicating that the SLC subfunction is enabled */
   uint8_t f_awa_enabled;  /* Flag indicating that the AWA subfunction is enabled */

   /* output for orcas */
   uint8_t bsw_alert[FBK_NUMBER_OF_SIDES]; /* flag indicating if there is a critical BSW object for left/right side */
   uint8_t bsw_id[FBK_NUMBER_OF_SIDES];    /* object ID of critical BSW object on the left/right side if present (0 for none)*/
   uint32_t bsw_unique_id[FBK_NUMBER_OF_SIDES]; /* object Unique ID of critical BSW object on the left/right side if present (0 for
                                                   none)*/
   float32_T bsw_ttp[FBK_NUMBER_OF_SIDES];      /* [s] calculated TTP of critical BSW object on the left/right side if present*/
   float32_T bsw_ttle[FBK_NUMBER_OF_SIDES];     /* [s] calculated TTLE of critical BSW object on the left/right side if present*/

   uint8_t cvw_alert[FBK_NUMBER_OF_SIDES]; /* flag indicating if there is a critical CVW object for left/right side */
   uint8_t cvw_id[FBK_NUMBER_OF_SIDES];    /* object ID of critical CVW object on the left/right side if present (0 for none)*/
   uint32_t cvw_unique_id[FBK_NUMBER_OF_SIDES]; /* object Unique ID of critical BSW object on the left/right side if present (0 for
                                                   none)*/
   float32_T cvw_ttc[FBK_NUMBER_OF_SIDES];      /* [s] calculated TTC of critical CVW object on the left/right side if present */
   float32_T cvw_ttp[FBK_NUMBER_OF_SIDES];      /* [s] calculated TTP of critical CVW object on the left/right side if present */
   float32_T cvw_ttle[FBK_NUMBER_OF_SIDES];     /* [s] calculated TTLE of critical CVW object on the left/right side if present */

   uint8_t slc_alert[FBK_NUMBER_OF_SIDES]; /* flag indicating if there is a critical SLC object for left/rightside */
   uint8_t slc_id[FBK_NUMBER_OF_SIDES];    /* object ID of critical SLC object on the left/right side if present (0 for none)*/
   uint32_t slc_unique_id[FBK_NUMBER_OF_SIDES]; /* object Unique ID of critical BSW object on the left/right side if present (0 for
                                                   none)*/
   float32_T slc_ttc[FBK_NUMBER_OF_SIDES];      /* [s] calculated TTC of critical SLC object on the left/right side if present */
   float32_T slc_ttp[FBK_NUMBER_OF_SIDES];      /* [s] calculated TTP of critical SLC object on the left/right side if present */
   float32_T slc_lane_change_probability[FBK_NUMBER_OF_SIDES]; /* lane change probability of critical SLC object on the left/right
                                                              side if present */

   uint8_t awa_alert[FBK_NUMBER_OF_SIDES]; /* flag indicating if there is a critical AWA object for left/rightside */
   uint8_t awa_id[FBK_NUMBER_OF_SIDES];    /* object ID of critical AWA object on the left/right side if present (0 for none)*/

   float32_T awa_ttc[FBK_NUMBER_OF_SIDES]; /* [s] calculated TTC of critical AWA object on the left/right side if present */
   float32_T awa_dec[FBK_NUMBER_OF_SIDES]; /* [m/s^2] calculated required deceleration of critical AWA object on the left/right
                                          side if present */

   /* output from lane model for debugging purposes */
   float32_T lane_width;                              /* [m] Lane width calculated in lane model */
   float32_T lane_center_offset;                      /* [m] Lane center offset calculated in lane model */
   float32_T lane_lateral_speed[FBK_NUMBER_OF_SIDES]; /* [m/s] Lateral speed of the host vehicle towards the lane marking */

   /* output for bmw which needs to be mapped on bmw signals on vehicle bus */
   uint8_t lcda_object_type_left;  /* info if critical object on the left side is BSW (1), CVW (2) or SLC (3) object, (0) if no
                                      critical object */
   uint8_t lcda_object_type_right; /* info if critical object on the left side is BSW (1), CVW (2) or SLC (3) object, (0) if no
                                      critical object */

   uint8_t lcda_object_id_left;         /* object ID of overall critical object on the left side if present */
   uint32_t lcda_object_unique_id_left; /* object Unique ID of overall critical object on the left side if present */
   float32_T lcda_object_width_left;
   float32_T lcda_object_length_left;
   float32_T lcda_object_px_left;  /* [m] longitudinal distance of overall critical object on the left side if present */
   float32_T lcda_object_py_left;  /* [m] lateral distance of overall critical object on the left side if present */
   float32_T lcda_object_ttc_left; /* [s] calculated TTC of overall critical object on the left side if present */
   float32_T lcda_object_vx_left;  /* [m/s] longitudinal speed of overall critical object on the left side if present */
   float32_T lcda_object_vy_left;  /* [m/s] lateral speed of overall critical object on the left side if present */
   uint16_t lcda_object_existance_probability_left; /* existence probability of overall critical object on the left side if present
                                                     */
   uint8_t lcda_object_lane_change_probability_left; /* lane change probability of overall critical object on the left side if
                                                        present */
   uint8_t lcda_object_id_right;                     /*object ID of overall critical object on the left side if present */
   uint32_t lcda_object_unique_id_right;             /* object Unique ID of overall critical object on the left side if present */
   float32_T lcda_object_width_right;
   float32_T lcda_object_length_right;
   float32_T lcda_object_px_right;  /* [m] longitudinal distance of overall critical object on the left side if present */
   float32_T lcda_object_py_right;  /* [m] lateral distance of overall critical object on the left side if present */
   float32_T lcda_object_ttc_right; /* [s] calculated TTC of overall critical object on the left side if present */
   float32_T lcda_object_vx_right;  /* [m/s] longitudinal speed of overall critical object on the left side if present */
   float32_T lcda_object_vy_right;  /* [m/s] lateral speed of overall critical object on the left side if present */
   uint16_t lcda_object_existance_probability_right;  /* existence probability of overall critical object on the left side if
                                                         present */
   uint8_t lcda_object_lane_change_probability_right; /* lane change probability of overall critical object on the left side if
                                                         present */
   Bmw_LCDA_Output_Bus_Signals_T bmw_lcda_output_bus_signals;
} Lcda_Output_T;

#endif /* LCDA_OUTPUT_T_H */
