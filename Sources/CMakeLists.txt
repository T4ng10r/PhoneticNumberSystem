cmake_minimum_required(VERSION 2.8)
project(PhoneticNumberSystem_Sources)

add_subdirectory(Main)
add_subdirectory(tools)
add_subdirectory(UnitTest)
if (Logger STREQUAL "log4Qt")
  add_subdirectory(log4qt)
endif (Logger STREQUAL "log4Qt")
