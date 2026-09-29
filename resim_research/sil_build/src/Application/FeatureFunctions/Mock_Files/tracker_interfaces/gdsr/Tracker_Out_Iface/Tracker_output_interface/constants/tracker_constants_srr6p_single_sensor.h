/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifdef UNIFIED_TRACKER_CONSTANTS_SELECTOR_H
/* This file can ONLY be included by tracker_constants_selector.h!!!*/
#ifndef TRACKER_CONSTANTS_SRR6P_SINGLE_SENSOR_H
#define TRACKER_CONSTANTS_SRR6P_SINGLE_SENSOR_H /*PRQA S 0883 *//* TRACKER_CONSTANTS_SRR6_SINGLE_SENSOR_H is the include guard. UNIFIED_TRACKER_CONSTANTS_SELECTOR_H above is an inverse include guard. */

/**
* \defgroup constants_adjustable_customer_srr6p_single_sensor SRR6 single sensor
* Do not directly include this file, include tracker_constants_selector.h instead.
* \ingroup constants_adjustable_customer
*/

/** 
 * Maximum number of sensors which could send data to this unit.
 * The SRR6 demo currently does not do sensor fusion.
 * \ingroup constants_adjustable_customer_srr6p_single_sensor
 */
#ifndef NUMBER_OF_SENSORS
#define NUMBER_OF_SENSORS    (1)
#endif

 /**
 * Number of detections per sensor
 * \ingroup constants_adjustable_customer_srr6p_single_sensor
 */
#ifndef NUMBER_OF_DETECTIONS
#define NUMBER_OF_DETECTIONS (128)
#endif

#endif
#endif

