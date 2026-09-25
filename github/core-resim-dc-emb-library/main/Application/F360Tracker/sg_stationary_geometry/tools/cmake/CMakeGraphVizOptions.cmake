list(APPEND ignored_resim_targets EXTERN_HIGHFIVE)
list(APPEND ignored_resim_targets EXTERN_ORDERED_MAP)
list(APPEND ignored_resim_targets EXTERN_PUGI_XML)
list(APPEND ignored_resim_targets EXTERN_CMP)
list(APPEND ignored_resim_targets hdf5::hdf5-shared)

list(APPEND ignored_ut_targets embedded_list_ut)
list(APPEND ignored_ut_targets fifo_ut)
list(APPEND ignored_ut_targets sg_math_ut)
list(APPEND ignored_ut_targets sg_iface_ut)
list(APPEND ignored_ut_targets sg_host_props_ut)
list(APPEND ignored_ut_targets memory_pool_ut)

list(APPEND ignored_gtest_targets gmock*)
list(APPEND ignored_gtest_targets gtest*)

set(GRAPHVIZ_GENERATE_PER_TARGET FALSE)
set(GRAPHVIZ_GENERATE_DEPENDERS FALSE)