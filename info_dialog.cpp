#include "info_dialog.h"

info_dialog::info_dialog(QString data,QWidget *parent){
    this->setWindowTitle("Information");
    this->setModal(true);
    infoViewer= new QTextEdit(data);
    infoViewer->setReadOnly(1);
    okButton = new QPushButton("OK");
    connect(okButton, SIGNAL(clicked()), this, SLOT(okPush()));
       QVBoxLayout *layout1 = new QVBoxLayout;

          QVBoxLayout *layout2 = new QVBoxLayout;
          QHBoxLayout *layout3 = new QHBoxLayout;

          layout2->addWidget(infoViewer);
          layout3->addWidget(okButton);
          layout1->addLayout(layout2);
          layout1->addLayout(layout3);

          setLayout(layout1);
          setFixedHeight(sizeHint().height());
}


void info_dialog::okPush(){
    done( QDialog::Accepted );
    this->close();
}
