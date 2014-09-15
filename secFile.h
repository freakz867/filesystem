#ifndef SECFILE_H
#define	SECFILE_H

#include "filesystemCore.h"
#include "file.h"

class secFile:public File {
public:

    secFile();
    secFile(const secFile& orig);
    virtual ~secFile();
    virtual int iAm()const;
    virtual secFile *clone()const;
    char *generateRandKey();
    int shredder(char *,int);
    int createSecured(const char *,int,int,Allocator *,string,string);
    int editSecured(const char *,int,Allocator *,filesystemCore *);
    const char *viewSecured(string,string,Allocator *);
    int findByUID(int)const;     //поиск по имени потока, выдача адреса
    void add(int,int,Allocator *,filesystemCore *);
    void erase(int,int,Allocator *,filesystemCore *);
    virtual int saveToDisk(string);
    string readFromDisk(ifstream&);
private:
  Vector<int> allowedUsers;
};

#endif	/* SECFILE_H */

