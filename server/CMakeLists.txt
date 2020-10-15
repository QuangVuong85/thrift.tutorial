cmake_minimum_required(VERSION 3.9)

project(server-thrift)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)

file(GLOB APP_FILES
        ../gen-cpp/*.h
        ../gen-cpp/Calculator.cpp
        ../gen-cpp/sharedmath_constants.cpp
        ../gen-cpp/sharedmath_types.cpp
        ../gen-cpp/SharedService.cpp
        ../gen-cpp/tutorialmath_constants.cpp
        ../gen-cpp/tutorialmath_types.cpp)

add_executable(server-thrift Server.cpp ${APP_FILES})
target_link_libraries(server-thrift -lthrift)
