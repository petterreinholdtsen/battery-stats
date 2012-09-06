include_directories(
    ${CMAKE_BINARY_DIR}
)

add_executable(battery-stats-collector
    battery-stats-collector.c
)

set(EXTRA_LIBS)
if(WANT_ACPI)
    list(APPEND EXTRA_LIBS acpi)
endif()

target_link_libraries(battery-stats-collector
    apm
    ${EXTRA_LIBS}
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/battery-graph.in
    ${CMAKE_CURRENT_BINARY_DIR}/battery-graph @ONLY
)

install(TARGETS
    battery-stats-collector
    ${INSTALL_TARGETS_DEFAULT_ARGS}
)
install(PROGRAMS
    ${CMAKE_CURRENT_BINARY_DIR}/battery-graph battery-log
    DESTINATION bin
)
install(FILES
    battery-stats.conf
    DESTINATION etc
)
install(FILES
    graph-setup
    DESTINATION ${DATA_DIR}
)
