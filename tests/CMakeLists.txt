cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(c_binary_parser_tests VERSION 1.0.0 LANGUAGES C)

include(download_gtest.cmake)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE 
    parser_tests_with_checkers.cpp 
    parser_without_checkers.cpp
    packer_tests_with_checkers.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE gtest_main bitp)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
