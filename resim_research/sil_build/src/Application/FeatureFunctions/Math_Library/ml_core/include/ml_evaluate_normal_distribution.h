/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef ML_EVALUATE_NORMAL_DISTRIBUTION_H
#define ML_EVALUATE_NORMAL_DISTRIBUTION_H
/**
 * \defgroup ml_evaluate_normal_distribution Evaluate normal distribution
 *
 */

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Evaluates the normal distribution described by standard_deviation and mean at the point value
 * \return Probability of value with given mean and standard deviation
 * \ingroup ml_evaluate_normal_distribution
 * \sdd{WI-28395}
 */
 float Evaluate_Normal_Distribution(
    float value, /**<[in] value*/
    float mean, /**<[in] mean of the normal distribution*/
    float standard_deviation /**<[in] standard_deviation of the normal distribution */
    );

#ifdef __cplusplus
}
#endif

#endif
