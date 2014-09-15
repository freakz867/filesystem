/* 
 * File:   secFile.cpp
 * Author: z
 * 
 * Created on 29 Октябрь 2013 г., 0:56
 */
#include <time.h>
#include "secFile.h"
#include "file.h"
#include "crypt.h"


secFile::secFile() {
}

secFile::secFile(const secFile& orig):File(orig) {
    for(int i=0;i<orig.allowedUsers.size();++i)
    {
        int temp=orig.allowedUsers[i];
        allowedUsers.push_back(temp);
    }
}

secFile::~secFile() {
    allowedUsers.clear();
}

int secFile::iAm() const{
    return 3;
}

int secFile::saveToDisk(string name){
    cout<<name<<endl<<endl;
    ofstream disk("filesystem.disk",ios::out  | ios::app | ios::binary);
        int temp=3;
        disk.write((char*)&temp,sizeof(int));
        unsigned char t=(*this).getAccess();
        disk.write((char*)&t,sizeof(t));
        temp=(*this).getSize();
        disk.write((char*)&temp,sizeof(int));
        const char *tempName = name.c_str();
        int lengthName=strlen(tempName);
        disk.write((char*)&lengthName,lengthName);
        disk.write(tempName,lengthName+1);
        //пишем потоки
        Tcreated.saveToDisk(disk);
        Tmodified.saveToDisk(disk);
        disk.write((char*)&ownerUID,sizeof(int));
        strTbl->saveToDisk(disk);
        //пишем таблицу пользователей
        int vectorSize=allowedUsers.size();
        disk.write((char *)&vectorSize,sizeof(int));
        for(int i=0;i<vectorSize;++i){
            int UID=allowedUsers[i];
            disk.write((char*)&UID,sizeof(int));
        }
        disk.close(); 
    
}

string secFile::readFromDisk(ifstream &disk){
        int type,size,childCount,sizeName;
        unsigned char access;
        disk.read((char *)&access,sizeof(char));
        disk.read((char *)&size,sizeof(int));
        disk.read((char *)&sizeName,sizeof(int));
        char *name=new char [sizeName+1];
        disk.read(name,sizeName+1);
        (*this).setSize(size);
        
        //далее заполняем дополнительной информацией
        int vectorSize;
        disk.read((char *)&vectorSize,sizeof(int));
        for(int i=0;i<vectorSize;++i){
                disk.read((char *)&sizeName,sizeof(int));
                char *streamName=new char [sizeName+1];
                disk.read(streamName,sizeName+1);
                int address,type;
                disk.read((char *)&type,sizeof(int));
                disk.read((char *)&address,sizeof(int));
                streamDescriptor *newStream=new streamDescriptor((const char *)streamName,type,address);
                strTbl->add(newStream);
        }

        int UID;
        disk.read((char *)&vectorSize,sizeof(int));
        for(int i=0;i<vectorSize;++i){
            disk.read((char *)&UID,sizeof(int));
            allowedUsers.push_back(UID);
        }
        return (const char*)name;
}

secFile *secFile::clone() const{
    return new secFile(*this);
}

void secFile::add(int UID,int group, Allocator *freeTable,filesystemCore *fs){
    if(fs->authorizedUID==(*this).getOwnerUID())
    {
        crypto Cryptor;
        stream currentStream;
        string ownerUsername=fs->viewUserDetails(ownerUID)->Username;
        string newUsername=fs->viewUserDetails(UID)->Username;
        char *ownerKey=const_cast<char *>(fs->viewUserDetails(ownerUID)->Key.c_str());
        char *userKey=const_cast<char *>(fs->viewUserDetails(UID)->Key.c_str());
        //чтение и дешифрование случайного ключа
        int address=strTbl->findByName(ownerUsername);
        int infoSize=16;
        char *s=new char [16];
        currentStream.read(address,infoSize);
        s=currentStream.getData();
        char *decryptedRandKey=Cryptor.decrypt(s,ownerKey);
        //ключ расшифрован
        char *encryptedRandKey=Cryptor.encrypt(decryptedRandKey,userKey);
                streamDescriptor *newStream=new streamDescriptor(newUsername,2,-1);
                strTbl->add(newStream);
                (*this).edit(encryptedRandKey,group,freeTable,2,newUsername);
        allowedUsers.push_back(UID);
    }
    else
        throw "You have not permissions for access to this file";
}

