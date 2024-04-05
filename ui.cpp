/**
 * @file ui.cpp
 * @author Jędrzej Boruczkowski
 * @brief Code in this file is responsible for user interface (we use curses.h)
 * @version 0.1
 * @date 2024-03-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

// TODO: add destructors with delete!

#include <curses.h>
#include <iostream>
#include <string>
#include <functional>
#include "ui_actions.h"
#include "ui.h"
#include "data_structures/I_data_structure.h"


// Constructor when label and ptr to function are not specified
MenuItemFunction::MenuItemFunction() : MenuItemInterface(), func(nullptr){}
// Constructor when label and ptr to function are specified
MenuItemFunction::MenuItemFunction(std::string label, std::function<int()> func) : MenuItemInterface(label), func(func){}

// Clicked function - we should run it when someone presses enter while having this item selected
int MenuItemFunction::clicked() 
{
    // Do not call func if it's null!
    if (func != nullptr) 
    {
        int result = func();
        return result;
    }
    // 2 is exit status meaning func is nullptr
    return 2;
}


Menu::Menu() : items_number(0), selected_option(0) {}

// Add element (that runs standalone function) to menu
void Menu::add_item(std::string label, std::function<int()> func) 
{
    if (items_number<MAX_ITEMS)
    {
        items[items_number] = new MenuItemFunction(label, func);
        items_number++;
    }
    else
    {
        throw "MAX_ITEMS in menu exceeded!";
    }
}

// Call clicked function in selected element
int Menu::handle_click()
{
    int result = items[selected_option]->clicked();
    return result;
}

// displays menu and handles input
int Menu::display_menu()
{
    int pressed_key;
    while (true)
    {
        // We clear terminal screen before printing new elements
        clear();
        // We iterate through every element and display it
        for (int item_index=0; item_index<items_number; item_index++)
        {
            // Add background to selected option
            if (item_index == selected_option)
                attron(A_REVERSE);
            mvprintw(item_index+LINES/2-(items_number/2), COLS/2-10, items[item_index]->label.c_str());
            attroff(A_REVERSE);
        }
        attron(A_ITALIC);
        attron(COLOR_PAIR(2));
        mvprintw(LINES-2, COLS/2-13, "Projekt analiza algorytmów");
        mvprintw(LINES-1, COLS/2-11, "by Jędrzej Boruczkowski");
        attroff(A_ITALIC);
        attroff(COLOR_PAIR(2));
        // refresh displays new elements
        refresh();

        // Wait for input and handle it
        pressed_key = getch();
        switch(pressed_key)
        {
            case KEY_DOWN:
            {
                selected_option = (selected_option + 1) % items_number;
                break;
            }
            case KEY_UP:
            {
                selected_option = (selected_option - 1);
                if (selected_option < 0)
                    selected_option = items_number - 1;
                break;
            }
            case MY_KEY_ENTER:
            {
                int result = handle_click();
                if (result == 1)
                    return 1;
                break;
            }
            case MY_KEY_EXIT:
                return 1;
        }
    }
    return 0;
}


// Conctructor for empty object
MenuItemSwitcher::MenuItemSwitcher() : MenuItemInterface(), menu_to_switch(nullptr){}
// Constructor when label and menu object are specified
MenuItemSwitcher::MenuItemSwitcher(std::string label, Menu *menu) : MenuItemInterface(label), menu_to_switch(menu){}

// Clicked function - we should run it when someone presses enter while having this item selected
int MenuItemSwitcher::clicked() 
{
    // Do not call func if it's null!
    if (menu_to_switch != nullptr) 
    {
        menu_to_switch->display_menu();
        return 3;
    }
    // 2 is exit status meaning menu_to_switch is nullptr
    return 2;
}


// Body of add_item Menu method
void Menu::add_item(std::string label, Menu *menu)
{
    if (items_number < MAX_ITEMS)
    {
        items[items_number] = new MenuItemSwitcher(label, menu);
        items_number++;
    }
    else
    {
        throw "MAX_ITEMS in menu exceeded!";
    }
}
