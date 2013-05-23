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
    int ch;

    struct Book
    {
        char* filename;
        char* filepath;
        char* title;
        char* author;
        char* publisher;
    };

    Book* books;
    char b_name[255], b_author[48], b_publisher[48];

    SearchScreen* srchscr;
    ManageScreen* mngscr;

    void printMenuElement(const char*, int);
    void clipFilename(char[]);
    void clipAttribute(char[]);
    void clearAttributeField(int);
    void getBookList(void);
    void delBookList(void);
public:
    LibScreen();
    ~LibScreen();
    void init();
    int update();
    void refresh();
};

#endif // LIBSCREEN_H_INCLUDED
