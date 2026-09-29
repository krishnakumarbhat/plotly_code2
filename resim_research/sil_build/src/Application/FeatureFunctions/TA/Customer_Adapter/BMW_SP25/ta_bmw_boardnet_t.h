#ifndef TA_BMW_BOARDNET_T_H
#define TA_BMW_BOARDNET_T_H

/* project specific includes */
#include "pa_reuse.h"
#include "ta_bmw_enums.h"

/**
 * This structure holds TA relevant BMW boardnet signals
 */
typedef struct
{
   float32_T acceleration_longitudinal_cog;           /* Laengsbeschleunigung_Schwerpunkt */
   float32_T acceleration_longitudinal_cog_qualifier; /* Qualifier_Laengsbeschleunigung_Schwerpunkt */
   float32_T acceleration_lateral_cog;                /* Querbeschleunigung_Schwerpunkt */
   float32_T acceleration_lateral_cog_qualifier;      /* Qualifier_Querbeschleunigung_Schwerpunkt */
   float32_T acceleration_driving_direction;          /* Laengsbeschleunigung Fahrtrichtung (2) */

   float32_T status_braking_driver; /* Status_Bremsung_Fahrer */

   float32_T accelerator_pedal_angle;           /* Ist_Winkel_Fahrpedal */
   float32_T accelerator_pedal_angle_gradient;  /* Gradient_Ist_Winkel_Fahrpedal */
   float32_T accelerator_pedal_angle_qualifier; /* Qualifier_Ist_Winkel_Fahrpedal */

   float32_T ego_trajectory_curvature;           /* Ist_Kruemmung_Fahrzeugbewegung */
   float32_T ego_trajectory_curvature_qualifier; /* Qualifier_Ist_Kruemmung_Fahrzeugbewegung */

   float32_T control_prewarning;          /* Steuerung_Option_Vorwarnung */
   float32_T control_active_safety;       /* Steuerung AEB on/off */
   float32_T control_move_off_prevention; /* Steuerung_Option_Anfahrverhinderung*/

   float32_T driving_direction_vehicle_confirmed; /* Status_Fahrtrichtung_Fahrzeug - StDrivDirVeh */

   float32_T status_warn_brake_coordinator;    /* Status_Warnbremskoordinator */
   float32_T qualifier_function_brake_chain;   /* Qualifier_Function_BrakeChain */
   float32_T qualifier_function_warning_chain; /* Qualifier_Function_WarningChain */

   float32_T steering_angle_driver;               /* Ist_Lenkwinkel_Fahrer */
   float32_T steering_angle_driver_qualifier;     /* Qualifier_Ist_Lenkwinkel_Fahrer */
   float32_T steering_angle_front_axle;           /* Lenkwinkel_Vorderachse_effektiv */
   float32_T steering_angle_front_axle_qualifier; /* Qualifier_Lenkwinkel_Vorderachse */
   float32_T steering_angle_front_axle_condition; /* Status_Zustand_Lenkwinkel_2  / Status condition steering angle */

   float32_T status_condition_vehicle;           /* Status_Zustand_Fahrzeug */
   float32_T status_condition_vehicle_qualifier; /* Qualifier_Status_Zustand_Fahrzeug */

   float32_T velocity_vehicle_longitudinal;           /* Geschwindigkeit_Fahrzeug_Laengs */
   float32_T velocity_vehicle_longitudinal_qualifier; /* Qualifier_Geschwindigkeit_Fahrzeug_Laengs */

   float32_T yawrate;           /* Giergeschwindigkeit_Fahrzeug */
   float32_T yawrate_qualifier; /* Qualifier_Giergeschwindigkeit_Fahrzeug */

   float32_T status_control_longitudinal_guidance;    /* Status_Regelung_Laengsfuehrung */
   float32_T status_interface_driverassistance_limit; /* Status_Schnittstelle_Fahrerassistenzsystem_Soll_Begrenzung */

   Ta_Bmw_Hmi_Warntrigger_T ta_warntrigger_hmi; /* RTA / TAP warntrigger chosen by driver (early, middle, late) that is also
                                                         relevant for LCW */
} Ta_BMW_Boardnet_T;


/**
 * @brief Getter function for boardnet pointer
 *
 * @return Pointer to boardnet struct
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-8597}
 * @verification{}
 */
Ta_BMW_Boardnet_T *Ta_Get_Ta_Bmw_Boardnet_Ptr(void);

#endif /* TA_BMW_BOARDNET_T_H */
