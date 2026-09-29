#ifndef _SOME_IP_INPUT_H_
#define _SOME_IP_INPUT_H_

#include "some_ip_com.h"
#include "fixmac.h"
#define SOME_IP_ECU_RX_SIGNAL_STRUCTURE_VERSION (2)

typedef signed long sint32;

#ifndef sint16
typedef signed short sint16;
#endif

/*Input signal Varibles Declaration*/
typedef uint8 RangeLaneMarkingEgo;
typedef uint16 Coefficient0LaneMarkingEgo;
typedef uint16 Coefficient1LaneMarkingEgo;
typedef uint16 Coefficient2LaneMarkingEgo;
typedef uint16 Coefficient3LaneMarkingEgo;
typedef uint8 IdLaneMarkingEgoProperty;
typedef uint8 SubsistenceProbabilitylaneMarkingEgo;

typedef uint8 StatusMeasurmentlaneMarkingEgo;
typedef uint8 TypelaneMarkingEgo;

typedef uint8 ColorLaneMarkingEgo;
typedef uint8 BranchLaneMarkingEgo;
typedef uint8 QualityLaneWidthLaneMarkingNext;
typedef uint8 LaneWidthLaneMarkingNext;
typedef uint8 WidthLaneBoundaryEgo;
typedef uint8 RatioLaneBoundaryEgo;
typedef uint16 LaneMarkingNanoseconds;
typedef uint8 Qualifer_EventData_LaneMarking;
typedef uint16 LaneMarkingSeconds;
typedef uint8 Qualifier_Extended_LaneMarking;
typedef uint8 LaneMarkingTrafficState;
typedef uint8 SynchronisationStatusLaneMarking;
typedef uint8 NO_VECH_1;
typedef uint8 NO_VECH_2;
typedef uint8 NO_VECH_3;
typedef uint8 NO_VECH_4;
typedef uint8 NO_VECH_5;
typedef uint8 NO_VECH_6;
typedef uint8 NO_VECH_7;
typedef uint32_t MILE_KM;
typedef uint32_t MileageKilometreType;
typedef uint16 UI_16_noIniVal_noDflVal_65535_1_0_noEnc_hasRng_2;
typedef uint32_t UI_32_noIniVal_noDflVal_4294967295_1_0_noEnc_hasRng;
typedef uint8 UI_8_noIniVal_noDflVal_255_0_50s_MI_40_noEnc_hasRng;
typedef uint8 QualifierActualValueBrakingTorqueSum;
typedef uint8 QualifierAccelerationLongitudinalCentreOfGravity;
typedef uint8 QualifierAccelerationLateralCenterOfGravity;
typedef uint8 QualifierActualValueAngleAcceleratorPedal;
typedef uint8 QualifierYawRate;
typedef int BOOL;
typedef uint16 Coefficient3LaneMarkingNext;
typedef float32 Coding_195888358;
typedef float32 Coding_195888336;
typedef float32 Coding_195886736;
typedef float32 Coding_195886770;
typedef float32 Coding_195886778;
typedef sint32 Coding_195887514;
typedef sint32 Coding_195887571;
typedef sint32 Coding_195887479;
typedef sint32 Coding_391887870;
typedef sint32 Coding_421894553;
typedef sint16 Coding_195887502;
typedef sint16 Coding_391887874;
typedef uint32_t Coding_104861712;
typedef float32 Coding_195887923;
typedef float32 Coding_195888035;
typedef float32 Coding_195887552;
typedef float32 Coding_195888393;
typedef float32 Coding_195888377;
typedef float32 Coding_195888422;
typedef float32 Coding_195888035;
typedef float32 Coding_195888066;
typedef float32 Coding_195888300;
typedef float32 Coding_195888251;
typedef float32 Coding_195887845;
typedef float32 Coding_195887768;
typedef float32 Coding_195887792;
typedef float32 Coding_195887654;
typedef float32 Coding_195887694;
typedef uint8 QualifierCurvature;
typedef uint8 QualifierFunctionABSStatus;
typedef uint8 QualifierFunctionABSStruct_qualifierFunctionABSIntervention;
typedef uint8 QualifierFunctionASCStatus;
typedef uint8 QualifierFunctionASC_qualifierFunctionASCIntervention;
typedef uint8 QualifierFunctionBrakeChain;
typedef uint8 QualifierFunctionCoordinatorBrakeRequest;
typedef uint8 QualifierFunctionDBCParametrisation;
typedef uint8 QualifierFunctionFDRStatus;
typedef uint8 QualifierFunctionFDRStruct_qualifierFunctionFDRIntervention;
typedef uint8 QualifierFunctionPrefill;
typedef uint8 QualifierSlipSideAngle;
typedef uint8 StatusDisplaySegmentExteriorMirrorDriver;
typedef uint8 QualifierFunctionWarningChain;

