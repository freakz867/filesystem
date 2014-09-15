#ifndef FILEVIEWDIALOG_H
#define FILEVIEWDIALOG_H

        #include <QDialog>
        #include <QTextEdit>
        #include <QString>
        #include "mainwindow.h"

        class fileViewDialog:public QDialog
        {
            Q_OBJECT
        private:
            QTextEdit *dataViewer;
            QPushButton *okButton;
            QPushButton *editButton;
            QString filenameTemp;
        public:
            fileViewDialog(QString filename,QString data,int editableFlag, QWidget *parent=0);
        public slots:
            void okPush();
            void editButtonPush();
        signals:
            void fileChanged(QString filenameTemp,QString newData);
        };

#endif // FILEVIEWDIALOG_H
