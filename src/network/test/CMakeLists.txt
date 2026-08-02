# Helper function to add tests
function(add_blockchain_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name}
        PRIVATE network
        PRIVATE ledger
        PRIVATE crypto
        PRIVATE gtest_main
    )
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_blockchain_test(test_message)
add_blockchain_test(test_node)
