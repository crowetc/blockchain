
# Helper function to add tests
function(add_blockchain_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name}
        PRIVATE ledger
        PRIVATE crypto
        PRIVATE gtest_main
    )
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_blockchain_test(test_transaction)
add_blockchain_test(test_merkle)
add_blockchain_test(test_block)
add_blockchain_test(test_chain)
