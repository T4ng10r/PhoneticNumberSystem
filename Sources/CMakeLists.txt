project(PhoneticNumberSystem_Sources)

set_c11_flags()

include_directories(${PNS_include_SOURCE_DIR})
add_subdirectory(Tools)
add_subdirectory(Main)
add_subdirectory(log4cpp)
