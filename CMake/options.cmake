cmake_policy(SET CMP0020 NEW )

find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Xml)
find_package(Qt5Test)

set(ENV{QTEST_COLORED} 1)

#boost build with bjam threading=multi runtime-link=static
#bjam link=static,shared threading=single,multi
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_LIBS ON)
#SET(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS date_time system filesystem unit_test_framework) #thread system date_time chrono)

set(Logger "log4cpp" CACHE STRING "Logger where to store logs from program executions")
set(LoggersNames "None;log4cpp")
set_property(CACHE Logger PROPERTY STRINGS ${LoggersNames})
SET ( LOG4CPP_LIBRARY_NAME "log4cpp" )

IF (Logger STREQUAL "log4cpp")
  add_definitions(-DUSE_LOG4CPP)
  SET(LOGGER_LIBRARY ${LOG4CPP_LIBRARY_NAME})
ELSE ()
  SET(LOGGER_LIBRARY "")
ENDIF ()


if (MSVC)
    set ( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /MP")
    #SET(CMAKE_CXX_FLAGS "/WL /MP /GR /EHsc" )
    message(STATUS "Added parallel build arguments to CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
endif()

if (UNIX)
	set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

function(GetLogingLibrary )

endfunction(GetLogingLibrary)


