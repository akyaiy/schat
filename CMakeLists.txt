cmake_minimum_required(VERSION 3.10)
include(FetchContent)

project(SChat
    VERSION 0.1
    DESCRIPTION "A Hello World Project"
)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(nlohmann_json)

add_executable(${PROJECT_NAME}
    SChat.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE nlohmann_json::nlohmann_json
)

target_include_directories(${PROJECT_NAME}
    PUBLIC "${PROJECT_BINARY_DIR}"
)
