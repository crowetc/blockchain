include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

# Helper function to add tests
function(add_blockchain_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE blockchain gtest_main)
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_blockchain_test(test_transaction)
add_blockchain_test(test_merkle)
add_blockchain_test(test_block)
add_blockchain_test(test_chain)
