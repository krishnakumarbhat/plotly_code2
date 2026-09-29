#pragma once
#include <algorithm>

/**< Converts angle in radians to degrees */
#define DC_RAD2DEG (57.295779513)

/**< Converts angle in degrees to radians */
#define DC_DEG2RAD (00.017453293)

/**< Restricts the value between lower_cap and upper_cap */
#define DC_RANGE_CAP(value, lower_cap, upper_cap) std::min(std::max(value, lower_cap), upper_cap)

/**< Merges customer and sensortype to get a single value */
#define DC_GET_VARIANT(customer, sensor_type) ((customer << 4) | (sensor_type))

/**< Quantize a value to the nearest step */
#define DC_QUANTIZE(value, step) ((std::fmod(value, step) >= 0) ? (value + (step - std::fmod(value, step))) : (value - (step + std::fmod(value, step))))

/**< Macro for conversion of temperature from Kelvin to Celsius  */
#define DC_KELVIN_TO_CELSIUS(temp_kelvin) (temp_kelvin - 273.15)

/**< Macro for conversion of temperature from Celsius to Kelvin  */
#define DC_CELSIUS_TO_KELVIN(temp_celcius) (temp_celcius + 273.15)

/**< Macro definitions for DC sensor configuration based on sensor type (SRR_DC or MRR_DC) */
#ifdef SRR_DC
#define MAX_DC_SENSORS (4)
#define DC_SENSOR      (0)
#elif MRR_DC
#define MAX_DC_SENSORS (1)
#define DC_SENSOR      (4)
#endif

#define N_FTA_OBJECTS        (8u)
#define LCDA_NUMBER_OF_SIDES (2u)
