# Tests

# Helper function to add parser tests
function(add_paracl_test test_name input_file expected_output)
    add_test(
        NAME ${test_name}
        COMMAND bash -c "$<TARGET_FILE:paracl> ${input_file} | diff -q - <(echo '${expected_output}')"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endfunction()

# Smoke test
add_test(
    NAME paracl_smoke
    COMMAND paracl "${CMAKE_SOURCE_DIR}/examples/hello.pcl"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
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
    COMMAND paracl "${CMAKE_SOURCE_DIR}/tests/test_error_undeclared.pcl"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)
set_tests_properties(test_error_undeclared PROPERTIES WILL_FAIL TRUE)
