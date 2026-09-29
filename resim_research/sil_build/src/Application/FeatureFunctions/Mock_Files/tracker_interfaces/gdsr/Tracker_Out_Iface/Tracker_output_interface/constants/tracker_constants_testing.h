/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifdef UNIFIED_TRACKER_CONSTANTS_SELECTOR_H
/* This file can ONLY be included by tracker_constants_selector.h!!!*/
#ifndef TRACKER_CONSTANTS_H
#define TRACKER_CONSTANTS_H /*PRQA S 0883 *//* TRACKER_CONSTANTS_DEFAULT_H is the include guard. UNIFIED_TRACKER_CONSTANTS_SELECTOR_H above is an inverse include guard. */

/**
* \defgroup constants_adjustable_customer_unit_test Unit Test
* Constants that need a Unit Test dependent value.
* Do not directly include this file, include tracker_constants_selector.h instead.
* \ingroup constants_adjustable_customer
*/

/** 
 * Maximum number of sensors which could send data to this unit.
 * Unit and functional tests use 4 sensors to be able to test 2*pi azimuth tracking.
 * \ingroup constants_adjustable_customer_unit_test
 */
#ifndef NUMBER_OF_SENSORS
#define NUMBER_OF_SENSORS    (4)
#endif

#endif
#endif

