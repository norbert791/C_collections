#include <inttypes.h>
#include <hash_table.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tgmath.h>
#include <assert.h>

static inline void create_destroy_test(size_t key_size, size_t value_size, size_t initial_size);

static inline void insert_remove_get_test(size_t key_size, size_t value_size, size_t initial_size);

static inline void copy_test(size_t key_size, size_t value_size, size_t initial_size);

static inline void put_test(size_t key_size, size_t value_size, size_t initial_size);

static inline void resize_test(size_t key_size, size_t value_size, size_t initial_size);

static inline void test_cases(size_t key_siez, size_t value_size, size_t initial_size);

static inline size_t trivial_hash(const void* obj);

static inline int trivial_is_different(const void* obj1, const void* obj2);

static inline void create_destroy_test(size_t key_size, size_t value_size, size_t initial_size) {

    if (key_size == 0) {
        HashTable* table = hash_table_create(0, value_size, initial_size, trivial_hash, trivial_is_different);
        assert(table == NULL);
        hash_table_destroy(table);
        return;
    }

    if (value_size == 0) {
        HashTable* table = hash_table_create(0, value_size, initial_size, trivial_hash, trivial_is_different);
        assert(table == NULL);
        hash_table_destroy(table);
        return;
    }
    
    {
        HashTable* table = hash_table_create(key_size, value_size, initial_size, trivial_hash, trivial_is_different);
        assert(table != NULL);
        hash_table_destroy(table);
    }

    {
        HashTable* table = hash_table_create(key_size, value_size, 0, trivial_hash, trivial_is_different);
        assert(table != NULL);
        hash_table_destroy(table);
    }

    {
        HashTable* table = hash_table_create(key_size, value_size, initial_size, NULL, trivial_is_different);
        assert(table == NULL);
        hash_table_destroy(table);
    }

    {
        HashTable* table = hash_table_create(key_size, value_size, initial_size, trivial_hash, NULL);
        assert(table == NULL);
        hash_table_destroy(table);
    }
}

static inline void insert_remove_get_test(size_t key_size, size_t value_size, size_t initial_size) {
    
    {
        HashTable* table = hash_table_create(key_size, value_size, initial_size, trivial_hash, trivial_is_different);
        
        assert(table != NULL);

        uint8_t key[key_size];
        uint8_t value[value_size];
        memset(key, 0, key_size);
        memset(value, 1, value_size);
        assert(hash_table_insert(&table, key, value) == 0);
        uint8_t target[value_size];
        assert(hash_table_get(table, key, target) == 0);
        assert(memcmp(value, target, value_size) == 0);
        hash_table_destroy(table);
        
    }

    {
        HashTable* table = hash_table_create(key_size, value_size, initial_size, trivial_hash, trivial_is_different);

        assert(table != NULL);

        for (size_t i = 0; i < initial_size; i++) {
            uint8_t counter = 0;
            uint8_t key[key_size];
            uint8_t value[value_size];
            memset(key, counter, key_size);
            memset(value, ~counter++, value_size);
            hash_table_insert(&table, key, value);
        }
        
        for (size_t i = 0; i < initial_size; i++) {
            uint8_t counter = 0;
            uint8_t key[key_size];
            uint8_t value[value_size];
            uint8_t target[value_size];
            memset(key, counter, key_size);
            memset(key, ~counter++, value_size);
            assert(hash_table_get(table, key, target) == 0);
            assert(memcmp(value, target, value_size) == 0);
        }

        hash_table_destroy(table);
    }
}

static inline void copy_test(size_t key_size, size_t value_size, size_t initial_size) {

    {
        HashTable* table = hash_table_create(key_size, value_size, initial_size, trivial_hash, trivial_is_different);

        assert(table != NULL);

        for (size_t i = 0; i < initial_size; i++) {
            uint8_t counter = 0;
            uint8_t key[key_size];
            uint8_t value[value_size];
            memset(key, counter, key_size);
            memset(value, ~counter++, value_size);
            hash_table_insert(&table, key, value);
        }

        HashTable* table_copy = hash_table_copy(table);

        assert(table_copy != NULL);

        for (size_t i = 0; i < initial_size; i++) {
            uint8_t counter = 0;
            uint8_t key[key_size];
            uint8_t value[value_size];
            uint8_t target[value_size];
            memset(key, counter, key_size);
            memset(key, ~counter++, value_size);
            assert(hash_table_get(table_copy, key, target) == 0);
            assert(memcmp(value, target, value_size) == 0);
        }

        hash_table_destroy(table);
        hash_table_destroy(table_copy);
    }
}

