#ifndef STULOGIN_H
#define STULOGIN_H

#include <QWidget>
#include "student.h"
#include "dboperator.h"

namespace Ui {
class StuLogin;
}

class StuLogin : public QWidget
{
    Q_OBJECT

public:
    explicit StuLogin(QWidget *parent = nullptr);
    ~StuLogin();

    void BackFromSurface();

    void ShowLoginPage();

private slots:
    void on_Return_clicked();

    void on_Login_clicked();

    void on_ReturnToPage1_clicked();

    void on_PasswordRetrival_clicked();

    void on_SignIn_clicked();

    void on_Back_clicked();

    void on_Confirm_clicked();

    void on_No_clicked();

    void on_Yes_clicked();

    void on_Emails_clicked();

    void on_PhoneNumbers_clicked();

    void on_Back_2_clicked();

    void on_Y_clicked();

    void on_N_clicked();

    void on_Y_2_clicked();

    void on_N_2_clicked();

    void on_Comfirm_clicked();

private:
    Ui::StuLogin *ui;

    Student Surface;

    DBOperator dbp;

    QString userAccount;


signals:
    void BackToMenu();
    void Exit();
    void Login();
};

#endif // STULOGIN_H
