#include <stdio.h>
#include <stdlib.h>

#include "src/list.h"

struct pair
{
    int first;
    int second;
};

int main()
{
    List test  = {};

    List_ctor(&test , sizeof(pair));

    pair  i       = {0, 0};
    int index[10] = {};

    List_push_order(&test, 0, &(i = {3, 3}));
    List_push_order(&test, 1, &(i = {4, 4}));
    List_push_order(&test, 2, &(i = {5, 5}));
    List_push_back (&test,    &(i = {8, 8}));
    List_pop_order (&test, 3);
    List_push_order(&test, 3, &(i = {9, 9}));
    List_push_order(&test, 1, &(i = {7, 7}));
    List_pop       (&test, 2);

    //List_dump(&test);

    int all = test.data_size - 1;

    for (int i = 0; i < all; ++i)
    {
        pair cur = {0, 0};

        List_front    (&test, &cur);
        printf("List[%d] = {%d, %d}\n", i, cur.first, cur.second);
        List_pop_front(&test);
    }

    List_dtor(&test);
}
