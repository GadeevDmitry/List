#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "src/list.h"

int main()
{
    List test = {};

    List_ctor(&test, sizeof(int));

    int i         = 0 ;
    int index[10] = {};

    List_push_order(&test, 0, &(i = 3));
    List_push_order(&test, 1, &(i = 4));
    List_push_order(&test, 2, &(i = 5));
    List_push_order(&test, 3, &(i = 8));
    List_pop_order (&test, 3);
    List_push_order(&test, 3, &(i = 9));
    List_push_order(&test, 1, &(i = 7));
    
    /*


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
    List_line(&test);
    List_dump(&test);
    */
    
    int all = test.data_size - 1;

    List_dump(&test);

    for (int i = 0; i < all; ++i)
    {
        int cur = 0;

        List_front    (&test, &cur);
        printf("List[%d] = %d\n", i, cur);
        List_pop_front(&test);
    }

    List_dtor(&test);
}
