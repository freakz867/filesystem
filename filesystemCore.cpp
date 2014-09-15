/* 
 * File:   filesystemCore.cpp
 * Author: z
 * 
 * Created on 7 Ноябрь 2013 г., 0:53
 */

#include "filesystemCore.h"
#include "secFile.h"
#include <sstream>

Q_DECLARE_METATYPE(std::string)

string stringViewAccess(unsigned char state){
    string res="For Users: \n";
    if ((state & (1<<5)) != 0 ){
        res+="read ";
    }
    if ((state & (1<<4)) != 0 ){
        res+=", write ";
    }
    if ((state & (1<<3)) != 0 ){
        res+=", execute ";
    }
    res+="; \nFor Others: \n";
    if ((state & (1<<2)) != 0 ){
        res+="read ";
    }
    if ((state & (1<<1)) != 0 ){
        res+=", write ";
    }
    if ((state & (1<<0)) != 0 ){
        res+=", execute";
    }
    return res;
}

filesystemCore::filesystemCore(const int restoreFlag, QObject *parent): QAbstractListModel(parent){
    if(userTable.empty())
    {
        root=new catalog;
        userTableElem usrTblAdm={0,"adm",""};
        userTable.insert(std::pair<int,userTableElem>(1,usrTblAdm));
        freeTable=new Allocator;
        authorizedUID=-1;
    }
    if(restoreFlag==1)
    {
        this->restoreState();
        for(int i=0;i<currentCatalog->count();++i)
        {
            string Name=currentCatalog->getNameByKey(i);
            QString qstr=QString::fromStdString(Name);
            stringList.push_back(qstr);
        }
    }

}

int filesystemCore::rowCount(const QModelIndex &parent) const
{
    return stringList.count();
}

QVariant filesystemCore::data(const QModelIndex &index, int role) const
 {
     if (!index.isValid())
         return QVariant();

     if (index.row() >= stringList.size())
         return QVariant();

     if (role == Qt::DisplayRole || role == Qt::EditRole)
         return stringList.at(index.row());
     else
         return QVariant();
 }

QVariant filesystemCore::headerData(int section, Qt::Orientation orientation,
                                     int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
        return QString("Column %1").arg(section);
    else
        return QString("Row %1").arg(section);
}


filesystemCore::filesystemCore() {
    if(userTable.empty())
    {
        root=new catalog;
        userTableElem usrTblAdm={0,"ADMIN","mypass"};
        userTable.insert(std::pair<int,userTableElem>(1,usrTblAdm));
        freeTable=new Allocator;
    }
    

}

filesystemCore::filesystemCore(const filesystemCore& orig) {

}

filesystemCore::~filesystemCore() {
    currentCatalog=NULL;
    root->setParentCatalog(NULL);
    delete root;
    userTable.clear();
    delete freeTable;
}

int filesystemCore::getAuthorizedUID() const{return authorizedUID;}

int filesystemCore::getUserGroup(int UID) const{
    for(map<int,userTableElem>::const_iterator itr=userTable.begin(); itr!=userTable.end(); ++itr)
    {
        if((*itr).first==UID)
            return ((*itr).second.userGroup);
    }
    return -1;  //если UID не существует
}

bool filesystemCore::startUserSession(string loginUsername,string authorizationKey){
    for(map<int,userTableElem>::iterator itr=userTable.begin(); itr!=userTable.end(); ++itr)
    {
        if((*itr).second.Username==loginUsername && (*itr).second.Key==authorizationKey)
        {
            authorizedUID=(*itr).first;
            return 1;
        }
    }
    return 0;
}

bool filesystemCore::endUserSession(){
    if(authorizedUID>0)
    {
        cout<<"Goodbye, "<<*(*this).getUsername(authorizedUID)<<endl;
        authorizedUID=-1;       //никто не вошел в систему
        return 1;
    }
    currentCatalog=NULL;
    return 0;
}

const string *filesystemCore::getUsername(int UID)const{
    //стандартный код поиска путём прохода всего контейнера. Используется map, так как таблица пользователей не будет большой - в пределах 10 пользователей
   for(map<int,userTableElem>::const_iterator itr=userTable.begin(); itr!=userTable.end(); ++itr)
    {
        if((*itr).first==UID)
            return &((*itr).second.Username);
    }
}

