cmake_minimum_required(VERSION 2.8.12)
ENABLE_TESTING()

project(PhoneticNumberSystem_PROJECT)
add_subdirectory(include)  #to add PNS_include identificator

#generate compile commands for YCM
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )
include(${PROJECT_SOURCE_DIR}/CMake/functions.cmake)
include(${PROJECT_SOURCE_DIR}/CMake/options.cmake)
include(${PROJECT_SOURCE_DIR}/CMake/plugins.cmake)

add_definitions(-D_VARIADIC_MAX=10)
add_subdirectory(Sources)
