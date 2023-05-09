#include "stulogin.h"
#include "student.h"
#include "ui_stulogin.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QTimer>

StuLogin::StuLogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StuLogin)
{
    ui->setupUi(this);
    this->setFixedSize(600,300);

    connect(&Surface, &Student::Back, this, &StuLogin::BackFromSurface);

    ui->stackedWidget->setCurrentWidget(ui->LoginPage);
    dbp.DBopen();


    QPalette pa(this->palette());
    QImage img = QImage(":/new/load/studentlog.jpg");
    img = img.scaled(this->size());
    QBrush *pic = new QBrush(img);

    pa.setBrush(QPalette::Window,*pic);
    //this->setAutoFillBackground(true);
    this->setPalette(pa);
}

StuLogin::~StuLogin()
{
    dbp.DBclose();

    delete ui;
}

void StuLogin::ShowLoginPage()
{
    ui->stackedWidget->setCurrentWidget(ui->LoginPage);
}

void StuLogin::on_Return_clicked()
{
    ui->Account->clear();
    ui->Password->clear();
    emit BackToMenu();
}

void StuLogin::on_Login_clicked()
{      
     QSqlQuery query = dbp.DBGetData();
     QString sqlSelect = QString("select * from user;");
    //在登陆时进行书籍借阅归还情况的检查与更新（对所有用户）
    if(!query.exec(sqlSelect))
    {
        QMessageBox::warning(this, "警告","select error");
        return;
    }
    QString sqlSelect1 = QString("select * from borrowlist");
    while(query.next())
    {
        QSqlQuery query1 = dbp.DBGetData();
        QString Account = query.value("userAccount").toString();
        int bookUnreturned = 0;
        if(!query1.exec(sqlSelect1))
        {
             QMessageBox::warning(this, "警告","select error");
             return;
        }
        while(query1.next())
        {//检查borrowlist中有没有当前user记录的账号，从而进行当前借阅书籍数量的更新
            bool unreturned = query1.value("unreturned").toBool();
            if(Account == query1.value("borrowerAccount").toString() && unreturned)
            {
                bookUnreturned++;
            }
        }
        QString sqlUpdate = QString("update user "
                                    "set bookUnreturned = %1 "
                                    "where userAccount = '%2' ").arg(bookUnreturned).arg(Account);
        if(!query.exec(sqlUpdate))
        {
            QMessageBox::warning(this, "警告","update error");
            return;
        }
    }


    // Check accounts and password.

    bool loginSuccess = false;
    QString Account = ui->Account->text();
    QString Password = ui->Password->text();

    if(!query.exec(sqlSelect))
    {
        QMessageBox::warning(this, "警告", "select error!");
    }
    while(query.next())
    {
        QString account = query.value("userAccount").toString();
        QString password = query.value("password").toString();
        bool activated = query.value("activated").toBool();
        bool freezed = query.value("freezed").toBool();
        int bookUnreturned = query.value("bookUnreturned").toInt();
        if(Account == account && Password == password)
        {
            if(activated)
            {
                loginSuccess = true;
                if(!freezed)
                {
                    if(bookUnreturned > 0)
                    {
                        QMessageBox::information(this, "提醒", "您有图书逾期未还\n请尽快归还!");
                    }
                    this->hide();
                    ui->Account->clear();
                    ui->Password->clear();
                    Surface.show();
                    ui->Account->clear();
                    ui->Password->clear();
                    Surface.setAccount(Account);
                }
                else
                {
                    QMessageBox::warning(this, "警告", "您有图书逾期未还\n账号已被冻结!请到图书馆前台进行解冻！");
                    return;
                }
                break;
            }
            else
            {
                QMessageBox::warning(this,"警告","账号未激活！");
                ui->Password->clear();
                ui->Account->clear();
                return;
            }
        }
    }
    if(!loginSuccess)
    {
        QMessageBox::warning(this, "警告", "用户名或密码错误!");
        ui->Password->clear();
        return;
    }

}


void StuLogin::BackFromSurface()
{
    Surface.close();
    emit Exit();
}

void StuLogin::on_ReturnToPage1_clicked()
{
    ui->NicknameEdit->clear();
    ui->PasswordEdit->clear();
    ui->stackedWidget->setCurrentWidget(ui->LoginPage);
}


void StuLogin::on_PasswordRetrival_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->PasswordRetrivalPage);
}


