add_library(gtest "src/gtest-all.cc")
target_link_libraries(gtest PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(gtest PRIVATE "include" ".")

add_library(gtest_main "src/gtest_main.cc")
target_include_directories(gtest_main PRIVATE "include")

target_include_directories(gtest SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
