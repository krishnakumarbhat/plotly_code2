#ifndef TA_POST_RUN_H
#define TA_POST_RUN_H

/**
 * @file ta_post_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the default post run header file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "ta_input_t.h"
#include "ta_instance_t.h"
#include "ta_output_t.h"

/**
 * @brief This function initializes the TA output structure and persistent data of post run to default values.
 *
 * @return void
 *
 * @SRS{SF-2278}
 * @SAE{SF-3239}
 * @SDD{SF-8650}
 * @verification{Check that the Ta output is reset correctly.}
 */
void Ta_Post_Run_Init(void);

/**
 * @brief This function is the entry for customer specific adaptions after the TA algorithm.
 *
 * This function can be adapted for every customer to adapt the values of the
 * output struct to the customer specific needs.
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3239}
 * @SDD{SF-8548}
 * @verification{}
 */
void Ta_Post_Run(Ta_Instance_T *p_ta_instance,
                 const Ta_Input_T *p_ta_input /**< TA Input */,
                 Ta_Output_T *p_ta_output /**< TA Output */);

#endif /* TA_POST_RUN_H */
