#include "headers/sort.h"
#include <assert.h>
#include <string.h>

__attribute__((flatten)) void
sort(void *const array, const size_t size, const int length, const Comp comp)
{
    insert_sort(array, size, length, comp);
}

// TODO: now that I've added a sorting function might as well check if it can be
// used elsewhere
void insert_sort(void *const array,
                 const size_t size,
                 const int length,
                 const Comp comp)
{
    assert(array != NULL && size > 0);
    char key[size];

    for (int j = 1; j < length; ++j)
    {
        memcpy(key, array + j * size, size);
        int pos = j - 1;

        while (pos >= 0 && comp(key, array + pos * size) < 0)
        {
            memmove(array + (pos + 1) * size, array + pos * size, size);
            pos--;
        }

        memcpy(array + (pos + 1) * size, key, size);
    }
}
