cmake_minimum_required(VERSION 3.1...3.16)

project(bitp CXX C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE include)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)) 
    add_executable(${PROJECT_NAME}_example)
    
    target_sources(${PROJECT_NAME}_example PRIVATE example/example.cpp)
    
    target_link_libraries(${PROJECT_NAME}_example PRIVATE ${PROJECT_NAME})
    
    if (BUILD_TESTING)
        enable_testing()
        add_subdirectory(tests)
    endif()
endif()