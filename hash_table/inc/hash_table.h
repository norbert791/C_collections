#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>

typedef struct HashTable HashTable;

HashTable* hash_table_create(size_t value_size, size_t initial_size,
                            size_t (*hash_function)(void*), int (*comperator)(void*, void*));

void hash_table_destroy(HashTable* table);

int hash_table_insert(HashTable** table, const void* element);

int hash_table_get(const HashTable* table, void* key, void* destination);

int hash_table_remove(HashTable** table, const void* key);

HashTable* hash_table_copy(const HashTable* table);

#endif /*HASH_TABLE_H*/