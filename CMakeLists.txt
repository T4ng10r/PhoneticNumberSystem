cmake_minimum_required(VERSION 2.8.12)
ENABLE_TESTING()

add_subdirectory(docs)
add_subdirectory(include)  #to add PNS_include identificator

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) #generate compile commands for YCM
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(functions)
include(options)
include(plugins)

add_definitions(-D_VARIADIC_MAX=10)
add_subdirectory(sources)
