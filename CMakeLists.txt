cmake_minimum_required(VERSION 3.1...3.16)

project(c_binary_parser CXX C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE include)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTING) 
    enable_testing()
    add_subdirectory(tests)
endif()