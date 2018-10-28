CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(redblacktree-cpp)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

FILE(GLOB_RECURSE template_sources  templates/*.tpp)
FILE(GLOB_RECURSE test_sources      tests/*.cpp)
FILE(GLOB_RECURSE sources           src/*.cpp)
FILE(GLOB_RECURSE headers           include/*.hpp include/*.h)

ADD_EXECUTABLE(tests ${test_sources} ${template_sources} ${headers})

TARGET_COMPILE_OPTIONS(tests PUBLIC -std=c++17 -Wall -Wextra -pedantic)

TARGET_INCLUDE_DIRECTORIES(tests PUBLIC include templates)

ADD_CUSTOM_TARGET(
    format
    COMMAND astyle ${test_sources} ${headers} ${template_sources}
)

