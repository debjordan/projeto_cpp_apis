# Adicionar source files com caminhos absolutos
add_executable(sync_server
    main.cpp
    SyncServer.cpp
    ${CMAKE_SOURCE_DIR}/src/common/Request.cpp
    ${CMAKE_SOURCE_DIR}/src/common/Response.cpp
)

target_link_libraries(sync_server pthread)
