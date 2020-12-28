cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

# KDE Application Version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "21")
set(RELEASE_SERVICE_VERSION_MINOR "03")
set(RELEASE_SERVICE_VERSION_MICRO "70")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kbreakout VERSION ${RELEASE_SERVICE_VERSION})

set (QT_MIN_VERSION "5.12.0")
set (KF5_MIN_VERSION "5.68.0")

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

find_package(KF5KDEGames 7.3.0 REQUIRED)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

include(FeatureSummary)
include(ECMInstallIcons)
include(ECMAddAppIcon)
include(ECMQtDeclareLoggingCategory)
include(ECMSetupVersion)

add_definitions(
    -DQT_NO_KEYWORDS
    -DQT_NO_FOREACH
)

add_subdirectory(src)
add_subdirectory(pics)
add_subdirectory(themes)
add_subdirectory(levelsets)
add_subdirectory(doc)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
