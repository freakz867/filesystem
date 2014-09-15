#include <QtGui>

 #include "mainwindow.h"

 MainWindow::MainWindow(QWidget *parent)
     : QMainWindow(parent)
 {
     setupUi(this);
        pasteButton->hide();

         QAbstractItemModel *model2 = new filesystemCore(1);
        FSlistView->setModel(model2);
        FSlistView->setGeometry(0,241,600,256);
        FSlistView->setContextMenuPolicy(Qt::ActionsContextMenu);
        FSlistView->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(FSlistView,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(ShowContextMenu(const QPoint&)));
        myAction = new QAction(tr("Edit"), this);
            myAction->setShortcut(tr("Ctrl+M"));
            myAction->setStatusTip(tr("My action demo"));
            connect(myAction, SIGNAL(triggered()), this, SLOT(createEditFileDialog()));
            FSlistView->addAction(myAction);
        loginStatusLabel->setText("Nobody logged in filesystem");



     connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));


     connect(FSlistView,SIGNAL(doubleClicked(QModelIndex)),model2,SLOT(open(QModelIndex)));
     connect(gotoParentCatalog_Button,SIGNAL(clicked()),model2,SLOT(goToParentCatalog()));
     connect(actionAdd_Catalog_2,SIGNAL(triggered()),this,SLOT(createAddCatalogDialog()));
     connect(actionAdd_File_2,SIGNAL(triggered()),this,SLOT(createAddFileDialog()));
     connect(this,SIGNAL(catalogName_done(QString)),model2,SLOT(model_addCatalog(QString)));
     connect(this,SIGNAL(fileName_done(QString)),model2,SLOT(model_addFile(QString)));
     connect(actionLogin,SIGNAL(triggered()),this,SLOT(createLoginDialog()));
     connect(this,SIGNAL(loginDetails_isGetted(QStringList)),model2,SLOT(model_login(QStringList)));
     connect(model2,SIGNAL(currentlyLogged(QString)),loginStatusLabel,SLOT(setText(QString)));
     connect(model2,SIGNAL(error(QString)),this,SLOT(createErrorDialog(QString)));
     connect(model2,SIGNAL(fileIsOpened(QString,QString,int)),this,SLOT(createFileViewDialog(QString,QString,int)));
     connect(actionSave_FileSystem,SIGNAL(triggered()),model2,SLOT(model_saveState()));
     connect(actionLoad_FileSystem,SIGNAL(triggered()),model2,SLOT(model_restoreState()));
     connect(this,SIGNAL(prepareToEditFile(QModelIndex)),model2,SLOT(editFileByIndex(QModelIndex)));
     connect(this,SIGNAL(editFile_signal(QString,QString)),model2,SLOT(model_editFile(QString,QString)));
     connect(this,SIGNAL(whatType(QModelIndex)),model2,SLOT(model_getType(QModelIndex)));
     connect(model2,SIGNAL(typeDefined(int)),this,SLOT(setCurrentType(int)));
     connect(this,SIGNAL(infoAboutFile_Index(QModelIndex)),model2,SLOT(model_getFileInfo(QModelIndex)));
     connect(model2,SIGNAL(fileInfo(QString)),this,SLOT(createInfoDialog(QString)));
     connect(this,SIGNAL(fileType_signal(QString)),this,SLOT(createInfoDialog(QString)));
     connect(this,SIGNAL(prepareCopyItem_index(QModelIndex)),model2,SLOT(model_prepareCopyItem(QModelIndex)));
     connect(model2,SIGNAL(itemCopy_prepared(fsElem*)),this,SLOT(setDataForCopy(fsElem*)));
     connect(pasteButton,SIGNAL(clicked()),this,SLOT(createCopyFileDialog()));
     connect(this,SIGNAL(pasteItem_preparedFull(fsElem*,QString)),model2,SLOT(model_copy(fsElem*,QString)));
     connect(model2,SIGNAL(copyFinished()),pasteButton,SLOT(hide()));
     connect(this,SIGNAL(prepareReplaceItem_index(QModelIndex)),model2,SLOT(model_prepareReplaceItem(QModelIndex)));
     connect(model2,SIGNAL(itemReplace_prepared(catalog*,string,int)),this,SLOT(setDataForReplace(catalog*,string,int)));
     connect(this,SIGNAL(pasteItemReplaced_prepared(catalog*,string,int)),model2,SLOT(model_replace(catalog*,string,int)));
     connect(model2,SIGNAL(replaceFinished()),pasteButton,SLOT(hide()));
     connect(this,SIGNAL(prepareDeleteItem_index(QModelIndex)),model2,SLOT(model_Delete(QModelIndex)));
     connect(this,SIGNAL(rename_prepared(string,string)),model2,SLOT(model_rename(string,string)));
     connect(this,SIGNAL(prepareRenameItem_index(QModelIndex)),model2,SLOT(model_prepareRenameItem(QModelIndex)));
     connect(model2,SIGNAL(renameItem_prepared(string)),this,SLOT(setNewItemName(string)));
     connect(actionLogout,SIGNAL(triggered()),model2,SLOT(model_endUserSession()));
     connect(model2,SIGNAL(logout()),this,SLOT(setNobobyLogged()));
     connect(actionAdd_user,SIGNAL(triggered()),this,SLOT(createNewUserDialog()));
     connect(this,SIGNAL(isCurrentUserAdmin()),model2,SLOT(ask_addUser()));
     connect(model2,SIGNAL(CurrentUserAdmin(bool)),this,SLOT(setAdminLogged(bool)));
     connect(this,SIGNAL(newUserDetails_prepared(int,string,string)),model2,SLOT(model_addNewUser(int,string,string)));
     connect(actionDelete_user,SIGNAL(triggered()),this,SLOT(createDeleteUserDialog()));
     connect(this,SIGNAL(deleteUserName_prepared(string)),model2,SLOT(model_deleteUser(string)));
 }

 void MainWindow::setAdminLogged(bool isAdmin){
     adminLogged=isAdmin;
 }

