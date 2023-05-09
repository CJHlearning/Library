#ifndef STUDENT_H
#define STUDENT_H

#include "ensure.h"
#include <QMainWindow>
#include"dboperator.h"
#include"changepassword.h"

namespace Ui {
class Student;
}

class Student : public QMainWindow
{
    Q_OBJECT

public:
    explicit Student(QWidget *parent = nullptr);
    ~Student();
    void setAccount(QString account);
private slots:

    void on_AccountSettings_clicked();

    void on_BookSearch_clicked();

    void on_LendList_clicked();

    void on_Exit_clicked();

    void on_SearchButton_clicked();

    void on_EditButton_clicked();

    void on_SaveButton_clicked();

    void timeUpdate();

    void buttonUpdate();

    void on_changeButton_clicked();

private:
    Ui::Student *ui;
    Ensure ensure;
    DBOperator dbp;
    QString Account;
    ChangePassword change;
    bool AS;
    bool BS;
    bool LL;
signals:
    void Back();
};

#endif // STUDENT_H
