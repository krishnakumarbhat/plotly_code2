ML_MathLibrary
==============

Formerly known as 'Shared Toolbox'.

This repository contains source files and their unit tests. The functionality
mainly assists with mathematical computations.

Usage
-----

Use ML_MathLibrary as a submodule in your git repository.
see [ml_core](ml_core/readme.md) for more information-

### CMake

~~~cmake
add_subdirectory(<path to your dependencies>/ML_MathLibrary/ml_core MLMathLibrary)
...
target_link_libraries(
    MyTarget
    PRIVATE MLMathLibrary
)
~~~
