//
// File: VSE_Master_Model_L2_types.h
//
// Code generated for Simulink model 'VSE_Master_Model_L2'.
//
// Model version                  : 1.794
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Thu Dec 26 19:00:41 2024
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM 64-bit (LP64)
// Code generation objectives:
//    1. RAM efficiency
//    2. ROM efficiency
//    3. Safety precaution
//    4. Execution efficiency
//    5. MISRA C:2012 guidelines
//    6. Traceability
//    7. Debugging
// Validation result: Not run
//
#ifndef RTW_HEADER_VSE_Master_Model_L2_types_h_
#define RTW_HEADER_VSE_Master_Model_L2_types_h_
#include "rtwtypes.h"
#ifndef DEFINED_TYPEDEF_FOR_enum_quality_factor_T_
#define DEFINED_TYPEDEF_FOR_enum_quality_factor_T_

typedef uint8_T enum_quality_factor_T;

// enum enum_quality_factor_T
#define UNDEFINED                      ((enum_quality_factor_T)0U) // Default value 
#define TEMP_UNDEFINED                 ((enum_quality_factor_T)1U)
#define NOT_ACCURATED                  ((enum_quality_factor_T)2U)
#define ACCURATED                      ((enum_quality_factor_T)3U)
#endif

#ifndef DEFINED_TYPEDEF_FOR_VCAN_VSE_
#define DEFINED_TYPEDEF_FOR_VCAN_VSE_

typedef struct {
  // Reverse status of the vehicle
  boolean_T VsVCAN_b_VehReverse;

  // Vehicle Stationary
  boolean_T VsVCAN_b_VehStationary;

  // Lateral Acceleration of Ego vehicle
  real32_T VsVCAN_mps2_RawLatAccel;

  // Quality factor for Lateral acceleration
  enum_quality_factor_T VeVCAN_RawLatAccelQF;

  // Longitudinal Acceleration of Ego vehicle
  real32_T VsVCAN_mps2_RawLongAccel;

  // Quality factor for Longitudinal acceleration
  enum_quality_factor_T VeVCAN_RawLongAccelQF;

  // Road wheel angle for the ego vehicle
  real32_T VsVCAN_deg_RawRoadWhlAngle;

  // Quality factor for Road wheel Angle
  enum_quality_factor_T VeVCAN_RawRoadWhlAngleQF;

  // Steering wheel angle of Ego vehicle
  real32_T VsVCAN_deg_RawSteeringAngle;

  // Quality factor for Steering wheel angle
  enum_quality_factor_T VeVCAN_RawSteeringAngleQF;

  // Unfiltered raw speed of the Ego Vehicle
  real32_T VsVCAN_mps_RawVehSpeed;

  // Quality factor for raw speed
  enum_quality_factor_T VeVCAN_RawVehSpeedQF;

  // Raw yaw rate of the Ego vehicle
  real32_T VsVCAN_rps_RawYawRate;

  // Quality factor for raw yaw rate
  enum_quality_factor_T VeVCAN_RawYawRateQF;

  // Engine Torque
  real32_T VsVCAN_Nm_SumTrqStatic;

  // Engine speed of the vehicle
  real32_T VsVCAN_rpm_EngSpd;

  // Brake torque
  real32_T VsVCAN_Nm_BrkTrq;

  // Gear reduction ratio
  real32_T VsVCAN_TrqAmpTrans;

  // Trailer connection status of TTM
  uint8_T VsVCAN_TrailerConnectionSts;

  // ITBM trailer connection status
  uint8_T VsVCAN_ITBM_TrlrStat;

  // Steering angle bias external (CAN)
  real32_T VsVCAN_deg_SteeringAngleBiasExt;

  // Quality factor for Steering angle bias external (CAN)
  enum_quality_factor_T VeVCAN_SteeringAngleBiasExtQF;

  // Rate of steering angle change
  real32_T VsVCAN_dps_RawSteeringAngleRate;

  // Lateral Acceleration of Ego vehicle (secondary source)
  real32_T VsVCAN_mps2_RawLatAccelSecondary;

  // Quality Factor for secondary source of lateral acceleration
  enum_quality_factor_T VeVCAN_RawLatAccelSecondaryQF;

  // Wheel rotation speed of left wheel
  real32_T VsVCAN_rpm_WheelRotSpd_FL;

  // Wheel rotation speed of right wheel
  real32_T VsVCAN_rpm_WheelRotSpd_FR;

  // Wheel linear speed (left wheel)
  real32_T VsVCAN_mps_WheelLinSpd_FL;

  // Wheel linear speed (right wheel)
  real32_T VsVCAN_mps_WheelLinSpd_FR;

  // Tire pressure of front left wheel
  real32_T VsVCAN_psi_TirePressure_FL;

  // Tire pressure of front right wheel
  real32_T VsVCAN_psi_TirePressure_FR;

  // Vehicle ignition status
  uint8_T VsVCAN_CmdIgnSts;

  // Blind Spot Alert feature selected
  uint8_T VsVCAN_BSDEnable;

  // Trailer detection blind spot mode
  uint8_T VsVCAN_TrailerDetectBlindSpot;
} VCAN_VSE;

