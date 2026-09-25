# Variant Configuration

# Set up preprocessor
add_definitions(-Drspp_variant_A=${RSPP_VARIANT_NS_STR})

# Create an interface folder with updated namespace
string(APPEND NS_STR1 ${RSPP_VARIANT_NS_STR} "::")
string(APPEND NS_STR2 "namespace " ${RSPP_VARIANT_NS_STR})
string(APPEND GUARD_STR "_VARIANT_" ${RSPP_VARIANT} "_H")
string(APPEND IFACE_DIR_NAME "rspp_iface_variant_" ${RSPP_VARIANT})

foreach(IFACE_FILE ${IFACE_SRC})
   file(READ ${IFACE_FILE} FILE_CONTENTS)
   string(REPLACE "rspp_variant_A::" ${NS_STR1} FILE_CONTENTS "${FILE_CONTENTS}")
   string(REPLACE "namespace rspp_variant_A" ${NS_STR2} FILE_CONTENTS "${FILE_CONTENTS}")
   string(REPLACE "_VARIANT_A_H" ${GUARD_STR} FILE_CONTENTS "${FILE_CONTENTS}")

   string(REPLACE "include" ${IFACE_DIR_NAME} IFACE_FILE "${IFACE_FILE}")
   file(WRITE ${CMAKE_BINARY_DIR}/${IFACE_FILE} "${FILE_CONTENTS}")
endforeach()

# Update the lib filename
string(APPEND LIB_NAME "rspp_variant_" ${RSPP_VARIANT})
set_target_properties(rspp PROPERTIES OUTPUT_NAME ${LIB_NAME})
