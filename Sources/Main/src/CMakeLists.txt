project(PhoneticNumberSystem)

#Automatically link Qt executables to qtmain target on Windows.
cmake_policy(SET CMP0020 NEW)
#Ignore COMPILE_DEFINITIONS_<Config> properties
cmake_policy(SET CMP0043 NEW)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)
include_directories($ENV{QTDIR} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS})

include_directories(${Boost_INCLUDE_DIR} )
ADD_DEFINITIONS( "-DHAS_BOOST" )
ADD_DEFINITIONS( "-DQT_NO_KEYWORDS" )

GatheringFiles(${CMAKE_CURRENT_SOURCE_DIR} "cpp;h;;c;cc")
CreateProjectStructure(${CMAKE_CURRENT_SOURCE_DIR} )

SET(TMP_FILES ${TMP_FILES} log4cpp.properties 
		Requirements.txt 
		TODO.txt
		PhoneticNumberSystem.xml
		log4cpp.properties
		${SOURCES_TOOLS} )
		  
add_executable(PhoneticNumberSystem ${TMP_FILES} main.cpp  )

add_library(PhoneticNumberSystem_lib STATIC ${TMP_FILES} )

  target_link_libraries(PhoneticNumberSystem ${Boost_LIBRARIES} tools_lib log4cpp 
                        Qt5::Widgets Qt5::Core Qt5::Xml)
  target_link_libraries(PhoneticNumberSystem_lib ${Boost_LIBRARIES} tools_lib log4cpp 
                        Qt5::Widgets Qt5::Core Qt5::Xml)
	
	add_custom_command(TARGET PhoneticNumberSystem POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/dict
		 COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/dictionaries/pl_PL.dic ${CMAKE_CURRENT_BINARY_DIR}/dict/pl_PL.dic
		 )
	add_custom_command(TARGET PhoneticNumberSystem POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/log4cpp.properties ${CMAKE_CURRENT_BINARY_DIR}/logs/log4cpp.properties
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

