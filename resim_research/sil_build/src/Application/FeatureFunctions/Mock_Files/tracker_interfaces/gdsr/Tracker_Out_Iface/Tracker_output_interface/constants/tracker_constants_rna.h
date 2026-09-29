/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifdef UNIFIED_TRACKER_CONSTANTS_SELECTOR_H
/* This file can ONLY be included by tracker_constants_selector.h!!!*/
#ifndef TRACKER_CONSTANTS_H
#define TRACKER_CONSTANTS_H

/**
 * \defgroup constants_adjustable_customer_rna RNA
 * Constants that need a RNA dependent value.
 * Do not directly include this file, include tracker_constants_selector.h instead.
 * \ingroup constants_adjustable_customer
 */

/**
 * Number of fused objects over the entire field of view
 * For RAM reduction number of objects is lowered.
 * \ingroup constants_adjustable_customer_rna
 */
#define NUMBER_OF_OBJECTS (48)

/**
 * to improve the true positive performance of the stationary_bounce algorithm
 * the algo shall consider more than one object
 * \ingroup constants_adjustable_customer_rna
 */
#define STATIONARY_BOUNCE_NUMBER_OF_MIRROR_OBJECTS (3)

#endif
#endif
