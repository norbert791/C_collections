#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <hash_table.h>

/*I am doing this with linked list Maciej >:) */
typedef struct HashTableNode {
    HashTableNode* next;
    uint8_t data[];
} HashTableNode;

struct HashTable
{
    size_t value_size;
    size_t elements_stored;
    size_t array_size;
    size_t (*hash_function)(void*);
    size_t (*comperator)(void*, void*);
    HashTableNode* node_array[]; /*FAM*/ 
};

static inline void double_table_size(HashTable** table);

static inline void cut_table_size(HashTable** table);

static inline void destroy_list(HashTableNode* list);

HashTable* hash_table_create(size_t value_size, size_t initial_size,
                             size_t (*hash_function)(void*), int (*comperator)(void*, void*)) {
    
    if (hash_function == NULL || comperator == NULL) {return NULL;}
    if (initial_size == 0) {initial_size = 32;}

    HashTable* result = calloc(1, sizeof(*result) + sizeof(*result->node_array) * initial_size);

    if (result == NULL) {return NULL;}

    result->elements_stored = 0;
    result->array_size = initial_size;
    result->value_size = value_size;
    result->hash_function = hash_function;
    result->comperator = comperator;

    return result;
}

static void hash_table_destroy(HashTable* hash_table) {
    if (hash_table != NULL) {
        for (size_t i = 0; i < hash_table->array_size; i++) {
            destroy_list(hash_table->node_array[i]);
        }
        free(hash_table);
    }
}

int hash_table_insert(HashTable** hash_table_ptr, const void* element) {
    
    HashTable* hash_table = *hash_table_ptr;
    size_t index = hash_table->hash_function(element);
    index %= hash_table->array_size;
    
    if (hash_table->node_array[0] == NULL) {
        HashTableNode* temp = malloc(sizeof(*temp) + sizeof(*temp->data) * hash_table->value_size);
        if (temp == NULL) {
            return 1;
        }
        temp->next == NULL;
        memcpy(temp->data, element, hash_table->value_size);
        hash_table->elements_stored++;

        if (hash_table->elements_stored * 2 >= hash_table->array_size) {
            double_table_size(hash_table_ptr);
        }

        return 0;
    }

    HashTableNode* it = hash_table->node_array[index];

    while (it->next != NULL && hash_table->comperator(element, &it->data) != 0) {
        it = it->next;
    }
    
    if (hash_table->comperator(element, &it->data[0]) == 0) {
        return 2;
    }

    it->next = malloc(sizeof(*it) + sizeof(it->data[0]) * hash_table->value_size);
    
    if (it == NULL) {
        return 1;
    }
    
    it->next == NULL;
    memcpy(it->data, element, hash_table->value_size);
    hash_table->elements_stored++; 

    if (hash_table->elements_stored * 2 >= hash_table->array_size) {
        double_table_size(hash_table_ptr);
    }

    return 0;
}

int hash_table_remove(HashTable** table_ptr, const void* key) {
    HashTable* table = table_ptr;

    size_t index = table->hash_function(key);
    index %= table->array_size;

    if (table->node_array[index] == NULL) {
        return 1;
    }

    if (table->comperator(key, &table->node_array[0]) == 0) {
        HashTableNode* temp = table->node_array[0]->next;
        free(table->node_array[0]);
        table->node_array[0] = temp;
        table->elements_stored--;
        
        if (table->elements_stored < table->array_size / 4) {
            cut_table_size(table_ptr);
        }

        return 0;
    }

    HashTableNode* it = table->node_array[index];

    while (it->next != NULL && table->comperator(key, &it->next->data[0]) != 0) {it = it->next;}

    if (table->comperator(key, &it->next->data[0]) == 0) {
        HashTableNode* temp = table->node_array[0]->next;
        free(table->node_array[0]);
        table->node_array[0] = temp;
        table->elements_stored--;
        
        if (table->elements_stored < table->array_size / 4) {
            cut_table_size(table_ptr);
        }

        return 0;
    }

    return 1;
}
