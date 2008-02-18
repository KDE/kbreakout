project(kbreakout)

include_directories( ${CMAKE_SOURCE_DIR}/libkdegames  )

set(kbreakout_SRCS 
   main.cpp
   mainwindow.cpp 
   canvasitems.cpp
   canvaswidget.cpp
   item.cpp
   gameengine.cpp
)

kde4_add_executable(kbreakout ${kbreakout_SRCS})

target_link_libraries(kbreakout  ${KDE4_KIO_LIBS} ${KDEGAMES_LIBRARY} )

install(TARGETS kbreakout  DESTINATION ${BIN_INSTALL_DIR} )
install(FILES tmp.svg DESTINATION ${DATA_INSTALL_DIR}/kbreakout)
install(FILES default.svg DESTINATION ${DATA_INSTALL_DIR}/kbreakout)
install(FILES levelsets/default.desktop DESTINATION ${DATA_INSTALL_DIR}/kbreakout/levelsets)
