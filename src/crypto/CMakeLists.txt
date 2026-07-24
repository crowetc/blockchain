add_library(crypto
    crypto_utils.cpp
)

target_include_directories(crypto
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${SODIUM_INCLUDE_DIRS}
)

target_link_libraries(crypto
    PRIVATE ${SODIUM_LIBRARIES}
)