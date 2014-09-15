#include "deleteuser_dialog.h"

#include "filename_dialog.h"
#include <QtGui>
#include "mainwindow.h"

deleteUser_dialog::deleteUser_dialog(QWidget *parent):QDialog(parent)
{
    this->setWindowTitle("Enter username");
    this->setModal(true);
    info= new QLabel("Enter username:");
    textLine=new QLineEdit;
    okButton = new QPushButton("delete");
    cancelButton = new QPushButton("cancel");

       connect(okButton, SIGNAL(clicked()), this, SLOT(sendData())); //нажатие кнопки диалога ОК
       connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

       QVBoxLayout *layout1 = new QVBoxLayout;

          QVBoxLayout *layout2 = new QVBoxLayout;
          QHBoxLayout *layout3 = new QHBoxLayout;

          layout2->addWidget(info);
          layout2->addWidget(textLine);
          layout3->addWidget(okButton);
          layout3->addWidget(cancelButton);

          layout1->addLayout(layout2);
          layout1->addLayout(layout3);

          setLayout(layout1);
          setFixedHeight(sizeHint().height());
}

void deleteUser_dialog::sendData(){
    filename=textLine->text();

    done( QDialog::Accepted );
    emit Send(filename);
}
QString deleteUser_dialog::data(){
    return filename;
}
