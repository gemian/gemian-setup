cmake_minimum_required(VERSION 3.13)
project(gemian-setup)

set(CMAKE_CXX_STANDARD 17)

add_executable(gemian-setup gemian_setup.cpp para_variables.cpp)

target_link_libraries(gemian-setup PRIVATE stdc++fs)

install(TARGETS gemian-setup RUNTIME DESTINATION sbin)
