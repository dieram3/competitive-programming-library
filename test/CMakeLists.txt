
function(add_unittest TEST_NAME)
  set(TARGET_ID "${TEST_NAME}_test")
  
  add_executable(${TARGET_ID} ${ARGN})
  target_link_libraries(${TARGET_ID} CP_utils gtest_main gtest)
  set_property(TARGET ${TARGET_ID} PROPERTY CXX_EXTENSIONS OFF)

  # Set warnings on AppleClang and Clang COMPILER IDS
  target_compile_options(${TARGET_ID} PRIVATE 
  $<$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>>:
  -Weverything 
  -Wno-c++98-compat 
  -Wno-c++98-compat-pedantic
  -Wno-padded
  -Wno-sign-conversion 
  -Wno-global-constructors
  >)
  
  add_test(NAME ${TEST_NAME} COMMAND ${TARGET_ID})
endfunction()

add_subdirectory(bits)
add_subdirectory(combinatorics)
add_subdirectory(data_structure)
add_subdirectory(geometry)
add_subdirectory(graph)
add_subdirectory(math)
add_subdirectory(number_theory)
add_subdirectory(search)
add_subdirectory(sorting)
add_subdirectory(strings)
add_subdirectory(utility)
