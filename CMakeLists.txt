cmake_minimum_required(VERSION 3.2.3)

project(crator)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/wubwubcmake/modules")

find_package(Essentia REQUIRED)
find_package(Boost COMPONENTS filesystem system REQUIRED)

#Made my "own" QT5_WRAP_UI because the official one outputs the generated file in the binary dir.
#I'm not a big fan of in source-builds and don't want to add my binary dir to my include path...
function(MY_QT5_WRAP_UI outfiles )
	set(options)
	set(oneValueArgs)
	set(multiValueArgs OPTIONS)

	cmake_parse_arguments(_WRAP_UI "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	set(ui_files ${_WRAP_UI_UNPARSED_ARGUMENTS})
	set(ui_options ${_WRAP_UI_OPTIONS})

	foreach(it ${ui_files})
		get_filename_component(outfile ${it} NAME_WE)
		get_filename_component(infile ${it} ABSOLUTE)
		set(outfile ${CMAKE_CURRENT_SOURCE_DIR}/ui_${outfile}.hpp)
		add_custom_command(OUTPUT ${outfile}
			COMMAND ${Qt5Widgets_UIC_EXECUTABLE}
			ARGS ${ui_options} -o ${outfile} ${infile}
			MAIN_DEPENDENCY ${infile}
			VERBATIM)
		list(APPEND ${outfiles} ${outfile})
	endforeach()
	set(${outfiles} ${${outfiles}} PARENT_SCOPE)
endfunction()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Core REQUIRED)

my_qt5_wrap_ui(QtGeneratedHeaders
	visualizerwindow.ui
)

qt5_wrap_cpp(QtGeneratedSources
	visualizerwindow.hpp
	visualizerwidget.hpp
)

set(crator_sources
	crator.cpp
	bpmdetection.hpp bpmdetection.cpp
	chunker.hpp chunker.cpp
	monoloader.hpp monoloader.cpp
	essentiastandardalgorithmwrapper.hpp essentiastandardalgorithmwrapper.cpp
	energy.hpp energy.cpp
	monowriter.hpp monowriter.cpp
	energychunks.hpp energychunks.cpp
	waveformalgorithm.hpp waveformalgorithm.cpp
	visualizerwindow.hpp visualizerwindow.cpp
	visualizerwidget.hpp visualizerwidget.cpp
	${QtGeneratedHeaders} ${QtGeneratedSources}
	ziprange.hpp
	shaderloader.hpp
)

set(shaders
	shaders/waveform.vert
	shaders/waveform.frag
)

add_executable(crator
	${crator_sources}
	${shaders}
)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/shaders
)

add_custom_target(shader_dir ALL
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/shaders
)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders/waveform.frag
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/shaders/waveform.frag ${CMAKE_CURRENT_BINARY_DIR}/shaders/
	MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/shaders/waveform.frag
)

add_custom_target(waveform.frag ALL
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/shaders/waveform.frag
)

add_dependencies(waveform.frag shader_dir)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders/waveform.vert
	COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/shaders/waveform.vert ${CMAKE_CURRENT_BINARY_DIR}/shaders/
	MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/shaders/waveform.vert 
)

add_custom_target(waveform.vert ALL
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/shaders/waveform.vert
)

add_dependencies(waveform.vert shader_dir)

add_dependencies(crator waveform.frag waveform.vert)

target_compile_options(crator
	PRIVATE -std=c++1z
)

target_include_directories(crator SYSTEM
	PRIVATE ${Boost_INCLUDE_DIRS}
)

target_link_libraries(crator
	PRIVATE Essentia
	PRIVATE ${Boost_LIBRARIES}
	PRIVATE Qt5::Widgets
	PRIVATE Qt5::Core
	PRIVATE Qt5::Gui
	PRIVATE GLEW
	PRIVATE GL
)
