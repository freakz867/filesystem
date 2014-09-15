#include "adduser_dialog.h"

#include <QtGui>
#include "mainwindow.h"

addUser_dialog::addUser_dialog(QWidget *parent):QDialog(parent)
{
    this->setWindowTitle("Enter new user details");
    this->setModal(true);
    info= new QLabel("Enter login and password:");
    loginEdit=new QLineEdit;
    passEdit=new QLineEdit;
    QLabel *info2=new QLabel("group: User");
    passEdit->setEchoMode(QLineEdit::Password);
    userGroup=new QCheckBox;    //если нажат - значит пользователь
    okButton = new QPushButton("add user");
    cancelButton = new QPushButton("cancel");

       connect(okButton, SIGNAL(clicked()), this, SLOT(sendData())); //нажатие кнопки диалога ОК
       connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

       QVBoxLayout *layout1 = new QVBoxLayout;

          QVBoxLayout *layout2 = new QVBoxLayout;
          QHBoxLayout *layout3 = new QHBoxLayout;
          QHBoxLayout *layout4 = new QHBoxLayout;

          layout2->addWidget(info);
          layout2->addWidget(loginEdit);
          layout2->addWidget(passEdit);
          layout4->addWidget(info2);
          layout4->addWidget(userGroup);
          layout3->addWidget(okButton);
          layout3->addWidget(cancelButton);
          layout2->addLayout(layout4);
          layout1->addLayout(layout2);
          layout1->addLayout(layout3);

          setLayout(layout1); // устанавливает менеджер компоновки для окна
          setFixedHeight(sizeHint().height()); // возвращает "идеальный" размер виджета
}

void addUser_dialog::sendData(){
    QStringList data;
    data.append(loginEdit->text());
    data.append(passEdit->text());
    if(userGroup->isChecked())
        data.append("1");
    else
        data.append("2");
    done( QDialog::Accepted );
    emit Send(data);
}
