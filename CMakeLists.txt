cmake_minimum_required(VERSION 3.8)
project(Rc6)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp ProcessingData.cpp ProcessingData.h RC6algorithm.cpp RC6algorithm.h)
add_executable(Rc6 ${SOURCE_FILES})