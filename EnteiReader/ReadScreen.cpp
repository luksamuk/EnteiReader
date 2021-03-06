#include "ReadScreen.h"

ReadScreen::ReadScreen()
{
    reader = newwin(23, 50, 0, 0);
    index = newwin(23, 30, 0, 50);
    indexlist = NULL;
    shownlist = NULL;
    scroll_h = scroll_v = 0;

    maxtextlines = maxtextcols = 0;
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
    mvwprintw(index, 1, 22, "Indice");
    mvwaddch(index, 2, 0, ACS_LTEE);
    mvwhline(index, 2, 1, ACS_HLINE, 28);
    mvwaddch(index, 2, 29, ACS_RTEE);

    // Janela de exibição
    mvwaddch(reader, 1, 48, ACS_UARROW);
    mvwaddch(reader, 20, 48, ACS_DARROW);
    mvwaddch(reader, 21, 1, ACS_LARROW);
    mvwaddch(reader, 21, 47, ACS_RARROW);

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
                case KEY_DOWN:
                    break;
                case KEY_UP:
                    break;
                case KEY_RIGHT:
                    break;
                case KEY_LEFT:
                    break;
            }

            for(int i = 0; i < 48; i++)
                mvwprintw(reader, 1, 1, text[i]);
        }
        else // active == WINDOW_INDEX
        {
            switch(ch)
            {
                case KEY_DOWN:
                    if(selection < nchapters - 1) ++selection;
                    getText();
                break;
                case KEY_UP:
                    if(selection > 0) --selection;
                    getText();
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
        mvwprintw(index, 3 + i, 1, shownlist[i]);
        if(selection == i) wattroff(index, A_REVERSE);
    }

    // Leitor
    // TAMANHO: 20x47, começando a partir de 1x1
    updatescrollbars();


    if(reader) wrefresh(reader);
    if(index)  wrefresh(index);
}

void ReadScreen::clearallscrs(void)
{
    for(int j = 0; j < 19; j++)
        for(int i = 0; i < 28; i++)
            mvwprintw(index, j + 3, i + 1, " ");

    for(int j = 0; j < 20; j++)
        for(int i = 0; i < 47; i++)
            mvwprintw(reader, j + 1, i + 1, " ");
}

void ReadScreen::updatescrollbars(void)
{
    for(int i = 2; i < 20; i++)
        mvwprintw(reader, i, 48, " ");
    for(int i = 2; i < 47; i++)
        mvwprintw(reader, 21, i, " ");

    mvwaddch(reader, 2 + scroll_v, 48, ACS_DIAMOND);
    mvwaddch(reader, 21, 2 + scroll_h, ACS_DIAMOND);
}

void ReadScreen::makeindex()
{
    nchapters = n_capitulos();
    indexlist = new char*[nchapters];
    shownlist = new char*[nchapters];
    capitulos(indexlist);
    capitulos(shownlist);

    // Deverá ser alterado para o nome real do capítulo.
    for(int i = 0; i < nchapters; i++)
    {
        if(strlen(shownlist[i]) > 27)
        {
            shownlist[i][28] = '\0';
            shownlist[i][27] = '.';
            shownlist[i][26] = '.';
            shownlist[i][25] = '.';
        }
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

    if(shownlist)
    {
        for(int i = 0; i < nchapters; i++)
            delete shownlist[i];
    }
    delete shownlist;
    shownlist = NULL;

    clearallscrs();
}

// Capítulos


int ReadScreen::n_capitulos(void)
{
    xml_document<> doc;
    xml_node<> *node;
    ifstream menu((*getEndereco)());
    vector<char> buffer ((istreambuf_iterator<char>(menu)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    node = doc.first_node("package");
    int i = 0;
    for(xml_node<> *caminho_node = node->first_node("manifest")->first_node("item"); caminho_node; caminho_node = caminho_node->next_sibling())
    {
        if(strcmp(caminho_node->first_attribute("media-type")->value(), "application/xhtml+xml") == 0)
        i++;
    }
    return i;
}

void ReadScreen::capitulos(char** &c)
{
    xml_document<> doc;
    xml_node<> *node;
    ifstream menu((*getEndereco)());
    vector<char> buffer ((istreambuf_iterator<char>(menu)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    node = doc.first_node("package");
    int i = 0;
    for(xml_node<> *caminho_node = node->first_node("manifest")->first_node("item"); caminho_node; caminho_node = caminho_node->next_sibling())
    {
       if(strcmp(caminho_node->first_attribute("media-type")->value(), "application/xhtml+xml") == 0)
       {
          c[i] = new char[255];
          strcpy(c[i], caminho_node->first_attribute("href")->value());
          i++;
       }
    }
}

void ReadScreen::getText()
{
    //xml_document<> doc;
    //xml_node<> *node = doc.first_node("package");
    //char* end;
    //ifstream menu(indexlist[selection]);
    //end = print(text[0], doc, 0);
    //doc.parse<0>(&text[0][0]);
}
