# Parser and lexer generation
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

if(NOT BISON_FOUND)
    message(FATAL_ERROR "Bison is required. Install it with: sudo apt install bison flex")
endif()

# Support vendored bison extracted under build/tools (offline/apt download workflow).
set(_VENDORED_BISON_SHARE "${CMAKE_BINARY_DIR}/tools/bison/root/usr/share/bison")
if(EXISTS "${_VENDORED_BISON_SHARE}/m4sugar/m4sugar.m4")
    set(ENV{BISON_PKGDATADIR} "${_VENDORED_BISON_SHARE}")
endif()
unset(_VENDORED_BISON_SHARE)

set(PARSER_GEN_DIR "${CMAKE_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${PARSER_GEN_DIR}")

bison_target(
    CompilerParser
    "${CMAKE_CURRENT_SOURCE_DIR}/compiler.y"
    "${PARSER_GEN_DIR}/compiler.cpp"
    DEFINES_FILE "${PARSER_GEN_DIR}/compiler.hpp"
)

flex_target(
    CompilerLexer
    "${CMAKE_CURRENT_SOURCE_DIR}/lexer.l"
    "${PARSER_GEN_DIR}/lexer.cpp"
)

add_flex_bison_dependency(CompilerLexer CompilerParser)

# Main executable
add_executable(paracl
    driver.cpp
    ${BISON_CompilerParser_OUTPUTS}
    ${FLEX_CompilerLexer_OUTPUTS}
)

target_include_directories(paracl PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${PARSER_GEN_DIR}"
)

target_link_libraries(paracl PRIVATE paracl_ast)

if(FLEX_LIBRARIES)
    target_link_libraries(paracl PRIVATE ${FLEX_LIBRARIES})
endif()
