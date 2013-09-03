CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT(compilador)
FIND_PACKAGE(FLEX REQUIRED)
FIND_PACKAGE(BISON REQUIRED)
FLEX_TARGET(scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
BISON_TARGET(parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c VERBOSE parser.output)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${compilador_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES(${compilador_SOURCE_DIR}/src/)
ADD_EXECUTABLE(main
  ${FLEX_scanner_OUTPUTS}
  ${BISON_parser_OUTPUTS}
  src/main.c
  src/gv.c
  src/comp_dict.c
  src/comp_list.c
  src/comp_tree.c
  src/comp_graph.c
)
TARGET_LINK_LIBRARIES(main ${FLEX_LIBRARIES})