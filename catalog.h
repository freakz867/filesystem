#ifndef CATALOG_H
#define	CATALOG_H
#include "fsElem/fsElem.h"
#include "file.h"

class catalogStruct{
private:
    map<string,fsElem *> structTable;
public:
    catalogStruct();
    catalogStruct(const catalogStruct&);
    catalogStruct *copy(const catalogStruct&,fsElem *);
    ~catalogStruct();
    catalogStruct *add(fsElem*,string);
    int viewAll();
    catalogStruct *viewAllCatalog(string);
    catalogStruct *saveAllCatalog();
    fsElem *findByName(string,int);
    fsElem *findByName(string);
    int rename(string,string);
    catalogStruct *erase(string);
    int count()const;
    pair<string,int> childNum(int)const;
};

class catalog:public fsElem {
public:

    catalog();
    catalog(int,const catalogStruct &,catalog*);
    catalog(const catalog&);
    catalog &setParentCatalog(catalog *);
    virtual ~catalog();
    virtual catalog *clone()const;
    virtual int iAm()const;
    int view(int)const;
    int add(fsElem*,string,int);
    int rename(string,string,int);
    int copy(fsElem*,string,int);
    int replace(string,catalog *,int,int);
    int erase(string,int,int);
    catalog *downToCatalog(string);
    catalog *upToCatalog();
    void viewAll(string);
    catalog *getParentCatalog();
    void saveAll(string);
    string readAll(ifstream&);
    int saveToDisk(string);
    string readFromDisk(ifstream&);
    File *findFile(string);
    int count()const;
    int removeAt(int,int);
    string getNameByKey(int)const;
    fsElem *find(string)const;
private:
    int address;
    catalogStruct *catalogStructure;
    catalog *parentCatalog;
};


#endif	/* CATALOG_H */


