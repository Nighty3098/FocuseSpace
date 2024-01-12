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

Tasks::Tasks(QMainWindow *parent) : QMainWindow(parent) {
    settings = new QSettings("Tasks", "Tasks");
    restoreGeometry(settings->value("geometry").toByteArray());

    GlobalSettings = new QSettings("Settings", "Settings");
    QFont selectedFont = GlobalSettings->value("font").value<QFont>();

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
    statusBar->setFont(QFont(selectedFont));
    statusBar->setStyleSheet("color: #FFFFFF; font-size:  9px;");

    taskList = new QListWidget(this);
    taskList->setStyleSheet("border-color: #dd4853; font-size: 10px;");
    taskList->setMinimumSize(50, 50);
    taskList->setFont(QFont(selectedFont));


    doneList = new QListWidget(this);
    doneList->setStyleSheet("border-color: #74db6c; font-size: 10px;");
    doneList->setMinimumSize(50, 50);
    doneList->setFont(QFont(selectedFont));


    taskInput = new QLineEdit(this);
    taskInput->setFont(QFont(selectedFont));
    taskInput->setMinimumSize(50, 30);
    taskInput->setAlignment(Qt::AlignCenter);
    taskInput->setPlaceholderText("Task");
    taskInput->setStyleSheet("color: #FFFFFF; font-size:  10px;");


    clearTask = new QPushButton("REMOVE", this);
    clearTask->setFont(QFont(selectedFont));
    clearTask->setMinimumSize(40, 30);
    clearTask->setStyleSheet("color: #FFFFFF; font-size:  10px;");
    
    backButton = new QPushButton("BACK", this);
    backButton->setFont(QFont(selectedFont));
    backButton->setMinimumSize(40, 30);
    backButton->setStyleSheet("color: #FFFFFF; font-size:  10px;");
    
    removeButton = new QPushButton("REMOVE", this);
    removeButton->setFont(QFont(selectedFont));
    removeButton->setMinimumSize(40, 30);
    removeButton->setStyleSheet("color: #FFFFFF; font-size:  10px;");
    
    saveButton = new QPushButton("ADD TASK", this);
    saveButton->setFont(QFont(selectedFont));
    saveButton->setMinimumSize(100, 30);
    saveButton->setStyleSheet("color: #FFFFFF; font-size:  10px;");
    

    connect(clearTask, SIGNAL(clicked()), this, SLOT(removeTask()));
    connect(backButton, SIGNAL(clicked()), this, SLOT(toMainWindow()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeDone()));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(addTask()));

    connect(taskInput, &QLineEdit::returnPressed, [=] {
        QString title = taskInput->text();

        if (title.isEmpty()) {
            qDebug() << "Task is empty";
            statusBar->showMessage("Task is empty");
        } else {
            addTask();
        }
    });

    connect(doneList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this,
            SLOT(moveToTasks(QListWidgetItem *)));

    connect(taskList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this,
            SLOT(moveToDone(QListWidgetItem *)));



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
    settings = new QSettings("Tasks", "Tasks");
    settings->setValue("geometry", saveGeometry());
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
    settings = new QSettings("Tasks", "Tasks");
    settings->setValue("geometry", saveGeometry());

    close();
    //MainWindow *mainWindow = new MainWindow(this);
    //mainWindow->setWindowIcon(QIcon("://home.svg"));
    //mainWindow->show();
}
