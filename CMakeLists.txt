cmake_minimum_required(VERSION 2.8.12)
ENABLE_TESTING()

project(PhoneticNumberSystem_PROJECT)
add_subdirectory(docs)
add_subdirectory(include)  #to add PNS_include identificator

#generate compile commands for YCM
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(${PROJECT_SOURCE_DIR}/cmake/functions.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/options.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/plugins.cmake)

add_definitions(-D_VARIADIC_MAX=10)
add_subdirectory(sources)
