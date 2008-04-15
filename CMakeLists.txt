project(kbreakout)
include_directories( ${CMAKE_SOURCE_DIR}/libkdegames  )

add_subdirectory(themes)

set(kbreakout_SRCS 
   main.cpp
   mainwindow.cpp 
   canvasitems.cpp
   canvaswidget.cpp
   textitems.cpp
   item.cpp
   gift.cpp
   brick.cpp
   ball.cpp
   gameengine.cpp
   renderer.cpp
   fontutils.cpp
)

kde4_add_kcfg_files(kbreakout_SRCS settings.kcfgc )

kde4_add_executable(kbreakout ${kbreakout_SRCS})

target_link_libraries(kbreakout  ${KDE4_KIO_LIBS} ${KDEGAMES_LIBRARY} )

install(TARGETS kbreakout  DESTINATION ${BIN_INSTALL_DIR} )

# TODO: put in a CMakeLists.txt in the subdirectory
install(FILES levelsets/default.levelset DESTINATION ${DATA_INSTALL_DIR}/kbreakout/levelsets)
