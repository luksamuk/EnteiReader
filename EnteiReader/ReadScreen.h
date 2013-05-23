#ifndef READSCREEN_H_INCLUDED
#define READSCREEN_H_INCLUDED

#include "stdafx.h"

typedef bool ACTIVEWINDOW;
#define WINDOW_READER false
#define WINDOW_INDEX  true

class ReadScreen
{
private:
    WINDOW* reader, *index;
    int selection;
    int ch;
    ACTIVEWINDOW active;

    int nchapters;
    char** indexlist;

    void makeindex();
    void delindex();
public:
    ReadScreen();
    void init();
    void update();
    void refresh();
};

#endif // READSCREEN_H_INCLUDED
