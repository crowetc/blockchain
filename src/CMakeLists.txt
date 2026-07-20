add_library(blockchain
    Block.cpp
    Chain.cpp
    Merkle_proof.cpp
    Merkle_tree.cpp
    sha256.cpp
    Transaction.cpp
)

target_include_directories (blockchain
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(main "main.cpp")
target_link_libraries(main blockchain)
