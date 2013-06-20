cmake_minimum_required(VERSION 2.8)

project(Tools)
if (USE_Qt5)
    include_directories($ENV{QTDIR})
else ()    
    include(${QT_USE_FILE})
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)

SET ( TOOLS_SRC
	qtTools.cpp
	loggers.cpp
	${CMAKE_SOURCE_DIR}/include/Tools/qtTools.h 
	${CMAKE_SOURCE_DIR}/include/Tools/loggers.h  )

IF (WIN32)
	SOURCE_GROUP(tools FILES ${TOOLS_SRC})
endif(WIN32)

add_library(Tools_Lib STATIC ${TOOLS_SRC} )
target_link_libraries(Tools_Lib ${LOGGER_LIBRARY} )

