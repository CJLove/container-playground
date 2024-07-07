cmake_minimum_required(VERSION 3.17)

project(container)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    .
)

# Threads
find_package(Threads)


set (container_SRC main.cpp)
add_executable( container ${container_SRC} )

target_link_libraries( container PUBLIC Threads::Threads )