typedef uint8 StatusBrakingVehicleStandstillErrorRecovery;
typedef uint8 WarnBrakeCoordinationStruct_functionForewarningFrontalProtectionSystem;
typedef uint8 WarnBrakeCoordinationStruct_functionAcuteWarningFrontalProtectionSystem;
typedef uint8 StatusAccelerationWishDriverWarnBrakeCoordinatorEnum;
typedef uint8 QualifierActualValueBrakingTorqueSumDriversChoice;
typedef uint8 StatusBrakingDriverStruct_statusBrakingDriver;
typedef uint8 QualifierOrientationVehicleBody;
typedef uint8 ActiveFunctionIndicateDirection;
typedef uint8 ControlIndicateDirection;
typedef uint8 CTR_ERRM_BN_U;
typedef uint8 ControlErrorMemoryHAFBordnetzVoltage;
typedef uint8 StatusUnitRunDSC;
typedef uint8 QualifierSteeringAngleFrontAxle;
typedef uint8 ST_TRAI;
typedef uint8 QualifierSteeringAngleDriver;
typedef uint8 ST_INTF_DRASY_TAR_LIM;
typedef uint8 ST_MOT_DRV;
typedef uint8 ST_PENG_PT;
typedef uint8 ST_UDP;
typedef uint8 ST_AVAI_INTV_PT_DRS;
typedef uint8 ST_ANO_MSA_ENG_STOP;
typedef uint8 ST_CENG_DRV;
typedef uint8 ST_ENERG_SUPY;
typedef uint16 SecureDateAndTimeSDaTTTAQType;
typedef uint8 SecureDateAndTimeSDaTTTSQType;
typedef uint8 ST_DRVDIR_DVCH;
typedef uint8 QU_AVL_RPM_BAX_RED;
typedef uint8 QU_ST_CON_VEH;
typedef uint8 QU_VYAW_VEH;
typedef uint16 RPM_GRB_NEGL_2;
typedef uint8 TR_HHASS_MSA;
typedef uint8 CTR_LED_PUBU_MSA;
typedef uint16 VehicleAccelerationLongitudinalEstimatedSK;
typedef uint16 VehicleSpeedLongitudinal;
typedef uint8 QualifierVehicleAccelerationLongitudinalEstimatedSK;
typedef uint8 StatusErrorSeatMatDR;
typedef uint8 StatusSeatOccupyingDR;
typedef uint8 StatusSeatOccupyingPS;
typedef uint8 StatusSeatOccupyingRLH;
typedef uint8 StatusSeatOccupyingRM;
typedef uint8 StatusSeatOccupyingRRH;
typedef uint8 ControlSettingLaneChangeWarning;
typedef uint8 ControlPrewarning;
typedef uint8 ControlLaneChangeWarning;
typedef uint8 ControlCrossTrafficBrake;
typedef uint8 ControlActiveSafety;
typedef uint8 IDX_PRES_SEG_NAVGRPH_2;
typedef uint16 DIR_CHNG_ANG_SEG_NAVGRPH_2;
typedef uint16 RAD_PRES_SEG_NAVGRPH_2;
typedef uint8 QUAN_LNS_NAVGRPH_2;
typedef uint8 TYP_STREET_NAVGRPH_2;
typedef uint8 L_SEG_NAVGRPH_2;
typedef uint8 LIM_V_NAVGRPH_2;
typedef uint16 PROP_MAP_NAVGRPH_2;
typedef uint8 StatusInsidePushButton;
typedef uint8 StatusBeltBuckleSwitchDR;
typedef uint8 StatusBeltBuckleSwitchPS;
typedef uint8 StatusBeltBuckleSwitchRLH;
typedef uint8 StatusBeltBuckleSwitchRM;
typedef uint8 StatusBeltBuckleSwitchRRH;
typedef uint8 StatusBeltBuckleSwitch3LeftHandTierRearSide;
typedef uint8 StatusBeltBuckleSwitch3MiddleTierRearSide;
typedef uint8 StatusBeltBuckleSwitch3RightHandTierRearSide;
typedef uint8 ControlSafeExitSound;
typedef uint8 ControlSafeExit;
typedef uint8 DrivingDirectionVehicleConfirmed;
typedef uint8 DrivingDirectionVehicleUnconfirmed;
typedef uint8 QualifierVelocityVehicleLongitudinal;
typedef uint8 StatusVelocityNearStandstill;
typedef uint8 QualifierVelocityVehicleCog;
typedef uint8 CTR_BS_PRTNT;
typedef uint32_t Anforderung_Funktionale_Teilnetze;
typedef uint8 ST_CON_VEH;
typedef uint16 VYAW_VEH;
typedef uint16 VYAW_VEH_ERR_AMP;
typedef uint16 AVL_V_SPDM_2;
typedef uint8 UN_V_SPDM;
typedef uint8 AVAI_DISP_KI;
typedef uint8 ST_DISP_DRASY_LAG;
typedef uint8 ST_DISP_GAP_INFO_2;
typedef uint8 StatusEndOfAssemblyLine;
typedef uint8 StatusDynamometer;
typedef uint8 QualifierVehicleAccelerationLongitudinalEstimated;
typedef uint16 ActiveCommunicationRequestCauseEnum;
typedef unsigned char Rte_DT_VehicleConditionWrapper_4;
typedef uint8 StatusValvetrain;
typedef UINT32 Rte_DT_StatusCombustionEngineSecureWrapper_9;
typedef uint8 StatusElectricMode;
typedef uint8 CTR_ACTVN_PWHOP;
typedef uint8 QualifierFunctionPreX;
typedef uint8 StatusWarnBrakeCoordinatorStruct;
typedef UINT8 Rte_DT_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoLeftHandProperty_3;
typedef UINT8 Rte_DT_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoLeftHandProperty_7;
typedef UINT8 Rte_DT_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoRightHandProperty_3;
typedef UINT8 Rte_DT_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoRightHandProperty_7;
typedef UINT8 Rte_DT_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingList_7;
typedef uint8 IntegrityLaneBoundaries;
typedef UINT32 Rte_DT_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingNext_16;
typedef uint8 StatusDoorSwitchDRD;
typedef uint8 StatusDoorSwitchPSD;
typedef uint8 StatusDoorSwitchDRDR;
typedef uint8 StatusDoorSwitchPSDR;
typedef uint8 ControlSetting2MUTMDrive;
typedef uint8 ControlSetting2FahrmodusMDrive;
typedef uint8 ControlSetting2RECWMDrive;
typedef uint8 StatusStageDrive;
typedef uint8 StatusActualGearDrive;
typedef UINT8 Rte_DT_StatusGearboxDriveWrapper_2;
typedef uint16 StatusGearAvailabilityDrive;
typedef uint8 Qualifier_StatusActualStageDrive;
typedef UINT8 Rte_DT_StatusGearboxDriveWrapper_6;

typedef struct
{
   Coding_195888358 yawRateErrAmp;
   BOOL invalidFlag;
} YawRateErrAmp;

typedef struct
{
   Coding_195888336 yawRate;
   BOOL invalidFlag;
} YawRate;

typedef struct
{
   Coding_195887514 actualValueBrakingTorqueSum;
   BOOL invalidFlag;
} ActualValueBrakingTorqueSum;

typedef struct
{
   Coding_421894553 gradientActualValueAngleAcceleratorPedal;
   BOOL invalidFlag;
} GradientActualValueAngleAcceleratorPedal;

typedef struct
{
   Coding_195887552 actualValueAngleAcceleratorPedal;
   BOOL invalidFlag;
} ActualValueAngleAcceleratorPedal;

typedef struct
{
   Coding_195888393 ActualValueSlipSideAngleVehicleMotion;
   BOOL invalidFlag;
} SlipSideAngle;

typedef struct
{
   Coding_195888377 ActualValueCurvatureVehicleMotion;
   BOOL invalidFlag;
} Curvature;

typedef struct
{
   Coding_195888422 ActualValueSlipSideAngleVehicleMotionErrorAmplitude;
   BOOL invalidFlag;
} SlipSideAngleErrAmp;

typedef struct
{
   Coding_391887870 actualValueBrakingTorqueSumDriversChoice;
   BOOL invalidFlag;
} ActualValueBrakingTorqueSumDriversChoice;

typedef struct
{
   Coding_391887874 actualValueBrakingTorqueSumDriversChoiceRaw;
   BOOL invalidFlag;
} ActualValueBrakingTorqueSumDriversChoiceRaw;

