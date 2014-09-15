#ifndef DELETEUSER_DIALOG_H
#define DELETEUSER_DIALOG_H

        #include <QDialog>
        #include <QLabel>
        #include <QLineEdit>
        #include <QString>
        #include "mainwindow.h"

        class deleteUser_dialog:public QDialog
        {
            Q_OBJECT
        private:
            QLabel *info;
            QLineEdit *textLine;
            QPushButton *okButton;
            QPushButton *cancelButton;
            QString filename;
        public:
            deleteUser_dialog(QWidget *parent=0);
            QString data();


        public slots:
            void sendData();

        signals:
                void Send(QString newFilename);
        };

#endif // DELETEUSER_DIALOG_H
