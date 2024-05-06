/**
 * @file ui_actions.cpp
 * @author Jędrzej Boruczkowski
 * @brief 
 * @version 0.1
 * @date 2024-03-09
 * 
 * @copyright Copyright (c) 2024
 * 
 * This file stores all functions/actions that can be assigned to menu items from ui.cpp file.
 * All return codes meaning:
 * 0 - success, no custom action
 * 1 - exit menu
 * 2 - nullptr, no action assigned to menu item
 * 3 - start new menu
 * 4 - input error
 */

#include "ui_actions.h"
#include <curses.h>
#include <iostream>

bool print_time = false;

/**
 * Returns 1 - this will be handled by menu and interpreted as call for menu exit
 * 
 * @return int 
 */
int exit_action()
{
    return 1;
}


/**
 * Returns 3 - this will be handled and interpreted as launching new menu, not used anymore
 * 
 * @return int 
 */
int launch_menu_action()
{
    return 3;
}


/**
 * Returns user input as int number
 * 
 * @return int 
 */
int user_input_action(std::string input_field_label)
{
    echo();
    clear();
    int user_input;
    mvprintw(1, 1, input_field_label.c_str());
    refresh();
    // Get int number provided by user from terminal window with scanw
    scanw("%d", &user_input);
    noecho();
    return user_input;
}

/**
 * Returns user input as float number
 * 
 * @return float 
 */
float user_input_action_float(std::string input_field_label)
{
    echo();
    clear();
    float user_input;
    mvprintw(1, 1, input_field_label.c_str());
    refresh();
    // Get float number provided by user from terminal window with scanw
    scanw("%f", &user_input);
    noecho();
    return user_input;
}

/**
 * Returns user input as string
 * 
 * @return string 
 */
std::string user_input_action_string(std::string input_field_label)
{
    echo();
    clear();
    // We use buffer char array cause scanw didn't want to work with std::string
    char buffer[256];
    mvprintw(1, 1, input_field_label.c_str());
    refresh();
    // Get string provided by user from terminal window with scanw (^\n means we do this until \n is next character and 255 means max input characters)
    scanw("%255[^\n]", buffer);
    // We dump our char buffer into new string just created
    std::string user_input(buffer);
    noecho();
    return user_input;
}

/**
 * Displays given string
 * 
 * @return int 
 */
int display_action(std::string display_label)
{
    clear();
    mvprintw(1, 1, display_label.c_str());
    mvprintw(2, 1, "Kliknij dowolny przycisk, aby kontynuować...");
    refresh();
    getch();
    return 0;
}

/**
 * Displays given string
 * 
 * @return int 
 */
int display_action_special(std::string display_label)
{
    clear();
    addstr(display_label.c_str());
    refresh();
    getch();
    return 0;
}

/**
 * Displays 2 given strings
 * 
 * @return int 
 */
int display_action2(std::string display_label, std::string display_label2)
{
    clear();
    mvprintw(1, 1, display_label.c_str());
    mvprintw(2, 1, display_label2.c_str());
    mvprintw(3, 1, "Kliknij dowolny przycisk, aby kontynuować...");
    refresh();
    getch();
    return 0;
}

/**
 * Changes print flag
 * 
 * @return int
 */
int change_print_flag()
{
    print_time = !print_time;
    return 0;
}

/**
 * Returns user input as film data struct
 * 
 * @return film_struct 
 */
film_struct user_input_action_film()
{
    film_struct new_film;
    new_film.title = user_input_action_string("Podaj tytuł filmu: ");
    new_film.rank = user_input_action_float("Podaj ranking filmu: ");
    return new_film;
}

/**
 * Displays help page
 * 
 * @return int 
 */
int display_help()
{
    clear();
    attron(COLOR_PAIR(3));
    attron(A_BOLD);
    mvprintw(1, 1, "Witaj w sekcji tłumaczącej jak nawigować po menu!");
    attroff(A_BOLD);
    attroff(COLOR_PAIR(3));

    attron(A_BOLD);
    attron(COLOR_PAIR(5));
    mvprintw(3, 1, "|Nawigacja/sterowanie|");
    mvprintw(9, 1, "|Aktywny obszar|");
    mvprintw(15, 1, "|Przyciski i dodatkowe ekrany|");
    mvprintw(20, 1, "|Struktury danych|");
    attroff(A_BOLD);
    attroff(COLOR_PAIR(5));

    mvprintw(4, 1, ">Pomiędzy przyciskami menu przełączaj korzystając ze strzałek GÓRA i DÓŁ");
    mvprintw(5, 1, ">ESC pełni funkcję wyjścia z menu lub z całego programu");
    mvprintw(6, 1, ">Wciskając ENTER możesz potwierdzić wybrany przycisk w menu lub zatwierdzić wpisaną przez siebie wartość");
    mvprintw(7, 1, ">Aby przełączyć pomiędzy aktywnym obszarem wciśnij klawisz SPACJI");


    mvprintw(10, 1, ">Będąc w trybie ręcznej edycji struktur danych możesz zauważyć dwie ramki w konsoli");
    mvprintw(11, 1, ">Żółta ramka oznacza obecnie aktywny obszar wewnątrz którego działają twoje klawisze strzałek z klawiatury");
    mvprintw(12, 1, ">Kiedy aktywny jest panel menu możesz wybierać przyciski menu strzałkami");
    mvprintw(13, 1, ">Kiedy aktywny jest panel struktur danych możesz przewijać góra-dół zawartość tego panelu");

    mvprintw(16, 1, ">Niektóre przyciski otwierają ekran wyświetlający coś i czekający na wciśnięcie dowolnego klawisza");
    mvprintw(17, 1, ">Niektóre przyciski otwierają pole które oczekuje na wpisanie ciągu znaków i potwierdzenie ENTEREM");
    mvprintw(18, 1, ">Niektóre przyciski przenoszą do nowego menu");

    mvprintw(21, 1, ">Menu do sterowania strukturą danych przechowuje strukturę");
    mvprintw(22, 1, ">Struktury danych nie dodadzą nowej wartości jeżeli podany indeks wykracza poza ich zakres");
    mvprintw(23, 1, ">Struktury danych wypiszą -1 jeżeli nie zostanie znaleziona szukana w nich wartość");
    mvprintw(24, 1, ">Struktury danych są indeksowane od 0");

    attron(COLOR_PAIR(1));
    mvprintw(26, 1, "Kliknij dowolny przycisk, aby wyjść...");
    attroff(COLOR_PAIR(1));

    refresh();
    getch();
    return 0;
}