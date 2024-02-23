file(GLOB SRC_FILES
    "cpp/*.cpp"

    # Editor
    "cpp/editor/*.cpp"

    # Highlighting
    "cpp/editor/highlighting/*.cpp"
    "cpp/editor/highlighting/syntaxes/*.cpp"

    # Keybindings
    "cpp/editor/keybindings/*.cpp"

    # Terminal
    "cpp/editor/terminal/*.cpp"

    # Exceptions
    "cpp/editor/exceptions/*.cpp"

)

add_executable(scribe ${SRC_FILES})

target_include_directories(scribe PRIVATE include)
