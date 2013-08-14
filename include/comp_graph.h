#ifndef COMP_GRAPH_H
#define COMP_GRAPH_H

#include "comp_list.h"

/**
 * Graph struct
 */
typedef struct comp_graph_t comp_graph_t;
struct comp_graph_t {
    void *value; /*!< Valued graph. Not present at specification. Defined to tests. */
    comp_list_t *edges; /*!< Pointer to the list of edges connected to this vertex */
    comp_graph_t *prev; /*!< Pointer to previous graph vertex */
    comp_graph_t *next; /*!< Pointer to next graph vertex */
};

/**
 * Graph edge struct
 * Permit oriented graph. Not present at specification.
 */
typedef struct comp_graph_edge_t comp_graph_edge_t;
struct comp_graph_edge_t {
    comp_graph_t *src_vertex; /*!< Pointer to src vertex */
    comp_graph_t *dst_vertex; /*!< Pointer to dst vertex */
};


/**
 * Initialize graph with value as NULL and next and prev pointing to graph itself
 * just for internal use.
 */
static inline void __comp_graph_init(comp_graph_t *graph);
/**
 * Return 1 if graph is empty
 */
static inline int __comp_graph_is_empty(comp_graph_t *graph);
/**
 * Create a graph, alloc mem and return address
 */
comp_graph_t *new_comp_graph();
/**
 * Free address memory used by graph
 */
void comp_graph_delete(comp_graph_t *graph);
/**
 * add vertex as the last in the graph
 */
void comp_graph_append(comp_graph_t *graph, comp_graph_t *new_vertex);
/**
 * add vertex as the first in the graph
 */
void comp_graph_insert(comp_graph_t *graph, comp_graph_t *new_vertex);
/*
 * remove vertex
 */
void comp_graph_remove(comp_graph_t *graph, comp_graph_t *vertex_to_remove);
/*
 * concat two graphs
 */
void comp_graph_concat(comp_graph_t *graph1, comp_graph_t *graph2);
/*
 * print graph values
 */
void comp_graph_print(comp_graph_t *graph) ;


/**
 * initialize graph_edge
 */
static inline void __comp_graph_edge_init(comp_graph_edge_t *graph_edge);
/**
 * create a graph_edge, alloc mem and return address
 */
comp_graph_edge_t *new_comp_graph_edge();
/**
 * free address memory used by graph edge
 */
void comp_graph_edge_delete(comp_graph_edge_t *graph_edge);
/**
 * get graph_edge src vertex
 */
comp_graph_t *graph_edge_src_vertex_get(comp_graph_edge_t *graph_edge);
/**
 * get graph_edge dst vertex
 */
comp_graph_t *graph_edge_dst_vertex_get(comp_graph_edge_t *graph_edge);
/**
 * set graph_edge src,dst vertex. Return 1 if set ok, 0 if not
 */
int comp_graph_edge_set(comp_graph_edge_t *graph_edge, comp_graph_t *src_vertex, comp_graph_t *dst_vertex);
/**
 * print graph_edge src,dst vertex
 */
void comp_graph_edge_print(comp_graph_edge_t *graph_edge);

#endif /* COMP_GRAPH_H */
