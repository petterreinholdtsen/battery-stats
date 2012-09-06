project(battery-stats)

cmake_minimum_required(VERSION 2.6)

include(CheckIncludeFiles)

set(VERSION_STRING 0.3.7rc1)

option(WANT_ACPI "Enable ACPI support" ON)

set(INSTALL_TARGETS_DEFAULT_ARGS
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION ${LIB_INSTALL_DIR}
  ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT Devel
)
set(DATA_DIR "share/battery-stats")

check_include_files(apm.h HAVE_APM_H)
check_include_files(libacpi.h HAVE_LIBACPI_H)

if(NOT HAVE_APM_H)
    message(FATAL_ERROR "Missing header: apm.h")
endif()

if(NOT HAVE_LIBACPI_H AND WANT_ACPI)
    message(FATAL_ERROR "Missing header: libacpi.h")
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

add_subdirectory(src)
add_subdirectory(man)

FILE(GLOB PIXMAP_FILES "pixmaps/*.png")
install(FILES ${PIXMAP_FILES} DESTINATION ${DATA_DIR}/pixmaps)
