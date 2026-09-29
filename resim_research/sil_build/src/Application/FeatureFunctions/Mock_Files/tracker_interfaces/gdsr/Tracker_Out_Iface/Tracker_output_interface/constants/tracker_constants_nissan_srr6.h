/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifdef UNIFIED_TRACKER_CONSTANTS_SELECTOR_H
/* This file can ONLY be included by tracker_constants_selector.h!!!*/
#ifndef TRACKER_CONSTANTS_H
#define TRACKER_CONSTANTS_H

/**
* \defgroup constants_adjustable_customer_nissan_srr6 NISSAN SRR6
* Constants that need a NISSAN dependent value.
* Do not directly include this file, include tracker_constants_selector.h instead.
* \ingroup constants_adjustable_customer
*/

/** 
 * Sieve size to be used for calculation of pseudo pos from mutiple detections 
 * This value was changed from default value 5 to 1 for BMW SRR3 in ticket ABX-782. Unfortunately, it is not documented why. 
 \ingroup constants_adjustable_customer_nissan_srr6
 */
#define NUMBER_OF_BOUNDING_BOX_SIEVES    (1)

/** 
* Number of fused objects over the entire field of view 
* To reduce RAM consumption NISSAN reduces the number of objects to 32
 \ingroup constants_adjustable_customer_nissan_srr6
*/
#define NUMBER_OF_OBJECTS (32)

/** 
 * Maximum number of detections which need to be stored with a NEW track before it becomes MATURE
 * Set to 4 to handle calibration switching correctly
 * \sa k_gp_min_dets_to_create_object
 * \ingroup constants_adjustable_customer_nissan_srr6
 */
#define DETECTION_BUFFER_SIZE    (4)

 /**
 * Number of buffered detections for concrete guardrail detection
 * used in module \ref concrete_guardrail_detector
 * Buffer size was increased by AYF-158 in order to reduce number of FP guardrails
 * \ingroup constants_adjustable_customer_nissan_srr6
 */
#define CONCRETE_GUARDRAIL_DETECTION_BUFFER_SIZE    (14)


#endif
#endif
