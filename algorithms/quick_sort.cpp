#include <iostream>
#include "../data_structures/dynamic_array.h"
#include "../data_structures/film_struct.h"
#include "../data_structures/stack.hpp"
#include "sorts.hpp"
#include <random>
#include <curses.h>

// Helper function to choose random pivot
unsigned int choose_pivot(unsigned int left, unsigned int right) 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(left, right);
    return dis(gen);
}

// Function partitioning array with given pivot
unsigned int partition_old(DynamicArray<film_struct>* dynarray, unsigned int left, unsigned int right)
{
	// Choose random pivot with our helper function and hen get its value and move to end
    unsigned int pivotIndex = left + (right - left) / 2;
    film_struct pivotValue = (*dynarray)[pivotIndex];
    std::swap((*dynarray)[pivotIndex], (*dynarray)[right]);
    unsigned int partitionIndex = left;

    for (unsigned int i = left; i < right; i++)
	{
        if ((*dynarray)[i].rank < pivotValue.rank)
		{
            std::swap((*dynarray)[i], (*dynarray)[partitionIndex]);
            partitionIndex++;
        }
    }
	// Place pivot on right place
    std::swap((*dynarray)[partitionIndex], (*dynarray)[right]);
    return partitionIndex;
}


// Function that is partitioning dyn array but works faster than older one because of usage of two variables to iterate at once
unsigned int partition(DynamicArray<film_struct>* dynarray, unsigned int left, unsigned int right) {
    unsigned int pivotIndex = left + (right - left) / 2;
    film_struct pivotValue = (*dynarray)[pivotIndex];
    unsigned int i = left;
    unsigned int j = right;

    while (i <= j)
    {
        while ((*dynarray)[i].rank < pivotValue.rank)
        {
            i++;
        }
        while ((*dynarray)[j].rank > pivotValue.rank)
        {
            j--;
        }
        if (i <= j)
        {
            std::swap((*dynarray)[i], (*dynarray)[j]);
            i++;
            j--;
        }
    }
    // We return partition place + 1
    return i;
}


// Function quick sorting given dynamic array object
void quick_sort(DynamicArray<film_struct>* dynarray, unsigned int left, unsigned int right)
{
    Stack<unsigned int> stack;
    stack.push(left);
    stack.push(right);

    while (!stack.empty()) 
	{
        right = stack.pop();
        left = stack.pop();
        unsigned int partition_index = partition(dynarray, left, right);
        

        // Recurency but with stack implementation
        if (partition_index-1 > left and partition_index > 0) {
            stack.push(left);
            stack.push(partition_index-1);
        }

        if (partition_index < right) {
            stack.push(partition_index);
            stack.push(right);
        }
    }
}
