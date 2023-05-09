#include "teacherlogin.h"
#include "ui_teacherlogin.h"
#include<QSqlQuery>
#include<QDebug>
#include<QMessageBox>

TeacherLogin::TeacherLogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TeacherLogin)
{
    ui->setupUi(this);
    this->setWindowTitle("登录界面");
    this->setFixedSize(600,300);
    dbp.DBopen();
    connect(&Teacher,&Teacher::Back, this, &TeacherLogin::BackFromTeacher);

    QPalette pa(this->palette());
    QImage img = QImage(":/new/load/teacherlog.jpg");
    img = img.scaled(this->size());
    QBrush *pic = new QBrush(img);

    pa.setBrush(QPalette::Window,*pic);
    //this->setAutoFillBackground(true);
    this->setPalette(pa);
}

TeacherLogin::~TeacherLogin()
{
    dbp.DBclose();
    delete ui;
}

//遍历管理员表中的账号与密码，找到是否有跟输入的账号密码相匹配的数据。
void TeacherLogin::on_Teacher_login_clicked()
{
    bool loginSuccess = false;
    QString Account = ui->AccountEdit->text();
    QString Password = ui->PasswordEdit->text();
    QString sqlSelect = QString("select * from administrator;");
    QSqlQuery query = dbp.DBGetData();
    if(!query.exec(sqlSelect))
    {
        qDebug() << "select error";
    }
    while(query.next())
    {
        QString account = query.value("administratorAccount").toString();
        QString password = query.value("password").toString();
        if(Account == account && Password == password)
        {
            loginSuccess = true;
            this->hide();
            Teacher.show();
            ui->AccountEdit->clear();
            ui->PasswordEdit->clear();
            break;
        }
    }
    if(!loginSuccess)
    {
        QMessageBox::warning(this, "警告", "用户名或密码错误!");
        ui->PasswordEdit->clear();
        return;
    }
}

void TeacherLogin::BackFromTeacher()
{
    Teacher.hide();
    emit BackFromTea();
}

void TeacherLogin::on_Teacher_exit_clicked()
{
    ui->AccountEdit->clear();
    ui->PasswordEdit->clear();//清除输入框
    emit TeacherLoginExit();
}

