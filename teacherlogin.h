#ifndef TEACHERLOGIN_H
#define TEACHERLOGIN_H

#include <QWidget>
#include "teacher.h"
#include"dboperator.h"

namespace Ui {
class TeacherLogin;
}

class TeacherLogin : public QWidget
{
    Q_OBJECT

public:
    explicit TeacherLogin(QWidget *parent = nullptr);
    ~TeacherLogin();
    void BackFromTeacher();

signals:
    void BackFromTea();
    void TeacherLoginExit();

private slots:
    void on_Teacher_login_clicked();

    void on_Teacher_exit_clicked();

private:
    Ui::TeacherLogin *ui;
    Teacher Teacher;
    DBOperator dbp;
};

#endif // TEACHERLOGIN_H
