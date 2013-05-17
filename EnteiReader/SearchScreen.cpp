#include "SearchScreen.h"

SearchScreen::SearchScreen()
{
    selection = 0;
    ordertype = 0;
    strcpy(findtext, "");
    win = newwin(SEARCHSCREEN_SIZE_Y, SEARCHSCREEN_SIZE_X, 6, 29);
}

void SearchScreen::init()
{
    // Cabe�alho
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "Pesquisa & Ordem");
    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, ACS_HLINE, SEARCHSCREEN_SIZE_X - 1);
    mvwaddch(win, 2, SEARCHSCREEN_SIZE_X - 1, ACS_RTEE);

    // �tens
    mvwprintw(win, 3, 1, "[ ] Por nome");
    mvwprintw(win, 4, 1, "[ ] Por autor");
    mvwprintw(win, 5, 1, "[ ] Por editora");

    mvwprintw(win, 7, 1, "Pesquisar por:");

    // Caixa de pesquisa
    mvwaddch(win, 8, 1, ACS_ULCORNER);
    mvwhline(win, 8, 2, ACS_HLINE, 16);
    mvwaddch(win, 8, 18, ACS_URCORNER);
    mvwaddch(win, 9, 1, ACS_VLINE);
    mvwaddch(win, 9, 18, ACS_VLINE);
    mvwaddch(win, 10, 1, ACS_LLCORNER);
    mvwhline(win, 10, 2, ACS_HLINE, 16);
    mvwaddch(win, 10, 18, ACS_LRCORNER);
}

void SearchScreen::refresh()
{
    // Tipos de ordem
    switch(ordertype)
    {
    case 0:   // Por nome
        mvwaddch(win, 3, 2, 'x');
        mvwaddch(win, 4, 2, ' ');
        mvwaddch(win, 5, 2, ' ');
        break;
    case 1:   // Por autor
        mvwaddch(win, 3, 2, ' ');
        mvwaddch(win, 4, 2, 'x');
        mvwaddch(win, 5, 2, ' ');
        break;
    case 2:   // Por editora
        mvwaddch(win, 3, 2, ' ');
        mvwaddch(win, 4, 2, ' ');
        mvwaddch(win, 5, 2, 'x');
        break;
    }

    if(win) wrefresh(win);
}

void SearchScreen::update()
{
    this->refresh();
    while(true)
    {
        ch = wgetch(stdscr);

        switch(ch)
        {
        case KEY_F(2):
            ordertype = 0;
            this->refresh();
            break;
        case KEY_F(3):
            ordertype = 1;
            this->refresh();
            break;
        case KEY_F(4):
            ordertype = 2;
            this->refresh();
            break;
        case 10:
            return;
        }
    }
}