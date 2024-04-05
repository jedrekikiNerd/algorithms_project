#include "Timer.h"
#include "data_structures/I_data_structure.h"
#include "data_structures/list.h"
#include "data_structures/list_ht.h"
#include "data_structures/list_double.h"
#include "data_structures/dynamic_array.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include "ui_actions.h"
#include <iomanip>

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
    std::ofstream file("test_results/"+filePath, std::ios_base::app); 
    if (file.is_open()) {
        file << line << std::endl; 
        file.close(); 
    } else {
        std::cerr << "Failed to open file: " << filePath << std::endl;
    }
    return 0;
}

// Function that runs all tests for all data structures and saves time series to files
// We use buffer (stream) because thats the solution we found to output more than 4 digits after decimal point to string
int run_all_tests_for_dt(IDataStructure<int>* dt)
{
    return 0;
}

// Function that loads data from file to each data structure and runs all tests for it
int run_tests()
{
    return 0;
}
