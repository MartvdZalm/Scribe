add_executable(ScribeExecutable
    cpp/Config.cpp
    cpp/Main.cpp
    cpp/Row.cpp
)

target_include_directories(ScribeExecutable PRIVATE include/Scribe)