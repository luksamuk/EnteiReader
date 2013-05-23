#include "ManageScreen.h"

ManageScreen::ManageScreen()
{
    win = newwin(20, 60, 1, 10);
}

ManageScreen::~ManageScreen()
{
}

void ManageScreen::init()
{
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "Gerenciador");
    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, ACS_HLINE, 58);
    mvwaddch(win, 2, 59, ACS_RTEE);
}

void ManageScreen::update()
{
    while(true)
    {
        ch = wgetch(stdscr);

        switch(ch)
        {
        case 10:
            vigil->update();
            (*clearscr)();
            return;
            break;
        }
    }
}

void ManageScreen::refresh()
{
    if(win) wrefresh(win);
}