bool filesystemCore::Secured_addUser(const userTableElem* usrTblNew){
    map<int,userTableElem>::iterator itr=userTable.end();
    int UID=userTable.size()+1;
    userTable.insert(std::pair<int,userTableElem>(UID,*usrTblNew));
    return 1;
}

bool filesystemCore::addUser(int userGroup, string* Username, string* Key){
    if(authorizedUID==1)
    {
    userTableElem newUser={userGroup,*Username,*Key};
    if((*this).Secured_addUser(&newUser))
        return 1;
    else
        return 0;
    }
    else
        return 0;
}

bool filesystemCore::Secured_deleteUser(int UID){
    if(UID>1 && UID<userTable.size()+1)
    {
        userTable.erase(UID);
        return 1;
    }
    else
        return 0;
}

bool filesystemCore::deleteUser(int UID){
    if(authorizedUID==1)
    {
        (*this).Secured_deleteUser(UID);
        return 1;
    }
    else
        return 0;
}

bool filesystemCore::viewStatistic() const{
    
}

int filesystemCore::getUID(string username){
    for(map<int,userTableElem>::const_iterator itr=userTable.begin(); itr!=userTable.end(); ++itr)
     {
         if((*itr).second.Username==username)
             return (*itr).first;
     }
    return -1;
}

void filesystemCore::readUsersTable(ifstream &disk){
    userTable.clear();
    int size=0;
    disk.read((char *)&size,sizeof(int));

    for(int i=0;i<size;++i)
    {
        int group=-1,sizeName;
        disk.read((char *)&group,sizeof(int));

        disk.read((char *)&sizeName,sizeof(int));
        char *username=new char [sizeName+1];
        disk.read(username,sizeName+1);

        disk.read((char *)&sizeName,sizeof(int));
        char *key=new char [sizeName+1];
        disk.read(key,sizeName+1);
        string usrName(username);
        string usrKey(key);
        userTableElem user={group,usrName,usrKey};
        userTable.insert(std::pair<int,userTableElem>(i+2,user));
    }

}

void filesystemCore::saveUsersTable()const{
    ofstream disk("filesystem.disk",ios::out  | ios::app | ios::binary);
    int size=userTable.size()-1;        //без админа
    disk.write((char*)&size,sizeof(int));
    for(map<int,userTableElem>::const_iterator itr=userTable.begin(); itr!=userTable.end(); ++itr)
    {
        if((*itr).first!=1){
        int group=(*itr).second.userGroup;
        disk.write((char*)&group,sizeof(int));

        const char *tempUserName = (*itr).second.Username.c_str();
        int lengthName=strlen(tempUserName);
        disk.write((char*)&lengthName,sizeof(lengthName));
        disk.write(tempUserName,lengthName+1);

        const char *tempKey = (*itr).second.Key.c_str();
        int lengthName2=strlen(tempKey);
        disk.write((char*)&lengthName2,sizeof(lengthName2));
        disk.write(tempKey,lengthName2+1);
        }
    }
        disk.close();
}



const userTableElem *filesystemCore::Secured_viewUserDetails(int &UID) const{
    for(map<int,userTableElem>::const_iterator itr=userTable.begin(); itr!=userTable.end(); ++itr)
    {
        if((*itr).first==UID)
        { 
                return &((*itr).second);
        }
    }
    return NULL;
}

const userTableElem *filesystemCore::viewUserDetails(int &UID) const{
    if(authorizedUID==1 )
    {
        for(map<int,userTableElem>::const_iterator itr=userTable.begin(); itr!=userTable.end(); ++itr)
        {
                if((*itr).first==UID)
                {
                    return (*this).Secured_viewUserDetails(UID);
                }
        }
        return NULL;
    }
    return NULL;
}

catalog *filesystemCore::getRootCatalog() const{
    return root;
}

catalog *filesystemCore::getCurrentCatalog() const{
    return currentCatalog;
}

filesystemCore &filesystemCore::setCurrentCatalog(catalog *newCurrentCatalog){
    currentCatalog=newCurrentCatalog;
}

