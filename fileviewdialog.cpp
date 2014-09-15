#include "fileviewdialog.h"


fileViewDialog::fileViewDialog(QString filename,QString data,int editableFlag,QWidget *parent):QDialog(parent)
{
    this->setWindowTitle("File View");
    this->setModal(true);
    dataViewer= new QTextEdit(data);
    if(editableFlag==0)
        dataViewer->setReadOnly(1);
    okButton = new QPushButton("OK");
    editButton = new QPushButton("edit");
    if(editableFlag==1)
    {
        filenameTemp=filename;

    }
    connect(okButton, SIGNAL(clicked()), this, SLOT(okPush()));
    connect(editButton,SIGNAL(clicked()),this,SLOT(editButtonPush()));

       QVBoxLayout *layout1 = new QVBoxLayout;

          QVBoxLayout *layout2 = new QVBoxLayout;
          QHBoxLayout *layout3 = new QHBoxLayout;

          layout2->addWidget(dataViewer);
          layout3->addWidget(okButton);
          if(editableFlag==1)
              layout3->addWidget(editButton);
          layout1->addLayout(layout2);
          layout1->addLayout(layout3);

          setLayout(layout1);
          setFixedHeight(sizeHint().height());
}

void fileViewDialog::okPush(){
    done( QDialog::Accepted );
    this->close();
}

void fileViewDialog::editButtonPush(){
    QString newData=dataViewer->toPlainText();
    done( QDialog::Accepted );
    emit fileChanged(filenameTemp,newData);
}
