project(PhoneticNumberSystem)

#Automatically link Qt executables to qtmain target on Windows.
cmake_policy(SET CMP0020 NEW)
#Ignore COMPILE_DEFINITIONS_<Config> properties
cmake_policy(SET CMP0043 NEW)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5 COMPONENTS Widgets Xml Core REQUIRED)
include_directories($ENV{QTDIR} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS})

include_directories(${Boost_INCLUDE_DIR}
                    ${Tools_SOURCE_DIR}
                    ${log4cplus_SOURCE_DIR}/include 
                    ${log4cplus_BINARY_DIR}/include )
ADD_DEFINITIONS( "-DHAS_BOOST" )
ADD_DEFINITIONS( "-DQT_NO_KEYWORDS" )

GatheringFiles(${CMAKE_CURRENT_SOURCE_DIR} "cpp;h;;c;cc")
CreateProjectStructure(${CMAKE_CURRENT_SOURCE_DIR} )

SET(TMP_FILES ${TMP_FILES}
    Requirements.txt
    TODO.txt
    PhoneticNumberSystem.xml
    log4cplus.properties
    ${SOURCES_TOOLS} )

add_executable(PhoneticNumberSystem ${TMP_FILES} main.cpp  )

add_library(PhoneticNumberSystem_lib STATIC ${TMP_FILES} )
set(PhoneticNumberSystem_lib_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)

  target_link_libraries(PhoneticNumberSystem ${Boost_LIBRARIES} tools_lib log4cplus 
                        Qt5::Widgets Qt5::Core Qt5::Xml)
  target_link_libraries(PhoneticNumberSystem_lib ${Boost_LIBRARIES} tools_lib log4cplus 
                        Qt5::Widgets Qt5::Core Qt5::Xml gcov)

  add_dependencies(PhoneticNumberSystem phonetic_data)

  set_property(TARGET PhoneticNumberSystem PROPERTY CXX_STANDARD 11)
  set_property(TARGET PhoneticNumberSystem_lib PROPERTY CXX_STANDARD 11)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage ")
  SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage ")
endif()

   add_custom_target(phonetic_data
     COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/dict
     COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/logs
     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/dictionaries/pl_PL.dic ${CMAKE_CURRENT_BINARY_DIR}/dict/pl_PL.dic
     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/dictionaries/pl_PL.aff ${CMAKE_CURRENT_BINARY_DIR}/dict/pl_PL.aff
     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/log4cplus.properties ${CMAKE_CURRENT_BINARY_DIR}/logs/log4cplus.properties
     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/PhoneticNumberSystem.xml ${CMAKE_CURRENT_BINARY_DIR}/PhoneticNumberSystem.xml)


  INSTALL(TARGETS PhoneticNumberSystem
    DESTINATION PhoneticNumberSystem
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
    GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
  INSTALL(FILES dictionaries/pl_PL.aff dictionaries/pl_PL.dic
    DESTINATION PhoneticNumberSystem/dict)
  INSTALL(FILES log4cpp.properties
    DESTINATION PhoneticNumberSystem/logs
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
    GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
  INSTALL(FILES PhoneticNumberSystem.xml
    DESTINATION PhoneticNumberSystem
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
    GROUP_READ GROUP_EXECUTE GROUP_WRITE
    WORLD_READ #WORLD EXECUTE #WORLD_WRITE
  )

