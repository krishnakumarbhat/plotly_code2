#ifndef CT_CALIBRATION_HEADER_T_H
#define CT_CALIBRATION_HEADER_T_H

/**
 * @file ct_calibration.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements the type definition needed for the generic informations of calibrations.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

#include "reuse.h"

#ifdef __cplusplus
#define DEFAULT_INITIALIZATION = {}
#else
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define DEFAULT_INITIALIZATION
#endif

#ifndef CAN_BE_UNUSED
#define CAN_BE_UNUSED(x) (void)(x)
#endif

#ifdef CT_BIG_ENDIAN
typedef struct
{
   uint8_t Cal_Type;
   uint8_t Chk_sum_Version;
   uint16_t Cal_Chk_Sum;
   uint16_t Section_Compatibility;
   uint16_t version;
   uint32_t Section_Size;
} Ct_Header_T;
#else
typedef struct
{
   uint32_t Section_Size;
   uint16_t version;
   uint16_t Section_Compatibility;
   uint16_t Cal_Chk_Sum;
   uint8_t Chk_sum_Version;
   uint8_t Cal_Type;
} Ct_Header_T;
#endif

#endif /*CT_CALIBRATION_HEADER_T_H*/