cmake_minimum_required(VERSION 4.3)
project(repoChecker)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG f091b2c061b307ee89b164c39976fc9202a1c79d
)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG 55f93686c01528224f448c19128836e7df245f72
)
FetchContent_MakeAvailable(cpr json)

add_executable(repoChecker main.cpp)
target_link_libraries(repoChecker PRIVATE
        cpr::cpr
        nlohmann_json::nlohmann_json
)
