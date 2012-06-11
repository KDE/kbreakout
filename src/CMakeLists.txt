include_directories( ${CMAKE_SOURCE_DIR}/libkdegames ${KDECLARATIVE_INCLUDE_DIR} )

set(kbreakout_SRCS 
   main.cpp
   mainwindow.cpp 
   canvaswidget.cpp
   gameengine.cpp
   levelloader.cpp
   canvasitem.cpp
   #generalsettings.cpp
)

kde4_add_ui_files(kbreakout_SRCS generalsettings.ui)

kde4_add_kcfg_files(kbreakout_SRCS settings.kcfgc )

kde4_add_app_icon(kbreakout_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../pics/hi*-app-kbreakout.png")

kde4_add_executable(kbreakout-qml ${kbreakout_SRCS})

target_link_libraries(kbreakout-qml kdegames kdegamesprivate ${KDE4_KDEUI_LIBS} ${QT_QTXML_LIBRARY} ${QT_QTDECLARATIVE_LIBRARY} ${KDECLARATIVE_LIBRARIES})

install(TARGETS kbreakout-qml ${INSTALL_TARGETS_DEFAULT_ARGS} )
install(FILES kbreakoutui.rc DESTINATION  ${DATA_INSTALL_DIR}/kbreakout)
install(DIRECTORY qml DESTINATION ${DATA_INSTALL_DIR}/kbreakout)
