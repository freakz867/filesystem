#include "error_dialog.h"


error_dialog::error_dialog(QString errorMsg,QWidget *parent):QDialog(parent)
{
    this->setWindowTitle(errorMsg);
    this->setModal(true);
    info= new QLabel(errorMsg);
    okButton = new QPushButton("OK");
    connect(okButton, SIGNAL(clicked()), this, SLOT(okPush())); //нажатие кнопки диалога ОК

       QVBoxLayout *layout1 = new QVBoxLayout;

          QVBoxLayout *layout2 = new QVBoxLayout;
          QHBoxLayout *layout3 = new QHBoxLayout;

          layout2->addWidget(info);
          layout3->addWidget(okButton);

          layout1->addLayout(layout2);
          layout1->addLayout(layout3);

          setLayout(layout1);
          setFixedHeight(sizeHint().height());
}

void error_dialog::okPush(){
    done( QDialog::Accepted );
    this->close();
}
