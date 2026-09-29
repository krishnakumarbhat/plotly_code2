/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifdef UNIFIED_TRACKER_CONSTANTS_SELECTOR_H
/* This file can ONLY be included by tracker_constants_selector.h!!!*/
#ifndef TRACKER_CONSTANTS_H
#define TRACKER_CONSTANTS_H

/**
* \defgroup constants_adjustable_customer_bmw_srr5_bpillar BMW SRR5 BPILLAR
* Constants that need a BMW dependent value.
* Do not directly include this file, include tracker_constants_selector.h instead.
* \ingroup constants_adjustable_customer
*/

/**
*The customer BMW_SRR5_BPILLAR is used for the stand-alone sensors in the b-pillar mounting
*position in the context of the BMW SRR5 HIGH program
*there is no sensor fusion and the sensor is providing 128 detections
*/

#define NUMBER_OF_DETECTIONS    (128)

#define NUMBER_OF_SENSORS    (1)

/** 
* Number of fused objects over the entire field of view 
* To reduce RAM consumption BMW reduces the number of objects to 32
 \ingroup constants_adjustable_customer_bmw_srr5
*/
#define NUMBER_OF_OBJECTS (32)

#endif
#endif
