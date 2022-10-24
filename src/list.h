#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "log.h"
#include "var_declaration.h"
#include "hash.h"

struct List_elem_info
{
    int32_t index;
    
    bool is_free;

    List_elem_info *next;
    List_elem_info *prev;
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

static uint32_t _List_verify        (List *const lst);
static void      List_dump          (List *const lst);
static void      List_error         (List *const lst, const uint32_t err);

static  int32_t  List_ctor          (List *const lst, const int elem_size);

static  int32_t  List_fill_free     (List *const lst);
static  int32_t  List_realloc       (List *const lst);

static  int32_t  List_push          (List *const lst, const int32_t index, void *const push_val);
static  int32_t  List_pop           (List *const lst, const int32_t index);
static  int32_t  List_get           (List *const lst, const int32_t index, void *const pull_val);

static  int32_t  List_add_free      (List *const lst, const int32_t index);
static  int32_t  List_del_free      (List *const lst);

static  int32_t  List_push_front    (List *const lst, void *const push_val);
static  int32_t  List_push_back     (List *const lst, void *push_val);

static  int32_t  List_pop_front     (List *const lst);
static  int32_t  List_pop_back      (List *const lst);

static int32_t   List_front         (List *const lst, void *const pull_val);
static int32_t   List_back          (List *const lst, void *const pull_val);

void            *List_value_iterator(List *const lst, const int32_t index);
List_elem_info  *List_info_iterator (List *const lst, const int32_t index);

/*____________________________________________________________________________________________*/

#define List_verify(lst)                                                                        \
    uint32_t verify_ret = _List_verify(lst);                                                    \
                                                                                                \
   if (verify_ret != OK)                                                                        \
   {                                                                                            \
        List_error(lst, verify_ret);                                                            \
        return -1;                                                                              \
   }

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

static void List_error(List *const lst, const uint32_t err)
{
    log_error("\nLIST_VERIFY_FAILED\n")

    for (uint8_t err_bit = 0; err_bit < 32; ++err_bit)
    {
        if (err & (1 << err_bit))
        {
            log_error(error_messages[err_bit] "\n");
        }
    }
}

static void List_dump(List *const lst)
{
    log_message("\n"
                "List[%p]\n", lst);

    if (lst == nullptr) return;

    log_message("List = {\n"
                "       data:           %p\n"
                "       data_size:      %d\n"
                "       data_capacity:  %d\n"
                "                         \n"
                "       elem_size:      %d\n"
                "                         \n"
                "       head:           %d\n"
                "       free:           %d\n"
                "                         \n"
                "       is_ctor:        %d\n"
                "       }\n",

                        lst->data,
                        lst->data_size,
                        lst->data_capacity,
                        
                        lst->elem_size,
                        
                        lst->head,
                        lst->free,
                        
                        lst->is_ctor);
    
    log_message("\n"
                "index: ");
    for (int index_cnt = 0; index_cnt < lst->data_capacity; ++index_cnt)
    {
        log_message("%-15d ", index_cnt);
    }

    log_message("\n"
                " next: ");
    for (int next_cnt  = 0; next_cnt  < lst->data_capacity; ++next_cnt)
    {
        log_message("%-15d ", List_info_iterator(lst, next_cnt)->next->index);
    }

    log_message("\n"
                " prev: ");
    for (int prev_cnt  = 0; prev_cnt  < lst->data_capacity; ++prev_cnt)
    {
        log_message("%-15d ", List_info_iterator(lst, prev_cnt)->prev->index);
    }
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
        List_elem_info * cur_info = List_info_iterator(lst, elem_cnt    );
        List_elem_info *next_info = List_info_iterator(lst, elem_cnt + 1);

        cur_info ->index   = elem_cnt ;
        cur_info ->is_free = true     ;
        cur_info ->next    = next_info;
        next_info->prev    = cur_info;
    }

    List_elem_info * last_info = List_info_iterator(lst, elem_cnt    );
    List_elem_info *first_info = List_info_iterator(lst, lst->free   );

    last_info->index    = elem_cnt  ;
    last_info ->is_free = true      ;
    last_info ->next    = first_info;
    first_info->prev    = last_info ;

    List_verify(lst);
    return OK;
}

/*____________________________________________________________________________________________*/

void *List_value_iterator(List *const lst, const int32_t index)
{
    assert(lst != nullptr);

    return (int8_t *) lst->data + index * (lst->elem_size + sizeof(List_elem_info));
}

List_elem_info *List_info_iterator(List *const lst, const int32_t index)
{
    assert(lst != nullptr);

    return (List_elem_info *) ((int8_t *) lst->data + index * (lst->elem_size + sizeof(List_elem_info)));
}

/*____________________________________________________________________________________________*/

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

