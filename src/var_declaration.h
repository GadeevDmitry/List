#ifndef VAR_DECLARATION_H
#define VAR_DECLARATION_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

#include "/home/dima/Github/my_libraries/logs/log.h"

struct var_declaration
{
    const char *name_file, *name_func, *name_var;
    uint32_t    line;    
};

const var_declaration poison_var_declaration = 
{
    nullptr, // name_file
    nullptr, // name_func
    nullptr, // name_var
    
          0  // line
};

/*___________________________FUNCTION_DECLARATION___________________________*/

static void var_ctor (var_declaration *const var, const char *name_file, const char *name_func, const char *name_var, const uint32_t line);
static void var_dtor (var_declaration *const var);
static void var_dump (var_declaration *const var);

/*__________________________________________________________________________*/

/**
*   @brief "var_declaration" constructor.
*
*   @param var       [out] - pointer to the "var_declaration" to construct
*   @param name_file [in]  - pointer to the first byte of null-terminated byte string means (var->name_file)-value
*   @param name_func [in]  - pointer to the first byte of null-terminated byte string means (var->name_func)-value
*   @param name_var  [in]  - pointer to the first byte of null-terminated byte string means (var->name_var )-value
*   @param line      [in]  - number means (var->line)-value
*
*   @return nothing
*/

static void var_ctor(var_declaration *const var, const char *name_file, const char *name_func, const char *name_var, const uint32_t line)
{
    assert(var != nullptr);
    
    assert(name_file != nullptr);
    assert(name_func != nullptr);
    assert(name_var  != nullptr);

    var->name_file = name_file;
    var->name_func = name_func;
    var->name_var  = name_var ;
    var->     line =      line;
}

/**
*   @brief Dumps "var_declaration" in LOG_FILE.
*
*   @param var [in] - pointer to the variable to dump
*
*   @return nothing
*/

static void var_dump(var_declaration *const var)
{
    assert(var != nullptr);

    log_message("\n");

    log_char_ptr("name    of var : ", var->name_var );
    log_char_ptr("created in file: ", var->name_file);
    log_char_ptr("created in func: ", var->name_func);
    log_message ("created in line: %d\n",  var->line);
    
    log_message("\n");
}

/**
*   @brief "var_declaration" destructor. Fills variable by poison-values.
*
*   @param var    [in] - pointer to the "var_declaration" to destruct
*   @param poison [in] - pointer to the "var_declaration" with poison-values
*
*   @return nothing
*/

static void var_dtor(var_declaration *const var)
{
    assert(var != nullptr);

    *var = poison_var_declaration;
}

#endif //VAR_DECLARATION_H