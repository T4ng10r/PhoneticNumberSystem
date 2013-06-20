cmake_minimum_required(VERSION 2.8)

project(PhoneticNumberSystem)

#dodajemy dane dotyczace QT - sciezki, biblioteki
if (USE_Qt5)
    set(CMAKE_AUTOMOC ON)
    find_package(Qt5Widgets)
    find_package(Qt5Core)
    find_package(Qt5Xml)
    include_directories($ENV{QTDIR} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS})
else (USE_Qt5)
    find_package(Qt4 REQUIRED QtNetwork QtGui QtCore QtXml QtTest)
    include(${QT_USE_FILE})
endif (USE_Qt5)   


INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ADD_DEFINITIONS( "-DHAS_BOOST" )
ADD_DEFINITIONS(-DWALLAROO_FORCE_USE_BOOST)

	include_directories(${CMAKE_SOURCE_DIR}/include)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	include_directories(${log4qt_SOURCE_DIR})

    SET ( SOURCES_TOOLS
		${CMAKE_SOURCE_DIR}/include/Tools/loggers.h 	
		${CMAKE_SOURCE_DIR}/include/Tools/qtTools.h 	 )
		
    SET ( SOURCES_OTHER
        Requirements.txt 
        TODO.txt
		#loggers.cpp
		PhoneticNumberSystem.xml
        ${SOURCES_TOOLS}
		  )
		  
    SET ( SOURCES_DATA
		Data/CDataThread.cpp         
		Data/CDataThread.h
		Data/CAppSettings.cpp
		Data/CAppSettings.h 
		  )
    
    SET ( SOURCES_GUI
		GUI/MainWindow.cpp
		GUI/MainWindow.h
		GUI/MainWindow_p.cpp
		GUI/MainWindow_p.h
    )
    SET ( SOURCES_GUI_SETTINGS
        GUI/Settings/SubstituteValuesConfigurationDlg.cpp 
        GUI/Settings/SubstituteValuesConfigurationDlg.h
    )

    SET ( SOURCES_MOC_HEADERS
		${CMAKE_CURRENT_SOURCE_DIR}/Data/CDataThread.h
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/MainWindow.h
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/Settings/SubstituteValuesConfigurationDlg.h
        )
        
if (NOT USE_Qt5)
    qt4_wrap_cpp(SOURCES_MOC ${SOURCES_MOC_HEADERS} OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)
    QT4_ADD_RESOURCES( SOURCES_RCC PhoneticNumberSystem.qrc )
endif ()
    
IF (WIN32)
    SOURCE_GROUP(Data FILES ${SOURCES_DATA})
    SOURCE_GROUP(GUI FILES ${SOURCES_GUI})
    SOURCE_GROUP(GUI\\Settings FILES ${SOURCES_GUI_SETTINGS})
    SOURCE_GROUP(moc FILES ${SOURCES_MOC})
    SOURCE_GROUP(tools FILES ${SOURCES_TOOLS})
ENDIF (WIN32)
    

IF (WIN32)
    add_executable(PhoneticNumberSystem ${SOURCES_DATA} ${SOURCES_MOC} 
        ${SOURCES_GUI} ${SOURCES_OTHER} ${SOURCES_RCC} 
        ${SOURCES_GUI_SETTINGS} main.cpp  )
ELSE (WIN32)
    add_executable(PhoneticNumberSystem ${SOURCES_DATA} ${SOURCES_MOC} 
        ${SOURCES_GUI} ${SOURCES_OTHER} ${SOURCES_RCC} 
        ${SOURCES_GUI_SETTINGS} main.cpp )
ENDIF (WIN32)

if (USE_Qt5)
    qt5_use_modules(PhoneticNumberSystem Widgets Core Xml)
endif()

    add_library(PhoneticNumberSystem_lib STATIC ${SOURCES_DATA} ${SOURCES_MOC} 
        ${SOURCES_GUI} ${SOURCES_OTHER} 
        ${SOURCES_GUI_SETTINGS} ${SOURCES_RCC})
        
	SET(LIBRARIES_LIST ${QT_LIBRARIES} ${Boost_LIBRARIES} Tools_Lib ) #Tools_Lib 
  addLoggerLibrariesDependencies(PhoneticNumberSystem LIBRARIES_LIST)
	addLoggerLibrariesDependencies(PhoneticNumberSystem_lib LIBRARIES_LIST)
	
	add_custom_command(TARGET PhoneticNumberSystem POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/PhoneticNumberSystem.xml ${CMAKE_CURRENT_BINARY_DIR}/PhoneticNumberSystem.xml)

	