void MainWindow::ShowContextMenu(const QPoint &pos){
    QPoint globalPos=FSlistView->mapToGlobal(pos);

    QModelIndex index=FSlistView->currentIndex();
    emit whatType(index);
    QMenu myMenu;
    //создание контекстного меню в зависимости от типа каталог/файл
    switch(currentItemType){
    case 1:
    {
        QAction *actionCopy = new QAction(tr("Copy"), this);
        myMenu.addAction(actionCopy);
        connect(actionCopy,SIGNAL(triggered()),this,SLOT(copyItem()));
        QAction *actionReplace = new QAction(tr("Replace"), this);
        myMenu.addAction(actionReplace);
        connect(actionReplace,SIGNAL(triggered()),this,SLOT(replaceItem()));
        QAction *actionDelete = new QAction(tr("Delete"), this);
        myMenu.addAction(actionDelete);
        connect(actionDelete,SIGNAL(triggered()),this,SLOT(deleteItem()));
        QAction *actionRename = new QAction(tr("Rename"), this);
        myMenu.addAction(actionRename);
        connect(actionRename,SIGNAL(triggered()),this,SLOT(renameItem()));
    }
        break;
    case 2:
    {
         QAction *actionFileInfo = new QAction(tr("Info"), this);
         myMenu.addAction(actionFileInfo);
         connect(actionFileInfo,SIGNAL(triggered()),this,SLOT(infoAboutFile()));
         QAction *actionFileType = new QAction(tr("Type of File"), this);
         myMenu.addAction(actionFileType);
         connect(actionFileType,SIGNAL(triggered()),this,SLOT(fileType()));
         QAction *actionFileGetAccess = new QAction(tr("Get permissions for this file"), this);
         myMenu.addAction(actionFileGetAccess);
         connect(actionFileGetAccess,SIGNAL(triggered()),this,SLOT(fileGetPermissions()));
         QAction *actionFileChangePermissions = new QAction(tr("Change permissions"), this);
         myMenu.addAction(actionFileChangePermissions);
         connect(actionFileChangePermissions,SIGNAL(triggered()),this,SLOT(fileChangePermissions()));
         myMenu.addSeparator();
         QAction *actionCopy = new QAction(tr("Copy"), this);
         myMenu.addAction(actionCopy);
         connect(actionCopy,SIGNAL(triggered()),this,SLOT(copyItem()));
         QAction *actionReplace = new QAction(tr("Replace"), this);
         myMenu.addAction(actionReplace);
         connect(actionReplace,SIGNAL(triggered()),this,SLOT(replaceItem()));
         QAction *actionDelete = new QAction(tr("Delete"), this);
         myMenu.addAction(actionDelete);
         connect(actionDelete,SIGNAL(triggered()),this,SLOT(deleteItem()));
         QAction *actionRename = new QAction(tr("Rename"), this);
         myMenu.addAction(actionRename);
         connect(actionRename,SIGNAL(triggered()),this,SLOT(renameItem()));
    }
        break;
    case 3:
    {
        myMenu.addSeparator();
        QAction *actionCopy = new QAction(tr("Copy"), this);
        myMenu.addAction(actionCopy);
        connect(actionCopy,SIGNAL(triggered()),this,SLOT(copyItem()));
        QAction *actionReplace = new QAction(tr("Replace"), this);
        myMenu.addAction(actionReplace);
        connect(actionReplace,SIGNAL(triggered()),this,SLOT(replaceItem()));
        QAction *actionDelete = new QAction(tr("Delete"), this);
        myMenu.addAction(actionDelete);
        connect(actionDelete,SIGNAL(triggered()),this,SLOT(deleteItem()));
        QAction *actionRename = new QAction(tr("Rename"), this);
        myMenu.addAction(actionRename);
        connect(actionRename,SIGNAL(triggered()),this,SLOT(renameItem()));
    }
        break;
    default:
    {

    }
    }


    QAction *selectedItem=myMenu.exec(globalPos);
    if(selectedItem)
    {

    }
    else
    {

    }
}

