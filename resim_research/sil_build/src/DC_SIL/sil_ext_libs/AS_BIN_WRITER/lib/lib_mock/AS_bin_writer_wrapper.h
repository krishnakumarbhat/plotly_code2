#ifndef AS_BIN_WRITER_WRAPPER_MOCK_H
#define AS_BIN_WRITER_WRAPPER_MOCK_H

typedef enum AS_BWW_MOUNTING_POSITIONS {
   AS_BWW_MOUNTING_UNKNOWN,
   AS_BWW_MOUNTING_REAR_LEFT,
   AS_BWW_MOUNTING_REAR_RIGHT,
   AS_BWW_MOUNTING_FRONT_RIGHT,
   AS_BWW_MOUNTING_FRONT_LEFT,
   AS_BWW_MOUNTING_REAR_CENTER,
   AS_BWW_MOUNTING_FRONT_CENTER,
   AS_BWW_MOUNTING_LEFT_CENTER,
   AS_BWW_MOUNTING_RIGHT_CENTER,
   AS_BWW_MOUNTING_INVALID_POSITION,
   AS_BWW_MOUNTING_CENTRAL_UNIT,
   AS_BWW_MOUNTING_NUMBER_OF_MOUNTING_STRINGS
} AS_BWW_MOUNTING_POSITIONS_T;

#ifdef _MSC_VER
/* Variant that gives no compiler warning in visual studio */
#define BIN_WRITER_NOOP ((void)0)
#else
/* Variant that gives no warning in QAC */
#define BIN_WRITER_NOOP \
   do {                 \
      ;                 \
   } while (0)
#endif

#define AS_BWW_INIT_MAC(logPath)                  BIN_WRITER_NOOP
#define AS_BWW_SET_MOUNTING_RP_MAC(radarPosition) BIN_WRITER_NOOP
#define AS_BWW_SET_TIME_STAMP_MAC(mntStr)         BIN_WRITER_NOOP
#define AS_BWW_SET_SCAN_INDEX_MAC(timestamp)      BIN_WRITER_NOOP

#define AS_BWW_SET_LIST_OF_SUPPRESSED_BIN_FILES_MAC(type, length)  BIN_WRITER_NOOP
#define AS_BWW_CLOSE_FILE_MAC(radarPosition)                       BIN_WRITER_NOOP
#define AS_BWW_WRITE_LINE_MAC(timeStamp, ScanIndex, radarPosition) BIN_WRITER_NOOP

#define STORE_VAL_MGR_WPR(type, var_name, value)                                                      BIN_WRITER_NOOP
#define STORE_VAL_MGR_WPR_CONDITIONAL(type, var_name, value, active)                                  BIN_WRITER_NOOP
#define STORE_VAL_MGR_WPR_INDEXED(type, var_name, value, index)                                       BIN_WRITER_NOOP
#define STORE_VAL_MGR_WPR_INDEXED_CONDITIONAL(type, var_name, value, index, active)                   BIN_WRITER_NOOP
#define STORE_ARRAY_ELEM_MGR_WPR(type, var_name, value, arr_index)                                    BIN_WRITER_NOOP
#define STORE_ARRAY_ELEM_MGR_WPR_CONDITIONAL(type, var_name, value, arr_index, active)                BIN_WRITER_NOOP
#define STORE_ARRAY_ELEM_MGR_WPR_INDEXED(type, var_name, value, arr_index, index)                     BIN_WRITER_NOOP
#define STORE_ARRAY_ELEM_MGR_WPR_INDEXED_CONDITIONAL(type, var_name, value, arr_index, index, active) BIN_WRITER_NOOP

#endif
