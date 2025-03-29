# Copyright (c) Martin Schweiger
# Licensed under the MIT License

# Static library for fastdxt utilities

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(fastdxt STATIC
	dxt.cpp
	intrinsic.cpp
	libdxt.cpp
	util.cpp
)
