#include "mainwindow.h"

#include <QDebug>
#include <QMainWindow>
#include <QPalette>
#include <QPixmap>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStyle>
#include <QWidget>
#include <QtSql/QSqlDatabase>

MainWindow::MainWindow(QMainWindow *parent) : QMainWindow(parent) {
    GlobalSettings = new QSettings("Settings", "Settings");
    QFont selectedFont = GlobalSettings->value("font").value<QFont>();
    
    setWindowTitle(" ~ focuse space ~ ");
    setFixedSize(700, 380);

    statusBar = new QStatusBar(this);
    statusBar->setFont(QFont(selectedFont));
    statusBar->setStyleSheet("color: #FFFFFF; font-size:  9px;");

    imageLabel = new QLabel(this);
    QPixmap image("://tasks.png");
    imageLabel->setGeometry(50, 80, 300, 300);
    imageLabel->setPixmap(image);

    titleMain = new QLabel("HOME PAGE", this);
    titleMain->setGeometry(0, 30, 700, 60);
    titleMain->setAlignment(Qt::AlignCenter);
    titleMain->setFont(QFont(selectedFont));
    titleMain->setStyleSheet(" color: #ffffff; font-size: 30px;");

    openTasks = new QPushButton("TASKS", this);
    openTasks->setGeometry(350, 120, 300, 40);
    openTasks->setFont(QFont(selectedFont));
    openTasks->setStyleSheet("color: #FFFFFF; font-size:  11px;");

    connect(openTasks, SIGNAL(clicked()), this, SLOT(openTasksWin()));

    openNotes = new QPushButton("NOTES", this);
    openNotes->setGeometry(350, 170, 300, 40);
    openNotes->setFont(QFont(selectedFont));
    openNotes->setStyleSheet("color: #FFFFFF; font-size:  11px;");

    connect(openNotes, SIGNAL(clicked()), this, SLOT(openNotesWin()));

    openTimer = new QPushButton("POMODORO", this);
    openTimer->setGeometry(350, 220, 300, 40);
    openTimer->setFont(QFont(selectedFont));
    openTimer->setStyleSheet("color: #FFFFFF; font-size:  11px;");

    connect(openTimer, SIGNAL(clicked()), this, SLOT(openPomodoroWin()));


    openSettings = new QPushButton("SETTINGS", this);
    openSettings->setGeometry(450, 270, 100, 40);
    openSettings->setFont(QFont(selectedFont));
    openSettings->setStyleSheet("color: #FFFFFF; font-size:  11px;");

    connect(openSettings, SIGNAL(clicked()), this, SLOT(openSettingsWin()));

    setStatusBar(statusBar);

    qDebug() << "OK";
    statusBar->showMessage("OK");
}

MainWindow::~MainWindow() {}

void MainWindow::openSettingsWin() {
    // hide();
    settings_window = new Settings(this);
    //settings_window->setWindowIcon(QIcon("://tasks.svg"));
    settings_window->show();
}

void MainWindow::openTasksWin() {
    // hide();
    tasks_window = new Tasks(this);
    tasks_window->setWindowIcon(QIcon("://tasks.svg"));
    tasks_window->show();
}

void MainWindow::openNotesWin() {
    // hide();
    notes_window = new Notes(this);
    notes_window->setWindowIcon(QIcon("://notes.svg"));
    notes_window->show();
}

void MainWindow::openPomodoroWin() {
    // hide();
    pomodoro_window = new Pomodoro(this);
    pomodoro_window->setWindowIcon(QIcon("://pomodoro.svg"));
    pomodoro_window->show();
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_1) {
        // hide();
        tasks_window = new Tasks(this);
        tasks_window->setWindowIcon(QIcon("://tasks.svg"));
        tasks_window->show();
    }
    if (e->key() == Qt::Key_2) {
        // hide();
        notes_window = new Notes(this);
        notes_window->setWindowIcon(QIcon("://notes.svg"));
        notes_window->show();
    }
    if (e->key() == Qt::Key_3) {
        // hide();
        pomodoro_window = new Pomodoro(this);
        pomodoro_window->setWindowIcon(QIcon("://pomodoro.svg"));
        pomodoro_window->show();
    }
}