#endif

#ifndef DEFINED_TYPEDEF_FOR_Tracker_VSE_
#define DEFINED_TYPEDEF_FOR_Tracker_VSE_

typedef struct {
  // Speed compensation factor from tracker
  real32_T VsTracker_VehSpdCompFac_5;

  // Quality factor for Speed compensation factor
  enum_quality_factor_T VeTracker_VehSpdCompFac_5_QF;

  // Speed compensation factor from tracker
  real32_T VsTracker_VehSpdCompFac_4;

  // Quality factor for Speed compensation factor
  enum_quality_factor_T VeTracker_VehSpdCompFac_4_QF;

  // Speed compensation factor from tracker
  real32_T VsTracker_VehSpdCompFac_3;

  // Quality factor for Speed compensation factor
  enum_quality_factor_T VeTracker_VehSpdCompFac_3_QF;

  // Speed compensation factor from tracker
  real32_T VsTracker_VehSpdCompFac_2;

  // Quality factor for Speed compensation factor
  enum_quality_factor_T VeTracker_VehSpdCompFac_2_QF;

  // Speed compensation factor from tracker
  real32_T VsTracker_VehSpdCompFac_1;

  // Quality factor for Speed compensation factor
  enum_quality_factor_T VeTracker_VehSpdCompFac_1_QF;
} Tracker_VSE;

#endif

#ifndef DEFINED_TYPEDEF_FOR_Vehicle_Parameters_
#define DEFINED_TYPEDEF_FOR_Vehicle_Parameters_

typedef struct {
  // X Position of center of gravity in same coordinate system used for outputs
  real32_T KsVSE_m_COGX;

  // Y Position of center of gravity in same coordinate system used for outputs
  real32_T KsVSE_m_COGY;

  // Longitudinal Distance from rear axle center to front bumper center
  real32_T KsVSE_m_DistRearAxleToVCS;

  // Lateral Distance from rear axle center to a given radar position
  real32_T KsVSE_m_LatDistRadarToRearAxle;

  // Longitudinal Distance from rear axle center to a given radar position
  real32_T KsVSE_m_LongDistRadarToRearAxle;

  // Slope of Rear Axle Sideslip Angle vs Lateral Acceleration
  real32_T KsVSE_RearCorneringCompliance;

  // Ratio of Steering Wheel Angle to Road Wheel Angle,  around steering angle at center 
  real32_T KsVSE_SteeringGearRatio;

  // Understeering coeficient of the vehicle
  real32_T KsVSE_UndersteeringCoeff;

  // Vehicle Width
  real32_T KsVSE_m_VehWidth;

  // Wheel Base,i.e., distance between front axle and rear axle
  real32_T KsVSE_m_WheelBase;

  // Front track width
  real32_T KsVSE_m_FrontTrackWidth;

  // Radius of front wheel
  real32_T KsVSE_m_FrontWhlRadiusStaticLoaded;
} Vehicle_Parameters;

#endif

#ifndef DEFINED_TYPEDEF_FOR_LAST_KEY_CYCLE_
#define DEFINED_TYPEDEF_FOR_LAST_KEY_CYCLE_

