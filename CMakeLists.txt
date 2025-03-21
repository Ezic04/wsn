cmake_minimum_required(VERSION 3.10.0)
project(backend VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)


# # Ścieżka do venv
set(VENV_PATH "${CMAKE_SOURCE_DIR}/.venv")

set(pybind11_DIR ${CMAKE_SOURCE_DIR}/.venv/Lib/site-packages/pybind11/share/cmake/pybind11)


# # Ręczne ustawienie ścieżek do Pythona w venv
# set(Python3_EXECUTABLE "${VENV_PATH}/Scripts/python.exe")  # Windows
# #set(Python3_EXECUTABLE "${VENV_PATH}/bin/python3")       # Linux/Mac

# set(Python3_INCLUDE_DIR "${VENV_PATH}/Include")            # Windows
# #set(Python3_INCLUDE_DIR "${VENV_PATH}/include/python3.10")# Linux/Mac

# set(Python3_LIBRARY "${VENV_PATH}/libs/python310.lib")     # Windows
# #set(Python3_LIBRARY "${VENV_PATH}/lib/libpython3.10.so")  # Linux/Mac

#set(Python3_ROOT ${VENV_PATH})
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 CONFIG REQUIRED)

# # Wypisanie ścieżek (debug)
message(STATUS "Python3_EXECUTABLE: ${Python3_EXECUTABLE}")
# message(STATUS "Python3_INCLUDE_DIR: ${Python3_INCLUDE_DIR}")
# message(STATUS "Python3_LIBRARY: ${Python3_LIBRARY}")

add_subdirectory(backend)
