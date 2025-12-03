file(GLOB SRC_FILES "*.cpp")
list(REMOVE_ITEM SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp")

add_library(editor_library ${SRC_FILES})

target_include_directories(editor_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
