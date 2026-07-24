add_library(ledger
    Block.cpp
    Chain.cpp
    Merkle_proof.cpp
    Merkle_tree.cpp
    Transaction.cpp
)

target_include_directories (ledger
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ledger
    PUBLIC crypto
)

add_subdirectory(test)