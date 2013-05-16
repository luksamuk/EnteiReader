#ifndef STDAFX_H_INCLUDED
#define STDAFX_H_INCLUDED

#ifdef _WIN32
    #include <curses.h>
    #include <panel.h>
#endif
#ifdef linux
    #include <ncurses.h>
    #include <panel.h>
#endif

#include <stdlib.h>
#include <string.h>

#endif // STDAFX_H_INCLUDED
