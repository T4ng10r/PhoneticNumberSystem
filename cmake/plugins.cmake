
function(addLoggerLibrariesDependencies LibraryName OtherLibraries)
  add_definitions(-DUSE_LOG4CPLUS)
  SET(LOGGER_LIBRARY ${LOG4CPLUS_LIBRARY_NAME})

#  message(STATUS "AddLoggersLibs for '" ${LibraryName} "' are :" ${${OtherLibraries}} " aaa " ${LOGGER_LIBRARY}  )
  target_link_libraries(${LibraryName} ${${OtherLibraries}} ${LOGGER_LIBRARY})
  #add_dependencies(${LibraryName} ${LOGGER_LIBRARY} ${${OtherLibraries}})
endfunction(addLoggerLibrariesDependencies)