void secFile::erase(int UID,int group, Allocator *freeTable,filesystemCore *fs){
    Vector<int>::iterator itr;
    for(itr=allowedUsers.begin(); itr!=allowedUsers.end();++itr)
    {
        if((*itr)==UID)
        {
            allowedUsers.erase(itr);
            --itr;
            //cout<<"size of vector: "<<streams.size()<<endl;
        }
    }
    string deletingUsername=fs->viewUserDetails(UID)->Username;
    int address=(*this).strTbl->findByName(deletingUsername);
    freeTable->addFreeBlock(address,16);
    (*this).strTbl->erase(2,deletingUsername);
}
char *secFile::generateRandKey(){
    char *randKey=new char [16];            //генерация случайного ключа
        char k;
        ifstream tempFile("/dev/random",ios::binary | ios::in);
        for(int i=0;i<16;++i)
        {
            tempFile.read(&k,1);
            if(k==0)
                k=112 %(i+1);  //перекрываем нуль байт
            else
                k=abs((int)k);
            randKey[i]=k;
        }
        cout<<randKey<<endl;
        tempFile.close();
        return randKey;
}

int secFile::shredder(char *data,int size){
    int j=0;
    char p;
    while(j<4)
    {
        if(j%2)
            p='0';
        else
            p='1';
        for(int i=0;i<size;++i){
            data[i]=p;
        }
        ++j;
    }
}

//создание зашифрованного файла
int secFile::createSecured(const char* data,int dataSize,int group,Allocator *freeTable,string username,string key){
        char *randKey=new char [16];
        randKey=generateRandKey();
        crypto Cryptor;
        char *encryptedData=new char [dataSize];
        encryptedData=Cryptor.encrypt(data,randKey);
        cout<<"encryptedData: "<<encryptedData<<endl;

        (*this).edit(encryptedData,group,freeTable,1,"main");
        (*this).getOwnerUID();
        char *ckey=const_cast<char *>(key.c_str());
        char *encryptedRandKey=new char [16];
        encryptedRandKey=Cryptor.encrypt(randKey,ckey);
        cout<<"encryptedRandKey: "<<encryptedRandKey<<endl;
        streamDescriptor *newStream=new streamDescriptor(username,2,-1);
        strTbl->add(newStream);
        (*this).edit(encryptedRandKey,group,freeTable,2,username);
        shredder(randKey,16);
}

const char * secFile::viewSecured(string username, string key,Allocator *freeTable){
        crypto Cryptor;
        stream currentStream;
        int address=strTbl->findByName(username);
        char *ckey=const_cast<char *>(key.c_str());
        int infoSize=16;
        char *s=new char [16];
        currentStream.read(address,infoSize);
        s=currentStream.getData();
        cout<<"encryptedRandKey: "<<s<<endl;
        char *decryptedRandKey=new char [16];
        decryptedRandKey=Cryptor.decrypt(s,ckey);
        cout<<"decryptedRandKey: "<<decryptedRandKey<<endl;
        
        address=strTbl->findByType(1);
        infoSize=(*this).getSize();
        char *s2=new char [infoSize];
        currentStream.read(address,infoSize);
        s2=currentStream.getData();
        char *decryptedData=new char [infoSize];
        decryptedData=Cryptor.decrypt(s2,decryptedRandKey);
        cout<<"decryptedData: "<<decryptedData<<endl;
        shredder(decryptedRandKey,16);
        return decryptedData;
}

int secFile::editSecured(const char *data, int group, Allocator *freeTable,filesystemCore *fs){
        char *randKey=new char [16];
        randKey=generateRandKey();
        crypto Cryptor;
        int ownerUID=(*this).getOwnerUID();
        char *encryptedData=Cryptor.encrypt(data,randKey);
        (*this).edit(encryptedData,group,freeTable,1,"main");
        //пишем все symkey для каждого пользователя после шифротекста основых данных
        string ownerUsername=fs->viewUserDetails(ownerUID)->Username;
        char *ownerKey=const_cast<char *>(fs->viewUserDetails(ownerUID)->Key.c_str());
        char *encryptedRandKey=new char [16];
        encryptedRandKey=Cryptor.encrypt(randKey,ownerKey);
                streamDescriptor *newStream=new streamDescriptor(ownerUsername,2,-1);
                strTbl->erase(2,ownerUsername);           //удаляем поток MAIN
                strTbl->add(newStream);
                (*this).edit(encryptedRandKey,group,freeTable,2,ownerUsername);
                //закончили запись нового шифротекста для нового ключа для хозяина файла
        for(int i=0;i<allowedUsers.size();++i)
        {
                char *ckey=const_cast<char *>(fs->viewUserDetails(allowedUsers[i])->Key.c_str());
                char *encryptedRandKey=new char [16];
                encryptedRandKey=Cryptor.encrypt(randKey,ckey);
                streamDescriptor *newStream=new streamDescriptor(fs->viewUserDetails(allowedUsers[i])->Username,2,-1);
                strTbl->erase(2,fs->viewUserDetails(allowedUsers[i])->Username);           //удаляем поток MAIN
                strTbl->add(newStream);
                (*this).edit(encryptedRandKey,group,freeTable,2,fs->viewUserDetails(allowedUsers[i])->Username);
        }
        shredder(randKey,16);
}
