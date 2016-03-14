# conf
install(FILES
    conf/battery-stats.conf
    DESTINATION /etc
)

# man
if(UNIX)
    FILE(GLOB MAN1_FILES "man/*.1")
    FILE(GLOB MAN5_FILES "man/*.5")
    FILE(GLOB MAN8_FILES "man/*.8")
    install(FILES ${MAN1_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/man/man1)
    install(FILES ${MAN5_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/man/man5)
    install(FILES ${MAN8_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/man/man8)
endif()

# pixmaps
FILE(GLOB PIXMAP_FILES "pixmaps/*.png")
install(FILES ${PIXMAP_FILES} DESTINATION ${DATA_DIR}/pixmaps)

# Desktop file
install(
  FILES battery-stats.desktop
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications
)
