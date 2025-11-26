cmake_minimum_required(VERSION 3.16)
project(AlPro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(AlPro
        uebungsblatt_06/examples/demo_graph.cpp
        uebungsblatt_06/src/simpler_graph.cpp
)

target_include_directories(AlPro PRIVATE
        ${CMAKE_SOURCE_DIR}/uebungsblatt_06/include
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(AlPro PRIVATE -Wall -Wextra -Werror)
elseif (MSVC)
    target_compile_options(AlPro PRIVATE /W4 /WX) # /WX = treat warnings as errors
endif()



enable_testing()
add_subdirectory(tests)

