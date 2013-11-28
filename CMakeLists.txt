CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT(compilador)
FIND_PACKAGE(FLEX REQUIRED)
FIND_PACKAGE(BISON REQUIRED)
FLEX_TARGET(scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
BISON_TARGET(parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c VERBOSE parser.output)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${compilador_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES(${compilador_SOURCE_DIR}/src/)
INCLUDE_DIRECTORIES(${compilador_SOURCE_DIR}/iloc_parser/include/)
ADD_EXECUTABLE(main
  ${FLEX_scanner_OUTPUTS}
  ${BISON_parser_OUTPUTS}
  ${BISONILOC_parser_OUTPUTS}
  src/main.c
  src/iks_gv.c
  src/iks_dict.c
  src/iks_list.c
  src/iks_tree.c
  src/iks_graph.c
  src/iks_stack.c
  src/iks_grammar.c
  src/iks_ast.c
  src/iks_types.c
  src/iks_iloc.c
)
TARGET_LINK_LIBRARIES(main ${FLEX_LIBRARIES})
TARGET_LINK_LIBRARIES(main ${compilador_SOURCE_DIR}/iloc_parser/build/parser.o)
TARGET_LINK_LIBRARIES(main ${compilador_SOURCE_DIR}/iloc_parser/build/lex.ilocparser.o)
TARGET_LINK_LIBRARIES(main ${compilador_SOURCE_DIR}/iloc_parser/build/iloc_parser.o)