typedef struct {
  // Flag which indicates if steering wheel angle bias stored in NVM has converged 
  boolean_T NsVSE_b_LastRemSWABiasFlag;

  // Value of steering bias in NVM
  real32_T NsVSE_deg_LastRemSWABias;

  // Value of speed compensation factor in NVM
  real32_T NsTracker_LastRemSpdCompFac;
} LAST_KEY_CYCLE;

#endif

#ifndef DEFINED_TYPEDEF_FOR_Tracker_VSE_Trailer_Signals_
#define DEFINED_TYPEDEF_FOR_Tracker_VSE_Trailer_Signals_

typedef struct {
  // Radar Trailer connection status
  uint8_T VsTracker_TrailerPresence;

  // Confidence level for radar trailer connection status
  uint8_T VsTracker_TrailerPresenceConfLvl;

  // Trailer length
  real32_T VsTracker_m_TrailerLength;

  // Confidence level for Trailer length
  uint8_T VsTracker_TrailerLengthConfLvl;

  // Trailer width
  real32_T VsTracker_m_TrailerWidth;

  // Confidence level for Trailer width
  uint8_T VsTracker_TrailerWidthConfLvl;

  // Hitch angle
  real32_T VsTracker_rad_TrailerAngle;

  // Hitch angle rate
  real32_T VsTracker_rps_TrailerAngleRate;

  // Confidence level for Hitch angle
  uint8_T VsTracker_TrailerAngleConfLvl;

  // Confidence level for Hitch angle rate
  uint8_T VsTracker_TrailerAngleRateConfLvl;

  // Gap between host vehicle and trailer
  real32_T VsTracker_m_TrailerHVGap;

  // trailer detection status
  uint8_T VsTracker_RadarDetectSts;

  // radar detection timer
  int32_T VsTracker_ms_RadarDetectTimer;

  // stationary timer
  int32_T VsTracker_ms_StationaryTimer;
} Tracker_VSE_Trailer_Signals;

#endif

#ifndef DEFINED_TYPEDEF_FOR_PROXI_
#define DEFINED_TYPEDEF_FOR_PROXI_

typedef struct {
  // TTM trailer is present or not
  boolean_T CsPROXI_CANNode63_TTM;

  // ITBM/ITCM trailer is present or not
  boolean_T CsPROXI_CANNode95_ITBM;

  // Steering Ratio rack type
  uint8_T CsPROXI_SteeringRatioRackPinionType;
} PROXI;

#endif

#ifndef DEFINED_TYPEDEF_FOR_VSE_OUT_
#define DEFINED_TYPEDEF_FOR_VSE_OUT_

