/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
/**
* \file
* Constants that need to be known by callers of the tracker as well as users of the tracker output.
* tracker_constants_selector.h drives setting these constants. First tracker_constants_selector.h
* includes the customer-dependent header "tracker_constant_*.h" which does set all constants
* that need to differ from the default value. Then tracker_constants_default.h is included and
* checks for each constant if it has been set already by a customer-dependent
* header and sets it to a default value if not.
*
* Users of tracker output shall include tracker_constants_selector.h to automatically get the correct constants.
* \ingroup constants_adjustable
*/
#ifndef UNIFIED_TRACKER_CONSTANTS_SELECTOR_H
#define UNIFIED_TRACKER_CONSTANTS_SELECTOR_H

#ifdef GDSR_TRACKER_PROJECT_VARIANT_BMW_SRR5_BPILLAR
#include "tracker_constants_bmw_srr5_bpillar.h"
#elif defined GDSR_TRACKER_PROJECT_VARIANT_HONDA_SRR6P
#include "tracker_constants_honda_srr6p.h"
#elif defined GDSR_TRACKER_PROJECT_VARIANT_RNA
#include "tracker_constants_rna.h"
#elif defined GDSR_TRACKER_PROJECT_VARIANT_NISSAN_SRR6
#include "tracker_constants_nissan_srr6.h"
#elif defined GDSR_TRACKER_PROJECT_VARIANT_SRR6P_SINGLE_SENSOR
#include "tracker_constants_srr6p_single_sensor.h"
#elif defined GDSR_TRACKER_PROJECT_VARIANT_TESTING
#include "tracker_constants_testing.h"
#endif

/* Now set standard values for all still unset constants */
#include "tracker_constants_default.h"

#endif

