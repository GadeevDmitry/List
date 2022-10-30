#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "var_declaration.h"
#include "log.h"
#include "algorithm.h"

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

const List default_list =
{
    {
        nullptr,
        nullptr,
        nullptr,
        0
    }       ,
    nullptr ,

    0       ,
    1       ,

    true
};

const List poison_list =
{
    {
        nullptr,
        nullptr,
        nullptr,
        0
    }       ,
    nullptr ,

    0       ,
    0       ,

    false
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

const char *error_messages[] = 
{
    "OK"                                        ,

    "List           is invalid"                 ,
    "List node      is invalid"                 ,
    "Fictional node is invalid"                 ,

    "List is already ctored"                    ,
    "List is not yet ctored"                    ,

    "Invalid list_element size"                 ,
    "Invalid list_data    size"                 ,

    "Cycle of list nodes is invalid"            ,

    "Memory limit exceeded"
};

static const int graph_file_size = 40           ;
static const int graph_cmd_size  = 100          ;

enum GRAPH_COLOR
{
    LIGHT_BLUE      ,
    LIGHT_RED       ,
    LIGHT_GREEN     ,
    LIGHT_GREY      ,

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
    "lightgrey"     ,

    "darkblue"      ,
    "darkred"       ,
    "darkgreen"     ,

    "white"         ,
    "black"
};

/*_____________________________________MACRO_DEFINITIONS_____________________________________*/

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

/*______________________________ADDITIONAL_FUNCTION_DECLARATIONS_____________________________*/

static unsigned int _List_verify                (List *const lst);
static unsigned int _List_node_verify           (List *const lst);

static void          List_simple_dump           (List *const lst);
static void          List_graph_dump_generate   (List *const lst, FILE *const stream);
static void          describe_node              (List_node *node, FILE *const stream, const int number);

static List_node   *_List_get_node              (List *const lst, int index);


/*___________________________________________________________________________________________*/

static unsigned int _List_verify(List *const lst)
{
    unsigned int err = OK;

    if (lst            == nullptr)      return (1 << NULLPTR_LIST)      ;
    if (lst->is_ctor   ==   false)      return (1 << NOT_YET_CTORED)    ;

    if (lst->fictional == nullptr) err = err | (1 << NULLPTR_FICTIONAL) ;
    if (lst->elem_size <=       0) err = err | (1 << INVALID_ELEM_SIZE) ;
    if (lst->data_size <=       0) err = err | (1 << INVALID_DATA_SIZE) ;
    if (lst->fictional != nullptr) err = err | _List_node_verify(lst)   ;

    return err;
}

static unsigned int _List_node_verify(List *const lst)
{
    List_node *cur_node = lst->fictional->next;

    for (int cnt = 1; cnt < lst->data_size; ++cnt)
    {
        if (cur_node ==        nullptr) return INVALID_NODE_CYCLE;
        if (cur_node == lst->fictional) return INVALID_NODE_CYCLE;

        cur_node = cur_node->next;
    }

    if (cur_node != lst->fictional) return INVALID_NODE_CYCLE;
    return OK;
}

void List_error(const unsigned int err)
{
    log_message("\n");
    log_error  ("LIST_VERIFY_FAILED\n");

    for (unsigned int err_bit = 1; err_bit < 8 * sizeof(int); ++err_bit)
    {
        if (err & (1 << err_bit)) log_error("%s\n", error_messages[err_bit]);
    }
}

/*___________________________________________________________________________________________*/

void List_dump(List *const lst)
{

    List_simple_dump(lst);
    if (lst->fictional == nullptr) return;

    log_message("\n\n"
                "index: ");
    for (int index_cnt = 0; index_cnt < lst->data_size; ++index_cnt)
    {
        log_message("%-16d ", index_cnt);
    }

    List_node *cur_node = lst->fictional;
    log_message("\n"
                " next: ");
    for (int next_cnt = 0; next_cnt < lst->data_size; ++next_cnt)
    {
        if      (cur_node       == nullptr) break;
        else if (cur_node->next == nullptr) log_message(RED   "%-16s " CANCEL, "NULL");
        else                                log_message(USUAL "%-16p " CANCEL, cur_node->next);

        cur_node = cur_node->next;
    }

    cur_node = lst->fictional;
    log_message("\n"
                " prev: ");
    for (int prev_cnt = 0; prev_cnt < lst->data_size; ++prev_cnt)
    {
        if      (cur_node       == nullptr)  break;
        else if (cur_node->prev == nullptr) log_message(RED   "%-16s " CANCEL, "NULL");
        else                                log_message(USUAL "%-16p " CANCEL, cur_node->prev);
    }
    log_message("\n");
}

/*___________________________________________________________________________________________*/

static void List_simple_dump(List *const lst)
{
    log_message("\n"
                "List[%p]\n", lst);

    if (lst == nullptr) return;

    var_dump(&lst->var_info);

    log_message("List = {\n"
                "       fictional   %p\n"
                "                     \n"
                "       elem_size:  %d\n"
                "       data_size:  %d\n"
                "                     \n"
                "       is_ctor:    %d\n"
                "       }\n",

                        lst->fictional,

                        lst->elem_size,
                        lst->data_size,

                        lst->is_ctor);
}

/*___________________________________________________________________________________________*/

void List_graph_dump(List *const lst)
{
    List_simple_dump(lst);
    if (lst->fictional == nullptr) return;

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
                    "node[shape=record, style=\"rounded, filled\", fontsize=8]\n");

    List_graph_dump_generate(lst, stream);

    fprintf(stream, "}\n");

    char cmd[graph_cmd_size] = "";
    sprintf     (cmd, "dot %s -T png -o dump_png/List_graph_dump%d.png", output_file, cur_dump);
    system      (cmd);
    log_message ("<img width=95%% src=dump_png/List_graph_dump%d.png>\n", cur_dump);

    fclose(stream);
    ++cur_dump;
}