typedef struct {
  // Timestamp in microseconds
  uint64_T VsVSE_us_Timestamp;

  // Increments every time the Vehicle State signals are updated
  uint32_T VsVSE_VehIndex;

  // Raw Speed of host vehicle
  real32_T VsVSE_mps_VehRawSpd;

  // Quality Factor for Raw Speed
  enum_quality_factor_T VeVSE_VehRawSpdQF;

  // Correction factor applied using stationary radar detections
  real32_T VsVSE_SpdCompFactor;

  // Quality Factor for Speed Compensation
  enum_quality_factor_T VeVSE_SpdCompFactorQF;

  // Filtered Veh Speed of host vehicle
  real32_T VsVSE_mps_VehFiltSpdOverGround;

  // Quality Factor for filtered Speed
  enum_quality_factor_T VeVSE_VehFiltSpdOverGroundQF;

  // Signed vehicle speed of the ego vehicle
  real32_T VsVSE_mps_VehFiltSignedSpdOverGround;

  // Raw Lat Acceleration of host vehicle
  real32_T VsVSE_mps2_RawLatAccel;

  // Quality factor for Lat acceleration
  enum_quality_factor_T VeVSE_RawLatAccelQF;

  // Raw Long Acceleration of host vehicle
  real32_T VsVSE_mps2_RawLongAccel;

  // Quality factor for Long acceleration
  enum_quality_factor_T VeVSE_RawLongAccelQF;

  // Raw yate rate of the ego vehicle
  real32_T VsVSE_rps_RawYawRate;

  // Quality factor for raw yaw rate
  enum_quality_factor_T VeVSE_RawYawRateQF;

  // Raw steering angle of the vehicle
  real32_T VsVSE_deg_RawSteeringAngle;

  // Quality factor for raw steering angle
  enum_quality_factor_T VeVSE_RawSteeringAngleQF;

  // steering wheel angle at the front wheel tires
  real32_T VsVSE_deg_RoadWhlAngle;

  // Quality factor for road wheel angle
  enum_quality_factor_T VeVSE_RoadWhlAngleQF;

  // Yaw Rate Estimated from Steering Angle
  real32_T VsVSE_rps_YawRateSA;

  // Quality factor for yaw rate using steering angle
  enum_quality_factor_T VeVSE_YawRateSAQF;

  // Bias of raw yaw rate of host vehicle
  real32_T VsVSE_rps_YawRateBias;

  // Quality factor for yaw rate rate bias
  enum_quality_factor_T VeVSE_YawRateBiasQF;
  boolean_T VsVSE_b_YawStopBiasConverged;

  // Unfiltered and bias compensated yaw rate of host vehicle
  real32_T VsVSE_rps_CompYawRateUnfilt;

  // Filtered and bias compensated yaw rate of host vehicle
  real32_T VsVSE_rps_CompYawRateFilt;

  // Quality factor for compensated yaw rate
  enum_quality_factor_T VeVSE_CompYawRateQF;

  // Instantaneous curvature at center of rear axle of host vehicle
  real32_T VsVSE_CurvatureRearAxle;

  // Sideslip angle at origin of Vehicle Coordinate System
  real32_T VsVSE_rad_VCSSideslip;

  // Longitudinal velocity at origin of Vehicle Coordinate System
  real32_T VsVSE_mps_VCSLongVel;

  // Lateral velocity at origin of Vehicle Coordinate System
  real32_T VsVSE_mps_VCSLatVel;

  // Sideslip angle at desired sensor location
  real32_T VsVSE_rad_SensorSideslip;

  // Long velocity at desired sensor location
  real32_T VsVSE_mps_SensorLongVel;

  // Lateral velocity at desired sensor location
  real32_T VsVSE_mps_SensorLatVel;

  // Vehicle Stationary Status
  uint8_T VsVSE_b_VehStationary;

  // Longitudinal Distance from rear axle center to front bumper center
  real32_T KsVSE_m_DistRearAxleToVCS;

  // Lat acceleration at origin of Vehicle Coordinate System
  real32_T VsVSE_mps2_VCSLatAccel;

  // Quality factor for compensated lateral acceleration at Vehicle coodinate system 
  enum_quality_factor_T VeVSE_VCSLatAccelQF;

  // Longitudinal acceleration at origin of Vehicle Coordinate System
  real32_T VsVSE_mps2_VCSLongAccel;

  // Quality factor for compensated long acceleration at Vehicle coodinate system 
  enum_quality_factor_T VeVSE_VCSLongAccelQF;

  // Compensated Lateral Acceleration
  real32_T VsVSE_mps2_CompLatAccel;

  // Quality factor for compensated lateral acceleration
  enum_quality_factor_T VeVSE_CompLatAccelQF;

  // Compensated Longitudinal acceleration
  real32_T VsVSE_mps2_CompLongAccel;

  // Quality factor for compensated long acceleration
  enum_quality_factor_T VeVSE_CompLongAccelQF;

  // Rate of change of lateral acceleration
  real32_T VsVSE_mps3_LatJerk;

  // Quality factor for Lateral Jerk
  enum_quality_factor_T VeVSE_LatJerkQF;

  // Rate of change of Long acceleration
  real32_T VsVSE_mps3_LongJerk;

  // Quality factor for Longitudinal Jerk
  enum_quality_factor_T VeVSE_LongJerkQF;

  // Road surface bank angle estimated with respect to true horizontal
  real32_T VsVSE_deg_BankAngle;

  // Quality Factor for road surface angle estimated with respect to true horizontal 
  enum_quality_factor_T VeVSE_BankAngleQF;

  // Road surface grade angle estimated with respect to true horizontal
  real32_T VsVSE_deg_GradeAngle;

  // Quality Factor for road surface angle estimated with respect to true horizontal 
  enum_quality_factor_T VeVSE_GradeAngleQF;

  // Compensated steering angle
  real32_T VsVSE_deg_CompSteeringAngle;

  // Quality Factor for compensated steering angle
  enum_quality_factor_T VeVSE_CompSteeringAngleQF;

  // Steering angle bias to NVM
  real32_T VsVSE_deg_SWABiasConvergedToNVM;

  // Flag to indicate steering bias has converged
  boolean_T VsVSE_b_SWABiasConvergedToNVMFlag;

  // Flag to indicate if steering bias calculated newly in the current key cycle has converged 
  boolean_T VsVSE_b_SWABiasConvergedInternalFlag;

  // Angular Speed of Steering Wheel
  real32_T VsVSE_dps_SteeringAngleRateFilt;

  // Sideslip angle at rear axle
  real32_T VsVSE_rad_SideslipRearAxle;

  // longitudinal velocity at center of gravity
  real32_T VsVSE_mps_COGLongVel;

  // lateral velocity at center of gravity
  real32_T VsVSE_mps_COGLatVel;

  // Longitudinal Acceleration of host vehicle at the CoG
  real32_T VsVSE_mps2_COGLongAccel;

  // Lateral Acceleration of host vehicle at the CoG
  real32_T VsVSE_mps2_COGLatAccel;

  // Vehicle reverse status
  uint8_T VsVSE_b_VehReverseSts;

  // Vehicle motion direction
  uint8_T VsVSE_VehMotionDirection;

  // The boolean flag indicates if plausibility fault has occurred for the speed CAN signal from BSM module 
  boolean_T VsVSE_b_RawSpdPlausibilityFault;

  // The boolean flag indicates if plausibility fault has occurred for the yaw rate CAN signal from ORC module 
  boolean_T VsVSE_b_RawYawRatePlausibilityFault;

  // The boolean flag indicates if plausibility fault has occurred for the lateral acceleration CAN signal from ORC module 
  boolean_T VsVSE_b_RawLatAccelPlausibilityFault;

  // The boolean flag indicates if plausibility fault has occurred for the longitudinal acceleration CAN signal from ORC module 
  boolean_T VsVSE_b_RawLongAccelPlausibilityFault;

  // The boolean flag indicates if plausibility fault has occurred for the steering angle CAN signal from EPS module 
  boolean_T VsVSE_b_RawSteeringAnglePlausibilityFault;

  // The boolean flag indicates if plausibility fault has occurred for the compensated speed signal  
  boolean_T VsVSE_b_CompSpdPlausibilityFault;

  // The boolean flag indicates if plausibility fault has occurred for the compensated yaw-rate signal  
  boolean_T VsVSE_b_CompYawRatePlausibilityFault;

  // Updates with addition of new modules/removal of modules/change in output interface 
  uint8_T CsVSE_MajorVer;

  // Updates for internal functional changes to algorithms.
  uint8_T CsVSE_MinorVer;

  // Updates after non-functional changes to algorithm, e.g. fixing MISRA compliance warnings 
  uint8_T CsVSE_FieldVer;

  // Updates when calibrations are changed without any other code change
  uint8_T CsVSE_CalibrationVer;
} VSE_OUT;

