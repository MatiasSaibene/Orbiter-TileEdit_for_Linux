cmake_minimum_required(VERSION 3.19)
project(TileEditForLinux LANGUAGES CXX)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
#find_package(OpenSceneGraph 2.0.0 REQUIRED osgDB osgUtil)
find_package(osgEarth CONFIG REQUIRED)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)

qt_standard_project_setup()

# Add subdirectories for external libraries
add_subdirectory(extern/fastdxt)
#add_subdirectory(extern/libpng)
#add_subdirectory(extern/zlib)

qt_add_executable(TileEditForLinux
    WIN32 MACOSX_BUNDLE
    main.cpp
    tileedit.cpp tileedit.h tileedit.ui tileedit.qrc
    dlgconfig.cpp dlgconfig.h dlgConfig.ui
    dlgelevconfig.cpp dlgelevconfig.h dlgElevConfig.ui
    dlgelevexport.cpp dlgelevexport.h dlgElevExport.ui
    dlgelevimport.cpp dlgelevimport.h dlgElevImport.ui
    dlgsurfimport.cpp dlgsurfimport.h dlgSurfImport.ui
    ZTreeMgr.cpp ZTreeMgr.h
    cmap.cpp cmap.h
    colorbar.cpp colorbar.h
    ddsread.cpp ddsread.h
    dxt_io.cpp dxt_io.h
    elevtile.cpp
    elv_io.cpp elv_io.h
    imagetools.cpp imagetools.h
    tile.cpp tile.h
    tileblock.cpp
    tilecanvas.cpp tilecanvas.h
    windows_compat.h
    icon1.ico
)

# Include directories
include_directories(
)

target_include_directories(TileEditForLinux PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link libraries (static or shared depending on build config)
target_link_libraries(TileEditForLinux
    PRIVATE
        Qt::Core
        Qt::Widgets
        fastdxt
        PNG::PNG
        ZLIB::ZLIB
        osgEarth::osgEarth
)

include(GNUInstallDirs)

install(TARGETS TileEditForLinux
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET TileEditForLinux
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
