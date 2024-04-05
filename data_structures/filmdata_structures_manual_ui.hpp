#ifndef manual_ui_dt
#define manual_ui_dt

#include <iostream>
#include "I_data_structure.h"
#include "dynamic_array.h"
#include "film_struct.h"


// Agent between UI and data structure add_front
int add_front(DynamicArray<film_struct> *dynarray);

// Agent between UI and data structure add_back
int add_back(DynamicArray<film_struct> *dynarray);

// Agent between UI and data structure add_at
int add_at(DynamicArray<film_struct> *dynarray);

// Agent between UI and data structure remove_front
int remove_front(DynamicArray<film_struct> *dynarray);

// Agent between UI and data structure remove_back
int remove_back(DynamicArray<film_struct> *dynarray);

// Agent between UI and data structure remove_at
int remove_at(DynamicArray<film_struct> *dynarray);

// Agent between UI and data structure clear_dt
int clear_dt(DynamicArray<film_struct> *dynarray);

// Agent between UI and data structure first_value
int first_value(DynamicArray<film_struct> *dynarray);

// Agent between UI and data structure last_value
int last_value(DynamicArray<film_struct> *dynarray);

// Agent between UI and data structure value_at
int value_at(DynamicArray<film_struct> *dynarray);

// Agent between UI and data structure get_size
int get_size(DynamicArray<film_struct> *dynarray);

// Agent between UI and data structure find
int find_by_title(DynamicArray<film_struct> *dynarray);

int find_and_delete_empty_ranks(DynamicArray<film_struct> *dynarray);

// Agent between UI and data structure get_as_string
int get_as_string(DynamicArray<film_struct> *dynarray);

// Perform sort on dynamic array
int sort(DynamicArray<film_struct> *dynarray);

// Check if sorted
int check_sort(DynamicArray<film_struct> *dynarray);

// Function that fills data structure with data from file
int fill_from_file(DynamicArray<film_struct> *dynarray);

// Agent bewteen UI and data structure change_at
int change_at(DynamicArray<film_struct> *dynarray);

#endif
