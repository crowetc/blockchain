cmake_minimum_required (VERSION 3.10)

project ("blockchain" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# build source
add_subdirectory(src)

# build tests
add_subdirectory(test)
