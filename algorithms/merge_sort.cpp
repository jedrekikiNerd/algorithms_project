#include <iostream>
#include "../data_structures/dynamic_array.h"
#include "../data_structures/film_struct.h"
#include "../data_structures/stack.hpp"
#include "sorts.hpp"

// Merge both halfs of dynarray (while merging sort is performed choosing smaller element to merge on left side)
void merge(DynamicArray<film_struct>* dynarray, int left, int middle, int right) 
{
    int n1 = middle - left + 1; // Left dynarray size
    int n2 = right - middle; // Right dynarray size

    // We make left and right arrays
    DynamicArray<film_struct> L(n1), R(n2);

    // We place corresponding data to left and right dynarray
    for (int i = 0; i < n1; i++)
        L[i] = (*dynarray)[left + i];
    for (int i = 0; i < n2; i++)
        R[i] = (*dynarray)[middle + 1 + i];

    // We merge both dynarrays
    int left_index = 0;
    int right_index = 0;
    int whole_index = left;

    while (left_index < n1 and right_index < n2) 
    {
        if (L[left_index].rank <= R[right_index].rank) 
        {
            (*dynarray)[whole_index] = L[left_index];
            left_index++;
        }
        else 
        {
            (*dynarray)[whole_index] = R[right_index];
            right_index++;
        }
        whole_index++;
    }
    // Copy all that left
    while (left_index < n1) {
        (*dynarray)[whole_index] = L[left_index];
        left_index++;
        whole_index++;
    }

    // Copy all that left
    while (right_index < n2) {
        (*dynarray)[whole_index] = R[right_index];
        right_index++;
        whole_index++;
    }
}

// Main merge sort function
void merge_sort(DynamicArray<film_struct>* dynarray, int left, int right)
{
    if (left < right)
    {
        int middle = left + (right - left) / 2; // Obliczamy środek tablicy

        // We call merge sorts until we get smallest parts
        merge_sort(dynarray, left, middle);
        merge_sort(dynarray, middle + 1, right);

        // We merge both parts
        merge(dynarray, left, middle, right);
    }
}
