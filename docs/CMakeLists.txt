
add_custom_target(docs COMMAND java -jar ${CMAKE_CURRENT_SOURCE_DIR}/plantuml.jar -o
  ${CMAKE_CURRENT_SOURCE_DIR}/uml ${CMAKE_CURRENT_SOURCE_DIR}/plants )
