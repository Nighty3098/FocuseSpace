#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMainWindow>
#include <QScrollBar>
#include <QSqlQuery>
#include <QWidget>

#include "mainwindow.h"

static bool createConnection() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.db");

    db.setUserName("admin");
    db.setHostName("localhost");
    db.setPassword("password");

    if (!db.open()) {
        qDebug() << db.lastError();
        return false;
    }

    return true;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    if (createConnection() == false) {
        return -1;
    }

    QFile file(":/stylesheet.qss");
    file.open(QFile::ReadOnly);


    MainWindow mainWindow;
    mainWindow.setWindowTitle(" ~ focuse space ~ ");
    mainWindow.setWindowIcon(QIcon("://home.svg"));
    mainWindow.show();

    a.setStyleSheet(file.readAll());

    return a.exec();
}
