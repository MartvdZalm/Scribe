cmake_minimum_required(VERSION 3.16.3)

project(Scribe)

file(COPY ${CMAKE_SOURCE_DIR}/resources/syntax DESTINATION ${CMAKE_BINARY_DIR}/resources)

include_directories(${CMAKE_SOURCE_DIR}/lib/rapidjson/include)

add_subdirectory(src/main)
add_subdirectory(src/test)
