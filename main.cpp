#include "mainwindow.h"
#include <QApplication>
#include <QTreeView>
#include <QAbstractItemModel>
#include <QStringList>
#include <QStringListModel>
#include <QListView>
#include <QtGui>
#include <cstdlib>
#include <bitset>
#include "file.h"
#include "catalog.h"
#include "filesystemCore.h"
#include "crypt.h"
#include "secFile.h"

using namespace std;


void createfs(){
        ofstream out("data.dat",ios::out | ios::binary); //Открываем файл в двоичном режиме для записи
        char y=0b11111101;
        for(int i=0;i<15;++i)
        {
            out.write((char*)&y,sizeof(y));
        }
        out.close(); //Закрываем файл
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<std::string>("std::string");
    MainWindow window;
    window.showMaximized();
    window.show();
    return a.exec();
}
