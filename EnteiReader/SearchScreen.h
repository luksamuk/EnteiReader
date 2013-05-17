#ifndef SEARCHSCREEN_H_INCLUDED
#define SEARCHSCREEN_H_INCLUDED

#include "stdafx.h"

#define SEARCHSCREEN_SIZE_Y      12
#define SEARCHSCREEN_SIZE_X      20
#define SEARCHSCREEN_UTILSIZE_Y   8
#define SEARCHSCREEN_UTILSIZE_X  18

class SearchScreen
{
private:
    WINDOW* win;
    int ch;
    int selection;
    int ordertype;
    char findtext[255];
    char displaytext[15];
public:
    SearchScreen();
    void init();
    void update();
    void refresh();
};

#endif // SEARCHSCREEN_H_INCLUDED
