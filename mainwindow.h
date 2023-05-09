#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stulogin.h"
#include "teacherlogin.h"
#include "teacher.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void BackFromTeacherLogin();

    void BackFromStudentLogin();

private slots:
    void on_StudentBtn_clicked();

    void on_TeacherBtn_clicked();

private:
    Ui::MainWindow *ui;
    StuLogin StudentLogin;
    TeacherLogin TeacherLogin;
    Teacher Teacher;
};
#endif // MAINWINDOW_H
