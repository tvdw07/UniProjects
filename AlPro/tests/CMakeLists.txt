cmake_minimum_required(VERSION 3.16)
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(googletest)

add_executable(AlPro_tests
  test_tuerme.cpp
  ${CMAKE_SOURCE_DIR}/uebungsblatt_05/hanoi/src/tuerme_von_Hanoi.cpp
)

target_include_directories(AlPro_tests PRIVATE
  ${CMAKE_SOURCE_DIR}/uebungsblatt_05/hanoi/include
)

target_link_libraries(AlPro_tests PRIVATE gtest_main)

add_test(NAME AlPro_tests COMMAND AlPro_tests)

