#ifndef ADDUSER_DIALOG_H
#define ADDUSER_DIALOG_H

        #include <QDialog>
        #include <QLabel>
        #include <QLineEdit>
#include <QString>
#include <QCheckBox>
        #include "mainwindow.h"

        class addUser_dialog:public QDialog
        {
            Q_OBJECT
        private:
            QLabel *info;
            QLineEdit *loginEdit;
            QLineEdit *passEdit;
            QCheckBox *userGroup;
            QPushButton *okButton;
            QPushButton *cancelButton;
        public:
            addUser_dialog(QWidget *parent=0);


        public slots:
            void sendData();

        signals:
                void Send(QStringList loginData);
        };
#endif // ADDUSER_DIALOG_H
