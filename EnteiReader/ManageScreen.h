#ifndef MANAGESCREEN_H_INCLUDED
#define MANAGESCREEN_H_INCLUDED

#include "stdafx.h"

class ManageScreen
{
private:
    int ch;
    WINDOW* win;
public:
    ManageScreen();
    ~ManageScreen();
    void init();
    void update();
    void refresh();
};
#endif // MANAGESCREEN_H_INCLUDED
