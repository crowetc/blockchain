add_library(network
    Node.cpp
    Tcp_peer.cpp
)

target_include_directories (network
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(network
    PUBLIC crypto
    Public ledger
)