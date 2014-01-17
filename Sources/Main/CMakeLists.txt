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
	INCLUDE_DIRECTORIES(${Boost_LIBRARY_DIRS})
	ADD_DEFINITIONS( "-DHAS_BOOST" )
	#Boost_LIBRARY_DIRS
	ADD_DEFINITIONS( "-DQT_NO_KEYWORDS" )

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
		log4cpp.properties
		${SOURCES_TOOLS}
		)
		  
	FILE(GLOB SOURCES_DATA "Data/*")
	FILE(GLOB SOURCES_GUI "GUI/*")
	FILE(GLOB SOURCES_GUI_SETTINGS "GUI/Settings/*")

    SET ( SOURCES_MOC_HEADERS
		${CMAKE_CURRENT_SOURCE_DIR}/Data/CAppSettings.h
		${CMAKE_CURRENT_SOURCE_DIR}/Data/CDataThread.h
		${CMAKE_CURRENT_SOURCE_DIR}/Data/CDictionaryData.h
		${CMAKE_CURRENT_SOURCE_DIR}/Data/CSubstituteSearch.h 
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/MainWindow.h
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/SearchPhoneticRepresentationsDlg.h
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/ComposeSubstituteSentenceWidget.h 
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/SearchResultWordWidget.h 
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/SearchNumberLineEdit.h
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/Settings/AppSettingsDlg.h 
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/Settings/DictionariesConfigurationDlg.h 
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
        
	SET(LIBRARIES_LIST ${QT_LIBRARIES} ${Boost_LIBRARIES} Tools_Lib )
	addLoggerLibrariesDependencies(PhoneticNumberSystem LIBRARIES_LIST)
	addLoggerLibrariesDependencies(PhoneticNumberSystem_lib LIBRARIES_LIST)
	
	add_custom_command(TARGET PhoneticNumberSystem POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/PhoneticNumberSystem.xml ${CMAKE_CURRENT_BINARY_DIR}/PhoneticNumberSystem.xml)
	add_custom_command(TARGET PhoneticNumberSystem POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/log4cpp.properties ${CMAKE_CURRENT_BINARY_DIR}/logs/log4cpp.properties)

	

