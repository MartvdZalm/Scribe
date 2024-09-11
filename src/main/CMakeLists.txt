# src/main/

file(GLOB SRC_FILES
    "cpp/*.cpp"
    "cpp/editor/*.cpp"
    "cpp/editor/keybindings/*.cpp"
    "cpp/editor/terminal/*.cpp"
    "cpp/editor/exceptions/*.cpp"
)

# Define the editor library
add_library(editor_library ${SRC_FILES})

# Include the headers directory for both the library and the executable
target_include_directories(editor_library PRIVATE include)

# Add the main program executable
add_executable(scribe cpp/Main.cpp)

# Link the editor library to the main executable
target_link_libraries(scribe editor_library)

# Include directories for the executable as well
target_include_directories(scribe PRIVATE include)