void MainWindow::setCurrentType(int type){
    currentItemType=type;
}

 void MainWindow::createAddCatalogDialog(){
     filename_dialog *addDialog=new filename_dialog;

     addDialog->show();
     connect(addDialog,SIGNAL(Send(QString)),this,SLOT(createCatalog(QString)));
     /*QString data;
     if(addDialog->exec() == QDialog::Accepted)
         {
             data = addDialog->data();

             this->setWindowTitle(data);

    }*/
 }

 void MainWindow::createAddFileDialog(){
     filename_dialog *addDialog=new filename_dialog;

     addDialog->show();
     connect(addDialog,SIGNAL(Send(QString)),this,SLOT(createFile(QString)));
 }

 void MainWindow::createLoginDialog(){
    login_dialog *addDialog=new login_dialog;
    addDialog->show();
    connect(addDialog,SIGNAL(Send(QStringList)),this,SLOT(login(QStringList)));
 }

void MainWindow::createCatalog(QString newFilename){
    emit catalogName_done(newFilename);
}

void MainWindow::createFile(QString newFilename){
    emit fileName_done(newFilename);
}

void MainWindow::login(QStringList loginData){
    emit loginDetails_isGetted((loginData));
}

void MainWindow::createErrorDialog(QString errorMsg){
    error_dialog *addErrorDialog=new error_dialog(errorMsg);
    addErrorDialog->show();
}

void MainWindow::createInfoDialog(QString msg){
    info_dialog *addInfoDialog=new info_dialog(msg);
    addInfoDialog->show();
}

void MainWindow::createFileViewDialog(QString filename,QString data,int editable){
    fileViewDialog *addDialog=new fileViewDialog(filename,data,editable);
    addDialog->show();
    connect(addDialog,SIGNAL(fileChanged(QString,QString)),this,SLOT(editFile(QString,QString)));
}

void MainWindow::createEditFileDialog(){
}

void MainWindow::editFile(QString filename,QString data){
    emit editFile_signal(filename,data);
}

void MainWindow::infoAboutFile(){
    QModelIndex index=FSlistView->currentIndex();
    emit infoAboutFile_Index(index);
}

