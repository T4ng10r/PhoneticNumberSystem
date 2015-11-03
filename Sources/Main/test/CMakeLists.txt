
project(UT_PNS)
cmake_policy(SET CMP0043 NEW)

#dodajemy dane dotyczace QT - sciezki, biblioteki
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Xml)
find_package(Qt5Test)
set(CMAKE_AUTOMOC ON)
include_directories($ENV{QTDIR})

set(ENV{QTEST_COLORED} 1)

include_directories(${Boost_INCLUDE_DIR}
                    ${PNS_SOURCE_DIR}/include
										${gmock_SOURCE_DIR}/include
										${gtest_SOURCE_DIR}/include
                    ${PhoneticNumberSystem_SOURCE_DIR}
                    ${PhoneticNumberSystem_Sources_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${PhoneticNumberSystem_SOURCE_DIR}/Sources/Main
										)

####################

####################
FILE(GLOB_RECURSE SOURCES_TEST_DATA "TestCases/*")

SET ( SOURCES_MAIN
      main.cpp
      TestRunner.h
    )

SET ( SOURCES_TEST_MOC 
    TestCases/CSubstituteValuesConfigurationDlgTest.h
    TestCases/CAppSettingsTest.h
    TestCases/CSystemDigitsConfigurationTest.h
    TestCases/CSubstituteSearchTest.h
    TestCases/mt_substitute_search_test.h
    TestCases/CSearchResultTreeTest.h
    TestCases/CDataThreadTest.h
    )

#QT5_WRAP_CPP(SOURCES_MOC ${SOURCES_TEST_MOC} OPTIONS -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)

IF (WIN32)
	SOURCE_GROUP(moc FILES ${SOURCES_MOC})
	SOURCE_GROUP(TestCases FILES ${SOURCES_TEST_DATA})
	SOURCE_GROUP(TestCases\\Plugins FILES ${SOURCES_TEST_PLUGINS_DATA})
	SET(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:\"LIBCMTD.lib\")
endif(WIN32)
    
add_executable(UnitTest ${SOURCES_MAIN} ${SOURCES_MOC} 
					${SOURCES_TEST_DATA} ${SOURCES_TEST_PLUGINS_DATA}
)
target_link_libraries(UnitTest ${QT_LIBRARIES} PhoneticNumberSystem_lib gmock gtest)
qt5_use_modules(UnitTest Widgets Core Xml Test)


add_custom_command(TARGET UnitTest POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/dict
		COMMAND ${CMAKE_COMMAND} -E copy ${PhoneticNumberSystem_SOURCE_DIR}/dictionaries/pl_PL.dic ${CMAKE_CURRENT_BINARY_DIR}/dict/pl_PL.dic
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/dictionaries/pl_PL_short.dic ${CMAKE_CURRENT_BINARY_DIR}/dict/pl_PL_short.dic
		)

#copy configuration XML from main application PhoneticNumberSystem.xml
add_custom_command(TARGET UnitTest POST_BUILD 
	 COMMAND ${CMAKE_COMMAND} -E copy ${PhoneticNumberSystem_SOURCE_DIR}/PhoneticNumberSystem.xml ${CMAKE_CURRENT_BINARY_DIR}/PhoneticNumberSystem.xml
	 COMMAND ${CMAKE_COMMAND} -E copy ${PhoneticNumberSystem_SOURCE_DIR}/log4cpp.properties ${CMAKE_CURRENT_BINARY_DIR}/logs/log4cpp.properties )
ADD_TEST(test1 UnitTest)
