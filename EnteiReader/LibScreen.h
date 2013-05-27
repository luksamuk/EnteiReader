#ifndef LIBSCREEN_H_INCLUDED
#define LIBSCREEN_H_INCLUDED

#include "stdafx.h"
#include "SearchScreen.h"
#include "ManageScreen.h"

#define LIBSCREEN_FILESFIELD_SIZE          30
#define LIBSCREEN_FILESFIELD_UTILSIZE      (LIBSCREEN_FILESFIELD_SIZE - 2)
#define LIBSCREEN_ATTRFIELD_SIZE           50
#define LIBSCREEN_ATTRFIELD_UTILSIZE       (LIBSCREEN_ATTRFIELD_SIZE - 2)
#define LIBSCREEN_ATTRFIELD_NAME           2
#define LIBSCREEN_ATTRFIELD_AUTHOR         5
#define LIBSCREEN_ATTRFIELD_PUBLISHER      8

class LibScreen
{
private:
    WINDOW* win, *attributes;
    int selection, n_books;
    int ch, curr_elem;

    struct Book
    {
        char* filename;
        char* filepath;
        char* title;
        char* author;
        char* publisher;
    };

    struct Node
    {
        int bookid;
        Book* book;
        Node *dir;
        Node *esq;
    };

    Book* books;
    Node* a;
    char b_name[255], b_author[48], b_publisher[48];

    SearchScreen* srchscr;
    ManageScreen* mngscr;

    void printMenuElement(const char*, int);
    void clipFilename(char[]);
    void clipAttribute(char[]);
    void clearAttributeField(int);
    void getBookList(void);
    void delBookList(void);

    char* getTitulo(char*);
    char* getAutores(char*);
    char* getEditora(char*);

    void insereElemento(Node*&, Book*, int);
    void printListRecursively(Node*);
public:
    LibScreen();
    ~LibScreen();
    void init();
    int update();
    void refresh();
};

#endif // LIBSCREEN_H_INCLUDED
