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

FetchContent_Declare(
    boost
    URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

add_executable(${PROJECT_NAME}
    src/SChat.cpp
    src/Config.cpp
    src/Session.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE nlohmann_json::nlohmann_json
)

target_include_directories(${PROJECT_NAME}
    PUBLIC "${PROJECT_BINARY_DIR}"
)