void MainWindow::fileType(){
    switch(currentItemType){
    case 2:
    {
        emit fileType_signal("This is usual file");
    }
        break;
    case 3:
    {
        emit fileType_signal("This is secured file");
    }
    default:
    {emit fileType_signal("Error type of file");}

}
}

void MainWindow::fileGetPermissions(){
    QModelIndex index=FSlistView->currentIndex();
    emit fileGetPermissions_Index(index);
}

void MainWindow::fileChangePermissions(){
    QModelIndex index=FSlistView->currentIndex();
    emit fileGetPermissions_Index(index);
}

void MainWindow::copyItem(){
    QModelIndex index=FSlistView->currentIndex();
    emit prepareCopyItem_index(index);
}

void MainWindow::replaceItem(){
    QModelIndex index=FSlistView->currentIndex();
    emit prepareReplaceItem_index(index);
}

void MainWindow::setDataForCopy(fsElem *copyingElem){
    pasteItem=copyingElem;
    pasteButton->show();
    replaceFlag=false;
}

void MainWindow::setDataForReplace(catalog *current, string name, int type){
    pasteItem=current;  //сохраняется каталог, в котором хранится перемещаемый элемент
    itemName=name;
    pasteItemType=type;
    pasteButton->show();
    replaceFlag=true;
}

void MainWindow::createCopyFileDialog(){
    if(!replaceFlag){
    filename_dialog *addDialog=new filename_dialog;
    addDialog->show();
    connect(addDialog,SIGNAL(Send(QString)),this,SLOT(pasteItemName_prepared(QString)));
    }
    else
    {
        catalog *parentItemCatalog=dynamic_cast<catalog *>(pasteItem);
        if(parentItemCatalog)
            emit pasteItemReplaced_prepared(parentItemCatalog,itemName,pasteItemType);
    }
}


void MainWindow::pasteItemName_prepared(QString newName){
    emit pasteItem_preparedFull(pasteItem,newName);
}

void MainWindow::deleteItem(){
    QModelIndex index=FSlistView->currentIndex();
    emit prepareDeleteItem_index(index);
}

void MainWindow::renameItem(){
    QModelIndex index=FSlistView->currentIndex();
    emit prepareRenameItem_index(index);
}

void MainWindow::createRenameDialog(){
    filename_dialog *addDialog=new filename_dialog;
    addDialog->show();
    connect(addDialog,SIGNAL(Send(QString)),this,SLOT(newItemName_prepared(QString)));
}

void MainWindow::setNewItemName(string newName){
    itemName=newName;
    this->createRenameDialog();
}

void MainWindow::newItemName_prepared(QString newName){
    emit rename_prepared(itemName,newName.toStdString());
}

void MainWindow::setNobobyLogged(){
    loginStatusLabel->setText("Nobody logged in filesystem");
}

void MainWindow::createNewUserDialog(){
    emit isCurrentUserAdmin();
    if(adminLogged)
    {
        addUser_dialog *addDialog=new addUser_dialog;
        addDialog->show();
        connect(addDialog,SIGNAL(Send(QStringList)),this,SLOT(addUser(QStringList)));
    }
    else
    {
        this->createErrorDialog("You not ADMIN, so you have not permissions for adding users to this filesystem");
    }
}

void MainWindow::addUser(QStringList newUserData){
    string username=newUserData.at(0).toStdString();
    string key=newUserData.at(1).toStdString();
    int group=newUserData.at(2).toInt();
    emit newUserDetails_prepared(group,username,key);
}

void MainWindow::createDeleteUserDialog(){
    emit isCurrentUserAdmin();
    if(adminLogged)
    {
        deleteUser_dialog *addDialog=new deleteUser_dialog;
        addDialog->show();
        connect(addDialog,SIGNAL(Send(QString)),this,SLOT(deleteUser(QString)));
    }
    else
    {
        this->createErrorDialog("You not ADMIN, so you have not permissions for adding users to this filesystem");
    }
}

void MainWindow::deleteUser(QString username){
    emit deleteUserName_prepared(username.toStdString());
}