typedef struct
{
   StatusBrakingDriverStruct_statusBrakingDriver statusBrakingDriver;
   BOOL invalidFlag;
} StatusBrakingDriverStruct;

typedef struct
{
   Coding_195886736 heightLevelVehicleBodyRearAxle;
   BOOL invalidFlag;
} HeightLevelVehicleBodyRearAxle;

typedef struct
{
   Coding_195886770 anglePitchVehicleBodyRearAxle;
   BOOL invalidFlag;
} AnglePitchVehicleBodyRearAxle;

typedef struct
{
   Coding_195886778 angleRollVehicleBodyRearAxle;
   BOOL invalidFlag;
} AngleRollVehicleBodyRearAxle;

typedef struct
{
   BOOL vehicleHoldOnZeroVelocity;
   BOOL moveOffTriggerExceeded;
   BOOL customerBrakingThresholdExceeded;
   BOOL invalidFlag;
} StatusBrakingMSAStruct;

typedef struct
{
   QualifierFunctionFDRStatus statusFunction;
   QualifierFunctionFDRStruct_qualifierFunctionFDRIntervention qualifierFunctionFDRIntervention;
} QualifierFunctionFDRStruct;

typedef struct
{
   QualifierFunctionABSStatus statusFunction;
   QualifierFunctionABSStruct_qualifierFunctionABSIntervention qualifierFunctionABSIntervention;
} QualifierFunctionABSStruct;

typedef struct
{
   QualifierFunctionASCStatus statusFunction;
   QualifierFunctionASC_qualifierFunctionASCIntervention qualifierFunctionASCIntervention;
} QualifierFunctionASC;

typedef struct
{
   Coding_195888035 steeringAngleFrontAxle;
   BOOL invalidFlag;
} SteeringAngleFrontAxle;

typedef struct
{
   Coding_195888066 steeringAngleFrontAxleErrAmp;
   BOOL invalidFlag;
} SteeringAngleFrontAxleErrAmp;

typedef struct
{
   Coding_195887923 steeringAngleDriver;
   BOOL invalidFlag;
} SteeringAngleDriver;

typedef struct
{
   Coding_195888300 velocityVehicleLongitudinal;
   BOOL invalidFlag;
} VelocityVehicleLongitudinal;

typedef struct
{
   Coding_195888251 velocityVehicleCog;
   BOOL invalidFlag;
} VelocityVehicleCog;

typedef struct
{
   Coding_195887845 VehicleAccelerationLongitudinalEstimated;
   BOOL invalidFlag;
} VehicleAccelerationLongitudinalEstimated;

typedef struct
{
   Coding_195887768 accelerationLongitudinalCentreOfGravity;
   BOOL invalidFlag;
} AccelerationLongitudinalCog;

typedef struct
{
   Coding_195887792 accelerationLongitudinalCentreOfGravityErrorAmplitude;
   BOOL invalidFlag;
} AccelerationLongitudinalCogErrAmp;

typedef struct
{
   Coding_195887654 accelerationLateralCentreOfGravity;
   BOOL invalidFlag;
} AccelerationLateralCog;

typedef struct
{
   Coding_195887694 accelerationLateralCentreOfGravityErrorAmplitude;
   BOOL invalidFlag;
} AccelerationLateralCogErrAmp;

#define Rte_TypeDef_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoLeftHandGeometry
typedef struct
{
   RangeLaneMarkingEgo rangeLaneMarkingEgoLeftHand;
   Coefficient0LaneMarkingEgo coefficient0LaneMarkingEgoLeftHand;
   Coefficient1LaneMarkingEgo coefficient1LaneMarkingEgoLeftHand;
   Coefficient2LaneMarkingEgo coefficient2LaneMarkingEgoLeftHand;
   Coefficient3LaneMarkingEgo coefficient3LaneMarkingEgoLeftHand;
} ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoLeftHandGeometry;

#define Rte_TypeDef_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoLeftHandProperty
typedef struct
{
   IdLaneMarkingEgoProperty iDLaneMarkingEgoLeftHand;
   SubsistenceProbabilitylaneMarkingEgo subsistenceProbabilityLaneMarkingEgoLeftHand;
   StatusMeasurmentlaneMarkingEgo statusMeasurmentLaneMarkingEgoLeftHand;
   Rte_DT_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoLeftHandProperty_3 dummy1;
   TypelaneMarkingEgo typeLaneMarkingEgoLeftHand;
   ColorLaneMarkingEgo colorLaneMarkingEgoLeftHand;
   BranchLaneMarkingEgo branchLaneMarkingEgoLeftHand;
   Rte_DT_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoLeftHandProperty_7 dummy2;
   RangeLaneMarkingEgo distanceBranchLaneMarkingEgoLeftHand;
   QualityLaneWidthLaneMarkingNext qualityLaneWidtLaneMarkingNextLeftHand;
   LaneWidthLaneMarkingNext laneWidthLaneMarkingNextLeftHand;
} ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoLeftHandProperty;

#define Rte_TypeDef_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoRightHandGeometry
typedef struct
{
   RangeLaneMarkingEgo rangeLaneMarkingEgoRightHand;
   Coefficient0LaneMarkingEgo coefficient0LaneMarkingEgoRightHand;
   Coefficient1LaneMarkingEgo coefficient1LaneMarkingEgoRightHand;
   Coefficient2LaneMarkingEgo coefficient2LaneMarkingEgoRightHand;
   Coefficient3LaneMarkingEgo coefficient3LaneMarkingEgoRightHand;
} ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoRightHandGeometry;

#define Rte_TypeDef_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoRightHandProperty
typedef struct
{
   IdLaneMarkingEgoProperty iDLaneMarkingEgoRightHand;
   SubsistenceProbabilitylaneMarkingEgo subsistenceProbabilityLaneMarkingEgoRightHand;
   StatusMeasurmentlaneMarkingEgo statusMeasurmentLaneMarkingEgoRightHand;
   Rte_DT_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoRightHandProperty_3 dummy1;
   TypelaneMarkingEgo typeLaneMarkingEgoRightHand;
   ColorLaneMarkingEgo colorLaneMarkingEgoRightHand;
   BranchLaneMarkingEgo branchLaneMarkingEgoRightHand;
   Rte_DT_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoRightHandProperty_7 dummy2;
   RangeLaneMarkingEgo distanceBranchLaneMarkingEgoRightHand;
   QualityLaneWidthLaneMarkingNext qualityLaneWidtLaneMarkingNextRightHand;
   LaneWidthLaneMarkingNext laneWidthLaneMarkingNextRightHand;
} ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoRightHandProperty;

