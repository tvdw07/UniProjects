cmake_minimum_required(VERSION 3.16)
project(AlPro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(AlPro
        uebungsblatt_05/minsort_in_place/examples/demo.cpp
        uebungsblatt_05/minsort_in_place/src/minsort_ip.cpp
)

target_include_directories(AlPro PRIVATE
        ${CMAKE_SOURCE_DIR}/uebungsblatt_05/minsort_in_place/include
)
