#ifndef INFO_DIALOG_H
#define INFO_DIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QString>
#include "mainwindow.h"

class info_dialog:public QDialog
{
    Q_OBJECT
private:
    QTextEdit *infoViewer;
    QPushButton *okButton;
public:
    info_dialog(QString data, QWidget *parent=0);
public slots:
    void okPush();
};
#endif // INFO_DIALOG_H