#define Rte_TypeDef_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingExtensionLeftRight
typedef struct
{
   WidthLaneBoundaryEgo widthLaneBoundaryEgoRight;
   WidthLaneBoundaryEgo widthLaneBoundaryEgoLeft;
   RatioLaneBoundaryEgo ratioLaneBoundaryEgoRight;
   RatioLaneBoundaryEgo ratioLaneBoundaryEgoLeft;
} ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingExtensionLeftRight;

#define Rte_TypeDef_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingList
typedef struct
{
   LaneMarkingNanoseconds nanoseconds;
   Qualifer_EventData_LaneMarking eventDataQualifier;
   LaneMarkingSeconds secondsLaneMarking;
   Qualifier_Extended_LaneMarking extendedQualifierLaneMarking;
   LaneMarkingTrafficState laneRoutingLaneMarking;
   SynchronisationStatusLaneMarking synchronizationStatusLaneMarking;
   IntegrityLaneBoundaries integrityLaneBoundaries;
   Rte_DT_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingList_7 dummy1;
} ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingList;

#define Rte_TypeDef_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingNext
typedef struct
{
   SubsistenceProbabilitylaneMarkingEgo subsitenceProbabilityLaneMarkingNextRightHand;
   SubsistenceProbabilitylaneMarkingEgo subsitenceProbabilityLaneMarkingNextLeftHand;
   StatusMeasurmentlaneMarkingEgo statusMeasurmentLaneMarkingNextRightHand;
   StatusMeasurmentlaneMarkingEgo statusMeasurmentLaneMarkingNextLeftHand;
   TypelaneMarkingEgo typeLaneMarkingNextRightHand;
   TypelaneMarkingEgo typeLaneMarkingNextLeftHand;
   RangeLaneMarkingEgo rangeLaneMarkingNextLeftHand;
   Coefficient0LaneMarkingEgo coefficient0LaneMarkingNextLeftHand;
   Coefficient1LaneMarkingEgo coefficient1LaneMarkingNextLeftHand;
   Coefficient2LaneMarkingEgo coefficient2LaneMarkingNextLeftHand;
   Coefficient3LaneMarkingNext coefficient3LaneMarkingNextLeftHand;
   RangeLaneMarkingEgo rangeLaneMarkingNextRightHand;
   Coefficient0LaneMarkingEgo coefficient0LaneMarkingNextRightHand;
   Coefficient1LaneMarkingEgo coefficient1LaneMarkingNextRightHand;
   Coefficient2LaneMarkingEgo coefficient2LaneMarkingNextRightHand;
   Coefficient3LaneMarkingNext coefficient3LaneMarkingNextRightHand;
   Rte_DT_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingNext_16 dummy1;
} ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingNext;

#define Rte_TypeDef_WarnBrakeCoordinationStruct
typedef struct
{
   QualifierFunctionBrakeChain qualifierFunctionBrakeChain;
   QualifierFunctionWarningChain qualifierFunctionWarningChain;
   QualifierFunctionPreX qualifierFunctionPreX;
   QualifierFunctionDBCParametrisation qualifierFunctionDBCParametrisation;
   StatusWarnBrakeCoordinatorStruct statusWarnBrakeCoordinator;
   WarnBrakeCoordinationStruct_functionForewarningFrontalProtectionSystem functionForewarningFrontalProtectionSystem;
   WarnBrakeCoordinationStruct_functionAcuteWarningFrontalProtectionSystem functionAcuteWarningFrontalProtectionSystem;
   QualifierFunctionCoordinatorBrakeRequest qualifierFunctionCoordinatorBrakeRequest;
   StatusAccelerationWishDriverWarnBrakeCoordinatorEnum statusAccelerationWishDriverWarnBrakeCoordinator;
   CommonEventDataQualifier eventDataQualifier;
} WarnBrakeCoordinationStruct;

#define Rte_TypeDef_CHASSIS_TargetBrakingTorqueDriverProvision_targetBrakingTorqueDriverProvision
typedef struct
{
   QualifierActualValueBrakingTorqueSumDriversChoice qualifierActualValueBrakingTorqueSumDriversChoice;
   ActualValueBrakingTorqueSumDriversChoice actualValueBrakingTorqueSumDriversChoice;
   ActualValueBrakingTorqueSumDriversChoiceRaw actualValueBrakingTorqueSumDriversChoiceRaw;
   StatusBrakingDriverStruct statusBrakingDriver;
   CommonEventDataQualifier eventDataQulaifier;
} CHASSIS_TargetBrakingTorqueDriverProvision_targetBrakingTorqueDriverProvision;

#define Rte_TypeDef_CHASSIS_OrientationVehicleBody_orientationVehicleBody
typedef struct
{
   HeightLevelVehicleBodyRearAxle heightLevelVehicleBodyRearAxle;
   AnglePitchVehicleBodyRearAxle anglePitchVehicleBodyRearAxle;
   AngleRollVehicleBodyRearAxle angleRollVehicleBodyRearAxle;
   QualifierOrientationVehicleBody qualifierOrientationVehicleBody;
   CommonEventDataQualifier eventDataQulaifier;
} CHASSIS_OrientationVehicleBody_orientationVehicleBody;

#define Rte_TypeDef_ActiveFunctionIndicateDirectionStruct
typedef struct
{
   ActiveFunctionIndicateDirection activeFunctionIndicateDirection;
   CommonEventDataQualifier eventDataQualifier;
   ControlIndicateDirection controlIndicateDirection;
} ActiveFunctionIndicateDirectionStruct;

#define Rte_TypeDef_ErrorMemoryBordnetVoltageWrapper
typedef struct
{
   CTR_ERRM_BN_U controlErrorMemoryBordnetVoltage;
   ControlErrorMemoryHAFBordnetzVoltage controlErrorMemoryHAFBordnetzVoltage;
} ErrorMemoryBordnetVoltageWrapper;

#define Rte_TypeDef_FunctionalStatusBrakeControlSystemProvisionStruct
typedef struct
{
   UINT8 statusBrakingMSA;
   StatusUnitRunDSC statusUnitRunDSC;
   uint16 qualifierFunctionFDR;
   uint16 qualifierFunctionABS;
   uint16 qualifierFunctionASC;
   CommonEventDataQualifier eventDataQualifier;
} FunctionalStatusBrakeControlSystemProvisionStruct;

#define Rte_TypeDef_StatusDisplaySegmentExteriorMirrorWrapper
typedef struct
{
   StatusDisplaySegmentExteriorMirrorDriver statusDisplaySegmentExteriorMirrorDriver;
   StatusDisplaySegmentExteriorMirrorDriver statusDisplaySegmentExteriorMirrorPassenger;
} StatusDisplaySegmentExteriorMirrorWrapper;

