#include "login_dialog.h"

#include <QtGui>
#include "mainwindow.h"

login_dialog::login_dialog(QWidget *parent):QDialog(parent)
{
    this->setWindowTitle("Enter login details");
    this->setModal(true);
    info= new QLabel("Enter login and password:");
    loginEdit=new QLineEdit;
    passEdit=new QLineEdit;
    passEdit->setEchoMode(QLineEdit::Password);
    okButton = new QPushButton("login");
    cancelButton = new QPushButton("cancel");

       connect(okButton, SIGNAL(clicked()), this, SLOT(sendData()));
       connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

       QVBoxLayout *layout1 = new QVBoxLayout;

          QVBoxLayout *layout2 = new QVBoxLayout;
          QHBoxLayout *layout3 = new QHBoxLayout;

          layout2->addWidget(info);
          layout2->addWidget(loginEdit);
          layout2->addWidget(passEdit);
          layout3->addWidget(okButton);
          layout3->addWidget(cancelButton);

          layout1->addLayout(layout2);
          layout1->addLayout(layout3);

          setLayout(layout1);
          setFixedHeight(sizeHint().height());
}

void login_dialog::sendData(){
    QStringList data;
    data.append(loginEdit->text());
    data.append(passEdit->text());
    done( QDialog::Accepted );
    emit Send(data);
}

