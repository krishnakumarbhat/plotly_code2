#include "f360_rectangle.h"
#include "f360_reuse.h"
#include <cassert>

namespace f360_variant_A
{
   /*=========================================================================
   * Method         Rectangle_T::Rectangle_T
   *
   * Description    Constructor of Rectangle_T class.
   *                Rectangle is defined without orientation angle, which means that
   *                '0' orientation angle is assumed.
   *
   * Parameters
   * const float32_t min_long_in - minimal longitudinal position
   * const float32_t max_long_in - maximal longitudinal positon
   * const float32_t min_lat_in - minimal lateral posiiton
   * const float32_t max_lat_in - maximal lateral position
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   Points defining extreme positions of rectangle have to be valid. Otherwise
   *                 class will have undefined behaviour.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Rectangle_T::Rectangle_T(
      const float32_t min_long_in, 
      const float32_t max_long_in,
      const float32_t min_lat_in,
      const float32_t max_lat_in):
      min_long(min_long_in),
      max_long(max_long_in),
      min_lat(min_lat_in),
      max_lat(max_lat_in)
   {
      assert(min_long < max_long);
      assert(min_lat < max_lat);
   }

   /*=========================================================================
   * Method         Rectangle_T::~Rectangle_T
   *
   * Description    Destructor of Rectangle_T class.
   *
   * Parameters     None.
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   Rectangle_T::~Rectangle_T()
   {
   }

   /*=========================================================================
   * Method         Rectangle_T::Contains
   *
   * Description    Method used to verify whether point is inside zone area
   *
   * Parameters     None.
   *
   * Returns        None.
   *
   * Externals:     None.
   *
   * Precondition   Rectangle orientation is equal to zero so point coordinates
   *                must be aligned with recangle coordinates.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   bool Rectangle_T::Contains(
      const float32_t long_posn, 
      const float32_t lat_posn) const
   {
      const bool f_is_inside =
         (min_long < long_posn) &&
         (long_posn < max_long) &&
         (min_lat < lat_posn) &&
         (lat_posn < max_lat);

      return f_is_inside;
   }
}
