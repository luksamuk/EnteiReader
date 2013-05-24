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
    void clearallscrs(void);

    int nchapters;
    char** indexlist, **shownlist;

    void makeindex();
    void delindex();

    // Criador de capitulos
    char* endereco(void);
    int n_capitulos(void);
    void capitulos(char**&);
public:
    ReadScreen();
    void init();
    void update();
    void refresh();
};

#endif // READSCREEN_H_INCLUDED
