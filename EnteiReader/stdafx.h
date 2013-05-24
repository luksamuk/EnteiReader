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

#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <vector>
#include "Vigil.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
using namespace std;
using namespace rapidxml;

// Delegate da tela de menus
#define MENU_READER  0
#define MENU_LIBRARY 1
#define MENU_SEARCH  2
#define MENU_MANAGE  3
typedef int MENUTYPE;

extern Vigil* vigil;
extern void (*showcontrols)(MENUTYPE);
extern void (*killall) (void);
extern void (*clearscr)(void);

typedef unsigned char byte;

#endif // STDAFX_H_INCLUDED
