/*===========================================================================*\
* FILE: f360_radar_sensor_calib.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains F360_Radar_Sensor_Calib_T structure declaration
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef F360_RADAR_SENSOR_CALIB_H
#define F360_RADAR_SENSOR_CALIB_H

#include "T360_Types.h"
#include "f360_look_ID.h"
#include "f360_sensor_type.h"
#include "f360_sensor_mounting_position.h"
#include "f360_mounting_location.h"
#include "f360_reuse.h"
#include "f360_types.h"

namespace f360_variant_A
{
	static const F360_UI32N_T Num_Vtx_Ant_Sens_Poly = 18U;

	struct F360_Internal_Reflections_Calib
	{
		F360_FPN_T min_host_vel;           // [m/s] identification of internal reflections only updates its buffer slots if host is moving at least this fast

		F360_FPN_T occurrence_lowerlimit;  // [fraction of tracker iterations] identification of internal reflections resets buffer slots below this occurrence rate
		F360_FPN_T occurrence_threshold;   // [fraction of tracker iterations] identification of internal reflections considers a buffer slot a reflection if it occurred at least this often (as a fraction of occurrence_count)

		F360_FPN_T amplitude_tolerance;    // [dB/m^2] identification of internal reflections counts recurrence of detections based on this amplitude tolerance
		F360_FPN_T azimuth_tolerance;      // [rad] identification of internal reflections counts recurrence of detections based on this azimuth tolerance
		F360_FPN_T range_tolerance;        // [m/s] identification of internal reflections counts recurrence of detections based on this range tolerance

		F360_FPN_T max_abs_range_rate;     // [m/s] identification of internal reflections only treats detections below abs(range_rate)
		F360_FPN_T amplitude_max;          // [dB/m^2] identification of internal reflections only treats detections below this amplitude
		F360_FPN_T range_max;              // [m] identification of internal reflections only treats detections within this range

		F360_UI16N_T age_threshold;        // [tracker iterations] Mininmum number of tracker iterations before a buffer slot is evaluated if it is an internal reflection

		F360_BOOLN_T f_enable; // [-] When set to true, the internal reflections functionality will be active for this sensor

		F360_UI8N_T padding[1];
	};


	typedef struct F360_Radar_Sensor_Calib_Tag
	{
		// Sensor properties
		F360_UI32N_T id; // Sensor index (given by the tracker wrapper)

		// Mounting properties
		F360_Sensor_Mounting_Position_T mounting_position;
		F360_SI32N_T polarity; // 1 = normal, -1 = flipped

		// Detection properties
		F360_FPN_T range_limits[F360_NUM_LOOK_ID];
		F360_FPN_T fov_min_az_rad[F360_NUM_LOOK_ID];
		F360_FPN_T fov_max_az_rad[F360_NUM_LOOK_ID];
		F360_FPN_T fov_min_el_rad[F360_NUM_LOOK_ID];
		F360_FPN_T fov_max_el_rad[F360_NUM_LOOK_ID];
		F360_FPN_T min_aliaised_range_rate[F360_NUM_LOOK_ID];
		F360_FPN_T rng_rate_interval_widths[F360_NUM_LOOK_ID];

		// Antenna sensitivity map
		F360_FPN_T ant_sens_SCS_azim[Num_Vtx_Ant_Sens_Poly];   // common azimuth angle knots for all antenna sensitivity maps of the sensor
		F360_FPN_T ant_sens_SCS_sq_rng_90[Num_Vtx_Ant_Sens_Poly]; // border of antenna sensitivity region for a given probability of detection
		F360_FPN_T ant_sens_SCS_sq_rng_50[Num_Vtx_Ant_Sens_Poly]; // border of antenna sensitivity region for a given probability of detection

		// CDC data
		F360_FPN_T   rbin_res[F360_DET_NUM_LOOK_ID];
		F360_FPN_T   dbin_res[F360_DET_NUM_LOOK_ID];
		F360_FPN_T   vua[F360_DET_NUM_LOOK_ID];

		// Version info
		F360_UI32N_T sensor_sw_version;

		// Internal reflection calibrations
		F360_Internal_Reflections_Calib internal_reflections; // Contains enable/disable & tuning parameters for the internal reflections functionality

		// Sensor properties
		F360_Sensor_Type_T sensor_type; // Type of sensor
		F360_BOOLN_T is_valid;

		// Mounting properties
		F360_Mounting_Location_T mounting_location;

		// Antenna sensitivity map
		F360_BOOLN_T f_ant_sens_available;
		F360_BOOLN_T f_ant_sens_degraded;

		// CDC data
		F360_BOOLN_T f_read_cdc_data;  // Read the CDC data for this sensor

		F360_UI8N_T padding[2];

	} F360_Radar_Sensor_Calib_T;

	static_assert(40 == sizeof(F360_Internal_Reflections_Calib), "sizeof(F360_Internal_Reflections_Calib) not as expected. Remember to align padding if needed");
	static_assert((
		sizeof(((F360_Radar_Sensor_Calib_T*)0)->range_limits)
		+ sizeof(((F360_Radar_Sensor_Calib_T*)0)->fov_min_az_rad)
		+ sizeof(((F360_Radar_Sensor_Calib_T*)0)->fov_max_az_rad)
		+ sizeof(((F360_Radar_Sensor_Calib_T*)0)->fov_min_el_rad)
		+ sizeof(((F360_Radar_Sensor_Calib_T*)0)->fov_max_el_rad)
		+ sizeof(((F360_Radar_Sensor_Calib_T*)0)->min_aliaised_range_rate)
		+ sizeof(((F360_Radar_Sensor_Calib_T*)0)->rng_rate_interval_widths)
		+ sizeof(((F360_Radar_Sensor_Calib_T*)0)->ant_sens_SCS_azim)
		+ sizeof(((F360_Radar_Sensor_Calib_T*)0)->ant_sens_SCS_sq_rng_90)
		+ sizeof(((F360_Radar_Sensor_Calib_T*)0)->ant_sens_SCS_sq_rng_50)
		+ sizeof(((F360_Radar_Sensor_Calib_T*)0)->rbin_res)
		+ sizeof(((F360_Radar_Sensor_Calib_T*)0)->dbin_res)
		+ sizeof(((F360_Radar_Sensor_Calib_T*)0)->vua)
		+ 80
		) == sizeof(F360_Radar_Sensor_Calib_T), "sizeof(F360_Radar_Sensor_Calib_T) not as expected. Remember to align padding if needed");
}
#endif
