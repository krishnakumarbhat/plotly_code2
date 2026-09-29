#ifndef ML_UNIT_CONVERSION_H
#define ML_UNIT_CONVERSION_H
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \defgroup unit_conversion Unit Conversion
 * \brief There is no unit conversion provided by the MathLibrary.
 *
 * All functions provided by the MathLibrary either use [SI units](https:\\en.wikipedia.org/wiki/International_System_of_Units)
 * or do not require a unit at all.
 * \section unit_conversion_why_not Why the MathLibrary does not offer unit conversion macros
 * Historically, differing units for the same property have led to various problems. Famous examples are
 * - [The mars climate orbiter](https:\\en.wikipedia.org/wiki/Mars_Climate_Orbiter#Cause_of_failure) crashed
 *   because a module delivered its measurement results in [united states custom units]
 *   (https:\\en.wikipedia.org/wiki/United_States_customary_units) while the receiving module expected
 *   the data in [SI units](https:\\en.wikipedia.org/wiki/International_System_of_Units)
 * - The [ell](https:\\en.wikipedia.org/wiki/Ell) which has been defined as the length of a man's arm from the
 *   elbow to the tip of the middle finger. This led to vastly differing length between countries. An ell could
 *   be between 0.45m and 1.18m
 *
 * \section unit_conversion_si SI Units
 * As a result of the before mentioned problems the international system of [SI units](https:\\en.wikipedia.org/wiki/International_System_of_Units) has been introduced.
 * The units of relevance in our area are:
 * Unit   | Symbol | Quantity
 * -------|--------|--------------
 * second | s      | time
 * meter  | m      | length
 *
 * From these various units can be derived:
 * Unit                     | Symbol        | Quantity
 * -------------------------|---------------|--------------
 * radian                   | rad           | plane angle
 * square meter             | \f$ m^2 \f$   | area
 * meter per second         | \f$ m/s \f$   | speed, velocity
 * meter per second squared | \f$ m/s^2 \f$ | acceleration
 *
 * A nice property of SI units is that the amount of defined units will not change. If we would not limit ourselves to SI units
 * we would need an incredible amount of conversion macros. Everybody can come up with new units any time:
 * - [List of unusual units of measurement](https:\\en.wikipedia.org/wiki/List_of_unusual_units_of_measurement)
 * - [List of obsolete units of measurement](https:\\en.wikipedia.org/wiki/List_of_obsolete_units_of_measurement)
 * - [List of humorous units of measurement](https:\\en.wikipedia.org/wiki/List_of_humorous_units_of_measurement)
 *
 * \section unit_conversion_concerns Answers to frequent concerns
 *
 * \subsection unit_conversion_customer_requirements What shall I do if my customer has a requirement that demands km/h?
 * The requirement asks that something shall happen at a specific speed. It does not demand how this is implemented.
 * A speed given in km/h can equivalently be given in m/s and all calibrations and calculations can be done in SI units
 * without harming the customer requirement.
 *
 * \subsection unit_conversion_degree I don't like radian, I want to use degree
 * A degree has the nice property of being divisible by nearly all numbers between 1 and 15 without reminder
 * Divisor | Result
 * --------|----------
 *  1      | 360
 *  2      | 180
 *  3      | 120
 *  4      |  90
 *  5      |  72
 *  6      |  60
 *  7      |  51.4285...
 *  8      |  45
 *  9      |  40
 * 10      |  36
 * 11      |  32.7272...
 * 12      |  30
 * 13      |  27.6923...
 * 14      |  25.7142...
 * 15      |  24
 *
 * You are sitting in front of a computer that does all the calculations for you. You therefore do not need this property.
 *
 * On the other hand:
 * - [Small-angle approximation](https:\\en.wikipedia.org/wiki/Small-angle_approximation) does only work in radian
 *   - \f$ \sin(x) = x \f$ if x is small
 *   - \f$ \cos(x) = 1 - x \f$ if x is small
 * - The distance on a circle is equivalent to the angle traveled times the radius: \f$ U = r * \alpha \f$
 *   - For \f$ \alpha = 2 \pi \f$ this results in the famous \f$ U = 2 \pi r \f$
 *
 * \subsection unit_conversion_local_macro Then 'll define a local macro
 * If you have some kind of variable that can only be used if a conversion macro is applied to it
 * \code{c}
 * if(p_vehicle_data->speed > CONVERT_MPH_TO_MPS(p_cals->my_threshold))
 * \endcode
 * then this will cause no harm as long as everybody knows about it and nobody forgets. As soon as you do not handle the code base anymore
 * (you may spend your honeymoon with the love of your live or may win the lottery and stop working for good) the next engineer
 * using my_threshold will most likely introduce a bug.
 */

#ifdef __cplusplus
}
#endif
#endif
