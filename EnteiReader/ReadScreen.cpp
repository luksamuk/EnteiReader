#include "ReadScreen.h"

ReadScreen::ReadScreen()
{
    selection = 0;

    // Janela de leitura
    reader = newwin(24, 50, 0, 0);

    // Janela de índice
    index = newwin(24, 30, 0, 50);
}

void ReadScreen::init()
{
    // Bordas
    box(reader, 0, 0);
    box(index, 0, 0);
    // Cabeçalho do índice
    mvwprintw(index, 1, 22, "índice");
    mvwaddch(index, 2, 0, ACS_LTEE);
    mvwhline(index, 2, 1, ACS_HLINE, 28);
    mvwaddch(index, 2, 29, ACS_RTEE);
}

void ReadScreen::update()
{
    while(true)
    {
        ch = wgetch(stdscr);
        switch(ch)
        {
        case KEY_F(2): // Selecionar outro livro
            return;
        case 27:       // Sair
            endwin();
            (*killall)();
            exit(0);
            break;
        }
        this->refresh();
    }
}

void ReadScreen::refresh()
{
    // Textos de teste.
    // Índice
    mvwprintw(index, 3, 1, "1. Lorem Ipsum");
    mvwprintw(index, 4, 1, " 1.1 Lorem ipsum dolor si...");
    mvwprintw(index, 5, 1, " 1.2 Ut orci risus, ferme...");
    mvwprintw(index, 6, 1, " 1.3 Suspendisse quis mag...");
    mvwprintw(index, 7, 1, " 1.4 Praesent condimentum...");
    mvwprintw(index, 8, 1, " 1.5 Duis ut ante risus. ...");

    // Leitor
    mvwprintw(reader, 1, 1,  "Lorem ipsum dolor sit amet, consectetur");
    mvwprintw(reader, 2, 1,  "adipiscing elit. Nunc eget nunc eget eros");
    mvwprintw(reader, 3, 1,  "vehicula interdum. Nunc ipsum metus, pharetra");
    mvwprintw(reader, 4, 1,  "non mattis vitae, aliquet a lacus. In odio");
    mvwprintw(reader, 5, 1,  "felis, posuere in commodo ut, fringilla id mi.");
    mvwprintw(reader, 5, 1,  "Maecenas et mauris ut lacus lobortis volutpat.");
    mvwprintw(reader, 6, 1,  "Mauris a odio sit amet augue interdum tempus at");
    mvwprintw(reader, 7, 1,  "sit amet enim. Sed posuere, felis sit amet");
    mvwprintw(reader, 8, 1,  "egestas egestas, mauris turpis mollis erat, at");
    mvwprintw(reader, 9, 1,  "semper arcu magna a dui. Fusce venenatis est");
    mvwprintw(reader, 10, 1, "mauris. Sed pellentesque condimentum metus,");
    mvwprintw(reader, 11, 1, "sit amet vestibulum orci auctor vitae. Nunc et");
    mvwprintw(reader, 12, 1, "velit eget arcu ullamcorper tincidunt in eget");
    mvwprintw(reader, 13, 1, "sem. Nulla sed laoreet risus.");


    if(reader) wrefresh(reader);
    if(index)  wrefresh(index);
}
