#include "LibScreen.h"

LibScreen::LibScreen()
{
    selection = 0;
    curr_elem = 0;
    // Inicializa a janela
    win        = newwin(23, LIBSCREEN_FILESFIELD_SIZE, 0, 0);
    attributes = newwin(23, LIBSCREEN_ATTRFIELD_SIZE, 0, LIBSCREEN_FILESFIELD_SIZE);
    srchscr    = new SearchScreen();
    mngscr     = new ManageScreen();

    vigil = new Vigil();

    vigil->addDir(".");
    vigil->addDir("./Books");
    vigil->update(false);
    a = NULL;

    getBookList();
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
    // Cabeçalho
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
        case KEY_UP:     // Próximo livro
            --selection;
            if(selection < 0) selection = n_books - 1;
            this->refresh();
            break;
        case KEY_F(2):
            (*showcontrols)(MENU_MANAGE);
            mngscr->init();
            mngscr->refresh();
            mngscr->update();
            delBookList();
            delete books;
            getBookList();
            (*showcontrols)(MENU_LIBRARY);
            this->init();
            this->refresh();
            break;
        case KEY_F(3):   // Pesquisa & Ordem
            (*showcontrols)(MENU_SEARCH); // Exibe os controles de busca
            srchscr->init();
            srchscr->refresh();
            srchscr->update();

            // TODO: Realizar a pesquisa/ordenação.

            (*showcontrols)(MENU_LIBRARY); // Exibe os controles de biblioteca
            this->init();
            this->refresh();
            break;
        case 10:         // Selecionar livro
            if(n_books > 0)
            {
                vigil->openFile(books[selection].filepath, true);
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
    clearAttributeField(LIBSCREEN_ATTRFIELD_NAME);
    clearAttributeField(LIBSCREEN_ATTRFIELD_AUTHOR);
    if(n_books > 0)
    {
        strcpy(b_name, books[selection].title);
        strcpy(b_author, books[selection].author);
        strcpy(b_publisher, books[selection].publisher);

        clipAttribute(books[selection].title);
        clipAttribute(books[selection].author);
        clipAttribute(books[selection].publisher);

        for(int i = 0; i < n_books; i++)
            printMenuElement(books[i].filename, i);
        box(attributes, 0, 0);
    }

    if(win)        wrefresh(win);
    if(attributes) wrefresh(attributes);
}

void LibScreen::printMenuElement(const char* text, int order)
{
    // TODO: Fazer com que textos maiores do que a janela não sejam mostrados.
    if(order == selection)
    {
        wattron(win, A_REVERSE);
        mvwprintw(win, 3 + order, 1, text);
        wattroff(win, A_REVERSE);

        // Imprime também atributos do livro na janela de informações
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

void LibScreen::printListRecursively(Node* a)
{
    if (a == NULL) return;
    printListRecursively(a->esq);
    printMenuElement(a->book->filename, curr_elem++);
    printListRecursively(a->dir);
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
    while(i != (int)strlen(filename))
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
    if(strlen(attribute) > LIBSCREEN_ATTRFIELD_UTILSIZE - 2)
    {
        attribute[LIBSCREEN_ATTRFIELD_UTILSIZE - 1]     = '\0';
        attribute[LIBSCREEN_ATTRFIELD_UTILSIZE - 2] = '.';
        attribute[LIBSCREEN_ATTRFIELD_UTILSIZE - 3] = '.';
        attribute[LIBSCREEN_ATTRFIELD_UTILSIZE - 4] = '.';
    }
}

void LibScreen::clearAttributeField(int linnum)
{
    for(int i = 1; i <= LIBSCREEN_ATTRFIELD_UTILSIZE; i++)
        mvwprintw(attributes, linnum, i, " ");
}

void LibScreen::getBookList(void)
{
    // Limpa a tela
    for(int i = 3; i < 22; i++)
        for(int j = 1; j < LIBSCREEN_ATTRFIELD_SIZE; j++)
            mvwprintw(win, i, j, " ");

    n_books = vigil->filesCount();
    books = new Book[n_books];

    Vigil::fileIndex* file;
    file = vigil->library();

    if(file != NULL)
    {
        for(int i = 0; i < n_books; i++)
        {
            books[i].filename  = new char[255];
            books[i].filepath  = new char[255];
            books[i].title     = new char[255];
            books[i].author    = new char[255];
            books[i].publisher = new char[255];
            sprintf(books[i].filepath, "%s", file->fileAddress);
            sprintf(books[i].filename, "%s", file->fileAddress);

            vigil->openFile(books[i].filepath, true);
            sprintf(books[i].title, "%s", getTitulo((*getEndereco)()));
            sprintf(books[i].author, "%s", getAutores((*getEndereco)()));
            sprintf(books[i].publisher, "%s", getEditora((*getEndereco)()));
            insereElemento(a, &books[i], i);

            clipFilename(books[i].filename);
            file = file->next;
        }
    }
}

void LibScreen::delBookList(void)
{
    selection = 0;
    n_books = 0;
}

char* LibScreen::getTitulo (char* a)
{
    xml_document<> doc;
    xml_node<> *node;
    ifstream menu (a);
    vector<char> buffer ((istreambuf_iterator<char>(menu)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    node = doc.first_node("package");
    xml_node<> *titulo_node = node->first_node("metadata")->first_node("dc:title");
    return titulo_node->value();
}
char* LibScreen::getAutores (char* a)
{
    xml_document<> doc;
    xml_node<> *node;
    ifstream menu (a);
    vector<char> buffer ((istreambuf_iterator<char>(menu)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    node = doc.first_node("package");
    xml_node<> *autores_node = node->first_node("metadata")->first_node("dc:creator");
    return autores_node->value();
}

char* LibScreen::getEditora (char* a)
{
    xml_document<> doc;
    xml_node<> *node;
    ifstream menu (a);
    vector<char> buffer ((istreambuf_iterator<char>(menu)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    node = doc.first_node("package");
    xml_node<> *editora_node = node->first_node("metadata")->first_node("dc:publisher");
    return editora_node->value();
}

// Relativo à árvore

void LibScreen::insereElemento(Node *&a, Book* book, int bookid)
{
    if (a == NULL)
    {
        a = new Node;
        a->dir = NULL;
        a->esq = NULL;
        a->book = book;
        a->bookid = bookid;
        return;
    }
    if (strcmp(book->title, a->book->title) == -1)
    {
        insereElemento(a->esq, book, bookid);
        return;
    }
    if (strcmp(book->title,a->book->title) == 1)
    {
        insereElemento(a->dir, book, bookid);
        return;
    }
}
