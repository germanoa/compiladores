CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT(compilador)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${compilador_SOURCE_DIR}/include/)
INCLUDE_DIRECTORIES(${compilador_SOURCE_DIR}/src/)
ADD_EXECUTABLE(main
  src/main.c
  src/comp_dict.c
  src/comp_list.c
  src/comp_tree.c
  src/comp_graph.c
)
