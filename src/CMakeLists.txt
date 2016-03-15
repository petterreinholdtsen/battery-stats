include_directories(
    ${CMAKE_BINARY_DIR}
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/battery-graph.in
    ${CMAKE_CURRENT_BINARY_DIR}/battery-graph @ONLY
)

install(PROGRAMS
    ${CMAKE_CURRENT_BINARY_DIR}/battery-graph
    battery-log
    DESTINATION bin
)
install(PROGRAMS
    battery-stats-collector
    DESTINATION sbin
)
install(FILES
    graph-setup
    DESTINATION ${DATA_DIR}
)
install(PROGRAMS
    collect-csv
    battery-stats-generate
    battery-stats-graph
    battery-stats-graph.py
    battery-stats-graph-flow
    DESTINATION ${DATA_DIR}
)
