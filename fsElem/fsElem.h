#ifndef FSELEM_H
#define	FSELEM_H

#include <QList>
#include <QVariant>
#include <QVector>

#include <iostream>
#include <fstream> 
#include <bitset>
#include <cstring>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include "myVector.h"
#include <map>
#include <utility>
using namespace std;

//класс-ядро файловой системы, на его основе строятся каталоги и файлы
class fsElem {
public:
    fsElem();
    fsElem(unsigned char,int);
    fsElem(const fsElem& orig);
    virtual ~fsElem();
    fsElem &Others_setAccess(bool,bool,bool);
    fsElem &Users_setAccess(bool,bool,bool);
    unsigned char getAccess()const;
    fsElem &setSize(int);
    fsElem &setAccess(unsigned char);
    int getSize()const;
    virtual int iAm()const=0;
    virtual fsElem *clone()const=0;
    virtual int saveToDisk(string )=0;

private:
    unsigned char access;        //побитовые флаги
    int size;           //размер каталога/файла
    bool setTrue(int);
    bool setFalse(int);  

};

struct date{
    int second,minute,hour;  //second,minute,hour
    int day,month,year;  //day,month,year
    date(int second0=0, int minute0=0, int hour0=0, int day0=1, int month0=1, int year0=2013):second(second0),minute(minute0),hour(hour0),day(day0),month(month0),year(year0){
        if(second0<0 || second0>59)
            throw "Error time";
        if(minute0<0 || minute0>59)
            throw "Error time";
        if(hour0<0 || hour0>23)
            throw "Error time";
        //расширить до проверки числел в каждом месяце, в общем случае пока 31 день
        if(day0<0 || day0>31)
            throw "Error time";
        if(month0<0 || month0>23)
            throw "Error time";
        if(year0<1900)
            throw "Error time";
    }
    int getCurrentTime();
    void saveToDisk(ofstream&)const;
    void readFromDisk(ifstream&);
};


bool Others_viewAccessForRead(unsigned char);
bool Others_viewAccessForWrite(unsigned char);
bool Others_viewAccessForExecute(unsigned char);
bool Users_viewAccessForRead(unsigned char);
bool Users_viewAccessForWrite(unsigned char);
bool Users_viewAccessForExecute(unsigned char);
#endif	/* FSELEM_H */





