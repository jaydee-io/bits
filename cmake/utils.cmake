function(target_enable_warnings TARGET VISIBILITY)
    target_compile_options(${TARGET} ${VISIBILITY}
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
            -Werror -Wall -Wextra
        >
        $<$<CXX_COMPILER_ID:MSVC>:
            "/W4"
            "/Zc:preprocessor" # To avoid preprocessor issue with __VA_ARGS__ macro
        >
    )
endfunction(target_enable_warnings)