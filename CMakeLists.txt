cmake_minimum_required (VERSION 3.10)

project ("blockchain" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# Dependencies
#

# --- Google Test ---
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

FetchContent_MakeAvailable(googletest)

# --- Libsodium ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)

set(SODIUM_INCLUDE_DIRS ${SODIUM_INCLUDE_DIRS} CACHE INTERNAL "")
set(SODIUM_LIBRARY_DIRS ${SODIUM_LIBRARY_DIRS} CACHE INTERNAL "")
set(SODIUM_LIBRARIES ${SODIUM_LIBRARIES} CACHE INTERNAL "")

# build source
enable_testing()
add_subdirectory(src)
