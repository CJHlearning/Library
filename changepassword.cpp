#include "changepassword.h"
#include "ui_changepassword.h"
#include<QMessageBox>

ChangePassword::ChangePassword(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChangePassword)
{
    ui->setupUi(this);
    dbp.DBopen();

    QPalette pa(this->palette());
    QImage img = QImage(":/new/load/window.png");
    img = img.scaled(this->size());
    QBrush *pic = new QBrush(img);

    pa.setBrush(QPalette::Window,*pic);
    //this->setAutoFillBackground(true);
    this->setPalette(pa);
}

void ChangePassword::GetAccout(QString Account)
{
    account = Account;
}

void ChangePassword::closeEvent(QCloseEvent* e)
{
    ui->oldEdit->clear();
    ui->newEdit->clear();
    ui->new2Edit->clear();
    this->close();
}

ChangePassword::~ChangePassword()
{
    delete ui;
    dbp.DBclose();
}

void ChangePassword::on_returnButton_clicked()
{
    this->closeEvent(nullptr);
}


void ChangePassword::on_confirmButton_clicked()
{
    QString oldPassword = ui->oldEdit->text();
    QString newPassword1 = ui->newEdit->text();
    QString newPassword2 = ui->new2Edit->text();
    QSqlQuery query = dbp.DBGetData();
    QString password;
    QString sqlSelect = QString("select * from user ");
    QString sqlUpdate = QString("update user "
                                "set password = '%1' "
                                "where userAccount = '%2' ").arg(newPassword1).arg(this->account);
    query.exec(sqlSelect);
    while(query.next())
    {
        if(query.value("userAccount").toString() == this->account)
        {
            password = query.value("password").toString();
            break;
        }
    }
    if(oldPassword == "" || newPassword1 == "" || newPassword2 == "")
    {
        QMessageBox::warning(this, "警告", "密码不能为空!");
        return;
    }
    if(password == oldPassword)
    {
        if(newPassword1 == newPassword2)
        {
            if(!query.exec(sqlUpdate))
            {
                QMessageBox::warning(this,"警告","密码修改失败！");

            }
            else
            {
                QMessageBox::information(this,"恭喜","密码修改成功！");
                this->close();
            }
        }
        else
        {
            QMessageBox::warning(this,"警告","两次密码输入不相同！");

        }
    }
    else
    {
        QMessageBox::warning(this,"警告","旧密码输入错误！");
    }
    ui->oldEdit->clear();
    ui->newEdit->clear();
    ui->new2Edit->clear();
}

