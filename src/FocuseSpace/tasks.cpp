#include "tasks.h"

#include <QDateTime>
#include <QDebug>
#include <QDialog>
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_tasks.h"

Tasks::Tasks(QMainWindow *parent) : QMainWindow(parent) {
    QSettings settings("Tasks", "Tasks");
    restoreGeometry(settings.value("geometry").toByteArray());

    QString str =
        "CREATE TABLE donetasks ( "
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "task VARCHAR(300),"
        "date VARCHAR(100)"
        ");";

    if (!query.exec(str)) {
        qDebug() << query.lastError();
    } else {
        qDebug() << "Таблица donetasks создана";
    }

    QString str2 =
        "CREATE TABLE tasks ( "
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "task VARCHAR (300),"
        "date VARCHAR(100)"
        ");";

    if (!query.exec(str2)) {
        qDebug() << query.lastError();
    } else {
        qDebug() << "Таблица tasks создана";
    }

    setWindowTitle("~ tasks ~");
    setMinimumSize(600, 400);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);
    titleLayout = new QHBoxLayout;
    tasksLayout = new QHBoxLayout;
    buttonsLayout = new QHBoxLayout;
    taskInputLayout = new QHBoxLayout;

    mainLayout->setSpacing(10);

    statusBar = new QStatusBar(this);

    taskList = new QListWidget(this);
    taskList->setStyleSheet("border-color: #dd4853;");
    taskList->setMinimumSize(50, 50);
    taskList->setFont(QFont("SF Pro Black", 12));
    connect(taskList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this,
            SLOT(moveToDone(QListWidgetItem *)));

    doneList = new QListWidget(this);
    doneList->setStyleSheet("border-color: #74db6c; ");
    doneList->setMinimumSize(50, 50);
    doneList->setFont(QFont("SF Pro Black", 12));
    connect(doneList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this,
            SLOT(moveToTasks(QListWidgetItem *)));

    taskInput = new QLineEdit(this);
    taskInput->setFont(QFont("SF Pro Black", 12));
    taskInput->setMinimumSize(50, 30);
    taskInput->setAlignment(Qt::AlignCenter);
    taskInput->setPlaceholderText("Task");
    connect(taskInput, &QLineEdit::returnPressed, [=] {
        QString title = taskInput->text();

        if (title.isEmpty()) {
            qDebug() << "Task is empty";
            statusBar->showMessage("Task is empty");
        } else {
            addTask();
        }
    });

    clearTask = new QPushButton("REMOVE", this);
    clearTask->setFont(QFont("SF Pro Black", 10));
    clearTask->setMinimumSize(40, 30);
    connect(clearTask, SIGNAL(clicked()), this, SLOT(removeTask()));

    backButton = new QPushButton("BACK", this);
    backButton->setFont(QFont("SF Pro Black", 10));
    backButton->setMinimumSize(40, 30);
    connect(backButton, SIGNAL(clicked()), this, SLOT(toMainWindow()));

    removeButton = new QPushButton("REMOVE", this);
    removeButton->setFont(QFont("SF Pro Black", 10));
    removeButton->setMinimumSize(40, 30);
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeDone()));

    saveButton = new QPushButton("ADD TASK", this);
    saveButton->setFont(QFont("SF Pro Black", 10));
    saveButton->setMinimumSize(100, 30);
    connect(saveButton, SIGNAL(clicked()), this, SLOT(addTask()));

    setStatusBar(statusBar);

    tasksLayout->addWidget(taskList);
    tasksLayout->addWidget(doneList);

    taskInputLayout->addWidget(taskInput);
    taskInputLayout->addWidget(saveButton);

    buttonsLayout->addWidget(clearTask);
    buttonsLayout->addWidget(backButton);
    buttonsLayout->addWidget(removeButton);

    mainLayout->addLayout(titleLayout);
    mainLayout->addLayout(tasksLayout);
    mainLayout->addLayout(taskInputLayout);
    mainLayout->addLayout(buttonsLayout);

    query.exec("SELECT * FROM tasks");
    while (query.next()) {
        int id = query.value("id").toInt();
        QString text = query.value("task").toString();

        QListWidgetItem *item = new QListWidgetItem(text);
        item->setData(Qt::UserRole, id);
        taskList->addItem(item);

        qDebug() << text << " was loaded";

        statusBar->showMessage("All tasks was loaded");
    }

    query.exec("SELECT * FROM donetasks");
    while (query.next()) {
        int id = query.value("id").toInt();
        QString text = query.value("task").toString();

        QListWidgetItem *item = new QListWidgetItem(text);
        item->setData(Qt::UserRole, id);
        doneList->addItem(item);

        qDebug() << text << " was loaded";

        statusBar->showMessage("All tasks was loaded");
    }
}

