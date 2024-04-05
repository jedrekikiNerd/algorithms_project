#include <iostream>
#include "../data_structures/dynamic_array.h"
#include "../data_structures/film_struct.h"
#include "sorts.hpp"

// Bucket sorting function
void bucket_sort(DynamicArray<film_struct>* dynarray, unsigned int n) 
{
    // We create empty buckets that we will fill later
    DynamicArray<DynamicArray<film_struct>> buckets(11);

    // We put films into buckets by their rank
    for (unsigned int i = 0; i < n; i++)
    {
        unsigned int bi = (*dynarray)[i].rank;
        buckets[bi].add_back((*dynarray)[i]);
    }

    // We reconstruct the dynarray from buckets - that will give us sorted dynarray
    unsigned int index = 0;
    for (unsigned int i = 0; i < 11; i++) 
    {
        for (unsigned int j = 0; j < buckets[i].get_size(); j++) 
        {
            (*dynarray)[index] = buckets[i][j];
            index++;
        }
    }
}