#define Rte_TypeDef_CHASSIS_CentralOdometryProvision_slipSideAngleErrAmp
typedef struct
{
   SlipSideAngleErrAmp slipSideAngleErrAmp;
   CommonEventDataQualifier eventDataQualifier;
} CHASSIS_CentralOdometryProvision_slipSideAngleErrAmp;

#define Rte_TypeDef_CHASSIS_CentralOdometryProvision_curvatureAndSlipSideAngle
typedef struct
{
   Curvature curvature;
   SlipSideAngle slipSideAngle;
   QualifierSlipSideAngle qualifierSlipSideAngle;
   QualifierCurvature qualifierCurvature;
   CommonEventDataQualifier eventDataQualifier;
} CHASSIS_CentralOdometryProvision_curvatureAndSlipSideAngle;

#define Rte_TypeDef_POWERTRAIN_AngleAcceleratorPedal2_angleAcceleratorPedal
typedef struct
{
   ActualValueAngleAcceleratorPedal actualValueAngleAcceleratorPedal;
   QualifierActualValueAngleAcceleratorPedal qualifierActualValueAngleAcceleratorPedal;
   GradientActualValueAngleAcceleratorPedal gradientActualValueAngleAcceleratorPedal;
   CommonEventDataQualifier eventDataQulaifier;
} POWERTRAIN_AngleAcceleratorPedal2_angleAcceleratorPedal;

#define Rte_TypeDef_CHASSIS_ActualBrakingTorqueProvision_actualBrakingTorqueProvision
typedef struct
{
   QualifierActualValueBrakingTorqueSum qualifierActualValueBrakingTorqueSum;
   ActualValueBrakingTorqueSum actualValueBrakingTorqueSum;
   CommonEventDataQualifier eventDataQualifier;
} CHASSIS_ActualBrakingTorqueProvision_actualBrakingTorqueProvision;

#define Rte_TypeDef_CHASSIS_YawRateProvision_yawRate
typedef struct
{
   YawRate yawRate;
   YawRateErrAmp yawRateErrAmp;
   QualifierYawRate qualifierYawRate;
   CommonEventDataQualifier eventDataQualifier;
} CHASSIS_YawRateProvision_yawRate;

#define Rte_TypeDef_INFRASTRUCTURE_VehicleInformation_RelativeTimeBN2020
typedef struct
{
   UI_32_noIniVal_noDflVal_4294967295_1_0_noEnc_hasRng timeSecondCounterRelative;
   UI_16_noIniVal_noDflVal_65535_1_0_noEnc_hasRng_2 timeDayCounterAbsolute;
} INFRASTRUCTURE_VehicleInformation_RelativeTimeBN2020;

#define Rte_TypeDef_RangeWrapper
typedef struct
{
   MILE_KM mileageKilometre;
} RangeWrapper;

#define Rte_TypeDef_ChassisNumberWrapper
typedef struct
{
   NO_VECH_1 numberVehicleChassis1;
   NO_VECH_2 numberVehicleChassis2;
   NO_VECH_3 numberVehicleChassis3;
   NO_VECH_4 numberVehicleChassis4;
   NO_VECH_5 numberVehicleChassis5;
   NO_VECH_6 numberVehicleChassis6;
   NO_VECH_7 numberVehicleChassis7;
} ChassisNumberWrapper;
#define Rte_TypeDef_CHASSIS_SteeringAngleProvision_steeringAngleFrontAxle
typedef struct
{
   SteeringAngleFrontAxle steeringAngleFrontAxle;
   QualifierSteeringAngleFrontAxle qualifierSteeringAngleFrontAxle;
   SteeringAngleFrontAxleErrAmp steeringAngleFrontAxleErrAmp;
   CommonEventDataQualifier eventDataQualifier;
} CHASSIS_SteeringAngleProvision_steeringAngleFrontAxle;

#define Rte_TypeDef_CHASSIS_SteeringAngleProvision_steeringAngleDriver
typedef struct
{
   SteeringAngleDriver steeringAngleDriver;
   QualifierSteeringAngleDriver qualifierSteeringAngleDriver;
   CommonEventDataQualifier evenDataQualifier;
} CHASSIS_SteeringAngleProvision_steeringAngleDriver;

#define Rte_TypeDef_SteeringAngleCondition
typedef struct
{
   UINT8 steeringAngleCondition;
   CommonEventDataQualifier eventDataQualifier;
} SteeringAngleCondition;

#define Rte_TypeDef_WheelMomentDrive4Wrapper
typedef struct
{
   ST_INTF_DRASY_TAR_LIM statusInterfaceDriverAssistenceSystemTargetLimit;
   ST_DRVDIR_DVCH statusDrivingDirectionDriversChoice;
   QU_AVL_RPM_BAX_RED qualifierActualValueRPMBackAxleRedundant;
   RPM_GRB_NEGL_2 actualValueRPMBackAxleRedundant;
   ST_PENG_PT statusPositiveEngagementPowerTrain;
   ST_AVAI_INTV_PT_DRS statusAvailibilityInterventionPowerTrainDRS;
   CTR_ACTVN_PWHOP dummy1;
} WheelMomentDrive4Wrapper;

#define Rte_TypeDef_StatusCombustionEngineSecureWrapper
typedef struct
{
   StatusValvetrain statusValvetrain;
   StatusElectricMode dummyStatusElectricMode;
   ST_CENG_DRV StatusCombustionEngineDrive;
   ST_UDP StatusVoltageDrop;
   ST_ENERG_SUPY StatusEnergySupply;
   ST_MOT_DRV StatusEMotorDrive;
   ST_ANO_MSA_ENG_STOP StatusAnnouncementMSAEngineStop;
   TR_HHASS_MSA TriggerHillholdAssistantMSA;
   CTR_LED_PUBU_MSA ControlLEDPushButtonMSA;
   Rte_DT_StatusCombustionEngineSecureWrapper_9 Gap2;
} StatusCombustionEngineSecureWrapper;

#define Rte_TypeDef_MileageSupreme
typedef struct
{
   Coding_104861712 mileageSupreme;
   CommonEventDataQualifier qualifierMileageSupreme;
} MileageSupreme;

#define Rte_TypeDef_ExteriorTemperatureWrapper
typedef struct
{
   UI_8_noIniVal_noDflVal_255_0_50s_MI_40_noEnc_hasRng temperatureExterior;
} ExteriorTemperatureWrapper;

#define Rte_TypeDef_TrailerWrapper
typedef struct
{
   ST_TRAI statusTrailer;
} TrailerWrapper;

