if (CMAKE_SYSTEM_NAME STREQUAL Linux)
  find_package(X11 REQUIRED)
  if (NOT X11_Xi_FOUND)
    message(FATAL_ERROR "X11 Xi library is required")
  endif ()
endif ()

find_package(Vulkan REQUIRED)
add_definitions(-DVULKAN)

## Vulkan HPP, default dispatcher
add_definitions(-DVULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1)

## for prevent cast issues betwwen vk:: and VK if compiling a 32 bits version
add_definitions(-DVULKAN_HPP_TYPESAFE_CONVERSION=1) 

set(DEPENDS_PATH 
	${Vulkan_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
	${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glm
	${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glslang
	${CMAKE_CURRENT_SOURCE_DIR}/libs
)

## contrib
include(cmake/glm.cmake)
include(cmake/stb.cmake)
include(cmake/glfw.cmake)
include(cmake/efsw.cmake)
include(cmake/glslang.cmake)
include(cmake/lodepng.cmake)
include(cmake/cpp_ipc.cmake)
include(cmake/freetype.cmake)
include(cmake/tinyxml2.cmake)

## contrib aiekick
include(cmake/ctools.cmake)
include(cmake/utypes.cmake)
include(cmake/fonticons.cmake)
include(cmake/vkprofiler.cmake)
include(cmake/vkframework.cmake)

## contrib imgui
include(cmake/imgui.cmake)
include(cmake/imnodes.cmake)
include(cmake/imguizmo.cmake)
include(cmake/imwidgets.cmake)
include(cmake/imgui_canvas.cmake)
include(cmake/imguifiledialog.cmake)
