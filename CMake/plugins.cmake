cmake_minimum_required(VERSION 2.8)



function(addLoggerLibrariesDependencies LibraryName OtherLibraries)
  IF (Logger STREQUAL "log4Qt")
    add_definitions(-DUSE_LOG4QT)
    SET(LOGGER_LIBRARY ${LOG4QT_LIBRARY_NAME})
  elseif (Logger STREQUAL "log4cpp")
  add_definitions(-DUSE_LOG4QCPP)
  SET(LOGGER_LIBRARY ${LOG4CPP_LIBRARY_NAME})
  ELSE (Logger STREQUAL "log4Qt")
    SET(LOGGER_LIBRARY "")
  ENDIF (Logger STREQUAL "log4Qt")

  #message(STATUS "AddLoggersLibs for '" ${LibraryName} "' are :" ${${OtherLibraries}} )
  target_link_libraries(${LibraryName} ${${OtherLibraries}} ${LOGGER_LIBRARY})
  #add_dependencies(${LibraryName} ${LOGGER_LIBRARY} ${${OtherLibraries}})
endfunction(addLoggerLibrariesDependencies)

