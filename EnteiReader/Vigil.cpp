#include "Vigil.h"

bool Vigil::exportLib()
{
    std::ofstream fout;
    fout.open("library.bin", std::ios::binary);
    if(fout.is_open())
    {
        folder * diraux = dirList;
        while(diraux!=NULL)
        {
            fout << diraux->location << "\n";
            diraux = diraux->next;
        }
        fout.close();
        return true;
    }
    else
        return false;
}
bool Vigil::recoverLib()
{
    std::ifstream fin;
    char buffer[MAXl];
    fin.open("library.bin");
    if(fin.is_open())
    {
        while(fin.good())
        {
            fin.getline(buffer, MAXl);
            addDir(buffer);
        }
        update(false);
        fin.close();
        return true;
    }
    else
        return false;
}
void Vigil::clearDir()
{
if (sys == WINDOWS)
    system("rd /s /q temp");
else if(sys == LINUX)
    system("rm -rf temp");

}

bool Vigil::openFile(const char * file, bool overwrite)
{
    if(!overwrite) clearDir();

    const char * dest = "temp";
    int cmdSize = (2*MAXl)+20;        //Verificar se o buffer é grande o suficiente.
    if((strlen(file)+strlen(dest) > (cmdSize -20)))
        return false;

    std::ifstream f (file, std::ios::binary);   //Verificar se o arquivo descrito existe.
    if(!f.is_open())
        return false;
    f.close();

    char cmd[cmdSize];
    char * execute;
    if(sys == LINUX)
        execute = "./7za x ";

    else if(sys == WINDOWS)
        execute = "7za.exe x ";

    else
        return false;

    sprintf(cmd, "%s%s -y -o%s", execute, file, dest);

    if(!system(cmd)) return true;
    return false;
}
bool Vigil::isValid(const char * filename)
{
    if (strlen(filename)>=MAXl) return false;

    char cmd[MAXl];
    if(sys == WINDOWS)  strcpy(cmd, "flightcrew-cli.exe");
    else if (sys == LINUX) strcpy(cmd,"./flightcrew-cli");

    sprintf(cmd, "%s %s", cmd, filename);

    return (system(cmd) == 0) ? true : false;
}
bool Vigil::isDir(const char * filename)
{
    DIR * dir;
    if((dir = opendir(filename))!=NULL)
    {
        closedir(dir);
        return true;
    }
    return false;
}
bool Vigil::isMonitored(const char * dirname)
{
    folder * aux = dirList;
    while (true)
    {
        if (aux == NULL) break;
        if (!strcmp(aux->location, dirname))
            return true;
        aux = aux->next;
    }
    return false;
}

void Vigil::destroy(node * &files)
{
    if(files != NULL)
    {
        if(files->next == NULL)
        {
            delete files;
            files = NULL;
        }
        else
        {
            node * i = files->next;
            delete files;
            files = NULL;

            while(i->next != NULL)
            {
                i = i->next;
                delete i->prev;
            }
            delete i;
        }
    }
}

void Vigil::destroy(folder * &dir)
{
    if(dir != NULL)
    {
        if(dir->next == NULL)
        {
            delete dir;
            dir = NULL;
            lastDir = NULL;
        }
        else
        {
            folder * i = dir->next;
            delete dir;
            dir = NULL;

            while(i->next != NULL)
            {
                i = i->next;
                delete i->prev;
            }
            delete i;
        }
    }
}

Vigil::Vigil()
{
#ifdef _WIN32
    sys = WINDOWS;
#endif
#ifdef linux
    sys = LINUX;
#endif

    dirList = NULL;
    dirCount = 0;
    totalCount = 0;
    recoverLib();
    system("mkdir temp");
    if(sys == LINUX) system("chmod +x 7za flightcrew-cli");
}
Vigil::~Vigil()
{
    exportLib();
    destroy(dirList);
    clearDir();
}

bool Vigil::addDir(const char * dirname)
{
    if (isDir(dirname) && !isMonitored(dirname))
    {
        if (dirList == NULL)
        {
            dirList = new folder;
            dirList->prev = NULL;
            lastDir = dirList;
        }

        else
        {
            lastDir->next = new folder;
            lastDir->next->prev = lastDir;
            lastDir = lastDir->next;
        }

        lastDir->files = NULL;
        lastDir->next = NULL;
        strcpy(lastDir->location, dirname);
        lastDir->fCount = 0;

        dirCount++;
        return true;
    }

    else
        return false;
}

