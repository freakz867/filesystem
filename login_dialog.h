#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include "mainwindow.h"

class login_dialog:public QDialog
{
    Q_OBJECT
private:
    QLabel *info;
    QLineEdit *loginEdit;
    QLineEdit *passEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
public:
    login_dialog(QWidget *parent=0);


public slots:
    //void okPush();
    void sendData();

signals:
        void Send(QStringList loginData);
};


#endif // LOGIN_DIALOG_H
