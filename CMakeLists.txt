cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(PROJECT_NAME point_cloud_annotation_tool)

set(CMAKE_CXX_COMPILER_VERSION 17)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/libs)

# Hide the console window in visual studio projects - Release
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()
# Use ccache to speed up compile process
#find_program(CCACHE_FOUND ccache)
#if(CCACHE_FOUND)
#    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
#    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
#endif(CCACHE_FOUND)

if(POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW)
endif()

if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()

find_package(VTK COMPONENTS
        vtkGUISupportQt
        vtkRenderingOpenGL2
        REQUIRED)

find_package(GLM REQUIRED)
find_package(OpenCV REQUIRED)

INCLUDE(${VTK_USE_FILE}) # include UseVTK.cmake

find_package(PCL REQUIRED segmentation filters visualization)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})
message(STATUS "PCL_LIBRARIES: ${PCL_LIBRARIES}")


include_directories(${CMAKE_SOURCE_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON) # For meta object compiler
set(CMAKE_AUTORCC ON) # Resource files
#set(CMAKE_AUTOUIC ON) # UI files
find_package(Qt5 COMPONENTS Widgets REQUIRED)
QT5_WRAP_UI(ui_mainwindow.h mainwindow.ui)

FILE(GLOB_RECURSE CPP RELATIVE ${PROJECT_SOURCE_DIR} "src/*.cpp")
FILE(GLOB_RECURSE HPP RELATIVE ${PROJECT_SOURCE_DIR} "include/*.hpp" "include/*.h")

set(project_source
        ${HPP} ${CPP}
        # pcl/visualization/pcl_visualizer_extented.h
        pcl/visualization/pcl_visualizer_extented.cpp
        # pcl/visualization/MyCloudLUT.h
        pcl/visualization/MyCloudLUT.cpp
        # pcl/visualization/PointCloudColorHandlerLUT.h
        # view/flowlayout.h
        view/flowlayout.cpp)

add_executable(${PROJECT_NAME} ${project_source} ui_mainwindow.h)
target_link_libraries(${PROJECT_NAME} ${PCL_LIBRARIES} Qt5::Widgets vtkGUISupportQt
        ${OpenCV_LIBS})
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS} include)

option(BUILD_TEST "build test exampes" ON)
if (BUILD_TEST)
   add_subdirectory(test)
endif()

option(BUILD_TOOLS "build tools" ON)
if (BUILD_TOOLS)
   add_subdirectory(tool)
endif()
