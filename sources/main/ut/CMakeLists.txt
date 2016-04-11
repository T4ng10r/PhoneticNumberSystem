cmake_policy(SET CMP0043 NEW)

#dodajemy dane dotyczace QT - sciezki, biblioteki
set(CMAKE_AUTOMOC ON)
include_directories($ENV{QTDIR})
include_directories(${Boost_INCLUDE_DIR}
                    ${PNS_include_SOURCE_DIR}
                    ${PhoneticNumberSystem_SOURCE_DIR}
                    ${PhoneticNumberSystem_Sources_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${PhoneticNumberSystem_SOURCE_DIR}/Sources/Main
                    ${log4cplus_SOURCE_DIR}/include 
                    ${log4cplus_BINARY_DIR}/include
										)

####################


####################
FILE(GLOB_RECURSE SOURCES_TEST_DATA "tests/*")

SET ( SOURCES_MAIN
      main.cpp
      TestRunner.h )

add_executable(UnitTest ${SOURCES_MAIN} ${SOURCES_TEST_DATA} )

target_link_libraries(UnitTest ${QT_LIBRARIES} PhoneticNumberSystem_lib
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

qt5_use_modules(UnitTest Widgets Core Xml Test)

set_property(TARGET UnitTest PROPERTY CXX_STANDARD 11)

add_custom_command(TARGET UnitTest POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/dict
		COMMAND ${CMAKE_COMMAND} -E copy ${PhoneticNumberSystem_SOURCE_DIR}/dictionaries/pl_PL.dic ${CMAKE_CURRENT_BINARY_DIR}/dict/pl_PL.dic
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/dictionaries/pl_PL_short.dic ${CMAKE_CURRENT_BINARY_DIR}/dict/pl_PL_short.dic
		)

#copy configuration XML from main application PhoneticNumberSystem.xml
add_custom_command(TARGET UnitTest POST_BUILD 
   COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/logs
	 COMMAND ${CMAKE_COMMAND} -E copy ${PhoneticNumberSystem_SOURCE_DIR}/PhoneticNumberSystem.xml ${CMAKE_CURRENT_BINARY_DIR}/PhoneticNumberSystem.xml
	 COMMAND ${CMAKE_COMMAND} -E copy ${PhoneticNumberSystem_SOURCE_DIR}/log4cplus.properties ${CMAKE_CURRENT_BINARY_DIR}/logs/log4cplus.properties )

if(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
  SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
  include(CodeCoverage)
  setup_target_for_coverage(UnitTest_coverage UnitTest coverage_ut)
endif()


ADD_TEST(UT UnitTest)
