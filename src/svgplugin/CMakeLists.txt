project( svgplugin )

set(svgplugin_SRCS
    svgitem.cpp
    svgitemplugin.cpp)

add_library(svgplugin SHARED ${svgplugin_SRCS})
target_link_libraries(svgplugin kdegames kdegamesprivate ${QT_QTDECLARATIVE_LIBRARY})

install(TARGETS svgplugin DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/games/core)
install(FILES qmldir DESTINATION ${IMPORTS_INSTALL_DIR}/org/kde/games/core)