#endif

#ifndef DEFINED_TYPEDEF_FOR_enum_road_type_T_
#define DEFINED_TYPEDEF_FOR_enum_road_type_T_

typedef int32_T enum_road_type_T;

// enum enum_road_type_T
#define UNKNOWN_ROAD                   (0)                       // Default value 
#define STRAIGHT_ROAD                  (1)
#define CURVED_ROAD                    (2)
#define INTERMEDIATE_ROAD              (3)
#endif

#ifndef DEFINED_TYPEDEF_FOR_VSE_RESIM_T_
#define DEFINED_TYPEDEF_FOR_VSE_RESIM_T_

typedef struct {
  // Flag to indicate if the yaw rate bias converged
  boolean_T VsVSE_b_YawRateBiasConverged;

  // Flag to indicate if the yaw rate bias converged while the vehicle was stationary 
  boolean_T VsVSE_b_YawRateStopBiasConverged;

  // Value calculated after first low pass filtering on raw yaw rate
  real32_T VsVSE_rps_YawRateBias1;

  // alue calculated after second low pass filter on raw yaw rate (i.e. low pass filter on yaw_rate_bias1) 
  real32_T VsVSE_rps_YawRateBias2;

  // Filtered difference between compensated yaw rate and yaw rate estimated from steering angle 
  real32_T VsVSE_rps_CompYawRateDiffFilt;

  // Difference between yaw rate bias calculated using fast filter and slow filter 
  real32_T VsVSE_rps_YawRateBiasDiff;

  // Value calculated after first fast low pass filtering on raw yaw rate
  real32_T VsVSE_rps_YawRateBiasFast1;

  // Value calculated after second fast low pass filter on raw yaw rate (i.e. fast low pass filter on yaw_rate_bias_fast_bias1)  
  real32_T VsVSE_rps_YawRateBiasFast2;

  // Type of road the vehicle is driving on. Calculated solely based on raw or compensated yaw rate and yaw rate estimated from steering angle and speed 
  enum_road_type_T VeVSE_RoadType;

  // Boolean flag indicating possible large bias in raw yaw rate
  boolean_T VsVSE_b_YawRateBiasShift;

  // Boolean flag to indicate if yaw rate has reached steady state
  boolean_T VsVSE_b_YawRateSteady;

  // Boolean flag indicating correctness of yaw rate execution period
  boolean_T VsVSE_b_ExecutionPeriodErrorPresistent;

  // Boolean flag indicating input value range correctness
  boolean_T VsVSE_b_InputInvalidPersistent;

  // Flag to indicate if yaw rate bias was accurate in the previous time step
  boolean_T VsVSE_b_BiasWasAccurate;

  // Time elapsed since Ignition ON
  real32_T VsVSE_s_IgnitionTime;
  boolean_T VsVSE_b_YawStopBiasConverged;

  // Internal State of Kalman Filter indicating curvature
  real32_T VsVSE_CurvKalmanFilterC0;

  // Internal State of Kalman Filter indicating curvature rate of change w.r.t distance 
  real32_T VsVSE_CurvKalmanFilterC1;
} VSE_RESIM_T;

