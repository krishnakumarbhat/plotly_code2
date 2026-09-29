/**
 * @file recw_output_debouncer.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the RECW output debouncer source file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "recw_output_debouncer.h"
#include "fbk_macros.h"
#include "ml_saturated_math.h"
#include "pa_reuse.h"
#include "recw.h"
#include "recw_types.h"
#include <assert.h>

/*===========================================================================*\
 * Local Functions Prototypes
\*===========================================================================*/

/**
 * @brief Processes alert qualification for RECW
 *
 * @return void
 *
 * @SRS{SF-1710}
 * @SAE{SF-2959}
 * @SDD{SF-7930}
 * @verification{}
 */
static void Recw_Apply_Alert_Qualification(Recw_Core_Output_T *p_core_output /**< Core output */,
                                           Recw_Persistent_T *p_persistent /**<Recw persistent data*/,
                                           const Recw_Core_Calibration_T *p_cals /**< Recw calibration*/);

/**
 * @brief Processes alert holding for RECW
 *
 * @return void
 *
 * @SRS{SF-1711}
 * @SAE{SF-2959}
 * @SDD{SF-7929}
 * @verification{}
 */
static void Recw_Apply_Alert_Holding(Recw_Core_Output_T *p_core_output /**< Core output */,
                                     Recw_Persistent_T *p_persistent /**<Recw persistent data*/,
                                     const Recw_Core_Calibration_T *p_cals /**< Recw calibration*/);

/**
 * @brief Checks if the maximal alert duration is exceeded and if true resets the alert
 *
 * @return void
 *
 * @SRS{SF-1712}
 * @SAE{SF-2959}
 * @SDD{SF-7928}
 * @verification{}
 */
static void Recw_Apply_Alert_Duration_Check(Recw_Core_Output_T *p_core_output /**< Core output */,
                                            Recw_Persistent_T *p_persistent /**<Recw persistent data*/,
                                            const Recw_Core_Calibration_T *p_cals /**< Recw calibration*/);


/*===========================================================================*\
* Global Function Defintions
\*===========================================================================*/

void Recw_Debounce_Alert_Level(Recw_Core_Output_T *p_core_output, Recw_Persistent_T *p_persistent, const Recw_Core_Calibration_T *p_cals)
{
   /* Asserts */
   assert(NULL != p_core_output);
   assert(NULL != p_persistent);
   assert(NULL != p_cals);

   /* Qualifying alert */
   Recw_Apply_Alert_Qualification(p_core_output, p_persistent, p_cals);

   /* Holding alert */
   Recw_Apply_Alert_Holding(p_core_output, p_persistent, p_cals);

   /* Check for maximal allowed warning duration */
   Recw_Apply_Alert_Duration_Check(p_core_output, p_persistent, p_cals);

   /* Save persistent data for next cycle*/
   p_persistent->recw_alert_prev_cycle = p_core_output->recw_alert_level;
   p_persistent->recw_id_prev_cycle    = p_core_output->recw_id;
   p_persistent->recw_index_prev_cycle = p_core_output->recw_index;
}

/*===========================================================================*\
 * Local Function Definitions
\*===========================================================================*/

static void Recw_Apply_Alert_Qualification(Recw_Core_Output_T *p_core_output,
                                           Recw_Persistent_T *p_persistent,
                                           const Recw_Core_Calibration_T *p_cals)
{
   /* Asserts */
   assert(NULL != p_core_output);
   assert(NULL != p_persistent);
   assert(NULL != p_cals);

   if (RECW_ALERT_ACTIVE_LEVEL_1 == p_core_output->recw_alert_level)
   {
      /* Increase qualifying counter */
      Sat_Inc_Uint8(&(p_persistent->recw_alert_qualifying_counter));

      if ((p_core_output->recw_id != p_persistent->recw_id_prev_cycle)
          && (p_persistent->recw_alert_qualifying_counter <= p_cals->k_recw_alert_qualifying_cycles))
      {
         /* Qualifying counter below threshold, thus suppress this alert */
         Recw_Reset_Core_Output(p_core_output);
      }
   }
   else if (RECW_ALERT_ACTIVE_LEVEL_2 == p_core_output->recw_alert_level)
   {
      /* Optionally check if alert levels are consecutive */
      if (Fbk_Is_True(p_cals->k_recw_f_only_allow_consecutive_alert_levels))
      {
         int32_t alert_level_delta = ((int32_t) p_core_output->recw_alert_level) - ((int32_t) p_persistent->recw_alert_prev_cycle);

         if (alert_level_delta > 1)
         {
            /* Alert levels not consecutive, thus suppress this alert */
            Recw_Reset_Core_Output(p_core_output);
         }
      }
   }
   else
   {
      /* No active alert in this cycle, thus reset qualification counter */
      p_persistent->recw_alert_qualifying_counter = FBK_ZERO_UINT;
   }
}

