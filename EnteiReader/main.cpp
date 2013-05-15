#include <panel.h>
#include <stdio.h>
// Prompt de comando tem tamanho 25x80.

using namespace std;

WINDOW* winreader, *winindex;

void mostra_controles(int);

int main()
{
    int ch;

    initscr();
    cbreak();
    noecho();
    curs_set(0);

    winreader = newwin(24, 50, 0, 0);
    box(winreader, 0, 0);

    winindex  = newwin(24, 30, 0, 50);
    box(winindex, 0, 0);

    mostra_controles(0);

    // Indice falso
    mvwprintw(winindex, 1, 22, "Indice");
    mvwaddch(winindex, 2, 0, ACS_LTEE);
    mvwhline(winindex, 2, 1, ACS_HLINE, 28);
    mvwaddch(winindex, 2, 29, ACS_RTEE);
    mvwprintw(winindex, 3, 1, "1. Lorem Ipsum");
    mvwprintw(winindex, 4, 1, " 1.1 Lorem ipsum dolor si...");
    mvwprintw(winindex, 5, 1, " 1.2 Ut orci risus, ferme...");
    mvwprintw(winindex, 6, 1, " 1.3 Suspendisse quis mag...");
    mvwprintw(winindex, 7, 1, " 1.4 Praesent condimentum...");
    mvwprintw(winindex, 8, 1, " 1.5 Duis ut ante risus. ...");

    // LIPSUM, texto falso
    mvwprintw(winreader, 1, 1,  "Lorem ipsum dolor sit amet, consectetur");
    mvwprintw(winreader, 2, 1,  "adipiscing elit. Nunc eget nunc eget eros");
    mvwprintw(winreader, 3, 1,  "vehicula interdum. Nunc ipsum metus, pharetra");
    mvwprintw(winreader, 4, 1,  "non mattis vitae, aliquet a lacus. In odio");
    mvwprintw(winreader, 5, 1,  "felis, posuere in commodo ut, fringilla id mi.");
    mvwprintw(winreader, 5, 1,  "Maecenas et mauris ut lacus lobortis volutpat.");
    mvwprintw(winreader, 6, 1,  "Mauris a odio sit amet augue interdum tempus at");
    mvwprintw(winreader, 7, 1,  "sit amet enim. Sed posuere, felis sit amet");
    mvwprintw(winreader, 8, 1,  "egestas egestas, mauris turpis mollis erat, at");
    mvwprintw(winreader, 9, 1,  "semper arcu magna a dui. Fusce venenatis est");
    mvwprintw(winreader, 10, 1, "mauris. Sed pellentesque condimentum metus,");
    mvwprintw(winreader, 11, 1, "sit amet vestibulum orci auctor vitae. Nunc et");
    mvwprintw(winreader, 12, 1, "velit eget arcu ullamcorper tincidunt in eget");
    mvwprintw(winreader, 13, 1, "sem. Nulla sed laoreet risus.");

    refresh();
    wrefresh(winreader);
    wrefresh(winindex);

    while((ch = getch()) != 27) {} // Espera apertar o ESC

    delwin(winreader);
    endwin();
    return 0;
}

void mostra_controles(int tipodemenu)
{
    switch(tipodemenu)
    {
    case 0: // Leitor
        attron(A_REVERSE);
        mvprintw(24, 0, "TAB");
        attroff(A_REVERSE);
        mvprintw(24, 4, "Alternar janela");

        attron(A_REVERSE);
        mvprintw(24, 20, "ESC");
        attroff(A_REVERSE);
        mvprintw(24, 24, "Sair");

        attron(A_REVERSE);
        mvprintw(24, 30, "F2");
        attroff(A_REVERSE);
        mvprintw(24, 33, "Escolher arquivo");
    }
    refresh();
}
