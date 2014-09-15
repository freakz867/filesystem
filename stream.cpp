#include "stream.h"

stream::stream():data(NULL){
}

stream::stream(const stream& orig) {
    data=new char [strlen(orig.data)+1];
    data=orig.data;
}

stream::~stream() {
    delete [] data;
}

int stream::read(int address,int filesize){
     data=new char [filesize];
     ifstream disk("data.disk",ios::binary|ios::in);
     disk.seekg(address);
        disk.read(data,filesize);
     disk.close();
     return 1;
}

int stream::write(const char *filename,int address,const char *data,int filesize){
    int newAddress=address;
     //Открываем файл в двоичном режиме для записи
        if(address==-1)
        {
            ofstream disk(filename,ios::out  | ios::app | ios::binary);
                newAddress=disk.tellp();
                disk.write((char*)data,filesize);
                disk.close();
        }
        else
        {
            fstream disk(filename,ios::out | ios::in | ios::ate | ios::binary);
            disk.seekp(address);
            disk.write((char*)data,filesize);
                disk.close();
        }
    (*this).free();
    return newAddress;
}

char* stream::getData() const{
    return data;
}

void stream::free(){
    delete [] data;
    data=NULL;
}

streamDescriptor::streamDescriptor():Name(""),Type(0),Address(0){
    
}

streamDescriptor::streamDescriptor(const streamDescriptor& orig):Type(orig.Type),Address(orig.Address){
    Name+=orig.Name;
}

streamDescriptor::streamDescriptor(string Name0, int Type0, int Address0):Name(Name0),Address(Address0){
    if(Type0!=1 && Type0!=2 && Type0!=3)
        throw "Error type for stream";
    Type=Type0;
}




int streamDescriptor::getType() const{
    return Type;
}

int streamDescriptor::getAddress() const{
    return Address;
}

string streamDescriptor::getName() const{
    return Name;
}

streamDescriptor &streamDescriptor::setAddress(int Address0){
    Address=Address0;
    return *this;
}

streamDescriptor &streamDescriptor::setName(string Name0){
    Name=Name0;
    return *this;
}

streamDescriptor &streamDescriptor::setType(int Type0){
    if(Type0!=1 || Type0!=2 || Type0!=3)
        throw "Error type for stream";
    Type=Type0;
    return *this;
}
streamDescriptor::~streamDescriptor(){

}

streamsTable::streamsTable(){
    
}

streamsTable::streamsTable(const streamsTable& orig){
    for(int i=0;i<orig.streams.size();++i)
    {
        streamDescriptor *newStream=new streamDescriptor(orig.streams[i]);
        streams.push_back(*newStream);
    }
}

streamsTable::~streamsTable(){
    streams.clear();
    //или пройтись по вектору с erase
}

streamsTable *streamsTable::copy(const streamsTable& orig,int fileSize){
    stream currentStream;
    for(int i=0;i<orig.streams.size();++i)
    {
        if(orig.streams[i].getType()==1)
                currentStream.read(orig.streams[i].getAddress(),fileSize);
        else
            currentStream.read(orig.streams[i].getAddress(),16);

        int newAddress=currentStream.write("data.disk",-1,currentStream.getData(),strlen(currentStream.getData()));
        streamDescriptor *newStream=new streamDescriptor(orig.streams[i].getName(),orig.streams[i].getType(),newAddress);
        streams.push_back(*newStream);
    }
}

streamsTable &streamsTable::add(streamDescriptor *newStreamDescr){
    streams.push_back(*newStreamDescr);
}

int streamsTable::findByType(int type)const{
    for(int i=0;i<streams.size();++i)
    {
        if(streams[i].getType()==type)
                return streams[i].getAddress();
    }
}

int streamsTable::findByName(string Name) const{
    for(int i=0;i<streams.size();++i)
    {
        if(streams[i].getName()==Name)
                return streams[i].getAddress();
    }
}

streamsTable &streamsTable::erase(int type,string name){
    Vector<streamDescriptor>::iterator itr;
    for(itr=streams.begin(); itr!=streams.end();++itr)
    {
        if((*itr).getType()==type && (*itr).getName()==name)
        {
            streams.erase(itr);
            --itr;
            //cout<<"size of vector: "<<streams.size()<<endl;
            return *this;
        }
    }
}

void streamsTable::saveToDisk(ofstream &disk) const{
    int vectorSize=streams.size();
    disk.write((char *)&vectorSize,sizeof(int));
    for(int i=0;i<vectorSize;++i){
        const char *tempName = streams[i].getName().c_str();
        int lengthName=strlen(tempName);
        disk.write((char*)&lengthName,sizeof(lengthName));
        disk.write(tempName,lengthName+1);
        int address,type;
        address=streams[i].getAddress();
        type=streams[i].getType();
        disk.write((char*)&type,sizeof(int));
        disk.write((char*)&address,sizeof(int));
    }
}

void streamsTable::rewriteToNewDisk(int fileSize){
    stream currentStream;
    int streamSize=streams.size();
    for(int i=0;i<streamSize;++i)
    {
        int length;
        char *temp;
        if(streams[i].getType()==1){
                currentStream.read(streams[i].getAddress(),fileSize);
                length=fileSize;
                temp=new char[fileSize];
        }
        else{
            currentStream.read(streams[i].getAddress(),16);
            length=16;
            temp=new char[16];
        }
        temp=currentStream.getData();
        int newAddress=currentStream.write("temp_data.disk",-1,temp,length);
        streamDescriptor *newStream=new streamDescriptor(streams[i].getName(),streams[i].getType(),newAddress);
        this->erase(streams[i].getType(),streams[i].getName());
        streams.push_back(*newStream);
    }

}
