#include "notes.h"

#include <QDateTime>
#include <QDebug>
#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTextEdit>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_notes.h"

Notes::Notes(QMainWindow *parent) : QMainWindow(parent) {
    QSettings settings("Notes", "Notes");
    restoreGeometry(settings.value("geometry").toByteArray());

    QSqlQuery query;

    QString str =
        "CREATE TABLE notes ( "
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name VARCHAR(100),"
        "text VARCHAR(999999999),"
        "createdTime VARCHAR(50)"
        ");";

    QString str2 =
        "CREATE TABLE notes_settings ( "
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "isListHide INTEGER, "
        "isFullScreen INTEGER, "
        "lastNote VARCHAR(200), "
        "isDarkMode INTEGER"
        ");";

    if (!query.exec(str)) {
        qDebug() << query.lastError();
    } else {
        qDebug() << "Таблица notes создана";
    }

    setMinimumSize(300, 350);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);
    listLayout = new QHBoxLayout;
    notesListItems = new QVBoxLayout;
    noteLayout = new QVBoxLayout;
    buttonsLayout = new QHBoxLayout;
    noteNameLayout = new QHBoxLayout;
    windowControl = new QHBoxLayout;

    mainLayout->setSpacing(10);

    statusBar = new QStatusBar(this);

    noteName = new QLineEdit(this);
    noteName->setMinimumSize(50, 40);
    noteName->setFont(QFont("SF Pro Black", 14));
    noteName->setPlaceholderText("Title");

    noteEdit = new QTextEdit(this);
    noteEdit->setMinimumSize(50, 50);
    noteEdit->setFont(QFont("SF Pro Black", 11));
    noteEdit->setPlaceholderText("Note");

    notesList = new QListWidget(this);
    notesList->setMinimumSize(50, 20);  // 560);
    notesList->setMaximumSize(215, notesList->maximumHeight());
    notesList->setFont(QFont("SF Pro Black", 12));
    connect(notesList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this,
            SLOT(doubleClick(QListWidgetItem *)));

    saveButton = new QPushButton("SAVE", this);
    saveButton->setMinimumSize(50, 30);
    saveButton->setFont(QFont("SF Pro Black", 10));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveNotes()));

    backButton = new QPushButton("BACK", this);
    backButton->setMinimumSize(50, 30);
    backButton->setFont(QFont("SF Pro Black", 10));
    connect(backButton, SIGNAL(clicked()), this, SLOT(toMainWindow()));

    removeButton = new QPushButton("REMOVE", this);
    removeButton->setMinimumSize(50, 30);
    removeButton->setFont(QFont("SF Pro Black", 10));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeNote()));

    hideNotesList = new QPushButton("HIDE", this);
    hideNotesList->setMaximumSize(101, 30);
    hideNotesList->setFont(QFont("SF Pro Black", 10));
    connect(hideNotesList, SIGNAL(clicked()), this, SLOT(clickHideList()));

    fullScreen = new QPushButton("MAXIMIZE", this);
    fullScreen->setMaximumSize(101, 30);
    fullScreen->setFont(QFont("SF Pro Black", 10));
    connect(fullScreen, SIGNAL(clicked()), this, SLOT(openFullScreenWindow()));

    setStatusBar(statusBar);

    windowControl->addWidget(hideNotesList);
    windowControl->addWidget(fullScreen);

    //  noteNameLayout->addLayout(windowControl);
    noteNameLayout->addWidget(noteName);

    // notesListItems->addLayout(windowControl);
    notesListItems->addWidget(notesList);

    noteLayout->addWidget(noteName);
    noteLayout->addWidget(noteEdit);

    listLayout->addLayout(notesListItems);
    listLayout->addLayout(noteLayout);

    buttonsLayout->addWidget(hideNotesList);
    buttonsLayout->addWidget(fullScreen);
    buttonsLayout->addWidget(backButton);
    buttonsLayout->addWidget(removeButton);
    buttonsLayout->addWidget(saveButton);

    mainLayout->addLayout(listLayout);
    mainLayout->addLayout(buttonsLayout);

    query.exec("SELECT * FROM notes");
    while (query.next()) {
        QString name = query.value("name").toString();
        QString createdTime = query.value("createdTime").toString();

        QString itemText = name + "\n" + createdTime + "\n";

        QListWidgetItem *item = new QListWidgetItem(itemText);
        notesList->addItem(item);

        qDebug() << name << " was loaded";
        statusBar->showMessage("All notes was loaded");
    }
}

Notes::~Notes() {
    QSettings settings("Notes", "Notes");
    settings.setValue("geometry", saveGeometry());

    delete ui;
}

