#pragma once
/**
 * Defines different sensor types.
 * Maximum of 16 sensor types(0 to 15) are allowed as per current implementation of DC_GET_VARIANT macro
 */
enum DCSensorType {
   DC_SENSOR_TYPE_SRR5_PLUS = 0,
   DC_SENSOR_TYPE_MRR3,
   DC_SENSOR_TYPE_SRR5,
   DC_SENSOR_TYPE_SRR3,
   DC_SENSOR_TYPE_FLR4,
   DC_SENSOR_TYPE_SRR6_PLUS,
   DC_SENSOR_TYPE_SRR6,
   DC_SENSOR_TYPE_FLR4_PLUS,
   DC_SENSOR_TYPE_SRR7_PLUS,
   DC_SENSOR_TYPE_FLR7,
   DC_SENSOR_TYPE_FLR8,
   DC_SENSOR_TYPE_SRR8_PLUS,
   DC_SENSOR_TYPE_SRR7_PLUS_UWB,
   DC_SENSOR_TYPE_MAX
};
