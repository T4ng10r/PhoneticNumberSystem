cmake_minimum_required(VERSION 2.6)

project(UnitTest)

#dodajemy dane dotyczace QT - sciezki, biblioteki
find_package(Qt4 REQUIRED QtNetwork QtGui QtCore QtXml QtTest)
include(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

	include_directories(${PhoneticNumberSystem_SOURCE_DIR})
	include_directories(${PhoneticNumberSystem_Sources_SOURCE_DIR})
	include_directories(${PhoneticNumberSystem_PROJECT_SOURCE_DIR}/include)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	include_directories(${PhoneticNumberSystem_SOURCE_DIR}/Sources/Main)

####################

####################
SET ( SOURCES_TEST_DATA
    CSubstituteValuesConfigurationDlgTest.cpp
    CSubstituteValuesConfigurationDlgTest.h
    CAppSettingsTest.cpp
    CAppSettingsTest.h
	  )

SET ( SOURCES_MAIN
      main.cpp
    )

SET ( SOURCES_TEST_MOC 
	CSubstituteValuesConfigurationDlgTest.h
    CAppSettingsTest.h
    )

QT4_WRAP_CPP(SOURCES_MOC ${SOURCES_TEST_MOC} OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)

IF (WIN32)
	SOURCE_GROUP(moc FILES ${SOURCES_MOC})
	SOURCE_GROUP(TestCases FILES ${SOURCES_TEST_DATA})
	SOURCE_GROUP(TestCases\\Plugins FILES ${SOURCES_TEST_PLUGINS_DATA})
	SET(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\"LIBCMTD.lib\")
endif(WIN32)
    
add_executable(UnitTest ${SOURCES_MAIN} ${SOURCES_MOC} 
					${SOURCES_TEST_DATA} ${SOURCES_TEST_PLUGINS_DATA}
)
target_link_libraries(UnitTest ${QT_LIBRARIES} PhoneticNumberSystem_lib )

#copy configuration XML from main application PhoneticNumberSystem.xml
add_custom_command(TARGET UnitTest POST_BUILD 
	 COMMAND ${CMAKE_COMMAND} -E copy ${PhoneticNumberSystem_SOURCE_DIR}/PhoneticNumberSystem.xml ${CMAKE_CURRENT_BINARY_DIR}/PhoneticNumberSystem.xml)

add_test(UnitTest UnitTest)