Allocator *filesystemCore::getFreeTable() const{
    return freeTable;
}

filesystemCore &filesystemCore::addCatalog(string name,int group,int size){
    catalog *newCatalog=new catalog;
    newCatalog->setSize(size);
    if(currentCatalog->add(newCatalog,name,group)==-1)
    {
        emit error("Error adding to catalog: you have not permissions for this action");
    }
}

filesystemCore &filesystemCore::addFile(string name,int group,int size){
    File *newFile=new File;
    newFile->setSize(size);
    newFile->create(authorizedUID,getUserGroup(authorizedUID));
    if(currentCatalog->add(newFile,name,group)==-1)
        emit error("Error adding to catalog: you have not permissions for this action");
}


void filesystemCore::saveState(){
    ofstream filesystemFile("filesystem.disk");
    if(filesystemFile.is_open()) 
        filesystemFile.close();
    remove("filesystem.disk");
    root->saveAll("/");
    ofstream dataFile("data.disk");
    if(dataFile.is_open()) 
        dataFile.close();
    remove("data.disk");
    if(!rename("temp_data.disk","data.disk"))
        cout<<"Successful saved"<<endl;
}

void filesystemCore::restoreState(){
    delete root;
    root=new catalog;
   ifstream disk("filesystem.disk",ios::binary|ios::in);
   int type;
   disk.read((char *)&type,sizeof(int));
   root->readAll(disk);
   currentCatalog=root;
   disk.close();
}

int filesystemCore::editFile(string filename,char *data){
    File *editingFile=currentCatalog->findFile(filename);
    if(editingFile){
        int changeSize=editingFile->edit(data,0,freeTable,1,"main");
        catalog *temp=currentCatalog;
        while(temp!=NULL){
            temp->setSize(temp->getSize()+changeSize);
            temp=temp->getParentCatalog();
        }
    }
    else
        throw "File not found";
}

void filesystemCore::transformIntoSecuredFile(string filename){
    File *oldFile=currentCatalog->findFile(filename);
    if(oldFile && oldFile->getOwnerUID()==authorizedUID){
        int filesize=oldFile->getSize();
        const char *data=new char [filesize];
        data=oldFile->view((*this).getUserGroup(authorizedUID)).c_str();
        int dataSize=strlen(data);
        secFile *securedFile=new secFile;
        securedFile->setAccess(oldFile->getAccess());
        securedFile->setOwnerUID(oldFile->getOwnerUID());
        securedFile->create(authorizedUID,(*this).getUserGroup(authorizedUID));
        securedFile->createSecured(data,dataSize,0,freeTable,(*this).Secured_viewUserDetails(authorizedUID)->Username,(*this).Secured_viewUserDetails(authorizedUID)->Key);
        currentCatalog->erase(filename,2,(*this).getUserGroup(authorizedUID));
        currentCatalog->add(securedFile,filename,(*this).getUserGroup(authorizedUID));
    }
    else
        throw "You have not permission for this access OR this file is existing";
}

void filesystemCore::transformIntoFile(string filename){
    secFile *oldFile=dynamic_cast<secFile *>(currentCatalog->findFile(filename));
    if(oldFile && oldFile->getOwnerUID()==authorizedUID){
        char *data=(char*)oldFile->viewSecured((*this).Secured_viewUserDetails(authorizedUID)->Username,(*this).Secured_viewUserDetails(authorizedUID)->Key,freeTable);
        File *newFile=new File;
        newFile->create(authorizedUID,(*this).getUserGroup(authorizedUID));
        currentCatalog->erase(filename,3,(*this).getUserGroup(authorizedUID));
        currentCatalog->add(newFile,filename,(*this).getUserGroup(authorizedUID));
        (*this).editFile(filename,data);
        
    }
    else
        throw "You have not permission for this access OR this file is existing";
}

