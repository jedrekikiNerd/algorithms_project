#ifndef UI_ACTIONS_H
#define UI_ACTIONS_H

#include <iostream>
#include "data_structures/film_struct.h"

// Flag for printing time when manual data structure control
extern bool print_time;

/**
 * Returns 1 - this will be handled by menu and interpreted as call for menu exit
 * 
 * @return int 
 */
int exit_action();

/**
 * Returns 3 - this will be handled and interpreted as launching new menu, not used anymore
 * 
 * @return int 
 */
int launch_menu_action();

/**
 * Returns user input as int number
 * 
 * @return int 
 */
int user_input_action(std::string input_field_label);

/**
 * Returns user input as float number
 * 
 * @return float 
 */
float user_input_action_float(std::string input_field_label);

/**
 * Returns user input as string
 * 
 * @return string 
 */
std::string user_input_action_string(std::string input_field_label);

/**
 * Returns user input as film data struct
 * 
 * @return film_struct 
 */
film_struct user_input_action_film();

/**
 * Displays given string
 * 
 * @return int 
 */
int display_action(std::string display_label);

/**
 * Displays given string
 * 
 * @return int 
 */
int display_action_special(std::string display_label);

/**
 * Displays 2 given strings
 * 
 * @return int 
 */
int display_action2(std::string display_label, std::string display_label2);

/**
 * Displays help page
 * 
 * @return int 
 */
int display_help();

/**
 * Changes print_time flag
 * 
 * @return int
 */
int change_print_flag();


#endif