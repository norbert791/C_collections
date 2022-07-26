#ifndef GRAPH_ADJ_MATRIX_H

#include <stddef.h>
#include <stdbool.h>

#define graph_weight_t double
#define GRAPH_WEIGHT_ZERO ((graph_weight_t)0)
#define GRAPH_WEIGHT(numvalue) ((graph_weight_t)numvalue)

typedef struct GraphAdjMatrix Graph;

typedef struct Vertice
{
  graph_weight_t weight;
  size_t id;
} Vertice;

typedef struct Edge
{
  graph_weight_t weight;
  size_t src;
  size_t dest;
} Edge;

Graph* graph_create(size_t no_vertices);
void graph_destroy(Graph* g);

size_t graph_get_no_vertices(const Graph* g);
size_t graph_get_no_edges(const Graph* g);
graph_weight_t graph_get_vertice_weight(const Graph* g, size_t id);
graph_weight_t graph_get_edge_weight(const Graph* g, size_t src, size_t dest);

bool graph_has_edge(const Graph* g, size_t src, size_t dest);

int graph_update_vertice(Graph* g, size_t id, graph_weight_t new_weight);
int graph_update_edge(Graph* g, size_t src, size_t dest, graph_weight_t new_weight);
int graph_update_edge_symmetric(Graph* const g, const size_t src, const size_t dest, const graph_weight_t new_weight);

#define GRAPH_ARRAY_UPDATE_ERROR_INDEX(index) \
  ((index) + 2) * -1

int graph_update_vertices_from_array(Graph* g, const Vertice vertices[], size_t no_veritces);
int graph_update_edges_from_array(Graph* g, const Edge edges[], size_t no_edges);

int graph_add_vertice(Graph *g, graph_weight_t weight);

// There's not much difference between adding an edge and updating an edge in matrix implementation
// so might as well alias it
#define graph_add_edge(g, src, dest, new_weight) graph_update_edge(g, src, dest, new_weight)
#define graph_add_edge_symmetric(g, src, dest, new_weight) graph_update_edge_symmetric(g, src, dest, new_weight)

#endif // !GRAPH_ADJ_MATRIX_H