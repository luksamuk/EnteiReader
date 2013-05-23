#include "ReadScreen.h"

ReadScreen::ReadScreen()
{
    reader = newwin(23, 50, 0, 0);
    index = newwin(23, 30, 0, 50);
    indexlist = NULL;
}

void ReadScreen::init()
{
    selection = 0;
    active = WINDOW_READER;

    // Bordas
    box(reader, 0, 0);
    box(index, 0, 0);

    // Cabeçalho da janela
    mvwaddch (reader, 0, 3, ACS_RTEE);
    mvwprintw(reader, 0, 4, " ");
    mvwaddch (reader, 0, 5, ACS_LTEE);

    mvwaddch (index, 0, 3, ACS_RTEE);
    mvwprintw(index, 0, 4, " ");
    mvwaddch (index, 0, 5, ACS_LTEE);

    // Cabeçalho do índice
    mvwprintw(index, 1, 22, "índice");
    mvwaddch(index, 2, 0, ACS_LTEE);
    mvwhline(index, 2, 1, ACS_HLINE, 28);
    mvwaddch(index, 2, 29, ACS_RTEE);

    // Janela de exibição
    mvwaddch(reader, 1, 48, ACS_UARROW);
    mvwaddch(reader, 21, 48, ACS_DARROW);

    // Cria o indice
    makeindex();
}

void ReadScreen::update()
{
    while(true)
    {
        ch = wgetch(stdscr);
        switch(ch)
        {
        case '\t':
            active = !active;
            break;
        case KEY_F(2): // Selecionar outro livro
            delindex();
            return;
        case 27:       // Sair
            endwin();
            (*killall)();
            exit(0);
            break;
        }

        if(active == WINDOW_READER)
        {
            switch(ch)
            {
            }
        }
        else // active == WINDOW_INDEX
        {
            switch(ch)
            {
                case KEY_DOWN:
                    if(selection < nchapters - 1) ++selection;
                    this->refresh();
                break;
                case KEY_UP:
                    if(selection > 0) --selection;
                break;
            }
        }

        this->refresh();
    }
}

void ReadScreen::refresh()
{
    if(active == WINDOW_INDEX)
    {
        mvwprintw(index,  0, 4, "*");
        mvwprintw(reader, 0, 4, " ");
    }
    else
    {
        mvwprintw(index,  0, 4, " ");
        mvwprintw(reader, 0, 4, "*");
    }

    // Índice
    // TAMANHO: 19x28, começando a partir de 3x1
    for(int i = 0; i < nchapters; i++)
    {
        if(selection == i) wattron(index, A_REVERSE);
        mvwprintw(index, 3 + i, 1, indexlist[i]);
        if(selection == i) wattroff(index, A_REVERSE);
    }

    // Leitor
    // TAMANHO: 21x47, começando a partir de 1x1



    if(reader) wrefresh(reader);
    if(index)  wrefresh(index);
}

void ReadScreen::makeindex()
{
    // Test purposes only
    nchapters = 3;
    indexlist = new char*[nchapters];
    for(int i = 0; i < nchapters; i++)
    {
        indexlist[i] = new char[255];
        sprintf(indexlist[i], "Capitulo %d", i);
    }
}

void ReadScreen::delindex()
{
    if(indexlist)
    {
        for(int i = 0; i < nchapters; i++)
            delete indexlist[i];
    }
    delete indexlist;
    indexlist = NULL;
}
