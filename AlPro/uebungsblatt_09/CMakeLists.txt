# Konfiguration des Buildsystems CMake
#-------------------------------------------#

# Minimale Version des Buildsystems
cmake_minimum_required(VERSION 3.14)
# Name des Projekts
project(Zettel09)

# Optionen
#-------------------------------------------#

# Setzte verwendeten C++-Standard auf C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Prüfe ob Heapspeicher gefunden wird,
# der nicht freigegeben wurde
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

# Füge selbsgeschriebene Includes hinzu
include_directories(include)
# Füge externe Includes hinzu
# include_directories(external)

# Baue Programme
#-------------------------------------------#
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(aufg1 examples/aufg1.cpp ${SRC_FILES})
add_executable(aufg2 examples/aufg2.cpp ${SRC_FILES})
add_executable(aufg4 examples/aufg4.cpp ${SRC_FILES})

