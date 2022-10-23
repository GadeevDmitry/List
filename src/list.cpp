#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "log.h"
#include "var_declaration.h"
#include "hash.h"

#define CANARY_LIST_PROTECTION
#define CANARY_DATA_PROTECTION
#define HASH_PROTECTION

#ifdef  CANARY_LIST_PROTECTION
#define CANARY_LIST_PROTECTION      1
#else
#define CANARY_LIST_PROTECTION      0
#endif

#ifdef  CANARY_DATA_PROTECTION
#define CANARY_DATA_PROTECTION      1
#else
#define CANARY_DATA_PROTECTION      0
#endif

#ifdef  HASH_PROTECTION
#define HASH_PROTECTION             1
#else
#define HASH_PROTECTION             0
#endif

#if     CANARY_LIST_PROTECTION || CANARY_DATA_PROTECTION || HASH_PROTECTION
#define PROTECTION                  1
#else
#define PROTECTION                  0
#endif

struct List_elem
{
    bool is_free;

    void *prev;
    void *cur;
    void *next;
};

struct List
{
    #if CANARY_LIST_PROTECTION
    uint64_t left_list_canary;
    #endif

    #if HASH_PROTECTION
    uint64_t hash;
    #endif

    #if CANARY_DATA_PROTECTION
    uint64_t left_data_canary;
    #endif

    void *data;

    #if CANARY_DATA_PROTECTION
    uint64_t right_data_canary;
    #endif

    int32_t data_size;
    int32_t data_capacity;
    int32_t elem_size;
    int32_t first_free;
    
    bool is_ctored;

    #if PROTECTION
    var_declaration info;
    #endif
    
    #if CANARY_LIST_PROTECTION
    uint64_t right_list_canary;
    #endif
};

enum LIST_VERDICT
{
    OK                                              ,

#if CANARY_LIST_PROTECTION
    FAILED_LIST_CANARY_PROTECTION_LEFT              ,
    FAILED_LIST_CANARY_PROTECTION_RIGHT             ,
#endif

#if CANARY_DATA_PROTECTION
    FAILED_DATA_CANARY_PROTECTION_LEFT              ,
    FAILED_DATA_CANARY_PROTECTION_RIGHT             ,
#endif

#if HASH_PROTECTION
    FAILED_HASH_PROTECTION                          ,
#endif

    NEGATIVE_SIZE                                   ,
    NEGATIVE_CAPACITY                               ,
    NEGATIVE_ELEM_SIZE                              ,
    NEGATIVE_FIRST_FREE                             ,

    INVALID_CAPACITY                                ,
    INVALID_FIRST_FREE                              ,
    INVALID_DATA                                    ,
    INVALID_CTOR                                    ,

    LIST_NULLPTR
};

static const char *verdict_message[] = 
{
    "OK"                                            ,

#if CANARY_LIST_PROTECTION
    "FAILED_LIST_CANARY_PROTECTION_LEFT"            ,
    "FAILED_LIST_CANARY_PROTECTION_RIGHT"           ,
#endif

#if CANARY_DATA_PROTECTION
    "FAILED_DATA_CANARY_PROTECTION_LEFT"            ,
    "FAILED_DATA_CANARY_PROTECTION_RIGHT"           ,
#endif

#if HASH_PROTECTION
    "FAILED_HASH_PROTECTION"                        ,
#endif

    "SIZE       LESS THAN ZERO"                     ,
    "CAPACITY   LESS THAN ZERO"                     ,
    "ELEM_SIZE  LESS THAN ZERO"                     ,
    "FIRST_FREE LESS THAN ZERO"                     ,


    "CAPACITY   LESS THAN  SIZE"                    ,
    "FIRST_FREE MORE THAN (SIZE - 1)"               ,
    "INVALID DATA"                                  ,
    "LIST HAS BEEN ALREADY CTORED"                  ,

    "POINTER TO THE LIST IS NULLPTR"
};

enum CANARY_VALUE
{
    CANARY_LIST_VALUE = 0xCAFEBABE                  ,
    CANARY_DATA_VALUE = 0xCAFED00D
};

/*___________________________FUNCTION_DECLARATION___________________________*/

uint32_t List_verify(List *const lst);

/*__________________________________________________________________________*/

uint32_t List_verify(List *const lst)
{
    uint32_t err = 0;

    if (lst == nullptr)
    {
        err = err | LIST_NULLPTR;
        return err;
    }

    #if CANARY_LIST_PROTECTION
    if (lst->left_list_canary  != CANARY_LIST_VALUE) err = err | FAILED_LIST_CANARY_PROTECTION_LEFT  ;
    if (lst->right_list_canary != CANARY_LIST_VALUE) err = err | FAILED_LIST_CANARY_PROTECTION_RIGHT ;
    #endif

    #if CANARY_DATA_PROTECTION
    if (lst->left_data_canary  != CANARY_DATA_VALUE) err = err | FAILED_DATA_CANARY_PROTECTION_LEFT  ;
    if (lst->right_data_canary != CANARY_DATA_VALUE) err = err | FAILED_DATA_CANARY_PROTECTION_RIGHT ;
    #endif

    #if HASH_PROTECTION
    if (!hash_verify(lst, &lst->hash, sizeof(List))) err = err | FAILED_HASH_PROTECTION              ;
    #endif

    if (lst->data_size     < 0)                      err = err | NEGATIVE_SIZE                       ;
    if (lst->data_capacity < 0)                      err = err | NEGATIVE_CAPACITY                   ;
    if (lst->elem_size     < 0)                      err = err | NEGATIVE_ELEM_SIZE                  ;
    if (lst->first_free    < 0)                      err = err | NEGATIVE_FIRST_FREE                 ;

    if (lst->data_capacity < lst->data_size)         err = err | INVALID_CAPACITY                    ;
    if (lst->first_free   >= lst->data_size)         err = err | INVALID_FIRST_FREE                  ;

    if (lst->data == nullptr && lst->data_size)      err = err | INVALID_DATA                        ;

    return err; 
}

#if PROTECTION

static uint32_t List_ctor(List *const lst, const uint32_t elem_size, const char    *name_file,
                                                                     const char    *name_func,
                                                                     const char    *name_var ,
                                                                     const uint32_t line)
{
    if (lst == nullptr) return LIST_NULLPTR;
    if (lst->is_ctored) return INVALID_CTOR;

    *lst = {};

    #if CANARY_LIST_PROTECTION
    lst->left_list_canary = lst->right_list_canary = CANARY_LIST_VALUE;
    #endif

    #if CANARY_DATA_PROTECTION
    lst->left_data_canary = lst->right_data_canary = CANARY_DATA_VALUE;
    #endif

    lst->elem_size = elem_size;
    var_ctor(&lst->info, name_file, name_func, name_var, line);

    #if HASH_PROTECTION
    lst->hash = hash_culc(lst, &lst->hash, sizeof(List));
    #endif

    return OK;
}
#else //!PROTECTION

static uint32_t List_ctor(List *const lst, uint32_t elem_size)
{
    if (lst == nullptr) return LIST_NULLPTR;
    if (lst->is_ctored) return INVALID_CTOR;

    *lst = {};

    lst->elem_size = elem_size;
    return OK;
}
#endif