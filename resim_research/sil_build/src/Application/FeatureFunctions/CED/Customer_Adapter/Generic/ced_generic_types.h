#ifndef CED_GENERIC_TYPES_H
#define CED_GENERIC_TYPES_H

#include "pa_reuse.h"
/**
 * @file ced_generic_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the generic types for CED.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */


/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Contains the CED target directions
 *
 * @SRS{CSCSA-122155}
 * @SAD{CSCSA-121741}
 * @SDD{CSCSA-122281}
 */
typedef enum
{
   UNDEF_DIRECTION = (0), /**< Undefined direction */
   REAR_DIRECTION  = (1), /**< Rear direction */
   FRONT_DIRECTION = (2)  /**< Front direction */
} Ced_Target_Travel_Direction_T;


#endif /* CED_GENERIC_TYPES_H */
