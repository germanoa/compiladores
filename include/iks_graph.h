#ifndef __IKS_GRAPH_H__
#define __IKS_GRAPH_H__

#include "iks_list.h"

/**
 * Graph struct
 */
typedef struct iks_graph_t iks_graph_t;
struct iks_graph_t {
    void *value; /*!< Valued graph. Not present at specification. Defined to tests. */
    iks_list_t *edges; /*!< Pointer to the list of edges connected to this vertex */
    iks_graph_t *prev; /*!< Pointer to previous graph vertex */
    iks_graph_t *next; /*!< Pointer to next graph vertex */
};

/**
 * Graph edge struct
 * Permit oriented graph. Not present at specification.
 */
typedef struct iks_graph_edge_t iks_graph_edge_t;
struct iks_graph_edge_t {
    iks_graph_t *src_vertex; /*!< Pointer to src vertex */
    iks_graph_t *dst_vertex; /*!< Pointer to dst vertex */
};


/**
 * Initialize graph with value as NULL and next and prev pointing to graph itself
 * just for internal use.
 */
static inline void __iks_graph_init(iks_graph_t *graph);
/**
 * Return 1 if graph is empty
 */
inline int iks_graph_is_empty(iks_graph_t *graph);
/**
 * Create a graph, alloc mem and return address
 */
iks_graph_t *new_iks_graph();
/**
 * Free address memory used by graph
 */
void iks_graph_delete(iks_graph_t *graph);
/**
 * add vertex as the last in the graph
 */
void iks_graph_append(iks_graph_t *graph, iks_graph_t *new_vertex);
/**
 * add vertex as the first in the graph
 */
void iks_graph_insert(iks_graph_t *graph, iks_graph_t *new_vertex);
/*
 * remove vertex
 */
void iks_graph_remove(iks_graph_t *graph, iks_graph_t *vertex_to_remove);
/*
 * concat two graphs
 */
void iks_graph_concat(iks_graph_t *graph1, iks_graph_t *graph2);
/*
 * print graph values
 */
void iks_graph_print(iks_graph_t *graph) ;


/**
 * initialize graph_edge
 */
static inline void __iks_graph_edge_init(iks_graph_edge_t *graph_edge);
/**
 * create a graph_edge, alloc mem and return address
 */
iks_graph_edge_t *new_iks_graph_edge();
/**
 * free address memory used by graph edge
 */
void iks_graph_edge_delete(iks_graph_edge_t *graph_edge);
/**
 * get graph_edge src vertex
 */
iks_graph_t *graph_edge_src_vertex_get(iks_graph_edge_t *graph_edge);
/**
 * get graph_edge dst vertex
 */
iks_graph_t *graph_edge_dst_vertex_get(iks_graph_edge_t *graph_edge);
/**
 * set graph_edge src,dst vertex. Return 1 if set ok, 0 if not
 */
int iks_graph_edge_set(iks_graph_edge_t *graph_edge, iks_graph_t *src_vertex, iks_graph_t *dst_vertex);
/**
 * print graph_edge src,dst vertex
 */
void iks_graph_edge_print(iks_graph_edge_t *graph_edge);

#endif /* __IKS_GRAPH_H__ */
