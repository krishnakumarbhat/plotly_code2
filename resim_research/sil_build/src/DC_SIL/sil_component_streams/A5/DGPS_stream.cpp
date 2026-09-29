#include <stdint.h> //compiler warning fixes , std definition overlapping with fixmac.h file
#include "DGPS_stream.h"
#include "DGPS_Decoder.h"

DC_DGPS_Data_T *GetDCDGPSDataptr();
DC_DGPS_Data_T gen7_dc_dgps_object;

void CopyDGPSData(DGPS_Data_T *DGPSData) {
   DC_DGPS_Data_T *DGPS_obj;
   DGPS_obj = GetDCDGPSDataptr();
   memset(DGPS_obj, 0, sizeof(DC_DGPS_Data_T));

   DGPS_obj->Pos_Local.POS_LOCALX                           = DGPSData->Pos_Local.POS_LOCALX;
   DGPS_obj->Pos_Local.POS_LOCALY                           = DGPSData->Pos_Local.POS_LOCALY;
   DGPS_obj->Vel_Yaw_Local.Vel_Local_X                      = DGPSData->Vel_Yaw_Local.Vel_Local_X;
   DGPS_obj->Vel_Yaw_Local.Vel_Local_Y                      = DGPSData->Vel_Yaw_Local.Vel_Local_Y;
   DGPS_obj->Vel_Yaw_Local.AngleLocalYaw                    = DGPSData->Vel_Yaw_Local.AngleLocalYaw;
   DGPS_obj->Vel_Yaw_Local.AngleLocalTrack                  = DGPSData->Vel_Yaw_Local.AngleLocalTrack;
   DGPS_obj->Rate_Vehicle_Host.AngRateX                     = DGPSData->Rate_Vehicle_Host.AngRateX;
   DGPS_obj->Rate_Vehicle_Host.AngRateY                     = DGPSData->Rate_Vehicle_Host.AngRateY;
   DGPS_obj->Rate_Vehicle_Host.AngRateZ                     = DGPSData->Rate_Vehicle_Host.AngRateZ;
   DGPS_obj->Range_Forward_Host.RangePosForward             = DGPSData->Range_Forward_Host.RangePosForward;
   DGPS_obj->Range_Forward_Host.RangeVelForward             = DGPSData->Range_Forward_Host.RangeVelForward;
   DGPS_obj->Range_Forward_Host.RangeTimeToCollisionForward = DGPSData->Range_Forward_Host.RangeTimeToCollisionForward;
   DGPS_obj->Range_Lateral_Host.RangePosLateral             = DGPSData->Range_Lateral_Host.RangePosLateral;
   DGPS_obj->Range_Lateral_Host.RangeVelLateral             = DGPSData->Range_Lateral_Host.RangeVelLateral;
   DGPS_obj->Range_Lateral_Host.RangeTimeToCollisionLateral = DGPSData->Range_Lateral_Host.RangeTimeToCollisionLateral;
   DGPS_obj->Host_Heading_Pitch_Roll.AngleHeading           = DGPSData->Host_Heading_Pitch_Roll.AngleHeading;
   DGPS_obj->Host_Heading_Pitch_Roll.AnglePitch             = DGPSData->Host_Heading_Pitch_Roll.AnglePitch;
   DGPS_obj->Host_Heading_Pitch_Roll.AngleRoll              = DGPSData->Host_Heading_Pitch_Roll.AngleRoll;
   DGPS_obj->Velocity_Host.VelNorth                         = DGPSData->Velocity_Host.VelNorth;
   DGPS_obj->Velocity_Host.VelEast                          = DGPSData->Velocity_Host.VelEast;
   DGPS_obj->Velocity_Host.VelDown                          = DGPSData->Velocity_Host.VelDown;
   DGPS_obj->Velocity_Host.Speed2D                          = DGPSData->Velocity_Host.Speed2D;
   DGPS_obj->Velocity_Level_Host.VelForward                 = DGPSData->Velocity_Level_Host.VelForward;
   DGPS_obj->Velocity_Level_Host.VelLateral                 = DGPSData->Velocity_Level_Host.VelLateral;
   DGPS_obj->Rate_Level_Host.AngRateForward                 = DGPSData->Rate_Level_Host.AngRateForward;
   DGPS_obj->Rate_Level_Host.AngRateLateral                 = DGPSData->Rate_Level_Host.AngRateLateral;
   DGPS_obj->Rate_Level_Host.AngRateDown                    = DGPSData->Rate_Level_Host.AngRateDown;
   DGPS_obj->Latitude_Longitude_Host.PosLon                 = DGPSData->Latitude_Longitude_Host.PosLon;
   DGPS_obj->Latitude_Longitude_Host.Poslat                 = DGPSData->Latitude_Longitude_Host.Poslat;
   DGPS_obj->Altitude_Host.PosAlt                           = DGPSData->Altitude_Host.PosAlt;
   DGPS_obj->Accel_Level_Host.AccelForward                  = DGPSData->Accel_Level_Host.AccelForward;
   DGPS_obj->Accel_Level_Host.AccelLateral                  = DGPSData->Accel_Level_Host.AccelLateral;
   DGPS_obj->Accel_Level_Host.AccelDown                     = DGPSData->Accel_Level_Host.AccelDown;
   DGPS_obj->Accel_Level_Host.AccelSlip                     = DGPSData->Accel_Level_Host.AccelSlip;
   DGPS_obj->Accel_Vehicle_Host.Accel_X                     = DGPSData->Accel_Vehicle_Host.Accel_X;
   DGPS_obj->Accel_Vehicle_Host.Accel_Y                     = DGPSData->Accel_Vehicle_Host.Accel_Y;
   DGPS_obj->Accel_Vehicle_Host.Accel_Z                     = DGPSData->Accel_Vehicle_Host.Accel_Z;
   DGPS_obj->Date_Time.TimeYear                             = DGPSData->Date_Time.TimeYear;
   DGPS_obj->Date_Time.TimeCentury                          = DGPSData->Date_Time.TimeCentury;
   DGPS_obj->Date_Time.TimeMonth                            = DGPSData->Date_Time.TimeMonth;
   DGPS_obj->Date_Time.TimeDay                              = DGPSData->Date_Time.TimeDay;
   DGPS_obj->Date_Time.TimeHSecond                          = DGPSData->Date_Time.TimeHSecond;
   DGPS_obj->Date_Time.TimeSecond                           = DGPSData->Date_Time.TimeSecond;
   DGPS_obj->Date_Time.TimeMinute                           = DGPSData->Date_Time.TimeMinute;
   DGPS_obj->Date_Time.TimeHour                             = DGPSData->Date_Time.TimeHour;
   DGPS_obj->Gps_Status_Host.GpsNumSats                     = DGPSData->Gps_Status_Host.GpsNumSats;
   DGPS_obj->Gps_Status_Host.GpsPosMode                     = DGPSData->Gps_Status_Host.GpsPosMode;
   DGPS_obj->Gps_Status_Host.GpsVelMode                     = DGPSData->Gps_Status_Host.GpsVelMode;
   DGPS_obj->Gps_Status_Host.GpsAttMode                     = DGPSData->Gps_Status_Host.GpsAttMode;
   DGPS_obj->Range_Local_Host.RANGE_LOCAL_DELTAX            = DGPSData->Range_Local_Host.RANGE_LOCAL_DELTAX;
   DGPS_obj->Range_Local_Host.RANGE_LOCAL_DELTAY            = DGPSData->Range_Local_Host.RANGE_LOCAL_DELTAY;
   DGPS_obj->Veh_Dimension_Host.VehicleLength               = DGPSData->Veh_Dimension_Host.VehicleLength;
   DGPS_obj->Veh_Dimension_Host.VehicleWidth                = DGPSData->Veh_Dimension_Host.VehicleWidth;
   DGPS_obj->Veh_Dimension_Host.VehicleHeight               = DGPSData->Veh_Dimension_Host.VehicleHeight;
   for (int i = 0; i < DGPS_TARGET_COUNT; i++) {
      DGPS_obj->Target_Heading_Pitch_Roll[i].AngleHeading = DGPSData->Target_Heading_Pitch_Roll[i].AngleHeading;
      DGPS_obj->Target_Heading_Pitch_Roll[i].AnglePitch   = DGPSData->Target_Heading_Pitch_Roll[i].AnglePitch;
      DGPS_obj->Target_Heading_Pitch_Roll[i].AngleRoll    = DGPSData->Target_Heading_Pitch_Roll[i].AngleRoll;
      DGPS_obj->Target_Velocity[i].VelNorth               = DGPSData->Target_Velocity[i].VelNorth;
      DGPS_obj->Target_Velocity[i].VelEast                = DGPSData->Target_Velocity[i].VelEast;
      DGPS_obj->Target_Velocity[i].VelDown                = DGPSData->Target_Velocity[i].VelDown;
      DGPS_obj->Target_Velocity[i].Speed3D                = DGPSData->Target_Velocity[i].Speed3D;
      DGPS_obj->Target_Velocity_Level[i].VelForward       = DGPSData->Target_Velocity_Level[i].VelForward;
      DGPS_obj->Target_Velocity_Level[i].VelLateral       = DGPSData->Target_Velocity_Level[i].VelLateral;
      DGPS_obj->Target_Accel_Level[i].AccelForward        = DGPSData->Target_Accel_Level[i].AccelForward;
      DGPS_obj->Target_Accel_Level[i].AccelLateral        = DGPSData->Target_Accel_Level[i].AccelLateral;
      DGPS_obj->Target_Accel_Level[i].AccelDown           = DGPSData->Target_Accel_Level[i].AccelDown;
      DGPS_obj->Target_Accel_Level[i].AccelSlip           = DGPSData->Target_Accel_Level[i].AccelSlip;
      DGPS_obj->Target_Accel_Vehicle[i].Accel_X           = DGPSData->Target_Accel_Vehicle[i].Accel_X;
      DGPS_obj->Target_Accel_Vehicle[i].Accel_Y           = DGPSData->Target_Accel_Vehicle[i].Accel_Y;
      DGPS_obj->Target_Accel_Vehicle[i].Accel_Z           = DGPSData->Target_Accel_Vehicle[i].Accel_Z;
      DGPS_obj->Target_Rate_Vehicle[i].AngRateX           = DGPSData->Target_Rate_Vehicle[i].AngRateX;
      DGPS_obj->Target_Rate_Vehicle[i].AngRateY           = DGPSData->Target_Rate_Vehicle[i].AngRateY;
      DGPS_obj->Target_Rate_Vehicle[i].AngRateZ           = DGPSData->Target_Rate_Vehicle[i].AngRateZ;
      DGPS_obj->Target_Date_Time.TimeYear                 = DGPSData->Target_Date_Time.TimeYear;
      DGPS_obj->Target_Date_Time.TimeCentury              = DGPSData->Target_Date_Time.TimeCentury;
      DGPS_obj->Target_Date_Time.TimeMonth                = DGPSData->Target_Date_Time.TimeMonth;
      DGPS_obj->Target_Date_Time.TimeDay                  = DGPSData->Target_Date_Time.TimeDay;
      DGPS_obj->Target_Date_Time.TimeHSecond              = DGPSData->Target_Date_Time.TimeHSecond;
      DGPS_obj->Target_Date_Time.TimeSecond               = DGPSData->Target_Date_Time.TimeSecond;
      DGPS_obj->Target_Date_Time.TimeMinute               = DGPSData->Target_Date_Time.TimeMinute;
      DGPS_obj->Target_Date_Time.TimeHour                 = DGPSData->Target_Date_Time.TimeHour;
      DGPS_obj->Altitude_Target.PosAlt                    = DGPSData->Altitude_Target.PosAlt;
      DGPS_obj->Latitude_Longitude_Target[i].PosLon       = DGPSData->Latitude_Longitude_Target[i].PosLon;
      DGPS_obj->Latitude_Longitude_Target[i].Poslat       = DGPSData->Latitude_Longitude_Target[i].Poslat;
      DGPS_obj->Range_Vehicle[i].RangeTargetVehicleLength = DGPSData->Range_Vehicle[i].RangeTargetVehicleLength;
      DGPS_obj->Range_Vehicle[i].RangeTargetVehicleWidth  = DGPSData->Range_Vehicle[i].RangeTargetVehicleWidth;
      DGPS_obj->Range_Vehicle[i].RangeTargetVehicleNumber = DGPSData->Range_Vehicle[i].RangeTargetVehicleNumber;
      DGPS_obj->Target_Rate_Level.AngRateForward          = DGPSData->Target_Rate_Level.AngRateForward;
      DGPS_obj->Target_Rate_Level.AngRateLateral          = DGPSData->Target_Rate_Level.AngRateLateral;
      DGPS_obj->Target_Rate_Level.AngRateDown             = DGPSData->Target_Rate_Level.AngRateDown;
   }
}

DC_DGPS_Data_T *GetDCDGPSDataptr() {
   return &gen7_dc_dgps_object;
}
