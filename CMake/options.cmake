cmake_minimum_required(VERSION 2.8)


find_package(Qt4 REQUIRED QtNetwork QtGui QtCore QtXml QtTest)

#boost build with bjam threading=multi runtime-link=static
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost REQUIRED COMPONENTS ) #thread system date_time chrono)

set(Logger "log4Qt" CACHE STRING "Logger where to store logs from program executions")
set(LoggersNames "None;log4Qt")
set_property(CACHE Logger PROPERTY STRINGS ${LoggersNames})
SET ( LOG4QT_LIBRARY_NAME "log4qt" )

option(COMPCONF_FROZEN_COLUMN "Use FROZEN COLUMN view and model" TRUE)
option(COMPCONF_FROZEN_ROW "Use FROZEN ROW view and model" TRUE)
option(PLUGIN_DEBUG "Activate DEBUG for plugins" TRUE)

if (MSVC)
    #SET(CMAKE_SHARED_LINKER_FLAGS /NODEFAULTLIB:\"LIBCMTD.lib\")
endif()
