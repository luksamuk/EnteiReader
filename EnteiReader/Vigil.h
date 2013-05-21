/*
    VIGIL - MONITORADOR DE DIRET�RIOS

    *O sistema de valida��o dos .epubs gera lat�ncia na execu��o do programa,
    *al�m disso, ele � mais restritivo do que o pr�prio leitor, uma vez que
    *diversos arquivos v�lidos foram considerados inv�lidos pelo mesmo.
    *Sendo assim, � recomend�vel que se utilize a fun��o update
    *com o modo de valida��o desativado caso encontre problemas com rela��o ao
    *monitoramento de arquivos .epub j� validados que o Vigil tenha problemas
    *durante o reconhecimento dos diret�rios cadastrados.

    *Para realizar debugs, basta desfazer as anota��es da fun��o printinfo() e
    *sua implementa��o no .cpp.

    *Para iterar sobre os elementos, crie um objeto fileIndex e fa�a-o receber
    *o retorno da fun��o library(). Esse m�todo retorna uma lista encadeada com a id
    *e endere�o de todos os .epubs, al�m de ter espa�o para uma estrutura de dados <T>
    *para indexamento de informa��es sobre o arquivo.


*/
#ifndef VIGIL_H_INCLUDED
#define VIGIL_H_INCLUDED

#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <stdio.h>

#define WINDOWS 1
#define LINUX 0

#define MAXf 100    //Tamanho m�ximo de um nome de um arquivo.
#define MAXl 500    //Tamanho m�ximo de um endere�o de um diret�rio.

class Vigil
{
private:
    struct node
    {
        int id;
        char filename[MAXf];
        node * next;
        node * prev;
    };
    struct folder
    {
        node * files;
        folder * prev;
        folder * next;
        char location[MAXl];
        int fCount;
    };
    bool sys;           //Informa se o OS � Linux ou Windows.
    int dirCount;       //Quantidade de diret�rios monitorados.
    int totalCount;     //Quantidade de .epubs monitorados.


    folder * dirList;   //A lista de diret�rios monitorados contem uma lista de todos os arquivos .epub dispon�veis no seu escopo.
    folder * lastDir;

    bool exportLib();			            //Exporta todos os diret�rios monitorados para um arquivo backup.
    bool recoverLib();                      //L� o arquivo e recupera a lista dos diret�rios monitorados.
    bool isValid(const char * filename);    //Retorna se um arquivo .epub � v�lido ou n�o.
    bool isDir(const char * filename);      //Retorna se uma entidade � um diret�rio ou n�o.
    bool isMonitored(const char * dirname); //Retorna se o diret�rio j� faz parte da biblioteca.
    void destroy(node * &files);
    void destroy(folder * &dir);
public:
    struct fileIndex
    {
        fileIndex * next;
        fileIndex * prev;
        int id;
        char fileAddress[MAXl];
    };
    Vigil();
    ~Vigil();
    bool addDir(const char * dirname);      //Adiciona um diret�rio � lista de diret�rios monitorados.
    void update(bool validate = true);             //M�todo respons�vel por monitorar todos diret�rios cadastrados.
                                            //Se bool validate == true, o programa s� adiciona .epubs v�lidos � biblioteca.
    bool removeDir(const char * dirname);	//Remove um diret�rio da lista de diret�rios monitorados.
    bool openFile(const char * file, bool overwrite = false);       //Extrai um arquivo zip para a pasta tempor�ria.
    void clearDir();                        //Deleta todos os arquivos de um diret�rio especificado.
    int directoriesCount();
    int filesCount();
    fileIndex * library();
    void printinfo();
};



#endif // VIGIL_H_INCLUDED
