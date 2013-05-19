#ifndef SEARCHSCREEN_H_INCLUDED
#define SEARCHSCREEN_H_INCLUDED

#include "stdafx.h"

#define SEARCHSCREEN_SIZE_Y      12
#define SEARCHSCREEN_SIZE_X      30
#define SEARCHSCREEN_UTILSIZE_Y   8
#define SEARCHSCREEN_UTILSIZE_X  18

class SearchScreen
{
private:
    WINDOW* win;
    int ch;
    int selection, ordertype, charnum;
    char findtext[255];
    char displaytext[25];

    void refreshdisplaytext();
    void cleardisplaytext();
public:
    SearchScreen();
    void init();
    void update();
    void refresh();
};

#endif // SEARCHSCREEN_H_INCLUDED
