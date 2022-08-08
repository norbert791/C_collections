#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>

typedef struct HashTable HashTable;

HashTable* hash_table_create(size_t key_size, size_t value_size, size_t initial_size,
                            size_t (*hash_function)(const void*), int (*is_different)(const void*, const void*));

void hash_table_destroy(HashTable* table);

int hash_table_insert(HashTable** table, const void* key, const void* value);

int hash_table_put(HashTable** table, const void* key, const void* value);

int hash_table_get(const HashTable* table, const void* key, void* destination);

int hash_table_remove(HashTable** table, const void* key);

HashTable* hash_table_copy(const HashTable* table);

int hash_table_resize(HashTable** table_ptr, size_t new_size);
#endif /*HASH_TABLE_H*/