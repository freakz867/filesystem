#include "catalog.h"
#include "file.h"
#include "secFile.h"

const char *Types[]={"","Catalog","File","Secured file"};
catalog::catalog():address(0),catalogStructure(NULL),parentCatalog(NULL) {
    catalogStructure=new catalogStruct;
}

//конструктор с полной инициализацей всего класса
catalog::catalog(int address,const catalogStruct& newCatalogStruct, catalog* newParentCatalog):address(0),catalogStructure(NULL),parentCatalog(newParentCatalog){
    //catalogStructure=new catalogStruct [1];
    parentCatalog=new catalog;
    catalogStructure->copy(newCatalogStruct,this);
}


catalog::catalog(const catalog& orig):fsElem(orig),address(orig.address){
    cout<<"Create parentCatalog "<<parentCatalog<<endl;
    catalogStructure=catalogStructure->copy(*(orig.catalogStructure),this);
    cout<<endl<<"Create catalogStructure   "<<catalogStructure<<endl;
}

catalog *catalog::clone() const{
    return new catalog(*this);
}

catalog::~catalog() {
    cout<<"deleting catalog "<<this<<endl;
    delete catalogStructure;
}


catalogStruct::catalogStruct(){}

catalogStruct::catalogStruct(const catalogStruct& orig){
    for(map<string,fsElem *>::const_iterator itr= orig.structTable.begin();itr!=orig.structTable.end();++itr)
    {
        if((*itr).second->iAm()==1)
        {
                cout<<"Copying catalog "<<(*itr).first<<" with "<<(*itr).second->getSize()<<endl;
                catalog *temp=dynamic_cast<catalog *>((*itr).second->clone());
                structTable.insert(std::pair<string,fsElem *>((*itr).first,temp));
                cout<<"Successful copied : ";

        }
        if((*itr).second->iAm()==2)
        {
            File *temp=dynamic_cast<File *>((*itr).second->clone());
            structTable.insert(std::pair<string,fsElem *>((*itr).first,temp));
                cout<<"Successful copied : ";
        }
        if((*itr).second->iAm()==3)
        {
            secFile *temp=dynamic_cast<secFile *>((*itr).second->clone());
            structTable.insert(std::pair<string,fsElem *>((*itr).first,temp));
                cout<<"Successful copied : ";
        }
    }
}

catalogStruct *catalogStruct::copy(const catalogStruct &orig,fsElem *newparentCatalog){
    catalogStruct *newCatalogStruct=new catalogStruct;
    for(map<string,fsElem *>::const_iterator itr= orig.structTable.begin();itr!=orig.structTable.end();++itr)
    {
        if((*itr).second->iAm()==1)
        {
                cout<<"Copying catalog "<<(*itr).first<<" with "<<(*itr).second->getSize()<<endl;;
                catalog *temp=dynamic_cast<catalog *>((*itr).second->clone());
                catalog *temp2=dynamic_cast<catalog *>(newparentCatalog);
                temp->setParentCatalog(temp2);
                newCatalogStruct->structTable.insert(std::pair<string,fsElem *>((*itr).first,temp));
                cout<<"Successful copied : ";          
        }
        if((*itr).second->iAm()==2)
        {
            
            File *temp=dynamic_cast<File *>((*itr).second->clone());
            newCatalogStruct->structTable.insert(std::pair<string,fsElem *>((*itr).first,temp));
                cout<<"Successful copied : ";
        }
        if((*itr).second->iAm()==3)
        {
            secFile *temp=dynamic_cast<secFile *>((*itr).second->clone());
            newCatalogStruct->structTable.insert(std::pair<string,fsElem *>((*itr).first,temp));
                cout<<"Successful copied : ";
        }
    }
    return newCatalogStruct;
    
}

catalogStruct::~catalogStruct(){
    cout<<endl;
    cout<<"deleting catalogStruct "<<this<<endl;
    (*this).viewAll();
    for(map<string,fsElem *>::iterator itr=structTable.begin();itr!=structTable.end();++itr)
{
        delete (*itr).second;
        structTable.erase(itr);
}
}

catalogStruct *catalogStruct::add(fsElem *elem, string Name){
    structTable.insert(std::pair<string,fsElem *>(Name,elem));
}

int catalogStruct::viewAll(){
    for( map<string,fsElem *>::iterator itr=structTable.begin(); itr!=structTable.end(); ++itr)
    {
	cout<<(*itr).first<<" | "<<Types[(*itr).second->iAm()]<<"| size:"<<(*itr).second->getSize()<<endl;
    }
    return 1;
}

