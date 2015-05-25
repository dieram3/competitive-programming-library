
set(GTEST_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/utils/gtest-1.7.0/include)
set(GTEST_BOTH_LIBRARIES gtest gtest_main)

function(add_unittest test_name)
  set(target_name "${test_name}_test")	
  add_executable(${target_name} ${ARGN})
  target_link_libraries(${target_name} ${GTEST_BOTH_LIBRARIES})
  target_include_directories(${target_name} PRIVATE ${GTEST_INCLUDE_DIRS})
  set_property(TARGET ${target_name} PROPERTY CXX_STANDARD 11)
  set_property(TARGET ${target_name} PROPERTY CXX_STANDARD_REQUIRED ON)
  set_property(TARGET ${target_name} PROPERTY CXX_EXTENSIONS OFF)

  add_test(NAME ${test_name} COMMAND ${target_name})
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