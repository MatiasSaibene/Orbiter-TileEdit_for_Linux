# Copyright (c) Martin Schweiger
# Licensed under the MIT License

cmake_minimum_required(VERSION 3.15)
project(tileedit VERSION 2021.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

add_executable(tileedit
    tileedit.cpp
    tileedit.qrc
    cmap.cpp
    colorbar.cpp
    ddsread.cpp
    dlgconfig.cpp
    dlgelevconfig.cpp
    dlgelevexport.cpp
    dlgelevimport.cpp
    dlgsurfimport.cpp
    dxt_io.cpp
    elevtile.cpp
    elv_io.cpp
    imagetools.cpp
    main.cpp
    tile.cpp
    tileblock.cpp
    tilecanvas.cpp
    ZTreeMgr.cpp
    tileedit.ui
    dlgConfig.ui
    dlgElevConfig.ui
    dlgElevExport.ui
    dlgElevImport.ui
    dlgSurfImport.ui
    tilecommon.h
    elevdata.h
    windows2linuxfunc.cpp
    cmap.h colorbar.h ddsread.h dlgconfig.h dlgelevconfig.h dlgelevexport.h dlgelevimport.h dlgsurfimport.h dxt_io.h elevdata.h elevtile.h elv_io.h imagetools.h resource.h tile.h tileblock.h tilecanvas.h tilecommon.h tileedit.h
)

add_dependencies(tileedit fastdxt)

target_include_directories(tileedit PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/libpng/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/zlib/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/fastdxt
)

target_link_libraries(tileedit
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/zlib/lib/libz.a
    ${CMAKE_CURRENT_SOURCE_DIR}/../extern/libpng/lib/libpng16.a
    ${FASTDXT_LIB}
)

# Instalación
install(TARGETS tileedit RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