//поиск файла/папки (зависит от типа, 1 - папка,2 - файл,3 - защищённый файл )
fsElem *catalogStruct::findByName(string Name, int type){
    //поиск файла перебором контейнера, решить проблему с использованием equal_range()
    for( map<string,fsElem *>::iterator itr=structTable.begin(); itr!=structTable.end(); ++itr)
    {
        if((*itr).first==Name && (*itr).second->iAm()==type)
            
            return (*itr).second;
    }
    return NULL;
}

fsElem *catalogStruct::findByName(string Name){
    //поиск файла перебором контейнера, решить проблему с использованием equal_range()
    for( map<string,fsElem *>::iterator itr=structTable.begin(); itr!=structTable.end(); ++itr)
    {
        if((*itr).first==Name)

            return (*itr).second;
    }
    return NULL;
}

int catalogStruct::rename(string TargetName, string NewName){
    for( map<string,fsElem *>::iterator itr=structTable.begin(); itr!=structTable.end(); ++itr)
    {
        if((*itr).first==TargetName)
        {
            fsElem *Copy=(*itr).second;
            structTable.erase(itr);
            structTable.insert(std::pair<string,fsElem *>(NewName,Copy));
        }
    }
}

catalogStruct *catalogStruct::viewAllCatalog(string path){
    cout<<endl<<path<<endl;
    (*this).viewAll();
    for( map<string,fsElem *>::iterator itr=structTable.begin(); itr!=structTable.end(); ++itr)
    {
        catalog *temp=dynamic_cast<catalog *>((*itr).second);        
        if(temp)
        {
            cout<<endl<<endl<<"address of parentCatalog"<<temp->getParentCatalog()<<endl;
            temp->viewAll(path+"/"+(*itr).first);
        }
    }
}

catalogStruct *catalogStruct::saveAllCatalog(){
    for( map<string,fsElem *>::iterator itr=structTable.begin(); itr!=structTable.end(); ++itr)
    {
        if((*itr).second->iAm()==1)
        {
            catalog *temp=dynamic_cast<catalog *>((*itr).second);
            if(temp)
            {
                temp->saveAll((*itr).first);
            }
        }
        if((*itr).second->iAm()==2)
        {
            File *temp=dynamic_cast<File *>((*itr).second);
            //перезапись в новый файл
            temp->rewriteToFile();
            temp->saveToDisk((*itr).first);
        }
        if((*itr).second->iAm()==3)
        {
            secFile *temp=dynamic_cast<secFile *>((*itr).second);
            //перезапись в новый файл
            temp->rewriteToFile();
            temp->saveToDisk((*itr).first);
        }
        
        
    }
}

catalogStruct *catalogStruct::erase(string TargetName){
    for( map<string,fsElem *>::iterator itr=structTable.begin(); itr!=structTable.end(); ++itr)
    {
        if((*itr).first==TargetName)
        {
            delete (*itr).second;
            structTable.erase(itr);
        }
    }
}

int catalogStruct::count() const{
    return structTable.size();
}

pair<string,int> catalogStruct::childNum(int num)const{
    if(num>structTable.size()){}
        //ничего не делать
    else
    {
        int i=0;
        for( map<string,fsElem *>::const_iterator itr=structTable.begin(); itr!=structTable.end(); ++itr)
        {
            if(i==num)
                return std::pair<string,int>((*itr).first,(*itr).second->iAm());
            ++i;
        }
    }

}

int catalog::iAm() const{return 1;}

void catalog::viewAll(string path){
    cout<<"currentcatalog: "<<this<<endl;
    (*this).catalogStructure->viewAllCatalog(path);
}

catalog &catalog::setParentCatalog(catalog *newParentCatalog){parentCatalog=newParentCatalog;}

int catalog::view(int group) const{
    //проверка на чтение
    switch (group){
        case 0:
        {
            cout<<"catalogStructure: "<<catalogStructure<<endl;
            (*this).catalogStructure->viewAll();
        }
        break;
        case 1:
        {
            if(!Users_viewAccessForRead((*this).getAccess())) 
                throw "You have not permission for this action";
            (*this).catalogStructure->viewAll();
        }
        break;
        case 2:
        {
            if(!Others_viewAccessForRead((*this).getAccess())) 
                throw "You have not permission for this action";
            (*this).catalogStructure->viewAll();
        }
        break;
        default:
        {
            throw "Existing user group";
        }
    }
}

