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

#include "T360_Types.h"


/*============================================================================*\
* Macros
\*============================================================================*/

/**
 * Number of all available objects
 * \Requirements
 * \reqtrace{}{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define PA_OBJ_NUMBER_OF_OBJECTS ((uint8_t) MAX_F360_OBJECTS)

/**
 * Number of available guardrails
 * \Requirements
 * \reqtrace{}{}
 */
/* coverity[misra_c_2012_rule_2_5_violation] */
#define PA_OBJ_NUMBER_OF_GUARDRAILS ((uint8_t) 2u)

#endif /*FBK_CONST_MACROS_H*/
