function(target_enable_warnings TARGET VISIBILITY)
    target_compile_options(${TARGET} ${VISIBILITY}
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
            -Werror -Wall -Wextra
        >
        $<$<CXX_COMPILER_ID:MSVC>:
            "W4"
        >
    )
endfunction(target_enable_warnings)