int catalog::add(fsElem *elemForAdd,string Name,int group){
    switch (group){
        case 0:
        {
            if(elemForAdd->iAm()==1)
            {
                catalog *newCatalog=dynamic_cast<catalog *>(elemForAdd);
                newCatalog->setParentCatalog(this);      
            }
            (*this).catalogStructure->add(elemForAdd,Name);
            
        }
        break;
        case 1:
        {
            if(!Users_viewAccessForWrite((*this).getAccess())) 
                throw "You have not permission for this action";
            if(elemForAdd->iAm()==1)
            {
                catalog *newCatalog=dynamic_cast<catalog *>(elemForAdd);
                newCatalog->setParentCatalog(this);
                
            }
            (*this).catalogStructure->add(elemForAdd,Name);
        }
        break;
        case 2:
        {
            if(!Others_viewAccessForWrite((*this).getAccess())) 
                throw "You have not permission for this action";
            if(elemForAdd->iAm()==1)
            {
                catalog *newCatalog=dynamic_cast<catalog *>(elemForAdd);
                newCatalog->setParentCatalog(this);
                
            }
            (*this).catalogStructure->add(elemForAdd,Name);
        }
        break;
        default:
        {
            return -1;
        }
    }
}

int catalog::rename(string targetName, string newName, int group){
    switch (group){
        case 0:
        {
                catalogStructure->rename(targetName,newName);
        }
        break;
        case 1:
        {
            if(!Users_viewAccessForWrite((*this).getAccess())) 
                throw "You have not permission for this action";
            catalogStructure->rename(targetName,newName);
        }
        break;
        case 2:
        {
            if(!Others_viewAccessForWrite((*this).getAccess())) 
                throw "You have not permission for this action";
            catalogStructure->rename(targetName,newName);
        }
        break;
        default:
        {
            throw "Existing user group";
        }
    }
}

int catalog::copy(fsElem* elemForAdd,string Name,int group){
    bool flag=false;
    switch(group){
    case 0:
    {flag=true;}
        break;
    case 1:
    {if(Users_viewAccessForWrite((*this).getAccess()))
        flag=true;
        }
        break;
    case 2:
    {if(!Others_viewAccessForWrite((*this).getAccess()))
        flag=true;
        }
        break;
    default:{}
    }

    if(elemForAdd->iAm()==1 && flag)
            {
                catalog *newCatalog=dynamic_cast<catalog *>(elemForAdd);
                if(newCatalog)
                {
                catalog *temp=newCatalog->clone();
                temp->setParentCatalog(this);
                cout<<"consist of temp catalog:"<<endl;
                temp->viewAll("temp");
                (*this).catalogStructure->add(temp,Name);
                int changeSize=temp->getSize();
                catalog *tempCat=this;
                while(tempCat!=NULL){
                        tempCat->setSize(tempCat->getSize()+changeSize);
                        tempCat=tempCat->getParentCatalog();
                }
                }
            }
    if(elemForAdd->iAm()==2 && flag)
    {
        File *newFile=dynamic_cast<File *>(elemForAdd);
        if(newFile){
            File *temp=newFile->clone();
            this->catalogStructure->add(temp,Name);
            int changeSize=temp->getSize();
            catalog *tempCat=this;
            while(tempCat!=NULL){
                tempCat->setSize(tempCat->getSize()+changeSize);
                tempCat=tempCat->getParentCatalog();
            }
        }
    }
    if(elemForAdd->iAm()==3 && flag)
    {
        secFile *newFile=dynamic_cast<secFile *>(elemForAdd);
        if(newFile){
            secFile *temp=newFile->clone();
            this->catalogStructure->add(temp,Name);
            int changeSize=temp->getSize();
            catalog *tempCat=this;
            while(tempCat!=NULL){
                tempCat->setSize(tempCat->getSize()+changeSize);
                tempCat=tempCat->getParentCatalog();
            }
        }
    }
    return 1;
}

int catalog::replace(string Name, catalog *Target,int type, int group){
    bool flag=false;
    switch(group){
    case 0:
    {flag=true;}
        break;
    case 1:
    {if(Users_viewAccessForWrite((*this).getAccess()))
        flag=true;
        }
        break;
    case 2:
    {if(!Others_viewAccessForWrite((*this).getAccess()))
        flag=true;
        }
        break;
    default:{}
    }

    if(flag){
    fsElem *temp;
    temp=catalogStructure->findByName(Name,type);
    Target->catalogStructure->add(temp,Name);
    catalogStructure->erase(Name);
    int changeSize=temp->getSize();
    catalog *tempCat=Target;
    while(tempCat!=NULL){
             tempCat->setSize(tempCat->getSize()+changeSize);
             tempCat=tempCat->getParentCatalog();
    }
    changeSize=-temp->getSize();
    tempCat=this;
    while(tempCat!=NULL){
             tempCat->setSize(tempCat->getSize()+changeSize);
             tempCat=tempCat->getParentCatalog();
    }
    }
}

