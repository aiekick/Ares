cmake_minimum_required(VERSION 3.1)

set(PROJECT Ares)

enable_language(C CXX)

project(${PROJECT} CXX)

add_definitions(-DVMA_VULKAN_VERSION=1002000)

set(LLVM_USE_CRT_DEBUG MTd CACHE STRING "" FORCE)
set(LLVM_USE_CRT_MINSIZEREL MT CACHE STRING "" FORCE)
set(LLVM_USE_CRT_RELEASE MT CACHE STRING "" FORCE)
set(LLVM_USE_CRT_RELWITHDEBINFO MT CACHE STRING "" FORCE)
set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)

## for group smake targets in the dir cmakeTargets
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmakeTargets")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

## some defines for debug mode (before 3rdparty.cmake)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")

add_definitions(-DUSE_RTX)
add_definitions(-DUSE_DECORATIONS_FOR_RESIZE_CHILD_WINDOWS) ## for the resize imgui issue when we have child glfw windows

message("${PROJECT} => ${FINAL_BIN_DIR}")
set(FINAL_BIN_DIR ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_INSTALL_PREFIX "${FINAL_BIN_DIR}")

if ("${CMAKE_EXE_LINKER_FLAGS}" STREQUAL "/machine:x64")
	set(ARCH x64)
else()
	set(ARCH x32)
endif()

include_directories(
	${CMAKE_SOURCE_DIR}/3rdparty
)

include(cmake/3rdparty.cmake)

set_target_properties(uninstall PROPERTIES FOLDER "cmakeTargets")

find_package(Vulkan REQUIRED)

## other define
add_definitions(-DGLM_FORCE_DEPTH_ZERO_TO_ONE)
add_definitions(-DGLM_FORCE_LEFT_HANDED)
add_definitions(-DPROJECT_PATH="${CMAKE_SOURCE_DIR}")
add_definitions(-DIMGUI_DISABLE_OBSOLETE_FUNCTIONS)
add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS)
add_definitions(-DIMGUI_ENABLE_FREETYPE)
add_definitions(${GLFW_DEFINITIONS})
add_definitions(-DUSE_IMGUI="${CMAKE_SOURCE_DIR}/3rdparty/imgui/imgui.h")
add_definitions(-DPROJECT_NAME="${PROJECT}")
add_definitions(-DUSE_STD_FILESYSTEM)
add_definitions(-DUSE_VULKAN)

if(UNIX)
    add_definitions(-DUNIX)
elseif(WIN32)
    add_definitions(-DWIN32)
endif()

if(MSVC)
    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(-DMSVC)
	add_definitions(-DNOMINMAX) ## for avoid issue regarding std::max and std::min
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wdeprecated-declarations -Wunused-parameter")
endif ()

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX _debug)
endif()

