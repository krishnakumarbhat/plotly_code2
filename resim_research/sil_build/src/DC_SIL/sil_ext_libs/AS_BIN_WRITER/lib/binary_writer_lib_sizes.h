#ifndef BINARY_WRITER_LIB_SIZES_H
#define BINARY_WRITER_LIB_SIZES_H

#ifndef AS_BIN_WRITER_MAX_FUSED_SENSORS
/** max. number of sensors whose data will be stored in one .bin file.
Some data types need to store data for all sensor */
#define AS_BIN_WRITER_MAX_FUSED_SENSORS (4)
#endif

/** The max size of a variable name (Changing this will lead to an invalid bin file)*/
#define AS_BIN_WRITER_MAX_VARIABLE_NAME_LEN (256)

#ifndef AS_BIN_WRITER_MAX_VARIABLES
/** The number of variables that can be handled. The number of arrays is the same. */
#define AS_BIN_WRITER_MAX_VARIABLES (1024)
#endif

#ifndef AS_BIN_WRITER_MAX_ARRAY_SIZE
/** number of values that can be stored in a debug array */
#define AS_BIN_WRITER_MAX_ARRAY_SIZE (1024)
#endif

#endif
