set(REL_PATH Fusion360/autocode_reuse)

set(SRC_LOCAL
    ${REL_PATH}/include/f360_linear_solver_data_types.h
    ${REL_PATH}/include/f360_matrix_vector_Init_real32_T.h
    ${REL_PATH}/include/f360_LinearSolvers.h
    ${REL_PATH}/source/f360_LinearSolvers.cpp
    ${REL_PATH}/source/f360_matrix_vector_Init_real32_T.cpp
)

set(SRC ${SRC} ${SRC_LOCAL})
