/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef GUARDRAIL_OUTPUT_T_H
#define GUARDRAIL_OUTPUT_T_H

#include "reuse.h"
#include "track_status_T.h"

/**
 * This is the guardrail data to be transferred to feature functions
 */

typedef struct
{
   float32_T lateral_position;              /**< [m] guardrail lateral position */
   float32_T existence_probability;         /**< [0...1] Measure for how likely the guardrail actually exists */
   track_status_T  status;                  /**< available values are INVALID, NEW, MATURE, and COASTED */
   uint8_t         age;                     /**< Number of cycles the guardrail has been consecutively in a state other than TRACK_STATUS_INVALID. Saturates at UINT8_MAX. \ref GUARDRAIL_OUTPUT_T::status */
   uint8_t         f_active            : 1; /**< flag indicating that this guardrail is enabled */
   uint8_t         f_guardrail_present : 1; /**< flag indicating that a guardrail exists near the sensor */
} GUARDRAIL_OUTPUT_T;

#endif
