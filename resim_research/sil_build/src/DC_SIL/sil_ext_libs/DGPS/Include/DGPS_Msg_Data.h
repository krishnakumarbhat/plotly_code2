#ifndef _DGPS_MSG_DATA_H_
#define _DGPS_MSG_DATA_H_
/**********************************************************************
 *
 *	Header %name:	DGPS_Msg_Data.h %
 *	Description:	This header file shall be used to add or modify any DGPS data structures
 *	%created_by:	 pzysy0 %
 *	%date_created:	 Fri Mar 11 07:37:38 2020 %
 *
 **********************************************************************/
// #include "SMValidationInterfaceDefinition.h"
/*Structure definition */
typedef struct
{
   float POS_LOCALX;
   float POS_LOCALY;
} Pos_Local_T;

typedef struct
{
   float RANGE_LOCAL_DELTAX;
   float RANGE_LOCAL_DELTAY;
} Range_Local_T;

typedef struct
{
   float Vel_Local_X;
   float Vel_Local_Y;
   float AngleLocalYaw;
   float AngleLocalTrack;
} Vel_Yaw_Local_T;

typedef struct
{
   float AngRateX;
   float AngRateY;
   float AngRateZ;
} Rate_Vehicle_T;

typedef struct
{
   float RangePosForward;
   float RangeVelForward;
   float RangeTimeToCollisionForward;
} Range_Foward_T;

typedef struct
{
   float RangePosLateral;
   float RangeVelLateral;
   float RangeTimeToCollisionLateral;
} Range_Lateral_T;

typedef struct
{
   float AngleHeading;
   float AnglePitch;
   float AngleRoll;
} Heading_Pitch_Roll_T;

typedef struct
{
   float RangeTargetVehicleLength;
   float RangeTargetVehicleWidth;
   float RangeTargetVehicleNumber;
} Range_Vehicle_T;

typedef struct
{
   float VehicleLength;
   float VehicleWidth;
   float VehicleHeight;
} Veh_Dimension_T;

typedef struct
{
   float VelNorth;
   float VelEast;
   float VelDown;
   float Speed2D;
} Velocity_T;

typedef struct
{
   float VelNorth;
   float VelEast;
   float VelDown;
   float Speed3D;
} Target_Velocity_T;

typedef struct
{
   float VelForward;
   float VelLateral;
} Velocity_Level_T;

typedef struct
{
   float AngRateForward;
   float AngRateLateral;
   float AngRateDown;
} Rate_Level_T;

typedef struct
{
   double Poslat;
   double PosLon;
} Latitude_Longitude_T;

typedef struct
{
   float PosAlt;
} Altitude_T;

typedef struct
{
   double Poslat;
   double PosLon;
} Target_Latitude_Longitude_T;

typedef struct
{
   float Accel_X;
   float Accel_Y;
   float Accel_Z;
} Accel_Vehicle_T;

typedef struct
{
   float AccelForward;
   float AccelLateral;
   float AccelDown;
   float AccelSlip;
} Accel_Level_T;

typedef struct
{
   float TimeYear;
   float TimeCentury;
   float TimeMonth;
   float TimeDay;
   float TimeHSecond;
   float TimeSecond;
   float TimeMinute;
   float TimeHour;
} Date_Time_T;

typedef struct
{
   float GpsNumSats;
   float GpsPosMode;
   float GpsVelMode;
   float GpsAttMode;
} GPS_Status_T;

typedef struct
{
   /************  HOST INFORMATION ************/
   Pos_Local_T Pos_Local;
   Vel_Yaw_Local_T Vel_Yaw_Local;
   Rate_Vehicle_T Rate_Vehicle_Host;
   Range_Foward_T Range_Forward_Host;
   Range_Lateral_T Range_Lateral_Host;
   Heading_Pitch_Roll_T Host_Heading_Pitch_Roll;
   Velocity_T Velocity_Host;
   Velocity_Level_T Velocity_Level_Host;
   Rate_Level_T Rate_Level_Host;
   Latitude_Longitude_T Latitude_Longitude_Host;
   Altitude_T Altitude_Host;
   Accel_Level_T Accel_Level_Host;
   Accel_Vehicle_T Accel_Vehicle_Host;
   Date_Time_T Date_Time;
   GPS_Status_T Gps_Status_Host;
   Range_Local_T Range_Local_Host;
   Veh_Dimension_T Veh_Dimension_Host;

   /************  TARGET INFORMATION ************/
   Heading_Pitch_Roll_T Target_Heading_Pitch_Roll[DGPS_TARGET_COUNT];
   Target_Velocity_T Target_Velocity[DGPS_TARGET_COUNT];
   Velocity_Level_T Target_Velocity_Level[DGPS_TARGET_COUNT];
   Accel_Level_T Target_Accel_Level[DGPS_TARGET_COUNT];
   Accel_Vehicle_T Target_Accel_Vehicle[DGPS_TARGET_COUNT];
   Rate_Vehicle_T Target_Rate_Vehicle[DGPS_TARGET_COUNT];
   Date_Time_T Target_Date_Time;
   Altitude_T Altitude_Target;
   Latitude_Longitude_T Latitude_Longitude_Target[DGPS_TARGET_COUNT];
   Range_Vehicle_T Range_Vehicle[DGPS_TARGET_COUNT];
   Rate_Level_T Target_Rate_Level;

} DGPS_Data_T;

#ifdef BINARY_DEBUG
extern void DGPS_Write_Debug(void);
#endif

#endif
/*===========================================================================*\
 * File Revision History (top to bottom: first revision to last revision)
 *===========================================================================
 *
 * Date            userid    (Description on following lines: SCR #, etc.)
 * ----------- --------
 * 11-03-2020     SARATH   Created Initial version of file with DGPS CAN strucutres
 * 24-03-2020     SARATH   Updated New Strucutres as per request from team
 *\*===========================================================================*/