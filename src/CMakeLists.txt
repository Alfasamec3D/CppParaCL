# AST library
add_library(paracl_ast STATIC
    Node.cpp
)

target_include_directories(paracl_ast PUBLIC
    "${CMAKE_SOURCE_DIR}/include"
)
