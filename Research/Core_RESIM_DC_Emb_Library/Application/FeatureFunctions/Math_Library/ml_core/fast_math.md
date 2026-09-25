Fast math  {#fast_math}
=========

The MathLibrary offers to speed up trigonometric functions (with some loss of precision)
by using precomputed tables.
While speeding up the computation they also use a substantial amount of memory, both in ROM and RAM.
Therefore the user of the MathLibrary can choose if these tables shall be present in the build.
Since this is a decision that needs to be made consistent in software in the loop and embedded builds
the MathLibrary enforces the decision to be made.

Options
-------

Since different usage requirements need to be supported these compile time options exist:

### Usage of precomputed tables

The tables are available at compile time. As a consequence space in ROM is required.

### Usage of tables computed *once* at runtime

No space in ROM needed. Some time is needed to compute the tables at startup. The computation
of the tables needs to be triggered by a function call to one or both of

* Compute_Trig_Tables()
* Compute_Exp_Table()

### Not using tables, fall back to math.h

No space in ROM needed, no additional time at start up needed. The functions in math.h are slower to execute
but give a higher precision.

Compile time options
--------------------

Which of the above options is used can be steered either using a CMake option or by setting a preprocessor switch (for all users of the MathLibrary).

### CMake

Two CMake switches exist:

- MLMathLibrary_fast_math_exp_table
- MLMathLibrary_fast_math_trig_table

The first one decides what option to be used for the Fast_Exp() function, the second one makes the same decision for all trigonometric functions.
The options are:

- use_table: Use a table made available at compile time
- use_runtime_table: Use a table populated at runtime (e.g. while booting up)
- use_function: Fall back to the functions from math.h

If MLMathLibrary_fast_math_exp_table has not been set prior to the add_subdirectory() command that adds
the MathLibrary CMakeList.txt file CMake will stop configuring.
Before the call to add_subdirectory() do the following:

~~~cmake
set(MLMathLibrary_fast_math_exp_table "choose" CACHE STRING "Shall the MathLibrary Fast_Exp macro use an exp table?")
set(MLMathLibrary_fast_math_trig_table "choose" CACHE STRING "Shall the MathLibrary Fast_Sin, Fast_Cos,.. macro use precomputed trigonometric function tables?")
~~~

with both of the occurrences of "choose" being replaced with either "use_table" or "use_function"

### Make

Two macros must be defined:

- ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE
- ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE

The first one decides what option to be used for the Fast_Exp() function, the second one makes the same decision for all trigonometric functions.
Both of these macros *must* be set to one of the following three:

- ML_MATH_LIBRARY_FAST_MATH_USE_TABLE: Use a table made available at compile time
- ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE: Use a table populated at runtime (e.g. while booting up)
- ML_MATH_LIBRARY_FAST_MATH_USE_FUNCTION: Fall back to the functions from math.h


Detailed description of the options
-----------------------------------

### Precomputed tables

Precomputed tables are used to speed up trigonometric functions. These tables need to reside in
ROM.
#### Advantages

Identical table content in an embedded build as well as in a SIL environment.
#### Disadvantages

- Memory consumption is high in ROM and RAM.
- significant precision loss

### Tables computed at runtime

Tables are used to speed up trigonometric functions. These tables need to be filled
once at runtime, e.g. while booting up.

#### Advantages

- Less memory consumed in ROM.

#### Disadvantages

- Memory consumption is high in RAM
- Slows down the boot process since the tables need to be filled once
- significant precision loss

#### Side effects

Since the table content will depend on the math library implementation (and potentially math library settings) as well as
the used compiler (and its settings) and processor the computation runs on the computed table must be made available to a
SIL environment.

### Fall back to math.h

Instead of using the tables to speed up calculation the trigonometric functions from <math.h>
are used.

#### Advantages

- No additional memory needed.

#### Disadvantages

- No computational speed up available.

Fast math serialization {#fast_math_serialization}
==================================================

In order to be able to use the same table content in a SIL environment the tables need to be serialized.

Buffer size
-----------

The needed sizes for the buffers currently are:

### Trigonometric functions

49168 bytes are needed

### exp()

11608  bytes are needed

Embedded build
--------------

In an embedded build do the following:

- Compute the necessary tables by calling the following functions:
  - Compute_Trig_Tables()
  - Compute_Exp_Table()
- Now the tables are filled with meaningful values
- Call the following functions to get a serialized data stream:
  - Serialize_Trig_Table(buffer)
  - Serialize_Exp_Table(buffer)
- Send the buffer on some bus to be logged

Make sure to validate the return value of the serializing functions.

SIL environment
---------------

- Read the serialized buffer from the log
- Pass the serialized data buffer using the following functions
  - Deserialize_Trig_Table(buffer)
  - Deserialize_Exp_Table(buffer)

Make sure to validate the return value of the serializing functions.

Simplified usage
----------------

Since the table content stays the same the above process needs to be done only
once. It needs to be redone if the compiler or any compiler option changes though.

To ensure that the MathLibrary is able to warn should the table not match the version
used in the ECU the following can be done:

- Call Compute_Trig_Tables() at startup of the ECU to compute the table
- Use Serialize_Trig_Table_Checksum() to serialize the table checksum on the ECU
- Send the serialized checksum stream to be logged
- In SIL call Set_Trig_Table_By_Checksum() with the serialized checksum stream

Should the MathLibrary know a matching table content it will set its trigonometric tables accordingly.
If not the return value will be SHARED_TOOLBOX_SRL_ERR_PARSE.

Someone from the MathLibrary team can help to integrate an additional table if this should be needed.

In case the return value is SHARED_TOOLBOX_SRL_ERR_PARSE the MathLibrary internal trigonometric tables will
*NOT* be set. Its up to the SIL to decide what to do now. As a bare minimum the user should be warned and
Compute_Trig_Tables() may be called to fill the table with values computed on the SIL machine. These values will
most likely NOT match what was used in the ECU!

Special functions available for the race runner platform
========================================================

If the macro PLATFORM_RACERUNNER_MATH is defined the availability of a header called optimised_basic_ops.h is
expected. This header must make two functions available that are mapped as follows:

* Fast_Sqrt() is mapped to mrr_base_ops_sqrt()
* Fast_Hypot() is mapped to mrr_opt_hypot()

Using a macro to compute the absolute value
===========================================

If the macro FAST_ABSF_MACRO is defined the macro Fast_Absf is defined.
Otherwise Fast_Absf() does call fabsf() from math.h