#ifndef LIST_H
#define LIST_H

#include "var_declaration.h"
#include "log.h"

struct List
{
    void *data;

    int32_t elem_size;
    int32_t data_size;
    int32_t data_capacity;
    
    int32_t free;

    bool is_ctor;
    bool is_linear;

    var_declaration var_info;
};

enum LIST_ERRORS
{
    OK                      ,

    NULLPTR_LIST            ,
    NULLPTR_LIST_INFO       ,

    ALREADY_CTORED          ,
    NOT_YET_CTORED          ,

    NEGATIVE_ELEM_SIZE      ,
    NEGATIVE_DATA_SIZE      ,
    NEGATIVE_DATA_CAPACITY  ,
    
    INVALID_CAPACITY        ,
    INVALID_FREE            ,
    INVALID_DATA            ,
    INVALID_INDEX           ,

    MEMORY_LIMIT_EXCEEDED   ,
};

/*__________________________________USER_MACRO_DEFINITIONS___________________________________*/

#define List_ctor(lst, elem_size)                                                               \
       if (true)                                                                                \
       {                                                                                        \
            int ret_ctor = _List_ctor(lst, elem_size, __FILE__           ,                      \
                                                      __PRETTY_FUNCTION__,                      \
                                                      #lst               ,                      \
                                                      __LINE__           );                     \
                                                                                                \
            if      (ret_ctor == -1) log_place();                                               \
            else if (ret_ctor != OK)                                                            \
            {                                                                                   \
                log_place ();                                                                   \
                List_error(ret_ctor);                                                           \
            }                                                                                   \
       }

#define List_dtor(lst)                                                                          \
        if (true)                                                                               \
        {                                                                                       \
            int ret_dtor = _List_dtor(lst);                                                     \
                                                                                                \
            if (ret_dtor == -1) log_place();                                                    \
        }

#define List_push(lst, index, push_val)                                                         \
       _List_push(lst, index, push_val, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#define List_push_order(lst, order, push_val)                                                   \
       _List_push_order(lst, order, push_val, __FILE__, __PRETTY_FUNCTION__, __LINE__)          \

#define List_pop(lst, index)                                                                    \
        if (true)                                                                               \
        {                                                                                       \
            int ret_pop = _List_pop(lst, index);                                                \
                                                                                                \
            if (ret_pop != OK)  log_place();                                                    \
        }

#define List_pop_order(lst, order)                                                              \
        if (true)                                                                               \
        {                                                                                       \
            int ret_pop = _List_pop(lst, _List_order_to_index(lst, order + 1));                 \
                                                                                                \
            if (ret_pop != OK) log_place();                                                     \
        }

#define List_get(lst, index, pull_val)                                                          \
        if (true)                                                                               \
        {                                                                                       \
            int ret_get = _List_get(lst, index, pull_val);                                      \
                                                                                                \
            if (ret_get != OK) log_place();                                                     \
        }

#define List_get_order(lst, order, pull_val)                                                    \
        if (true)                                                                               \
        {                                                                                       \
            int ret_get = _List_get(lst, _List_order_to_index(lst, order + 1), pull_val);       \
                                                                                                \
            if (ret_get != OK) log_place();                                                     \
        }

#define List_push_front(lst, push_val)                                                          \
       _List_push_front(lst, push_val, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#define List_push_back(lst, push_val)                                                           \
       _List_push_back(lst, push_val, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#define List_pop_front(lst)                                                                     \
        if (true)                                                                               \
        {                                                                                       \
            int ret_pop_front = _List_pop_front(lst);                                           \
                                                                                                \
            if (ret_pop_front != OK) log_place();                                               \
        }

#define List_pop_back(lst)                                                                      \
        if (true)                                                                               \
        {                                                                                       \
            int ret_pop_back = _List_pop_back(lst);                                             \
                                                                                                \
            if (ret_pop_back != OK) log_place();                                                \
        }

#define List_front(lst, pull_val)                                                               \
        if (true)                                                                               \
        {                                                                                       \
            int ret_front = _List_front(lst, pull_val);                                         \
                                                                                                \
            if (ret_front != OK) log_place();                                                   \
        }

#define List_back(lst, pull_val)                                                                \
        if (true)                                                                               \
        {                                                                                       \
            int ret_back = _List_back(lst, pull_val);                                           \
                                                                                                \
            if (ret_back != OK) log_place();                                                    \
        }

#define List_line(lst)                                                                          \
        if (true)                                                                               \
        {                                                                                       \
            int ret_line = _List_line(lst);                                                     \
                                                                                                \
            if (ret_line != OK) log_place();                                                    \
        }

/*________________________________USER_FUNCTION_DECLARATIONS_________________________________*/

void              List_dump             (List *const lst);
void              List_simple_dump      (List *const lst);
void              List_graph_order_dump (List *const lst);

int              _List_line             (List *const lst);

void              List_error            (const unsigned int err);
int              _List_order_to_index   (List *const lst, int order);

int              _List_ctor             (List *const lst, const int elem_size,  const char    *name_file,
                                                                                const char    *name_func,
                                                                                const char    *name_var ,
                                                                                const int      line);
int              _List_dtor             (List *const lst);

int              _List_push             (List *const lst, const int index, void *const         push_val ,
                                                                                 const char   *call_file,
                                                                                 const char   *call_func,
                                                                                 const int     call_line);

int              _List_pop              (List *const lst, const int index);
int              _List_get              (List *const lst, const int index, void *const pull_val);

int              _List_push_front       (List *const lst, void *const push_val, const char   *call_file,
                                                                                const char   *call_func,
                                                                                const int     call_line);

int              _List_push_back        (List *const lst, void *const push_val, const char   *call_file,
                                                                                const char   *call_func,
                                                                                const int     call_line);

int              _List_pop_front        (List *const lst);
int              _List_pop_back         (List *const lst);

int              _List_front            (List *const lst, void *const pull_val);
int              _List_back             (List *const lst, void *const pull_val);

int              _List_push_order       (List *const lst, const int order, void *const       push_val,
                                                                                 const char *call_file,
                                                                                 const char *call_func,
                                                                                 const int   call_line);

/*___________________________________________________________________________________________*/

#endif //LIST_H