void filesystemCore::open(const QModelIndex &index){
    string name=currentCatalog->getNameByKey(index.row());
    QString filename=QString::fromStdString(name);
    int type=currentCatalog->find(name)->iAm();
    if(type==1)
    {
        currentCatalog=currentCatalog->downToCatalog(name);
        stringList.clear();
        emit layoutChanged();
        for(int i=0;i<currentCatalog->count();++i)
        {
            string Name=currentCatalog->getNameByKey(i);
            QString qstr=QString::fromStdString(Name);
            stringList.push_back(qstr);
        }
    }
    if(type==2){
        File *currentFile=currentCatalog->findFile(name);
        QString result=QString::fromStdString(currentFile->view(getUserGroup(authorizedUID)));
        unsigned char access=currentFile->getAccess();
        switch(getUserGroup(authorizedUID))
        {
        case 0:
        {
            emit fileIsOpened(filename,result,1);
        }
            break;
        case 1:
        {
            if(Users_viewAccessForWrite(access))
                emit fileIsOpened(filename,result,1);
            else
                emit fileIsOpened(filename,result,0);
        }
            break;
        case 2:
        {
            if(Others_viewAccessForWrite(access))
                emit fileIsOpened(filename,result,1);
            else
                emit fileIsOpened(filename,result,0);
        }
            break;

    default: {
            emit fileIsOpened(filename,result,0);}
        }
    }
    if(type==3){

    }
}

void filesystemCore::goToParentCatalog(){
    if(currentCatalog->getParentCatalog()!=NULL)
    {
        currentCatalog=currentCatalog->getParentCatalog();
        stringList.clear();
        emit layoutChanged();
        for(int i=0;i<currentCatalog->count();++i)
        {
            string Name=currentCatalog->getNameByKey(i);
            QString qstr=QString::fromStdString(Name);
            stringList.push_back(qstr);
        }
    }
}

void filesystemCore::model_addCatalog(QString filename){
    this->addCatalog(filename.toStdString(),getUserGroup(authorizedUID),0);
    stringList.clear();
    emit layoutChanged();
    for(int i=0;i<currentCatalog->count();++i)
    {
        string Name=currentCatalog->getNameByKey(i);
        QString qstr=QString::fromStdString(Name);
        stringList.push_back(qstr);
    }
}

void filesystemCore::model_addFile(QString filename){
    this->addFile(filename.toStdString(),getUserGroup(authorizedUID),0);
    stringList.clear();
    emit layoutChanged();
    for(int i=0;i<currentCatalog->count();++i)
    {
        string Name=currentCatalog->getNameByKey(i);
        QString qstr=QString::fromStdString(Name);
        stringList.push_back(qstr);
    }
}

void filesystemCore::model_login(QStringList loginData){
    string login=loginData.at(0).toStdString();
    QString logged="Currently logged: "+QString::fromStdString(login);
    string pass=loginData.at(1).toStdString();
    if(this->startUserSession(login,pass)==1)
        emit currentlyLogged(logged);

}

void filesystemCore::model_saveState(){
    if(authorizedUID>0){
        this->saveState();
    }
    else
        emit error("Error saving filesystem state: no one user is logged");
}

void filesystemCore::model_restoreState(){
    if(authorizedUID>0){
        this->restoreState();
        stringList.clear();
        emit layoutChanged();
        for(int i=0;i<currentCatalog->count();++i)
        {
            string Name=currentCatalog->getNameByKey(i);
            QString qstr=QString::fromStdString(Name);
            stringList.push_back(qstr);
        }
    }
    else
        emit error("Error saving filesystem state: no one user is logged");
}

void filesystemCore::editFileByIndex(const QModelIndex& index){
    string name=currentCatalog->getNameByKey(index.row());
    File *editFile=currentCatalog->findFile(name);
    editFile->view(0);
}

void filesystemCore::model_editFile(QString filename, QString data){
    this->editFile(filename.toStdString(),const_cast<char *>(data.toStdString().c_str()));
}

void filesystemCore::model_getType(const QModelIndex &index){
    if(stringList.size()!=0){
    string name=currentCatalog->getNameByKey(index.row());
    fsElem *current=currentCatalog->find(name);
    emit typeDefined(current->iAm());
    }
    else
        emit typeDefined(0);
}