static void Recw_Apply_Alert_Holding(Recw_Core_Output_T *p_core_output,
                                     Recw_Persistent_T *p_persistent,
                                     const Recw_Core_Calibration_T *p_cals)
{
   /* Asserts */
   assert(NULL != p_core_output);
   assert(NULL != p_persistent);
   assert(NULL != p_cals);

   if (p_core_output->recw_alert_level < p_persistent->recw_alert_prev_cycle)
   {
      /* Increase holding counter */
      Sat_Inc_Uint8(&(p_persistent->recw_alert_holding_counter));

      if (((RECW_ALERT_ACTIVE_LEVEL_1 == p_persistent->recw_alert_prev_cycle)
           && (p_persistent->recw_alert_holding_counter <= p_cals->k_recw_alert_holding_cycles[RECW_INDEX_ALERT_LEVEL_1]))
          || ((RECW_ALERT_ACTIVE_LEVEL_2 == p_persistent->recw_alert_prev_cycle)
              && (p_persistent->recw_alert_holding_counter <= p_cals->k_recw_alert_holding_cycles[RECW_INDEX_ALERT_LEVEL_2])))
      {
         /* Holding counter below threshold, thus hold RECW alert */
         Recw_Reset_Core_Output(p_core_output);
         p_core_output->recw_alert_level = p_persistent->recw_alert_prev_cycle;
         p_core_output->recw_id          = p_persistent->recw_id_prev_cycle;
         p_core_output->recw_index       = p_persistent->recw_index_prev_cycle;
      }
      else
      {
         p_persistent->recw_alert_holding_counter = FBK_ZERO_UINT;
         p_persistent->recw_ttc_value_hold        = p_core_output->recw_ttc;
      }
   }
   else
   {
      /* No alert is held in this cycle, thus reset holding counter */
      p_persistent->recw_alert_holding_counter = FBK_ZERO_UINT;
      p_persistent->recw_ttc_value_hold        = p_core_output->recw_ttc;
   }
}

static void Recw_Apply_Alert_Duration_Check(Recw_Core_Output_T *p_core_output,
                                            Recw_Persistent_T *p_persistent,
                                            const Recw_Core_Calibration_T *p_cals)
{
   /* Asserts */
   assert(NULL != p_core_output);
   assert(NULL != p_persistent);
   assert(NULL != p_cals);

   if ((RECW_NO_ALERT != p_core_output->recw_alert_level) && (p_core_output->recw_alert_level == p_persistent->recw_alert_prev_cycle))
   {
      Sat_Inc_Uint8(&(p_persistent->recw_alert_duration_counter));
      if (((RECW_ALERT_ACTIVE_LEVEL_1 == p_core_output->recw_alert_level)
           && (p_persistent->recw_alert_duration_counter > p_cals->k_recw_max_cycles_alert_duration[RECW_INDEX_ALERT_LEVEL_1]))
          || ((RECW_ALERT_ACTIVE_LEVEL_2 == p_core_output->recw_alert_level)
              && (p_persistent->recw_alert_duration_counter > p_cals->k_recw_max_cycles_alert_duration[RECW_INDEX_ALERT_LEVEL_2])))
      {
         /* Maximal allowed warning duration exceeded, thus suppress this alert */
         Recw_Reset_Core_Output(p_core_output);
         p_persistent->recw_alert_duration_counter   = FBK_ZERO_UINT;
         p_persistent->recw_alert_qualifying_counter = FBK_ZERO_UINT;
         p_persistent->recw_alert_holding_counter    = FBK_ZERO_UINT;
      }
   }
   else
   {
      p_persistent->recw_alert_duration_counter = FBK_ZERO_UINT;
   }
}