void Vigil::update(bool validate)
{
    DIR * dir;
    dirent * ent;
    node * lastfile = NULL;
    folder * diraux = dirList;
    const char * ext = ".epub";
    bool val;

    totalCount = 0;

    while(diraux != NULL)
    {
        diraux->fCount = 0;

        if (isDir(diraux->location))
        {
            dir = opendir(diraux->location);
            int extLength = strlen(ext);
            destroy(diraux->files);

            while ((ent = readdir(dir)) != NULL)
            {
                int filenameLength = strlen(ent->d_name);

                if(filenameLength >= extLength)
                {
                    char filename[filenameLength], extension[extLength];

                    strcpy(filename, ent->d_name);
                    for(int i = 0; i < extLength + 1; i++)
                        extension[i] = filename[(strlen(filename) - extLength) + i];


                    if(!strcmp(extension, ext))
                    {
                        char faddress[MAXl];
                        sprintf(faddress, "%s/%s", diraux->location, filename);

                        val = true;
                        if(isDir(faddress)) val = false;

                        if(validate)
                            val = isValid(faddress);

                        if(val)
                        {
                            if (diraux->files == NULL)
                            {
                                diraux->files = new node;
                                diraux->files->prev = NULL;
                                lastfile = diraux->files;
                            }
                            else
                            {
                                lastfile->next = new node;
                                lastfile->next->prev = lastfile;
                                lastfile = lastfile->next;
                            }

                            strcpy(lastfile->filename, filename);
                            lastfile->next = NULL;
                            diraux->fCount++;
                            lastfile->id = ++totalCount;
                        }
                    }
                }
            }
        }

        diraux = diraux->next;
    }
}

bool Vigil::removeDir(const char * dirname)
{
    if(isDir(dirname))
    {
        folder * i = dirList;
        while (true)
        {
            if (i == NULL) return false;
            if (!strcmp(dirname, i->location))
                break;

            i = i->next;
        }

        destroy(i->files);

        if(i->prev != NULL && i->next != NULL)
        {
            i->prev->next = i->next;
            i->next->prev = i->prev;
        }
        else if(i->prev == NULL && i->next != NULL)
        {
            i->next->prev = NULL;
            dirList = i->next;
        }
        else if(i->prev != NULL && i->next == NULL)
            i->prev->next = NULL;
        else
            dirList = NULL;

        totalCount = totalCount - i->fCount;
        dirCount--;
        delete i;
        return true;
    }
    else
        return false;
}

int Vigil::directoriesCount()
{
    return dirCount;
}
int Vigil::filesCount()
{
    return totalCount;
}
Vigil::fileIndex * Vigil::library()
{
    fileIndex * ret = NULL;
    fileIndex * last = NULL;
    folder * diraux = dirList;
    node * file = NULL;

    if(dirList == NULL) return ret;
    else
    {
        while (true)
        {
            if(diraux == NULL) break;
            file = diraux->files;
            while(true)
            {
                if(file == NULL) break;

                if(ret == NULL)
                {
                    ret = new fileIndex;
                    ret->prev = NULL;
                    last = ret;
                }
                else
                {
                    last->next = new fileIndex;
                    last->next->prev = last;
                    last = last->next;
                }

                last->id = file->id;
                sprintf(last->fileAddress, "%s/%s", diraux->location, file->filename);
                last->next = NULL;

                file = file->next;
            }
            diraux = diraux->next;
        }
    }
    return ret;
}

void Vigil::printinfo()
{
    printf("\n-----FILES------\n");
    folder * aux = dirList;
    node * naux  = NULL;

    while(aux!=NULL)
    {
        printf("Directory: %s\n", aux->location);
        naux = aux->files;
        while(true)
        {
            if (naux == NULL) break;
            printf("\t%s\n", naux->filename);
            naux = naux->next;
        }
        printf("- %i Files available.\n\n",aux->fCount);
        aux = aux->next;
    }
    printf("\n--Total number of monitored directories...: %i"
           "\n-Total number of files currently addressed: %i\n\n", dirCount, totalCount);
}