static int32_t List_push(List *const lst, const int32_t index, void *const push_val)
{
    //>>>>>>>>>>>>>
    printf("\nList_push() :\n");
    //<<<<<<<<<<<<<

    List_verify (lst);
    List_realloc(lst);
    
    void           *pocket_elem = List_value_iterator(lst, index);
    void           *pushed_elem = List_value_iterator(lst, lst->free);
    List_elem_info *pocket_info = List_info_iterator (lst, index);
    List_elem_info *pushed_info = List_info_iterator (lst, lst->free);

    List_del_free(lst);

    memcpy(pushed_elem, push_val, lst->elem_size);
    
    pushed_info->next = pocket_info->next;
    pocket_info->next = pushed_info;
    pushed_info->prev = pocket_info;

    pushed_info->next->prev = pushed_info;

    ++lst->data_size;

    List_verify(lst);

    return pushed_info->index;
}

static int32_t List_pop(List *const lst, const int32_t index)
{
    List_verify(lst);

    void           *poped_elem = List_value_iterator(lst, index);
    List_elem_info *poped_info = List_info_iterator (lst, index);

    List_add_free(lst, index);

    poped_info->prev->next = poped_info->next;
    poped_info->next->prev = poped_info->prev;

    --lst->data_size;

    List_verify(lst);
    return OK;
}

static int32_t List_get(List *const lst, const int32_t index, void *const pull_val)
{
    List_verify(lst);

    memcpy(pull_val, List_value_iterator(lst, index), lst->elem_size);

    List_verify(lst);

    return OK;
}

static int32_t List_add_free(List *const lst, const int32_t index)
{
    List_verify(lst);

    List_elem_info *coming_info = List_info_iterator(lst, index);
    List_elem_info *free_info   = List_info_iterator(lst, lst->free);

    if (coming_info->index == lst->head) lst->head = coming_info->next->index;

    coming_info->next = free_info;
    coming_info->prev = free_info->prev;
    free_info  ->prev = coming_info;

    free_info->prev->next = coming_info;
    free_info->is_free    = true;

    return OK;
}

static int32_t List_del_free(List *const lst)
{
    //>>>>>>>>>>>>>>>
    log_message("\nList_del_free():\n");
    //<<<<<<<<<<<<<<<

    List_verify(lst);

    List_elem_info *free_info = List_info_iterator(lst, lst->free);

    if (free_info->next == free_info) lst->free = lst->data_size;
    else
    {
        free_info->prev->next = free_info->next;
        free_info->next->prev = free_info->prev;
        
        lst->free = free_info->prev->index;
    }

    free_info->is_free = false;

    return OK;
}

static int32_t List_push_front(List *const lst, void *const push_val)
{
    //<<<<<<<<<<<<<<<<
    log_message("\nList_push_front():\n");
    //>>>>>>>>>>>>>>>>

    List_verify(lst);

    if (lst->head == default_list.head)
    {
        List_realloc(lst);

        void           *pocket_elem = List_value_iterator(lst, lst->free);
        List_elem_info *pocket_info = List_info_iterator (lst, lst->free);

        List_del_free(lst);

        lst        ->head = pocket_info->index;
        pocket_info->next = pocket_info->prev   = pocket_info;

        memcpy(pocket_elem, push_val, lst->elem_size);

        ++lst->data_size;

        List_verify(lst);

        return lst->head;
    }

    int32_t index     = List_info_iterator(lst, lst->head)->prev->index;
    return  lst->head = List_push         (lst, index, push_val);
}

static int32_t List_push_back(List *const lst, void *push_val)
{
    //>>>>>>>>>>>>>
    log_message("\nList_push_back():\n");
    //<<<<<<<<<<<<<

    List_verify(lst);

    if (lst->head == default_list.head) return List_push_front(lst, push_val);

    int32_t index = List_info_iterator(lst, lst->head)->prev->index;
    return          List_push         (lst, index, push_val);
}

static int32_t List_pop_front(List *const lst)
{
    List_verify(lst);

    int32_t index = List_info_iterator(lst, lst->head)->index;
    return          List_pop          (lst, index);
}

static int32_t List_pop_back(List *const lst)
{
    List_verify(lst);

    int32_t index = List_info_iterator(lst, lst->head)->prev->index;
    return          List_pop          (lst, index);
}

static int32_t List_front(List *const lst, void *const pull_val)
{
    List_verify(lst);

    int32_t index = List_info_iterator(lst, lst->head)->index;
    return          List_get          (lst, index, pull_val);
}

static int32_t List_back(List *const lst, void *const pull_val)
{
    List_verify(lst);

    int32_t index = List_info_iterator(lst, lst->head)->prev->index;
    return          List_get          (lst, index, pull_val);
}

#endif // LIST_H