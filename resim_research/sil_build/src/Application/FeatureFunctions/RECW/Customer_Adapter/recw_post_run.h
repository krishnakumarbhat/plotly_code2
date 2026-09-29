#ifndef RECW_POST_RUN_H
#define RECW_POST_RUN_H

/**
 * @file recw_post_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the RECW post run header file shared by all customers.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "recw_input_t.h"
#include "recw_instance.h"

#include "recw_output_t.h"


/*===========================================================================*\
* Local Function Prototypess
\*===========================================================================*/

/**
 * @brief Calls routine for customer dependend Recw postrun
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2961}
 * @SDD{SF-7969}
 * @verification{}
 */
void Recw_Post_Run(const Recw_Instance_T *p_recw_instance /**< Recw instance */,
                   const Recw_Input_T *p_recw_input /**< Recw input */,
                   Recw_Output_T *p_recw_output /**< Recw output */);


#endif /* RECW_POST_RUN_H */
