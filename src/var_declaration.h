#ifndef VAR_DECLARATION_H
#define VAR_DECLARATION_H

struct var_declaration
{
    const char *name_file, *name_func, *name_var;
    uint32_t    line;    
};

/*___________________________FUNCTION_DECLARATION___________________________*/

void var_ctor (var_declaration *const var, const char *name_file, const char *name_func, const char *name_var, const uint32_t line);
void var_dtor (var_declaration *const var);
void var_dump (var_declaration *const var);

/*__________________________________________________________________________*/

#endif //VAR_DECLARATION_H