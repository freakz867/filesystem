#include "file.h"
#include "stream.h"


File::File():Tcreated(),Tmodified(),ownerUID(1){
    streamsTable *tbl=new streamsTable;
    strTbl=tbl;
}

File::File(streamsTable *ptr):strTbl(ptr){
}

File::File(const File& orig):fsElem(orig),Tcreated(orig.Tcreated),Tmodified(orig.Tmodified),ownerUID(orig.ownerUID) {
    strTbl=new streamsTable;
    strTbl->copy(*(orig.strTbl),(*this).getSize());
    
}

File *File::clone() const{
    return new File(*this);
}
File::~File() {
    cout<<"deleting file "<<this<<endl;
    delete strTbl;
}

int File::saveToDisk(string name){
    cout<<name<<endl<<endl;
    ofstream disk("filesystem.disk",ios::out  | ios::app | ios::binary);
        int temp=2;
        disk.write((char*)&temp,sizeof(int));
        unsigned char t=(*this).getAccess();
        disk.write((char*)&t,sizeof(t));
        temp=(*this).getSize();
        disk.write((char*)&temp,sizeof(int));
        const char *tempName = name.c_str();
        int lengthName=strlen(tempName);
        disk.write((char*)&lengthName,sizeof(lengthName));
        disk.write(tempName,lengthName+1);
        //запись стандартной части окончена,пишем даты,хозяина и потоки
        Tcreated.saveToDisk(disk);
        Tmodified.saveToDisk(disk);
        disk.write((char*)&ownerUID,sizeof(int));
        strTbl->saveToDisk(disk);
        disk.close();
}

string File::readFromDisk(ifstream &disk){
        int type,size,childCount,sizeName;
        unsigned char access;
        disk.read((char *)&access,sizeof(char));
        disk.read((char *)&size,sizeof(int));
        disk.read((char *)&sizeName,sizeof(int));
        char *name=new char [sizeName+1];
        disk.read(name,sizeName+1);
        (*this).setSize(size);          //далее заполняем дополнительной инфой, сначала временем и ownerUID
        Tcreated.readFromDisk(disk);
        Tmodified.readFromDisk(disk);
        disk.read((char *)&ownerUID,sizeof(int));
        int vectorSize;                 //потом потоки читаем
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
        return (const char*)name;
}

date File::getTimeCreated() const{
    return Tcreated;
}

date File::getTimeModified() const{
    return Tmodified;
}

int File::getOwnerUID() const{
    return ownerUID;
}


int File::openForWrite(int group)const{
    unsigned char access;
    access=(*this).getAccess();
    switch(group){
        case 0:
        {
            return 1;
        }
        break;
        case 1:
        {
            if(Users_viewAccessForWrite(access))
                return 1;
        }
        break;
        case 2:
        {
            if(Others_viewAccessForWrite(access))
                return 1;
        }
        break;
        default:
        {
            return 0;
        }
    }
    
}

int File::openForRead(int group)const{
    unsigned char access;
    access=(*this).getAccess();
    switch(group){
        case 0:
        {
            return 1;
        }
        break;
        case 1:
        {
            if(Users_viewAccessForRead(access))
                return 1;
        }
        break;
        case 2:
        {
            if(Others_viewAccessForRead(access))
                return 1;
        }
        break;
        default:
        {
            return 0;
        }
    }
}

int File::viewAccess() const{
    unsigned char access;
    access=(*this).getAccess();
    cout<<"For Others: "<<endl;
    if(Others_viewAccessForRead(access))
        cout<<"read ";       //есть права на чтение
    if(Others_viewAccessForWrite(access))
        cout<<"write ";       //есть права на чтение
    if(Others_viewAccessForExecute(access))
        cout<<"execute";       //есть права на чтение
    cout<<"For Users: "<<endl;
    if(Users_viewAccessForRead(access))
        cout<<"read ";       //есть права на чтение
    if(Users_viewAccessForWrite(access))
        cout<<"write ";       //есть права на чтение
    if(Users_viewAccessForExecute(access))
        cout<<"execute";       //есть права на чтение
}

