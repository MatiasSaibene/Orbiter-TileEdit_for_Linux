cmake_minimum_required(VERSION 3.14)
project(fastdxt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(osgEarth CONFIG REQUIRED)

# Crear la librería estática con sus fuentes
add_library(fastdxt STATIC
    dxt.cpp
    intrinsic.cpp
    libdxt.cpp
    util.cpp
)

target_include_directories(fastdxt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Definiciones necesarias
target_compile_definitions(fastdxt PRIVATE _CRT_SECURE_NO_WARNINGS)

# Opciones específicas de build
target_compile_options(fastdxt PRIVATE
    $<$<CONFIG:Debug>:-O0 -g>
    $<$<CONFIG:Release>:-O3>
)
