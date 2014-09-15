#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <Qt>
#include <QMainWindow>
#include "filename_dialog.h"
#include "login_dialog.h"
#include "ui_mainwindow.h"
#include "error_dialog.h"
#include "info_dialog.h"
#include "fileviewdialog.h"
#include "adduser_dialog.h"
#include "deleteuser_dialog.h"
#include <QMainWindow>
#include <QModelIndex>
#include "filesystemCore.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    QAction *myAction;
    int currentItemType;    //тип выбранного элемента, необходимо для создания разных контекстных меню
    fsElem *pasteItem;int pasteItemType;
    string itemName;
    bool replaceFlag=false,adminLogged;     //false=режим копирования, true=режим перемещения

public slots:

private slots:
    void createAddCatalogDialog();
    void createAddFileDialog();
    void createLoginDialog();
    void setCurrentType(int);
    void createCatalog(QString);
    void createFile(QString);
    void login(QStringList);
    void createErrorDialog(QString);
    void createInfoDialog(QString);
    void createFileViewDialog(QString,QString,int);
    void createEditFileDialog();
    void editFile(QString,QString);
    void ShowContextMenu(const QPoint&);
    void infoAboutFile();
    void fileType();
    void fileGetPermissions();
    void fileChangePermissions();
    void copyItem();
    void setDataForCopy(fsElem*);
    void createCopyFileDialog();
    void pasteItemName_prepared(QString);
    void replaceItem();
    void setDataForReplace(catalog*,string,int);
    void deleteItem();
    void renameItem();
    void newItemName_prepared(QString);
    void setNewItemName(string);
    void createRenameDialog();
    void setNobobyLogged();
    void createNewUserDialog();
    void setAdminLogged(bool);
    void addUser(QStringList);
    void createDeleteUserDialog();
    void deleteUser(QString);
signals:
    void catalogName_done(QString);
    void fileName_done(QString);
    void loginDetails_isGetted(QStringList);
    void prepareToEditFile(const QModelIndex &);
    void editFile_signal(QString,QString);
    void whatType(const QModelIndex &);
    void infoAboutFile_Index(const QModelIndex &);
    void fileType_signal(QString);
    void fileGetPermissions_Index(const QModelIndex &);
    void fileChangePermissions_Index(const QModelIndex &);
    void prepareCopyItem_index(const QModelIndex &);
    void prepareReplaceItem_index(const QModelIndex &);
    void pasteItem_preparedFull(fsElem*,QString);
    void pasteItemReplaced_prepared(catalog*,string,int);
    void prepareDeleteItem_index(const QModelIndex &);
    void rename_prepared(string,string);
    void prepareRenameItem_index(const QModelIndex &);
    void renameItem_prepared(string,string);
    void isCurrentUserAdmin();
    void newUserDetails_prepared(int,string,string);
    void deleteUserName_prepared(string);
};


#endif // MAINWINDOW_H
