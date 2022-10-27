#ifndef ALGORITHM_H
#define ALGORITHM_H

static void my_swap(void *a, void *b, const int elem_size)
{
    assert(a != nullptr);
    assert(b != nullptr);

    unsigned char temp = '\0';

    for (size_t i = 0; i < elem_size; ++i)
    {
        temp = *((unsigned char *)a + i);
               *((unsigned char *)a + i) = *((unsigned char *)b + i);
               *((unsigned char *)b + i) = temp;
    }
}

static void *get_ptr(void *begin_array, const int index, const int elem_size)
{
    assert(begin_array != nullptr);

    void  *ptr = (unsigned char *)begin_array + index * elem_size;
    return ptr;
}

static void my_quick_sort(void *data, int elem_size, const int left, const int right, int (*cmp) (void * elem1, void * elem2))
{
    assert(data != nullptr);
    assert(cmp  != nullptr);

    if    (left >= right) return;

    int mid = (left + right) / 2;

    my_swap(get_ptr(data, left, elem_size), get_ptr(data, mid, elem_size), elem_size);

    int cut = left;
    for (int i = left + 1; i <= right; ++i) {

        if ((*cmp)(get_ptr(data, i, elem_size), get_ptr(data, left, elem_size)) <= 0) {

            ++cut;

            my_swap(get_ptr(data, cut, elem_size), get_ptr(data, i, elem_size), elem_size);
        }
    }

    my_swap(get_ptr(data, cut, elem_size), get_ptr(data, left, elem_size), elem_size);

    my_quick_sort(data, elem_size,    left, cut - 1, cmp);
    my_quick_sort(data, elem_size, cut + 1,   right, cmp);
}

#endif //ALGORITHM_H