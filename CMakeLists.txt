cmake_minimum_required(VERSION 3.16)
project(CppParaCL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

if(NOT BISON_FOUND)
    message(FATAL_ERROR "Bison is required. Install it with: sudo apt install bison flex")
endif()

# Support vendored bison extracted under build/tools (offline/apt download workflow).
set(_VENDORED_BISON_SHARE "${CMAKE_CURRENT_BINARY_DIR}/tools/bison/root/usr/share/bison")
if(EXISTS "${_VENDORED_BISON_SHARE}/m4sugar/m4sugar.m4")
    set(ENV{BISON_PKGDATADIR} "${_VENDORED_BISON_SHARE}")
endif()
unset(_VENDORED_BISON_SHARE)

set(PARSER_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${PARSER_GEN_DIR}")

bison_target(
    CompilerParser
    "${CMAKE_CURRENT_SOURCE_DIR}/parser/compiler.y"
    "${PARSER_GEN_DIR}/compiler.cpp"
    DEFINES_FILE "${PARSER_GEN_DIR}/compiler.hpp"
)

flex_target(
    CompilerLexer
    "${CMAKE_CURRENT_SOURCE_DIR}/parser/lexer.l"
    "${PARSER_GEN_DIR}/lexer.cpp"
)

add_flex_bison_dependency(CompilerLexer CompilerParser)

add_library(paracl_ast STATIC
    src/Node.cpp
)
target_include_directories(paracl_ast PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_executable(paracl
    parser/driver.cpp
    ${BISON_CompilerParser_OUTPUTS}
    ${FLEX_CompilerLexer_OUTPUTS}
)

target_include_directories(paracl PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/parser"
    "${PARSER_GEN_DIR}"
)

target_link_libraries(paracl PRIVATE paracl_ast)

if(FLEX_LIBRARIES)
    target_link_libraries(paracl PRIVATE ${FLEX_LIBRARIES})
endif()

enable_testing()

# Helper function to add parser tests
function(add_paracl_test test_name input_file expected_output)
    add_test(
        NAME ${test_name}
        COMMAND bash -c "$<TARGET_FILE:paracl> ${input_file} | diff -q - <(echo '${expected_output}')"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
endfunction()

# Smoke test
add_test(
    NAME paracl_smoke
    COMMAND paracl "${CMAKE_CURRENT_SOURCE_DIR}/examples/hello.pcl"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

# Lexer and parser tests
add_paracl_test(test_lexer "tests/test_lexer.pcl" "142")
add_paracl_test(test_operators "tests/test_operators.pcl" "13\n7\n30\n3\n1")
add_paracl_test(test_comparisons "tests/test_comparisons.pcl" "1\n3\n4\n5\n6")
add_paracl_test(test_logical "tests/test_logical.pcl" "2\n3\n4")
add_paracl_test(test_parentheses "tests/test_parentheses.pcl" "90\n6")
add_paracl_test(test_nested_scopes "tests/test_nested_scopes.pcl" "6\n3\n1")
add_paracl_test(test_while "tests/test_while.pcl" "0\n1\n2\n3\n4\n55")
add_paracl_test(test_if_while_combined "tests/test_if_while_combined.pcl" "2\n4\n6\n8\n10")

# Error handling test (should fail to parse)
add_test(
    NAME test_error_undeclared
    COMMAND paracl "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_error_undeclared.pcl"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
set_tests_properties(test_error_undeclared PROPERTIES WILL_FAIL TRUE)
