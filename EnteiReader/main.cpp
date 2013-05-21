#include "stdafx.h"
#include "LibScreen.h"
#include "ReadScreen.h"

// Prompt de comando tem tamanho 25x80.

WINDOW* controls;
void (*showcontrols)(MENUTYPE);
void (*killall)(void);

void mostra_controles(int);
void pair_colors(void);
void KillAll(void);
LibScreen*  libscr;
ReadScreen* readscr;

int main()
{
    int ch;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);

    if(has_colors()) pair_colors();
    showcontrols = &mostra_controles;
    killall = &KillAll;
    controls = newwin(0, 80, 24, 0);
    libscr = new LibScreen();
    readscr = new ReadScreen();

    refresh();

    while(true)
    {
        // Pede o livro
        libscr->init();
        mostra_controles(1);
        wrefresh(controls);
        libscr->update();

        // Mostra o livro
        readscr->init();
        mostra_controles(0);
        wrefresh(controls);
        readscr->refresh();
        readscr->update();
    }
}

void mostra_controles(MENUTYPE tipodemenu)
{
    // Limpa a caixa de controles
    for(int i = 0; i < COLS; i++)
        mvwprintw(controls, 0, i, " ");

    switch(tipodemenu)
    {
    case MENU_READER: // Leitor
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
    case MENU_LIBRARY: // Biblioteca
        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 0, "ESC");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 4, "Sair");

        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 10, "CIMA/BAIXO");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 22, "Escolher");

        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 32, "ENTER");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 38, "Selecionar");

        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 50, "F2");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 53, "Gerenciar");

        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 64, "F3");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 68, "Pesq./Ordem");
        break;
    case MENU_SEARCH: // Janela de pesquisa
        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 0, "F2");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 3, "Por nome");

        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 12, "F3");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 15, "Por autor");

        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 25, "F4");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 28, "Por editora");

        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 40, "ENTER");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 46, "Pesquisar");

        mvwprintw(controls, 0, 62, "Digite a pesquisa.");
        break;
    }
    wrefresh(controls);
}

void pair_colors(void)
{
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
}

void KillAll(void)
{
    delete libscr;
    delete readscr;
}
