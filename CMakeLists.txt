cmake_minimum_required(VERSION 3.10.0)
project(backend_module VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(VENV_PATH "${CMAKE_SOURCE_DIR}/.venv")

set(pybind11_DIR ${CMAKE_SOURCE_DIR}/.venv/Lib/site-packages/pybind11/share/cmake/pybind11)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG REQUIRED)

message(STATUS "Python3_EXECUTABLE: ${Python3_EXECUTABLE}")

add_subdirectory(backend_module)
