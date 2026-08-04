add_subdirectory(crypto)
add_subdirectory(ledger)
add_subdirectory(network)

add_executable(mining-demo mining_demo.cpp)

target_link_libraries(mining-demo
    PRIVATE ledger
    PRIVATE crypto
)