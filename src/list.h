#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

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
    
    int32_t free;

    bool is_ctor;
};

const List default_list = 
{
    nullptr, // data

          0, // elem_size
          1, // data_size
          4, // data_capcity

          1, // free

       true  // is_ctor
};

enum LIST_POISON
{
    POISON_LIST_BYTE = (uint8_t) 345
};

enum LIST_ERRORS
{
    OK                      ,

    LIST_NULLPTR            ,

    ALREADY_CTORED          ,
    NOT_YET_CTORED          ,

    NEGATIVE_ELEM_SIZE      ,
    NEGATIVE_DATA_SIZE      ,
    NEGATIVE_DATA_CAPACITY  ,
    
    INVALID_CAPACITY        ,
    INVALID_FREE            ,
    INVALID_DATA            ,

    MEMORY_LIMIT_EXCEEDED
};

const char *error_messages[] = 
{
    "OK"                                        ,

    "List is invalid"                           ,

    "List is already ctored"                    ,
    "List is not yet ctored"                    ,

    "Size     of list_element less than zero"   ,
    "Size     of data         less than zero"   ,
    "Capacity of data         less than zero"   ,

    "Capacity of data is invalid"               ,
    "Free     of list is invalid"               ,
    "Data             is invalid"               ,

    "Memory limit exceeded"
};

/*__________________________________USER_MACRO_DEFINITIONS___________________________________*/

#define List_ctor(lst, elem_size)                                                               \
       if (true)                                                                                \
       {                                                                                        \
            int32_t ret_ctor = _List_ctor(lst, elem_size);                                      \
                                                                                                \
            if (ret_ctor == -1)                                                                 \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
            if (ret_ctor != OK)                                                                 \
            {                                                                                   \
                log_place();                                                                    \
                List_error(ret_ctor);                                                           \
                return -1;                                                                      \
            }                                                                                   \
       }