int catalog::erase(string Name,int type, int group){
    bool flag=false;
    switch(group){
    case 0:
    {flag=true;}
        break;
    case 1:
    {if(Users_viewAccessForWrite((*this).getAccess()))
        flag=true;
        }
        break;
    case 2:
    {if(!Others_viewAccessForWrite((*this).getAccess()))
        flag=true;
        }
        break;
    default:{}
    }

    fsElem *temp;
    if(temp && flag)
    {
        temp=catalogStructure->findByName(Name,type);
        catalogStructure->erase(Name);
        int changeSize=-temp->getSize();
        catalog *tempCat=this;
        while(tempCat!=NULL){
             tempCat->setSize(tempCat->getSize()+changeSize);
             tempCat=tempCat->getParentCatalog();
        }
    }
    else
    {
    }
    return 1;
}

catalog *catalog::downToCatalog(string Name){
    catalog *newCurrentCatalog=dynamic_cast<catalog *>(catalogStructure->findByName(Name,1));
    if(!newCurrentCatalog)
        throw "Error in operation go to catalog (bad_cast error)";
    cout<<newCurrentCatalog<<endl;
    return newCurrentCatalog;
}

catalog *catalog::upToCatalog(){
    return (*this).parentCatalog;
}

catalog *catalog::getParentCatalog(){
    return parentCatalog;
}

void catalog::saveAll(string name){
    (*this).saveToDisk(name);
    catalogStructure->saveAllCatalog();
}

string catalog::readAll(ifstream &disk){
        int type=0,size=0,childCount=0,sizeName=0;
        unsigned char access;
        disk.read((char *)&access,sizeof(char));
        disk.read((char *)&size,sizeof(int));
        disk.read((char *)&sizeName,sizeof(int));
        char *name=new char  [sizeName+1];
        disk.read(name,sizeName+1);
        disk.read((char *)&childCount,sizeof(int));
        //вызов конструктора с полученными данными
        (*this).setSize(size);
        (*this).setAccess(access);
        for(int i=0;i<childCount;++i){
            disk.read((char *)&type,sizeof(int));
            if(type==1)
            {
                catalog *newCatalog=new catalog();
                string tempname=newCatalog->readAll(disk);
                (*this).add(newCatalog,tempname,0);
            }
            if(type==2)
            {
                File *newFile=new File();
                string tempname=newFile->readFromDisk(disk);
                (*this).add(newFile,tempname,0);
            }
            if(type==3)
            {
                secFile *newFile=new secFile();
                string tempname=newFile->readFromDisk(disk);
                (*this).add(newFile,tempname,0);
            }
        }
        return (const char *)name;
}
int catalog::saveToDisk(string name){
    cout<<name<<endl<<endl;
        ofstream disk("filesystem.disk",ios::out  | ios::app | ios::binary);
        int temp=1;
        disk.write((char*)&temp,sizeof(int));
        unsigned char t=(*this).getAccess();
        disk.write((char*)&t,sizeof(t));
        temp=(*this).getSize();
        disk.write((char*)&temp,sizeof(int));
        const char *tempName = name.c_str();
        int lengthName=strlen(tempName);
        disk.write((char*)&lengthName,sizeof(lengthName));
        disk.write(tempName,lengthName+1);
        temp=catalogStructure->count();
        disk.write((char*)&temp,sizeof(temp));
        disk.close(); 
        
}

string catalog::readFromDisk(ifstream &disk){      
        int type,size,childCount,sizeName;
        unsigned char access;     
        disk.read((char *)&type,sizeof(int));
        disk.read((char *)&access,sizeof(char));
        disk.read((char *)&size,sizeof(int));
        disk.read((char *)&sizeName,sizeof(int));
        char *name=new char [sizeName+1];
        disk.read(name,sizeName+1);
        disk.read((char *)&childCount,sizeof(int));
        return (const char *)name;
}

File *catalog::findFile(string filename){
    File *editingFile=dynamic_cast<File *>(catalogStructure->findByName(filename,2));
    return editingFile;
}

fsElem *catalog::find(string filename)const{
    return catalogStructure->findByName(filename);
}

int catalog::count()const{
    return catalogStructure->count();
}

int catalog::removeAt(int index,int group){
    pair<string,int> res=catalogStructure->childNum(index);
    this->erase(res.first,res.second,group);

}

string catalog::getNameByKey(int index)const{
    pair<string,int> res=catalogStructure->childNum(index);
    return res.first;
}
