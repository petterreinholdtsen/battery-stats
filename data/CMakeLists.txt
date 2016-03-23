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
  FILES
  battery-stats.desktop
  battery-stats-graph.desktop
  DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications
)

FILE(GLOB PMSLEEP_FILES "pm-utils-sleep.d/*")
install(
    PROGRAMS ${PMSLEEP_FILES}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pm-utils/sleep.d
)
FILE(GLOB PMPOWER_FILES "pm-utils-power.d/*")
install(
    PROGRAMS ${PMPOWER_FILES}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pm-utils/power.d
)
FILE(GLOB SYSTEMDSLEEP_FILES "systemd-system-sleep/*")
install(
    PROGRAMS ${SYSTEMDSLEEP_FILES}
    DESTINATION /lib/systemd/system-sleep
)

FILE(GLOB UDEVRULES_FILES "udev-rules.d/*")
install(
    FILES ${UDEVRULES_FILES}
    DESTINATION /lib/udev/rules.d
)
