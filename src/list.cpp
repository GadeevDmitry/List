#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "log.h"
#include "var_declaration.h"
#include "hash.h"

struct List_elem_info
{
    bool is_free;

    void *next;
    void *prev;
};

struct List
{
    void *data;

    int32_t elem_size;
    int32_t data_size;
    int32_t data_capacity;
    
    int32_t head;
    int32_t free;

    bool is_ctor;
};

const List default_list = 
{
    nullptr, // data

          0, // elem_size
          0, // data_size
          4, // data_capcity

         -1, // head
          0, // free

       true  // is_ctor
};

enum LIST_POISON
{
    POISON_LIST_BYTE = (uint8_t) 345
};

enum LIST_ERRORS
{
    OK ,

    LIST_NULLPTR ,

    ALREADY_CTORED ,
    NOT_YET_CTORED ,

    NEGATIVE_ELEM_SIZE ,
    NEGATIVE_DATA_SIZE ,
    NEGATIVE_DATA_CAPACITY ,
    
    INVALID_CAPACITY ,
    INVALID_HEAD ,
    INVALID_FREE ,
    INVALID_DATA ,

    MEMORY_LIMIT_EXCEEDED
};

const char *error_messages[] = 
{
    "OK" ,

    "List is invalid" ,

    "List is already ctored" ,
    "List is not yet ctored" ,

    "Size     of list_element less than zero" ,
    "Size     of data         less than zero" ,
    "Capacity of data         less than zero" ,

    "Capacity of data is invalid" ,
    "Head     of list is invalid" ,
    "Free     of list is invalid" ,
    "Data             is invalid" ,

    "Memory limit exceeded"
};

/*___________________________________FUNCTION_DECLARATION_____________________________________*/

static uint32_t _List_verify    (List *const lst);
static  int32_t  List_ctor      (List *const lst, const int elem_size);
static  int32_t  List_fill_free (List *const lst);
static  int32_t  List_realloc   (List *const lst);

/*____________________________________________________________________________________________*/

#define List_verify(lst)                                                                        \
   if (_List_verify(lst) != OK) return -1;

static uint32_t _List_verify(List *const lst)
{
    uint32_t err = 0;

    if (lst          == nullptr)                               return LIST_NULLPTR              ;
    if (lst->is_ctor == false)                                 return NOT_YET_CTORED            ;

    if (lst->elem_size     < 0)                     err = err | (1 << NEGATIVE_ELEM_SIZE)       ;
    if (lst->data_size     < 0)                     err = err | (1 << NEGATIVE_DATA_SIZE)       ;
    if (lst->data_capacity < 0)                     err = err | (1 << NEGATIVE_DATA_CAPACITY)   ;

    if (lst->data_capacity < lst->data_size)        err = err | (1 << INVALID_CAPACITY)         ;
    if (lst->head          < default_list.head ||
        lst->head          > lst->data_size)        err = err | (1 << INVALID_HEAD)             ;
    if (lst->free          < default_list.free ||
        lst->free          > lst->data_size)        err = err | (1 << INVALID_FREE)             ;
    if (lst->data         == nullptr)               err = err | (1 << INVALID_DATA)             ;

    return err;
}

static int32_t List_ctor(List *const lst, const int elem_size)
{
    if (lst          == nullptr) return 1 << LIST_NULLPTR;
    if (lst->is_ctor == true)    return 1 << ALREADY_CTORED;
    
   *lst                = default_list; 
    lst->elem_size     = elem_size   ;
    lst->data          = calloc(lst->data_capacity, sizeof(List_elem_info) + lst->elem_size);

    if (lst->data == nullptr) return 1 << MEMORY_LIMIT_EXCEEDED;

    List_fill_free(lst);

    List_verify(lst);
    return OK;
}

static int32_t List_fill_free(List *const lst)
{
    List_verify(lst);

    int32_t elem_cnt  = lst->free = lst->data_size;
    for (;  elem_cnt != lst->data_capacity - 1; ++elem_cnt)
    {
        void           * cur_elem =                     (int8_t *) lst->data + elem_cnt * (lst->elem_size + sizeof(List_elem_info));
        void           *next_elem =                     (int8_t *) cur_elem  +            (lst->elem_size + sizeof(List_elem_info));
        List_elem_info * cur_info = (List_elem_info *) ((int8_t *) cur_elem  +             lst->elem_size                         );
        List_elem_info *next_info = (List_elem_info *) ((int8_t *) cur_info  +             lst->elem_size + sizeof(List_elem_info));

        cur_info ->is_free = true     ;
        cur_info ->next    = next_elem;
        next_info->prev    = cur_elem ;

        memset(cur_elem, POISON_LIST_BYTE, lst->elem_size);
    }

    void           * last_elem =                     (int8_t *) lst->data + elem_cnt * (lst->elem_size + sizeof(List_elem_info));
    void           *first_elem =                     (int8_t *) lst->data                                                       ;
    List_elem_info * last_info = (List_elem_info *) ((int8_t *) last_elem +             lst->elem_size                         );
    List_elem_info *first_info = (List_elem_info *) ((int8_t *) last_info +             lst->elem_size + sizeof(List_elem_info));

    last_info ->is_free = true      ;
    last_info ->next    = first_elem;
    first_info->prev    = last_elem ;

    memset(last_elem, POISON_LIST_BYTE, lst->elem_size);

    List_verify(lst);
    return OK;
}

static int32_t List_realloc(List *const lst)
{
    List_verify(lst);

    if (lst->data_size == lst->data_capacity)
    {
        int32_t future_capacity = 2 * lst->data_capacity;
        void   *temp_lst_data   = realloc(lst->data, (lst->elem_size + sizeof(List_elem_info)) * future_capacity);

        if (temp_lst_data == nullptr) return 1 << MEMORY_LIMIT_EXCEEDED;

        lst->data_capacity = future_capacity;
        lst->data          = temp_lst_data  ;

        List_fill_free(lst);

        List_verify(lst);
    }
    return OK;
}

