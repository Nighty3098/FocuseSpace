#ifndef POMODORO_H
#define POMODORO_H

#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class Pomodoro; }
QT_END_NAMESPACE

class Pomodoro : public QMainWindow
{
    Q_OBJECT

public:
    explicit Pomodoro(QMainWindow *parent = nullptr);
    ~Pomodoro();

    QSettings *GlobalSettings;
    QSettings *settings;

private slots:
    void toMainWindow();
    void startPomodoro();
    void updateTimer();
    void startBreak();
    void change();

private:
    QLabel *titleLabel;
    QProgressBar *progressBar;
    QTimer *timer;
    QMessageBox *notificationwork;
    QMessageBox *notificationpause;

    int timeLeft;
    bool isPomodoro;
    bool isBreak;

    QPushButton *startButton;
    QPushButton *changeButton;
    QPushButton *backButton;


    QVBoxLayout *mainLayout;
    QVBoxLayout *infoLayout;
    QHBoxLayout *buttonsLayout_1;
    QHBoxLayout *buttonsLayout_2;
    
    Ui::Pomodoro *ui;
    Pomodoro *pomodoro;
};



#endif // POMODORO_H
