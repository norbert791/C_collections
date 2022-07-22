#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define VECTOR_INITIAL_CAPACITY 4
#define VECTOR_DEFAULT_CAPACITY_OPTION 0
#define VECTOR_MAXIMUM_CAPACITY 65536

typedef struct Vector Vector;

Vector* vector_create(size_t elem_size, size_t max_size);
void vector_destroy(Vector* v);

size_t vector_get_elem_size(const Vector *v);
size_t vector_get_curr_size(const Vector *v);
size_t vector_get_capacity(const Vector *v);
size_t vector_get_max_size(const Vector *v);

bool vector_is_empty(const Vector *v);
bool vector_is_full(const Vector *v);

int vector_resize(Vector** v, size_t new_size);

int vector_push_back(Vector** restrict v, const void* restrict elem);

int vector_pop_back(Vector* restrict v, void* restrict elem);

size_t vector_find(const Vector* restrict v, const void* restrict elem);
int vector_at(Vector* restrict v, size_t index, void* restrict elem);

int vector_update(Vector* restrict v, size_t index, const void* restrict new_value);
int vector_delete(Vector* v, size_t index);

#endif // !VECTOR_H