file(GLOB PROJECT_MAIN ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
source_group(main FILES ${PROJECT_MAIN})

file(GLOB_RECURSE PROJECT_SRC_RECURSE 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX src FILES ${PROJECT_SRC_RECURSE})

file(GLOB IMGUIIMPL_SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_glfw.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/imgui/backends/imgui_impl_glfw.h)
source_group(src\\ImGui_Impl FILES ${IMGUIIMPL_SOURCES})

file(GLOB SHADER_SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert
		${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag
		${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.geom
		${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.tesseval
		${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.tessctrl
		${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.comp)
source_group(Shaders FILES ${SHADER_SOURCES})

file(GLOB_RECURSE PROJECT_SHADERS_RECURSE 
	${CMAKE_CURRENT_SOURCE_DIR}/Builtins/*.glsl 
	${CMAKE_CURRENT_SOURCE_DIR}/Builtins/*.scen 
	${CMAKE_CURRENT_SOURCE_DIR}/Builtins/*.comp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX Shaders FILES ${PROJECT_SHADERS_RECURSE})

file(GLOB_RECURSE CMAKE_RECURSE 
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/*.cmake)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/cmake PREFIX cmake FILES ${CMAKE_RECURSE})

file(GLOB README_MD 
	${CMAKE_CURRENT_SOURCE_DIR}/*.md)
source_group(help FILES ${README_MD})

if (MSVC)
	set(ICON res/MSVCInfo.rc)
elseif (APPLE)
	set(ICON MACOSX_BUNDLE res/icon.icns)
	set_source_files_properties(res/icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/res PREFIX res FILES ${ICON})

add_executable(${PROJECT}
	${ICON}
	${README_MD}
	${PROJECT_MAIN}
	${CMAKE_RECURSE}
	${SHADER_SOURCES}
	${IMGUIIMPL_SOURCES}
	${PROJECT_SRC_RECURSE}
	${IMGUI_FREETYPE_SOURCES}
	${PROJECT_SHADERS_RECURSE}
)

target_compile_definitions(${PROJECT} PUBLIC -DPROJECT_NAME=${PROJECT})
target_compile_definitions(${PROJECT} PUBLIC -DCUSTOM_IMGUIFILEDIALOG_CONFIG="${CMAKE_SOURCE_DIR}/src/Gui/CustomImGuiFileDialogConfig.h")
target_compile_definitions(${IMGUIFILEDIALOG_LIBRARIES} PUBLIC -DCUSTOM_IMGUIFILEDIALOG_CONFIG="${CMAKE_SOURCE_DIR}/src/Gui/CustomImGuiFileDialogConfig.h")

## set_target_properties(${PROJECT} PROPERTIES FOLDER z_Apps)

set(${PROJECT}_Output_Name "${PROJECT}_${CMAKE_SYSTEM_NAME}$<$<CONFIG:Debug>:_Debug>$<$<CONFIG:Release>:_Release>$<$<CONFIG:MinSizeRel>:_MinSizeRel>$<$<CONFIG:RelWithDebInfo>:_RelWithDebInfo>_${ARCH}")
set_target_properties(${PROJECT} PROPERTIES	OUTPUT_NAME "${${PROJECT}_Output_Name}")

set_target_properties(${PROJECT} PROPERTIES	RUNTIME_OUTPUT_DIRECTORY "${FINAL_BIN_DIR}")

message("BACKEND_INCLUDE_DIRS : ${Vulkan_INCLUDE_DIRS}")
message("BACKEND_LIBRARIES : ${Vulkan_LIBRARIES}")

target_include_directories(${PROJECT} PRIVATE
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/libs
	${CMAKE_SOURCE_DIR}/3rdparty
	${CMAKE_SOURCE_DIR}/3rdparty/glm
	${CMAKE_SOURCE_DIR}/3rdparty/glslang
	${EFSW_INCLUDE_DIR}
	${GLFW_INCLUDE_DIR}
	${IMGUI_INCLUDE_DIR}
	${CTOOLS_INCLUDE_DIR}	
	${UTYPES_INCLUDE_DIR}
	${CPPIPC_INCLUDE_DIR}
	${IMNODES_INCLUDE_DIR}
	${Vulkan_INCLUDE_DIRS}
	${IMNODES_INCLUDE_DIR}	
	${TINYXML2_INCLUDE_DIR}
	${FREETYPE_INCLUDE_DIR}
	${IMGUIZMO_INCLUDE_DIR}
	${IMWIDGETS_INCLUDE_DIR}
	${FONTICONS_INCLUDE_DIR}
	${VKPROFILER_INCLUDE_DIR}	
	${VKFRAMEWORK_INCLUDE_DIR}
	${IMGUI_CANVAS_INCLUDE_DIR}	
	${IMGUIFILEDIALOG_INCLUDE_DIR}	
)

target_link_libraries(${PROJECT} PRIVATE
	${EFSW_LIBRARIES}
	${GLFW_LIBRARIES}
	${IMGUI_LIBRARIES}
	${CPPIPC_LIBRARIES}
	${UTYPES_LIBRARIES}
	${CTOOLS_LIBRARIES}
	${IMNODES_LIBRARIES}
	${Vulkan_LIBRARIESS}
	${IMNODES_LIBRARIES}
	${TINYXML2_LIBRARIES}
	${FREETYPE_LIBRARIES}
	${IMGUIZMO_LIBRARIES}
	${FONTICONS_LIBRARIES}
	${IMWIDGETS_LIBRARIES}	
	${VKPROFILER_LIBRARIES}	
	${VKFRAMEWORK_LIBRARIES}
	${IMGUI_CANVAS_LIBRARIES}
	${IMGUIFILEDIALOG_LIBRARIES}
)

########################################################
################# BUILD INCREMENT TOOL #################
########################################################

## only for Debug
add_subdirectory(3rdparty/BuildInc)
set_target_properties(BuildInc PROPERTIES FOLDER libs/tools)

add_custom_command(
	TARGET ${PROJECT} PRE_BUILD
	COMMAND BuildInc -prefix=${PROJECT} 1000:10 $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/src/Headers/${PROJECT}Build.h>
	DEPENDS ${PROJECT}
)

#############################################################
### INSTALL #################################################
#############################################################

install(TARGETS 
	${PROJECT} 
	${ASSIMP_LIBRARIES}
	RUNTIME 
	DESTINATION /
	COMPONENT APP_BINARY)
	
#############################################################
### VERSION #################################################
#############################################################

file(READ ${CMAKE_SOURCE_DIR}/src/Headers/${PROJECT}Build.h BuildIdFile)
string(REGEX MATCH "#define ${PROJECT}_BuildNumber ([0-9]*)" _ ${BuildIdFile})
set(BuildNumber ${CMAKE_MATCH_1})
string(REGEX MATCH "#define ${PROJECT}_MinorNumber ([0-9]*)" _ ${BuildIdFile})
set(MinorNumber ${CMAKE_MATCH_1})
string(REGEX MATCH "#define ${PROJECT}_MajorNumber ([0-9]*)" _ ${BuildIdFile})
set(MajorNumber ${CMAKE_MATCH_1})
set(${PROJECT}_Version "v${MajorNumber}.${MinorNumber}.${BuildNumber}")
message("${PROJECT} Version to install : ${${PROJECT}_Version}")

#############################################################
### PACK ####################################################
## https://gitlab.kitware.com/cmake/community/-/wikis/doc/cpack/Component-Install-With-CPack
#############################################################

if (USE_PACKAGE_TO_NSIS)
	set(CPACK_GENERATOR "NSIS")
else()
	set(CPACK_GENERATOR "ZIP")
endif()
set(CPACK_MONOLITHIC_INSTALL 1)
set(CPACK_ZIP_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE 1)
set(CPACK_PACKAGE_FILE_NAME "${${PROJECT}_Output_Name}_v${MajorNumber}.${MinorNumber}.${BuildNumber}")
set(CPACK_PACKAGE_VENDOR "FunParadigm")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${${PROJECT}_Output_Name} - offline shader editor")
set(CPACK_PACKAGE_VERSION "${MajorNumber}.${MinorNumber}.${BuildNumber}")
set(CPACK_PACKAGE_VERSION_MAJOR "${MajorNumber}")
set(CPACK_PACKAGE_VERSION_MINOR "${MinorNumber}")
set(CPACK_PACKAGE_VERSION_PATCH "${BuildNumber}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${FINAL_BIN_DIR}")
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "${CMAKE_PROJECT_NAME}")
##set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
##set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/VERSION")
SET(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_INSTALL_PREFIX}")
include(CPack)
