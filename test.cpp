#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "src/list.h"

int main()
{
    List test = {};

    List_ctor(&test, sizeof(int));

    //var_dump(&test.var_info);

    int i         = 0 ;
    int index[10] = {};

    index[1] = List_push_front(&test,           &(i = 1));
    index[3] = List_push_back (&test,           &(i = 3));
    index[8] = List_push      (&test, index[1], &(i = 8));
    index[4] = List_push      (&test, index[1], &(i = 4));
    index[5] = List_push      (&test, index[4], &(i = 5));
    index[6] = List_push      (&test, index[1], &(i = 6));
    index[7] = List_push_front(&test,           &(i = 7));

    List_pop(&test, index[1]);
    List_pop(&test, index[8]);

    List_dump(&test);

    for (int i = 0; i < 5; ++i)
    {
        int cur = 0;

        List_front    (&test, &cur);
        printf("List[%d] = %d\n", i, cur);
        List_pop_front(&test);
    }

    List_dtor(&test);
}
