cmake_minimum_required(VERSION 3.5)

project(CPPMAT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CPPMAT main.cpp cppmat.h cppmat.cpp paraget.h paraget.cpp)


install(TARGETS CPPMAT
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