void StuLogin::on_SignIn_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->SignInPage);
    ui->Password->clear();
    ui->Account->clear();
}


void StuLogin::on_Back_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->PasswordRetrivalPage);
}


void StuLogin::on_Confirm_clicked()
{

    QString sA1 = ui->AL1->text();
    QString sA2 = ui->AL2->text();
    QString sA3 = ui->AL3->text();
    QSqlQuery query = dbp.DBGetData();
    QString sqlSelect = QString("select * from user;");
    if(!query.exec(sqlSelect))
    {
        QMessageBox::warning(this,"警告","select error!");
    }
    while(query.next())
    {
        QString sa1 = query.value("sA1").toString();
        QString sa2 = query.value("sA2").toString();
        QString sa3 = query.value("sA3").toString();
        QString Account = query.value("userAccount").toString();
        if(Account == this->userAccount)
        {
            if(sA1 != sa1)
            {
                QMessageBox::warning(this,"警告","答案一错误！");
                return;
            }
            else if(sA2 != sa2)
            {
                QMessageBox::warning(this,"警告","答案二错误！");
                return;
            }
            else if(sA3 != sa3)
            {
                QMessageBox::warning(this,"警告","答案三错误！");
                return;
            }
            else
            {
                ui->stackedWidget->setCurrentWidget(ui->PasswordReset);
                ui->AL1->clear();
                ui->AL2->clear();
                ui->AL3->clear();
            }
        }
    }
}


void StuLogin::on_No_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->LoginPage);
}


void StuLogin::on_Yes_clicked()
{
    QString password1 = ui->PasswordResetEdit->text();
    QString password2 = ui->PasswordConfirmEdit->text();
    QSqlQuery query = dbp.DBGetData();
    QString sqlUpdate = QString("update user "
                                "set password = '%1' "
                                "where userAccount = '%2';").arg(password1).arg(this->userAccount);
    if(password2 == "" || password1 == "")
    {
        QMessageBox::warning(this,"警告","密码输入不能为空!");
        ui->PasswordConfirmEdit->clear();
        ui->PasswordResetEdit->clear();
        return;
    }
    if(password1 != password2)
    {
        QMessageBox::warning(this,"警告","两次输入密码不相同");
        return;
    }
    else
    {
        if(!query.exec(sqlUpdate))
        {
            QMessageBox::warning(this,"警告","重置密码失败!");
            return;
        }
        else
        {
            QMessageBox::information(this,"恭喜","重置密码成功!");
            ui->stackedWidget->setCurrentWidget(ui->LoginPage);
        }
    }
}


void StuLogin::on_Emails_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->EmailPage);
}


void StuLogin::on_PhoneNumbers_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->PhoneNumberPage);
}


void StuLogin::on_Back_2_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->LoginPage);
}


void StuLogin::on_Y_clicked()
{
    QString account = ui->AccountNumEdit->text();
    QString phone = ui->PhoneNumEdit->text();
    QSqlQuery query = dbp.DBGetData();
    bool isFound = false;
    QString sqlSelect = QString("select * from user;");
    if(!query.exec(sqlSelect))
    {
        QMessageBox::warning(this,"警告","select error!");
    }
    while(query.next())
    {
        QString Phone = query.value("phone").toString();
        QString sQ1 = query.value("sQ1").toString();
        QString sQ2 = query.value("sQ2").toString();
        QString sQ3 = query.value("sQ3").toString();
        QString UserAccount = query.value("userAccount").toString();
        bool activated = query.value("activated").toBool();
        if(account == UserAccount)
        {
            isFound = true;
            if(activated)
            {
                if(Phone != "")
                {
                    if(Phone == phone)
                    {
                        userAccount = UserAccount;
                        if(sQ1 == "" && sQ2 == "" && sQ3 == "")
                        {
                            QMessageBox::warning(this,"警告","该账号未设置安全问题\n无法找回密码!");
                            return;
                        }
                        else
                        {
                            ui->SQT1->setText(sQ1);
                            ui->SQT2->setText(sQ2);
                            ui->SQT3->setText(sQ3);
                            ui->stackedWidget->setCurrentWidget(ui->SQ);
                            ui->PhoneNumEdit->clear();
                            break;
                        }
                    }
                    else
                    {
                        QMessageBox::warning(this,"警告","手机号输入错误!");
                    }
                }
                else
                {
                    QMessageBox::warning(this,"警告","该账号未绑定手机号!");
                }
            }
            else
            {
                QMessageBox::warning(this,"警告","账号未激活!");
            }
        }
    }
    if(!isFound)
    {
        QMessageBox::warning(this,"警告","不存在该账号!");
        return;
    }
    ui->AccountNumEdit->clear();
    ui->PhoneNumEdit->clear();
}


