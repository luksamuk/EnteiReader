#include "LibScreen.h"

LibScreen::LibScreen() // TODO: add param Vigil* vigilante
{
    selection = 0;
    //vigil = vigilante;

    // Inicializa a janela
    win        = newwin(24, LIBSCREEN_FILESFIELD_SIZE, 0, 0);
    attributes = newwin(24, LIBSCREEN_ATTRFIELD_SIZE, 0, LIBSCREEN_FILESFIELD_SIZE);
    srchscr    = new SearchScreen();
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

    //TODO: adicionar e formatar nomes dos livros.
    //n_books = vigil->filesCount();
    //�rea �til do nome dos livros: 58 caracteres.
    // Algo maior que isso � reduzido para 55 caracteres + "...\0".

    // Experimental:
    n_books = 7;
    books = new char*[n_books];
    for(int i = 0; i < n_books; i++)
        books[i] = new char[255];
    strcpy(books[0], "lipsum.epub");
    strcpy(books[1], "brownfox.epub");
    strcpy(books[2], "brascubas.epub");
    strcpy(books[3], "fallofreach.epub");
    strcpy(books[4], "ghostonyx.epub");
    strcpy(books[5], "justforfun.epub");
    strcpy(books[6], "levelup.epub");
    for(int i = 0; i < n_books; i++)
        clipFilename(books[i]);
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
            return selection;
            break;
        case 27:         // Sair
            endwin();
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
    case 0:
        strcpy(b_name, "Lorem Ipsum");
        strcpy(b_author, "Dom�nio P�blico");
        strcpy(b_publisher, "Dom�nio P�blico");
        break;
    case 1:
        strcpy(b_name, "The Quick Brown Fox Jumps Over The Lazy Dog");
        strcpy(b_author, "Dom�nio P�blico");
        strcpy(b_publisher, "Dom�nio P�blico");
        break;
    case 2:
        strcpy(b_name, "Mem�rias P�stumas de Br�s Cubas");
        strcpy(b_author, "Machado de Assis");
        strcpy(b_publisher, "Eleva��o");
        break;
    case 3:
        strcpy(b_name, "The Fall of Reach");
        strcpy(b_author, "Eric Nylund");
        strcpy(b_publisher, "TOR");
        break;
    case 4:
        strcpy(b_name, "Ghosts of Onyx");
        strcpy(b_author, "Eric Nylund");
        strcpy(b_publisher, "TOR");
        break;
    case 5:
        strcpy(b_name, "Just For Fun");
        strcpy(b_author, "Linus Torvalds");
        strcpy(b_publisher, "Harpercollins USA");
        break;
    case 6:
        strcpy(b_name, "Level Up - Um Guia para o Design de Grandes Jogos");
        strcpy(b_author, "Scott Rogers");
        strcpy(b_publisher, "Blutcher");
    }

    for(int i = 0; i < n_books; i++)
        printMenuElement(books[i], i);

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
