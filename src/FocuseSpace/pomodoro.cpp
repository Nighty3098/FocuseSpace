#include "pomodoro.h"

#include <QDebug>
#include <QDialog>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <iomanip>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_pomodoro.h"

Pomodoro::Pomodoro(QMainWindow *parent) : QMainWindow(parent) {
    setWindowTitle("~ pomodoro ~");

    QSettings settings("Pomodoro", "Pomodoro");
    restoreGeometry(settings.value("geometry").toByteArray());

    setMinimumSize(300, 220);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);
    infoLayout = new QVBoxLayout;
    buttonsLayout_1 = new QHBoxLayout;
    buttonsLayout_2 = new QHBoxLayout;

    titleLabel = new QLabel(this);
    titleLabel->setFont(QFont("SF Pro Black", 40));
    titleLabel->setText("POMODORO");
    titleLabel->setGeometry(0, 5, 300, 120);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(" color: #ffffff; ");

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));

    startButton = new QPushButton("START", this);
    startButton->setMinimumSize(100, 30);
    startButton->setFont(QFont("SF Pro Black", 12));
    connect(startButton, SIGNAL(clicked()), this, SLOT(startPomodoro()));

    changeButton = new QPushButton("CHANGE", this);
    changeButton->setMinimumSize(100, 30);
    changeButton->setFont(QFont("SF Pro Black", 12));
    connect(changeButton, SIGNAL(clicked()), this, SLOT(change()));

    backButton = new QPushButton("BACK", this);
    backButton->setMinimumSize(100, 30);
    backButton->setFont(QFont("SF Pro Black", 12));
    connect(backButton, SIGNAL(clicked()), this, SLOT(toMainWindow()));

    infoLayout->addWidget(titleLabel);

    buttonsLayout_1->addWidget(startButton);

    buttonsLayout_2->addWidget(changeButton);
    buttonsLayout_2->addWidget(backButton);

    mainLayout->addLayout(infoLayout);
    mainLayout->addLayout(buttonsLayout_1);
    mainLayout->addLayout(buttonsLayout_2);

    timeLeft = 25 * 60;
    isPomodoro = false;
    isBreak = false;
}

Pomodoro::~Pomodoro() {
    delete ui;
    QSettings settings("Pomodoro", "Pomodoro");
    settings.setValue("geometry", saveGeometry());
}

void Pomodoro::startPomodoro() {
    if (isPomodoro == false) {
        isBreak = false;
        isPomodoro = true;
        timeLeft = 25 * 60;

        titleLabel->setText("WORK");
        titleLabel->setFont(QFont("SF Pro Black", 70));
        titleLabel->setStyleSheet(" color: #c75057; ");

        startButton->setText("STOP");
        timer->start(1000);
    }

    else {
        setWindowTitle(" ~ pomodoro ~ ");

        titleLabel->setText("POMODORO");
        titleLabel->setStyleSheet(" color: #ffffff; ");
        titleLabel->setGeometry(0, 20, 250, 50);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setFont(QFont("SF Pro Black", 40));

        startButton->setText("START");

        timeLeft = 0;
        isPomodoro = false;
        isBreak = true;

        timer->stop();
    }
}

void Pomodoro::startBreak() {
    isBreak = true;
    isPomodoro = false;
    timeLeft = 5 * 60;

    titleLabel->setText("BREAK");
    titleLabel->setFont(QFont("SF Pro Black", 70));
    titleLabel->setStyleSheet(" color: #81c750; ");

    setWindowTitle("~ break ~");
    timer->start(1000);
}

void Pomodoro::updateTimer() {
    if (timeLeft > 0) {
        timeLeft--;

        int minutes = timeLeft / 60;
        int seconds = timeLeft % 60;

        QString timeString = QString("%1:%2")
                                 .arg(minutes, 2, 10, QChar('0'))
                                 .arg(seconds, 2, 10, QChar('0'));
        titleLabel->setText(timeString);
        titleLabel->setFont(QFont("SF Pro Black", 70));

    } else {
        timer->stop();
        if (isPomodoro) {
            isPomodoro = false;

            startBreak();
        } else {
            isPomodoro = true;

            titleLabel->setText("WORK");
            titleLabel->setFont(QFont("SF Pro Black", 40));
        }
    }
}

void Pomodoro::change() {
    if (isPomodoro) {
        if (isBreak) {
            startPomodoro();
        } else {
            startBreak();
        }
    } else {
        startPomodoro();
    }
}

void Pomodoro::toMainWindow() {
    close();

    QSettings settings("Pomodoro", "Pomodoro");
    settings.setValue("geometry", saveGeometry());

    /*
    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->setWindowIcon(QIcon("images/home.svg"));
    mainWindow->show();
    */
}
