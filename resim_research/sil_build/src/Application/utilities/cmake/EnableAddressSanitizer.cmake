
function(enable_address_sanitizer)
    if(NOT DEFINED ADDRESS_SANITIZER_ENABLED)
        option(ENABLE_ADDRESS_SANITIZER "Enable address sanitizer for GCC/Clang builds" OFF)

        if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
            if(ENABLE_ADDRESS_SANITIZER)
                message(
                    "------------------------------------------------------------------------------------\n"
                    "To run address sanitizer export the following env variables prior to executing resim\n"
                    "export UBSAN_OPTIONS=\"print_stacktrace=1:halt_on_error=1\"\n"
                    "export ASAN_OPTIONS=\"check_initialization_order=1:detect_stack_use_after_return=1:fast_unwind_on_malloc=0:alloc_dealloc_mismatch=0:halt_on_error=1\"\n"
                    "------------------------------------------------------------------------------------")
                add_compile_options(-g -fno-omit-frame-pointer -fsanitize=address,undefined -fno-sanitize=enum)
                add_link_options(-fsanitize=address,undefined)
            endif()
        endif()
        set(ADDRESS_SANITIZER_ENABLED TRUE PARENT_SCOPE)
    endif()
endfunction()
