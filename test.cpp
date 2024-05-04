#include "Timer.h"
#include "data_structures/I_data_structure.h"
#include "data_structures/list_double.hpp"
#include "data_structures/dynamic_array.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include "ui_actions.h"
#include <iomanip>
#include "algorithms/sorts.hpp"

namespace fs = std::filesystem;
Timer timer;

//clearing the folder before adding files
void removeFilesInFolder2(const std::string folderName) {
    if (!fs::exists(folderName)) {
        fs::create_directories(folderName);
    }

    for (const auto& entry : fs::directory_iterator(folderName)) {
        if (entry.is_regular_file()) {
            fs::remove(entry.path());
        }
    }
}

// Adds one line to given file
int add_line_to_file(std::string line, std::string filePath)
{
    std::ofstream file("tests_results/"+filePath, std::ios_base::app); 
    if (file.is_open()) {
        file << line << std::endl; 
        file.close(); 
    } else {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
    return 0;
}

double find_and_delete_empty_ranks_test(DynamicArray<film_struct> *dynarray)
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
    return timer.elapsedSeconds();
}



// Check if sorted
int check_sort_test(DynamicArray<film_struct> *dynarray)
{
    for(int i=1; i<dynarray->get_size(); i++)
    {
        if ((*dynarray)[i-1].rank > (*dynarray)[i].rank)
        {
            return 1;
        }
    }
    return 0;
}

// Average rank value
float calc_average(DynamicArray<film_struct> *dynarray)
{
    float sum = 0;
    for(int i=0; i<dynarray->get_size(); i++)
    {
        sum += (*dynarray)[i].rank;
    }
    return sum/(dynarray->get_size());
}

// Center rank value
float calc_center(DynamicArray<film_struct> *dynarray)
{
    if (dynarray->get_size() % 2 == 1)
        return (*dynarray)[dynarray->get_size()/2].rank;
    else
        return ((*dynarray)[dynarray->get_size()/2].rank + (*dynarray)[(dynarray->get_size()/2)-1].rank)/2;
}

// Check if sorted
double sort_test(DynamicArray<film_struct> *dynarray, std::string sort_name)
{
    Timer timer;
    if (sort_name == "quick")
    {
        timer.start();
        quick_sort(dynarray, 0, dynarray->get_size()-1);
    }
    else if (sort_name == "merge")
    {
        timer.start();
        merge_sort(dynarray, 0, dynarray->get_size()-1);
    }
    else if (sort_name == "bucket")
    {
        timer.start();
        bucket_sort(dynarray, dynarray->get_size());
    }
    timer.stop();
    
    return timer.elapsedSeconds();;
}

int fill_from_file(DynamicArray<film_struct> *dynarray, unsigned int data_number)
{
    //std::string file_path = user_input_action_string("Podaj ścieżkę względną do pliku: ");
    std::ifstream file("data.csv");
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

// Function that runs all tests and saves time series to files
// We use buffer (stream) because thats the solution we found to output more than 4 digits after decimal point to string
int run_all_tests_for_data_structure(DynamicArray<film_struct>* dynarray, int data_size, int data_sample_number, int series_number)
{
    std::ostringstream stream;
    std::string measure_line;
    std::string data_sample = std::to_string(data_sample_number);
    std::string repetition = std::to_string(series_number);
    double temp_double_buffor;

    // Find and delete empty ranks test
    temp_double_buffor = find_and_delete_empty_ranks_test(dynarray);
    stream << std::fixed << std::setprecision(10) << temp_double_buffor;
    measure_line = std::to_string(data_size) + ";" + stream.str();
    add_line_to_file(measure_line, ("find_and_delete_empty" + data_sample + "_" + repetition + ".txt"));
    stream.str("");

    DynamicArray<film_struct> dynarray1;
    DynamicArray<film_struct> dynarray2;
    DynamicArray<film_struct> dynarray3;

    for (int i=0; i<dynarray->get_size(); i++)
    {
        dynarray1.add_back((*dynarray)[i]);
        dynarray2.add_back((*dynarray)[i]);
        dynarray3.add_back((*dynarray)[i]);
    }

    // Quick sort test
    temp_double_buffor = sort_test(&dynarray1, "quick");
    stream << std::fixed << std::setprecision(10) << temp_double_buffor;
    measure_line = std::to_string(dynarray1.get_size()) + ";" + stream.str();
    add_line_to_file(measure_line, ("quicksort" + data_sample + "_" + repetition + ".txt"));
    stream.str("");

    if(check_sort_test(&dynarray1) == 1)
        display_action("QUICK SORT SIĘ NIE POWIÓDŁ!");

    // Merge sort test
    temp_double_buffor = sort_test(&dynarray2, "merge");
    stream << std::fixed << std::setprecision(10) << temp_double_buffor;
    measure_line = std::to_string(dynarray2.get_size()) + ";" + stream.str();
    add_line_to_file(measure_line, ("mergesort" + data_sample + "_" + repetition + ".txt"));
    stream.str("");

    if(check_sort_test(&dynarray2) == 1)
        display_action("MERGE SORT SIĘ NIE POWIÓDŁ!");

    // Bucket sort test
    temp_double_buffor = sort_test(&dynarray3, "bucket");
    stream << std::fixed << std::setprecision(10) << temp_double_buffor;
    measure_line = std::to_string(dynarray3.get_size()) + ";" + stream.str();
    add_line_to_file(measure_line, ("bucketsort" + data_sample + "_" + repetition + ".txt"));
    stream.str("");

    if(check_sort_test(&dynarray3) == 1)
        display_action("BUCKET SORT SIĘ NIE POWIÓDŁ!");

    // center value
    temp_double_buffor = calc_center(&dynarray1);
    stream << std::fixed << std::setprecision(10) << temp_double_buffor;
    measure_line = std::to_string(dynarray1.get_size()) + ";" + stream.str();
    add_line_to_file(measure_line, ("center_values" + data_sample + "_" + repetition + ".txt"));
    stream.str("");

    // average value
    temp_double_buffor = calc_average(&dynarray1);
    stream << std::fixed << std::setprecision(10) << temp_double_buffor;
    measure_line = std::to_string(dynarray1.get_size()) + ";" + stream.str();
    add_line_to_file(measure_line, ("average_values" + data_sample + "_" + repetition + ".txt"));
    stream.str("");

    return 0;
}

// Function that loads data from file to each data structure and runs all tests for it
int run_tests()
{
    std::string confirm = user_input_action_string("Wpisz [TAK] jeżeli chcesz kontynuować: ");
    if (confirm != "TAK")
        return 0;
    int measure_points[7] = {5000, 10000, 50000, 100000, 500000, 1000000, 1010295};
    DynamicArray<film_struct> dyn_array;

    removeFilesInFolder2("tests_results");

    int repetition = user_input_action("Podaj ile razy powtarzać pomiar: ");

    for(int i=0; i<4; i++)
    {
        for(int j=1; j<=repetition; j++)
        {
            fill_from_file(&dyn_array, measure_points[i]);
            run_all_tests_for_data_structure(&dyn_array, measure_points[i], 1, j);
            dyn_array.clear();
        }
    }
    return 0;
}
