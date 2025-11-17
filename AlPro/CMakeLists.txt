cmake_minimum_required(VERSION 3.16)
project(AlPro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(AlPro
        uebungsblatt_05/hanoi/examples/demo.cpp
        uebungsblatt_05/hanoi/src/Tuerme_von_Hanoi.cpp
)

target_include_directories(AlPro PRIVATE
        ${CMAKE_SOURCE_DIR}/uebungsblatt_05/hanoi/include
)
