cmake_minimum_required(VERSION 3.21)

project(MCPSandbox C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

include(FetchContent)

FetchContent_Declare(
  picohttpparser
  GIT_REPOSITORY "https://github.com/h2o/picohttpparser.git"
  GIT_TAG        "v1.2"
  GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(picohttpparser)

set(PICO_SRC "${picohttpparser_SOURCE_DIR}/picohttpparser.c")
set(PICO_INC "${picohttpparser_SOURCE_DIR}")

add_library(picohttpparser_lib STATIC
    ${picohttpparser_SOURCE_DIR}/picohttpparser.c
)
target_include_directories(picohttpparser_lib
    PUBLIC ${picohttpparser_SOURCE_DIR}
)

find_package(json-c REQUIRED)
find_package(PostgreSQL REQUIRED)

add_executable(mcp_sandbox
    src/main.c
    src/postgres.c
    src/server.c
)

target_include_directories(mcp_sandbox
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include/mcp-sandbox
)

target_link_libraries(mcp_sandbox
    PRIVATE
        picohttpparser_lib
        json-c::json-c
        PostgreSQL::PostgreSQL
)
