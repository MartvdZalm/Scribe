# add_executable(ScribeExecutable
#     cpp/Main.cpp
# )

file(GLOB SRC_FILES
    "cpp/Editor/*.cpp"
    "cpp/Main.cpp"
)

add_executable(ScribeExecutable ${SRC_FILES})

target_include_directories(ScribeExecutable PRIVATE include/Scribe)