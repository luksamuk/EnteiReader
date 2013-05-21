/*
    VIGIL - MONITORADOR DE DIRETÓRIOS

    *O sistema de validação dos .epubs gera latência na execução do programa,
    *além disso, ele é mais restritivo do que o próprio leitor, uma vez que
    *diversos arquivos válidos foram considerados inválidos pelo mesmo.
    *Sendo assim, é recomendável que se utilize a função update
    *com o modo de validação desativado caso encontre problemas com relação ao
    *monitoramento de arquivos .epub já validados que o Vigil tenha problemas
    *durante o reconhecimento dos diretórios cadastrados.

    *Para realizar debugs, basta desfazer as anotações da função printinfo() e
    *sua implementação no .cpp.

    *Para iterar sobre os elementos, crie um objeto fileIndex e faça-o receber
    *o retorno da função library(). Esse método retorna uma lista encadeada com a id
    *e endereço de todos os .epubs, além de ter espaço para uma estrutura de dados <T>
    *para indexamento de informações sobre o arquivo.


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

#define MAXf 100    //Tamanho máximo de um nome de um arquivo.
#define MAXl 500    //Tamanho máximo de um endereço de um diretório.

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
    bool sys;           //Informa se o OS é Linux ou Windows.
    int dirCount;       //Quantidade de diretórios monitorados.
    int totalCount;     //Quantidade de .epubs monitorados.


    folder * dirList;   //A lista de diretórios monitorados contem uma lista de todos os arquivos .epub disponíveis no seu escopo.
    folder * lastDir;

    bool exportLib();			            //Exporta todos os diretórios monitorados para um arquivo backup.
    bool recoverLib();                      //Lê o arquivo e recupera a lista dos diretórios monitorados.
    bool isValid(const char * filename);    //Retorna se um arquivo .epub é válido ou não.
    bool isDir(const char * filename);      //Retorna se uma entidade é um diretório ou não.
    bool isMonitored(const char * dirname); //Retorna se o diretório já faz parte da biblioteca.
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
    bool addDir(const char * dirname);      //Adiciona um diretório à lista de diretórios monitorados.
    void update(bool validate = true);             //Método responsável por monitorar todos diretórios cadastrados.
                                            //Se bool validate == true, o programa só adiciona .epubs válidos à biblioteca.
    bool removeDir(const char * dirname);	//Remove um diretório da lista de diretórios monitorados.
    bool openFile(const char * file, bool overwrite = false);       //Extrai um arquivo zip para a pasta temporária.
    void clearDir();                        //Deleta todos os arquivos de um diretório especificado.
    int directoriesCount();
    int filesCount();
    fileIndex * library();
    void printinfo();
};



#endif // VIGIL_H_INCLUDED
