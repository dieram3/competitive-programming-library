if(DOXYGEN_FOUND)
	configure_file(Doxyfile.in HTML_Doxyfile @ONLY)

  add_custom_target(doc
  	COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/HTML_Doxyfile
  	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  	COMMENT "Generating HTML documentation with Doxygen" VERBATIM
  	SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
  	)
endif(DOXYGEN_FOUND)