static void List_graph_dump_generate(List *const lst, FILE *const stream)
{
    List_node *node = lst->fictional;

    for (int cnt = 0; cnt < lst->data_size && node != nullptr; ++cnt)
    {
        describe_node(node, stream, cnt);
        node = node->next;
    }
    node = lst->fictional;
    for (int cnt = 0; cnt < lst->data_size && node != nullptr; ++cnt)
    {
        if (node->next != nullptr) fprintf(stream, "node%d->node%d[weight=10000, color=\"red\"]\n", cnt, cnt+1);
        else                       break;
    }
}

static void describe_node(List_node *node, FILE *const stream, const int number)
{
    assert(node   != nullptr);
    assert(stream != nullptr);

    GRAPH_COLOR fillcolor = WHITE;
    GRAPH_COLOR     color = WHITE; 

    if (!number)
    {
        fillcolor = LIGHT_GREY;
            color = BLACK;
    }
    else
    {
        fillcolor = LIGHT_GREEN;
            color =  DARK_GREEN;
    }

    fprintf(stream, "node%d[color=\"%s\", fillcolor=\"%s\""
                    "label=\"{data = %p} \n| {prev = %p} \n| {next = %p}\"]", number, color_names[color], color_names[fillcolor],
                        node->data,     node->prev,     node->next);
}

/*___________________________________________________________________________________________*/

int _List_ctor(List *const lst, const int elem_size,    const char *name_file,
                                                        const char *name_func,
                                                        const char *name_var ,
                                                        const int   line     )
{
    if (lst          == nullptr) return 1 << NULLPTR_LIST      ;
    if (lst->is_ctor == true   ) return 1 << ALREADY_CTORED    ;
    
   *lst            = default_list;
    lst->elem_size = elem_size   ;
    var_ctor(&lst->var_info, name_file, name_func, name_var, line);

    lst->fictional = (List_node *) calloc(1, sizeof(List_node));
    if (lst->fictional == nullptr) return MEMORY_LIMIT_EXCEEDED;
    lst->fictional->prev = lst->fictional->next = lst->fictional;

    List_verify(lst);
    return OK;
}

int _List_dtor(List *const lst)
{
    List_verify(lst);

    List_node *node = lst->fictional->prev;
    while (true)
    {
        free(node->next->data);
        free(node->next);
        
        if  (node->prev == lst->fictional)
        {
            free(node);
            break;
        }
        node = node->prev;
    }

   *lst = poison_list;
    var_dtor(&lst->var_info);

   return OK;
}

/*___________________________________________________________________________________________*/

int _List_insert(List *const lst, int index, void *const push_val)
{
    List_verify(lst);

    List_node *pocket_node = _List_get_node(lst, index);

    List_node *pushed_node = (List_node *) calloc(1, sizeof(List_node));
    if        (pushed_node == nullptr)     return MEMORY_LIMIT_EXCEEDED;

    pushed_node->data = calloc(1, lst->elem_size);
    if (pushed_node->data == nullptr) return MEMORY_LIMIT_EXCEEDED;

    memcpy(pushed_node->data, push_val, lst->elem_size);

    pushed_node->next = pocket_node->next;
    pushed_node->prev = pocket_node      ;
    pocket_node->next = pushed_node      ;
    pushed_node->next->prev = pushed_node;

    List_verify(lst);
    return OK;
}

/*___________________________________________________________________________________________*/

int _List_erase(List *const lst, int index)
{
    List_verify(lst);

    List_node *node = _List_get_node(lst, index + 1);

    node->prev->next = node->next;
    node->next->prev = node->prev;

    free(node->data);
    free(node);

    List_verify(lst);
    return OK;
}

/*___________________________________________________________________________________________*/

int _List_get(List *const lst, int index, void *const pull_val)
{
    List_verify(lst);

    List_node *node = _List_get_node(lst, index + 1);
    memcpy(pull_val, node->data, lst->elem_size);

    List_verify(lst);

    return OK;
}

/*___________________________________________________________________________________________*/

static List_node *_List_get_node(List *const lst, int index)
{
    List_node *node = lst->fictional;
    while (index--) node = node->next;

    return node;
}

/*___________________________________________________________________________________________*/

int _List_push_front(List *const lst, void *const push_val)
{
    List_verify(lst);
    List_insert(lst, 0, push_val);
    return OK;
}

/*___________________________________________________________________________________________*/

int _List_push_back(List *const lst, void *const push_val)
{
    List_verify(lst);
    List_insert(lst, lst->data_size - 1, push_val);
    return OK;
}

/*___________________________________________________________________________________________*/

int _List_pop_front(List *const lst)
{
    List_verify(lst);
    List_erase (lst, 0);
    return OK;
}

/*___________________________________________________________________________________________*/

int _List_pop_back(List *const lst)
{
    List_verify(lst);
    List_erase (lst, lst->data_size - 2);
    return OK;
}

/*___________________________________________________________________________________________*/

int _List_front(List *const lst, void *const pull_val)
{
    List_verify(lst);
    List_get   (lst, 0, pull_val);
    return OK;
}

/*___________________________________________________________________________________________*/

int _List_back(List *const lst, void *const pull_val)
{
    List_verify(lst);
    List_get   (lst, lst->data_size - 2, pull_val);
    return OK;
}

/*___________________________________________________________________________________________*/
