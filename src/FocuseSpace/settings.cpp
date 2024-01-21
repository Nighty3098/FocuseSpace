#include "settings.h"
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
#include <QPushButton>
#include <QFontComboBox>
#include <QFont>

Settings::Settings(QMainWindow *parent) : QMainWindow(parent) {
    setWindowTitle(" ~ settings ~ ");
    setMinimumSize(300, 300);

    GlobalSettings = new QSettings("Settings", "Settings");
    //restoreGeometry(AppSettings->value("geometry").toByteArray());

    QFont selectedFont = GlobalSettings->value("font").value<QFont>();

    statusBar = new QStatusBar(this);
    statusBar->setFont(QFont(selectedFont));
    statusBar->setStyleSheet("color: #FFFFFF; font-size:  9px;");

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    title = new QLabel("Settings", this);
    title->setAlignment(Qt::AlignCenter);
    title->setFont(QFont(selectedFont));
    title->setStyleSheet("color: #FFFFFF; font-size:  20px;");

    fontChanger = new QFontComboBox(this);
    fontChanger->setMinimumSize(200, 30);
    fontChanger->setFont(QFont(selectedFont));
    fontChanger->setStyleSheet("color: #FFFFFF; font-size:  10px;");

    changeThemeButton = new QPushButton("Change theme", this);
    changeThemeButton->setMinimumSize(200, 30);
    changeThemeButton->setFont(QFont(selectedFont));
    changeThemeButton->setStyleSheet("color: #FFFFFF; font-size:  10px;");

    applyButton = new QPushButton("Apply", this);
    applyButton->setMinimumSize(200, 30);
    applyButton->setFont(QFont(selectedFont));
    applyButton->setStyleSheet("color: #FFFFFF; font-size:  10px;");

    fontChanger->setCurrentFont(selectedFont);


    connect(applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));

    mainLayout->addWidget(title);
    mainLayout->addWidget(fontChanger);
    mainLayout->addWidget(changeThemeButton);
    mainLayout->addWidget(applyButton);

    setStatusBar(statusBar);
    statusBar->showMessage("OK");
}

Settings::~Settings() {
    GlobalSettings = new QSettings("Settings", "Settings");
    //AppSettings->setValue("geometry", saveGeometry());
    QFont selectedFont = fontChanger->currentFont();
    GlobalSettings->setValue("font", selectedFont);

    delete ui;
}

void Settings::changeTheme() {}

void Settings::applyChanges() {
    GlobalSettings = new QSettings("Settings", "Settings");
    QFont selectedFont = fontChanger->currentFont();
    GlobalSettings->setValue("font", selectedFont);

    changeThemeButton->setFont(selectedFont);
    applyButton->setFont(selectedFont);
    fontChanger->setFont(selectedFont);

    statusBar->showMessage("Changes applied. Please, restart app");
}
