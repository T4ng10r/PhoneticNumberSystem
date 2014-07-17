project(PhoneticNumberSystem_Sources)

set_c11_flags()

include_directories(${PNS_include_SOURCE_DIR})
add_subdirectory(Tools)
add_subdirectory(Main)
if (Logger STREQUAL "log4Qt")
  add_subdirectory(log4qt)
elseif (Logger STREQUAL "log4cpp")
  add_subdirectory(log4cpp)
endif (Logger STREQUAL "log4Qt")
