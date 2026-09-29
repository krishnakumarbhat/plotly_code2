
.. toctree::
    :maxdepth: 2

Calibration header
-------------------
This calibration header returns information about the size of the calibration structure, checksum and so on.
In order to fit with the generated files, it is available in two different orders. In case that a big-endian
system is used (most likely PowerPC-architectures) this macro needs to be defined for the calibration tool target.
By default a little endian system is assumed.

.. literalinclude:: ../../c_src/ct_calibration_header_t.h
    :language: c

Endian Swap
------------
The definition of the given type is also internally assumed in the calibration tool python code base. In case of adaption,
this also needs to be changed in the python codebase.

.. literalinclude:: ../../c_src/ct_endianness_switch.h
    :language: c

This function is mainly used for big endian systems to revert the order of bytes in arrays.

.. literalinclude:: ../../c_src/ct_endianness_switch.c
    :language: c
