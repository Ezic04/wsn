include_directories(${PYBIND_PATH})

set(core_src 
    core/Simulation.cpp
    core/Sensor.cpp
    core/GenerateLDGraph.cpp
)

set(api_src 
    api/bind.cpp
    api/SimulationManager.cpp
)

pybind11_add_module(backend_module ${core_src} ${api_src})

target_include_directories(backend_module PRIVATE ${include_dir_path})

set_target_properties(backend_module PROPERTIES OUTPUT_NAME "backend_module" SUFFIX ".pyd")

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/lib/backend_module")

install(TARGETS backend_module
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
)

add_executable(cpp_test test.cpp ${core_src})

target_include_directories(cpp_test PRIVATE ${include_dir_path})