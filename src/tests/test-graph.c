#include <stdio.h>
#include "comp_dict.h"
#include "comp_graph.h"
#include "comp_tree.h"
#include "comp_graph.h"
#include "tokens.h"

int main (int argc, char **argv)
{
    comp_graph_t *g1,*g2,*g3,*g4;
    g1 = new_comp_graph();
    g2 = new_comp_graph();
    g3 = new_comp_graph();
    g4 = new_comp_graph();
    g1->value = (void *)1;
    g2->value = (void *)2;
    g3->value = (void *)3;
    g4->value = (void *)4;

    comp_graph_edge_t *e1,*e2,*e3,*e4;
    e1 = new_comp_graph_edge();
    e2 = new_comp_graph_edge();
    e3 = new_comp_graph_edge();
    e4 = new_comp_graph_edge();

    comp_graph_edge_set(e1,g1,g2);
    comp_graph_edge_set(e2,g1,g3);
    comp_graph_edge_set(e3,g4,g1);
    comp_graph_edge_set(e4,g4,g3);

    comp_graph_edge_print(e1);
    comp_graph_edge_print(e2);
    comp_graph_edge_print(e3);
    comp_graph_edge_print(e4);

    comp_graph_print(g1);
    comp_graph_append(g1,g2);
    comp_graph_print(g1);
    comp_graph_print(g1);
    comp_graph_concat(g1,g3);
    comp_graph_insert(g1,g4);
    comp_graph_delete(g3);
    comp_graph_print(g1);
    comp_graph_remove(g1,g2);
    comp_graph_remove(g1,g3);
    comp_graph_print(g1);

    return 0;
}
