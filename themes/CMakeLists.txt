# SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause

function(install_theme name)
    cmake_parse_arguments(ARG "" "ID" "" ${ARGN})
    if (NOT ARG_ID)
        set(ARG_ID "${name}")
    endif()
    set(svgz "${CMAKE_CURRENT_BINARY_DIR}/${name}.svgz")
    generate_svgz(${name}.svg ${svgz} "theme-")
    install(
        FILES
            ${ARG_ID}.desktop
            ${name}_preview.png
            ${svgz}
        DESTINATION ${KDE_INSTALL_DATADIR}/kbreakout/themes
    )
endfunction()

install_theme(egyptianbreakout ID default)
install_theme(alienbreakout)
install_theme(web20)
install_theme(crystal)
install_theme(simple)
install_theme(IceWorld)
