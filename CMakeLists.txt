cmake_minimum_required(VERSION 3.14)
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    project(DumpSTL VERSION 0.0.1 DESCRIPTION "It is debug tool for debugging 3d geometry and 3d models" LANGUAGES CXX)
endif()

add_library(DumpSTL INTERFACE)
target_include_directories(DumpSTL INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(DumpSTL INTERFACE cxx_std_17)

add_library(DumpSTL::DumpSTL ALIAS DumpSTL)


option(BUILD_DumpSTL_EXAMPLE "Build DumpSTL example" ${is_top_level})

if(BUILD_DumpSTL_EXAMPLE)
    add_subdirectory(example)
endif()