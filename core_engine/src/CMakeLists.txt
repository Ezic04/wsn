include_directories(${PYBIND_PATH})

pybind11_add_module(core_engine bind.cpp main.cpp)

set_target_properties(core_engine PROPERTIES OUTPUT_NAME "core_engine" SUFFIX ".pyd")

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/lib/core_engine")

install(TARGETS core_engine
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
)


#set(BACKEND_SRC main.cpp)
#add_library(backend SHARED ${BACKEND_SRC})
#target_link_libraries()