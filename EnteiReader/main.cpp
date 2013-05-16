#include "stdafx.h"
#include "LibScreen.h"
#include "ReadScreen.h"

// Prompt de comando tem tamanho 25x80.

WINDOW* controls;

void mostra_controles(int);
LibScreen libscr;
ReadScreen readscr;

int main()
{
    int ch;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);

    controls = newwin(0, 80, 24, 0);
    libscr.init();
    readscr.init();

    refresh();

    mostra_controles(1);
    wrefresh(controls);
    libscr.update();

    mostra_controles(0);
    wrefresh(controls);
    readscr.refresh();
    readscr.update();

    endwin();
    return 0;
}

void mostra_controles(int tipodemenu)
{
    // Limpa a caixa de controles
    for(int i = 0; i < 80; i++)
        mvwprintw(controls, 0, i, " ");

    switch(tipodemenu)
    {
    case 0: // Leitor
        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 0, "ESC");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 4, "Sair");

        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 10, "TAB");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 14, "Alternar janela");

        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 31, "F2");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 34, "Abrir outro livro");
        break;
    case 1: // Biblioteca
        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 0, "ESC");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 4, "Sair");

        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 10, "ENTER");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 16, "Escolher arquivo");
        break;
    }
    wrefresh(controls);
}