#define List_push(lst, index, push_val)                                                         \
        if (true)                                                                               \
        {                                                                                       \
            int32_t ret_push = _List_push(lst);                                                 \
                                                                                                \
            if (ret_push == -1)                                                                 \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_pop(lst, index)                                                                    \
        if (true)                                                                               \
        {                                                                                       \
            int32_t ret_pop = _List_pop(lst, index)                                             \
                                                                                                \
            if (ret_pop != OK)                                                                  \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_get(lst, index, pull_val)                                                          \
        if (true)                                                                               \
        {                                                                                       \
            int32_t ret_get = _List_get(lst, index, pull_val);                                  \
                                                                                                \
            if (ret_get != OK)                                                                  \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_push_front(lst, push_val)                                                          \
        if (true)                                                                               \
        {                                                                                       \
            int32_t ret_push_front = _List_push_front(lst, push_val);                           \
                                                                                                \
            if (ret_push_front == -1)                                                           \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_push_back(lst, push_val)                                                           \
        if (true)                                                                               \
        {                                                                                       \
            int32_t ret_push_back = _List_push_back(lst, push_val);                             \
                                                                                                \
            if (ret_push_back == -1)                                                            \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_pop_front(lst)                                                                     \
        if (true)                                                                               \
        {                                                                                       \
            int32_t ret_pop_front = _List_pop_front(lst);                                       \
                                                                                                \
            if (ret_pop_front != OK)                                                            \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_pop_back(lst)                                                                      \
        if (true)                                                                               \
        {                                                                                       \
            int32_t ret_pop_back = _List_pop_back(lst);                                         \
                                                                                                \
            if (ret_pop_back != OK)                                                             \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_front(lst, pull_val)                                                               \
        if (true)                                                                               \
        {                                                                                       \
            int32_t ret_front = _List_front(lst, pull_val);                                     \
                                                                                                \
            if (ret_front != OK)                                                                \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_back(lst, pull_val)                                                                \
        if (true)                                                                               \
        {                                                                                       \
            int32_t ret_back = _List_back(lst, pull_val);                                       \
                                                                                                \
            if (ret_back != OK)                                                                 \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

/*_______________________________ADDITIONAL_MACRO_DEFENITIONS________________________________*/

#define List_verify(lst)                                                                        \
        if (true)                                                                               \
        {                                                                                       \
            uint32_t ret_verify = _List_verify(lst);                                            \
                                                                                                \
            if (ret_verify != OK)                                                               \
            {                                                                                   \
                log_place();                                                                    \
                List_error(ret_verify);                                                         \
                List_dump(lst);                                                                 \
                                                                                                \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_fill_free(lst)                                                                     \
        if (true)                                                                               \
        {                                                                                       \
            int32_t ret_fill_free = _List_fill_free(lst);                                       \
                                                                                                \
            if (ret_fill_free != OK)                                                            \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_realloc(lst)                                                                       \
        if (true)                                                                               \
        {                                                                                       \
            int32_t ret_realloc = _List_realloc(lst);                                           \
                                                                                                \
            if (ret_realloc != OK)                                                              \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_add_free(lst, index)                                                               \
        if (true)                                                                               \
        {                                                                                       \
            int32_t ret_add_free = _List_add_free(lst, index);                                  \
                                                                                                \
            if (ret_add_free != OK)                                                             \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_del_free(lst)                                                                      \
        if (true)                                                                               \
        {                                                                                       \
            int32_t ret_del_free = _List_del_free(lst);                                         \
                                                                                                \
            if (ret_del_free != OK)                                                             \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

/*________________________________USER_FUNCTION_DECLARATIONS_________________________________*/

static void              List_dump              (List *const lst);

static int32_t          _List_ctor              (List *const lst, const int elem_size);

static int32_t          _List_push              (List *const lst, const int32_t index, void *const push_val);
static int32_t          _List_pop               (List *const lst, const int32_t index);
static int32_t          _List_get               (List *const lst, const int32_t index, void *const pull_val);

static int32_t          _List_push_front        (List *const lst, void *const push_val);
static int32_t          _List_push_back         (List *const lst, void *const push_val);

static int32_t          _List_pop_front         (List *const lst);
static int32_t          _List_pop_back          (List *const lst);

static int32_t          _List_front             (List *const lst, void *const pull_val);
static int32_t          _List_back              (List *const lst, void *const pull_val);

/*______________________________ADDITIONAL_FUNCTION_DECLARATIONS_____________________________*/

static uint32_t         _List_verify            (List *const lst);
static void              List_error             (const uint32_t err);

static void             *List_value_iterator    (List *const lst, const int32_t index);
static List_elem_info   *List_info_iterator     (List *const lst, const int32_t index);

static int32_t          _List_realloc           (List *const lst);
static int32_t          _List_fill_free         (List *const lst);

static int32_t          _List_add_free          (List *const lst, const int32_t index);
static int32_t          _List_del_free          (List *const lst);

/*___________________________________________________________________________________________*/

static uint32_t _List_verify(List *const lst)
{
    uint32_t err = OK;

    if (lst          == nullptr)                         return (1 << LIST_NULLPTR)             ;
    if (lst->is_ctor == false)                           return (1 << NOT_YET_CTORED)           ;

    if (lst->elem_size     < 0)                     err = err | (1 << NEGATIVE_ELEM_SIZE)       ;
    if (lst->data_size     < 0)                     err = err | (1 << NEGATIVE_DATA_SIZE)       ;
    if (lst->data_capacity < 0)                     err = err | (1 << NEGATIVE_DATA_CAPACITY)   ;

    if (lst->data_capacity < lst->data_size)        err = err | (1 << INVALID_CAPACITY)         ;
    if (lst->free          < default_list.free ||
        lst->free          > lst->data_capacity)    err = err | (1 << INVALID_FREE)             ;
    if (lst->data         == nullptr)               err = err | (1 << INVALID_DATA)             ;

    return err;
}

static void List_error(const uint32_t err)
{
    log_message("\n");
    log_error  ("LIST_VERIFY_FAILED\n");

    for (uint32_t err_bit = 0; err_bit < 8 * sizeof(int32_t); ++err_bit)
    {
        if (err & (1 << err_bit)) log_error("%s\n", error_messages[err_bit]);
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
                "       free:           %d\n"
                "                         \n"
                "       is_ctor:        %d\n"
                "       }\n",

                        lst->data,
                        lst->data_size,
                        lst->data_capacity,
                        
                        lst->elem_size,
                        
                        lst->free,
                        
                        lst->is_ctor);
    
    log_message("\n"
                "index: ");
    for (int index_cnt = 0; index_cnt < lst->data_capacity; ++index_cnt)
    {
        log_message("%-8d ", index_cnt);
    }

    log_message("\n"
                "value: ");
    for (int index_cnt = 0; index_cnt < lst->data_capacity; ++index_cnt)
    {
        List_elem_info *cur_info = List_info_iterator (lst, index_cnt);
        void           *cur_elem = List_value_iterator(lst, index_cnt);

        if      (cur_info          == nullptr) log_message(RED   "%-8s " CANCEL, "NO INFO"            );
        else if (cur_info->is_free == false)   log_message(USUAL "%-8d " CANCEL, *(int32_t *) cur_elem);
        else                                   log_message(OLIVE "%-8s " CANCEL, "FREE"               );
    }

    log_message("\n"
                " next: ");
    for (int next_cnt  = 0; next_cnt  < lst->data_capacity; ++next_cnt)
    {
        List_elem_info *cur_info = List_info_iterator(lst, next_cnt);

        if      (cur_info       == nullptr) log_message(RED   "%-8s " CANCEL, "NO INFO");
        else if (cur_info->next == nullptr) log_message(OLIVE "%-8s " CANCEL, "NULL"   );
        else                                log_message(USUAL "%-8d " CANCEL, cur_info->next->index);
    }

    log_message("\n"
                " prev: ");
    for (int prev_cnt  = 0; prev_cnt  < lst->data_capacity; ++prev_cnt)
    {
        List_elem_info *cur_info = List_info_iterator(lst, prev_cnt);

        if      (cur_info       == nullptr) log_message(RED   "%-8s " CANCEL, "NO_INFO");
        else if (cur_info->prev == nullptr) log_message(OLIVE "%-8s " CANCEL, "NULL"   );
        else                                log_message(USUAL "%-8d " CANCEL, cur_info->prev->index);
    }
    log_message("\n");
}

/*___________________________________________________________________________________________*/

static int32_t _List_realloc(List *const lst)
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
        List_verify   (lst);
    }
    return OK;
}

/*___________________________________________________________________________________________*/

static int32_t _List_fill_free(List *const lst)
{
    List_verify(lst);

    int32_t elem_cnt = lst->free = lst->data_size;
    for (;  elem_cnt < lst->data_capacity - 1; ++elem_cnt)
    {
        List_elem_info * cur_info = List_info_iterator(lst, elem_cnt    );
        List_elem_info *next_info = List_info_iterator(lst, elem_cnt + 1);

        cur_info->index   = elem_cnt ;
        cur_info->is_free = true     ;
        cur_info->next    = next_info;
        cur_info->prev    = nullptr  ;
    }

    List_elem_info * last_info = List_info_iterator(lst, elem_cnt);

    last_info->index   = elem_cnt;
    last_info->is_free = true    ;
    last_info->next    = nullptr ;
    last_info->prev    = nullptr ;

    List_verify(lst);
    return OK;
}

/*___________________________________________________________________________________________*/

static void *List_value_iterator(List *const lst, const int32_t index)
{
    assert(lst != nullptr);

    return (int8_t *) lst->data + index * (lst->elem_size + sizeof(List_elem_info));
}

static List_elem_info *List_info_iterator(List *const lst, const int32_t index)
{
    assert(lst != nullptr);

    return (List_elem_info *) ((int8_t *) lst->data + index * (lst->elem_size + sizeof(List_elem_info)) + lst->elem_size);
}

/*___________________________________________________________________________________________*/

static int32_t _List_ctor(List *const lst, const int elem_size)
{
    if (lst          == nullptr) return 1 << LIST_NULLPTR      ;
    if (lst->is_ctor == true   ) return 1 << ALREADY_CTORED    ;
    
   *lst                = default_list; 
    lst->elem_size     = elem_size   ;
    lst->data          = calloc(lst->data_capacity, sizeof(List_elem_info) + lst->elem_size);
    
    List_elem_info *fictional = List_info_iterator(lst, 0);
    
    fictional->index          = 0;
    fictional->is_free        = false;
    fictional->next           = fictional;
    fictional->prev           = fictional;

    if (lst->data == nullptr) return 1 << MEMORY_LIMIT_EXCEEDED;

    List_fill_free(lst);
    List_verify   (lst);
    return OK;
}

/*___________________________________________________________________________________________*/

static int32_t _List_push(List *const lst, const int32_t index, void *const push_val)
{
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

/*___________________________________________________________________________________________*/

static int32_t _List_pop(List *const lst, const int32_t index)
{
    List_verify(lst);

    void           *poped_elem = List_value_iterator(lst, index);
    List_elem_info *poped_info = List_info_iterator (lst, index);


    poped_info->prev->next = poped_info->next;
    poped_info->next->prev = poped_info->prev;

    --lst->data_size;

    List_add_free(lst, index);
    List_verify  (lst);
    return OK;
}

/*___________________________________________________________________________________________*/

static int32_t _List_get(List *const lst, const int32_t index, void *const pull_val)
{
    List_verify(lst);

    memcpy(pull_val, List_value_iterator(lst, index), lst->elem_size);

    List_verify(lst);

    return OK;
}

/*___________________________________________________________________________________________*/

static int32_t _List_add_free(List *const lst, const int32_t index)
{
    List_verify(lst);

    List_elem_info *coming_info = List_info_iterator(lst, index);

    if (lst->free == lst->data_capacity)
    {
        lst->free = index;

        coming_info->next    = nullptr;
        coming_info->prev    = nullptr;
    }
    else 
    {
        List_elem_info *free_info = List_info_iterator(lst, lst->free);
        
        coming_info->next = free_info;
        coming_info->prev = nullptr;

        lst->free = index; 
    }

    coming_info->is_free = true;

    List_verify(lst);
    return OK;
}

/*___________________________________________________________________________________________*/

static int32_t _List_del_free(List *const lst)
{
    List_verify(lst);

    List_elem_info *free_info = List_info_iterator(lst, lst->free);

    if (free_info->next == nullptr) lst->free = lst      ->data_capacity;
    else                            lst->free = free_info->next->index  ;

    free_info->is_free = false;

    List_verify(lst);
    return OK;
}

/*___________________________________________________________________________________________*/

static int32_t _List_push_front(List *const lst, void *const push_val)
{
    List_verify(lst);

    return _List_push(lst, 0, push_val);
}

/*___________________________________________________________________________________________*/

static int32_t _List_push_back(List *const lst, void *const push_val)
{
    List_verify(lst);

    return _List_push(lst, List_info_iterator(lst, 0)->prev->index, push_val);
}

/*___________________________________________________________________________________________*/

static int32_t _List_pop_front(List *const lst)
{
    List_verify(lst);

    return _List_pop(lst, List_info_iterator(lst, 0)->next->index);
}

/*___________________________________________________________________________________________*/

static int32_t _List_pop_back(List *const lst)
{
    List_verify(lst);

    return _List_pop(lst, List_info_iterator(lst, 0)->prev->index);
}

/*___________________________________________________________________________________________*/

static int32_t _List_front(List *const lst, void *const pull_val)
{
    List_verify(lst);

    return _List_get(lst, List_info_iterator(lst, 0)->next->index, pull_val);
}

/*___________________________________________________________________________________________*/

static int32_t _List_back(List *const lst, void *const pull_val)
{
    List_verify(lst);

    return _List_get(lst, List_info_iterator(lst, 0)->prev->index, pull_val);
}

/*___________________________________________________________________________________________*/

#endif // LIST_H
