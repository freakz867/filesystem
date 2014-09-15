#ifndef FILESYSTEMCORE_H
#define	FILESYSTEMCORE_H

#include "fsElem/fsElem.h"
#include "catalog.h"
#include "allocator.h"
#include "file.h"
#include <QAbstractListModel>
#include <QVariant>
#include <QStringList>
#include <QString>

struct FSlistItem{
    string name;
    fsElem *ptr;
};

struct userTableElem{
    int userGroup;      //0 - ADMIN, 1 - USERS,2 - OTHERS
    string Username;
    string Key;
};

 class secFile;

class filesystemCore:public QAbstractListModel{
    Q_OBJECT
public:

    //QT
    filesystemCore(const int restoreFlag, QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    friend class secFile;
    filesystemCore();
    filesystemCore(const filesystemCore& orig);
    virtual ~filesystemCore();
    int getAuthorizedUID()const;
    const string *getUsername(int)const;
    int getUserGroup(int)const;
    bool startUserSession(string,string);
    bool endUserSession();
    bool addUser(int,string*,string*);
    bool deleteUser(int);
    const userTableElem *viewUserDetails(int&)const;
    bool viewStatistic()const;
    catalog *getRootCatalog()const;
    int getUID(string);
    catalog *getCurrentCatalog()const;
    filesystemCore &setCurrentCatalog(catalog *);
    Allocator *getFreeTable()const;
    void saveUsersTable()const;
    void readUsersTable(ifstream &);
    filesystemCore &addCatalog(string,int,int);
    filesystemCore &addFile(string,int,int);
    void saveState();
    void restoreState();
    int editFile(string,char*);
    void transformIntoSecuredFile(string);
    void transformIntoFile(string);

public slots:
    void open(const QModelIndex &index);
    void goToParentCatalog();
    void model_addCatalog(QString filename);
    void model_addFile(QString filename);
    void model_login(QStringList loginData);
    void model_saveState();
    void model_restoreState();
    void model_editFile(QString filename,QString data);
    void editFileByIndex(const QModelIndex &index);
    void model_getType(const QModelIndex &index);
    void model_getFileInfo(const QModelIndex &index);
    void model_prepareCopyItem(const QModelIndex &index);
    void model_prepareReplaceItem(const QModelIndex &index);
    void model_prepareRenameItem(const QModelIndex &index);
    void model_Delete(const QModelIndex &index);
    void model_copy(fsElem *elemForAdd,QString newName);
    void model_replace(catalog *replaceParent,string replaceName,int replaceType);
    void model_rename(string oldName,string newName);
    void model_endUserSession();
    void ask_addUser();
    void model_addNewUser(int group,string filename,string key);
    void model_deleteUser(string username);
signals:
    void currentlyLogged(QString);
    void error(QString);
    void fileIsOpened(QString,QString,int);
    void typeDefined(int);
    void fileInfo(QString);
    void itemCopy_prepared(fsElem*);
    void itemReplace_prepared(catalog*,string,int);
    void copyFinished();
    void replaceFinished();
    void itemDelete_prepared(string,int);
    void renameItem_prepared(string);
    void logout();
    void CurrentUserAdmin(bool);
    
private:
    catalog *currentCatalog;
    catalog *root;
    //string currentPath;
    map<int,userTableElem> userTable;
    Allocator *freeTable;
    int authorizedUID;
    bool Secured_addUser(const userTableElem *);
    bool Secured_deleteUser(int);
    const userTableElem *Secured_viewUserDetails(int&)const;
    QStringList stringList;
};

#endif	/* FILESYSTEMCORE_H */

