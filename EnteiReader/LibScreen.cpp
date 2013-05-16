#include "LibScreen.h"

LibScreen::LibScreen() // TODO: add param Vigil* vigilante
{
    selection = 0;
    //vigil = vigilante;
}

void LibScreen::init()
{
    // Inicializa a janela
    win = newwin(24, 60, 0, 10);
    // Insere a caixa
    box(win, 0, 0);
    // Cabeçalho
    mvwprintw(win, 1, 1, "Biblioteca");
    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, ACS_HLINE, 58);
    mvwaddch(win, 2, 59, ACS_RTEE);

    //TODO: adicionar e formatar nomes dos livros.
    //n_books = vigil->filesCount();
    //Área útil do nome dos livros: 58 caracteres.
    // Algo maior que isso é reduzido para 55 caracteres + "...\0".

    // Experimental:
    n_books = 5;
    books = new char*[n_books];
    for(int i = 0; i < n_books; i++)
        books[i] = new char[255];
    strcpy(books[0], "Lorem Ipsum");
    strcpy(books[1], "The Quick Brown Fox Jumps Over The Lazy Dog");
    strcpy(books[2], "Memórias Póstumas de Brás Cubas");
    strcpy(books[3], "The Fall of Reach");
    strcpy(books[4], "Ghosts of Onyx");

}

int LibScreen::update()
{
    this->refresh();
    while(true)
    {
        ch = wgetch(stdscr);
        switch(ch)
        {
        case KEY_DOWN:
            ++selection;
            if(selection >= n_books) selection = 0;
            this->refresh();
            break;
        case KEY_UP:
            --selection;
            if(selection < 0) selection = n_books - 1;
            this->refresh();
            break;
        case 10:
            return selection;
            break;
        case 27:
            exit(0);
            break;
        }
    }
}

void LibScreen::refresh()
{
    for(int i = 0; i < n_books; i++)
        printMenuElement(books[i], i);

    if(win) wrefresh(win);
}

void LibScreen::printMenuElement(const char* text, int order)
{
    // TODO: Fazer com que textos maiores do que a janela não sejam mostrados.
    if(order == selection)
    {
        wattron(win, A_REVERSE);
        mvwprintw(win, 3 + order, 1, text);
        wattroff(win, A_REVERSE);
    }
    else mvwprintw(win, 3 + order, 1, text);
}
