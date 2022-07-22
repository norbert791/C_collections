#include <vector.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// TODO: Add observability
// TODO: Test observability

struct Vector
{
  size_t elem_size;
  size_t curr_size;
  size_t capacity;
  size_t max_size;
  uint8_t elems[];
};

Vector* vector_create(const size_t elem_size, const size_t max_size)
{
  if (elem_size == 0)
    return NULL;

  register const size_t max_capacity = max_size == 0 ? VECTOR_MAXIMUM_CAPACITY : max_size;
  register const size_t initial_capacity = VECTOR_INITIAL_CAPACITY <= max_capacity ? VECTOR_INITIAL_CAPACITY : max_capacity;

  Vector* const v = calloc(1, sizeof(*v) + elem_size * initial_capacity);

  if (v == NULL)
    return NULL;

  v->elem_size = elem_size;
  v->max_size = max_capacity;
  v->capacity = initial_capacity;

  return v;
}

void vector_destroy(Vector* const v)
{
  if (v == NULL)
    return;

  free(v);
}

size_t vector_get_elem_size(const Vector* const v)
{
  if (v == NULL)
    return 0;
  
  return v->elem_size;
}

size_t vector_get_curr_size(const Vector* const v)
{
  if (v == NULL)
    return 0;
  
  return v->curr_size;
}

size_t vector_get_capacity(const Vector* const v)
{
  if (v == NULL)
    return 0;
  
  return v->capacity;
}

size_t vector_get_max_size(const Vector* const v)
{
  if (v == NULL)
    return 0;
  
  return v->max_size;
}

bool vector_is_empty(const Vector* const v)
{
  if (v == NULL)
    return false;

  return v->curr_size == 0;
}

bool vector_is_full(const Vector* const v)
{
  if (v == NULL)
    return false;

  return v->curr_size == v->capacity;
}

int vector_resize(Vector** const v, size_t new_size)
{
  if (v == NULL || *v == NULL)
    return -1;

  if (new_size > (*v)->max_size || new_size == 0)
    return -1;

  register const size_t prev_size = (*v)->curr_size;
  register const size_t elem_size = (*v)->elem_size;

  Vector* const temp = realloc(*v, sizeof(**v) + new_size * elem_size);

  if (temp == NULL)
    return 1;

  temp->capacity = new_size;
  
  if (prev_size > new_size)
    temp->curr_size = prev_size;

  temp->elem_size = elem_size;

  *v = temp;

  return 0;
}

int vector_push_back(Vector** const restrict v, const void* const restrict elem)
{
  if (v == NULL || *v == NULL)
    return -1;

  if (elem == NULL)
    return -1;
  
  if (vector_is_full(*v))
    return 1;

  register const size_t elem_size = (*v)->elem_size;
  register const size_t capacity = (*v)->capacity;

  uint8_t* const ptr = &(*v)->elems[(*v)->curr_size * elem_size];
  memcpy(ptr, elem, elem_size);

  (*v)->curr_size++;

  // Double the size if more than half full
  register const size_t double_size = capacity << 1;
  register const size_t half_size = capacity>> 1;
  if ((*v)->curr_size > half_size)
    (void) vector_resize(v, double_size);

  return 0;
}

int vector_pop_back(Vector* const restrict v, void* const restrict elem)
{
  if (v == NULL)
    return -1;

  if (elem == NULL)
    return -1;
  
  if (vector_is_empty(v))
    return 1;

  register const size_t elem_size = v->elem_size;
  const uint8_t* const ptr = &v->elems[(v->curr_size - 1) * elem_size];

  memcpy(elem, ptr, elem_size);

  v->curr_size--;

  return 0;
}

size_t vector_find(const Vector* const restrict v, const void* const restrict elem)
{
  if (v == NULL || elem == NULL)
    return SIZE_MAX;

  register const size_t curr_size = v->curr_size;

  for (size_t i = 0; i < curr_size; i++)
  {
    if (memcmp(&v->elems[i * v->elem_size], elem, v->elem_size) == 0)
      return i;
  }

  return SIZE_MAX;
}

int vector_at(Vector* const restrict v, const size_t index, void* const restrict elem)
{
  if (v == NULL)
    return -1;

  if (elem == NULL)
    return -1;

  if (index >= v->curr_size)
    return 1;

  const uint8_t* const ptr = &v->elems[index * v->elem_size];
  memcpy(elem, ptr, v->elem_size);

  return 0;
}

int vector_update(Vector* const restrict v, const size_t index, const void* const restrict new_value)
{
  if (v == NULL)
    return -1;

  if (new_value == NULL)
    return -1;

  if (index >= v->curr_size)
    return 1;

  uint8_t* const ptr = &v->elems[index * v->elem_size];
  memcpy(ptr, new_value, v->elem_size);

  return 0;
}

int vector_delete(Vector* const v, const size_t index)
{
  if (v == NULL)
    return -1;

  if (vector_is_empty(v))
    return 1;

  if (index >= v->curr_size)
    return 1;

  if (index == v->curr_size - 1)
  {
    v->curr_size--;
    return 0;
  }

  register const size_t elem_size = v->elem_size;

  uint8_t* to_del = &v->elems[index * elem_size];
  uint8_t* to_move = &v->elems[(index + 1) * elem_size];
  register const size_t no_elems_to_move = v->curr_size - (index + 1); // 0 1 2 4, delete 1: 4 - (1 + 1) = 2
  memmove(to_del, to_move, no_elems_to_move * elem_size);

  v->curr_size--;

  return 0;
}
