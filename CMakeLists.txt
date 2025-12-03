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

add_executable(scribe src/Main.cpp)

target_link_libraries(scribe PRIVATE editor_library)

target_include_directories(scribe PUBLIC ${CMAKE_SOURCE_DIR}/src)
