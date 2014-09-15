#ifndef FILE_H
#define	FILE_H
#include "fsElem/fsElem.h"
#include "stream.h"
#include <sys/time.h>
#include "allocator.h"

class File:public fsElem {
public:

    File();
    File(streamsTable *);
    File(const File& orig);
    virtual ~File();
    virtual File *clone()const;    
    File &setTimeCreated(date);
    date getTimeCreated()const;
    File &setTimeModified(date);
    date getTimeModified()const;
    File &setOwnerUID(int);
    int getOwnerUID()const;
    
    int openForRead(int group)const;
    int openForWrite(int group)const;
    int viewAccess()const;
    int close();
    string view(int);
    int edit(const char *,int,Allocator *,int,string);
    int create(int,int);
    int info()const;
    int type()const;
    int changeAccess(int,int,bool,bool,bool);
    virtual int iAm()const;
    virtual int saveToDisk(string);
    virtual string readFromDisk(ifstream&);
    void rewriteToFile();
    
protected:
    date Tcreated,Tmodified;
    int ownerUID;
    streamsTable *strTbl;
};

#endif	/* FILE_H */

