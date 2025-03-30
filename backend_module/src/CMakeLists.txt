include_directories(${PYBIND_PATH})



set(backend_module_src 
    WSNManager.cpp
    Simulation.cpp
    Sensor.cpp
)

pybind11_add_module(backend_module bind.cpp ${backend_module_src})

set_target_properties(backend_module PROPERTIES OUTPUT_NAME "backend_module" SUFFIX ".pyd")

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/lib/backend_module")

install(TARGETS backend_module
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
)

add_executable(cpp_test test.cpp ${backend_module_src})

#set(BACKEND_SRC main.cpp)
#add_library(backend SHARED ${BACKEND_SRC})
#target_link_libraries()