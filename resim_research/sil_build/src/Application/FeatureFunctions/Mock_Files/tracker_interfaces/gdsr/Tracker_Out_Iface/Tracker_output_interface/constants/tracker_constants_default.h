/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifdef UNIFIED_TRACKER_CONSTANTS_SELECTOR_H
/* This file can ONLY be included by tracker_constants_selector.h!!!*/
#ifndef TRACKER_CONSTANTS_DEFAULT_H
#define TRACKER_CONSTANTS_DEFAULT_H /*PRQA S 0883 *//* TRACKER_CONSTANTS_DEFAULT_H is the include guard. UNIFIED_TRACKER_CONSTANTS_SELECTOR_H above is an inverse include guard. */

/**
 * \defgroup constants_public Tracker Public Constants
 * \ingroup Constants
 * Default values for tracker constants. See tracker_constants_selector.h documentation.
 *
 * \defgroup constants_adjustable Adjustable Constants
 * \ingroup constants_public
 * Default values for tracker constants. See tracker_constants_selector.h documentation.
 *
 * \defgroup constants_adjustable_default Default Values For Adjustable Constants
 * \ingroup constants_adjustable
 * Default values for tracker constants. See tracker_constants_selector.h documentation.
 *
 \defgroup constants_adjustable_customer Customer Dependant Values For Adjustable Constants
 * \ingroup constants_adjustable
 * Tracker constants changed for specific customer projects.
 */

/**
 * Maximum number of sensors which could send data to this unit
 * \ingroup constants_adjustable_default
 */
#ifndef NUMBER_OF_SENSORS
#define NUMBER_OF_SENSORS    (2)
#endif

/**
 * Maximum number of look types per sensor
 * \ingroup constants_adjustable_default
 */
#ifndef NUMBER_OF_LOOKS
#define NUMBER_OF_LOOKS    (2)
#endif

/**
 * Number of detections per sensor
 * \ingroup constants_adjustable_default
 */
#ifndef NUMBER_OF_DETECTIONS
#define NUMBER_OF_DETECTIONS    (64)
#endif

/**
 * Number of tracks per sensor
 * \ingroup constants_adjustable_default
 */
#ifndef NUMBER_OF_TRACKS
#define NUMBER_OF_TRACKS    (64)
#endif

/**
 * Number of fused objects over the entire field of view
 * \ingroup constants_adjustable_default
 */
#ifndef NUMBER_OF_OBJECTS
#define NUMBER_OF_OBJECTS    (64)
#endif

/**
 * The number of RSDS sensors in the car (all, not just the fusing ones)
 * \ingroup constants_adjustable_default
 */
#ifndef NUMBER_OF_RSDS_SENSORS_IN_CAR
#define NUMBER_OF_RSDS_SENSORS_IN_CAR    (4)
#endif

/**
 * The number of objects send to other RSDS sensors over a bus system
 * \ingroup constants_adjustable_default
 */
#ifndef NUMBER_OF_OPP_OBJECTS
#define NUMBER_OF_OPP_OBJECTS    (6)
#endif

/**
 * Maximum number of detections which need to be stored with a NEW track before it becomes MATURE
 * \ingroup constants_adjustable_default
 */
#ifndef DETECTION_BUFFER_SIZE
#define DETECTION_BUFFER_SIZE    (3)
#endif

/**
 * Maximum number of cycles that a tracklet may get no updates due to its range. This is relevant for the highest range that the radar uses.
 * The behavior of the system depends on the look types used. This means this is outside the control of the tracker. The raw signal processing defines
 * how the system behaves.
 * As soon as there is a way to exchange information between tracker and raw signal processing this information needs to be transmitted as well.
 * \ingroup constants_adjustable_default
 * */
#ifndef TRACKER_MAX_CYCLES_NO_UPDATE_DUE_TO_RANGE
#define TRACKER_MAX_CYCLES_NO_UPDATE_DUE_TO_RANGE    (4)
#endif

/**
 * Maximum number of detection-track pairs in dataAssociation
 * \ingroup constants_adjustable_default
 */
#ifndef MAX_DATA_ASSOC_MATCHES
#define MAX_DATA_ASSOC_MATCHES    (1000)
#endif

/**
 * Number of bins in the guardrail detector
 * \ingroup constants_adjustable_default
 */
#ifndef GUARDRAIL_DETECT_NUMBER_OF_BINS
#define GUARDRAIL_DETECT_NUMBER_OF_BINS    (57)
#endif

 /**
 * Number of buffered detections for concrete guardrail detection
 * used in module \ref concrete_guardrail_detector
 * \ingroup constants_adjustable_default
 * \sdd{WI-4108}
 */
