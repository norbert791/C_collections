#include <graph_adj_matrix.h>
#include <stdlib.h>
#include <stdint.h>

struct GraphAdjMatrix
{
  size_t no_vertices;
  size_t no_edges;
  Vector* vertices;
  Matrix* adjacency;
};

#define GRAPH_FIELD_NULL_CHECK(field) \
  do { \
    if ((field) == NULL) \
    { \
      graph_destroy(g); \
      return NULL; \
    } \
  } while(0)

Graph* graph_create(const size_t no_vertices)
{
  if (no_vertices == 0)
    return NULL;

  Graph* const g = malloc(sizeof(*g));

  if (g == NULL)
    return NULL;
  
  *g = (Graph){
                .no_edges = 0,
                .no_vertices = no_vertices,
              };

  g->vertices = vector_create(sizeof(Vertice), VECTOR_DEFAULT_CAPACITY_OPTION);
  GRAPH_FIELD_NULL_CHECK(g->vertices);

  for (size_t i = 0; i < no_vertices; i++)
    vector_push_back(&g->vertices, &(Vertice){.id = i, .weight = GRAPH_WEIGHT_ZERO});

  g->adjacency = matrix_create(no_vertices, no_vertices, sizeof(graph_weight_t));
  GRAPH_FIELD_NULL_CHECK(g->adjacency);

  return g;
}

void graph_destroy(Graph* const g)
{
  if (g == NULL)
    return;

  // Each function checks for nulls, so it's ok to omit null checking here.
  vector_destroy(g->vertices);
  matrix_destroy(g->adjacency);

  free(g);
}

size_t graph_get_no_vertices(const Graph* const g)
{
  if (g == NULL)
    return 0;

  return g->no_vertices;
}

size_t graph_get_no_edges(const Graph* const g)
{
  if (g == NULL)
    return 0;

  return g->no_edges;
}

graph_weight_t graph_get_vertice_weight(const Graph* const g, const size_t id)
{
  if (g == NULL)
    return GRAPH_WEIGHT_ZERO;

  if (id >= g->no_vertices)
    return GRAPH_WEIGHT_ZERO;

  Vertice* const vertice = malloc(sizeof(*vertice));

  if (vertice == NULL)
    return GRAPH_WEIGHT_ZERO;

  register const int vector_at_result = vector_at(g->vertices, id, vertice);

  if (vector_at_result != 0)
    return GRAPH_WEIGHT_ZERO;

  register const graph_weight_t result = vertice->weight;

  free(vertice);

  return result;
}

graph_weight_t graph_get_edge_weight(const Graph* const g, const size_t src, const size_t dest)
{
  if (g == NULL)
    return GRAPH_WEIGHT_ZERO;

  if (src >= g->no_vertices || dest >= g->no_vertices)
    return GRAPH_WEIGHT_ZERO;

  graph_weight_t* const weight = malloc(sizeof(*weight));

  if (weight == NULL)
    return GRAPH_WEIGHT_ZERO;

  register const int matrix_at_result = matrix_at(g->adjacency, src, dest, weight);

  if (matrix_at_result != 0)
    return GRAPH_WEIGHT_ZERO;

  register const graph_weight_t result = *weight;

  free(weight);

  return result;
}

bool graph_has_edge(const Graph* const g, const size_t src, const size_t dest)
{
  if (g == NULL)
    return false;

  if (src >= g->no_vertices || dest >= g->no_vertices)
    return false;
  
  graph_weight_t* const weight = malloc(sizeof(*weight));

  if (weight == NULL)
    return false;

  register const int adjacency_at_result = matrix_at(g->adjacency, src, dest, weight);

  if (adjacency_at_result != 0)
    return false;

  register const bool result = (*weight != GRAPH_WEIGHT_ZERO);

  free(weight);

  return result;
}

int graph_update_vertice(Graph* const g, const size_t id, const graph_weight_t new_weight)
{
  if (g == NULL)
    return -1;
  
  if (id >= g->no_vertices)
    return -1;

  register const int vector_update_result = vector_update(g->vertices, id, &(Vertice){.id = id, .weight = new_weight});

  if (vector_update_result != 0)
    return 1;

  return 0;
}

