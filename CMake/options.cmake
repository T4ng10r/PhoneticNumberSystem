cmake_minimum_required(VERSION 2.8)

find_package(Qt4 REQUIRED QtNetwork QtGui QtCore QtXml QtTest)

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

function(GetLogingLibrary )

endfunction(GetLogingLibrary)


