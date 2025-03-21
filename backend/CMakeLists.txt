
# # Dodanie pybind11 (jeśli jest zainstalowany w venv)
# set(PYBIND_PATH "${VENV_PATH}/Lib/site-packages/pybind11/include")  # Windows
# #include_directories("${VENV_PATH}/include/python3.10")              # Linux/Mac

# set(pybind11_DIR ${CMAKE_SOURCE_DIR}/.venv/Lib/site-packages/pybind11/share/cmake/pybind11)
# find_package(pybind11 REQUIRED)
# message(STATUS "pybind11_INCLUDE_DIRS: ${pybind11_INCLUDE_DIRS}")


# message(${Python3_INCLUDE_DIRS} " bla bla bla ---------------------")

# set(Python3_LIBRARIES "")

# set(PYBIND_PATH ${CMAKE_SOURCE_DIR}/.venv/Lib/site-packages/pybind11/include)
# include_directories(${Python3_INCLUDE_DIRS})
# include_directories(${PYBIND_PATH})

include_directories(include)
add_subdirectory(src)