void StuLogin::on_N_clicked()
{
    ui->AccountNumEdit->clear();
    ui->PhoneNumEdit->clear();
    ui->stackedWidget->setCurrentWidget(ui->LoginPage);
    ui->AccountNumEdit->clear();
    ui->PhoneNumEdit->clear();
}


void StuLogin::on_Y_2_clicked()
{
    QString account = ui->AccountEdit->text();
    QString email = ui->EmailEdit->text();
    QSqlQuery query = dbp.DBGetData();
    bool isFound = false;
    QString sqlSelect = QString("select * from user;");
    if(!query.exec(sqlSelect))
    {
        QMessageBox::warning(this,"警告","select error!");
    }
    while(query.next())
    {
        QString Email = query.value("email").toString();
        QString sQ1 = query.value("sQ1").toString();
        QString sQ2 = query.value("sQ2").toString();
        QString sQ3 = query.value("sQ3").toString();
        QString UserAccount = query.value("userAccount").toString();
        bool activated = query.value("activated").toBool();
        if(UserAccount == account)
        {
            isFound = true;
            if(activated)
            {
                if(Email != "")
                {
                    if(Email == email)
                    {
                        userAccount = UserAccount;
                        if(sQ1 == "" && sQ2 == "" && sQ3 == "")
                        {
                            QMessageBox::warning(this,"警告","该账号未设置安全问题\n无法找回密码!");
                            return;
                        }
                        else
                        {
                            ui->SQT1->setText(sQ1);
                            ui->SQT2->setText(sQ2);
                            ui->SQT3->setText(sQ3);
                            ui->stackedWidget->setCurrentWidget(ui->SQ);
                            ui->EmailEdit->clear();
                            break;
                        }
                    }
                    else
                    {
                         QMessageBox::warning(this,"警告","邮箱输入错误!");
                    }
                }
                else
                {
                     QMessageBox::warning(this,"警告","该账号未绑定邮箱!");
                }
            }
            else
            {
                 QMessageBox::warning(this,"警告","账号未激活!");
            }
        }
    }
    if(!isFound)
    {
        QMessageBox::warning(this,"警告","不存在该账号!");
        return;
    }
    ui->AccountEdit->clear();
    ui->EmailEdit->clear();
}


void StuLogin::on_N_2_clicked()
{
    ui->EmailEdit->clear();
    ui->AccountEdit->clear();
    ui->stackedWidget->setCurrentWidget(ui->LoginPage);
    ui->AccountEdit->clear();
    ui->EmailEdit->clear();
}


void StuLogin::on_Comfirm_clicked()
{
    QString StdId = ui->NicknameEdit->text();
    QString password = ui->PasswordEdit->text();
    QSqlQuery query = dbp.DBGetData();
    bool found = false;
    bool samePassword = false;
    QString sqlselect = QString("select * from user");
    if(!query.exec(sqlselect))
    {
        QMessageBox::warning(this,"警告","select error");
    }
    else
    {
        while(query.next())
        {
            if(StdId == query.value("userAccount").toString())
            {
                found = true;
                if(password == query.value("password").toString())
                {
                    samePassword = true;
                }
            }
        }
    }
    if(found)
    {
        if(samePassword)
        {
            QString sqlUpdate = QString("update user "
                                        "set activated = %1 "
                                        "where userAccount = '%2' and password = '%3'").arg(true).arg(StdId).arg(password);
            if(!query.exec(sqlUpdate))
            {
                QMessageBox::warning(this,"警告","update error!");
            }
            else
            {
                QMessageBox::information(this,"恭喜","激活成功！");
                 ui->stackedWidget->setCurrentWidget(ui->LoginPage);
            }
        }
        else
        {
             QMessageBox::warning(this,"警告","密码输入错误\n激活失败！");
        }
    }
    else
    {
        QMessageBox::warning(this,"警告","账户不存在！");
    }

    ui->NicknameEdit->clear();
    ui->PasswordEdit->clear();
}