#endif

#ifndef DEFINED_TYPEDEF_FOR_enum_Presence_Status_
#define DEFINED_TYPEDEF_FOR_enum_Presence_Status_

typedef uint8_T enum_Presence_Status;

// enum enum_Presence_Status
#define Neither_Connected              ((enum_Presence_Status)0U) // Default value 
#define HW_Connected_Only              ((enum_Presence_Status)1U)
#define RADAR_Connected_Only           ((enum_Presence_Status)2U)
#define Both_Connected                 ((enum_Presence_Status)3U)
#define Undefined                      ((enum_Presence_Status)4U)
#endif

#ifndef DEFINED_TYPEDEF_FOR_VSE_OUT_Trailer_
#define DEFINED_TYPEDEF_FOR_VSE_OUT_Trailer_

typedef struct {
  // Estimated trailer mass
  real32_T VsVSE_Kg_TrlrMass;

  // Estimated host vehicle mass
  real32_T VsVSE_Kg_HostVehMass;

  // Trailer length
  real32_T VsVSE_m_TrlrLen;

  // Trailer width
  real32_T VsVSE_m_TrlrWidth;

  // Estimated host Vehicle Moment of Inertia
  real32_T VsVSE_Kgm2_HostVehIzz;

  // Trailer Moment of Inertia Converged
  real32_T VsVSE_Kgm2_TrlrIzz;

  // Hitch force Fx 
  real32_T VsVSE_N_HitchFx;

  // Hitch force Fy
  real32_T VsVSE_N_HitchFy;

  // Hitch yaw moment
  real32_T VsVSE_Nm_HitchMz;

  // Hitch angle
  real32_T VsVSE_rad_HitchAng;

  // Hitch angle rate
  real32_T VsVSE_rps_HitchAngSpd;

  // Gap between host vehicle and trailer
  real32_T VsVSE_m_TrlrHostVehGap;

  // Trailer connection status
  enum_Presence_Status VeVSE_TrlrDetectSts;

  // Trailer connection status Confidence level
  uint8_T VeVSE_TrlrDetectStsConfLvl;

  // Trailer mass converged
  boolean_T VsVSE_b_TrlrMassConv;

  // Host vehicle mass converged
  boolean_T VsVSE_b_HostVehMassConv;

  // Trailer length confidence level
  uint8_T VeVSE_TrlrLenConfLvl;

  // Trailer width confidence level
  uint8_T VeVSE_TrlrWidthConfLvl;

  // Host vehicle Moment of Inertia converged
  boolean_T VsVSE_b_HostVehIzzConv;

  // Trailer Moment of Inertia converged
  boolean_T VsVSE_b_TrlrIzzConv;

  // Hitch force Fx confidence level
  uint8_T VeVSE_HitchFxConfLvl;

  // Hitch force Fy confidence level
  uint8_T VeVSE_HitchFyConfLvl;

  // Hitch moment confidence level
  uint8_T VeVSE_HitchMzConfLvl;

  // Hitch angle confidence level
  uint8_T VeVSE_HitchAngConfLvl;

  // Hitch angle rate confidence level
  uint8_T VeVSE_HitchAngSpdConfLvl;

  // Indication of whether the state of radar trailer detection process should be entered or not 
  boolean_T VeVSE_b_RadarDetectionSts;

  // The number of radar cycles for radar detection algo to generate the detection result increasing 1 by 1 and 50 ms in each increasing 
  real32_T VsVSE_s_RadarDetectionTimer;

  // The number of radar cycles for host in stationary status as defined in requirement increasing 1 by 1 and 50 ms in each increasing 
  real32_T VsVSE_s_StationaryTimer;

  // Normalized undesired trailer oscillation magnitude
  real32_T VsVSE_rps_TrlrOscMag;

  // Frequency estimation of undesired trailer oscillation
  real32_T VsVSE_hz_TrlrOscFreq;
} VSE_OUT_Trailer;

