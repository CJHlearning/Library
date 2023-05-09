#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QMainWindow>
#include"dboperator.h"
#include "qevent.h"
#include<QCloseEvent>

namespace Ui {
class ChangePassword;
}

class ChangePassword : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChangePassword(QWidget *parent = nullptr);
    ~ChangePassword();
    void GetAccout(QString Account);
    void closeEvent(QCloseEvent* e);

private slots:
    void on_returnButton_clicked();

    void on_confirmButton_clicked();

private:
    Ui::ChangePassword *ui;
    DBOperator dbp;
    QString account;
};

#endif // CHANGEPASSWORD_H
