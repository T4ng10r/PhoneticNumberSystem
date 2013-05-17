cmake_minimum_required(VERSION 2.8)

project(CyfrowyAlfabetFonetyczny)

#dodajemy dane dotyczace QT - sciezki, biblioteki
find_package(Qt4 REQUIRED QtNetwork QtGui QtCore QtXml QtTest)
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ADD_DEFINITIONS( "-DHAS_BOOST" )

	include_directories(${CyfrowyAlfabetFonetyczny_PROJECT_SOURCE_DIR}/include)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	include_directories(${log4qt_SOURCE_DIR})

    SET ( SOURCES_TOOLS
		${CyfrowyAlfabetFonetyczny_PROJECT_SOURCE_DIR}/include/Tools/loggers.h 	
		${CyfrowyAlfabetFonetyczny_PROJECT_SOURCE_DIR}/include/Tools/qtTools.h 	
		)
    SET ( SOURCES_OTHER
        main.cpp
		loggers.cpp
        ${SOURCES_TOOLS}
		  )
		  
    SET ( SOURCES_DATA
		Data/CDataThread.cpp         
		Data/CDataThread.h
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
        
    #qt4_wrap_cpp(CyfrowyAlfabetFonetyczny SOURCES_MOC ${SOURCES_MOC_HEADERS})
    qt4_wrap_cpp(SOURCES_MOC ${SOURCES_MOC_HEADERS})
    QT4_ADD_RESOURCES( SOURCES_RCC CyfrowyAlfabetFonetyczny.qrc )
    
IF (WIN32)
    SOURCE_GROUP(Data FILES ${SOURCES_DATA})
    SOURCE_GROUP(GUI FILES ${SOURCES_GUI})
    SOURCE_GROUP(GUI\\Settings FILES ${SOURCES_GUI_SETTINGS})
    SOURCE_GROUP(moc FILES ${SOURCES_MOC})
    SOURCE_GROUP(tools FILES ${SOURCES_TOOLS})
ENDIF (WIN32)
    
IF (WIN32)
    add_executable(CyfrowyAlfabetFonetyczny ${SOURCES_DATA} ${SOURCES_MOC} 
        ${SOURCES_GUI} ${SOURCES_OTHER} ${SOURCES_RCC} 
        ${SOURCES_GUI_SETTINGS} main.cpp TODO.txt )
ELSE (WIN32)
    add_executable(CyfrowyAlfabetFonetyczny ${SOURCES_DATA} ${SOURCES_MOC} 
        ${SOURCES_GUI} ${SOURCES_OTHER} ${SOURCES_RCC} 
        ${SOURCES_GUI_SETTINGS} main.cpp TODO.txt )
ENDIF (WIN32)

    add_library(CyfrowyAlfabetFonetyczny_lib STATIC ${SOURCES_DATA} ${SOURCES_MOC} 
        ${SOURCES_GUI} ${SOURCES_OTHER} 
        main.cpp ${SOURCES_RCC})
        
	SET(LIBRARIES_LIST ${QT_LIBRARIES} ${Boost_LIBRARIES}) #Tools_Lib 
  	addLoggerLibrariesDependencies(CyfrowyAlfabetFonetyczny LIBRARIES_LIST)
	addLoggerLibrariesDependencies(CyfrowyAlfabetFonetyczny_lib LIBRARIES_LIST)
