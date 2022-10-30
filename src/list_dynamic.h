#ifndef LIST_DYNAMIC_H
#define LIST_DYNAMIC_H

#include "var_declaration.h"
#include "log.h"

struct List_node
{
    void      *data;
    List_node *next;
    List_node *prev;
};

struct List
{
    var_declaration  var_info ;
    List_node       *fictional;

    int elem_size;
    int data_size;

    bool is_ctor;
};

enum LIST_ERRORS
{
    OK                      ,

    NULLPTR_LIST            ,
    NULLPTR_LIST_NODE       ,
    NULLPTR_FICTIONAL       ,

    ALREADY_CTORED          ,
    NOT_YET_CTORED          ,

    INVALID_ELEM_SIZE       ,
    INVALID_DATA_SIZE       ,
    INVALID_NODE_CYCLE      ,

    MEMORY_LIMIT_EXCEEDED
};

/*_____________________________________MACRO_DEFINITIONS_____________________________________*/

#define List_ctor(lst, elem_size)                                                               \
       if (true)                                                                                \
       {                                                                                        \
            int ret_ctor = _List_ctor(lst, elem_size, __FILE__,                                 \
                                                      __PRETTY_FUNCTION__,                      \
                                                      #lst,                                     \
                                                      __LINE__);                                \
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

#define List_dtor(lst)                                                                          \
        if (true)                                                                               \
        {                                                                                       \
            int ret_dtor = _List_dtor(lst);                                                     \
                                                                                                \
            if (ret_dtor == -1)                                                                 \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_insert(lst, index, push_val)                                                       \
        if (true)                                                                               \
        {                                                                                       \
            int ret_insert = _List_insert(lst, index, push_val);                                \
                                                                                                \
            if (ret_insert == -1)                                                               \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
            if (ret_insert != OK)                                                               \
            {                                                                                   \
                log_place();                                                                    \
                List_error(ret_insert);                                                         \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_erase(lst, index)                                                                  \
        if (true)                                                                               \
        {                                                                                       \
            int ret_erase = _List_erase(lst, index);                                            \
                                                                                                \
            if (ret_erase != OK)                                                                \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_get(lst, index, pull_val)                                                          \
        if (true)                                                                               \
        {                                                                                       \
            int ret_get = _List_get(lst, index, pull_val);                                      \
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
            int ret_push_front = _List_push_front(lst, push_val);                               \
                                                                                                \
            if (ret_push_front == -1)                                                           \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
            if (ret_push_front != OK)                                                           \
            {                                                                                   \
                log_place();                                                                    \
                List_error(ret_push_front);                                                     \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_push_back(lst, push_val)                                                           \
        if (true)                                                                               \
        {                                                                                       \
            int ret_push_back = _List_push_back(lst, push_val);                                 \
                                                                                                \
            if (ret_push_back == -1)                                                            \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
            if (ret_push_back != OK)                                                            \
            {                                                                                   \
                log_place();                                                                    \
                List_error(ret_push_back);                                                      \
                return -1;                                                                      \
            }                                                                                   \
        }
  
#define List_pop_front(lst)                                                                     \
        if (true)                                                                               \
        {                                                                                       \
            int ret_pop_front = _List_pop_front(lst);                                           \
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
            int ret_pop_back = _List_pop_back(lst);                                             \
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
            int ret_front = _List_front(lst, pull_val);                                         \
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
            int ret_back = _List_back(lst, pull_val);                                           \
                                                                                                \
            if (ret_back != OK)                                                                 \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

/*________________________________USER_FUNCTION_DECLARATIONS_________________________________*/

void List_error         (const unsigned int err);

void List_dump          (List *const lst);
void List_graph_dump    (List *const lst);

int _List_ctor          (List *const lst, const int elem_size,  const char *name_file,
                                                                const char *name_func,
                                                                const char *name_var ,
                                                                const int   line     );
int _List_dtor          (List *const lst);

int _List_insert        (List *const lst, int index, void *const push_val);
int _List_erase         (List *const lst, int index);
int _List_get           (List *const lst, int index, void *const pull_val);

int _List_push_front    (List *const lst, void *const push_val);
int _List_push_back     (List *const lst, void *const push_val);

int _List_pop_front     (List *const lst);
int _List_pop_back      (List *const lst);

int _List_front         (List *const lst, void *const pull_val);
int _List_back          (List *const lst, void *const pull_val);

/*___________________________________________________________________________________________*/

#endif //LIST_DYNAMIC_H