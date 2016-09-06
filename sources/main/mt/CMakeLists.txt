cmake_policy(SET CMP0043 NEW)

#dodajemy dane dotyczace QT - sciezki, biblioteki
set(CMAKE_AUTOMOC ON)
include_directories($ENV{QTDIR})

set(ENV{QTEST_COLORED} 1)

include_directories(${Boost_INCLUDE_DIR}
                    ${PNS_SOURCE_DIR}/include
                    ${PhoneticNumberSystem_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${PhoneticNumberSystem_SOURCE_DIR}/Sources/Main
                    ${log4cplus_SOURCE_DIR}/include 
                    ${log4cplus_BINARY_DIR}/include )

####################
SET(PNS_MT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

####################
FILE(GLOB_RECURSE SOURCES_TEST_DATA "tests/*")

SET ( SOURCES_MAIN
      main.cpp
      TestRunner.h
    )

add_executable(module_test ${SOURCES_MAIN} ${SOURCES_MOC}
          ${SOURCES_TEST_DATA} ${SOURCES_TEST_PLUGINS_DATA}
)


target_link_libraries(module_test ${QT_LIBRARIES} PhoneticNumberSystem_lib
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
                      ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
qt5_use_modules(module_test Widgets Core Xml Test)

set_property(TARGET module_test PROPERTY CXX_STANDARD 11)
add_dependencies(unit_test phonetic_mt_data)

add_custom_target(phonetic_mt_data
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/dict
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/logs
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/dictionaries/pl_PL_short.dic ${CMAKE_CURRENT_BINARY_DIR}/dict/pl_PL_short.dic
    COMMAND ${CMAKE_COMMAND} -E copy ${PhoneticNumberSystem_SOURCE_DIR}/dictionaries/pl_PL.dic ${CMAKE_CURRENT_BINARY_DIR}/dict/pl_PL.dic
    COMMAND ${CMAKE_COMMAND} -E copy ${PhoneticNumberSystem_SOURCE_DIR}/dictionaries/pl_PL.aff ${CMAKE_CURRENT_BINARY_DIR}/dict/pl_PL.aff
    COMMAND ${CMAKE_COMMAND} -E copy ${PhoneticNumberSystem_SOURCE_DIR}/log4cplus.properties ${CMAKE_CURRENT_BINARY_DIR}/logs/log4cplus.properties
    COMMAND ${CMAKE_COMMAND} -E copy ${PhoneticNumberSystem_SOURCE_DIR}/PhoneticNumberSystem.xml ${CMAKE_CURRENT_BINARY_DIR}/PhoneticNumberSystem.xml)

if(CMAKE_COMPILER_IS_GNUCXX AND CMAKE_BUILD_TYPE STREQUAL "Debug")
  SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
  SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
  include(CodeCoverage)
  setup_target_for_coverage(module_test_coverage module_test ${PhoneticNumberSystem_BINARY_DIR} coverage_mt)
endif()

ADD_TEST(MT module_test)
