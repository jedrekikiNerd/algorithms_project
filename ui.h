#ifndef UI_H
#define UI_H
#include <iostream>
#include <functional>
#include "data_structures/I_data_structure.h"
#include "data_structures/dynamic_array.h"
#include "ui_actions.h"

// Key definitions for handling UI interactions with keyboard
#define MY_KEY_EXIT 27
#define MY_KEY_ENTER 10
#define MY_KEY_SPACE 32

/**
 * Inteface of MenuItems
 * 
 */
class MenuItemInterface {
public:
    std::string label;
    MenuItemInterface() : label("") {} // Constructor when no label
    MenuItemInterface(std::string label) : label(label) {} // Constructor when label specified
    virtual int clicked() = 0; //clicked function that gives our button a functionality
    virtual ~MenuItemInterface() {} // Virtual destructor to make sure all subclasses have their destructors run
};


/**
 * Class that represents menu button that should run given function. It stores label and function that should be called when element is selected and ENTER key is pressed.
 * 
 */
class MenuItemFunction : public MenuItemInterface
{
    private:
        // Function we call when element is clicked (ptr to it)
        std::function<int()> func;

    public:

        MenuItemFunction();
        MenuItemFunction(std::string label, std::function<int()> func);
        int clicked();
};


/**
 * Class that represents menu button that should run given function and pass pointer to IDataStructure as argument. It stores label and function that should be called when element is selected and ENTER key is pressed.
 * 
 */
template <typename Type>
class MenuItemDTFunction : public MenuItemInterface
{
    private:
        // Function we call when element is clicked (ptr to it)
        std::function<int(DynamicArray<Type>*)> func;
        DynamicArray<Type> *dt;

    public:

        MenuItemDTFunction() : MenuItemInterface(), func(nullptr), dt(nullptr){}
        MenuItemDTFunction(std::string label, std::function<int(DynamicArray<Type>*)> func, DynamicArray<Type> *dt) : MenuItemInterface(label), func(func), dt(dt){}
        int clicked()
        {
            // Do not call func if it's null!
            if (func != nullptr) 
            {
                int result = func(dt);
                return result;
            }
            // 2 is exit status meaning func is nullptr
            return 2;
        }
};


/**
 * Class representing single menu. It stores items with labels and callable functions and datastructure - look at MenuItem, it also stores which element is selected.
 * 
 **/
class Menu
{
    protected:
        static const int MAX_ITEMS = 15;

    public:
        MenuItemInterface *items[MAX_ITEMS];
        int items_number;
        int selected_option;

        Menu();
        virtual ~Menu() 
        {
            // Delete all buttons this menu stores
            for(int i=0; i<items_number; i++)
            {
                delete items[i];
                items[i] = nullptr;
            }
        }
        void add_item(std::string label, std::function<int()> func);
        void add_item(std::string label, Menu *menu);
        int handle_click();
        virtual int display_menu();
};


/**
 * Class representing menu that interacts with datastructure. It stores data structure of given type and can have menu items that call functions that edit given datastructure
 * 
 **/
template <typename Type>
class MenuDt : public Menu
{
    private:
        DynamicArray<Type> *dt;

    public:
        MenuDt() : Menu(), dt(nullptr){}
        MenuDt(DynamicArray<Type> *ptr_to_dt) : Menu(), dt(ptr_to_dt) {}
        ~MenuDt()
        {
            delete dt;
            dt = nullptr;
        }
        void add_item_dt(std::string label, std::function<int(DynamicArray<Type>*)> func)
        {
            if (items_number<MAX_ITEMS)
            {
                items[items_number] = new MenuItemDTFunction<Type>(label, func, dt);
                items_number++;
            }
            else
            {
                throw "MAX_ITEMS in menu exceeded!";
            } 
        }

