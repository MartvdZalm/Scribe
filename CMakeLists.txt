cmake_minimum_required(VERSION 3.16.3)

project(Scribe)

file(COPY 
    ${CMAKE_SOURCE_DIR}/resources/syntax
    ${CMAKE_SOURCE_DIR}/resources/themes
    ${CMAKE_SOURCE_DIR}/resources/preferences.json
    DESTINATION ${CMAKE_BINARY_DIR}/resources
)

include_directories(${CMAKE_SOURCE_DIR}/lib/)

add_subdirectory(src)