void Notes::toMainWindow() {
    QString text = noteEdit->toPlainText();
    if (text.isEmpty()) {
        QSettings settings("Notes", "Notes");
        settings.setValue("geometry", saveGeometry());

        close();
        MainWindow *mainWindow = new MainWindow(this);
        mainWindow->setWindowIcon(QIcon("://home.svg"));
        mainWindow->show();
    } else {
        qDebug() << "Save the note first.";
        statusBar->showMessage("Save the note first.");
    }
}

void Notes::saveNotes() {
    QString createdTime =
        QDateTime::currentDateTime().toString("hh:mm dd.MM.yyyy");

    QSqlQuery query;

    QString text = noteEdit->toPlainText();
    QString name = noteName->text();

    QString itemText = name + "\n" + createdTime + "\n";

    if (!text.isEmpty()) {
        if (query.exec("SELECT * FROM library WHERE '" + name +
                       "' LIKE name")) {
            if (query.exec("UPDATE notes SET name = '" + name + "' text='" +
                           text + "' ")) {
                qDebug() << name << " was updated";
                statusBar->showMessage(name + " was updated");

                noteEdit->clear();
                noteName->clear();
            } else {
                qDebug() << query.lastError();
                statusBar->showMessage(query.lastError().text());
            }
        } else {
            if (query.exec(
                    "INSERT INTO notes (name, text, createdTime) VALUES('" +
                    name + "', '" + text + "', '" + createdTime + "');")) {
                qDebug() << name << " was saved";
                statusBar->showMessage(name + " was saved");

                QListWidgetItem *item = new QListWidgetItem(itemText);
                notesList->addItem(item);

                noteEdit->clear();
                noteName->clear();
            } else {
                qDebug() << query.lastError();
                statusBar->showMessage(query.lastError().text());
            }
        }
    } else {
        qDebug() << "Note is empty";
        statusBar->showMessage("Notes is empty");
    }
}

void Notes::doubleClick(QListWidgetItem *item) {
    QString text = noteEdit->toPlainText();
    if (text.isEmpty()) {
        QSqlQuery query;

        QListWidgetItem *selectedItem = notesList->currentItem();
        QString itemText = selectedItem->text();

        QStringList text = itemText.split('\n');

        QString name = text[0];

        query.exec("SELECT * FROM notes WHERE name = '" + name + "'");

        if (query.exec()) {
            while (query.next()) {
                QString name = query.value("name").toString();
                QString text = query.value("text").toString();

                noteName->setText(name);
                noteEdit->setText(text);

                qDebug() << name << " was opened";
                statusBar->showMessage(name + " was opened");
            }
        } else {
            qDebug() << "Error moving data: " << query.lastError().text();
            statusBar->showMessage(query.lastError().text());
        }

        query.prepare("DELETE FROM notes WHERE name = '" + name + "'");

        if (query.exec()) {
            delete notesList->takeItem(notesList->row(item));
            qDebug() << name << " was opened";
            statusBar->showMessage(name + " was opened");
        } else {
            qDebug() << "Error moving data: " << query.lastError().text();
            statusBar->showMessage(query.lastError().text());
        }
    } else {
        qDebug() << "Previous changes were not saved. save them and try again.";
        statusBar->showMessage(
            "Previous changes were not saved. save them and try again.");
    }
}

void Notes::removeNote() {
    QSqlQuery query;

    QListWidgetItem *selectedItem = notesList->currentItem();

    QString itemText = selectedItem->text();

    QStringList text = itemText.split('\n');

    QString name = text[0];

    query.prepare("DELETE FROM notes WHERE name = '" + name + "'");

    if (query.exec()) {
        delete selectedItem;
        qDebug() << name << " was deleted";
        statusBar->showMessage(name + " was deleted");
    } else {
        qDebug() << "Error deleting data:" << query.lastError().text();
        statusBar->showMessage(query.lastError().text());
    }
}

// 0 - false
// 1 - true

void Notes::clickHideList() {
    QSqlQuery query;

    if (isListHide == 0) {
        notesList->hide();
        isListHide = 1;
        query.exec("UPDATE notes_settings SET isListHide = 1 ");

        hideNotesList->setText("SHOW");
    } else {
        notesList->show();
        isListHide = 0;
        query.exec("UPDATE notes_settings SET isListHide = 0 ");

        hideNotesList->setText("HIDE");
    }
}

void Notes::openFullScreenWindow() {
    QSqlQuery query;

    if (isFullScreen == 0) {
        Notes::showFullScreen();
        isFullScreen = 1;
        query.exec("UPDATE notes_settings SET isFullScreen = 1 ");

        fullScreen->setText("MINIMIZE");
    } else {
        Notes::showNormal();
        isFullScreen = 0;
        query.exec("UPDATE notes_settings SET isFullScreen = 0 ");

        fullScreen->setText("MAXIMIZE");
    }
}
