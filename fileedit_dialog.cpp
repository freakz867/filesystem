#include "fileedit_dialog.h"


fileEdit_dialog::fileEdit_dialog(QString data,QWidget *parent):QDialog(parent)
{
    this->setWindowTitle("File Edit");
    this->setModal(true);
    dataViewer= new QTextEdit(data);
    okButton = new QPushButton("OK");
    connect(okButton, SIGNAL(clicked()), this, SLOT(okPush()));

       QVBoxLayout *layout1 = new QVBoxLayout;

          QVBoxLayout *layout2 = new QVBoxLayout;
          QHBoxLayout *layout3 = new QHBoxLayout;

          layout2->addWidget(dataViewer);
          layout3->addWidget(okButton);

          layout1->addLayout(layout2);
          layout1->addLayout(layout3);

          setLayout(layout1);
          setFixedHeight(sizeHint().height());
}

void fileEdit_dialog::okPush(){
    QString data=dataViewer->toPlainText();

    done( QDialog::Accepted );
    emit Send(data);
}
