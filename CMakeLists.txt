cmake_minimum_required(VERSION 2.8.12)
ENABLE_TESTING()
project(phonetic_number_system)

add_subdirectory(docs)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1 ) #generate compile commands for YCM
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(functions)
include(options)
include(plugins)
set_c11_flags()

add_subdirectory(thirdparty)

add_definitions(-D_VARIADIC_MAX=10)
add_subdirectory(sources)
add_subdirectory(tests)