Tasks::~Tasks() {
    QSettings settings("Tasks", "Tasks");
    settings.setValue("geometry", saveGeometry());
    delete ui;
}

void Tasks::addTask() {
    QString date = QDateTime::currentDateTime().toString("hh:mm dd.MM.yyyy");
    QString task = taskInput->text();
    if (!task.isEmpty()) {
        taskList->addItem(task);
        taskInput->clear();
    }

    if (!query.exec("INSERT INTO tasks (task, date) VALUES('" + task + "', '" +
                    date + "');")) {
        qDebug() << query.lastError();
    } else {
        qDebug() << task << " was saved";
        statusBar->showMessage(task + " was saved");
    }
}

void Tasks::moveToDone(QListWidgetItem *item) {
    QString date = QDateTime::currentDateTime().toString("hh:mm dd.MM.yyyy");
    QString done = item->text();

    if (!query.exec("INSERT INTO donetasks (task, date) VALUES('" + done +
                    "', '" + date + "');")) {
        qDebug() << query.lastError();
    } else {
        QListWidgetItem *item = taskList->takeItem(taskList->currentRow());
        doneList->addItem(item);
        qDebug() << done << " was moved";
        statusBar->showMessage(done + " was moved");
    }

    QString task = item->text();

    query.prepare("DELETE FROM tasks WHERE task = '" + task + "'");

    if (query.exec()) {
        qDebug() << done << " was moved";
        statusBar->showMessage(done + " was moved");
    } else {
        qDebug() << "Error deleting data:" << query.lastError().text();
        statusBar->showMessage(query.lastError().text());
    }
}

void Tasks::moveToTasks(QListWidgetItem *item) {
    QString task = item->text();

    if (!query.exec("INSERT INTO tasks (task) VALUES('" + task + "');")) {
        qDebug() << query.lastError();
    } else {
        QListWidgetItem *item = doneList->takeItem(doneList->currentRow());
        taskList->addItem(item);
        qDebug() << task << " was moved";
        statusBar->showMessage(task + " was moved");
    }

    QString done = item->text();

    query.prepare("DELETE FROM donetasks WHERE task = '" + done + "'");

    if (query.exec()) {
        qDebug() << done << " was moved";
        statusBar->showMessage(done + " was moved");
    } else {
        qDebug() << "Error deleting data:" << query.lastError().text();
        statusBar->showMessage(query.lastError().text());
    }
}

void Tasks::removeTask() {
    QListWidgetItem *item = taskList->takeItem(taskList->currentRow());

    QString task = item->text();

    query.prepare("DELETE FROM tasks WHERE task = '" + task + "'");

    if (query.exec()) {
        qDebug() << task << " was deleted";
        statusBar->showMessage(task + " was deleted");
    } else {
        qDebug() << "Error deleting data:" << query.lastError().text();
        statusBar->showMessage(query.lastError().text());
    }
}

void Tasks::removeDone() {
    QListWidgetItem *item = doneList->takeItem(doneList->currentRow());

    QString task = item->text();

    query.prepare("DELETE FROM donetasks WHERE task = '" + task + "'");

    if (query.exec()) {
        qDebug() << task << " was deleted";
        statusBar->showMessage(task + " was deleted");
    } else {
        qDebug() << "Error deleting data:" << query.lastError().text();
        statusBar->showMessage(query.lastError().text());
    }
}

void Tasks::toMainWindow() {
    QSettings settings("Tasks", "Tasks");
    settings.setValue("geometry", saveGeometry());

    close();
    MainWindow *mainWindow = new MainWindow(this);
    mainWindow->setWindowIcon(QIcon("://home.svg"));
    mainWindow->show();
}
