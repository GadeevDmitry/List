#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>

#include "var_declaration.h"
#include "log.h"
#include "algorithm.h"

struct List_elem_info
{
    int index;
    
    bool is_free;

    List_elem_info *next;
    List_elem_info *prev;
};

struct List
{
    void *data;

    int elem_size;
    int data_size;
    int data_capacity;
    
    int free;

    bool is_ctor;
    bool is_linear;

    var_declaration var_info;
};

const List default_list = 
{
    nullptr, // data

          0, // elem_size
          1, // data_size
          4, // data_capcity

          1, // free

       true, // is_ctor
       true, //is_linear

    {
    nullptr,
    nullptr,
    nullptr,
          0
    }        // var_info
};

const List poison_list = 
{
    nullptr, // data

         -1, // elem_size
         -1, // data_size
         -1, // data_capacity

          0, // free

      false, // is_ctor
      false, // is_linear

    {
    nullptr,
    nullptr,
    nullptr,
          0
    }        // var_info
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

const char *error_messages[] = 
{
    "OK"                                        ,

    "List              is invalid"              ,
    "List_info pointer is invalid"              ,

    "List is already ctored"                    ,
    "List is not yet ctored"                    ,

    "Size     of list_element less than zero"   ,
    "Size     of data         less than zero"   ,
    "Capacity of data         less than zero"   ,

    "Capacity of data      is invalid"          ,
    "Free     of list      is invalid"          ,
    "Data                  is invalid"          ,
    "Index    of list_info is invalid"          ,

    "Memory limit exceeded"
};

static const int graph_file_size = 40           ;
static const int graph_cmd_size  = 100          ;

enum GRAPH_COLOR
{
    LIGHT_BLUE      ,
    LIGHT_RED       ,
    LIGHT_GREEN     ,

    DARK_BLUE       ,
    DARK_RED        ,
    DARK_GREEN      ,

    WHITE           ,
    BLACK
};

const char *color_names[] =
{
    "lightblue"     ,
    "lightred"      ,
    "lightgreen"    ,

    "darkblue"      ,
    "darkred"       ,
    "darkgreen"     ,

    "white"         ,
    "black"
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

#define List_push(lst, index, push_val)                                                         \
       _List_push(lst, index, push_val, __FILE__, __PRETTY_FUNCTION__, __LINE__)

#define List_push_order(lst, order, push_val)                                                   \
       _List_push_order(lst, order, push_val, __FILE__, __PRETTY_FUNCTION__, __LINE__)          \

#define List_pop(lst, index)                                                                    \
        if (true)                                                                               \
        {                                                                                       \
            int ret_pop = _List_pop(lst, index);                                                \
                                                                                                \
            if (ret_pop != OK)                                                                  \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_pop_order(lst, order)                                                              \
        if (true)                                                                               \
        {                                                                                       \
            int ret_pop = _List_pop(lst, _List_order_to_index(lst, order + 1));                 \
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
            int ret_get = _List_get(lst, index, pull_val);                                      \
                                                                                                \
            if (ret_get != OK)                                                                  \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_get_order(lst, order, pull_val)                                                    \
        if (true)                                                                               \
        {                                                                                       \
            int ret_get = _List_get(lst, _List_order_to_index(lst, order + 1), pull_val);       \
                                                                                                \
            if (ret_get != OK)                                                                  \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
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

#define List_verify(lst)                                                                        \
        if (true)                                                                               \
        {                                                                                       \
            unsigned int ret_verify = _List_verify(lst);                                        \
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
            int ret_fill_free = _List_fill_free(lst);                                           \
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
            int ret_realloc = _List_realloc(lst);                                               \
                                                                                                \
            if (ret_realloc != OK)                                                              \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_line(lst)                                                                          \
        if (true)                                                                               \
        {                                                                                       \
            int ret_line = _List_line(lst);                                                     \
                                                                                                \
            if (ret_line != OK)                                                                 \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

#define List_add_free(lst, index)                                                               \
        if (true)                                                                               \
        {                                                                                       \
            int ret_add_free = _List_add_free(lst, index);                                      \
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
            int ret_del_free = _List_del_free(lst);                                             \
                                                                                                \
            if (ret_del_free != OK)                                                             \
            {                                                                                   \
                log_place();                                                                    \
                return -1;                                                                      \
            }                                                                                   \
        }

/*________________________________USER_FUNCTION_DECLARATIONS_________________________________*/

void              List_dump             (List *const lst);
void              List_simple_dump      (List *const lst);
void              List_graph_dump       (List *const lst);

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

/*______________________________ADDITIONAL_FUNCTION_DECLARATIONS_____________________________*/

static unsigned int     _List_verify            (List *const lst);
static unsigned int     _List_info_verify       (List *const lst);

static void             *List_value_iterator    (List *const lst, const int index);
static List_elem_info   *List_info_iterator     (List *const lst, const int index);

static int              _List_realloc           (List *const lst);
static int              _List_fill_free         (List *const lst);

static int               List_cmp               (void *lst_a, void *lst_b);

static int              _List_check_linearity   (List *const lst, const int index);

static int              _List_add_free          (List *const lst, const int index);
static int              _List_del_free          (List *const lst);

static int              _List_push              (List *const lst, const int index, void *const push_val);
static int              _List_push_front        (List *const lst,                  void *const push_val);
static int              _List_push_back         (List *const lst,                  void *const push_val);

static void              describe_node          (List_elem_info *info, FILE *const stream, const int number);

/*___________________________________________________________________________________________*/

static unsigned int _List_verify(List *const lst)
{
    unsigned int err = OK;

    if (lst          == nullptr)                         return (1 << NULLPTR_LIST)             ;
    if (lst->is_ctor == false)                           return (1 << NOT_YET_CTORED)           ;

    if (lst->elem_size     < 0)                     err = err | (1 << NEGATIVE_ELEM_SIZE)       ;
    if (lst->data_size     < 0)                     err = err | (1 << NEGATIVE_DATA_SIZE)       ;
    if (lst->data_capacity < 0)                     err = err | (1 << NEGATIVE_DATA_CAPACITY)   ;

    if (lst->data_capacity < lst->data_size)        err = err | (1 << INVALID_CAPACITY)         ;
    if (lst->free          < default_list.free ||
        lst->free          > lst->data_capacity)    err = err | (1 << INVALID_FREE)             ;
    if (lst->data         == nullptr)               err = err | (1 << INVALID_DATA)             ;

    err = err | _List_info_verify(lst);

    return err;
}

static unsigned int _List_info_verify(List *const lst)
{
    unsigned nt err = 0;

    for (int info_index = 0; info_index < lst->data_size; ++info_index)
    {
        List_elem_info *cur_info = List_info_iterator(lst, info_index);

        if (cur_info->index   != info_index)    err = err | INVALID_INDEX       ;
        if (cur_info->is_free == false)
        {
            if (cur_info->next  == nullptr)     err = err | NULLPTR_LIST_INFO   ;
            if (cur_info->prev  == nullptr)     err = err | NULLPTR_LIST_INFO   ;
        }
    }

    return err;
}

void List_error(const unsigned int err)
{
    log_message("\n");
    log_error  ("LIST_VERIFY_FAILED\n");

    for (unsigned int err_bit = 0; err_bit < 8 * sizeof(int); ++err_bit)
    {
        if (err & (1 << err_bit)) log_error("%s\n", error_messages[err_bit]);
    }
}

/*___________________________________________________________________________________________*/

void List_dump(List *const lst)
{
    List_simple_dump(lst);  
    if (lst->data == nullptr) return;
    
    log_message("\n"
                "index: ");
    for (int index_cnt = 0; index_cnt < lst->data_capacity; ++index_cnt)
    {
        log_message("%-8d ", index_cnt);
    }

    log_message("\n\n"
                "value: ");
    for (int index_cnt = 0; index_cnt < lst->data_capacity; ++index_cnt)
    {
        List_elem_info *cur_info = List_info_iterator (lst, index_cnt);
        void           *cur_elem = List_value_iterator(lst, index_cnt);

        if      (cur_info          == nullptr) log_message(RED   "%-8s " CANCEL, "NO INFO"            );
        else if (cur_info->is_free == false)   log_message(USUAL "%-8d " CANCEL, *(int *) cur_elem);
        else                                   log_message(OLIVE "%-8s " CANCEL, "FREE"               );
    }

    log_message("\n"
                " next: ");
    for (int next_cnt  = 0; next_cnt  < lst->data_capacity; ++next_cnt)
    {
        List_elem_info *cur_info = List_info_iterator(lst, next_cnt);

        if      (cur_info       == nullptr)  log_message(RED   "%-8s " CANCEL, "NO INFO");
        else if (cur_info->next == nullptr)
        {
            if  (cur_info->is_free == false) log_message(RED   "%-8s " CANCEL, "NULL"   );
            else                             log_message(OLIVE "%-8s " CANCEL, "NULL"   );
        }
        else                                 log_message(USUAL "%-8d " CANCEL, cur_info->next->index);
    }

    log_message("\n"
                " prev: ");
    for (int prev_cnt  = 0; prev_cnt  < lst->data_capacity; ++prev_cnt)
    {
        List_elem_info *cur_info = List_info_iterator(lst, prev_cnt);

        if      (cur_info       == nullptr)  log_message(RED   "%-8s " CANCEL, "NO_INFO");
        else if (cur_info->prev == nullptr)
        {
            if  (cur_info->is_free == false) log_message(RED   "%-8s " CANCEL, "NULL"   );
            else                             log_message(OLIVE "%-8s " CANCEL, "NULL"   );
        }
        else                                 log_message(USUAL "%-8d " CANCEL, cur_info->prev->index);
    }
    log_message("\n");
}

/*___________________________________________________________________________________________*/

void List_simple_dump(List *const lst)
{
    log_message("\n"
                "List[%p]\n", lst);

    if (lst == nullptr) return;

    var_dump(&lst->var_info);

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
                "       is_linear:      %d\n"
                "       }\n",

                        lst->data,
                        lst->data_size,
                        lst->data_capacity,
                        
                        lst->elem_size,
                        
                        lst->free,
                        
                        lst->is_ctor,
                        lst->is_linear);
}

/*___________________________________________________________________________________________*/

void List_graph_dump(List *const lst)
{
    List_simple_dump(lst);
    if (lst->data == nullptr) return;

    static int cur_dump = 1;

    char    output_file[graph_file_size] = "";
    sprintf(output_file, "dump_txt/List_graph_dump%d.txt", cur_dump);

    FILE *stream =  fopen(output_file, "w");
    if   (stream == nullptr)
    {
        log_error("Can't open grah_dump output file\n");
        return;
    }

    setvbuf(stream, nullptr, _IONBF, 0);
    fprintf(stream, "digraph {\n"
                    "rankdir=LR\n"
                    "node[shape=record, style=\"rounded\", fontsize=8]\n");

    List_elem_info *info = nullptr;

    for (int cnt = 0; cnt < lst->data_capacity; ++cnt)
    {
        info = List_info_iterator(lst, cnt);
        describe_node(info, stream,    cnt);
    }

    info = List_info_iterator(lst, 0);
    for (int cnt = 0;; cnt++)
    {
        if (info->next != nullptr) 
        {
            if (info->next->index != 0) fprintf(stream, "node%d->node%d[weight=10000, color=\"red\"]\n", info->index, info->next->index);
            else                        fprintf(stream, "node%d->node%d[weight=1    , color=\"red\"]\n", info->index, info->next->index);
        }

        info = List_info_iterator(lst, info->next->index);

        if (info->index == 0 || cnt > lst->data_size) break;
    }

    info = List_info_iterator(lst, lst->free);
    for (int cnt = lst->free;; cnt++)
    {
        if (info->next != nullptr) fprintf(stream, "node%d->node%d[weight=10000, color=\"red\"]\n"       , info->index, info->next->index);

        info = List_info_iterator(lst, info->next->index);

        if (info->index == lst->free || cnt > lst->data_capacity - lst->data_size) break;
    }

    fprintf(stream, "}\n");

    char cmd[graph_cmd_size] = "";
    sprintf     (cmd, "dot %s -T png -o dump_png/List_graph_dump%d.png", output_file, cur_dump);
    system      (cmd);
    log_message ("<img width=500 src=dump_png/List_graph_dump%d.png>\n", cur_dump);

    fclose(stream);
    ++cur_dump;
}

static void describe_node(List_elem_info *info, FILE *const stream, const int number)
{
    assert(info   != nullptr);
    assert(stream != nullptr);

    GRAPH_COLOR fillcolor = WHITE;
    GRAPH_COLOR     color = WHITE; 

    if (!number)
    {
        fillcolor = WHITE;
            color = BLACK;
    }
    else if (info->is_free)
    {
        fillcolor = LIGHT_BLUE;
            color =  DARK_BLUE;
    }
    else
    {
        fillcolor = LIGHT_GREEN;
            color =  DARK_GREEN;
    }

    if (info->prev == nullptr)
    {
        if (info->next != nullptr)
            fprintf(stream, "node%d[color=\"%s\", fillcolor=\"%s\""
                            "label=\"{is_free = %d | index = %d | next = %d | prev = NULL}\"]\n" , number, color_names[color], color_names[fillcolor],
                                                                                                   info->is_free,
                                                                                                   info->index,
                                                                                                   info->next->index);
        else
            fprintf(stream, "node%d[color=\"%s\", fillcolor=\"%s\""
                            "label=\"{is_free = %d | index = %d | next = NULL | prev = NULL}\"]\n" , number, color_names[color], color_names[fillcolor],
                                                                                                     info->is_free,
                                                                                                     info->index  );
    }
    else //info->prev != nullptr
    {
        if (info->next != nullptr)
            fprintf(stream, "node%d[color=\"%s\", fillcolor=\"%s\""
                            "label=\"{is_free = %d | index = %d | next = %d | prev = %d}\"]\n" , number, color_names[color], color_names[fillcolor],
                                                                                                 info->is_free,
                                                                                                 info->index,
                                                                                                 info->next->index,
                                                                                                 info->prev->index);
        else
            fprintf(stream, "node%d[color=\"%s\", fillcolor=\"%s\""
                            "label=\"{is_free = %d | index = %d | next = NULL | prev = %d}\"]\n" , number, color_names[color], color_names[fillcolor],
                                                                                                   info->is_free,
                                                                                                   info->index,
                                                                                                   info->prev->index);
    }
}

/*___________________________________________________________________________________________*/

static int _List_realloc(List *const lst)
{
    List_verify(lst);

    if (lst->data_size == lst->data_capacity)
    {
        int future_capacity = 2 * lst->data_capacity;
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

static int _List_fill_free(List *const lst)
{
    List_verify(lst);

    int elem_cnt = lst->free = lst->data_size;
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

static void *List_value_iterator(List *const lst, const int index)
{
    assert(lst != nullptr);

    return (char *) lst->data + index * (lst->elem_size + sizeof(List_elem_info)) + sizeof(List_elem_info);
}

static List_elem_info *List_info_iterator(List *const lst, const int index)
{
    assert(lst != nullptr);

    return (List_elem_info *) ((char *) lst->data + index * (lst->elem_size + sizeof(List_elem_info)));
}

/*___________________________________________________________________________________________*/

int _List_ctor(List *const lst, const int elem_size,    const char *name_file,
                                                        const char *name_func,
                                                        const char *name_var ,
                                                        const int   line   )
{
    if (lst          == nullptr) return 1 << NULLPTR_LIST      ;
    if (lst->is_ctor == true   ) return 1 << ALREADY_CTORED    ;
    
   *lst                = default_list;
    lst->elem_size     = elem_size   ;
    lst->data          = calloc(lst->data_capacity, sizeof(List_elem_info) + lst->elem_size);
    
    if (lst->data == nullptr) return 1 << MEMORY_LIMIT_EXCEEDED;

    var_ctor(&lst->var_info, name_file, name_func, name_var, line);

    List_elem_info *fictional = List_info_iterator(lst, 0);
    
    fictional->index          = 0;
    fictional->is_free        = false;
    fictional->next           = fictional;
    fictional->prev           = fictional;

    List_fill_free(lst);
    List_verify   (lst);
    return OK;
}

int _List_dtor(List *const lst)
{
    List_verify(lst);

   free(lst->data);

   *lst = poison_list;
   var_dtor(&lst->var_info);

   return OK;
}

/*___________________________________________________________________________________________*/

static int List_cmp(void *lst_a, void *lst_b)
{
    assert(lst_a != nullptr);
    assert(lst_b != nullptr);

    List_elem_info *a_info = (List_elem_info *) lst_a;
    List_elem_info *b_info = (List_elem_info *) lst_b;

    if ((long long) a_info->prev < (long long) b_info->prev) return -1;
    if ((long long) a_info->prev > (long long) b_info->prev) return  1;

    return 0;
}

int _List_line(List *const lst)
{
    List_verify(lst);

    long long       logic_cnt = 0;
    List_elem_info *cur_info  = List_info_iterator(lst, 0);

    for (; logic_cnt < lst->data_size; ++logic_cnt)
    {
        cur_info->prev = (List_elem_info *) logic_cnt;
        cur_info       = cur_info->next;
    }

    if (lst->free != lst->data_capacity)
    {
        cur_info = List_info_iterator(lst, lst->free);

        for (; cur_info != nullptr; ++logic_cnt)
        {
            cur_info->prev = (List_elem_info *) logic_cnt;
            cur_info       = cur_info->next;
        }
    }

    my_quick_sort(lst->data, sizeof(List_elem_info) + lst->elem_size, 0, lst->data_capacity - 1, List_cmp);

    for (int cnt = 0; cnt < lst->data_size; ++cnt)
    {
        List_elem_info *cur_info = List_info_iterator(lst, cnt);
        int prev_index           = (cnt - 1) <               0 ? lst->data_size - 1 : cnt - 1;
        int next_index           = (cnt + 1) == lst->data_size ?                  0 : cnt + 1;

        cur_info->index = cnt;
        cur_info->next  = List_info_iterator(lst, next_index);
        cur_info->prev  = List_info_iterator(lst, prev_index);
    }

    lst->is_linear = true;

    List_fill_free(lst);
    List_verify   (lst);
    return OK;
}

/*___________________________________________________________________________________________*/

int _List_order_to_index(List *const lst, int order)
{
    assert(lst != nullptr);

    if (lst->is_linear) return order;

    int             index = 0;
    List_elem_info *info  = List_info_iterator(lst, index);

    if (order <= lst->data_size)
    {
        while (order--)
        {
            info  = info->next;
            index = info->index;
        }
    }
    else
    {
        order = lst->data_size - order;
        while (order--)
        {
            info  = info->prev;
            index = info->index;
        }
    }

    return index;
}

static int _List_check_linearity(List *const lst, const int index)
{
    List_verify(lst);

    List_elem_info *cur_info = List_info_iterator(lst, 0);

    if (cur_info->prev->index != index) lst->is_linear = false;
    return OK;
}

/*___________________________________________________________________________________________*/

int _List_push(List *const lst, const int index, void *const push_val,  const char   *call_file,
                                                                        const char   *call_func,
                                                                        const int     call_line)
{
    assert(call_file != nullptr);
    assert(call_func != nullptr);

    List_verify(lst);

    int ret_push = _List_push(lst, index, push_val);
    if     (ret_push == -1)
    {
        log_param_place(call_file, call_func, call_line);
        return -1;
    }

    return ret_push;
}

int _List_push(List *const lst, const int index, void *const push_val)
{
    List_verify         (lst);
   _List_check_linearity(lst, index);
    List_realloc        (lst);
    
    
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

int _List_push_order(List *const lst, const int order, void *const push_val, const char *call_file,
                                                                             const char *call_func,
                                                                             const int   call_line)
{
    assert(call_file != nullptr);
    assert(call_func != nullptr);

    List_verify(lst);

    int ret_push = _List_push(lst, _List_order_to_index(lst, order), push_val);

    if (ret_push == -1)
    {
        log_param_place(call_file, call_func, call_line);
        return -1;
    }
    return ret_push;
}

/*___________________________________________________________________________________________*/

int _List_pop(List *const lst, const int index)
{
    List_verify         (lst);
   _List_check_linearity(lst, index);

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

int _List_get(List *const lst, const int index, void *const pull_val)
{
    List_verify(lst);

    memcpy(pull_val, List_value_iterator(lst, index), lst->elem_size);

    List_verify(lst);

    return OK;
}

/*___________________________________________________________________________________________*/

static int _List_add_free(List *const lst, const int index)
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

static int _List_del_free(List *const lst)
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

int _List_push_front(List *const lst, void *const push_val, const char   *call_file,
                                                            const char   *call_func,
                                                            const int     call_line)
{
    assert(call_file != nullptr);
    assert(call_func != nullptr);

    List_verify(lst);

    int ret_push_front = _List_push_front(lst, push_val);
    if (ret_push_front == -1)
    {
        log_param_place(call_file, call_func, call_line);
        return -1;
    }

    return ret_push_front;
}

static int _List_push_front(List *const lst, void *const push_val)
{
    List_verify(lst);

    return List_push(lst, 0, push_val);
}

/*___________________________________________________________________________________________*/

int _List_push_back(List *const lst, void *const push_val,  const char   *call_file,
                                                            const char   *call_func,
                                                            const int     call_line)
{
    assert(call_file != nullptr);
    assert(call_func != nullptr);

    List_verify(lst);

    int ret_push_back = _List_push_back(lst, push_val);                             
    if (ret_push_back == -1)        
    {
        log_param_place(call_file, call_func, call_line);
        return -1;
    }

    return ret_push_back;
}

int _List_push_back(List *const lst, void *const push_val)
{
    List_verify(lst);

    return List_push(lst, List_info_iterator(lst, 0)->prev->index, push_val);
}

/*___________________________________________________________________________________________*/

int _List_pop_front(List *const lst)
{
    List_verify(lst);

    return _List_pop(lst, List_info_iterator(lst, 0)->next->index);
}

/*___________________________________________________________________________________________*/

int _List_pop_back(List *const lst)
{
    List_verify(lst);

    return _List_pop(lst, List_info_iterator(lst, 0)->prev->index);
}

/*___________________________________________________________________________________________*/

int _List_front(List *const lst, void *const pull_val)
{
    List_verify(lst);

    return _List_get(lst, List_info_iterator(lst, 0)->next->index, pull_val);
}

/*___________________________________________________________________________________________*/

int _List_back(List *const lst, void *const pull_val)
{
    List_verify(lst);

    return _List_get(lst, List_info_iterator(lst, 0)->prev->index, pull_val);
}

/*___________________________________________________________________________________________*/
