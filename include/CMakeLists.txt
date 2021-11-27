################################################################################
##                                    bits
##
## This file is distributed under the 3-clause Berkeley Software Distribution
## License. See LICENSE for details.
################################################################################
set(LIB_NAME bits)

add_library(${LIB_NAME} INTERFACE)

target_include_directories(${LIB_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
target_code_coverage(${LIB_NAME} INTERFACE)

install(DIRECTORY bits DESTINATION include)
install(TARGETS ${LIB_NAME} EXPORT BitsExport)