
function(add_unittest TEST_NAME)
  set(TARGET_ID "${TEST_NAME}_test")	
  add_executable(${TARGET_ID} ${ARGN})
  target_link_libraries(${TARGET_ID} CP_utils gtest_main gtest)
  set_property(TARGET ${TARGET_ID} PROPERTY CXX_EXTENSIONS OFF)
  target_compile_options(${TARGET_ID} PRIVATE 
  $<$<CXX_COMPILER_ID:AppleClang>:-Wall -Wextra -pedantic -Wdocumentation>
  )
  add_test(NAME ${TEST_NAME} COMMAND ${TARGET_ID})
endfunction()

add_subdirectory(combinatorics)
add_subdirectory(data_structure)
add_subdirectory(geometry)
add_subdirectory(graph)
add_subdirectory(math)
add_subdirectory(miscellaneous)
add_subdirectory(number_theory)
add_subdirectory(search)
add_subdirectory(sorting)
add_subdirectory(strings)
add_subdirectory(utility)
