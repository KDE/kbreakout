cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "20")
set(RELEASE_SERVICE_VERSION_MINOR "08")
set(RELEASE_SERVICE_VERSION_MICRO "3")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kbreakout VERSION ${RELEASE_SERVICE_VERSION})

set (QT_MIN_VERSION "5.12.0")
set (KF5_MIN_VERSION "5.59.0")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets Qml Quick QuickWidgets)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons 
    Config 
    Crash
    WidgetsAddons 
    Config 
    DBusAddons 
    I18n 
    ConfigWidgets 
    XmlGui
    DocTools
)

find_package(KF5KDEGames 4.9.0 REQUIRED)

include(FeatureSummary)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMAddAppIcon)
include(ECMQtDeclareLoggingCategory)
include(ECMSetupVersion)

add_definitions(-DQT_NO_FOREACH)

add_subdirectory(src)
add_subdirectory(pics)
add_subdirectory(themes)
add_subdirectory(levelsets)
add_subdirectory(doc)
install(PROGRAMS org.kde.kbreakout.desktop  DESTINATION  ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kbreakout.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES kbreakout.categories  DESTINATION  ${KDE_INSTALL_LOGGINGCATEGORIESDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