        int display_menu()
        {
            int pressed_key;
            bool current_window = true;
            int dt_box_h = LINES/2-(items_number/2)-4;
            int menu_box_h = LINES - dt_box_h;
            // We make some windows inside terminal (bg are for border decoration, dt_window is for structure panel we can scroll and menu_window is for menu)
            WINDOW *dt_window_bg = newwin(dt_box_h , COLS, 0, 0);
            WINDOW *menu_window_bg = newwin(menu_box_h, COLS, LINES/2-(items_number/2)-4, 0);
            // Pad can hold more data then it's size - we use it for scrolling over data structure
            WINDOW *dt_window = newpad(10000, COLS);
            WINDOW *menu_window = newwin(menu_box_h-2, COLS - 2, LINES/2-(items_number/2)-3, 1);
            wattron(menu_window_bg, COLOR_PAIR(5));
            wattroff(dt_window_bg, COLOR_PAIR(5));
            box(menu_window_bg, 0, 0);
            box(dt_window_bg, 0, 0);
            clear();
            int scroll_pos = 0;
            while (true)
            {
                // We clear terminal screen before printing new elements
                // If current selected window is data structure we dont want to clear it
                if (current_window)
                    wclear(dt_window);
                wclear(menu_window);

                // We iterate through every element and display it
                for (int item_index=0; item_index<items_number; item_index++)
                {
                    // Add background to selected option
                    if (item_index == selected_option)
                        wattron(menu_window, A_REVERSE);
                    mvwprintw(menu_window, item_index+5, COLS/2-10, items[item_index]->label.c_str());
                    wattroff(menu_window, A_REVERSE);
                }

                // Here some text in left upper corner and on the last line authors and title with attributes such as bold, colors etc.
                wattron(menu_window, A_ITALIC);
                wattron(menu_window, COLOR_PAIR(2));
                mvwprintw(menu_window, menu_box_h-4, COLS/2-13, "Projekt analiza algorytmów");
                mvwprintw(menu_window, menu_box_h-3, COLS/2-11, "by Jędrzej Boruczkowski");
                wattroff(menu_window, A_ITALIC);
                wattroff(menu_window, COLOR_PAIR(2));

                wattron(menu_window, COLOR_PAIR(1));
                mvwprintw(menu_window, 0, 0, "UWAGA: Indeksowanie od 0!");
                wattroff(menu_window, COLOR_PAIR(1));

                mvwprintw(menu_window, 1, 0, "Struktura zajmuje:");
                wattron(menu_window, COLOR_PAIR(3));
                wattron(menu_window, A_BOLD);
                mvwprintw(menu_window, 1, 19, (std::to_string(dt->get_byte_size()) + "B").c_str());
                wattroff(menu_window, COLOR_PAIR(3));
                wattroff(menu_window, A_BOLD);

                // Informtaion about print_time flag
                mvwprintw(menu_window, 2, 0, "Wypisywanie czasu operacji:");
                if(print_time)
                {
                    wattron(menu_window, COLOR_PAIR(2));
                    mvwprintw(menu_window, 2, 28, "AKTYWNE");
                    wattroff(menu_window, COLOR_PAIR(2));
                }
                else
                {
                    wattron(menu_window, COLOR_PAIR(1));
                    mvwprintw(menu_window, 2, 28, "NIEAKTYWNE");
                    wattroff(menu_window, COLOR_PAIR(1));                    
                }

                if (current_window)
                {
                    waddstr(dt_window, dt->get_as_string().c_str());
                }
                // refresh displays new elements
                refresh();
                wrefresh(menu_window_bg);
                wrefresh(menu_window);
                wrefresh(dt_window_bg);
                // inside pefresh we specify size, position inside terminal window and offset of data that are inside this panel (scroll_pos in y axis)
                prefresh(dt_window, scroll_pos, 0, 1, 1, dt_box_h - 2, COLS - 2);  

                // Wait for input and handle it
                pressed_key = getch();
                switch(pressed_key)
                {
                    case KEY_DOWN:
                    {
                        // We check if we are in menu and need to switch between buttons or if we are in data structure and want to scroll
                        if (current_window)
                            selected_option = (selected_option + 1) % items_number;
                        else
                            if (scroll_pos < 10000)
                                scroll_pos += 1;
                        break;
                    }
                    case KEY_UP:
                    {
                        // We check if we are in menu and need to switch between buttons or if we are in data structure and want to scroll
                        if (current_window)
                        {
                            selected_option = (selected_option - 1);
                            if (selected_option < 0)
                                selected_option = items_number - 1;
                        }
                        else
                            if (scroll_pos != 0)
                                scroll_pos -= 1;
                        break;
                    }
                    case MY_KEY_ENTER:
                    {
                        int result = handle_click();
                        if (result == 1)
                        {
                            // Before exiting free all windows from memory
                            delwin(dt_window);
                            delwin(dt_window_bg);
                            delwin(menu_window_bg);
                            delwin(menu_window);
                            return 1;
                        }
                        box(menu_window_bg, 0, 0);
                        box(dt_window_bg, 0, 0);
                        break;
                    }
                    case MY_KEY_EXIT:
                    {
                        // Before exiting free all windows from memory
                        delwin(dt_window);
                        delwin(dt_window_bg);
                        delwin(menu_window_bg);
                        delwin(menu_window);
                        return 1;
                    }
                    case MY_KEY_SPACE:
                    {
                        // Change current active area on terminal (data structure panel or menu)
                        current_window = !current_window;
                        wclear(dt_window_bg);
                        wclear(menu_window_bg);
                        wclear(dt_window);
                        if (!current_window)
                        {
                            wattron(dt_window_bg,COLOR_PAIR(5));
                            wattroff(menu_window_bg,COLOR_PAIR(5));
                        }
                        else
                        {
                            wattron(menu_window_bg, COLOR_PAIR(5));
                            wattroff(dt_window_bg, COLOR_PAIR(5));
                        }

                        box(menu_window_bg, 0, 0);
                        box(dt_window_bg, 0, 0);
                        waddstr(dt_window, dt->get_as_string().c_str());
                        break;
                    }
                }
            }
            return 0;
        }
};


/**
 * Class that represents menu button that should run other menu. It stores label and menu that should be activated when selected.
 * 
 */
class MenuItemSwitcher : public MenuItemInterface
{
    private:
        Menu *menu_to_switch;

    public:
        MenuItemSwitcher();
        MenuItemSwitcher(std::string label, Menu *menu);
        int clicked();
        // We have to delete menu it stores
        ~MenuItemSwitcher()
        {
            delete menu_to_switch;
            menu_to_switch = nullptr;
        }
};

#endif