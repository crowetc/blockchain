add_library(blockchain
    Block.cpp
    Chain.cpp
    crypto_utils.cpp
    Merkle_proof.cpp
    Merkle_tree.cpp
    Transaction.cpp
)

target_include_directories (blockchain
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${SODIUM_INCLUDE_DIRS}
)

target_link_libraries(blockchain
    PUBLIC ${SODIUM_LIBRARIES}
)

add_executable(main "main.cpp")

target_link_libraries(main
    PRIVATE blockchain
    PRIVATE ${SODIUM_LIBRARIES}
)