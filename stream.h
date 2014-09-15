#ifndef STREAM_H
#define	STREAM_H
#include "fsElem/fsElem.h"
#include <fstream>



//поток MAIN
class stream{
public:
    stream();
    stream(const stream& orig);
    virtual ~stream();
    int read(int,int);
    int write(const char *,int,const char *,int);
    char *getData()const;
    void free();
private:
    char *data;  //буфер
};


//описатель потока
class streamDescriptor{
private:
    string Name;        //имя потока
    int Type;           //тип потока
    int Address;                //адрес на диске
public:
    streamDescriptor();
    streamDescriptor(string,int,int);
    streamDescriptor(const streamDescriptor&);
    ~streamDescriptor();
    streamDescriptor &setName(string);
    string getName()const;
    streamDescriptor &setType(int);
    int getType()const;
    streamDescriptor &setAddress(int);
    int getAddress()const;
};

//таблица потоков
class streamsTable{
private:
    Vector<streamDescriptor> streams;    //массив описателей
public:
    streamsTable();
    streamsTable(const streamsTable& orig);
    ~streamsTable();
    streamsTable *copy(const streamsTable& orig,int);
    int findByType(int)const;     //поиск по типу потока, выдача адреса
    int findByName(string)const;    //поиск по имени потока, выдача адреса
    streamsTable &add(streamDescriptor *);
    streamsTable &erase(int,string);
    void saveToDisk(ofstream&)const;
    void rewriteToNewDisk(int);

    
};


#endif	/* STREAM_H */

