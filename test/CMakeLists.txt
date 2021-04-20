cmake_minimum_required(VERSION 3.13)
project(tests)

set(CMAKE_CXX_STANDARD 17)

find_package(Catch2 REQUIRED)

include_directories(..)

file(COPY para-gb.img DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY para-jp.img DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY para-dvorak.img DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY para-nosig1.img DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY para-nosig2.img DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY para-broken.img DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(tests
        test_keyboard_selection.cpp
        test_locales_identification.cpp
        ../para_variables.cpp
        ../locales_identification.cpp
)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(tests)