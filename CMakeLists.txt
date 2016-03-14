project(battery-stats)

cmake_minimum_required(VERSION 2.6)

include(CheckIncludeFiles)

set(VERSION_STRING 0.4.0)

set(INSTALL_TARGETS_DEFAULT_ARGS
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${LIB_INSTALL_DIR}
  ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT Devel
)
set(DATA_DIR "share/battery-stats")

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

add_subdirectory(data)
add_subdirectory(src)
