
set(kbreakout_SRCS
   main.cpp
   mainwindow.cpp
   canvaswidget.cpp
   gameengine.cpp
   levelloader.cpp
)

ecm_setup_version(${KDE_APPLICATIONS_VERSION} VARIABLE_PREFIX KBREAKOUT VERSION_HEADER kbreakout_version.h)

ecm_qt_declare_logging_category(kbreakout_SRCS HEADER kbreakout_debug.h IDENTIFIER KBREAKOUT_General CATEGORY_NAME KBreakout)

qt5_add_resources(kbreakout_SRCS kbreakout.qrc)
ki18n_wrap_ui(kbreakout_SRCS generalsettings.ui)

kconfig_add_kcfg_files(kbreakout_SRCS settings.kcfgc )

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../pics/*-apps-kbreakout.png")
ecm_add_app_icon(kbreakout_SRCS ICONS ${ICONS_SRCS})

add_executable(kbreakout ${kbreakout_SRCS})

target_link_libraries(kbreakout
    KF5KDEGames
    KF5KDEGamesPrivate
    Qt5::Xml
    KF5::DBusAddons
    KF5::XmlGui
    KF5::Crash
)

install(TARGETS kbreakout ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )
install(DIRECTORY qml DESTINATION ${KDE_INSTALL_DATADIR}/kbreakout)
