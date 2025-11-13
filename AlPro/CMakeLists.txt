cmake_minimum_required(VERSION 3.16)
project(AlPro LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(SOURCES
        # übungsblatt_02/uebungscode_02_02.cpp
        # übungsblatt_02/uebungscode_02_03.cpp
        # übungsblatt_03/uebungsblatt_03.cpp
        übungsblatt_05/Tuerme_von_Hanoi.cpp

)

add_executable(AlPro ${SOURCES})