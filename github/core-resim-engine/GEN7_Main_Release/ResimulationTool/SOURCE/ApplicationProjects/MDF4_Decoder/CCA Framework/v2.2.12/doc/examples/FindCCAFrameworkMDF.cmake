set(CCAFrameworkMDF_ARCH "x64" CACHE STRING "Target architecture.")
set(CCAFrameworkMDF_VERSION "2.2.12" CACHE STRING "Framework Version.")
set(CCAFrameworkMDF_FOLDER "" CACHE PATH "Path to Framework installation.")

set(version ${CCAFrameworkMDF_VERSION})
string(REPLACE "." "_" version_under ${CCAFrameworkMDF_VERSION})
string(REPLACE "." "-" version_dash ${CCAFrameworkMDF_VERSION})
set(ccalib ccalib_mdf_${CCAFrameworkMDF_ARCH}_${version_dash})

if(NOT CCAFrameworkMDF_INCLUDE_DIR)
    find_path(CCAFrameworkMDF_INCLUDE_DIR vgm_cca.h
        NO_DEFAULT_PATH
        PATHS
            ${CCAFrameworkMDF_FOLDER}/include
            /usr/include
            $ENV{CCA_FRAMEWORK_MDF_HOME_${version_under}}include
    )
endif()

if(NOT CCAFrameworkMDF_LIBRARY)
    find_library(CCAFrameworkMDF_LIBRARY
      NO_DEFAULT_PATH
      NAMES
        ${ccalib}
        lib${ccalib}
      PATHS
        ${CCAFrameworkMDF_FOLDER}/lib
        /usr/lib
        $ENV{CCA_FRAMEWORK_MDF_HOME_${version_under}}lib
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    CCAFrameworkMDF
    DEFAULT_MSG
    CCAFrameworkMDF_LIBRARY
    CCAFrameworkMDF_INCLUDE_DIR
)

# imported target
add_library(CCAFrameworkMDF SHARED IMPORTED GLOBAL)
set_target_properties(CCAFrameworkMDF PROPERTIES
    IMPORTED_IMPLIB "${CCAFrameworkMDF_LIBRARY}"
    IMPORTED_LOCATION "${CCAFrameworkMDF_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${CCAFrameworkMDF_INCLUDE_DIR}"
)

if(UNIX)
    set_target_properties(CCAFrameworkMDF PROPERTIES
        IMPORTED_LINK_INTERFACE_LIBRARIES "pthread"
    )
endif()
