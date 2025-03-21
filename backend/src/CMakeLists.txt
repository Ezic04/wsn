
pybind11_add_module(backend bind.cpp main.cpp)

set_target_properties(backend PROPERTIES OUTPUT_NAME "backend" SUFFIX ".pyd")

set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/lib/backend_package")

install(TARGETS backend
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
)


#set(BACKEND_SRC main.cpp)
#add_library(backend SHARED ${BACKEND_SRC})
#target_link_libraries()