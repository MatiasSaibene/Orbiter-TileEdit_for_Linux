# Copyright (c) Martin Schweiger
# Licensed under the MIT License

cmake_minimum_required(VERSION 3.15)

project (tileedit VERSION 2025.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FASTDXT_LIB $<TARGET_FILE:fastdxt>)

find_package(Qt6 QUIET COMPONENTS Widgets Core Gui
	HINTS ${QTDIR}
)

if(Qt6_FOUND)
	add_subdirectory(extern/fastdxt)
	add_subdirectory(src)
endif()
