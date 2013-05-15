#include <curses.h>
using namespace std;
//ATEN��O: Coordenadas em curses s�o dadas em (y,x) e n�o em (x,y)

WINDOW* win;                                    // Janela de teste

int main()
{
    initscr();                                  // Inicializa modo curses
    cbreak();                                   // Desabilitar buffer de linha, passar tudo ao escrever na tela
    noecho();                                   // N�o mostra ao digitar
    curs_set(0);                                // Cursor invis�vel

    win = newwin(10, 30, 1, 1);                 // Cria uma nova janela
    box(win, 0, 0);                             // Cria uma caixa comum ao redor da janela

    printw("Hello, world!");                    // Imprime "Hello World!" na janela principal (stdscr)

    mvwprintw(win, 1, 1, "Testing window...");  // Move o cursor para a posi��o (y = 1, x = 1) da janela
    mvwprintw(win, 2, 1, "Press a key to exit");// win e imprime o texto

    refresh();                                  // Faz o flush da stdscr para a tela
    wrefresh(win);                              // Faz o flush da win para a tela

    getch();                                    // Espera por um pressionamento qualquer de tecla

    delwin(win);                                // Deleta win
    endwin();                                   // Termina o modo curses

    return 0;
}