#define Rte_TypeDef_VehicleDynamicDataLongitudinal2Wrapper
typedef struct
{
   VehicleAccelerationLongitudinalEstimatedSK vehicleAccelerationLongitudinalEstimated;
   VehicleSpeedLongitudinal vehicleSpeedLongitudinal;
   QualifierVehicleAccelerationLongitudinalEstimatedSK qualifierVehicleAccelerationLongitudinalEstimated;
   QualifierVehicleAccelerationLongitudinalEstimatedSK qualifierVehicleSpeedLongitudinal;
   UINT8 dummy;
} VehicleDynamicDataLongitudinal2Wrapper;

#define Rte_TypeDef_StatusOccpantDetectionStruct
typedef struct
{
   StatusErrorSeatMatDR statusErrorSeatMatDR;
   StatusSeatOccupyingDR statusSeatOccupyingDR;
   StatusSeatOccupyingPS statusSeatOccupyingPS;
   StatusSeatOccupyingRLH statusSeatOccupyingRLH;
   StatusSeatOccupyingRRH statusSeatOccupyingRRH;
   StatusSeatOccupyingRM statusSeatOccupyingRM;
   CommonEventDataQualifier eventDataQualifier;
} StatusOccpantDetectionStruct;

#define Rte_TypeDef_ControlSettingLaneChangeWarningWrapper
typedef struct
{
   ControlSettingLaneChangeWarning controlSettingLaneChangeWarning;
} ControlSettingLaneChangeWarningWrapper;

#define Rte_TypeDef_ControlPrewarningWrapper
typedef struct
{
   ControlPrewarning controlPrewarning;
} ControlPrewarningWrapper;

#define Rte_TypeDef_ControlLaneChangeWarningWrapper
typedef struct
{
   ControlLaneChangeWarning controlLaneChangeWarning;
} ControlLaneChangeWarningWrapper;

#define Rte_TypeDef_ControlCrossTrafficBrakeWrapper
typedef struct
{
   ControlCrossTrafficBrake controlCrossTrafficBrake;
} ControlCrossTrafficBrakeWrapper;

#define Rte_TypeDef_ContolActiveSafetyWrapper
typedef struct
{
   ControlActiveSafety controlActiveSafety;
} ContolActiveSafetyWrapper;

#define Rte_TypeDef_INFOTAINMENT_ADASProtocol2_navGraph2CurrentSegment
typedef struct
{
   IDX_PRES_SEG_NAVGRPH_2 indexPresentSegmentNavGraph2;
   DIR_CHNG_ANG_SEG_NAVGRPH_2 directionChangeAngleSegmentNavGraph2;
   RAD_PRES_SEG_NAVGRPH_2 radiusPresentSegmentNavGraph2;
   QUAN_LNS_NAVGRPH_2 quantityLanesNavGraph2;
   TYP_STREET_NAVGRPH_2 typeStreetNavGraph2;
   L_SEG_NAVGRPH_2 lengthSegmentNavGraph2;
   PROP_MAP_NAVGRPH_2 propertyMapNavGraph2;
   LIM_V_NAVGRPH_2 limitSpeedNavGraph2;
} INFOTAINMENT_ADASProtocol2_navGraph2CurrentSegment;

#define Rte_TypeDef_StatusDoorLockWrapper
typedef struct
{
   StatusInsidePushButton statusInsidePushButtonDriverDoor;
   StatusInsidePushButton statusInsidePushButtonPassengerDoor;
   StatusInsidePushButton statusInsidePushButtonDriverDoorRear;
   StatusInsidePushButton statusInsidePushButtonPassengerDoorRear;
} StatusDoorLockWrapper;

#define Rte_TypeDef_StatusSeatBeltsStruct
typedef struct
{
   StatusBeltBuckleSwitchDR statusBeltBuckleSwitchDR;
   StatusBeltBuckleSwitchPS statusBeltBuckleSwitchPS;
   StatusBeltBuckleSwitchRLH statusBeltBuckleSwitchRLH;
   StatusBeltBuckleSwitchRRH statusBeltBuckleSwitchRRH;
   StatusBeltBuckleSwitchRM statusBeltBuckleSwitchRM;
   StatusBeltBuckleSwitch3LeftHandTierRearSide statusBeltBuckleSwitch3LeftHandTierRearSide;
   StatusBeltBuckleSwitch3RightHandTierRearSide statusBeltBuckleSwitch3RightHandTierRearSide;
   StatusBeltBuckleSwitch3MiddleTierRearSide statusBeltBuckleSwitch3MiddleTierRearSide;
   CommonEventDataQualifier eventDataQualifier;
} StatusSeatBeltsStruct;

#define Rte_TypeDef_ControlSafeExitSoundWrapper
typedef struct
{
   ControlSafeExitSound controlSafeExitSound;
} ControlSafeExitSoundWrapper;

#define Rte_TypeDef_ControlSafeExitWrapper
typedef struct
{
   ControlSafeExit controlSafeExit;
} ControlSafeExitWrapper;

#define Rte_TypeDef_DrivingDirection
typedef struct
{
   DrivingDirectionVehicleConfirmed drivingDirectionVehicleConfirmed;
   DrivingDirectionVehicleUnconfirmed drivingDirectionVehicleUnconfirmed;
} DrivingDirection;

#define Rte_TypeDef_VEHICLEDYNAMICS_VelocityAndVehicleConditionProvision_velocityVehicleLongitudinal
typedef struct
{
   VelocityVehicleLongitudinal velocityVehicleLongitudinal;
   QualifierVelocityVehicleLongitudinal qualifierVelocityVehicleLongitudinal;
   CommonEventDataQualifier eventDataQualifier;
} VEHICLEDYNAMICS_VelocityAndVehicleConditionProvision_velocityVehicleLongitudinal;

#define Rte_TypeDef_VEHICLEDYNAMICS_VelocityAndVehicleConditionProvision_velocityVehicle
typedef struct
{
   StatusVelocityNearStandstill statusVelocityNearStandstill;
   VelocityVehicleCog velocityVehicleCog;
   QualifierVelocityVehicleCog qualifierVelocityVehicleCog;
   CommonEventDataQualifier eventDataQualifier;
} VEHICLEDYNAMICS_VelocityAndVehicleConditionProvision_velocityVehicle;

#define Rte_TypeDef_VehicleConditionWrapper
typedef struct
{
   CTR_BS_PRTNT controlBasePartialNetworks;
   Anforderung_Funktionale_Teilnetze controlFunctionalPartialNetworks;
   ST_CON_VEH statusConditionVehicle;
   QU_ST_CON_VEH qualifierStatusConditionVehicle;
   Rte_DT_VehicleConditionWrapper_4 dummy;
} VehicleConditionWrapper;

#define Rte_TypeDef_YawSpeedVehicleWrapper
typedef struct
{
   UINT8 dummy;
   VYAW_VEH yawVelocityVehicle;
   VYAW_VEH_ERR_AMP yawVelocityVehicleErrorAmplitude;
   QU_VYAW_VEH qualifierYawVelocityVehicle;
} YawSpeedVehicleWrapper;

