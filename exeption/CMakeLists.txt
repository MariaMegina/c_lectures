cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -std=c++17")

SET(src exeption.h)

add_executable(test exeption.cpp ${SRC})
target_link_libraries(test gtest gmock)
