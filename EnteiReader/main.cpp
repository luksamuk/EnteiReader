#include "stdafx.h"
#include "LibScreen.h"
#include "ReadScreen.h"
// Prompt de comando tem tamanho 24x80.

WINDOW* controls;
void (*showcontrols)(MENUTYPE);
void (*killall)(void);
void (*clearscr)(void);
char* (*getEndereco)(void);

void mostra_controles(int);
void pair_colors(void);
void KillAll(void);
void ClearScr(void);
char* endereco(void);

Vigil* vigil;
LibScreen*  libscr;
ReadScreen* readscr;

int main()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);

    if(has_colors()) pair_colors();
    showcontrols = &mostra_controles;
    killall = &KillAll;
    clearscr = &ClearScr;
    getEndereco = &endereco;
    controls = newwin(0, 80, 23, 0);
    libscr = new LibScreen();
    readscr = new ReadScreen();

    refresh();

    while(true)
    {
        // Pede o livro
        libscr->init();
        mostra_controles(MENU_LIBRARY);
        wrefresh(controls);
        libscr->update();

        // Mostra o livro
        readscr->init();
        mostra_controles(MENU_READER);
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
        mvwprintw(controls, 0, 10, "F2");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 14, "Gerenciar");

        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 25, "F3");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 29, "Pesquisa & Ordem");
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
    case MENU_MANAGE:
        wattron(controls, A_REVERSE);
        mvwprintw(controls, 0, 0, "ENTER");
        wattroff(controls, A_REVERSE);
        mvwprintw(controls, 0, 7, "Voltar");
        break;
    }
    wrefresh(controls);
}

void pair_colors(void)
{
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
}

void KillAll(void)
{
    delete libscr;
    delete readscr;
}

void ClearScr(void)
{
    for(int i = 0; i < COLS; i++)
        for(int j = 0; j < LINES; j++)
            mvprintw(j, i, " ");
    refresh();
}

char* endereco(void)
{
    xml_document<> doc;
    xml_node<> *node;
    ifstream menu ("./temp/META-INF/container.xml");
    vector<char> buffer ((istreambuf_iterator<char>(menu)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    node = doc.first_node("container");
    xml_node<> *endereco_node = node->first_node("rootfiles")->first_node("rootfile");
    char* end = new char[255];
    strcpy(end, "./temp/");
    strcat(end, endereco_node->first_attribute("full-path")->value());
    return end;
}