#define Rte_TypeDef_DisplayDriverAssistenceSystemWrapper
typedef struct
{
   AVL_V_SPDM_2 actualValueSpeedSpeedometer2;
   UN_V_SPDM unitOfMeasurementSpeedSpeedometer;
   AVAI_DISP_KI availibilityDisplayInstrumentPack;
   ST_DISP_DRASY_LAG statusDisplayDriverAssistenceSystemLongitudinalGuidance;
   ST_DISP_GAP_INFO_2 statusDisplayGapInformation2;
} DisplayDriverAssistenceSystemWrapper;

#define Rte_TypeDef_StatusDynamometerAndEndOfAssemblyLine
typedef struct
{
   StatusEndOfAssemblyLine statusEndOfAssemblyLine;
   StatusDynamometer statusDynamometer;
   CommonEventDataQualifier eventDataQualifier;
} StatusDynamometerAndEndOfAssemblyLine;

#define Rte_TypeDef_CHASSIS_AccelerationLongitudinalLateralProvision_vehicleAccelerationLongitudinalEstimated
typedef struct
{
   VehicleAccelerationLongitudinalEstimated vehicleAccelerationLongitudinalEstimated;
   QualifierVehicleAccelerationLongitudinalEstimated qualifierVehicleAccelerationLongitudinalEstimated;
   CommonEventDataQualifier eventDataQualifier;
} CHASSIS_AccelerationLongitudinalLateralProvision_vehicleAccelerationLongitudinalEstimated;

#define Rte_TypeDef_CHASSIS_AccelerationLongitudinalLateralProvision_accelerationLongitudinalCog
typedef struct
{
   AccelerationLongitudinalCog accelerationLongitudinalCog;
   AccelerationLongitudinalCogErrAmp accelerationLongitudinalCogErrAmp;
   QualifierAccelerationLongitudinalCentreOfGravity qualifierAccelerationLongitudinalCog;
   CommonEventDataQualifier eventDataQualifier;
} CHASSIS_AccelerationLongitudinalLateralProvision_accelerationLongitudinalCog;

#define Rte_TypeDef_CHASSIS_AccelerationLongitudinalLateralProvision_accelerationLateralCog
typedef struct
{
   AccelerationLateralCog accelerationLateralCog;
   AccelerationLateralCogErrAmp accelerationLateralCogErrAmp;
   QualifierAccelerationLateralCenterOfGravity qualifierAccelerationLateralCog;
   CommonEventDataQualifier eventDataQualifier;
} CHASSIS_AccelerationLongitudinalLateralProvision_accelerationLateralCog;

#define Rte_TypeDef_INFRASTRUCTURE_ActiveCommunicationRequest_ActiveCommunicationRequest
typedef struct
{
   ActiveCommunicationRequestCauseEnum activeCommunicationRequestCause;
   UINT8 activeCommunicationRequestEcuId;
} INFRASTRUCTURE_ActiveCommunicationRequest_ActiveCommunicationRequest;

typedef struct
{
   StatusStageDrive statusActualStageDrive;
   StatusActualGearDrive statusActualGearDrive;
   Rte_DT_StatusGearboxDriveWrapper_2 dummy1;
   StatusStageDrive statusTargetStageDrive;
   StatusGearAvailabilityDrive dummy2;
   Qualifier_StatusActualStageDrive qualifierStatusActualStageDrive;
   Rte_DT_StatusGearboxDriveWrapper_6 dummy3;
} StatusGearboxDriveWrapper;

typedef struct
{
   StatusBrakingDriverStruct statusBrakingDriver;
   CommonEventDataQualifier eventDataQualifier;
} CHASSIS_TargetBrakingTorqueDriverProvision_statusBrakingDriverProvision;

typedef struct
{
   ControlSetting2MUTMDrive controlSetting2MUTMDrive;
   ControlSetting2FahrmodusMDrive controlSetting2FahrmodusMDrive;
   ControlSetting2RECWMDrive controlSetting2RECWMDrive;
} ControlSetting2MDriveWrapper;

typedef struct
{
   StatusDoorSwitchDRD statusDoorSwitchDRD;
   StatusDoorSwitchPSD statusDoorSwitchPSD;
   StatusDoorSwitchDRDR statusDoorSwitchDRDR;
   StatusDoorSwitchPSDR statusDoorSwitchPSDR;
   CommonEventDataQualifier eventDataQualifier;
} StatusContactDoorsStruct;

