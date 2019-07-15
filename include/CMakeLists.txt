set(LIB_NAME bits)

add_library(${LIB_NAME} INTERFACE)

target_include_directories(${LIB_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

install(DIRECTORY bits DESTINATION include)
install(TARGETS ${LIB_NAME} EXPORT BitsExport)