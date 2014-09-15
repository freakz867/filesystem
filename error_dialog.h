#ifndef ERROR_DIALOG_H
#define ERROR_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QString>
#include "mainwindow.h"

class error_dialog:public QDialog
{
    Q_OBJECT
private:
    QLabel *info;
    QPushButton *okButton;
public:
    error_dialog(QString errorMsg, QWidget *parent=0);
public slots:
    void okPush();
};
#endif // ERROR_DIALOG_H