void filesystemCore::model_getFileInfo(const QModelIndex &index){
    string name=currentCatalog->getNameByKey(index.row());
    File *currentFile=currentCatalog->findFile(name);
    QString info,size;
    date create=currentFile->getTimeCreated(),modification=currentFile->getTimeModified();
    stringstream ss;
    ss<<"info about file:"<<endl<<"Size: "<<currentFile->getSize()<<endl<<"; Owner: "<<*this->getUsername(currentFile->getOwnerUID())<<endl<<"; Date of creation: "
     <<create.hour<<":"<<create.minute<<":"<<create.second<<" "<<create.day<<"."<<create.month<<"."<<create.year<<endl<<"; Date of modification: "
       <<modification.hour<<":"<<modification.minute<<":"<<modification.second<<" "<<modification.day<<"."<<modification.month<<"."<<modification.year<<endl
      <<"; "<<stringViewAccess(currentFile->getAccess())<<endl;
    emit fileInfo(QString::fromStdString(ss.str()));
}

void filesystemCore::model_prepareCopyItem(const QModelIndex &index){
    string name=currentCatalog->getNameByKey(index.row());
    fsElem *current=currentCatalog->find(name);
    if(current)
        emit itemCopy_prepared(current);
    else
        throw "Element not found";
}

void filesystemCore::model_prepareReplaceItem(const QModelIndex &index){
    string name=currentCatalog->getNameByKey(index.row());
    fsElem *current=currentCatalog->find(name);
    if(current)
        emit itemReplace_prepared(currentCatalog,name,current->iAm());
    else
        throw "Element not found";
}

void filesystemCore::model_copy(fsElem *elemForAdd, QString newName){
    string tempName=newName.toStdString();
    if(elemForAdd)
    {
        currentCatalog->copy(elemForAdd,tempName,this->getUserGroup(authorizedUID));
        stringList.clear();
        emit layoutChanged();
        for(int i=0;i<currentCatalog->count();++i)
        {
            string Name=currentCatalog->getNameByKey(i);
            QString qstr=QString::fromStdString(Name);
            stringList.push_back(qstr);
        }

    }
    emit copyFinished();
}

void filesystemCore::model_replace(catalog *replaceParent, string replaceName, int replaceType){
    if(replaceParent)
    {
        replaceParent->replace(replaceName,currentCatalog,replaceType,this->getUserGroup(authorizedUID));
        stringList.clear();
        emit layoutChanged();
        for(int i=0;i<currentCatalog->count();++i)
        {
            string Name=currentCatalog->getNameByKey(i);
            QString qstr=QString::fromStdString(Name);
            stringList.push_back(qstr);
        }
    }
    emit replaceFinished();
}

void filesystemCore::model_Delete(const QModelIndex &index){

    string name=currentCatalog->getNameByKey(index.row());
    fsElem *current=currentCatalog->find(name);
    if(current)
    {
        currentCatalog->erase(name,current->iAm(),this->getUserGroup(authorizedUID));
        stringList.clear();
        emit layoutChanged();
        for(int i=0;i<currentCatalog->count();++i)
        {
            string Name=currentCatalog->getNameByKey(i);
            QString qstr=QString::fromStdString(Name);
            stringList.push_back(qstr);
        }
    }

    else
        throw "Element not found";
}

void filesystemCore::model_prepareRenameItem(const QModelIndex &index){
    string name=currentCatalog->getNameByKey(index.row());
    emit renameItem_prepared(name);
}

void filesystemCore::model_rename(string oldName, string newName){
    currentCatalog->rename(oldName,newName,this->getUserGroup(authorizedUID));
    stringList.clear();
    emit layoutChanged();
    for(int i=0;i<currentCatalog->count();++i)
    {
        string Name=currentCatalog->getNameByKey(i);
        QString qstr=QString::fromStdString(Name);
        stringList.push_back(qstr);
    }
}

void filesystemCore::model_endUserSession(){
    this->endUserSession();
    emit logout();
}

void filesystemCore::ask_addUser(){
    bool isAdmin=false;
    if(this->getUserGroup(authorizedUID)==0)
        isAdmin=true;
    emit CurrentUserAdmin(isAdmin);
}

void filesystemCore::model_addNewUser(int group, string filename, string key){
    this->addUser(group,&filename,&key);
}

void filesystemCore::model_deleteUser(string username){
    int deletingUID=this->getUID(username);
    if(deletingUID>1)
        this->deleteUser(deletingUID);
}
