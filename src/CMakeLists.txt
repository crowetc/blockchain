add_subdirectory(crypto)
add_subdirectory(ledger)

add_executable(example example.cpp)

target_link_libraries(example
    PRIVATE ledger
    PRIVATE crypto
)