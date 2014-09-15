#include "fsElem.h"
#include "catalog.h"
//создаем пустой элемент с нулевым размером и запретом на любой доступ
fsElem::fsElem():size(0){
    access=0b00110100;
}
fsElem::fsElem(unsigned char access0=0b00000000, int size0=0):access(access0),size(size0){
    //проверки вводимых данных
}

fsElem::fsElem(const fsElem& orig):access(orig.access),size(orig.size){
    //проверки вводимых данных
}

fsElem::~fsElem(){
    cout<<"Deleting fsElem "<<size<<endl;
}

int fsElem::iAm() const{
    return 0;
}

int date::getCurrentTime(){

    time_t rawtime;
    struct tm *timeinfo;
    if(time(&rawtime) == -1) return 0;
    if((timeinfo = localtime(&rawtime)) == NULL)
    {
        throw "Can not get current time";
    }
    (*this).second=timeinfo->tm_sec;
    (*this).minute=timeinfo->tm_min;
    (*this).hour=timeinfo->tm_hour;
    (*this).day=timeinfo->tm_mday;
    (*this).month=timeinfo->tm_mon;
    (*this).year=timeinfo->tm_year+1900;
    return 1;
}

void date::saveToDisk(ofstream &disk)const{
    disk.write((char*)&second,sizeof(int));
    disk.write((char*)&minute,sizeof(int));
    disk.write((char*)&hour,sizeof(int));
    
    disk.write((char*)&day,sizeof(int));
    disk.write((char*)&month,sizeof(int));
    disk.write((char*)&year,sizeof(int));
}

void date::readFromDisk(ifstream &disk){
    disk.read((char *)&second,sizeof(int));
    disk.read((char*)&minute,sizeof(int));
    disk.read((char*)&hour,sizeof(int));
    
    disk.read((char*)&day,sizeof(int));
    disk.read((char*)&month,sizeof(int));
    disk.read((char*)&year,sizeof(int));
}
//установка байта доступа
fsElem &fsElem::Others_setAccess(bool a1,bool a2,bool a3){
    if(a1)
        setTrue(3);
    else
        setFalse(3);
    if(a2)
        setTrue(2);
    else
        setFalse(2);
    if(a3)
        setTrue(1);
    else
        setFalse(1);
    return *this;
}

fsElem &fsElem::Users_setAccess(bool a1,bool a2,bool a3){
    if(a1)
        setTrue(6);
    else
        setFalse(6);
    if(a2)
        setTrue(5);
    else
        setFalse(5);
    if(a3)
        setTrue(4);
    else
        setFalse(4);
    return *this;
}

unsigned char fsElem::getAccess()const{
    return access;
}

//установка размера каталога/файла
fsElem &fsElem::setSize(int a){
    size=a;
    return *this;
}

fsElem &fsElem::setAccess(unsigned char newAccess){
    access=newAccess;
    return *this;
}

int fsElem::getSize()const{
    return size;
}

bool fsElem::setTrue(int n){
    --n;
    if(n>2 || n<0)
        return 1;
    else{
        access=access | (1<<n);
        return 0;
    }     
}

bool fsElem::setFalse(int n){
    --n;
    if(n>2 || n<0)
        return 1;
    else{
        access=access & (~(1<<n));
        return 0;
    }     
}






bool Others_viewAccessForRead(unsigned char state){
    if ((state & (1<<2)) != 0 )
        return 1;
    else
        return 0;
}

bool Others_viewAccessForWrite(unsigned char state){
    if ((state & (1<<1)) != 0 )
        return 1;
    else
        return 0;
}

bool Others_viewAccessForExecute(unsigned char state){
    if ((state & (1<<0)) != 0 )
        return 1;
    else
        return 0;
}

bool Users_viewAccessForRead(unsigned char state){
    if ((state & (1<<5)) != 0 )
        return 1;
    else
        return 0;
}

bool Users_viewAccessForWrite(unsigned char state){
    if ((state & (1<<4)) != 0 )
        return 1;
    else
        return 0;
}

bool Users_viewAccessForExecute(unsigned char state){
    if ((state & (1<<3)) != 0 )
        return 1;
    else
        return 0;
}


