cmake_minimum_required (VERSION 3.17)

project (container_test)

enable_testing()

# Disable clang-tidy checks for unit test code
set(CMAKE_CXX_CLANG_TIDY "")

include_directories( 
    .
    ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set (container_test_SRC
    test_Dict.cpp

)



add_executable ( container_test ${container_test_SRC} )


target_link_libraries( container_test gtest gtest_main )

target_link_libraries( container_test )

add_test (container_test container_test )