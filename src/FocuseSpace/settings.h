#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStatusBar>
#include <QPalette>
#include <QStyle>
#include <QKeyEvent>
#include <QFontComboBox>

#include "notes.h"
#include "pomodoro.h"
#include "tasks.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Settings; }
QT_END_NAMESPACE

class Settings : public QMainWindow
{
    Q_OBJECT

public:
    QSettings *GlobalSettings;

    Settings(QMainWindow *parent = nullptr);
    ~Settings();

private slots:
    void changeTheme();
    void applyChanges();


private:
    QStatusBar *statusBar;

    QVBoxLayout *mainLayout;

    QLabel *title;

    QFontComboBox *fontChanger;
    
    QPushButton *changeThemeButton;
    QPushButton *applyButton;

    Ui::Settings *ui;
    Settings *settings;
};
#endif // SETTINGS_H
