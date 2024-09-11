# src/test/

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB TEST_FILES
    "cpp/*.cpp"
    "cpp/editor/*.cpp"
    "cpp/editor/keybindings/*.cpp"
    "cpp/editor/terminal/*.cpp"
    "cpp/editor/exceptions/*.cpp"
)

add_executable(unit_tests ${TEST_FILES})

# Include the headers directory for both the library and the test executable
target_include_directories(unit_tests PRIVATE ../main/include)

# Link Google Test and your project source files
target_link_libraries(unit_tests ${GTEST_LIBRARIES} pthread editor_library)