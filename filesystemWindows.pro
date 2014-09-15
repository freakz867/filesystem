#-------------------------------------------------
#
# Project created by QtCreator 2013-11-21T18:40:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = filesystemWindows
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fsElem/fsElem.cpp \
    catalog.cpp \
    allocator.cpp \
    crypt.cpp \
    file.cpp \
    filesystemCore.cpp \
    stream.cpp \
    secFile.cpp \
    myVector.cpp \
    filename_dialog.cpp \
    login_dialog.cpp \
    error_dialog.cpp \
    fileviewdialog.cpp \
    fileedit_dialog.cpp \
    info_dialog.cpp \
    adduser_dialog.cpp \
    deleteuser_dialog.cpp

HEADERS  += mainwindow.h \
    fsElem/fsElem.h \
    catalog.h \
    allocator.h \
    crypt.h \
    file.h \
    filesystemCore.h \
    stream.h \
    secFile.h \
    myVector.h \
    filename_dialog.h \
    login_dialog.h \
    error_dialog.h \
    fileviewdialog.h \
    fileedit_dialog.h \
    info_dialog.h \
    adduser_dialog.h \
    deleteuser_dialog.h

FORMS    += mainwindow.ui
