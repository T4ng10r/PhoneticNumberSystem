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
SET(LOG4CPLUS_BUILD_TESTING OFF)
SET(LOG4CPLUS_QT5 ON)
SET(WITH_UNIT_TESTS OFF)
find_package(Boost REQUIRED COMPONENTS date_time system filesystem ) #thread system date_time chrono)

if (NOT Boost_VERSION VERSION_GREATER "1.55.00")
    message(STATUS "Boost should be at least 1.56")
endif()

SET ( LOG4CPLUS_LIBRARY_NAME "log4cplus" )

add_definitions(-DUSE_LOG4CPLUS)
SET(LOGGER_LIBRARY ${LOG4CPLUS_LIBRARY_NAME})


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

SET(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
SET(GCC_COVERAGE_LINK_FLAGS    "-lgcov")
