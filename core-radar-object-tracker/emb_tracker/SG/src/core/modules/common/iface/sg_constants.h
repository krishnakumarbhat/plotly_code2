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

#include <cstdint>

namespace sg
{
static constexpr std::int32_t SG_INVALID_ID          = -1;
static constexpr std::uint8_t SG_INVALID_UNSIGNED_ID = 0U;
static constexpr std::int32_t SG_INVALID_OUTPUT_ID   = 0;
static constexpr std::uint32_t SG_DEFAULT_AGE        = 0U;

static constexpr float SG_MIN_PROBABILITY = 0.0F;
static constexpr float SG_MAX_PROBABILITY = 1.0F;

static constexpr float SG_MIN_PRIORITY = 0.0F;
static constexpr float SG_MAX_PRIORITY = 1.0F;

static constexpr float SG_EPSILON = 1.19e-07F;

static constexpr std::uint16_t INVALID_CLUSTER_ID   = 0U;
static constexpr std::uint32_t INVALID_DETECTION_ID = 0U;
static constexpr std::uint32_t INVALID_CONTOUR_ID   = 0U;
static constexpr std::uint32_t INVALID_SEGMENT_ID   = 0U;
static constexpr std::uint16_t INAVLID_VERTEX_AGE   = 0U;
static constexpr float INVALID_DISTANCE             = 0.0F;

static constexpr std::int8_t MOTION_STATUS_STATIONARY_ID = 0;
static constexpr std::int8_t MOTION_STATUS_AMBIGUOUS_ID  = 2;

static constexpr double TIME_UNIT_SEC_USEC_CONVERSION_COEF = 1e6;
} // namespace sg
#endif