#ifndef CONCRETE_GUARDRAIL_DETECTION_BUFFER_SIZE
#define CONCRETE_GUARDRAIL_DETECTION_BUFFER_SIZE    (10)
#endif

/**
 * window length for calculation of existence probability
 * \ingroup constants_adjustable_default
 */
#ifndef WIN_LEN
#define WIN_LEN    (20)
#endif

/**
 * number of snail points to keep
 * \ingroup constants_adjustable_default
 */
#ifndef NUMBER_OF_SNAIL_POINTS
#define NUMBER_OF_SNAIL_POINTS    (20)
#endif

/**
 * maximum sectors used for obstruction check in processdetections
 * \ingroup constants_adjustable_default
 */
#ifndef NUMBER_OF_CLUTTER_REGIONS
#define NUMBER_OF_CLUTTER_REGIONS    (30)
#endif

/**
 * finagle shift boxes: number of boxes available
 * \ingroup constants_adjustable_default
 */
#ifndef NUMBER_OF_SHIFT_BOXES
#define NUMBER_OF_SHIFT_BOXES    (5)
#endif

/**
 * max number of detections used for object size estimation
 * \sdd{WI-17921}
 */
#ifndef MAX_NUMBER_DET_OBJECT_SIZE_ESTIMATION
#define MAX_NUMBER_DET_OBJECT_SIZE_ESTIMATION    (20)
#endif

/**
 * max age of detection used for estimating object size
 * \ingroup constants_adjustable_default
 */
#ifndef MAX_AGE_DET_OBJECT_SIZE_ESTIMATION
#define MAX_AGE_DET_OBJECT_SIZE_ESTIMATION    (5)
#endif

/**
 * number of recovery cycles for the Tracker
 * \ingroup constants_adjustable_default
 */
#ifndef TRACKER_RECOVERY_CYCLES
#define TRACKER_RECOVERY_CYCLES    (10)
#endif

/**
 * Sieve size to be used for calculation of pseudo position from multiple detections
 * \ingroup constants_adjustable_default
 */
#ifndef NUMBER_OF_BOUNDING_BOX_SIEVES
#define NUMBER_OF_BOUNDING_BOX_SIEVES    (5)
#endif

/**
 * The number of distinct IDs the tracker can assign to objects. Needs to be bigger or equal than NUMBER_OF_OBJECTS.
 * The distinct ID is not connected to the object index at all.
 * \ingroup constants_adjustable_default
 */
#ifndef NUMBER_OF_DISTINCT_IDS
#define NUMBER_OF_DISTINCT_IDS    (64)
#endif

/**
 * The number of objects which the stationary_bounce countermeasure shall consider as possible mirrors
 * that may cause false detections.
 * \ingroup constants_adjustable_default
 */
#ifndef STATIONARY_BOUNCE_NUMBER_OF_MIRROR_OBJECTS
#define STATIONARY_BOUNCE_NUMBER_OF_MIRROR_OBJECTS	(1)
#endif


 /**
 * Number of wheelspin clusters in tracker internals.
 * \ingroup constants_adjustable_default
 */
#ifndef NUMBER_OF_WHEELSPIN_CLUSTERS
#define NUMBER_OF_WHEELSPIN_CLUSTERS     (10)
#endif

/**
 * \defgroup constants_fixed Fixed constants
 * Constants not depending on a customer.
 * \ingroup constants_public
 */

/**
 * Number of guardrails
 * Always 2: Index 0:left, index 1: right
 * \ingroup constants_fixed
 * */
#define NUMBER_OF_GUARDRAILS           (2)

/**
 * Number of stationary clouds per sensor
 * Always 2: Index 0:left, index 1 : right
 * \ingroup constants_fixed
 */
#define NUMBER_OF_STATIONARY_CLOUDS    (2)

/**
 * The index of the sensor on which the fusion is performed
 * \ingroup constants_fixed
 */
#define FUSION_SENSOR_INDEX            (0)


/**
 * \defgroup constants_derived Derived constants
 * Constants derived from other constants.
 * \ingroup constants_public
 */

/**
 * Maximum number of data points in data buffer in radar parameter
 * \ingroup constants_derived
 */
#define RADAR_PARAMETER_BUFFER_SIZE    (DETECTION_BUFFER_SIZE * TRACKER_MAX_CYCLES_NO_UPDATE_DUE_TO_RANGE)

/**
 * Number of detection clusters in tracker internals.
 * \ingroup constants_derived
 */
#ifndef NUMBER_OF_DETECTION_CLUSTERS
#define NUMBER_OF_DETECTION_CLUSTERS     (NUMBER_OF_DETECTIONS)
#endif


#endif
#endif