typedef struct Some_Ip_RX_Signals_Tag {
   INFRASTRUCTURE_ActiveCommunicationRequest_ActiveCommunicationRequest Appl_ActiveCommunicationRequest;
   CHASSIS_AccelerationLongitudinalLateralProvision_accelerationLateralCog Appl_BCP21_GW_CHASSIS_AccelerationLongitudinalLateralProvision_accelerationLateralCog;
   CHASSIS_AccelerationLongitudinalLateralProvision_accelerationLongitudinalCog Appl_BCP21_GW_CHASSIS_AccelerationLongitudinalLateralProvision_accelerationLongitudinalCog;
   CHASSIS_AccelerationLongitudinalLateralProvision_vehicleAccelerationLongitudinalEstimated Appl_BCP21_GW_CHASSIS_AccelerationLongitudinalLateralProvision_vehicleAccelerationLongitudinalEstimated;
   StatusDynamometerAndEndOfAssemblyLine Appl_BCP21_GW_StatusDynamometerAndEndOfAssemblyLine;
   DisplayDriverAssistenceSystemWrapper Appl_BCP21_GW_DisplayDriverAssistenceSystemWrapper;
   YawSpeedVehicleWrapper Appl_YawSpeedVehicleWrapper;
   VehicleConditionWrapper Appl_VehicleConditionWrapper;
   VEHICLEDYNAMICS_VelocityAndVehicleConditionProvision_velocityVehicle Appl_BCP21_GW_VEHICLEDYNAMICS_VelocityAndVehicleConditionProvision_velocityVehicle;
   VEHICLEDYNAMICS_VelocityAndVehicleConditionProvision_velocityVehicleLongitudinal Appl_BCP21_GW_VEHICLEDYNAMICS_VelocityAndVehicleConditionProvision_velocityVehicleLongitudinal;
   DrivingDirection Appl_BCP21_GW_DrivingDirection;
   ControlSafeExitWrapper Appl_BCP21_GW_ControlSafeExitWrapper;
   ControlSafeExitSoundWrapper Appl_BCP21_GW_ControlSafeExitSoundWrapper;
   StatusSeatBeltsStruct Appl_BCP21_GW_StatusSeatBeltsStruct;
   StatusDoorLockWrapper Appl_BCP21_GW_StatusDoorLockWrapper;
   INFOTAINMENT_ADASProtocol2_navGraph2CurrentSegment Appl_BCP21_GW_NavGraph2CurrentSegmentWrapper;
   ContolActiveSafetyWrapper Appl_BCP21_GW_ContolActiveSafetyWrapper;
   ControlCrossTrafficBrakeWrapper Appl_BCP21_GW_ControlCrossTrafficBrakeWrapper;
   ControlLaneChangeWarningWrapper Appl_BCP21_GW_ControlLaneChangeWarningWrapper;
   ControlPrewarningWrapper Appl_BCP21_GW_ControlPrewarningWrapper;
   ControlSettingLaneChangeWarningWrapper Appl_BCP21_GW_ControlSettingLaneChangeWarningWrapper;
   StatusOccpantDetectionStruct Appl_BCP21_GW_StatusOccpantDetectionStruct;
   VehicleDynamicDataLongitudinal2Wrapper Appl_BCP21_GW_VehicleDynamicDataLongitudinal2Wrapper;
   TrailerWrapper Appl_BCP21_GW_TrailerWrapper;
   ExteriorTemperatureWrapper Appl_BCP21_GW_ExteriorTemperatureWrapper;
   MileageSupreme Appl_BCP21_GW_MileageSupreme;
   StatusCombustionEngineSecureWrapper Appl_BCP21_GW_StatusCombustionEngineWrapper;
   WheelMomentDrive4Wrapper Appl_BCP21_GW_WheelMomentDrive4Wrapper;
   SteeringAngleCondition Appl_BCP21_GW_SteeringAngleCondition;
   CHASSIS_SteeringAngleProvision_steeringAngleDriver Appl_BCP21_GW_CHASSIS_SteeringAngleProvision_steeringAngleDriver;
   CHASSIS_SteeringAngleProvision_steeringAngleFrontAxle Appl_BCP21_GW_CHASSIS_SteeringAngleProvision_steeringAngleFrontAxle;
   CHASSIS_TargetBrakingTorqueDriverProvision_targetBrakingTorqueDriverProvision Appl_BCP21_GW_CHASSIS_TargetBrakingTorqueDriverProvision;
   ChassisNumberWrapper Appl_BCP21_GW_ChassisNumberWrapper;
   RangeWrapper Appl_BCP21_GW_RangeWrapper;
   INFRASTRUCTURE_VehicleInformation_RelativeTimeBN2020 Appl_BCP21_GW_INFRASTRUCTURE_VehicleInformation_RelativeTimeBN2020;
   CHASSIS_YawRateProvision_yawRate Appl_BCP21_GW_CHASSIS_YawRateProvision_yawRate;
   CHASSIS_ActualBrakingTorqueProvision_actualBrakingTorqueProvision Appl_BCP21_GW_CHASSIS_ActualBrakingTorqueProvision_actualBrakingTorqueProvision;
   POWERTRAIN_AngleAcceleratorPedal2_angleAcceleratorPedal Appl_BCP21_GW_POWERTRAIN_AngleAcceleratorPedal2_angleAcceleratorPedal;
   CHASSIS_CentralOdometryProvision_curvatureAndSlipSideAngle Appl_BCP21_GW_CHASSIS_CentralOdometryProvision_curvatureAndSlipSideAngle;
   CHASSIS_CentralOdometryProvision_slipSideAngleErrAmp Appl_BCP21_GW_CHASSIS_CentralOdometryProvision_slipSideAngleErrAmp;
   StatusDisplaySegmentExteriorMirrorWrapper Appl_BCP21_GW_StatusDisplaySegmentExteriorMirrorWrapper;
   FunctionalStatusBrakeControlSystemProvisionStruct Appl_BCP21_GW_FunctionalStatusBrakeControlSystemProvisionStruct;
   ErrorMemoryBordnetVoltageWrapper Appl_BCP21_GW_ErrorMemoryBordnetVoltageWrapper;
   ActiveFunctionIndicateDirectionStruct Appl_BCP21_GW_ActiveFunctionIndicateDirectionStruct;
   CHASSIS_OrientationVehicleBody_orientationVehicleBody Appl_BCP21_GW_CHASSIS_OrientationVehicleBody_orientationVehicleBody;
   CHASSIS_TargetBrakingTorqueDriverProvision_targetBrakingTorqueDriverProvision Appl_BCP21_GW_CHASSIS_TargetBrakingTorqueDriverProvision_targetBrakingTorqueDriverProvision;
   WarnBrakeCoordinationStruct Appl_BCP21_GW_WarnBrakeCoordinationStruct;
   RequestCTBWrapper Appl_BCP21_GW_RequestCTBWrapper_Setter;
   RequestCTBWrapper Appl_BCP21_GW_RequestCTBWrapper_Setter_Response;
   RequestCTBWrapper Appl_BCP21_GW_RequestCTBWrapper_Notifier;
   StatusContactDoorsStruct Appl_BCP21_GW_StatusContactDoorsStruct;
   ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoLeftHandGeometry Appl_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoLeftHandGeometry;
   ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoLeftHandProperty Appl_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoLeftHandProperty;
   ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoRightHandGeometry Appl_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoRightHandGeometry;
   ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoRightHandProperty Appl_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingEgoRightHandProperty;
   ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingExtensionLeftRight Appl_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingExtensionLeftRight;
   ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingList Appl_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingList;
   ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingNext Appl_ENVIRONMENTALMODEL_LaneBoundariesLegacy2_laneMarkingNext;
   StatusGearboxDriveWrapper App_StatusGearboxDriveWrapper;
   CHASSIS_TargetBrakingTorqueDriverProvision_statusBrakingDriverProvision App_CHASSIS_TargetBrakingTorqueDriverProvision_statusBrakingDriverProvision;
   ControlSetting2MDriveWrapper Appl_ControlSetting2MDriveWrapper;
} RECU_Some_Ip_RX_Signals_T;

typedef struct Some_IP_RX_Signal_Symbol_tag {
   uint16_t version;
   uint32_t size;
   RECU_Some_Ip_RX_Signals_T some_IP_RX_Signals;
} DC_Some_IP_RX_Signal_Symbol_t;

#endif /*_SOME_IP_INPUT_H_*/

/*=======================================================================================================*\
* File Revision History (top to bottom: first revision to last revision)
*=======================================================================================================

* 08-JAN-2020	qj4jv7		DET-110 Split the RECU RESIM SOMIP current header file into input and output header files
* 09-OCT-2023   sj9zp3      changed type of BOOL to overcome redefinition error

*\*========================================================================================================*/