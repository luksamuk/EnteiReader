#ifndef LIBSCREEN_H_INCLUDED
#define LIBSCREEN_H_INCLUDED

#include "stdafx.h"

class LibScreen
{
private:
    //Vigil* vigil;
    WINDOW* win;
    int selection, n_books;
    int ch;
    char** books;

    void printMenuElement(const char*, int);
public:
    LibScreen();
    void init();
    int update();
    void refresh();
};

#endif // LIBSCREEN_H_INCLUDED
