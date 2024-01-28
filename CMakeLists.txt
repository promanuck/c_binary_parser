cmake_minimum_required(VERSION 3.1...3.16)

project(c_binary_parser C)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE
    src/coder.c
    src/parser.c 
)

target_include_directories(${PROJECT_NAME} PUBLIC include)