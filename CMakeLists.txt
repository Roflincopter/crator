cmake_minimum_required(VERSION 3.2.3)

project(crator)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/wubwubcmake/modules")

find_package(Essentia REQUIRED)
find_package(Boost COMPONENTS filesystem system REQUIRED)

set(crator_sources
	crator.cpp
	bpmdetection.hpp bpmdetection.cpp
	chunker.hpp chunker.cpp
	monoloader.hpp monoloader.cpp
	essentiastandardalgorithmwrapper.hpp essentiastandardalgorithmwrapper.cpp
	energy.hpp energy.cpp
	monowriter.hpp monowriter.cpp
)

add_executable(crator
	${crator_sources}
)

target_compile_options(crator
	PRIVATE -std=c++1z
)

target_include_directories(crator SYSTEM
	PRIVATE ${Boost_INCLUDE_DIRS}
)

target_link_libraries(crator
	PRIVATE Essentia
	PRIVATE ${Boost_LIBRARIES}
)
