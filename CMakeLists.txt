cmake_minimum_required(VERSION 3.2.3)

project(crator)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/wubwubcmake/modules")

find_package(Essentia REQUIRED)

set(crator_sources
	crator.cpp
)

add_executable(crator
	${crator_sources}
)

target_compile_options(crator
	PRIVATE -std=c++1z
)

target_link_libraries(crator
	PRIVATE Essentia
)
