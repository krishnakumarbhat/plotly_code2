add_library(SharedDevelopmentTools "")

include(${CMAKE_CURRENT_LIST_DIR}/Timing/timing.cmake)

set_target_properties(SharedDevelopmentTools PROPERTIES
		FOLDER "Libraries"
	)

target_compile_definitions(SharedDevelopmentTools PUBLIC -DSHARED_DEVELOPMENT_TOOLS_TIMERS_AVAILABLE)

TARGET_LINK_LIBRARIES(SharedDevelopmentTools PRIVATE Shared_Toolbox)

get_target_property(SharedDevelopmentTools_SOURCES SharedDevelopmentTools SOURCES)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${SharedDevelopmentTools_SOURCES})
