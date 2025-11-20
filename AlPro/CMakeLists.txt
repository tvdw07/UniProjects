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