static inline void put_test(size_t key_size, size_t value_size, size_t initial_size) {
    {
        HashTable* table = hash_table_create(key_size, value_size, initial_size, trivial_hash, trivial_is_different);

        assert(table != NULL);

        for (size_t i = 0; i < initial_size; i++) {
            uint8_t counter = 0;
            uint8_t key[key_size];
            uint8_t value[value_size];
            memset(key, counter, key_size);
            memset(value, ~counter++, value_size);
            hash_table_insert(&table, key, value);
        }

        for (size_t i = 0; i < initial_size; i++) {
            uint8_t counter = 0;
            uint8_t key[key_size];
            uint8_t value[value_size];
            memset(key, counter, key_size);
            memset(value, counter++, value_size);
            hash_table_put(&table, key, value);
        }
        
        for (size_t i = 0; i < initial_size; i++) {
            uint8_t counter = 0;
            uint8_t key[key_size];
            uint8_t value[value_size];
            uint8_t target[value_size];
            memset(key, counter, key_size);
            memset(key, counter++, value_size);
            assert(hash_table_get(table, key, target) == 0);

            assert(memcmp(value, target, value_size) == 0);
        }

        hash_table_destroy(table);
    }
}

static inline void resize_test(size_t key_size, size_t value_size, size_t initial_size) {
    if (key_size == 0 || value_size == 0) {return;}
    {
        HashTable* table = hash_table_create(key_size, value_size, initial_size, trivial_hash, trivial_is_different);
        hash_table_resize(&table, initial_size * 2);
        hash_table_destroy(table);
    }

    {
        HashTable* table = hash_table_create(key_size, value_size, initial_size, trivial_hash, trivial_is_different);
        hash_table_resize(&table, initial_size / 2);
        hash_table_destroy(table);
    }

    {
        HashTable* table = hash_table_create(key_size, value_size, initial_size, trivial_hash, trivial_is_different);
        assert(table != NULL);
        uint8_t* key_array = malloc(sizeof(*key_array) * key_size);

        for (size_t i = 0; i < initial_size; i++) {
            int temp = rand();
            uint8_t value[value_size];
            memcpy(value, &temp, fmin(sizeof(temp), value_size));

            for (size_t j = 0; j < value_size; j++) {
                memset(&value[j], ~value[j], sizeof(value[0]));
            }

            memcpy(&key_array[i * key_size], value, fmin(key_size, value_size));
            hash_table_insert(&table, &key_array[i * key_size], value);
        }

        hash_table_resize(&table, initial_size / 2);

        for (size_t i = 0; i < initial_size; i++) {
            uint8_t value[value_size];
            uint8_t exp_value[value_size];
            memcpy(exp_value, &key_array[i * key_size], fmin(key_size, value_size));

            for (size_t j = 0; j < key_size; j++) {
                memset(&exp_value[j], ~exp_value[j], sizeof(value[0]));
            }

            assert(hash_table_get(table, &key_array[i * key_size], value) == 0);
            assert(memcmp(value, exp_value, sizeof(value[0])) == 0);
        }

        hash_table_resize(&table, initial_size * 2);

        for (size_t i = 0; i < initial_size; i++) {
            uint8_t value[value_size];
            uint8_t exp_value[value_size];
            memcpy(exp_value, &key_array[i * key_size], fmin(key_size, value_size));

            for (size_t j = 0; j < key_size; j++) {
                memset(&exp_value[j], ~exp_value[j], sizeof(value[0]));
            }

            assert(hash_table_get(table, &key_array[i * key_size], value) == 0);
            assert(memcmp(value, exp_value, sizeof(value[0])) == 0);
        }
    }
}

static inline void test_cases(size_t key_size, size_t value_size, size_t initial_size) {
    create_destroy_test(key_size, value_size, initial_size);
    if (key_size == 0 || value_size == 0) {return;}
    resize_test(key_size, value_size, initial_size);
    insert_remove_get_test(key_size, value_size, initial_size);
    copy_test(key_size, value_size, initial_size);
    put_test(key_size, value_size, initial_size);
}

int main(void) {
    srand(time(NULL));

    test_cases(0, 1, 10);
    test_cases(1, 0, 10);
    for(size_t i = 1; i < sizeof(long double); i++) {
        for(size_t j = 1; j < sizeof(long double); j++) {
            test_cases(i, j, 0);
        }
    }
    
    return 0;
}

static inline size_t trivial_hash(const void* obj) {
    const int8_t* temp = obj;

    return (size_t) *temp;
}

static inline int trivial_is_different(const void* obj1, const void* obj2) {
    uint8_t* obj1_ = (uint8_t*) obj1;
    uint8_t* obj2_ = (uint8_t*) obj2;

    return !(*obj1_ == *obj2_);
}