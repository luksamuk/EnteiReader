#ifndef READSCREEN_H_INCLUDED
#define READSCREEN_H_INCLUDED

#include "stdafx.h"

class ReadScreen
{
private:
    WINDOW* reader, *index;
    int selection;
    int ch;
public:
    ReadScreen();
    void init();
    void update();
    void refresh();
};

#endif // READSCREEN_H_INCLUDED
