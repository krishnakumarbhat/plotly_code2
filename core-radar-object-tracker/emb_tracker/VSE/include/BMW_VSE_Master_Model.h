//
// File: BMW_VSE_Master_Model.h
//
// Code generated for Simulink model 'BMW_VSE_Master_Model'.
//
// Model version                  : 1.495
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Tue Jan 10 23:55:40 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM 64-bit (LLP64)
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
#ifndef RTW_HEADER_BMW_VSE_Master_Model_h_
   #define RTW_HEADER_BMW_VSE_Master_Model_h_
   #ifndef BMW_VSE_Master_Model_COMMON_INCLUDES_
      #define BMW_VSE_Master_Model_COMMON_INCLUDES_
      #include "rtwtypes.h"
   #endif // BMW_VSE_Master_Model_COMMON_INCLUDES_

   #include "BMW_VSE_Master_Model_types.h"

   // Child system includes
   #include "Acceleration_VCS.h"
   #include "SpeedCompensation.h"
   #include "Speed_PlausibilityCheck.h"
   #include "SteeringWhlAngle_PlausibilityCheck.h"
   #include "VehicleCurvatureAndSideslipEstimation_Simulink.h"
   #include "YawRateCompensation.h"
   #include "YawRateEstimationUsingSteeringWheelAngle.h"
   #include "YawRate_PlausibilityCheck.h"

// Macros for accessing real-time model data structure

// Block signals and states (default storage) for system '<Root>'
typedef struct
{
   struct
   {
      uint_T is_c2_BMW_VSE_Master_Model:2;        // '<S4>/Chart'
      uint_T is_active_c2_BMW_VSE_Master_Model:1; // '<S4>/Chart'
      uint_T f_initialize_vse:1;                  // '<S4>/Chart'
   } bitsForTID0;
} DW_BMW_VSE_Master_Model_T;

// Constant parameters (default storage)
typedef struct
{
   // Pooled Parameter (Expression: 1)
   //  Referenced by:
   //    '<S1>/default_gps_comp_factor'
   //    '<S3>/Constant1'

   real32_T pooled1;

   // Pooled Parameter (Expression: 0)
   //  Referenced by:
   //    '<S1>/Constant4'
   //    '<S1>/default_f_use_gps_comp_factor'

   boolean_T pooled2;
} ConstP_BMW_VSE_Master_Model_T;

// External inputs (root inport signals with default storage)
typedef struct
{
   VCAN_VSE VCAN_VSE_p;                     // '<Root>/VCAN_VSE'
   Tracker_VSE Tracker_VSE_j;               // '<Root>/Tracker_VSE'
   Vehicle_Parameters Vehicle_Parameters_f; // '<Root>/Vehicle_Parameters'
   uint64_T System_Time_Micro_Sec;          // '<Root>/System_Time_Micro_Sec'
} ExtU_BMW_VSE_Master_Model_T;

// External outputs (root outports fed by signals with default storage)
typedef struct
{
   VSE_OUT VSE_Output;  // '<Root>/VSE_Output'
   VSE_RESIM VSE_Resim; // '<Root>/VSE_Resim'
} ExtY_BMW_VSE_Master_Model_T;

// Constant parameters (default storage)
extern const ConstP_BMW_VSE_Master_Model_T BMW_VSE_Master_Model_ConstP;

// Class declaration for model BMW_VSE_Master_Model
class BMW_VSE_Master_ModelModelClass
{
   // public data and function members
 public:
   // External inputs
   ExtU_BMW_VSE_Master_Model_T BMW_VSE_Master_Model_U;

   // External outputs
   ExtY_BMW_VSE_Master_Model_T BMW_VSE_Master_Model_Y;

   // model initialize function
   void initialize();

   // model step function
   void step();

   // Constructor
   BMW_VSE_Master_ModelModelClass();

   // Destructor
   ~BMW_VSE_Master_ModelModelClass();

   // private data and function members
 private:
   // Block signals and states
   DW_BMW_VSE_Master_Model_T BMW_VSE_Master_Model_DW;

   // model instance variable for '<S1>/Acceleration_VCS'
   AccelerationRearAxleModelClass Acceleration_VCSMDLOBJ1;

   // model instance variable for '<S1>/SpeedCompensation'
   SpeedCompensationModelClass SpeedCompensationMDLOBJ2;

   // model instance variable for '<S1>/Speed_PlausibilityCheck'
   Speed_PlausibilityCheckModelClass Speed_PlausibilityCheckMDLOBJ3;

   // model instance variable for '<S1>/SteeringWhlAngle_PlausibilityCheck'
   SteeringWhlAngle_PlausibilityCheckModelClass SteeringWhlAngle_PlausibilityCheckMDLOBJ4;

   // model instance variable for '<S1>/VehicleCurvatureAndSideslipEstimation'
   VehicleCurvatureAndSideslipEstimation_SimulinkModelClass VehicleCurvatureAndSideslipEstimationMDLOBJ5;

   // model instance variable for '<S1>/YawRateCompensation'
   YawRateCompensationModelClass YawRateCompensationMDLOBJ6;

   // model instance variable for '<S1>/YawRateEstimationUsingSteeringWheelAngle'
   YawRateEstimationUsingSteeringWheelAngleModelClass YawRateEstimationUsingSteeringWheelAngleMDLOBJ7;

   // model instance variable for '<S1>/YawRate_PlausibilityCheck'
   YawRate_PlausibilityCheckModelClass YawRate_PlausibilityCheckMDLOBJ8;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S2>/Data Type Conversion' : Eliminate redundant data type conversion

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'BMW_VSE_Master_Model'
//  '<S1>'   : 'BMW_VSE_Master_Model/VSE_Master_Model'
//  '<S2>'   : 'BMW_VSE_Master_Model/VSE_Master_Model/InputWrapper'
//  '<S3>'   : 'BMW_VSE_Master_Model/VSE_Master_Model/OutputWrapper'
//  '<S4>'   : 'BMW_VSE_Master_Model/VSE_Master_Model/VehicleIndexCalculation'
//  '<S5>'   : 'BMW_VSE_Master_Model/VSE_Master_Model/VehicleIndexCalculation/Chart'

#endif // RTW_HEADER_BMW_VSE_Master_Model_h_

//
// File trailer for generated code.
//
// [EOF]
//
