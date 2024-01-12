#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStatusBar>
#include <QPalette>
#include <QStyle>
#include <QKeyEvent>

#include "notes.h"
#include "pomodoro.h"
#include "tasks.h"
#include "settings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class QMainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow *parent = nullptr);
    ~MainWindow();

    QSettings *settings;
    QSettings *GlobalSettings;

protected:
    void keyPressEvent(QKeyEvent *e) override;


private slots:
    void openTasksWin();
    void openNotesWin();
    void openPomodoroWin();
    void openSettingsWin();


private:
    QStatusBar *statusBar;
    QLabel *titleMain;
    QPushButton *openTasks;
    QPushButton *openNotes;
    QPushButton *openTimer;
    QPushButton *openSettings;
    QLabel *imageLabel;

    Tasks *tasks_window;
    Notes *notes_window;
    Pomodoro *pomodoro_window;
    Settings *settings_window;

};
#endif // MAINWINDOW_H
