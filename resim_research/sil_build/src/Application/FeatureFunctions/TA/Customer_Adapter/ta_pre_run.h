#ifndef TA_PRE_RUN_H
#define TA_PRE_RUN_H

/**
 * @file ta_pre_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the default Pre Run header file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "fbk_output.h"
#include "Radar_Config.h"
#include "ta_input_t.h"
#include "ta_instance_t.h"
/**
 * @brief This function initializes the customer input
 *
 * @return void
 *
 * @SRS{SF-2361,SF-2300,SF-2298,SF-2297}
 * @SAE{}
 * @SDD{SF-8551}
 * @verification{}
 */
#ifdef __cplusplus
extern "C"
{
#endif
   extern void Ta_Init_Input(Ta_Input_T *p_ta_input /**< TA Input */,
                             const Radar_Position_T radar_position /**< Radar mounting position */);
#ifdef __cplusplus
}
#endif

/**
 * @brief This function pre processes data before handing it to the TA algorithm
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3237}
 * @SDD{SF-8550}
 * @verification{}
 */
void Ta_Pre_Run(Ta_Instance_T *p_ta_instance /**< TA Instance */,
                const Ta_Input_T *p_ta_input /**< TA Input */,
                const Fbk_Output_T *p_fbk_output);

#endif /* TA_PRE_RUN_H */
