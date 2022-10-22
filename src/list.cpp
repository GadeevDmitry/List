#include "list.h"

enum LIST_VERDICT = 
{
    OK                                      ,   // 0
    FAILED_CANARY_PROTECTION_LEFT           ,   // 1
    FAILED_CANARY_PROTECTION_RIGHT          ,   // 2
    FAILED_HASH_PROTECTION                  ,   // 3
    
};

