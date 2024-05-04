#include <curses.h>
#include <iostream>
#include "ui.h"
#include "ui_actions.h"
#include "data_structures/dynamic_array.h"
#include "data_structures/graph.hpp"
#include "data_structures/filmdata_structures_manual_ui.hpp"
#include "data_structures/graph_ui_manual.hpp"
#include "test.h"
#include "data_structures/film_struct.h"


// Create and return menu for graph on neighbor list
MenuGraph<std::string>* setup_graph1_menu()
{
    MenuGraph<std::string> *graph1_menu = new MenuGraph<std::string>(new DirectedWeightedGraph<std::string>);
    graph1_menu->add_item_dt("|Dodaj wierzchołek", add_vertex);
    graph1_menu->add_item_dt("|Dodaj krawędź", add_edge);
    graph1_menu->add_item_dt("|Usuń wierzchołek", remove_vertex);
    graph1_menu->add_item_dt("|Usuń krawędź", remove_edge);
    graph1_menu->add_item("|Wyświetlanie czasu", change_print_flag);
    graph1_menu->add_item("|Wróć do wyboru struktur", exit_action);

    return graph1_menu;
}

// Create and return menu for dynarray
MenuDt<film_struct>* setup_dynarray_menu()
{
    MenuDt<film_struct> *dynarray_menu = new MenuDt<film_struct>(new DynamicArray<film_struct>);
    dynarray_menu->add_item_dt("|Dodaj z przodu", add_front);
    dynarray_menu->add_item_dt("|Dodaj z tyłu", add_back);
    dynarray_menu->add_item_dt("|Dodaj na wybranej pozycji", add_at);
    dynarray_menu->add_item_dt("|Usuń z przodu", remove_front);
    dynarray_menu->add_item_dt("|Usuń z tyłu", remove_back);
    dynarray_menu->add_item_dt("|Usuń na pozycji", remove_at);
    dynarray_menu->add_item_dt("|Wyczyść", clear_dt);
    dynarray_menu->add_item_dt("|Znajdź film", find_by_title);
    dynarray_menu->add_item_dt("|Znajdź i usuń puste rankingi", find_and_delete_empty_ranks);
    dynarray_menu->add_item_dt("|Zmień wartość na danym polu", change_at);
    //dynarray_menu->add_item_dt("|Wypisz rozmiar", get_size);
    dynarray_menu->add_item_dt("|Sortuj", sort);
    dynarray_menu->add_item_dt("|Sprawdź czy posortowano", check_sort);
    dynarray_menu->add_item_dt("|Wczytaj z pliku", fill_from_file);
    dynarray_menu->add_item("|Wyświetlanie czasu", change_print_flag);
    dynarray_menu->add_item("|Wróć do wyboru struktur", exit_action);

    return dynarray_menu;
}

// Create and return main menu, also setup values of curses library
Menu* set_up_ui()
{
    // Init PDCurses and wait for user before jumping to main menu
    initscr();
    setlocale(LC_ALL, "pl_PL.utf8");
    keypad(stdscr, TRUE);
    noecho();
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    curs_set(0);
    printw( "Wcisnij dowolny przycisk, aby rozpoczac prace..\n" );
    curs_set(0);
    getch();

    // Create main_menu and add menu items to it
    Menu *main_menu = new Menu();
    main_menu->add_item("|Wykonaj automatyczne testy", run_tests);
    main_menu->add_item("|Praca z listą filmów", setup_dynarray_menu());
    main_menu->add_item("|Praca z grafem", setup_graph1_menu());
    main_menu->add_item("|Pomoc", display_help);
    main_menu->add_item("|Exit", exit_action);
    
    return main_menu;
}


int main()
{
    Menu *main_menu = set_up_ui();
    main_menu->display_menu();
    endwin();

    delete main_menu;

    return 0;
}
