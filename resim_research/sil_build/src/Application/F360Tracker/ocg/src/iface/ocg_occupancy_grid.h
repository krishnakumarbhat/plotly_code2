#ifndef OCG_OCCUPANCY_GRID_H
#define OCG_OCCUPANCY_GRID_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4820)
#endif

#include "ocg_occupancy_grid_types.h"
#include "ocg_calibrations.h"
#include "ocg_internals_log_type_converters.h"
#include "ocg_output_log_type.h"

namespace ocg
{
   constexpr uint8_t ocg_header_version = 1U;

   class Occupancy_Grid
   {
   public:
      Occupancy_Grid() = default;

      void initialize(const OCG_Inputs_T &input);
      void step(double timestamp, const OCG_Inputs_T &input);
      void get_output(OCG_Outputs_T &ocg_output) const;
      void get_internals(OCG_Internals_T &ocg_internal) const;

      void log_internals(OCG_Internals_Log_T& ocg_internals_log);
      void log_output(OCG_Output_Log_T& ocg_output_log);
      bool initialize_from_internals_log(OCG_Internals_Log_T& ocg_internals_log);

   private:
      void fill_log_header(uint8_t& version, unsigned int& ocg_version_major, unsigned int& ocg_version_minor,
         unsigned int& ocg_version_patch, uint8_t& num_cells_x, uint8_t& num_cells_y, uint8_t& ocg_variant);
      void get_log_internals(OCG_Internals_Log_T &ocg_internals_log) const;
      void get_log_output(OCG_Output_Log_T &ocg_output_log) const;
      OCG_Underdrivability_Internal_T m_underdrivability;
      OCG_Calibrations_T m_calibrations;
      double m_timestamp;
      bool m_active;
   };
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
