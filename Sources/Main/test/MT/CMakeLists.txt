cmake_policy(SET CMP0043 NEW)

#dodajemy dane dotyczace QT - sciezki, biblioteki
#find_package(Qt5Widgets)
#find_package(Qt5Core)
#find_package(Qt5Xml)
#find_package(Qt5Test)
set(CMAKE_AUTOMOC ON)
include_directories($ENV{QTDIR})

set(ENV{QTEST_COLORED} 1)

include_directories(${Boost_INCLUDE_DIR}
                    ${PNS_SOURCE_DIR}/include
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

add_executable(ModuleTest ${SOURCES_MAIN} ${SOURCES_MOC} 
					${SOURCES_TEST_DATA} ${SOURCES_TEST_PLUGINS_DATA}
)


target_link_libraries(ModuleTest ${QT_LIBRARIES} PhoneticNumberSystem_lib
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
qt5_use_modules(ModuleTest Widgets Core Xml Test)


add_custom_command(TARGET ModuleTest POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/dict
		COMMAND ${CMAKE_COMMAND} -E copy ${PhoneticNumberSystem_SOURCE_DIR}/dictionaries/pl_PL.dic ${CMAKE_CURRENT_BINARY_DIR}/dict/pl_PL.dic
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/dictionaries/pl_PL_short.dic ${CMAKE_CURRENT_BINARY_DIR}/dict/pl_PL_short.dic
		)

#copy configuration XML from main application PhoneticNumberSystem.xml
add_custom_command(TARGET ModuleTest POST_BUILD 
	 COMMAND ${CMAKE_COMMAND} -E copy ${PhoneticNumberSystem_SOURCE_DIR}/PhoneticNumberSystem.xml ${CMAKE_CURRENT_BINARY_DIR}/PhoneticNumberSystem.xml
	 COMMAND ${CMAKE_COMMAND} -E copy ${PhoneticNumberSystem_SOURCE_DIR}/log4cpp.properties ${CMAKE_CURRENT_BINARY_DIR}/logs/log4cpp.properties )
ADD_TEST(test1 ModuleTest)
