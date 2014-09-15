#ifndef FILEEDIT_DIALOG_H
#define FILEEDIT_DIALOG_H

        #include <QDialog>
        #include <QTextEdit>
        #include <QString>
        #include "mainwindow.h"

        class fileEdit_dialog:public QDialog
        {
            Q_OBJECT
        private:
            QTextEdit *dataViewer;
            QPushButton *okButton;
        public:
            fileEdit_dialog(QString data, QWidget *parent=0);
        public slots:
            void okPush();
        signals:
                void Send(QString newFilename);
        };
#endif // FILEEDIT_DIALOG_H
