//
// File: SpeedCompensation.cpp
//
// Code generated for Simulink model 'SpeedCompensation'.
//
// Model version                  : 1.486
// Simulink Coder version         : 9.0 (R2018b) 24-May-2018
// C/C++ source code generated on : Tue Jan 10 23:54:56 2023
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
#include "SpeedCompensation.h"
#include "SpeedCompensation_private.h"

// Named constants for MATLAB Function: '<S4>/MATLAB Function'
#define SpeedCompensation_input_rate_within_limit_maturation_time (0.05F)
#define SpeedCompensation_lower_speed_rate_lim                    (-10.0F)
#define SpeedCompensation_speed_comp_vse_sampling_time            (0.05F)
#define SpeedCompensation_upper_speed_rate_lim                    (8.0F)

// Output and update for referenced model: 'SpeedCompensation'
void SpeedCompensationModelClass::step(const real32_T *rtu_raw_speed_mps, const enum_quality_factor_T *rtu_raw_speed_qf,
                                       const real32_T *rtu_VehicleCompFactor_KA,
                                       const enum_quality_factor_T *rtu_VehicleCompFactor_KA_QF,
                                       const real32_T *rtu_gps_comp_factor, const boolean_T *rtu_f_use_gps_comp_factor,
                                       real32_T *rty_filt_veh_speed_over_ground,
                                       enum_quality_factor_T *rty_filt_veh_speed_over_ground_qf)
{
   real32_T rate;
   real32_T input_rate;
   boolean_T guard1 = false;

   // MATLAB Function: '<S4>/MATLAB Function'
   if (!SpeedCompensation_DW.bitsForTID0.last_val_out_not_empty)
   {
      SpeedCompensation_DW.last_val_out                       = *rtu_raw_speed_mps;
      SpeedCompensation_DW.bitsForTID0.last_val_out_not_empty = true;
      SpeedCompensation_DW.last_val_in                        = *rtu_raw_speed_mps;
   }

   rate       = ((*rtu_raw_speed_mps) - SpeedCompensation_DW.last_val_out) / SpeedCompensation_speed_comp_vse_sampling_time;
   input_rate = ((*rtu_raw_speed_mps) - SpeedCompensation_DW.last_val_in) / SpeedCompensation_speed_comp_vse_sampling_time;
   guard1     = false;
   if (rate > SpeedCompensation_upper_speed_rate_lim)
   {
      rate   = 0.4F + SpeedCompensation_DW.last_val_out;
      guard1 = true;
   }
   else if (rate < SpeedCompensation_lower_speed_rate_lim)
   {
      rate   = -0.5F + SpeedCompensation_DW.last_val_out;
      guard1 = true;
   }
   else
   {
      rate                                                    = *rtu_raw_speed_mps;
      SpeedCompensation_DW.last_val_out                       = *rtu_raw_speed_mps;
      SpeedCompensation_DW.last_val_in                        = *rtu_raw_speed_mps;
      SpeedCompensation_DW.time_check_input_rate_within_limit = 0.0F;
   }

   if (guard1)
   {
      if ((input_rate < SpeedCompensation_upper_speed_rate_lim) && (input_rate > SpeedCompensation_lower_speed_rate_lim))
      {
         SpeedCompensation_DW.time_check_input_rate_within_limit += SpeedCompensation_speed_comp_vse_sampling_time;
      }
      else
      {
         SpeedCompensation_DW.time_check_input_rate_within_limit = 0.0F;
      }

      if (SpeedCompensation_DW.time_check_input_rate_within_limit > SpeedCompensation_input_rate_within_limit_maturation_time)
      {
         rate = (0.388984501F * SpeedCompensation_DW.last_val_out) + (0.611015499F * (*rtu_raw_speed_mps));
      }

      SpeedCompensation_DW.last_val_out = rate;
      SpeedCompensation_DW.last_val_in  = *rtu_raw_speed_mps;
   }

   // End of MATLAB Function: '<S4>/MATLAB Function'

   // Switch: '<S1>/Switch2' incorporates:
   //   Constant: '<S5>/Constant'
   //   Logic: '<S2>/Logical Operator1'
   //   MinMax: '<S3>/Min'
   //   RelationalOperator: '<S5>/Compare'
   //   Switch: '<S1>/Switch1'

   if ((*rtu_f_use_gps_comp_factor) && (rate < 15.0F))
   {
      *rty_filt_veh_speed_over_ground_qf = *rtu_raw_speed_qf;

      // Switch: '<S2>/Switch1' incorporates:
      //   Product: '<S2>/Product1'

      *rty_filt_veh_speed_over_ground = (*rtu_gps_comp_factor) * rate;
   }
   else
   {
      if ((*rtu_VehicleCompFactor_KA_QF) < (*rtu_raw_speed_qf))
      {
         // MinMax: '<S3>/Min'
         *rty_filt_veh_speed_over_ground_qf = *rtu_VehicleCompFactor_KA_QF;
      }
      else
      {
         *rty_filt_veh_speed_over_ground_qf = *rtu_raw_speed_qf;
      }

      // Switch: '<S3>/Switch' incorporates:
      //   Constant: '<S6>/Constant'
      //   Constant: '<S7>/Constant'
      //   Logic: '<S3>/Logical Operator'
      //   Product: '<S3>/Product'
      //   RelationalOperator: '<S6>/Compare'
      //   RelationalOperator: '<S7>/Compare'

      if ((((uint32_T)(*rtu_VehicleCompFactor_KA_QF)) == ACCURATED) ||
          (((uint32_T)(*rtu_VehicleCompFactor_KA_QF)) == NOT_ACCURATED))
      {
         *rty_filt_veh_speed_over_ground = rate * (*rtu_VehicleCompFactor_KA);
      }
      else
      {
         *rty_filt_veh_speed_over_ground = rate;
      }

      // End of Switch: '<S3>/Switch'
   }

   // End of Switch: '<S1>/Switch2'
}

// Constructor
SpeedCompensationModelClass::SpeedCompensationModelClass()
{
   // Currently there is no constructor body generated.
}

// Destructor
SpeedCompensationModelClass::~SpeedCompensationModelClass()
{
   // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