#endif

#ifndef DEFINED_TYPEDEF_FOR_RoadWheel_vs_SteeringWheel_Table_
#define DEFINED_TYPEDEF_FOR_RoadWheel_vs_SteeringWheel_Table_

typedef struct {
  real32_T SteeringWheelAngle_Deg[31];
  real32_T RoadWheelAngle_Deg[31];
} RoadWheel_vs_SteeringWheel_Table;

#endif

#ifndef DEFINED_TYPEDEF_FOR_YC_INTERNAL_RESIM_T_
#define DEFINED_TYPEDEF_FOR_YC_INTERNAL_RESIM_T_

typedef struct {
  real32_T yaw_rate_bias1;
  real32_T yaw_rate_bias2;
  real32_T yaw_rate_bias_fast_bias1;
  real32_T yaw_rate_bias_fast_bias2;
  real32_T comp_yaw_rate_diff_filt;
  real32_T yaw_rate_bias_diff;
  enum_road_type_T road_type;
  boolean_T f_yaw_rate_bias_shift;
  boolean_T f_Yaw_Rate_Bias_Converged;
  boolean_T f_stop_bias_converged;
  boolean_T f_yaw_rate_steady;
  boolean_T f_input_invalid_persistent;
  boolean_T f_execution_period_error_persistent;
  boolean_T f_bias_was_accurate;
  real32_T ignition_time;
  boolean_T f_yaw_stop_bias_converged;
} YC_INTERNAL_RESIM_T;

#endif

#ifndef DEFINED_TYPEDEF_FOR_enum_yaw_rate_bias_accuracy_T_
#define DEFINED_TYPEDEF_FOR_enum_yaw_rate_bias_accuracy_T_

typedef int32_T enum_yaw_rate_bias_accuracy_T;

// enum enum_yaw_rate_bias_accuracy_T
#define BIAS_UNDEFINED                 (0)                       // Default value 
#define BIAS_NOT_ACCURATE              (1)
#define BIAS_ACCURATE                  (2)
#endif

#ifndef DEFINED_TYPEDEF_FOR_tire_radius_estimation_mode_T_
#define DEFINED_TYPEDEF_FOR_tire_radius_estimation_mode_T_

typedef uint8_T tire_radius_estimation_mode_T;

// enum tire_radius_estimation_mode_T
#define DEFAULT_RADIUS                 ((tire_radius_estimation_mode_T)0U) // Default value 
#define GPS_BASED_RADIUS               ((tire_radius_estimation_mode_T)1U)
#define RADAR_BASED_RADIUS             ((tire_radius_estimation_mode_T)2U)
#endif
#endif                                 // RTW_HEADER_VSE_Master_Model_L2_types_h_ 

//
// File trailer for generated code.
//
// [EOF]
//
