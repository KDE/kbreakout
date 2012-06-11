project( kbreakout )

if(NOT COMPILING_KDEGAMES)
	find_package(KDE4 REQUIRED)
	include(KDE4Defaults)
	include(MacroLibrary)
	find_package(KDEGames 4.9.0 REQUIRED)

	add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
	add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
	include_directories(${KDEGAMES_INCLUDE_DIRS} ${KDE4_INCLUDES} ${QDBUS_INCLUDE_DIRS})
endif(NOT COMPILING_KDEGAMES)

find_package(KDeclarative REQUIRED)

add_subdirectory( src ) 
add_subdirectory( pics )
add_subdirectory( themes )
add_subdirectory( levelsets )
add_subdirectory(doc)
install( PROGRAMS kbreakout.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )
