#ifndef NOTES_H
#define NOTES_H

#include <QDebug>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStatusBar>
#include <QTextEdit>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class Notes;
}
QT_END_NAMESPACE

class Notes : public QMainWindow {
    Q_OBJECT

   public:
    // 0 - false
    // 1 - true
    int isListHide = 0;
    int isDarkMode;
    int isFullScreen = 0;

    // layouts
    QVBoxLayout *mainLayout;
    QHBoxLayout *listLayout;
    QVBoxLayout *notesListItems;
    QVBoxLayout *noteLayout;
    QHBoxLayout *buttonsLayout;
    QHBoxLayout *windowControl;
    QHBoxLayout *noteNameLayout;

    explicit Notes(QMainWindow *parent = nullptr);
    ~Notes();

   private slots:
    void toMainWindow();
    void saveNotes();
    void removeNote();
    void doubleClick(QListWidgetItem *);
    void clickHideList();
    void openFullScreenWindow();

   protected:
   private:
    QStatusBar *statusBar;
    QTextEdit *noteEdit;
    QListWidget *notesList;
    QLineEdit *noteName;

    QWidget *centralWidget;

    QPushButton *saveButton;
    QPushButton *backButton;
    QPushButton *removeButton;
    QPushButton *hideNotesList;
    QPushButton *fullScreen;

    Ui::Notes *ui;
    Notes *notes;
};

#endif  // WINDOW2_H
