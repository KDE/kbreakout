ecm_setup_version(${RELEASE_SERVICE_VERSION}
    VARIABLE_PREFIX KBREAKOUT
    VERSION_HEADER kbreakout_version.h
)

add_executable(kbreakout)

target_sources(kbreakout PRIVATE
    main.cpp
    mainwindow.cpp
    canvaswidget.cpp
    gameengine.cpp
    levelloader.cpp

    kbreakout.qrc
)

ecm_qt_declare_logging_category(kbreakout
    HEADER kbreakout_debug.h
    IDENTIFIER KBREAKOUT_General
    CATEGORY_NAME kbreakout
    DESCRIPTION "KBreakout game"
    EXPORT KBREAKOUT
)

ki18n_wrap_ui(kbreakout generalsettings.ui)

kconfig_add_kcfg_files(kbreakout settings.kcfgc)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../pics/*-apps-kbreakout.png")
ecm_add_app_icon(kbreakout ICONS ${ICONS_SRCS})

target_link_libraries(kbreakout
    KF5KDEGames
    KF5KDEGamesPrivate
    KF5::DBusAddons
    KF5::XmlGui
    KF5::Crash
    Qt${QT_MAJOR_VERSION}::Xml
)

install(TARGETS kbreakout ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(DIRECTORY qml DESTINATION ${KDE_INSTALL_DATADIR}/kbreakout)

install(PROGRAMS org.kde.kbreakout.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kbreakout.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})

ecm_qt_install_logging_categories(
    EXPORT KBREAKOUT
    FILE kbreakout.categories
    DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)
