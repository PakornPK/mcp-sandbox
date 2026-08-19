cmake_minimum_required(VERSION 3.14)

project(MCPSandbox C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin
)

if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
endif()

find_package(json-c REQUIRED)

add_executable(mcp_sandbox
    src/main.c
    src/server.c
)

target_include_directories(mcp_sandbox
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include/mcp-sandbox
)

target_link_libraries(mcp_sandbox
    PRIVATE
        json-c::json-c
)
