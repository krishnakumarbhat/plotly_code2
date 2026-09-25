project(SFL_Interface)
add_library(SFL_Interface STATIC)

target_sources(
   SFL_Interface 
      PUBLIC
         sfl_adapter.h
         sfl_adapter.cpp
         )
target_include_directories(
    SFL_Interface
		PUBLIC
		   ../../F360Tracker/Wrappers/SFL_Wrapper
		   ../CED/Customer_Adapter
		   ../CED/Customer_Adapter/Generic
		   ../CED/Source
		   ../CED/Calibration/CED_Core
		   ../CED/Calibration/Generic
		   ../CTA/Customer_Adapter
		   ../CTA/Customer_Adapter/Generic
		   ../CTA/Source
		   ../CTA/Calibration/Generic
		   ../CTA/Calibration/CTA_Core
		   ../ESA/Customer_Adapter
		   ../ESA/Customer_Adapter/Generic
		   ../ESA/Source
		   ../LCDA/Customer_Adapter
		   ../LCDA/Customer_Adapter/Generic
		   ../LCDA/Source
		   ../LCDA/Calibration/LCDA_Core
		   ../LCDA/Calibration/Generic
		   ../LTB/Customer_Adapter
		   ../LTB/Customer_Adapter/Generic
		   ../LTB/Source
		   ../LTB/Calibration/LTB_Core
		   ../LTB/Calibration/Generic
		   ../Math_Library/development/dependencies
		   ../Math_Library/ml_core/include
		   ../Math_Library/ml_core/include/legacy
		   ../PT/Customer_Adapter
		   ../PT/Customer_Adapter/Generic
		   ../PT/Source
		   ../PT/Calibration/PT_Core
		   ../PT/Calibration/Generic
		   ../RECW/Customer_Adapter
		   ../RECW/Customer_Adapter/Generic
		   ../RECW/Source
		   ../RECW/Calibration/RECW_Core
		   ../RECW/Calibration/Generic
		   ../SCW/Customer_Adapter
		   ../SCW/Customer_Adapter/Generic
		   ../SCW/Source
		   ../SCW/Calibration/SCW_Core
		   ../SCW/Calibration/Generic
		   ../TA/Customer_Adapter
		   ../TA/Customer_Adapter/Generic
		   ../TA/Source
		   ../TA/Calibration/TA_Core
		   ../TA/Calibration/Generic
		   ../ESA/Calibration/ESA_Core
		   ../ESA/Customer_Adapter
		   ../ESA/Customer_Adapter/Generic
		   ../ESA/Calibration/Generic
		   ../Calibration_Tool/c_src
		   ../Feature_Building_Kit/Calibration/Feature_Building_Kit_Core
		   ../Feature_Building_Kit/Interface
		   ../Feature_Building_Kit/Interface/boundaries
		   ../Feature_Building_Kit/Interface/debug_writer
		   ../Feature_Building_Kit/Interface/fill_pa_data
		   ../Feature_Building_Kit/Interface/host_trail
		   ../Feature_Building_Kit/Interface/index_lookup
		   ../Feature_Building_Kit/Interface/object_ageing
		   ../Feature_Building_Kit/Platform_Abstraction_Layer
		   ../Feature_Building_Kit/Platform_Abstraction_Layer/Generic/inc/data_ports
		   ../Feature_Building_Kit/Platform_Abstraction_Layer/Generic/inc
		   ../Feature_Building_Kit/Shared_Feature_Functions
		   ../Feature_Building_Kit/Shared_Feature_Functions/interpolation
		   ../Feature_Building_Kit/Shared_Feature_Functions/reference_point
		   ../Feature_Building_Kit/Shared_Feature_Functions/trajectory_prediction
		   ../Feature_Building_Kit/Shared_Feature_Functions/validation
		   ../Feature_Building_Kit/Shared_Feature_Functions/zone
		   ../Mock_Files/SIL_Library
)
set(MACRO_FOR_PA_OBJECTS 50 CACHE STRING "Number of objects in SFL Interface")
add_compile_definitions(PA_OBJ_NUMBER_OF_OBJECTS=${MACRO_FOR_PA_OBJECTS})


target_link_libraries(${target_name} 	Feature_Building_Kit )
target_link_libraries(${target_name} 	LCDA)
target_link_libraries(${target_name} 	CTA)
target_link_libraries(${target_name} 	TA)
target_link_libraries(${target_name} 	CED)
target_link_libraries(${target_name} 	RECW)
target_link_libraries(${target_name} 	PT)
target_link_libraries(${target_name} 	LTB)
target_link_libraries(${target_name} 	ESA)
if (MSVC)
    add_compile_options(
        "$<$<CONFIG:DEBUG>:/MDd>" # taken from raw VS
        "$<$<CONFIG:RELEASE>:/MD>" # taken from raw VS
        "$<$<CONFIG:RELEASE>:/Gy>" # taken from raw VS
        "$<$<CONFIG:RELEASE>:/Oi>" # taken from raw VS
        "$<$<CONFIG:RELEASE>:/GL>" # taken from raw VS
        /sdl- # taken from raw VS
        /W4 # taken from raw VS
        #/WX # taken from raw VS
        /Gm- # taken from raw VS
    )

    add_link_options(
        "$<$<CONFIG:RELEASE>:/LTCG>" # taken from raw VS
    )

    add_compile_definitions(
        _SCL_SECURE_NO_WARNINGS # taken from raw VS
        _CRT_SECURE_NO_WARNINGS # taken from raw VS
        $<$<CONFIG:DEBUG>:_DEBUG>
    )
endif()


if(NOT CMAKE_CROSSCOMPILING)
    target_compile_definitions(SFL_Interface PRIVATE -DPC_RESIM)
endif(NOT CMAKE_CROSSCOMPILING)

if(MSVC)
    target_compile_definitions(SFL_Interface PRIVATE _XKEYCHECK_H)
endif(MSVC)


