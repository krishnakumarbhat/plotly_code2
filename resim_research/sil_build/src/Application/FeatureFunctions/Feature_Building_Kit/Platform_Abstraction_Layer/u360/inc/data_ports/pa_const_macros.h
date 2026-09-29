#ifndef FBK_CONST_MACROS_H
#define FBK_CONST_MACROS_H

/**
 * @file fbk_const_macros.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains constant macros. Those are split up from the rest of function like macros so that cyclic dependencies are
 * resolved.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

/*============================================================================*\
 * Includes
\*============================================================================*/

#include "TRACKER_OUTPUT_SIDE_T.h"

/*============================================================================*\
* Macros
\*============================================================================*/

/**
 * Number of all available objects
 * \Requirements
 * \reqtrace{}{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define PA_OBJ_NUMBER_OF_OBJECTS ((uint8_t) NUMBER_OF_OBJECTS)

/**
 * Number of available guardrails
 * \Requirements
 * \reqtrace{}{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define PA_OBJ_NUMBER_OF_GUARDRAILS (2u)

#endif