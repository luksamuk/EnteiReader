#include "LibScreen.h"

LibScreen::LibScreen()
{
    selection = 0;
    // Inicializa a janela
    win        = newwin(23, LIBSCREEN_FILESFIELD_SIZE, 0, 0);
    attributes = newwin(23, LIBSCREEN_ATTRFIELD_SIZE, 0, LIBSCREEN_FILESFIELD_SIZE);
    srchscr    = new SearchScreen();

    vigil = new Vigil();

    vigil->addDir(".");
    vigil->addDir("./Books");
    vigil->update(false);

    books = new Book[n_books];
    n_books = vigil->filesCount();

    Vigil::fileIndex* file = vigil->library();

    if(file != NULL)
    {
        for(int i = 0; i < n_books; i++)
        {
            books[i].filename = new char[255];
            books[i].filepath = new char[255];
            sprintf(books[i].filename, "%s", file->fileAddress);
            sprintf(books[i].filepath, "%s", file->fileAddress);
            clipFilename(books[i].filename);
            file = file->next;
        }
    }
}

LibScreen::~LibScreen()
{
    vigil->~Vigil();
    delete vigil;
}

void LibScreen::init()
{
    // Bordas
    box(win, 0, 0);
    box(attributes, 0, 0);
    // Cabe�alho
    mvwprintw(win, 1, 1, "Biblioteca");
    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, ACS_HLINE, 28);
    mvwaddch(win, 2, 29, ACS_RTEE);

    if(has_colors()) wattron(attributes, COLOR_PAIR(1));
    mvwprintw(attributes, 1, 1, "NOME");
    mvwprintw(attributes, 4, 1, "AUTOR");
    mvwprintw(attributes, 7, 1, "EDITORA");
    if(has_colors()) wattroff(attributes, COLOR_PAIR(1));
}

int LibScreen::update()
{
    this->refresh();
    while(true)
    {
        ch = wgetch(stdscr);
        switch(ch)
        {
        case KEY_DOWN:    // Livro anterior
            ++selection;
            if(selection >= n_books) selection = 0;
            this->refresh();
            break;
        case KEY_UP:     // Pr�ximo livro
            --selection;
            if(selection < 0) selection = n_books - 1;
            this->refresh();
            break;
        case KEY_F(3):   // Pesquisa & Ordem
            (*showcontrols)(MENU_SEARCH); // Exibe os controles de busca
            srchscr->init();
            srchscr->refresh();
            srchscr->update(); // TODO: Realizar a pesquisa/ordena��o.
            (*showcontrols)(MENU_LIBRARY); // Exibe os controles de biblioteca
            this->init();
            this->refresh();
            break;
        case 10:         // Selecionar livro
            if(n_books > 0)
            {
                endwin();
                vigil->openFile(books[selection].filepath, true);
                reset_prog_mode();
                refresh();
                return selection;
            }
            break;
        case 27:         // Sair
            endwin();
            (*killall)();
            exit(0);
            break;
        }
    }
}

void LibScreen::refresh()
{
    // Diferencia atributos. TESTE!!!
    switch(selection)
    {
    default:
        strcpy(b_name, "Lorem Ipsum");
        strcpy(b_author, "Dom�nio P�blico");
        strcpy(b_publisher, "Dom�nio P�blico");
        break;
    }

    for(int i = 0; i < n_books; i++)
        printMenuElement(books[i].filename, i);

    if(win)        wrefresh(win);
    if(attributes) wrefresh(attributes);
}

void LibScreen::printMenuElement(const char* text, int order)
{
    // TODO: Fazer com que textos maiores do que a janela n�o sejam mostrados.
    if(order == selection)
    {
        wattron(win, A_REVERSE);
        mvwprintw(win, 3 + order, 1, text);
        wattroff(win, A_REVERSE);

        // Imprime tamb�m atributos do livro na janela de informa��es
        clipAttribute(b_name);
        clipAttribute(b_author);
        clipAttribute(b_publisher);
        clearAttributeField(LIBSCREEN_ATTRFIELD_NAME);
        mvwprintw(attributes, LIBSCREEN_ATTRFIELD_NAME, 1, b_name);
        clearAttributeField(LIBSCREEN_ATTRFIELD_AUTHOR);
        mvwprintw(attributes, LIBSCREEN_ATTRFIELD_AUTHOR, 1, b_author);
        clearAttributeField(LIBSCREEN_ATTRFIELD_PUBLISHER);
        mvwprintw(attributes, LIBSCREEN_ATTRFIELD_PUBLISHER, 1, b_publisher);
    }
    else mvwprintw(win, 3 + order, 1, text);
}

void LibScreen::clipFilename(char filename[])
{
    char aux[255];
    int i = strlen(filename), j = 0;

    // Extrai o nome do arquivo
    while(true)
    {
        if(filename[i] != '/') i--;
        else break;
    }
    while(i != strlen(filename))
    {
        i++;
        aux[j] = filename[i];
        j++;
    }
    aux[j] = '\0';
    strcpy(filename, aux);

    if(strlen(filename) > LIBSCREEN_FILESFIELD_UTILSIZE - 1)
    {
        filename[LIBSCREEN_FILESFIELD_UTILSIZE]     = '\0';
        filename[LIBSCREEN_FILESFIELD_UTILSIZE - 1] = '.';
        filename[LIBSCREEN_FILESFIELD_UTILSIZE - 2] = '.';
        filename[LIBSCREEN_FILESFIELD_UTILSIZE - 3] = '.';
    }
}

void LibScreen::clipAttribute(char attribute[])
{
    if(strlen(attribute) > LIBSCREEN_ATTRFIELD_UTILSIZE - 1)
    {
        attribute[LIBSCREEN_ATTRFIELD_UTILSIZE]     = '\0';
        attribute[LIBSCREEN_ATTRFIELD_UTILSIZE - 1] = '.';
        attribute[LIBSCREEN_ATTRFIELD_UTILSIZE - 2] = '.';
        attribute[LIBSCREEN_ATTRFIELD_UTILSIZE - 3] = '.';
    }
}

void LibScreen::clearAttributeField(int linnum)
{
    for(int i = 1; i <= LIBSCREEN_ATTRFIELD_UTILSIZE; i++)
        mvwprintw(attributes, linnum, i, " ");
}
