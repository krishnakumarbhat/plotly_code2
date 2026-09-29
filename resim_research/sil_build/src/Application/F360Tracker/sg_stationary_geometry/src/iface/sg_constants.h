/*===========================================================================*\
* FILE: sg_constants.h
*============================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains constants used in Stationary Geometries like array sizes, etc.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#ifndef SG_CONSTANTS_H
#define SG_CONSTANTS_H

#include "rspp_constants.h" // remove this dependency later, it is only for MAX_NUMBER_OF_DETECTIONS
#include "sg_input.h"
#include "sg_reuse.h"

static constexpr uint16_t SG_MAX_NUM_CONTOURS = 100U;

static constexpr uint16_t SG_MAX_NUM_VERTICES = 200U;

static constexpr uint16_t SG_MAX_NUM_VERTICES_PER_CONTOUR = 50U;
static_assert(SG_MAX_NUM_VERTICES_PER_CONTOUR <= SG_MAX_NUM_VERTICES,
              "Number of vertices per contour cannot be higher than maximum number of vertices");

static constexpr uint16_t SG_MAX_NUM_DETS_PER_SUBSEGMENT = 10U;

static constexpr uint16_t SG_MAX_NUM_INTERNAL_CLUSTERS = 250U;

static constexpr uint16_t SG_MAX_NUM_INPUT_DETS = sg::rspp::MAX_NUMBER_OF_DETECTIONS; // derived from F360 constants
static_assert(SG_MAX_NUM_INPUT_DETS == sg::rspp::MAX_NUMBER_OF_DETECTIONS,
              "Number of SG input detections should be the same as the number of RSPP detections");

static constexpr uint16_t SG_MAX_NUM_SUBVERTICES_PER_CONTOUR = 100U;

static constexpr uint16_t SG_MAX_NUM_NONDRIVABLE_DETS = 235U;

static constexpr uint16_t SG_MAX_NUM_UNDERDRIVABLE_DETS = 65U;

static constexpr uint16_t SG_MAX_NUM_ENDING_SEGMENT_DETS = 20U;

static constexpr uint16_t SG_MAX_NUM_INTERNAL_DETS = SG_MAX_NUM_NONDRIVABLE_DETS + SG_MAX_NUM_UNDERDRIVABLE_DETS;

static constexpr uint16_t SG_DET_CLASSES_SUM = SG_MAX_NUM_NONDRIVABLE_DETS + SG_MAX_NUM_UNDERDRIVABLE_DETS;
static_assert(SG_DET_CLASSES_SUM == SG_MAX_NUM_INTERNAL_DETS,
              "The maximum number of detections from each drivability class must sum up to the maximum total number of internal "
              "detections");

static constexpr uint8_t SG_MAX_NUM_DET_CLUSTERING_NEIGHBOURS = 250U;
static_assert(SG_MAX_NUM_DET_CLUSTERING_NEIGHBOURS <= SG_MAX_NUM_INTERNAL_DETS,
              "SG_MAX_NUM_DET_CLUSTERING_NEIGHBOURS cannot be greater than SG_MAX_NUM_INTERNAL_DETS");

static constexpr uint16_t SG_MAX_NUM_FUSED_VERTICES = SG_MAX_NUM_VERTICES * 2U;

static constexpr uint16_t SG_MAX_NUM_FUSED_CONTOURS = SG_MAX_NUM_CONTOURS;

static constexpr uint16_t SG_MAX_NUM_FUSED_VERTICES_PER_CONTOUR = SG_MAX_NUM_SUBVERTICES_PER_CONTOUR;

static constexpr uint16_t SG_MAX_NUM_OUTPUT_CONTOURS = SG_MAX_NUM_FUSED_CONTOURS;

static_assert(SG_MAX_NUM_OUTPUT_CONTOURS <= SG_MAX_NUM_FUSED_CONTOURS,
              "Number of output contours cannot be highier than maximum number of fused contours");

static constexpr uint16_t SG_MAX_NUM_OUTPUT_VERTICES = SG_MAX_NUM_FUSED_VERTICES;
static_assert(SG_MAX_NUM_OUTPUT_VERTICES <= SG_MAX_NUM_FUSED_VERTICES,
              "Number of output vertices cannot be highier than maximum number of fused vertices");

static constexpr uint16_t SG_MAX_NUM_REDUCED_OUTPUT_CONTOURS = 10U;
static_assert(SG_MAX_NUM_REDUCED_OUTPUT_CONTOURS <= SG_MAX_NUM_REDUCED_OUTPUT_CONTOURS,
              "Number of reduced output contours cannot be highier than maximum number of output contours");

static constexpr uint16_t SG_MAX_NUM_REDUCED_OUTPUT_VERTICES = 100U;
static_assert(SG_MAX_NUM_REDUCED_OUTPUT_VERTICES <= SG_MAX_NUM_OUTPUT_VERTICES,
              "Number of reduced output vertices cannot be highier than maximum number of output vertices");


// Measurement update
static constexpr uint8_t SG_H_MATRIX_SIZE = 4U;

// Detection bins
static constexpr auto SG_BIN_WIDTH      = 1.0F;
static constexpr auto SG_BIN_BOUNDS_MIN = 0.0F;
static constexpr auto SG_BIN_BOUNDS_MAX = 250.0F;
static constexpr auto SG_NUM_BINS       = static_cast<uint16_t>((SG_BIN_BOUNDS_MAX - SG_BIN_BOUNDS_MIN) / SG_BIN_WIDTH);

// Aliases.
static constexpr int32_t SG_INVALID_ID          = -1;
static constexpr uint8_t SG_INVALID_UNSIGNED_ID = 0U;
static constexpr int32_t SG_INVALID_OUTPUT_ID   = 0;
static constexpr uint32_t SG_DEFAULT_AGE        = 0U;

static constexpr float SG_MIN_PROBABILITY = 0.0F;
static constexpr float SG_MAX_PROBABILITY = 1.0F;

static constexpr float SG_MIN_PRIORITY = 0.0F;
static constexpr float SG_MAX_PRIORITY = 1.0F;

static constexpr float SG_EPSILON = 1.19e-07F;

static constexpr float SG_MIN_DENOMINATOR = SG_EPSILON;

static constexpr uint16_t INVALID_CLUSTER_ID   = 0U;
static constexpr uint32_t INVALID_DETECTION_ID = 0U;
static constexpr uint32_t INVALID_CONTOUR_ID   = 0U;
static constexpr uint32_t INVALID_SEGMENT_ID   = 0U;
static constexpr uint16_t INAVLID_VERTEX_AGE   = 0U;
static constexpr float INVALID_DISTANCE        = 0.0F;

static constexpr uint8_t DC_MAX_COORDINATES_REGION_SIZE               = 8U;
static constexpr uint8_t DC_COORDINATES_DIMENSION                     = 2U;
static constexpr uint8_t DC_NUM_POLYGON_VERTICES_BEFORE_INTERPOLATION = 4U;
static constexpr int8_t MOTION_STATUS_STATIONARY_ID                   = 0;
static constexpr int8_t MOTION_STATUS_AMBIGUOUS_ID                    = 2;

static constexpr double TIME_UNIT_SEC_USEC_CONVERSION_COEF = 1e6;

static constexpr float SG_DEFAULT_DRIVABILITY_CONFIDENCE = 65.0F;

static constexpr float STARTING_VELOCITY_THRESHOLD = 0.0F;
static constexpr float PARKING_VELOCITY_THRESHOLD  = 3.0F;
static constexpr float CITY_VELOCITY_THRESHOLD     = 14.0F;
#endif
