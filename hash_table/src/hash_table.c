#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <hash_table.h>

/*I am doing this with linked list Maciek >:) */
typedef struct HashTableNode {
    struct HashTableNode* next;
    uint8_t data[];
} HashTableNode;

struct HashTable
{
    size_t key_size;
    size_t value_size;
    size_t elements_stored;
    size_t array_size;
    size_t (*hash_function)(const void*);
    int (*is_different)(const void*, const void*);
    HashTableNode* node_array[]; /*FAM*/ 
};

static inline void destroy_list(HashTableNode* list);

static inline void hash_table_fixup(HashTable* table);

static inline HashTableNode* copy_list(const HashTableNode* list, size_t key_size, size_t value_size);

HashTable* hash_table_create(size_t key_size, size_t value_size, size_t initial_size,
                             size_t (*hash_function)(const void*), int (*is_different)(const void*, const void*)) {
    
    if (hash_function == NULL || is_different == NULL 
        || key_size == 0 || value_size == 0) {return NULL;}
    if (initial_size == 0) {initial_size = 32;}

    HashTable* result = calloc(1, sizeof(*result) + sizeof(result->node_array[0]) * initial_size);

    if (result == NULL) {return NULL;}

    result->key_size = key_size;
    result->elements_stored = 0;
    result->array_size = initial_size;
    result->value_size = value_size;
    result->hash_function = hash_function;
    result->is_different = is_different;

    return result;
}

void hash_table_destroy(HashTable* hash_table) {
    if (hash_table != NULL) {
        for (size_t i = 0; i < hash_table->array_size; i++) {
            destroy_list(hash_table->node_array[i]);
        }
        free(hash_table);
    }
}

int hash_table_put(HashTable** hash_table_ptr, register const void* key, register const void* value) {
    HashTable* hash_table = *hash_table_ptr;
    size_t index = hash_table->hash_function(key);
    index %= hash_table->array_size;
    
    if (hash_table->node_array[0] == NULL) {
        HashTableNode* temp = malloc(sizeof(*temp) + sizeof(*temp->data)
                            * (hash_table->value_size + hash_table->key_size));

        if (temp == NULL) {
            return 1;
        }

        temp->next == NULL;
        memcpy(temp->data, key, hash_table->key_size);
        memcpy(&temp->data[hash_table->key_size], value, hash_table->value_size);
        hash_table->elements_stored++;

        if (hash_table->elements_stored * 2 >= hash_table->array_size) {
            hash_table_resize(hash_table_ptr, (*hash_table_ptr)->array_size * 2);
        }

        return 0;
    }

    HashTableNode* it = hash_table->node_array[index];

    while (it->next != NULL && hash_table->is_different(key, it->data) != 0) {
        it = it->next;
    }
    
    if (hash_table->is_different(key, it->data) == 0) {
        memcpy(&it->data[hash_table->key_size], value, hash_table->value_size);
        return 0;
    }

    it->next = calloc(1, sizeof(*it) + sizeof(it->data[0]) * hash_table->value_size);
    
    if (it->next == NULL) {
        return 1;
    }
    
    it = it->next;

    memcpy(it->data, key, hash_table->key_size);
    memcpy(&it->data[hash_table->key_size], value, hash_table->value_size);
    hash_table->elements_stored++; 

    if (hash_table->elements_stored * 2 >= hash_table->array_size) {
        hash_table_resize(hash_table_ptr, (*hash_table_ptr)->array_size * 2);
    }

    return 0;
}

int hash_table_insert(HashTable** hash_table_ptr, register const void* key, register const void* value) {
    
    HashTable* hash_table = *hash_table_ptr;
    size_t index = hash_table->hash_function(key);
    index %= hash_table->array_size;
    
    HashTableNode** node_ptr = &hash_table->node_array[index];

    while (*node_ptr != NULL && hash_table->is_different((*node_ptr)->data, key) != 0) {
        *node_ptr = (*node_ptr)->next;
    }

    if (*node_ptr != NULL) {return 1;}
    
    *node_ptr = malloc(sizeof(**node_ptr) + sizeof((*node_ptr)->data[0])
                    * (hash_table->key_size + hash_table->value_size));
    
    if (*node_ptr == NULL) {return 2;}

    (*node_ptr)->next = NULL;
    memcpy((*node_ptr)->data, key, hash_table->key_size);
    memcpy(&(*node_ptr)->data[hash_table->key_size], value, hash_table->value_size);

    hash_table->elements_stored++;

    if (hash_table->elements_stored > hash_table->array_size / 2) {
        return hash_table_resize(hash_table_ptr, sizeof(*hash_table)
                                + sizeof(hash_table->node_array[0])
                                * hash_table->array_size * 2);
    }
    
    return 0;
}

