#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#ifdef CANARY_LIST_PROTECTION || CANARY_DATA_PROTECTION || HASH_PROTECTION
#define PROTECtION
#endif

struct List_elem
{
    int32_t elem_size;
    int32_t index;

    bool is_free;

    void *prev;
    void *cur;
    void *next;
};

struct List
{
    #ifdef CANARY_LIST_PROTECTION
    uint64_t left_list_canary;
    #endif

    #ifdef HASH_PROTECTION
    uint64_t hash;
    #endif

    #ifdef CANARY_DATA_PROTECTION
    uint64_t left_data_protection;
    #endif

    void *data;

    #ifdef CANARY_DATA_PROTECTION
    uint64_t right_data_protection;
    #endif

    int32_t data_size;
    int32_t data_capacity;
    int32_t elem_size;

    int32_t first_free;

    #ifdef PROTECTION
    var_declaration info;
    #endif
    
    #ifdef CANARY_LIST_PROTECTION
    uint64_t right_list_canary;
    #endif
};

#endif //LIST_H