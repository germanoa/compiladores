#include "iks_graph.h"
#include <stdlib.h>
#include <stdio.h>

static inline void __iks_graph_init(iks_graph_t *graph) {
	graph->value = NULL; 
	graph->edges = new_iks_list(); 
    graph->prev = graph;
    graph->next = graph;
}

inline int iks_graph_is_empty(iks_graph_t *graph) {
	if(graph == NULL)
		return 1;
    else return graph == graph->next;
}

iks_graph_t *new_iks_graph() {
    iks_graph_t *graph;
    graph = malloc(sizeof(iks_graph_t));
    __iks_graph_init(graph);
    return graph;
}

void iks_graph_delete(iks_graph_t *graph) {
    if (!iks_graph_is_empty(graph)) {
        iks_graph_t *temp;
        temp = graph->next;
        do {
            temp = temp->next;    
            free(temp->prev);
            temp->prev = NULL;
        } while(temp != graph);
    }
    free(graph);
    graph = NULL; /* http://stackoverflow.com/questions/1025589/setting-variable-to-null-after-free */
}

void iks_graph_append(iks_graph_t *graph, iks_graph_t *new_vertex) {
    iks_graph_t *vertex_to_append;
    /* to append the same graph any times we need a new one, but
       when active this, we cant find the graph by reference, just by value value */
    //vertex_to_append = new_iks_graph();
    //vertex_to_append->value = new_vertex->value;

    vertex_to_append = new_vertex;
    
    graph->prev->next = vertex_to_append;
    vertex_to_append->prev = graph->prev;
    vertex_to_append->next = graph;
    graph->prev = vertex_to_append;
}

void iks_graph_insert(iks_graph_t *graph, iks_graph_t *new_vertex) {
    iks_graph_t *vertex_to_insert;
    /* same as append */
    //vertex_to_insert = new_iks_graph();
    //vertex_to_insert->value = new_vertex->value;

    vertex_to_insert = new_vertex;

    graph->next->prev = vertex_to_insert;
    vertex_to_insert->prev = graph;
    vertex_to_insert->next = graph->next;
    graph->next = vertex_to_insert;
}

void iks_graph_remove(iks_graph_t *graph, iks_graph_t *vertex_to_remove) {
    iks_graph_t *temp;
    temp = graph->next;
    while(temp != graph) {
        if (temp == vertex_to_remove) {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            break;
        }
        temp = temp->next;
    }
}

void iks_graph_concat(iks_graph_t *graph1, iks_graph_t *graph2) {
    graph1->prev->next = graph2;
    graph2->prev->next = graph1;
    iks_graph_t *temp_graph1_prev;
    temp_graph1_prev = graph1->prev;
    graph1->prev = graph2->prev;
    graph2->prev = temp_graph1_prev;
}

void iks_graph_print(iks_graph_t *graph) {
    iks_graph_t *temp;
    temp = graph;
    do {
        printf("%d,",(int)temp->value);
        temp = temp->next;    
    } while(temp != graph);
    printf("\n");
}

static inline void __iks_graph_edge_init(iks_graph_edge_t *graph_edge) {
    graph_edge->src_vertex = NULL;
    graph_edge->dst_vertex = NULL;
}

iks_graph_edge_t *new_iks_graph_edge() {
    iks_graph_edge_t *graph_edge;
    graph_edge = malloc(sizeof(iks_graph_edge_t));
    __iks_graph_edge_init(graph_edge);
    return graph_edge;
}

void iks_graph_edge_delete(iks_graph_edge_t *graph_edge) {
    /*
    * disable because we defined that deleting a edge we dont delete respective vertex's.
    free(graph_edge->src_vertex);
    graph_edge->src_vertex = NULL;
    free(graph_edge->dst_vertex);
    graph_edge->dst_vertex = NULL;
    */
    free(graph_edge);
    graph_edge = NULL;
}

iks_graph_t *graph_edge_src_vertex_get(iks_graph_edge_t *graph_edge) {
    return graph_edge->src_vertex;
}

iks_graph_t *graph_edge_dst_vertex_get(iks_graph_edge_t *graph_edge) {
    return graph_edge->dst_vertex;
}

int iks_graph_edge_set(iks_graph_edge_t *graph_edge, iks_graph_t *src_vertex, iks_graph_t *dst_vertex) {
    int ret = 1;
    graph_edge->src_vertex = malloc(sizeof(iks_graph_edge_t));
    graph_edge->src_vertex = src_vertex;
    graph_edge->dst_vertex = malloc(sizeof(iks_graph_edge_t));
    graph_edge->dst_vertex = dst_vertex;
    iks_list_append(src_vertex->edges,(void*)graph_edge);
    iks_list_append(dst_vertex->edges,(void*)graph_edge);
    return ret;
}

void iks_graph_edge_print(iks_graph_edge_t *graph_edge) {
    printf("%X -> %X\n",graph_edge->src_vertex,graph_edge->dst_vertex);
}

