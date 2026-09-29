#ifndef RADAR_CONFIG_H
#define RADAR_CONFIG_H

typedef enum Radar_Position_Tag
{
   UNKNOWN_POSITION = 0x00,
   REAR_LEFT        = 0x01,
   REAR_RIGHT       = 0x02,
   FRONT_RIGHT      = 0x03,
   FRONT_LEFT       = 0x04,
   REAR_CENTER      = 0x05,
   FRONT_CENTER     = 0x06,
   LEFT_CENTER      = 0x07,
   RIGHT_CENTER     = 0x08,
   INVALID_POSITION = 0x09
} Radar_Position_T;

#endif /* RADAR_CONFIG_H */
