include_directories( ${CMAKE_SOURCE_DIR}/libKF5KDEGames )

set(kbreakout_SRCS
   main.cpp
   mainwindow.cpp
   canvaswidget.cpp
   gameengine.cpp
   levelloader.cpp
   #generalsettings.cpp
)

ki18n_wrap_ui(kbreakout_SRCS generalsettings.ui)

kconfig_add_kcfg_files(kbreakout_SRCS settings.kcfgc )

kde4_add_app_icon(kbreakout_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../pics/hi*-apps-kbreakout.png")

add_executable(kbreakout ${kbreakout_SRCS})

target_link_libraries(kbreakout KF5KDEGames KF5KDEGamesPrivate KF5::KDELibs4Support Qt5::Declarative Qt5::Xml KF5::DBusAddons)

install(TARGETS kbreakout ${INSTALL_TARGETS_DEFAULT_ARGS} )
install(FILES kbreakoutui.rc DESTINATION  ${KXMLGUI_INSTALL_DIR}/kbreakout)
install(DIRECTORY qml DESTINATION ${DATA_INSTALL_DIR}/kbreakout)
