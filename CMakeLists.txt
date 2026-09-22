cmake_minimum_required (VERSION 3.20)

project(
    blockchain
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# Testing
#

include(CTest)

#
# Dependencies
#

include(FetchContent)

# --- Google Test ---
if(BUILD_TESTING)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )

    FetchContent_MakeAvailable(googletest)
endif()

# --- Libsodium ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(
    SODIUM
    REQUIRED
    IMPORTED_TARGET
    libsodium
)

#
# Project
#

add_subdirectory(src)
