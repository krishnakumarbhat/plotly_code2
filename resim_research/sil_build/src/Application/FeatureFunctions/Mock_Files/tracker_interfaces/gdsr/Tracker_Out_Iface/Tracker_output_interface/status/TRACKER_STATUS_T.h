/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef TRACKER_STATUS_T_H
#define TRACKER_STATUS_T_H

#include "reuse.h"
#include "tracker_errors_T.h"
#include "VERSION_NUMBER_T.h"
#include "tracker_cal_type_T.h"

/**
 * Holds the status output of the last tracker run.
 */

typedef struct
{
   float32_T max_measured_range_current_look; /**< [m] maximum measured range current look. In case detection saturation is
                                               * detected the measured maximum range is shown, the sensors range coverage (all
                                               * looks) otherwise. */
   float32_T time_stamp_delta;      /**< [s] difference between the time stamp of the current and previous call of tracker */
   TRACKER_ERRORS_T tracker_errors; /**< structure holding Tracker error flags */

   tracker_cal_type_T active_tracker_cal_type; /**< The tracker can be started using primary or secondary cals. 1=primary,
                                                * 2=secondary, 255=invalid. This way the features can check if the expected cal
                                                * type is active.*/

   VERSION_NUMBER_T tracker_version; /**< this tracker SW release version (date and iteration)*/

   uint16_t scan_index; /**< scan index of tracking sensor (also named 'master' or 'own)*/

   uint8_t rolling_count_fusionTracker; /**< rolling count for the tracker. Will be increased by one each time the tracker is
                                           called. Used for debugging.*/

   uint8_t f_reset : 1;   /**< flag indicating that the tracker was reset in the current cycle*/
   uint8_t f_updated : 1; /**< flag indicating the full tracker output has been updated. This includes vehicle data and objects as
                             well as detections.*/
   uint8_t f_tracking_behind_guardrails_enabled : 1; /**< flag indicating that the tracker may produce objects located behind
                                                        guardrails.*/
} TRACKER_STATUS_T;

#endif