string File::view(int group){
    unsigned char access;
    access=(*this).getAccess();
    switch(group)
    {
    case 0:
    {
        stream currentStream;
        int address=strTbl->findByType(1);
        int infoSize=(*this).getSize();
        string s;
        currentStream.read(address,infoSize);
        s=currentStream.getData();
        cout<<s<<endl;
        return s;
    }
        break;
    case 1:
    {
        if(Users_viewAccessForRead(access)){
            stream currentStream;
            int address=strTbl->findByType(1);
            int infoSize=(*this).getSize();
            string s;
            currentStream.read(address,infoSize);
            s=currentStream.getData();
            cout<<s<<endl;
            return s;
        }
    }
        break;
    case 2:
    {
        if(Others_viewAccessForRead(access)){
            stream currentStream;
            int address=strTbl->findByType(1);
            int infoSize=(*this).getSize();
            string s;
            currentStream.read(address,infoSize);
            s=currentStream.getData();
            cout<<s<<endl;
            return s;
        }
    }
        break;
    default:
        return "You have not permissions to open this file";
    }
}

int File::create(int newOwnerUID,int group){
    (*this).Tcreated.getCurrentTime();
    ownerUID=newOwnerUID;
    (*this).setSize(0);
    (*this).Users_setAccess(1,1,0);
    (*this).Others_setAccess(1,0,0);
    streamDescriptor *newStream=new streamDescriptor("main",1,-1);
    strTbl->add(newStream);
    return 1;
    
}

int File::edit(const char *data,int group,Allocator *freeTable,int streamType,string streamName){
    stream currentStream;
    int filesize=strlen(data);
    int address=strTbl->findByName(streamName);
    if(filesize>(*this).getSize())
    {
        if((*this).getSize()!=0 && address!=-1)
            freeTable->addFreeBlock(address,(*this).getSize());
        address=currentStream.write("data.disk",-1,data,filesize);
    }
    else
    {
        address=currentStream.write("data.disk",address,data,filesize);
        if((*this).getSize()-filesize!=0)
                freeTable->addFreeBlock(address+filesize,(*this).getSize()-filesize);
    }
    int change=filesize-(*this).getSize();
    if(streamType==1)
        (*this).setSize(filesize);
    streamDescriptor *newStream=new streamDescriptor(streamName,streamType,address);
    strTbl->erase(streamType,streamName);           //удаляем поток
    strTbl->add(newStream);
        (*this).Tmodified.getCurrentTime();
        return change;      //возвращаем изменение файла, знаковое
}

int File::info() const{
    cout<<"Size: "<<(*this).getSize()<<" Owner UID: "<<(*this).getOwnerUID()<<" Time Of Creation: "<<(*this).getTimeCreated().hour<<":"<<(*this).getTimeCreated().minute<<":"<<(*this).getTimeCreated().second<<endl;
}

int File::changeAccess(int UID,int group,bool a1,bool a2, bool a3){
    if(ownerUID==UID)
    {
        switch(group){
            case 1:
            {
                (*this).Users_setAccess(a1,a2,a3);
                return 1;
            }
            break;
            case 2:
            {
                (*this).Others_setAccess(a1,a2,a3);
                return 1;
            }
            break;
            default:
            {
                throw "Existing user group";
            }
        }
    }
    
}

int File::type() const{
    if (iAm()==2)
    cout<<"File"<<endl;
    else
       cout<<"Secured File"<<endl; 
    return iAm();
}

int File::iAm() const{return 2;}

void File::rewriteToFile(){
    strTbl->rewriteToNewDisk((*this).getSize());
    
}

File &File::setOwnerUID(int UID){
    ownerUID=UID;
    return *this;
}



