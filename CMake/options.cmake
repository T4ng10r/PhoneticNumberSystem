cmake_minimum_required(VERSION 2.8)

option(USE_Qt5 "Use Qt5 version" FALSE)
#store path to Qt in QTDIR env

if (USE_Qt5)
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Xml)
find_package(Qt5Test)
else (USE_Qt5)
find_package(Qt4 REQUIRED QtNetwork QtGui QtCore QtXml QtTest)
endif (USE_Qt5)   

#boost build with bjam threading=multi runtime-link=static
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost REQUIRED COMPONENTS ) #thread system date_time chrono)

set(Logger "log4cpp" CACHE STRING "Logger where to store logs from program executions")
set(LoggersNames "None;log4cpp")
set_property(CACHE Logger PROPERTY STRINGS ${LoggersNames})
SET ( LOG4QT_LIBRARY_NAME "log4qt" )
SET ( LOG4CPP_LIBRARY_NAME "log4cpp" )

IF (Logger STREQUAL "log4Qt")
  add_definitions(-DUSE_LOG4QT)
  SET(LOGGER_LIBRARY ${LOG4QT_LIBRARY_NAME})
ELSEIF (Logger STREQUAL "log4cpp")
  add_definitions(-DUSE_LOG4CPP)
  SET(LOGGER_LIBRARY ${LOG4CPP_LIBRARY_NAME})
ELSE (Logger STREQUAL "log4Qt")
  SET(LOGGER_LIBRARY "")
ENDIF (Logger STREQUAL "log4Qt")


if (MSVC)
    set ( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /MP")
    #SET(CMAKE_CXX_FLAGS "/WL /MP /GR /EHsc" )
    message(STATUS "Added parallel build arguments to CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
endif(MSVC)

function(GetLogingLibrary )

endfunction(GetLogingLibrary)


