#include <iostream>
#include "../data_structures/dynamic_array.h"
#include "../data_structures/film_struct.h"

void quick_sort(DynamicArray<film_struct>* dynarray, unsigned int left, unsigned int right);
void bucket_sort(DynamicArray<film_struct>* dynarray, unsigned int n);