int graph_update_edge(Graph* const g, const size_t src, const size_t dest, const graph_weight_t new_weight)
{
  if (g == NULL)
    return -1;
  
  if (src >= g->no_vertices || dest >= g->no_vertices || new_weight == GRAPH_WEIGHT_ZERO)
    return -1;

  register const int matrix_update_result = matrix_update(g->adjacency, src, dest, &(graph_weight_t){new_weight});

  if (matrix_update_result != 0)
    return 1;

  return 0;
}

int graph_update_edge_symmetric(Graph* const g, const size_t src, const size_t dest, const graph_weight_t new_weight)
{
  if (g == NULL)
    return -1;
  
  if (src >= g->no_vertices || dest >= g->no_vertices || new_weight == GRAPH_WEIGHT_ZERO)
    return -1;

  register const int matrix_update_result = matrix_update_symmetric(g->adjacency, src, dest, &(graph_weight_t){new_weight});

  if (matrix_update_result != 0)
    return 1;

  return 0;
}

int graph_update_vertices_from_array(Graph* g, const Vertice vertices[], size_t no_veritces)
{
  if (g == NULL || vertices == NULL)
    return -1;

  if (no_veritces == 0)
    return -1;

  for (size_t i = 0; i < no_veritces; i++)
  {
    int result = graph_update_vertice(g, vertices[i].id, vertices[i].weight);
    if (result != 0)
      return -i - 2; // (res + 1) * -1 should give an index
  }

  return 0;
}

int graph_update_edges_from_array(Graph* g, const Edge edges[], size_t no_edges)
{
  if (g == NULL || edges == NULL)
    return -1;

  if (no_edges == 0)
    return -1;

  for (size_t i = 0; i < no_edges; i++)
  {
    int result = graph_update_edge(g, edges[i].src, edges[i].dest, edges[i].weight);
    if (result != 0)
      return -i - 2; // (res + 1) * -1 should give an index
  }

  return 0;
}

int graph_add_vertice(Graph *g, graph_weight_t weight)
{
  if (g == NULL)
    return -1;

  // Add new vertice to the vector

  int vector_push_back_result = 
    vector_push_back(&g->vertices, &(Vertice){
                                              .id = g->no_vertices, 
                                              .weight = GRAPH_WEIGHT(weight)
                                            });

  if (vector_push_back_result != 0)
    return 1;

  g->no_vertices++;

  // Expand adjacency matrix

  int matrix_expand_result = matrix_expand(&g->adjacency, 1);
  
  if (matrix_expand_result != 0)
    return 1;

  return 0;
}

int graph_add_edge(Graph* g, size_t src, size_t dest, graph_weight_t new_weight)
{
  int result = graph_update_edge(g, src, dest, new_weight);

  if (result != 0)
    return result;

  g->no_edges++;

  return result;
}

int graph_add_edge_symmetric(Graph* const g, const size_t src, const size_t dest, const graph_weight_t new_weight)
{
  int result = graph_update_edge_symmetric(g, src, dest, new_weight);

  if (result != 0)
    return result;

  g->no_edges++;

  return result;
}

Vector* graph_copy_vertices(const Graph* g)
{
  if (g == NULL)
    return NULL;

  return vector_copy(g->vertices);
}

Vector* graph_vertice_neighbours(const Graph* g, size_t id)
{
  if (g == NULL)
    return NULL;

  if (id >= g->no_vertices)
    return NULL;

  Vector* neighbours = vector_create(sizeof(Edge), g->no_vertices);

  if (neighbours == NULL)
    return NULL;

  for (size_t i = 0; i < g->no_vertices; i++)
  {
    graph_weight_t edge_weight = graph_get_edge_weight(g, id, i);
    if (edge_weight != GRAPH_WEIGHT_ZERO)
    {
      int result = vector_push_back(&neighbours, &(Edge){ 
                                                          .src = id,
                                                          .dest = i,
                                                          .weight = edge_weight
                                                        });
      if (result != 0)
      {
        vector_destroy(neighbours);
        return NULL;
      }
    }
  }

  return neighbours;
}