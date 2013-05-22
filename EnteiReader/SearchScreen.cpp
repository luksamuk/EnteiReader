#include "SearchScreen.h"

SearchScreen::SearchScreen()
{
    charnum = 0;
    selection = 0;
    ordertype = 0;
    strcpy(findtext, "");
    win = newwin(SEARCHSCREEN_SIZE_Y, SEARCHSCREEN_SIZE_X,
                 12 - (SEARCHSCREEN_SIZE_Y / 2),
                 40 - (SEARCHSCREEN_SIZE_X / 2));
}

void SearchScreen::init()
{
    // Cabeçalho
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "Pesquisa & Ordem");
    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, ACS_HLINE, SEARCHSCREEN_SIZE_X - 1);
    mvwaddch(win, 2, SEARCHSCREEN_SIZE_X - 1, ACS_RTEE);

    // Ítens
    mvwprintw(win, 3, 1, "[ ] Por nome");
    mvwprintw(win, 4, 1, "[ ] Por autor");
    mvwprintw(win, 5, 1, "[ ] Por editora");

    mvwprintw(win, 7, 1, "Pesquisar por:");

    // Caixa de pesquisa
    mvwaddch(win, 8, 1, ACS_ULCORNER);
    mvwhline(win, 8, 2, ACS_HLINE, 26);
    mvwaddch(win, 8, 28, ACS_URCORNER);
    mvwaddch(win, 9, 1, ACS_VLINE);
    mvwaddch(win, 9, 28, ACS_VLINE);
    mvwaddch(win, 10, 1, ACS_LLCORNER);
    mvwhline(win, 10, 2, ACS_HLINE, 26);
    mvwaddch(win, 10, 28, ACS_LRCORNER);
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

    // Texto de pesquisa
    mvwprintw(win, 9, 2, displaytext);

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
        case 8:
        case KEY_BACKSPACE:
            if(charnum > 0)
            {
                findtext[charnum - 1] = '\0';
                charnum--;
            }
            cleardisplaytext();
            refreshdisplaytext();
            this->refresh();
            break;

        case '\t': // Apenas para evitar input errado.
        case KEY_F(1):
        break;

        default:
            if(charnum < 255)
            {
                findtext[charnum] = ch;
                charnum++;
            }
            cleardisplaytext();
            refreshdisplaytext();
            this->refresh();
            break;
        case 10:
            return;
        }
    }
}

void SearchScreen::refreshdisplaytext()
{
    if(strlen(findtext) < 25)
        strcpy(displaytext, findtext);
    else
    {
        for(int i = 25; i > 0; i--)
            displaytext[i] = findtext[i + (strlen(findtext) - 25)];
        displaytext[0] = '<';
    }
}

void SearchScreen::cleardisplaytext()
{
    for(int i = 2; i <= 26; i++)
        mvwprintw(win, 9, i, " ");
}
