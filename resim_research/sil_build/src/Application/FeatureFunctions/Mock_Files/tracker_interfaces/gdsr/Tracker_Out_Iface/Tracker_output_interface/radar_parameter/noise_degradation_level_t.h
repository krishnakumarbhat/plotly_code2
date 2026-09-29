/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/

#ifndef NOISE_DEGRADATION_LEVEL_T_H
#define NOISE_DEGRADATION_LEVEL_T_H

/**
 * The noise degradation level indicates that the noise floor of the received radar signal is increased above average.
 * This signal was added first in January 2021 for being used on SRR5+ sensors. As of this date it's considered as an 
 * optional input signal, intended for future use, in case of increased negative impacts by radar interference, which
 * this signal is an indicator for.
 */
typedef enum
{
	NOISE_DEGRADATION_LEVEL_NONE,     /**< noise level is not more than 5dB above average */
	NOISE_DEGRADATION_LEVEL_LOW,      /**< noise level is 6-9db above average */
	NOISE_DEGRADATION_LEVEL_MEDIUM,   /**< noise level is 10-14db above average */
	NOISE_DEGRADATION_LEVEL_HIGH      /**< noise level is >=15dB above average */
} Noise_Degradation_Level_T;

#endif
