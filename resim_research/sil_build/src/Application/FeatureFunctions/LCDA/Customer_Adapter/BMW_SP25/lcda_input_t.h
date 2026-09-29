/**
 * @file lcda_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the BMW SRR5 input data structure for LCDA.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#ifndef LCDA_INPUT_T_H
#define LCDA_INPUT_T_H

#include "camera_data_t.h"
#include "lcda_bmw_sp25_types.h"
#include "lcda_core_calibration_t.h"
#include "pa_data.h"
#include "pa_reuse.h"


/**
 * @brief BMW SRR5 specific LCDA input structure.
 *
 * @SRS{}
 * @SAE{SF-2782}
 * @SDD{SF-6861}
 */
typedef struct
{

   uint8_t f_lcda_enable;     /* enable/disable LCDA feature */
   uint8_t f_lcda_enable_bsw; /* enable/disable LCDA BSW subfeature */
   uint8_t f_lcda_enable_cvw; /* enable/disable LCDA CVW subfeature */
   uint8_t f_lcda_enable_slc; /* enable/disable LCDA SLC subfeature */
   uint8_t f_lcda_enable_awa; /* enable/disable LCDA AWA subfeature */

   uint8_t f_lcda_enable_dropback;                     /* enable/disable LCDA dropback handler */
   uint8_t f_lcda_enable_fallback;                     /* enable/disable LCDA fallback handler */
   uint8_t f_lcda_enable_environment_plausibilization; /* enable/disable pausibility check "object implausible due to detected
                                                          environment" */

   uint8_t f_lcda_trailer_mode;      /*enable/disable the trailer mode*/
   uint8_t f_lcda_trailer_connected; /*flag to indicate that a trailer is attached*/

   uint8_t f_lcda_enable_cvw_limit_zone; /* enable/disable the longitudinal zone length restriction for CVW */
   uint8_t lcda_cvw_limit_zone_range;    /* length of the longitudinal CVW warning zone restriction behind the ego [m] */
   uint8_t f_lcda_enable_bsw_GBT;        /* enable GBT (china) changes for BSW */

   uint8_t lcda_warntrigger_hmi; /* LCDA warntrigger (early, middle, late) which driver can choose */

   uint8_t f_lcda_enable_basic_lane_model;    /* enable/disable the basic lane model (driving dynamics and navigation data) */
   uint8_t f_lcda_enable_extended_lane_model; /* enable/disable the extended lane model (camera data) */
   uint8_t driver_side;                       /* Driver side (0 = left hand side, 1 = right hand side) */
   uint8_t country_type;                      /* Defines the country to distinguish between lane widths */

   uint8_t navigation_data_road_type;       /* road type based on navigation data */
   uint8_t navigation_data_number_of_lanes; /* number of lanes based on navigation data */

   Bmw_LCDA_Input_Bus_Signals_T lcda_input_signals; /* Contains all customer specific input signals from the vehicle bus */
   Lcda_Coding_Parameters_T lcda_coding_parameters; /* Contains all customer specific inputs from the coding parameter list */
   Camera_Data_T *camera_data;                      /* (processed) camera data */

} Lcda_Input_T;

#endif /* LCDA_INPUT_T_H */
