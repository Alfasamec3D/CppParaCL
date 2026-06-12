cmake_minimum_required(VERSION 3.16)
project(CppParaCL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable testing before adding subdirectories
enable_testing()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(parser)
add_subdirectory(tests)