int hash_table_remove(HashTable** table_ptr, const void* key) {
    HashTable* table = *table_ptr;

    size_t index = table->hash_function(key);
    index %= table->array_size;

    if (table->node_array[index] == NULL) {
        return 1;
    }

    if (table->is_different(key, &table->node_array[0]) == 0) {
        HashTableNode* temp = table->node_array[0]->next;
        free(table->node_array[0]);
        table->node_array[0] = temp;
        table->elements_stored--;
        
        if (table->elements_stored < table->array_size / 4) {
                        hash_table_resize(table_ptr, (*table_ptr)->array_size / 2);

        }

        return 0;
    }

    HashTableNode* it = table->node_array[index];

    while (it->next != NULL && table->is_different(key, &it->next->data[0]) != 0) {it = it->next;}

    if (table->is_different(key, &it->next->data[0]) == 0) {
        HashTableNode* temp = table->node_array[0]->next;
        free(table->node_array[0]);
        table->node_array[0] = temp;
        table->elements_stored--;
        
        if (table->elements_stored < table->array_size / 4) {
             hash_table_resize(table_ptr, (*table_ptr)->array_size / 2);

        }

        return 0;
    }

    return 1;
}

int hash_table_get(register const HashTable* const table,
                    register const void* const key, void* destination) {
    size_t index = table->hash_function(key);
    index %= table->array_size;
    HashTableNode* it = table->node_array[index];

    while (it != NULL && table->is_different(key, &it->data[0]) != 0) {it = it->next;}

    if (it == NULL) {return 1;}

    memcpy(destination, &it->data[table->key_size], table->value_size);
    return 0;
}

HashTable* hash_table_copy(const HashTable* const table) {
    HashTable* result = hash_table_create(table->key_size, table->value_size, table->array_size,
                        table->hash_function, table->is_different);
    if (result == NULL) {return NULL;}

    for (size_t i = 0; i < table->elements_stored; i++) {
        if (table->node_array[i] == NULL) {continue;}

        result->node_array[i] = copy_list(table->node_array[i], table->key_size, table->value_size);

        if (result->node_array == NULL) {
            hash_table_destroy(result);
            return NULL;
        }
    }

    memcpy(result, table, sizeof(*table));

    return result;
}

int hash_table_resize(HashTable** table_ptr, const size_t new_size) {
    if (new_size == 0) {return 2;}
    /*Shrinking case*/
    if (new_size < (*table_ptr)->array_size) {
        HashTableNode* tail = (*table_ptr)->node_array[0];
        
        for (size_t i = new_size; i < (*table_ptr)->array_size; i++) {
            while(tail->next != NULL) {tail = tail->next;}
            tail->next = (*table_ptr)->node_array[i];
        }
    }

    /*Enlargment case*/
    HashTable* result_table = realloc(*table_ptr, sizeof(*result_table)
                                      + sizeof(result_table->node_array[0]) * new_size);

    if (result_table == NULL) {return 1;}

    result_table->array_size = new_size;
    hash_table_fixup(result_table);
    table_ptr = &result_table;
}

static inline HashTableNode* copy_list(const HashTableNode* node, const size_t key_size, const size_t value_size) {
    if (node == NULL) {return NULL;}

    HashTableNode* result = malloc(sizeof(*result) + sizeof(result->data[0]) * (key_size + value_size));

    if (result == NULL) {return NULL;}

    memcpy(result->data, node->data, key_size);

    HashTableNode* ptr = result;

    while (node->next != NULL) {
        node = node->next;
        ptr->next = malloc(sizeof(*result) + sizeof(result->data[0]) * (key_size + value_size));
        
        if (ptr->next == NULL) {
            destroy_list(result);
            return NULL;
        }

        ptr = ptr->next;
        memcpy(ptr->data, node->data, key_size + value_size);
    }

    return result;
}

static inline void hash_table_fixup(HashTable* table) {//change to pointer to pointer
    for (size_t i = 0; i < table->array_size; i++) {
        if (table->node_array[i] == NULL) {continue;}

        HashTableNode** prev = &table->node_array[i];
        HashTableNode* current = table->node_array[i];

        while(current != NULL) {
            size_t new_index = table->hash_function(current->data) % table->array_size;
            if (new_index != i) {
                uint8_t temp_memory[table->value_size + table->key_size];
                memcpy(temp_memory, current->data, table->value_size + table->key_size);
                *prev = current->next;
                free(current);
                current = *prev;
                hash_table_insert(&table, temp_memory, &temp_memory[table->key_size]); 
            }
            else {
                prev = &current->next;
                current = current->next;
            }
        }
    }
}

static inline void destroy_list(HashTableNode* list) {
    HashTableNode* it = list;

    while(it != NULL) {
        HashTableNode* destroyed = it;
        it = it->next;
        free(destroyed);
    }
}
