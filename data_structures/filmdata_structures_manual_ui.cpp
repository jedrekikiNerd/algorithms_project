#include <iostream>
#include "../ui_actions.h"
#include "filmdata_structures_manual_ui.hpp"
#include "I_data_structure.h"
#include "dynamic_array.h"
#include "../Timer.hpp"
#include <fstream>
#include <limits>
#include <iomanip>
#include "../algorithms/sorts.hpp"

// Agent between UI and data structure add_front
int add_front(DynamicArray<film_struct> *dynarray)
{
    film_struct value = user_input_action_film();
    Timer timer;
    timer.start();
    dynarray->add_front(value);
    timer.stop();
    if (print_time)
        display_action("Dodawanie zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}

// Agent between UI and data structure add_back
int add_back(DynamicArray<film_struct> *dynarray)
{
    film_struct value = user_input_action_film();
    Timer timer;
    timer.start();
    dynarray->add_back(value);
    timer.stop();
    if (print_time)
        display_action("Dodawanie zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}

// Agent between UI and data structure add_at
int add_at(DynamicArray<film_struct> *dynarray)
{
    film_struct value = user_input_action_film();
    unsigned int pos = user_input_action("Podaj pozycję na której chcesz dodać film: ");
    Timer timer;
    timer.start();
    dynarray->add_at(value, pos);
    timer.stop();
    if (print_time)
        display_action("Dodawanie zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}

// Agent between UI and data structure remove_front
int remove_front(DynamicArray<film_struct> *dynarray)
{
    Timer timer;
    timer.start();
    dynarray->remove_front();
    timer.stop();
    if (print_time)
        display_action("Usuwanie zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}

// Agent between UI and data structure remove_back
int remove_back(DynamicArray<film_struct> *dynarray)
{
    Timer timer;
    timer.start();
    dynarray->remove_back();
    timer.stop();
    if (print_time)
        display_action("Usuwanie zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}

// Agent between UI and data structure remove_at
int remove_at(DynamicArray<film_struct> *dynarray)
{
    unsigned int pos = user_input_action("Podaj pozycję na której chcesz usunąć film: ");
    Timer timer;
    timer.start();
    dynarray->remove_at(pos);
    timer.stop();
    if (print_time)
        display_action("Usuwanie zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}

// Agent between UI and data structure clear
int clear_dt(DynamicArray<film_struct> *dynarray)
{
    Timer timer;
    timer.start();
    dynarray->clear();
    timer.stop();
    if (print_time)
        display_action("Czyszczenie zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}

// Agent between UI and data structure first_value
int first_value(DynamicArray<film_struct> *dynarray)
{
    Timer timer;
    timer.start();
    film_struct val = dynarray->first_value();
    timer.stop();
    if (print_time)
        display_action2(val.title, "Pobranie wartości zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    else
        display_action(val.title);
    return 0;
}

// Agent between UI and data structure last_value
int last_value(DynamicArray<film_struct> *dynarray)
{
    Timer timer;
    timer.start();
    film_struct val = dynarray->last_value();
    timer.stop();
    if (print_time)
        display_action2(val.title, "Pobranie wartości zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    else
        display_action(val.title);
    return 0;
}

// Agent between UI and data structure value_at
int value_at(DynamicArray<film_struct> *dynarray)
{
    unsigned int pos = user_input_action("Podaj pozycję z której chcesz wyświetlić film: ");
    Timer timer;
    timer.start();
    film_struct val = dynarray->value_at(pos);
    timer.stop();
    if (print_time)
        display_action2(val.title, "Pobranie wartości zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    else
        display_action(val.title);
    return 0;
}

// Agent between UI and data structure get_size
int get_size(DynamicArray<film_struct> *dynarray)
{
    Timer timer;
    timer.start();
    int val = dynarray->get_size();
    timer.stop();
    if (print_time)
        display_action2(std::to_string(val), "Pobranie rozmiaru zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    else
        display_action(std::to_string(val));
    return 0;
}

// Agent between UI and data structure find
int find_by_title(DynamicArray<film_struct> *dynarray)
{
    std::string value_to_find = user_input_action_string("Podaj tytuł filmu: ");
    Timer timer;
    timer.start();
    bool found = false;
    unsigned int val = 0;
    for (unsigned int i=0; i<dynarray->get_size(); i++)
    {
        if (value_to_find == (*dynarray)[i].title)
        {
            val = i;
            found = true;
            break;
        }
    }
    timer.stop();
    if (!found)
    {
        if (print_time)
            display_action2("Nie znaleziono!", "Znalezienie indeksu zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
        else
            display_action("Nie znaleziono!");
        return 0;
    }
    if (print_time)
        display_action2(std::to_string(val), "Znalezienie indeksu zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    else
        display_action(std::to_string(val));
    return 0;
}

int find_and_delete_empty_ranks(DynamicArray<film_struct> *dynarray)
{
    int value_to_find = -1;
    Timer timer;
    timer.start();
    for (unsigned int i=0; i<dynarray->get_size(); i++)
    {
        if (value_to_find == (*dynarray)[i].rank)
        {
            dynarray->remove_at(i);
            i--;
        }
    }
    timer.stop();
    if (print_time)
        display_action("Przeszukanie i usunięcie zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    return 0;
}

// Perform sort on dynamic array
int sort(DynamicArray<film_struct> *dynarray)
{
    std::string sort_type = user_input_action_string("Podaj nazwę algorytmu sortującego (merge, quick, bucket): ");
    if (sort_type == "quick")
    {
        quick_sort(dynarray, 0, dynarray->get_size()-1);
    }
    else if (sort_type == "bucket")
    {
        bucket_sort(dynarray, dynarray->get_size());
    }
    else if (sort_type == "merge")
    {
        merge_sort(dynarray, 0, dynarray->get_size()-1);
    }
    return 0;
}

// Check if sorted
int check_sort(DynamicArray<film_struct> *dynarray)
{
    bool ascending = user_input_action("Wybierz czy sprawdzać czy elementy rosną - wpisz 1, maleją - wpisz 0: ");
    for(int i=1; i<dynarray->get_size(); i++)
    {
        if(ascending)
        {
            if ((*dynarray)[i-1].rank > (*dynarray)[i].rank)
            {
                display_action("Tablica dynamiczna filmów nie jest posortowana!");
                return 0;
            }
        }
        else
        {
            if ((*dynarray)[i-1].rank < (*dynarray)[i].rank)
            {
                display_action("Tablica dynamiczna filmów nie jest posortowana!");
                return 0;
            }
        }
    }
    display_action("Tablica dynamiczna filmów jest poprawnie posortowana!");

    return 0;
}

// Agent between UI and data structure get_as_string
int get_as_string(DynamicArray<film_struct> *dynarray)
{
    display_action(dynarray->get_as_string());
    return 0;
}

// Agent bewteen UI and data structure change_at
int change_at(DynamicArray<film_struct> *dynarray)
{
    unsigned int position_to_change = user_input_action("Podaj indeks na którym chcesz coś zmienić: ");
    film_struct new_value = user_input_action_film();

    Timer timer;
    timer.start();
    dynarray->change_at(new_value, position_to_change);
    timer.stop();

    if (print_time)
        display_action("Wypełnienie danymi z pliku zajęło " + std::to_string(timer.elapsedSeconds()) + "ns");
    
    return 0;
}

int fill_from_file(DynamicArray<film_struct> *dynarray)
{
    //std::string file_path = user_input_action_string("Podaj ścieżkę względną do pliku: ");
    std::ifstream file("data.csv");
    unsigned int data_number = user_input_action("Podaj ile danych chcesz wczytać: ");
    std::string line;
    std::getline(file, line);
    if (!file)
    {
        display_action("Błąd otwierania pliku!");
        return 0;
    }

    for(unsigned int i=0; i<data_number; i++)
    {
        if (!std::getline(file, line))
        {
            break;
        }
        // Skip empty lines
        if (line.empty())
        {
            continue;
        }

        std::stringstream ss(line);
        std::string field;
        film_struct film;

        // Read number
        std::getline(ss, field, ',');

        // Read title
        std::getline(ss, field, ',');
        std::string temp;
        if (field.front() == '"' and field.back() != '"')
        {
            temp = field;
            while (std::getline(ss, field, ','))
            {
                temp += "," + field;
                if (field.back() == '"')
                    break;
            }
            film.title = temp;
        }
        else
        {
            film.title = field;
        }

        // Read rank
        std::getline(ss, field, '\n');
        if (field != "")
            film.rank = std::stof(field);
        else
            film.rank = -1;

        dynarray->add_back(film);
    }
    return 0;
}
